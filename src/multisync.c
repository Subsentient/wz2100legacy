/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.

Warzone 2100 Legacy is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA*/
/*
 * MultiSync.c
 *
 * synching issues
 * This file handles the constant backstream of net info, checking that recvd info
 * is concurrent with the local world, and correcting as required. Magic happens here.
 *
 * All conflicts due to non-guaranteed messaging are detected/resolved here.
 *
 * Alex Lee, pumpkin Studios, bath.
 */

#include "lib/framework/frame.h"
#include "lib/framework/input.h"
#include "lib/framework/strres.h"

#include "stats.h"
#include "lib/gamelib/gtime.h"
#include "map.h"
#include "objects.h"
#include "display.h"								// for checking if droid in view.
#include "order.h"
#include "action.h"
#include "hci.h"									// for byte packing funcs.
#include "display3ddef.h"							// tile size constants.
#include "console.h"
#include "geometry.h"								// for gettilestructure
#include "mapgrid.h"								// for move droids directly.
#include "lib/netplay/netplay.h"
#include "multiplay.h"
#include "frontend.h"								// for titlemode
#include "multistat.h"
#include "power.h"									// for power checks
#include "multirecv.h"
#include "multiint.h"

// ////////////////////////////////////////////////////////////////////////////
// function definitions

static BOOL sendStructureCheck	(void);							//Structure
static void packageCheck		(const DROID *pD);
static BOOL sendDroidCheck		(void);							//droids

static void highLevelDroidUpdate(DROID *psDroid,
								 float fx,
								 float fy,
								 uint32_t state,
								 uint32_t order,
								 BASE_OBJECT *psTarget,
								 float experience);


static void onscreenUpdate		(DROID *pDroid, uint32_t dam,		// the droid and its damage
								 uint16_t dir,					// direction it should facing
								 DROID_ORDER order);			// what it should be doing

static void offscreenUpdate		(DROID *pDroid, uint32_t dam,
								 float fx, float fy,
								 uint16_t dir,
								 DROID_ORDER order);

static BOOL sendPowerCheck(void);
static uint32_t averagePing(void);

// ////////////////////////////////////////////////////////////////////////////
// Defined numeric values
// NOTE / FIXME: Current MP games are locked at 45ms
#define MP_FPS_LOCK			45
#define AV_PING_FREQUENCY	MP_FPS_LOCK * 400		// how often to update average pingtimes (host only, when players join)
#define PING_FREQUENCY		MP_FPS_LOCK * 400		// how often to update pingtimes. (About every ~1.8 secs)
#define STRUCT_FREQUENCY	MP_FPS_LOCK * 10		// how often to send a structure check. (About every 450ms)
#define DROID_FREQUENCY		MP_FPS_LOCK * 7			// how often to send droid checks.(About every 315ms)
#define POWER_FREQUENCY		MP_FPS_LOCK * 14		// how often to send power levels (About every 630ms)
#define SCORE_FREQUENCY		MP_FPS_LOCK * 100		// how often to update global score. (About every ~4.5 secs)

static uint32_t				PingSend[MAX_PLAYERS];	//stores the time the ping was called.

// ////////////////////////////////////////////////////////////////////////////
// test traffic level.
static BOOL okToSend(void)
{
	// Update checks and go no further if any exceeded.
	// removing the received check again ... add NETgetRecentBytesRecvd() to left hand side of equation if this works badly
	if (NETgetRecentBytesSent() >= MAX_BYTESPERSEC)
	{
		return false;
	}

	return true;
}

// ////////////////////////////////////////////////////////////////////////////
// Droid checking info. keep position and damage in sync.
BOOL sendCheck(void)
{
	uint32_t i;

	NETgetBytesSent();			// update stats.
	NETgetBytesRecvd();
	NETgetPacketsSent();
	NETgetPacketsRecvd();

	// dont send checks till all players are present.
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		if(isHumanPlayer(i) && ingame.JoiningInProgress[i])
		{
			return true;
		}
	}

	// send Checks. note each send has it's own send criteria, so might not send anything.
	// Priority is droids -> structures -> power -> score -> ping

	if(okToSend())
	{
		sendDroidCheck();
		sync_counter.sentDroidCheck++;
	}
	else
	{
		sync_counter.unsentDroidCheck++;
	}
	if(okToSend())
	{
		sendStructureCheck();
		sync_counter.sentStructureCheck++;

	}
	else
	{
		sync_counter.unsentStructureCheck++;
	}
	if(okToSend())
	{
		sendPowerCheck();
		sync_counter.sentPowerCheck++;
	}
	else
	{
		sync_counter.unsentPowerCheck++;
	}
	if(okToSend())
	{
		sendScoreCheck();
		sync_counter.sentScoreCheck++;
	}
	else
	{
		sync_counter.unsentScoreCheck++;
	}
	if(okToSend())
	{
		sendPing();
		sync_counter.sentPing++;
	}
	else
	{
		sync_counter.unsentPing++;
	}

	if (isMPDirtyBit)
	{
		sync_counter.sentisMPDirtyBit++;
	}
	else
	{
		sync_counter.unsentisMPDirtyBit++;
	}
	// FIXME: reset flag--For now, we always do this since we have no way of knowing which routine(s) we had to set this flag
	isMPDirtyBit = false;
	return true;
}

// ////////////////////////////////////////////////////////////////////////////
// pick a droid to send, NULL otherwise.
static DROID *pickADroid(void)
{
	DROID *pD = NULL;						// current droid we're checking
	unsigned int i;
	static uint32_t	droidnum = 0;						// how far down the playerlist to go.
	static uint32_t	player = 0;						// current player we're checking
	static uint32_t	maxtrys = 0;

	// Don't send stuff that isn't our problem
	while (!myResponsibility(player))
	{
		player = (player + 1) % MAX_PLAYERS;
		droidnum = 0;

		// Bail out if we've tried for each available player already
		if (++maxtrys >= MAX_PLAYERS)
		{
			maxtrys = 0;
			return NULL;
		}
	}

	// Find the 'droidnum'th droid for the current player
	for (i = 0, pD = apsDroidLists[player];
			i < droidnum && pD != NULL;
			++i, pD = pD->psNext)
	{}

	// If we've already dealt with the last droid for this player
	if (pD == NULL) // droid is no longer there or list end.
	{
		// Deal with the next player now
		player = (player + 1) % MAX_PLAYERS;
		droidnum = 0;

		// Bail out if we've tried for each available player already
		if (++maxtrys >= MAX_PLAYERS)
		{
			maxtrys = 0;
			return NULL;
		}

		// Invoke ourselves to pick a droid from the next player
		return pickADroid();
	}

	++droidnum;
	maxtrys = 0;

	return pD;
}

/** Force a droid to be synced
 *
 *  Call this when you need to update the given droid right now.
 */
BOOL ForceDroidSync(const DROID *droidToSend)
{
	uint8_t count = 1;		// *always* one

	ASSERT(droidToSend != NULL, "NULL pointer passed");

	debug(LOG_SYNC, "Force sync of droid %u from player %u", droidToSend->id, droidToSend->player);

	NETbeginEncode(NET_CHECK_DROID, NET_ALL_PLAYERS);
	NETuint8_t(&count);
	packageCheck(droidToSend);
	return NETend();
}

// ///////////////////////////////////////////////////////////////////////////
// send a droid info packet.
static BOOL sendDroidCheck(void)
{
	DROID			*pD, **ppD;
	uint8_t			i, count;
	static uint32_t	lastSent = 0;		// Last time a struct was sent.
	uint32_t			toSend = 6;

	if (lastSent > gameTime)
	{
		lastSent = 0;
	}

	// Only send a struct send if not done recently or if isMPDirtyBit is set
	if (gameTime - lastSent < DROID_FREQUENCY && !isMPDirtyBit)
	{
		return true;
	}

	lastSent = gameTime;

	NETbeginEncode(NET_CHECK_DROID, NET_ALL_PLAYERS);

	// Allocate space for the list of droids to send
	ppD = alloca(sizeof(DROID *) * toSend);

	// Get the list of droids to sent
	for (i = 0, count = 0; i < toSend; i++)
	{
		pD = pickADroid();

		// If the droid is valid add it to the list
		if (pD)
		{
			ppD[count++] = pD;
		}
		// All droids are synced! (We're done.)
		else
		{
			break;
		}
	}

	// Send the number of droids to expect
	NETuint8_t(&count);

	// Add the droids to the packet
	for (i = 0; i < count; i++)
	{
		packageCheck(ppD[i]);
	}

	return NETend();
}

// ////////////////////////////////////////////////////////////////////////////
// Send a Single Droid Check message
static void packageCheck(const DROID *pD)
{
	// Copy these variables so that we don't have to violate pD's constness
	uint8_t player = pD->player;
	uint32_t droidID = pD->id;
	int32_t order = pD->order;
	uint32_t secondaryOrder = pD->secondaryOrder;
	uint32_t body = pD->body;
	float direction = pD->direction;
	float experience = pD->experience;
	float sMoveX = pD->sMove.fx;
	float sMoveY = pD->sMove.fy;

	// Send the player to which the droid belongs
	NETuint8_t(&player);

	// Now the droid's ID
	NETuint32_t(&droidID);

	// The droid's order
	NETint32_t(&order);

	// The droids secondary order
	NETuint32_t(&secondaryOrder);

	// Droid's current HP
	NETuint32_t(&body);

	// Direction it is going in
	NETfloat(&direction);

	NETfloat(&sMoveX);
	NETfloat(&sMoveY);

	if (pD->order == DORDER_ATTACK)
	{
		uint32_t targetID = pD->psTarget->id;

		NETuint32_t(&targetID);
	}
	else if (pD->order == DORDER_MOVE)
	{
		uint16_t orderX = pD->orderX;
		uint16_t orderY = pD->orderY;

		NETuint16_t(&orderX);
		NETuint16_t(&orderY);
	}

	// Last send the droid's experience
	NETfloat(&experience);
}


// ////////////////////////////////////////////////////////////////////////////
// receive a check and update the local world state accordingly
BOOL recvDroidCheck()
{
	uint8_t		count;
	int		i;

	NETbeginDecode(NET_CHECK_DROID);

	// Get the number of droids to expect
	NETuint8_t(&count);

	for (i = 0; i < count; i++)
	{
		DROID		*pD;
		BASE_OBJECT	*psTarget = NULL;
		float		fx = 0, fy = 0;
		DROID_ORDER	order = 0;
		BOOL		onscreen;
		uint8_t		player;
		float		direction, experience;
		uint16_t	tx, ty;
		uint32_t	ref, body, target = 0, secondaryOrder;

		// Fetch the player
		NETuint8_t(&player);

		// Fetch the droid being checked
		NETuint32_t(&ref);

		// The droid's order
		NETenum(&order);

		// Secondary order
		NETuint32_t(&secondaryOrder);

		// HP
		NETuint32_t(&body);

		// Direction
		NETfloat(&direction);

		// Fractional move
		NETfloat(&fx);
		NETfloat(&fy);

		// Find out what the droid is aiming at
		if (order == DORDER_ATTACK)
		{
			NETuint32_t(&target);
		}
		// Else if the droid is moving where to
		else if (order == DORDER_MOVE)
		{
			NETuint16_t(&tx);
			NETuint16_t(&ty);
		}

		// Get the droid's experience
		NETfloat(&experience);

		/*
		 * Post processing
		 */

		// Find the droid in question
		if (!IdToDroid(ref, player, &pD))
		{
			NETlogEntry("Recvd Unknown droid info. val=player", SYNC_FLAG, player);
			debug(LOG_SYNC, "Received checking info for an unknown (as yet) droid. player:%d ref:%d", player, ref);
			continue;
		}

		// If there is a target find it
		if (target)
		{
			psTarget = IdToPointer(target, ANYPLAYER);
		}

		/*
		 * Decide how best to sync the droid. If it is onscreen and visible
		 * and the player who owns the droid has a low ping then do an
		 * onscreen update, otherwise do an offscreen one.
		 */
		if (droidOnScreen(pD, 0)
				&& ingame.PingTimes[player] < PING_LIMIT)
		{
			onscreen = true;
		}
		else
		{
			onscreen = false;
		}

		// Update the droid
		if (onscreen || isVtolDroid(pD))
		{
			onscreenUpdate(pD, body, direction, order);
		}
		else
		{
			offscreenUpdate(pD, body, fx, fy, direction, order);
		}

//			debug(LOG_SYNC, "difference in position for droid %d; was (%g, %g); did %s update", (int)pD->id,
//			      fx - pD->sMove.fx, fy - pD->sMove.fy, onscreen ? "onscreen" : "offscreen");

		// Update the higher level stuff
		if (!isVtolDroid(pD))
		{
			highLevelDroidUpdate(pD, fx, fy, secondaryOrder, order, psTarget, experience);
		}

		// ...and repeat!
	}

	NETend();

	return true;
}

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// higher order droid updating. Works mainly at the order level. comes after the main sync.
static void highLevelDroidUpdate(DROID *psDroid, float fx, float fy,
								 uint32_t state, uint32_t order,
								 BASE_OBJECT *psTarget, float experience)
{
	// update kill rating.
	psDroid->experience = experience;

	// remote droid is attacking, not here tho!
	if(order == DORDER_ATTACK && psDroid->order != DORDER_ATTACK && psTarget)
	{
		turnOffMultiMsg(true);
		orderDroidObj(psDroid, DORDER_ATTACK, psTarget);
		turnOffMultiMsg(false);
	}

	// secondary orders.
	if(psDroid->secondaryOrder != state)
	{
		psDroid->secondaryOrder = state;
	}

	// see how well the sync worked, optionally update.
	// offscreen updates will make this ok each time.
	if (psDroid->order == DORDER_NONE && order == DORDER_NONE)
	{
		if(  (fabs(fx - psDroid->sMove.fx) > (TILE_UNITS * 2))		// if more than 2 tiles wrong.
				|| (fabs(fy - psDroid->sMove.fy) > (TILE_UNITS * 2)) )
		{
			turnOffMultiMsg(true);
			debug(LOG_SYNC, "Move order from %d,%d to %d,%d", (int)psDroid->pos.x, (int)psDroid->pos.y, (int)fx, (int)fy);
			orderDroidLoc(psDroid, DORDER_MOVE, fx, fy);
			turnOffMultiMsg(false);
		}
	}
}

// ////////////////////////////////////////////////////////////////////////////
// droid on screen needs modifying
static void onscreenUpdate(DROID *psDroid,
						   uint32_t dam,
						   uint16_t dir,
						   DROID_ORDER order)
{
	BASE_OBJECT *psClickedOn;
	BOOL		bMouseOver = false;

	psClickedOn = mouseTarget();
	if( psClickedOn != NULL && psClickedOn->type == OBJ_DROID)
	{
		if(psClickedOn->id == psDroid->id && mouseDown(MOUSE_RMB))
		{
			bMouseOver = true;						// override, so you dont see the updates.
		}
	}

	if(!bMouseOver)
	{
		psDroid->body = dam;						// update damage
	}

//	if(psDroid->order == DORDER_NONE || (psDroid->order == DORDER_GUARD && psDroid->action == DACTION_NONE) )
//	{
//		psDroid->direction	 = dir  %360;				//update rotation
//	}

	return;
}

// ////////////////////////////////////////////////////////////////////////////
// droid offscreen needs modyfying.
static void offscreenUpdate(DROID *psDroid,
							uint32_t dam,
							float fx,
							float fy,
							uint16_t dir,
							DROID_ORDER order)
{
	PROPULSION_STATS	*psPropStats;
	int			oldX, oldY;

	if (fabs((float)psDroid->pos.x - fx) > TILE_UNITS || fabs((float)psDroid->pos.y - fy) > TILE_UNITS)
	{
		debug(LOG_SYNC, "Moving droid %d from (%u,%u) to (%u,%u) (has order %s)",
			  (int)psDroid->id, psDroid->pos.x, psDroid->pos.y, (uint32_t)fx, (uint32_t)fy, getDroidOrderName(order));
	}

	oldX			= psDroid->pos.x;
	oldY			= psDroid->pos.y;
	psDroid->pos.x		= fx;				// update x
	psDroid->pos.y		= fy;				// update y
	psDroid->sMove.fx	= fx;
	psDroid->sMove.fy	= fy;
	psDroid->direction	= dir % 360;			// update rotation
	psDroid->body		= dam;								// update damage
	if (oldX != psDroid->pos.x || oldY != psDroid->pos.y)
	{
		gridMoveDroid(psDroid, oldX, oldY);
	}

	// stage one, update the droid's position & info, LOW LEVEL STUFF.
	if(	   order == DORDER_ATTACK
			|| order == DORDER_MOVE
			|| order ==	DORDER_RTB
			|| order == DORDER_RTR)	// move order
	{
		// reroute the droid.
		turnOffMultiMsg(true);
		moveDroidTo(psDroid, psDroid->sMove.DestinationX, psDroid->sMove.DestinationY);
		turnOffMultiMsg(false);
	}

	// stop droid if remote droid has stopped.
	if ((order == DORDER_NONE || order == DORDER_GUARD)
			&& !(psDroid->order == DORDER_NONE || psDroid->order == DORDER_GUARD))
	{
		turnOffMultiMsg(true);
		moveStopDroid(psDroid);
		turnOffMultiMsg(false);
	}

	// snap droid(if on ground)  to terrain level at x,y.
	psPropStats = asPropulsionStats + psDroid->asBits[COMP_PROPULSION].nStat;
	ASSERT( psPropStats != NULL, "offscreenUpdate: invalid propulsion stats pointer" );
	if(	psPropStats->propulsionType != PROPULSION_TYPE_LIFT )		// if not airborne.
	{
		psDroid->pos.z = map_Height(psDroid->pos.x, psDroid->pos.y);
	}
	return;
}


// ////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////
// Structure Checking, to ensure smoke and stuff is consistent across machines.
// this func is recursive!
static  STRUCTURE *pickAStructure(void)
{
	static uint32_t	player = 0;					// player currently checking.
	static uint32_t	snum = 0;						// structure index for this player.
	STRUCTURE		*pS = NULL;
	static uint32_t	maxtrys = 0;				// don't loop forever if failing/.
	uint32_t			i;

	if ( !myResponsibility(player) )			// dont send stuff that's not our problem.
	{
		player ++;								// next player next time.
		player = player % MAX_PLAYERS;
		snum = 0;

		if(maxtrys < MAX_PLAYERS)
		{
			maxtrys ++;
			return pickAStructure();
		}
		else
		{
			maxtrys = 0;
			return NULL;
		}
	}

	pS = apsStructLists[player];				// find the strucutre
	for(i = 0; ((i < snum) && (pS != NULL)) ; i++)
	{
		pS = pS->psNext;
	}


	if (pS == NULL)								// last structure or no structures at all
	{
		player ++;								// go onto the next player
		player = player % MAX_PLAYERS;
		snum = 0;

		if(maxtrys < MAX_PLAYERS)
		{
			maxtrys ++;
			return pickAStructure();
		}
		else
		{
			maxtrys = 0;
			return NULL;
		}
	}
	snum ++;										// next structure next time

	maxtrys = 0;
	return pS;
}

// ////////////////////////////////////////////////////////////////////////
// Send structure information.
static BOOL sendStructureCheck(void)
{
	static uint32_t	lastSent = 0;	// Last time a struct was sent
	STRUCTURE		*pS;
	uint8_t			capacity;

	if (lastSent > gameTime)
	{
		lastSent = 0;
	}
	// Only send a struct send if not done recently or if isMPDirtyBit is set
	if ((gameTime - lastSent) < STRUCT_FREQUENCY && !isMPDirtyBit)
	{
		return true;
	}

	lastSent = gameTime;

	pS = pickAStructure();
	// Only send info about complete buildings
	if (pS && (pS->status == SS_BUILT))
	{
		NETbeginEncode(NET_CHECK_STRUCT, NET_ALL_PLAYERS);
		NETuint8_t(&pS->player);
		NETuint32_t(&pS->id);
		NETuint32_t(&pS->body);
		NETuint32_t(&pS->pStructureType->ref);
		NETuint16_t(&pS->pos.x);
		NETuint16_t(&pS->pos.y);
		NETuint16_t(&pS->pos.z);
		NETfloat(&pS->direction);

		switch (pS->pStructureType->type)
		{

			case REF_RESEARCH:
				capacity = ((RESEARCH_FACILITY *) pS->pFunctionality)->capacity;
				NETuint8_t(&capacity);
				break;
			case REF_FACTORY:
			case REF_VTOL_FACTORY:
				capacity = ((FACTORY *) pS->pFunctionality)->capacity;
				NETuint8_t(&capacity);
				break;
			case REF_POWER_GEN:
				capacity = ((POWER_GEN *) pS->pFunctionality)->capacity;
				NETuint8_t(&capacity);
			default:
				break;
		}

		NETend();
	}

	return true;
}

// receive checking info about a structure and update local world state
BOOL recvStructureCheck()
{
	STRUCTURE		*pS;
	BOOL			hasCapacity = true;
	int				j;
	float			direction;
	uint8_t			player, ourCapacity;
	uint32_t		body;
	uint16_t		x, y, z;
	uint32_t		ref, type;

	NETbeginDecode(NET_CHECK_STRUCT);
	NETuint8_t(&player);
	NETuint32_t(&ref);
	NETuint32_t(&body);
	NETuint32_t(&type);
	NETuint16_t(&x);
	NETuint16_t(&y);
	NETuint16_t(&z);
	NETfloat(&direction);

	if (player >= MAX_PLAYERS)
	{
		debug(LOG_ERROR, "Bad NET_CHECK_STRUCT received!");
		NETend();
		return false;
	}

	// If the structure exists our job is easy
	if ((pS = IdToStruct(ref, player)))
	{
		pS->body = body;
		pS->direction = direction;

		// Check its finished
		if (pS->status != SS_BUILT)
		{
			pS->direction = direction;
			pS->id = ref;
			pS->status = SS_BUILT;
			buildingComplete(pS);
		}

		// If the structure has a capacity
		switch (pS->pStructureType->type)
		{
			case REF_RESEARCH:
				ourCapacity = ((RESEARCH_FACILITY *) pS->pFunctionality)->capacity;
				j = researchModuleStat;
				break;
			case REF_FACTORY:
			case REF_VTOL_FACTORY:
				ourCapacity = ((FACTORY *) pS->pFunctionality)->capacity;
				j = factoryModuleStat;
				break;
			case REF_POWER_GEN:
				ourCapacity = ((POWER_GEN *) pS->pFunctionality)->capacity;
				j = powerModuleStat;
				break;
			default:
				hasCapacity = false;
				break;
		}

		// So long as the struct has a capacity fetch it from the packet
		if (hasCapacity)
		{
			uint8_t actualCapacity = 0;

			NETuint8_t(&actualCapacity);

			// If our capacity is different upgrade ourself
			for (; ourCapacity < actualCapacity; ourCapacity++)
			{
				buildStructure(&asStructureStats[j], pS->pos.x, pS->pos.y, pS->player, false);

				// Check it is finished
				if (pS && pS->status != SS_BUILT)
				{
					pS->id = ref;
					pS->status = SS_BUILT;
					buildingComplete(pS);
				}
			}
		}
	}

	NETend();
	return true;
}


// ////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////
// Power Checking. Send a power level check every now and again.
static BOOL sendPowerCheck()
{
	static uint32_t	lastsent = 0;
	uint8_t			player = selectedPlayer;
	uint32_t		power = getPower(player);

	if (lastsent > gameTime)
	{
		lastsent = 0;
	}

	// Only send if not done recently or if isMPDirtyBit is set
	if (gameTime - lastsent < POWER_FREQUENCY && !isMPDirtyBit)
	{
		return true;
	}

	lastsent = gameTime;

	NETbeginEncode(NET_CHECK_POWER, NET_ALL_PLAYERS);
	NETuint8_t(&player);
	NETuint32_t(&power);
	return NETend();
}

BOOL recvPowerCheck()
{
	uint8_t		player;
	uint32_t	power, power2;

	NETbeginDecode(NET_CHECK_POWER);
	NETuint8_t(&player);
	NETuint32_t(&power);
	NETend();

	if (player >= MAX_PLAYERS)
	{
		debug(LOG_ERROR, "Bad NET_CHECK_POWER packet: player is %d : %s",
			  (int)player, isHumanPlayer(player) ? "Human" : "AI");
		return false;
	}

	power2 = getPower(player);
	if (power != power2)
	{
//		debug(LOG_SYNC, "NET_CHECK_POWER: Adjusting power for player %d (%s) from %u to %u",
//		      (int)player, isHumanPlayer(player) ? "Human" : "AI", power2, power);
		setPower( (uint32_t)player, power);
	}
	return true;
}
void HandleBadParam(const char *msg, const int from, const int actual)
{
	char buf[255];
	LOBBY_ERROR_TYPES KICK_TYPE = ERROR_CHEAT;

	ssprintf(buf, "!!>Msg: %s, Actual: %d, Bad: %d", msg, actual, from);
	NETlogEntry(buf, SYNC_FLAG, actual);
	if (NetPlay.isHost)
	{
		ssprintf(buf, "Auto kicking player %s for cheating", NetPlay.players[actual].name);
		sendTextMessage(buf, true);
		kickPlayer(actual, buf, KICK_TYPE);
	}
}
// ////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////
// Score
// We use setMultiStats() to broadcast the score when needed.
BOOL sendScoreCheck(void)
{
	static uint32_t	lastsent = 0;

	if (lastsent > gameTime)
	{
		lastsent = 0;
	}

	if (gameTime - lastsent < SCORE_FREQUENCY)
	{
		return true;
	}

	lastsent = gameTime;

	// Broadcast any changes in other players, but not in FRONTEND!!!
	if (titleMode != MULTIOPTION && titleMode != MULTILIMIT)
	{
		uint8_t			i;

		for (i = 0; i < MAX_PLAYERS; i++)
		{
			PLAYERSTATS		stats;

			// Host controls AI's scores + his own...
			if (myResponsibility(i))
			{
				// Update score
				stats = getMultiStats(i);

				// Add recently scored points
				stats.recentKills += stats.killsToAdd;
				stats.totalKills  += stats.killsToAdd;
				stats.recentScore += stats.scoreToAdd;
				stats.totalScore  += stats.scoreToAdd;

				// Zero them out
				stats.killsToAdd = stats.scoreToAdd = 0;

				// Send score to everyone else
				setMultiStats(i, stats, false);
			}
		}
	}

	return true;
}

// ////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////
// Pings

static uint32_t averagePing(void)
{
	uint32_t i, count = 0, total = 0;

	for(i = 0; i < MAX_PLAYERS; i++)
	{
		if(isHumanPlayer(i))
		{
			total += ingame.PingTimes[i];
			count ++;
		}
	}
	return total / MAX(count, 1);
}

BOOL sendPing(void)
{
	BOOL			isNew = true;
	uint8_t			player = selectedPlayer;
	int				i;
	static uint32_t	lastPing = 0;	// Last time we sent a ping
	static uint32_t	lastav = 0;		// Last time we updated average

	// Only ping every so often
	if (lastPing > gameTime)
	{
		lastPing = 0;
	}

	if (gameTime - lastPing < PING_FREQUENCY)
	{
		return true;
	}

	lastPing = gameTime;

	// If host, also update the average ping stat for joiners
	if (NetPlay.isHost)
	{
		if (lastav > gameTime)
		{
			lastav = 0;
		}

		if (gameTime - lastav > AV_PING_FREQUENCY)
		{
			NETsetGameFlags(2, averagePing());
			lastav = gameTime;
		}
	}

	/*
	 * Before we send the ping, if any player failed to respond to the last one
	 * we should re-enumerate the players.
	 */

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (isHumanPlayer(i)
				&& PingSend[i]
				&& ingame.PingTimes[i]
				&& i != selectedPlayer)
		{
			ingame.PingTimes[i] = PING_LIMIT;
		}
		else if (!isHumanPlayer(i)
				 && PingSend[i]
				 && ingame.PingTimes[i]
				 && i != selectedPlayer)
		{
			ingame.PingTimes[i] = 0;
		}
	}

	NETbeginEncode(NET_PING, NET_ALL_PLAYERS);
	NETuint8_t(&player);
	NETbool(&isNew);
	NETend();

	// Note when we sent the ping
	for (i = 0; i < MAX_PLAYERS; i++)
	{
		PingSend[i] = gameTime2;
	}

	return true;
}

// accept and process incoming ping messages.
BOOL recvPing()
{
	BOOL	isNew;
	uint8_t	sender, us = selectedPlayer;

	NETbeginDecode(NET_PING);
	NETuint8_t(&sender);
	NETbool(&isNew);
	NETend();

	if (sender >= MAX_PLAYERS)
	{
		debug(LOG_ERROR, "Bad NET_PING packet, sender is %d", (int)sender);
		return false;
	}

	// If this is a new ping, respond to it
	if (isNew)
	{
		NETbeginEncode(NET_PING, sender);
		// We are responding to a new ping
		isNew = false;

		NETuint8_t(&us);
		NETbool(&isNew);
		NETend();
	}
	// They are responding to one of our pings
	else
	{
		// Work out how long it took them to respond
		ingame.PingTimes[sender] = (gameTime2 - PingSend[sender]) / 2;

		// Note that we have received it
		PingSend[sender] = 0;
	}

	return true;
}
