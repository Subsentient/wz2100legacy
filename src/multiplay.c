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
 * Multiplay.c
 *
 * Alex Lee, Sep97, Pumpkin Studios
 *
 * Contains the day to day networking stuff, and received message handler.
 */
#include <string.h>

#include "lib/framework/frame.h"
#include "lib/framework/input.h"
#include "lib/framework/strres.h"
#include "map.h"

#include "stats.h"									// for templates.
#include "game.h"									// for loading maps
#include "hci.h"

#include <time.h>									// for recording ping times.
#include "research.h"
#include "display3d.h"								// for changing the viewpoint
#include "console.h"								// for screen messages
#include "power.h"
#include "cmddroid.h"								//  for commanddroidupdatekills
#include "wrappers.h"								// for game over
#include "component.h"
#include "frontend.h"
#include "lib/sound/audio.h"
#include "lib/sound/audio_id.h"
#include "levels.h"
#include "selection.h"
#include "spectate.h"

#include "init.h"
#include "warcam.h"	// these 4 for fireworks
#include "mission.h"
#include "effects.h"
#include "lib/gamelib/gtime.h"
#include "keybind.h"

#include "lib/script/script.h"				//Because of "ScriptTabs.h"
#include "scripttabs.h"			//because of CALL_AI_MSG
#include "scriptcb.h"			//for console callback
#include "scriptfuncs.h"

#include "lib/netplay/netplay.h"								// the netplay library.
#include "multiplay.h"								// warzone net stuff.
#include "multijoin.h"								// player management stuff.
#include "multirecv.h"								// incoming messages stuff
#include "multistat.h"
#include "multigifts.h"								// gifts and alliances.
#include "multiint.h"

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// globals.
bool						isMPDirtyBit = false;		// When we are forced to use turnOffMultiMsg() we set this
BOOL						bMultiPlayer				= false;	// true when more than 1 player.
BOOL						bMultiMessages				= false;	// == bMultiPlayer unless multimessages are disabled
BOOL						openchannels[MAX_PLAYERS] = {true};
uint8_t						bDisplayMultiJoiningStatus;

MULTIPLAYERGAME				game;									//info to describe game.
MULTIPLAYERINGAME			ingame;

char						beaconReceiveMsg[MAX_PLAYERS][MAX_CONSOLE_STRING_LENGTH];	//beacon msg for each player
char								playerName[MAX_PLAYERS][MAX_STR_LENGTH];	//Array to store all player names (humans and AIs)
BOOL						bPlayerReadyGUI[MAX_PLAYERS] = {false};

#define WIN_ROT_SPEED 48 // formula: MAX_ROT_SPEED / WIN_ROT_SPEED - as you decrease this value, speed is increased.

/////////////////////////////////////
/* multiplayer message stack stuff */
/////////////////////////////////////
#define MAX_MSG_STACK	100				// must be *at least* 64

static char msgStr[MAX_MSG_STACK][MAX_STR_LENGTH];
static int32_t msgPlFrom[MAX_MSG_STACK];
static int32_t msgPlTo[MAX_MSG_STACK];
static int32_t callbackType[MAX_MSG_STACK];
static int32_t locx[MAX_MSG_STACK];
static int32_t locy[MAX_MSG_STACK];
static DROID *msgDroid[MAX_MSG_STACK];
static int32_t msgStackPos = -1;				//top element pointer

// ////////////////////////////////////////////////////////////////////////////
// Remote Prototypes
extern RESEARCH			*asResearch;							//list of possible research items.
extern PLAYER_RESEARCH		*asPlayerResList[MAX_PLAYERS];
extern int NET_PlayerConnectionStatus;
// ////////////////////////////////////////////////////////////////////////////
// Local Prototypes

static BOOL recvBeacon(void);
static BOOL recvDestroyTemplate(void);
static BOOL recvResearch(void);

bool		multiplayPlayersReady		(bool bNotifyStatus);
void		startMultiplayerGame		(void);

// ////////////////////////////////////////////////////////////////////////////
// temporarily disable multiplayer mode.
BOOL turnOffMultiMsg(BOOL bDoit)
{
	if (!bMultiPlayer)
	{
		return true;
	}

	bMultiMessages = !bDoit;
	if (bDoit)
	{
		isMPDirtyBit = true;
	}
	return true;
}


// ////////////////////////////////////////////////////////////////////////////
// throw a pary when you win!
BOOL multiplayerWinSequence(BOOL firstCall)
{
	static Vector3i pos;
	Vector3i pos2;
	static uint32_t last = 0;
	float		rotAmount;
	STRUCTURE	*psStruct;

	if(firstCall)
	{
		pos  = cameraToHome(selectedPlayer, true);			// pan the camera to home if not already doing so
		last = 0;

		// stop all research
		CancelAllResearch(selectedPlayer);

		// stop all manufacture.
		for(psStruct = apsStructLists[selectedPlayer]; psStruct; psStruct = psStruct->psNext)
		{
			if (StructIsFactory(psStruct))
			{
				if (((FACTORY *)psStruct->pFunctionality)->psSubject)//check if active
				{
					cancelProduction(psStruct);
				}
			}
		}
	}

	// rotate world
	if (MissionResUp && !getWarCamStatus())
	{
		rotAmount = timeAdjustedIncrement(MAP_SPIN_RATE / WIN_ROT_SPEED, true);
		player.r.y += rotAmount;
	}

	if(last > gameTime)
	{
		last = 0;
	}
	if((gameTime - last) < 500 )							// only  if not done recently.
	{
		return true;
	}
	last = gameTime;

	if(rand() % 3 == 0)
	{
		pos2 = pos;
		pos2.x +=  (rand() % world_coord(8)) - world_coord(4);
		pos2.z +=  (rand() % world_coord(8)) - world_coord(4);

		if (pos2.x < 0)
		{
			pos2.x = 128;
		}

		if ((unsigned)pos2.x > world_coord(mapWidth))
		{
			pos2.x = world_coord(mapWidth);
		}

		if (pos2.z < 0)
		{
			pos2.z = 128;
		}

		if ((unsigned)pos2.z > world_coord(mapHeight))
		{
			pos2.z = world_coord(mapHeight);
		}

		addEffect(&pos2, EFFECT_FIREWORK, FIREWORK_TYPE_LAUNCHER, false, NULL, 0);	// throw up some fire works.
	}

	// show the score..


	return true;
}

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MultiPlayer main game loop code.
BOOL multiPlayerLoop(void)
{
	uint32_t		i;
	uint8_t		joinCount;

	sendCheck();						// send some checking info if possible
	processMultiPlayerArtifacts();		// process artifacts

	joinCount = 0;
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		if(isHumanPlayer(i) && ingame.JoiningInProgress[i] )
		{
			joinCount++;
		}
	}
	if(joinCount)
	{
		setWidgetsStatus(false);
		bDisplayMultiJoiningStatus = joinCount;	// someone is still joining! say So

		// deselect anything selected.
		selDroidDeselect(selectedPlayer);

		if(keyPressed(KEY_ESC) )// check for cancel
		{
			bDisplayMultiJoiningStatus = 0;
			setWidgetsStatus(true);
			setPlayerHasLost(true);
		}
	}
	else		//everyone is in the game now!
	{
		if(bDisplayMultiJoiningStatus)
		{
			bDisplayMultiJoiningStatus = 0;
			setWidgetsStatus(true);
		}
		if (!ingame.TimeEveryoneIsInGame)
		{
			ingame.TimeEveryoneIsInGame = gameTime;
			debug(LOG_NET, "I have entered the game @ %d", ingame.TimeEveryoneIsInGame );
			NETlogEntry("player entered game @ ", SYNC_FLAG, ingame.TimeEveryoneIsInGame);
			if (!NetPlay.isHost)
			{
#ifdef DEBUG
				addConsoleMessage("Sending data check...", LEFT_JUSTIFY, NOTIFY_MESSAGE);
#endif
				debug(LOG_NET, "=== Sending hash to host ===");
				NETlogEntry("player sent hash to host ", SYNC_FLAG, selectedPlayer);
				sendDataCheck();
			}
		}
		// Only have to do this on a true MP game
		if (NetPlay.isHost && !ingame.isAllPlayersDataOK && NetPlay.bComms)
		{
			if (gameTime - ingame.TimeEveryoneIsInGame > GAME_TICKS_PER_SEC * 60)
			{
				// NOTE: 60 secs is used for slow systems and dialup users.
				// we waited 60 secs to make sure people didn't bypass the data integrity checks
				int index;
				for (index = 0; index < MAX_PLAYERS; index++)
				{
					if (ingame.DataIntegrity[index] == false && isHumanPlayer(index) && index != NET_HOST_ONLY)
					{
						char msg[256] = {'\0'};

						snprintf(msg, 256, _("Kicking player %s, because they tried to bypass data integrity check!"), getPlayerName(index));
						sendTextMessage(msg, true);
						addConsoleMessage(msg, LEFT_JUSTIFY, NOTIFY_MESSAGE);
						NETlogEntry(msg, SYNC_FLAG, index);

						kickPlayer(index, "it is not nice to cheat!", ERROR_CHEAT);
						debug(LOG_INFO, "Kicking Player %s (%u), they tried to bypass data integrity check!", getPlayerName(index), index);
					}
				}
				ingame.isAllPlayersDataOK = true;
			}
		}
	}

	recvMessage();						// get queued messages


	// if player has won then process the win effects...
	if(testPlayerHasWon())
	{
		multiplayerWinSequence(false);
	}
	return true;
}


// ////////////////////////////////////////////////////////////////////////////
// quikie functions.

// to get droids ...
BOOL IdToDroid(uint32_t id, uint32_t player, DROID **psDroid)
{
	uint32_t i;
	DROID *d;

	if(player == ANYPLAYER)
	{
		for(i = 0; i < MAX_PLAYERS; i++)			// find the droid to order form them all
		{
			d = apsDroidLists[i];
			while((d != NULL ) && (d->id != id) )
			{
				d = d->psNext;
			}
			if(d)
			{
				*psDroid = d;
				return true;
			}
		}
		return false;
	}
	else									// find the droid, given player
	{
		if (player >= MAX_PLAYERS)
		{
			debug(LOG_FEATURE, "Feature detected");
			// feature hack, player = 9 are features
			return false;
		}
		d = apsDroidLists[player];
		while( (d != NULL ) && (d->id != id))
		{
			d = d->psNext;
		}
		if(d)
		{
			*psDroid = d;
			return true;
		}
		return false;
	}
}

// ////////////////////////////////////////////////////////////////////////////
// find a structure
STRUCTURE *IdToStruct(uint32_t id, uint32_t player)
{
	STRUCTURE	*psStr = NULL;
	uint32_t		i;

	if(player == ANYPLAYER)
	{
		for(i = 0; i < MAX_PLAYERS; i++)
		{
			for(psStr = apsStructLists[i]; ( (psStr != NULL) && (psStr->id != id)); psStr = psStr->psNext);
			if(psStr)
			{
				return psStr;
			}
		}
	}
	else
	{
		if (player >= MAX_PLAYERS)
		{
			debug(LOG_FEATURE, "Feature detected");
			// feature hack, player = 9 are features
			return NULL;
		}
		for(psStr = apsStructLists[player]; ((psStr != NULL ) && (psStr->id != id) ); psStr = psStr->psNext);
	}
	return psStr;
}

// ////////////////////////////////////////////////////////////////////////////
// find a feature
FEATURE *IdToFeature(uint32_t id, uint32_t player)
{
	FEATURE	*psF = NULL;
	uint32_t	i;

	if(player == ANYPLAYER)
	{
		for(i = 0; i < MAX_PLAYERS; i++)
		{
			for(psF = apsFeatureLists[i]; ( (psF != NULL) && (psF->id != id)); psF = psF->psNext);
			if(psF)
			{
				return psF;
			}
		}
	}
	else
	{
		if (player >= MAX_PLAYERS)
		{
			debug(LOG_FEATURE, "Feature detected");
			// feature hack, player = 9 are features
			return NULL;
		}
		for(psF = apsFeatureLists[player]; ((psF != NULL ) && (psF->id != id) ); psF = psF->psNext);
	}
	return psF;
}

// ////////////////////////////////////////////////////////////////////////////

DROID_TEMPLATE *IdToTemplate(uint32_t tempId, uint32_t player)
{
	DROID_TEMPLATE *psTempl = NULL;
	uint32_t		i;

	// First try static templates from scripts (could potentially also happen for currently human controlled players)
	for (psTempl = apsStaticTemplates; psTempl && psTempl->multiPlayerID != tempId; psTempl = psTempl->psNext) ;
	if (psTempl)
	{
		return psTempl;
	}

	// Check if we know which player this is from, in that case, assume it is a player template
	if (player != ANYPLAYER && player < MAX_PLAYERS)
	{
		for (psTempl = apsDroidTemplates[player];			// follow templates
				(psTempl && (psTempl->multiPlayerID != tempId ));
				psTempl = psTempl->psNext);

		return psTempl;
	}

	// We have no idea, so search through every player template
	for (i = 0; i < MAX_PLAYERS; i++)
	{
		for (psTempl = apsDroidTemplates[i]; psTempl && psTempl->multiPlayerID != tempId; psTempl = psTempl->psNext) ;
		if (psTempl)
		{
			return psTempl;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////
//  Returns a pointer to base object, given an id and optionally a player.
BASE_OBJECT *IdToPointer(uint32_t id, uint32_t player)
{
	DROID		*pD;
	STRUCTURE	*pS;
	FEATURE		*pF;
	// droids.
	if (IdToDroid(id, player, &pD))
	{
		return (BASE_OBJECT *)pD;
	}

	// structures
	pS = IdToStruct(id, player);
	if(pS)
	{
		return (BASE_OBJECT *)pS;
	}

	// features
	pF = IdToFeature(id, player);
	if(pF)
	{
		return (BASE_OBJECT *)pF;
	}

	return NULL;
}


// ////////////////////////////////////////////////////////////////////////////
// return a players name.
const char *getPlayerName(unsigned int player)
{
	ASSERT_OR_RETURN(NULL, player < MAX_PLAYERS , "Wrong player index: %u", player);

	if (game.type != CAMPAIGN)
	{
		if (strcmp(playerName[player], "") != 0)
		{
			return (char *)&playerName[player];
		}
	}

	if (strlen(NetPlay.players[player].name) == 0)
	{
		// make up a name for this player.
		return getPlayerColourName(player);
	}

	return NetPlay.players[player].name;
}

BOOL setPlayerName(uint32_t player, const char *sName)
{
	ASSERT_OR_RETURN(false, player < MAX_PLAYERS, "Player index (%u) out of range", player);
	sstrcpy(playerName[player], sName);
	return true;
}

// ////////////////////////////////////////////////////////////////////////////
// to determine human/computer players and responsibilities of each..
BOOL isHumanPlayer(uint32_t player)
{
	if (player >= MAX_PLAYERS)
	{
		return false;	// obvious, really
	}
	return NetPlay.players[player].allocated;
}

// returns player responsible for 'player'
uint32_t  whosResponsible(uint32_t player)
{
	if (isHumanPlayer(player))
	{
		return player;			// Responsible for him or her self
	}
	else if (player == selectedPlayer)
	{
		return player;			// We are responsibly for ourselves
	}
	else
	{
		return NET_HOST_ONLY;	// host responsible for all AIs
	}
}

//returns true if selected player is responsible for 'player'
BOOL myResponsibility(uint32_t player)
{
	if(whosResponsible(player) == selectedPlayer)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//returns true if 'player' is responsible for 'playerinquestion'
BOOL responsibleFor(uint32_t player, uint32_t playerinquestion)
{
	if(whosResponsible(playerinquestion) == player)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// ////////////////////////////////////////////////////////////////////////////
// probably temporary. Places the camera on the players 1st droid or struct.
Vector3i cameraToHome(uint32_t player, BOOL scroll)
{
	Vector3i res;
	uint32_t x, y;
	STRUCTURE	*psBuilding;

	for (psBuilding = apsStructLists[player];
			psBuilding && (psBuilding->pStructureType->type != REF_HQ);
			psBuilding = psBuilding->psNext);

	if(psBuilding)
	{
		x = map_coord(psBuilding->pos.x);
		y = map_coord(psBuilding->pos.y);
	}
	else if (apsDroidLists[player])				// or first droid
	{
		x = map_coord(apsDroidLists[player]->pos.x);
		y =	map_coord(apsDroidLists[player]->pos.y);
	}
	else if (apsStructLists[player])							// center on first struct
	{
		x = map_coord(apsStructLists[player]->pos.x);
		y = map_coord(apsStructLists[player]->pos.y);
	}
	else														//or map center.
	{
		x = mapWidth / 2;
		y = mapHeight / 2;
	}


	if(scroll)
	{
		requestRadarTrack(world_coord(x), world_coord(y));
	}
	else
	{
		setViewPos(x, y, true);
	}

	res.x = world_coord(x);
	res.y = map_TileHeight(x, y);
	res.z = world_coord(y);
	return res;
}


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Recv Messages. Get a message and dispatch to relevant function.
BOOL recvMessage(void)
{
	uint8_t type;

	while(NETrecv(&type))			// for all incoming messages.
	{
		// messages only in game.
		if(!ingame.localJoiningInProgress)
		{
			switch(type)
			{
				case NET_DROID:						// new droid of known type
					recvDroid();
					break;
				case NET_DROIDINFO:					//droid update info
					recvDroidInfo();
					break;
				case NET_DROIDDEST:					// droid destroy
					recvDestroyDroid();
					break;
				case NET_DROIDMOVE:					// move a droid to x,y command.
					recvDroidMove();
					break;
				case NET_GROUPORDER:				// an order for more than 1 droid.
					recvGroupOrder();
					break;
				case NET_CHECK_DROID:				// droid damage and position checks
					recvDroidCheck();
					break;
				case NET_CHECK_STRUCT:				// structure damage checks.
					recvStructureCheck();
					break;
				case NET_CHECK_POWER:				// Power level syncing.
					recvPowerCheck();
					break;
				case NET_TEXTMSG:					// simple text message
					recvTextMessage();
					break;
				case NET_DATA_CHECK:
					recvDataCheck();
					break;
				case NET_AITEXTMSG:					//multiplayer AI text message
					recvTextMessageAI();
					break;
				case NET_BEACONMSG:					//beacon (blip) message
					recvBeacon();
					break;
				case NET_BUILD:						// a build order has been sent.
					recvBuildStarted();
					break;
				case NET_BUILDFINISHED:				// a building is complete
					recvBuildFinished();
					break;
				case NET_STRUCTDEST:				// structure destroy
					recvDestroyStructure();
					break;
				case NET_SECONDARY:					// set a droids secondary order level.
					recvDroidSecondary();
					break;
				case NET_SECONDARY_ALL:					// set a droids secondary order level.
					recvDroidSecondaryAll();
					break;
				case NET_DROIDEMBARK:
					recvDroidEmbark();              //droid has embarked on a Transporter
					break;
				case NET_DROIDDISEMBARK:
					recvDroidDisEmbark();           //droid has disembarked from a Transporter
					break;
				case NET_GIFT:						// an alliance gift from one player to another.
					recvGift();
					break;
				case NET_SCORESUBMIT:				//  a score update from another player [UNUSED] see NET_PLAYER_STATS
					break;
				case NET_VTOL:
					recvHappyVtol();
					break;
				case NET_LASSAT:
					recvLasSat();
					break;

				case NET_SPECTATE:
					recvSpectateRequest();
					break;
				default:
					break;
			}
		}

		// messages usable all the time
		switch(type)
		{
			case NET_TEMPLATE:					// new template
				recvTemplate();
				break;
			case NET_TEMPLATEDEST:				// template destroy
				recvDestroyTemplate();
				break;
			case NET_FEATUREDEST:				// feature destroy
				recvDestroyFeature();
				break;
			case NET_PING:						// diagnostic ping msg.
				recvPing();
				break;
			case NET_DEMOLISH:					// structure demolished.
				recvDemolishFinished();
				break;
			case NET_RESEARCH:					// some research has been done.
				recvResearch();
				break;
			case NET_OPTIONS:
				recvOptions();
				break;
			case NET_PLAYER_DROPPED:				// remote player got disconnected
			{
				uint32_t player_id;

				NETbeginDecode(NET_PLAYER_DROPPED);
				{
					NETuint32_t(&player_id);
				}
				NETend();

				if (whosResponsible(player_id) != NetMsg.source && NetMsg.source != NET_HOST_ONLY)
				{
					HandleBadParam("NET_PLAYER_DROPPED given incorrect params.", player_id, NetMsg.source);
					break;
				}

				debug(LOG_INFO, "** player %u has dropped!", player_id);

				MultiPlayerLeave(player_id);		// get rid of their stuff
				NET_PlayerConnectionStatus = 2;		//DROPPED_CONNECTION
				break;
			}
			case NET_PLAYERRESPONDING:			// remote player is now playing
			{
				uint32_t player_id;

				resetReadyStatus(false);

				NETbeginDecode(NET_PLAYERRESPONDING);
				// the player that has just responded
				NETuint32_t(&player_id);
				NETend();
				if (player_id >= MAX_PLAYERS)
				{
					debug(LOG_ERROR, "Bad NET_PLAYERRESPONDING received, ID is %d", (int)player_id);
					break;
				}
				// This player is now with us!
				ingame.JoiningInProgress[player_id] = false;
				break;
			}
			// FIXME: the next 4 cases might not belong here --check (we got two loops for this)
			case NET_COLOURREQUEST:
				recvColourRequest();
				break;
			case NET_POSITIONREQUEST:
				recvPositionRequest();
				break;
			case NET_TEAMREQUEST:
				recvTeamRequest();
				break;
			case NET_READY_REQUEST:
				if (NetPlay.isHost)
				{
					recvReadyRequest();
					if (multiplayPlayersReady(false))
					{
						// if hosting try to start the game if everyone is ready
						startMultiplayerGame();
					}
				}
				break;
			case NET_ARTIFACTS:
				recvMultiPlayerRandomArtifacts();
				break;
			case NET_ARTIFACTS_REQUEST:
				if (NetPlay.isHost)
				{
					HandleArtifact();
				}
				break;
			case NET_FEATURES:
				recvMultiPlayerFeature();
				break;
			case NET_ALLIANCE:
				recvAlliance(true);
				break;
			case NET_KICK:
			{
				// In game kick
				uint32_t player_id;
				char reason[MAX_KICK_REASON];
				LOBBY_ERROR_TYPES KICK_TYPE = ERROR_NOERROR;

				NETbeginDecode(NET_KICK);
				NETuint32_t(&player_id);
				NETstring( reason, MAX_KICK_REASON);
				NETenum(&KICK_TYPE);
				NETend();

				if (NetMsg.source != NET_HOST_ONLY)
				{
					char buf[250] = {'\0'};

					ssprintf(buf, "Player %d (%s : %s) tried to kick %u", (int) NetMsg.source, NetPlay.players[NetMsg.source].name, NetPlay.players[NetMsg.source].IPtextAddress, player_id);
					NETlogEntry(buf, SYNC_FLAG, 0);
					debug(LOG_ERROR, "%s", buf);
					if (NetPlay.isHost)
					{
						kickPlayer(NetMsg.source, reason, KICK_TYPE);
					}
					break;
				}

				if (selectedPlayer == player_id)  // we've been told to leave.
				{
					debug(LOG_ERROR, "You were kicked because %s", reason);
					setPlayerHasLost(true);
				}
				else
				{
					NETplayerKicked(player_id);
				}
				break;
			}
			case NET_FIREUP:				// frontend only
				debug(LOG_NET, "NET_FIREUP was received (frontend only?)");
				break;
			case NET_RESEARCHSTATUS:
				recvResearchStatus();
				break;
			case NET_PLAYER_STATS:
				recvMultiStats();
				break;
			case NET_PAGEPLAYER:
			{
				recvPageSig();
				break;
			}
			default:
				break;
		}
	}

	return true;
}


// ////////////////////////////////////////////////////////////////////////////
// Research Stuff. Nat games only send the result of research procedures.
BOOL SendResearch(uint8_t player, uint32_t index)
{
	uint8_t i;
	PLAYER_RESEARCH *pPlayerRes;

	// Send the player that is researching the topic and the topic itself
	NETbeginEncode(NET_RESEARCH, NET_ALL_PLAYERS);
	NETuint8_t(&player);
	NETuint32_t(&index);
	NETend();

	/*
	 * Since we are called when the state of research changes (completed,
	 * stopped &c) we also need to update our own local copy of what our allies
	 * are doing/have done.
	 */
	if (game.type == SKIRMISH)
	{
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (alliances[i][player] == ALLIANCE_FORMED)
			{
				pPlayerRes = asPlayerResList[i] + index;

				// If we have it but they don't
				if (!IsResearchCompleted(pPlayerRes))
				{
					// Do the research for that player
					MakeResearchCompleted(pPlayerRes);
					researchResult(index, i, false, NULL);
				}
			}
		}
	}

	return true;
}

// recv a research topic that is now complete.
static BOOL recvResearch()
{
	uint8_t			player;
	uint32_t		index;
	int				i;
	PLAYER_RESEARCH	*pPlayerRes;
	RESEARCH		*pResearch;

	NETbeginDecode(NET_RESEARCH);
	NETuint8_t(&player);
	NETuint32_t(&index);
	NETend();

	if (player >= MAX_PLAYERS || index >= numResearch)
	{
		debug(LOG_ERROR, "Bad NET_RESEARCH received, player is %d, index is %u", (int)player, index);
		return false;
	}

	pPlayerRes = asPlayerResList[player] + index;

	if (!IsResearchCompleted(pPlayerRes))
	{
		MakeResearchCompleted(pPlayerRes);
		researchResult(index, player, false, NULL);

		// Take off the power if available
		pResearch = asResearch + index;
		usePower(player, pResearch->researchPower);
	}

	// Update allies research accordingly
	if (game.type == SKIRMISH)
	{
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (alliances[i][player] == ALLIANCE_FORMED)
			{
				pPlayerRes = asPlayerResList[i] + index;

				if (!IsResearchCompleted(pPlayerRes))
				{
					// Do the research for that player
					MakeResearchCompleted(pPlayerRes);
					researchResult(index, i, false, NULL);
				}
			}
		}
	}

	return true;
}

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// New research stuff, so you can see what others are up to!
// inform others that I'm researching this.
BOOL sendReseachStatus(STRUCTURE *psBuilding, uint32_t index, uint8_t player, BOOL bStart)
{
	if (!myResponsibility(player) || gameTime < 5)
	{
		return true;
	}

	NETbeginEncode(NET_RESEARCHSTATUS, NET_ALL_PLAYERS);
	NETuint8_t(&player);
	NETbool(&bStart);

	// If we know the building researching it then send its ID
	if (psBuilding)
	{
		NETuint32_t(&psBuilding->id);
	}
	else
	{
		NETnull();
	}

	// Finally the topic in question
	NETuint32_t(&index);
	NETend();

	return true;
}

BOOL recvResearchStatus()
{
	STRUCTURE			*psBuilding;
	PLAYER_RESEARCH		*pPlayerRes;
	RESEARCH_FACILITY	*psResFacilty;
	RESEARCH			*pResearch;
	uint8_t				player;
	BOOL				bStart;
	uint32_t			index, structRef;

	NETbeginDecode(NET_RESEARCHSTATUS);
	NETuint8_t(&player);
	NETbool(&bStart);
	NETuint32_t(&structRef);
	NETuint32_t(&index);
	NETend();

	if (player >= MAX_PLAYERS || index >= numResearch)
	{
		debug(LOG_ERROR, "Bad NET_RESEARCHSTATUS received, player is %d, index is %u", (int)player, index);
		return false;
	}

	pPlayerRes = asPlayerResList[player] + index;

	// psBuilding may be null if finishing
	if (bStart)							// Starting research
	{
		psBuilding = IdToStruct(structRef, player);

		// Set that facility to research
		if (psBuilding && psBuilding->pFunctionality)
		{
			psResFacilty = (RESEARCH_FACILITY *) psBuilding->pFunctionality;

			if (psResFacilty->psSubject)
			{
				cancelResearch(psBuilding);
			}

			// Set the subject up
			pResearch				= asResearch + index;
			psResFacilty->psSubject = (BASE_STATS *) pResearch;

			// If they have previously started but cancelled there is no need to accure power
			if (IsResearchCancelled(pPlayerRes))
			{
				psResFacilty->powerAccrued	= pResearch->researchPower;
			}
			else
			{
				psResFacilty->powerAccrued	= 0;
			}

			// Start the research
			MakeResearchStarted(pPlayerRes);
			psResFacilty->timeStarted		= ACTION_START_TIME;
			psResFacilty->timeStartHold		= 0;
			psResFacilty->timeToResearch	= pResearch->researchPoints / MAX(psResFacilty->researchPoints, 1);

			// A failsafe of some sort
			if (psResFacilty->timeToResearch == 0)
			{
				psResFacilty->timeToResearch = 1;
			}
		}

	}
	// Finished/cancelled research
	else
	{
		// If they completed the research, we're done
		if (IsResearchCompleted(pPlayerRes))
		{
			return true;
		}

		// If they did not say what facility it was, look it up orselves
		if (!structRef)
		{
			// Go through the structs to find the one doing this topic
			for (psBuilding = apsStructLists[player]; psBuilding; psBuilding = psBuilding->psNext)
			{
				if (psBuilding->pStructureType->type == REF_RESEARCH
						&& psBuilding->status == SS_BUILT
						&& ((RESEARCH_FACILITY *) psBuilding->pFunctionality)->psSubject
						&& ((RESEARCH_FACILITY *) psBuilding->pFunctionality)->psSubject->ref - REF_RESEARCH_START == index)
				{
					break;
				}
			}
		}
		else
		{
			psBuilding = IdToStruct(structRef, player);
		}

		// Stop the facility doing any research
		if (psBuilding)
		{
			cancelResearch(psBuilding);
		}
	}

	return true;
}


//Prints a list of current games in the lobby to the Legacy command console.
static void ListLobbyIngame(void)
{
	unsigned Inc = 0, GameCount = 0;
	
	GameCount = NETfindGame(); //Get list of games.
	
	if (!GameCount)
	{//No games.
		addConsoleMessage("No games in lobby.", DEFAULT_JUSTIFY, CMD_MESSAGE);
		return;
	}
	else
	{
		char OutBuf[256];
		
		snprintf(OutBuf, sizeof OutBuf, "%u games in lobby:", GameCount);
		addConsoleMessage(OutBuf, DEFAULT_JUSTIFY, CMD_MESSAGE);
	}
		
	for (Inc = 0; Inc < GameCount; ++Inc)
	{
		const GAMESTRUCT *const Game = NetPlay.games + Inc;
		char OutBuf[2048], ModList[sizeof Game->modlist + 64];
		
		if (*Game->modlist) snprintf(ModList, sizeof ModList, " (mods: %s)", Game->modlist);
		
		snprintf(OutBuf, sizeof OutBuf, "Name: %s || Map: %s || Host: %s || Players: %d/%d || Version: %s%s",
				Game->name, Game->mapname, Game->hostname, (int)Game->desc.dwCurrentPlayers, (int)Game->desc.dwMaxPlayers,
				Game->versionstring, *Game->modlist ? ModList : "");
				
		addConsoleMessage(OutBuf, DEFAULT_JUSTIFY, CMD_MESSAGE);
	}
	
}	
/*This function is used to try and understand all the odd commands you can give the Legacy console.*/
BOOL parseConsoleCommands(const char *InBuffer, short IsGameConsole)
{
	/*I don't like bools.*/
#define Matches(y) !strcmp(InBuffer, y)
#define StartsWith(y) !strncmp(InBuffer, y, strlen(y))
	char ConsoleOut[MAX_CONSOLE_STRING_LENGTH] = "No string."; //Meh, failsafe.
	struct
	{
		const char *CmdName;
		BOOL AvailableAlways;
		BOOL TakesArg;
	} AvailableCommands[] =
	{
		{ "!help", true, false }, { "!name", false, true }, { "!kick", true, true }, { "!playerlist", true, false},
		{ "!beep", true, true }, {"!toggleticker", false, false}, { "!spectate", false, false }, { "!lobbygames", false, false },
		{ NULL, false, false }
	};
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/*Begin in-game only commands*/
	if (IsGameConsole)
	{
		if (StartsWith("!name "))
		{
			InBuffer += strlen("!name ");

			printConsoleNameChange(NetPlay.players[selectedPlayer].name, (char*)InBuffer);

			NETchangePlayerName(selectedPlayer, (char*)InBuffer);

			return true;
		}
		else if (Matches("!toggleticker"))
		{
			kf_ToggleTicker();
			return true;
		}
		else if (Matches("!spectate"))
		{
			SendSpectateRequest();
			return true;
		}
		else if (Matches("!lobbygames"))
		{
			ListLobbyIngame();
		}
	}
	/*End in-game only commands*/

	if (Matches("!help"))
	{
		/*Help command.*/
		unsigned long Inc, Counter;
		const char *Format[2] = { " [%s ... ] ", " [%s] " };
		short FormatNum;

		addConsoleMessage(_("The following console commands are available: "), DEFAULT_JUSTIFY, CMD_MESSAGE);

		for (Inc = Counter = ConsoleOut[0] = 0; AvailableCommands[Inc].CmdName != NULL; ++Inc)
		{
			if (AvailableCommands[Inc].TakesArg)
			{
				FormatNum = 0;
			}
			else
			{
				FormatNum = 1;
			}

			if (IsGameConsole || AvailableCommands[Inc].AvailableAlways)
			{
				char TmpConsole[MAX_CONSOLE_STRING_LENGTH];

				snprintf(TmpConsole, MAX_CONSOLE_STRING_LENGTH, Format[FormatNum], AvailableCommands[Inc].CmdName);
				strncat(ConsoleOut, TmpConsole, MAX_CONSOLE_STRING_LENGTH);
				++Counter;
			}

			if (AvailableCommands[Inc + 1].CmdName == NULL)
			{
				addConsoleMessage(ConsoleOut, DEFAULT_JUSTIFY, CMD_MESSAGE);
				ConsoleOut[0] = '\0';
				Counter = 0;
			}

		}

		addConsoleMessage(_("Items followed by periods expect a parameter."), DEFAULT_JUSTIFY, CMD_MESSAGE);

		return true;
	}
	else if (InBuffer[0] == '/') /*Allow for true /me messages.*/
	{
		char OutSend[MAX_CONSOLE_STRING_LENGTH];

		if (StartsWith("/me "))
		{
			InBuffer += (sizeof "/me " - 1);
		}
		else /*Give choice of / or /me.*/
		{
			++InBuffer;
		}

		snprintf(OutSend, MAX_CONSOLE_STRING_LENGTH, " » %s %s « ", /*8-bit woes.*/
				 NetPlay.players[selectedPlayer].name, InBuffer);

		addConsoleMessage(OutSend, LEFT_JUSTIFY, selectedPlayer);

		NETbeginEncode(NET_TEXTMSG, NET_ALL_PLAYERS);
		NETuint32_t(&selectedPlayer);
		NETstring(OutSend, MAX_CONSOLE_STRING_LENGTH);
		NETend();

		return true;
	}
	else if (StartsWith("!kick "))
	{
		/*Kick command, useful for quickness.*/
		if (NetPlay.isHost)
		{
			char OutText[MAX_CONSOLE_STRING_LENGTH];
			short PlayerToKick;

			InBuffer += strlen("!kick ");

			if (!isdigit(InBuffer[0]))
			{
				addConsoleMessage(_("Please enter a player number to kick."), LEFT_JUSTIFY, CMD_MESSAGE);
				return true;
			}

			PlayerToKick = (short)atoi(InBuffer);

			if (PlayerToKick >= MAX_PLAYERS || PlayerToKick < 0)
			{
				addConsoleMessage(_("Invalid player number."), LEFT_JUSTIFY, CMD_MESSAGE);
				return true;
			}

			if (PlayerToKick == selectedPlayer)
			{
				/*Uhh, can't kick ourselves.*/
				addConsoleMessage(_("You cannot kick yourself."), LEFT_JUSTIFY, CMD_MESSAGE);
				return true;
			}

			if (!NetPlay.bComms && bMultiPlayer && !isHumanPlayer(PlayerToKick))
			{
				addConsoleMessage(_("You cannot kick AIs in a skirmish."), LEFT_JUSTIFY, CMD_MESSAGE);
				return true;
			}

			sprintf(OutText, _("The host has kicked %s from the game!"), getPlayerName(PlayerToKick));

			sendTextMessage(OutText, true);

			kickPlayer(PlayerToKick, _("you are unwanted by the host."), ERROR_KICKED);
		}
		else
		{
			addConsoleMessage(_("You are not the host."), LEFT_JUSTIFY, CMD_MESSAGE);
		}

		return true;
	}
	else if (StartsWith("!beep "))
	{
		uint32_t PlayerToBeep;
		const char *PageFormat = _("Paging %s.");

		InBuffer += strlen("!beep ");

		if (!isdigit(InBuffer[0]))
		{
			addConsoleMessage(_("Please enter a player number to beep."), LEFT_JUSTIFY, CMD_MESSAGE);
			return true;
		}

		PlayerToBeep = atoi(InBuffer);

		if (PlayerToBeep >= MAX_PLAYERS)
		{
			addConsoleMessage(_("Invalid player number."), LEFT_JUSTIFY, CMD_MESSAGE);
			return true;
		}

		if (PlayerToBeep == selectedPlayer)
		{
			addConsoleMessage(_("You cannot beep yourself."), LEFT_JUSTIFY, CMD_MESSAGE);
			return true;
		}

		if (!isHumanPlayer(PlayerToBeep))
		{
			addConsoleMessage(_("You can only beep human players."), LEFT_JUSTIFY, CMD_MESSAGE);
			return true;
		}

		snprintf(ConsoleOut, MAX_CONSOLE_STRING_LENGTH, PageFormat, NetPlay.players[PlayerToBeep].name);

		addConsoleMessage(ConsoleOut, DEFAULT_JUSTIFY, CMD_MESSAGE);

		NETbeginEncode(NET_PAGEPLAYER, PlayerToBeep);
		NETuint32_t(&PlayerToBeep);
		NETuint32_t(&selectedPlayer);
		NETend();

		return true;
	}
	else if (Matches("!playerlist"))
	{
		unsigned Inc = 0;
		char Temp[MAX_CONSOLE_STRING_LENGTH];

		strncpy(ConsoleOut, _("Players: "), MAX_CONSOLE_STRING_LENGTH);

		for (; Inc < game.maxPlayers; ++Inc)
		{
			snprintf(Temp, MAX_CONSOLE_STRING_LENGTH, _("%s (%s) at slot %d; "), getPlayerName(Inc), getPlayerColourName(Inc), Inc);
			strncat(ConsoleOut, Temp, MAX_CONSOLE_STRING_LENGTH);
		}

		ConsoleOut[strlen(ConsoleOut) - 2] = '.'; //Remove trailing semicolon, replace with period.
		addConsoleMessage(ConsoleOut, DEFAULT_JUSTIFY, CMD_MESSAGE);

		return true;
	}

	return false;
}

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Text Messaging between players. proceed string with players to send to.
// eg "123hi there" sends "hi there" to players 1,2 and 3.
BOOL sendTextMessage(const char *pStr, BOOL all)
{
	BOOL				normal = true;
	BOOL				sendto[MAX_PLAYERS];
	int					posTable[MAX_PLAYERS];
	uint32_t				i;
	char				display[MAX_CONSOLE_STRING_LENGTH];
	char				msg[MAX_CONSOLE_STRING_LENGTH];
	char				*curStr = (char *)pStr, *TmpStr, NameScan[1024];

	if (!ingame.localOptionsReceived)
	{
		if(!bMultiPlayer)
		{
			// apparently we are not in a mp game, so dump the message to the console.
			addConsoleMessage(curStr, LEFT_JUSTIFY, SYSTEM_MESSAGE);
		}
		return true;
	}

	memset(display, 0x0, sizeof(display));	//clear buffer
	memset(msg, 0x0, sizeof(display));		//clear buffer
	memset(sendto, 0x0, sizeof(sendto));		//clear private flag
	memset(posTable, 0x0, sizeof(posTable));		//clear buffer
	sstrcpy(msg, curStr);

	/*This bit of code goes past our names, which are passed to this function.*/
	snprintf(NameScan, 1024, "%s: ", getPlayerName(selectedPlayer));
	TmpStr = strstr(curStr, NameScan);
	if (TmpStr == NULL)
	{
		TmpStr = curStr;
	}
	else
	{
		TmpStr += strlen(NameScan);
	}

	if (!all)
	{
		// create a position table
		for (i = 0; i < game.maxPlayers; i++)
		{
			posTable[NetPlay.players[i].position] = i;
		}

		if (TmpStr[0] == '.')
		{
			TmpStr++;
			for (i = 0; i < game.maxPlayers; i++)
			{
				if (i != selectedPlayer && aiCheckAlliances(selectedPlayer, i))
				{
					sendto[i] = true;
				}
			}
			normal = false;
			if (!all)
			{
				sstrcpy(display, _("(allies"));
			}
		}
		for (; TmpStr[0] >= '0' && TmpStr[0] <= '7'; TmpStr++)		// for each 0..7 numeric char encountered
		{
			i = posTable[TmpStr[0] - '0'];
			if (normal)
			{
				sstrcpy(display, _("(private to "));
			}
			else
			{
				sstrcat(display, ", ");
			}
			if ((isHumanPlayer(i) || (game.type == SKIRMISH && i < game.maxPlayers && game.skDiff[i] ) ))
			{
				sstrcat(display, getPlayerName(posTable[TmpStr[0] - '0']));
				sendto[i] = true;
			}
			else
			{
				sstrcat(display, _("[invalid]"));
			}
			normal = false;
		}

		if (!normal)	// lets user know it is a private message
		{
			if (TmpStr[0] == ' ')
			{
				TmpStr++;
			}
			sstrcat(display, ") ");
			if (TmpStr != NULL)
			{
				sstrcat(display, NameScan);
			}
			sstrcat(display, TmpStr);
		}
	}

	if (all)	//broadcast
	{
		NETbeginEncode(NET_TEXTMSG, NET_ALL_PLAYERS);
		NETuint32_t(&selectedPlayer);		// who this msg is from
		NETstring(msg, MAX_CONSOLE_STRING_LENGTH);	// the message to send
		NETend();
	}
	else if (normal)
	{
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (i != selectedPlayer && openchannels[i])
			{
				if (isHumanPlayer(i))
				{
					NETbeginEncode(NET_TEXTMSG, i);
					NETuint32_t(&selectedPlayer);		// who this msg is from
					NETstring(msg, MAX_CONSOLE_STRING_LENGTH);	// the message to send
					NETend();
				}
				else	//also send to AIs now (non-humans), needed for AI
				{
					sendAIMessage(msg, selectedPlayer, i);
				}
			}
		}
	}
	else	//private msg
	{
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if (sendto[i])
			{
				if (isHumanPlayer(i))
				{
					NETbeginEncode(NET_TEXTMSG, i);
					NETuint32_t(&selectedPlayer);				// who this msg is from
					NETstring(display, MAX_CONSOLE_STRING_LENGTH);	// the message to send
					NETend();
				}
				else	//also send to AIs now (non-humans), needed for AI
				{
					sendAIMessage(curStr, selectedPlayer, i);
				}
			}
		}
	}

	//This is for local display
	sstrcpy(msg, (normal ? curStr : display));						// add message

	addConsoleMessage(msg, DEFAULT_JUSTIFY, selectedPlayer);	// display

	return true;
}

void printConsoleNameChange(const char *oldName, const char *newName)
{
	char msg[MAX_CONSOLE_STRING_LENGTH];

	// Player changed name.
	sstrcpy(msg, oldName);                               // Old name.
	sstrcat(msg, " → ");                                 // Separator
	sstrcat(msg, newName);  // New name.

	addConsoleMessage(msg, DEFAULT_JUSTIFY, selectedPlayer);  // display
}


//AI multiplayer message, send from a certain player index to another player index
BOOL sendAIMessage(char *pStr, uint32_t player, uint32_t to)
{
	uint32_t	sendPlayer;

	//check if this is one of the local players, don't need net send then
	if (to == selectedPlayer || myResponsibility(to))	//(the only) human on this machine or AI on this machine
	{
		//Just show "him" the message
		displayAIMessage(pStr, player, to);

		//Received a console message from a player callback
		//store and call later
		//-------------------------------------------------
		if (!msgStackPush(CALL_AI_MSG, player, to, pStr, -1, -1, NULL))
		{
			debug(LOG_ERROR, "sendAIMessage() - msgStackPush - stack failed");
			return false;
		}
	}
	else		//not a local player (use multiplayer mode)
	{
		if (!ingame.localOptionsReceived)
		{
			return true;
		}

		//find machine that is hosting this human or AI
		sendPlayer = whosResponsible(to);

		if (sendPlayer >= MAX_PLAYERS)
		{
			debug(LOG_ERROR, "sendAIMessage() - sendPlayer >= MAX_PLAYERS");
			return false;
		}

		if (!isHumanPlayer(sendPlayer))		//NETsend can't send to non-humans
		{
			debug(LOG_ERROR, "sendAIMessage() - player is not human.");
			return false;
		}

		//send to the player who is hosting 'to' player (might be himself if human and not AI)
		NETbeginEncode(NET_AITEXTMSG, sendPlayer);
		NETuint32_t(&player);			//save the actual sender
		//save the actual player that is to get this msg on the source machine (source can host many AIs)
		NETuint32_t(&to);				//save the actual receiver (might not be the same as the one we are actually sending to, in case of AIs)
		NETstring(pStr, MAX_CONSOLE_STRING_LENGTH);		// copy message in.
		NETend();
	}

	return true;
}

//
// At this time, we do NOT support messages for beacons
//
BOOL sendBeacon(int32_t locX, int32_t locY, int32_t forPlayer, int32_t sender, const char *pStr)
{
	int sendPlayer;
	//debug(LOG_WZ, "sendBeacon: '%s'",pStr);

	//find machine that is hosting this human or AI
	sendPlayer = whosResponsible(forPlayer);

	if(sendPlayer >= MAX_PLAYERS)
	{
		debug(LOG_ERROR, "sendAIMessage() - whosResponsible() failed.");
		return false;
	}

	// I assume this is correct, looks like it sends it to ONLY that person, and the routine
	// kf_AddHelpBlip() itterates for each player it needs.
	NETbeginEncode(NET_BEACONMSG, sendPlayer);		// send to the player who is hosting 'to' player (might be himself if human and not AI)
	NETint32_t(&sender);                                // save the actual sender

	// save the actual player that is to get this msg on the source machine (source can host many AIs)
	NETint32_t(&forPlayer);                             // save the actual receiver (might not be the same as the one we are actually sending to, in case of AIs)
	NETint32_t(&locX);                                  // save location
	NETint32_t(&locY);

	// const_cast: need to cast away constness because of the const-incorrectness of NETstring (const-incorrect when sending/encoding a packet)
	NETstring((char *)pStr, MAX_CONSOLE_STRING_LENGTH); // copy message in.
	NETend();

	return true;
}

void displayAIMessage(char *pStr, int32_t from, int32_t to)
{
	char				tmp[255];

	if (isHumanPlayer(to))		//display text only if receiver is the (human) host machine itself
	{
		strcpy(tmp, getPlayerName(from));
		strcat(tmp, ": ");											// seperator
		strcat(tmp, pStr);											// add message

		addConsoleMessage(tmp, DEFAULT_JUSTIFY, from);
	}
}

// Write a message to the console.
BOOL recvTextMessage()
{
	uint32_t	playerIndex;
	char	msg[MAX_CONSOLE_STRING_LENGTH];

	memset(msg, 0x0, sizeof(msg));

	NETbeginDecode(NET_TEXTMSG);
	// Who this msg is from
	NETuint32_t(&playerIndex);
	// The message to send
	NETstring(msg, MAX_CONSOLE_STRING_LENGTH);
	NETend();

	if (whosResponsible(playerIndex) != NetMsg.source)
	{
		playerIndex = NetMsg.source;  // Fix corrupted playerIndex.
	}

	if (playerIndex >= MAX_PLAYERS)
	{
		return false;
	}

	addConsoleMessage(msg, DEFAULT_JUSTIFY, playerIndex);

	// Multiplayer message callback
	// Received a console message from a player, save
	MultiMsgPlayerFrom = playerIndex;
	MultiMsgPlayerTo = selectedPlayer;

	sstrcpy(MultiplayMsg, msg);
	eventFireCallbackTrigger((TRIGGER_TYPE)CALL_AI_MSG);

	// make some noise!
	if (titleMode == MULTIOPTION || titleMode == MULTILIMIT)
	{
		audio_PlayTrack(FE_AUDIO_MESSAGEEND);
	}
	else if (!ingame.localJoiningInProgress)
	{
		audio_PlayTrack(ID_SOUND_MESSAGEEND);
	}

	return true;
}

//AI multiplayer message - received message from AI (from scripts)
BOOL recvTextMessageAI()
{
	uint32_t	sender, receiver;
	char	msg[MAX_CONSOLE_STRING_LENGTH];
	char	newmsg[MAX_CONSOLE_STRING_LENGTH];

	NETbeginDecode(NET_AITEXTMSG);
	NETuint32_t(&sender);			//in-game player index ('normal' one)
	NETuint32_t(&receiver);			//in-game player index
	NETstring(newmsg, MAX_CONSOLE_STRING_LENGTH);
	NETend();

	if (whosResponsible(sender) != NetMsg.source)
	{
		sender = NetMsg.source;  // Fix corrupted sender.
	}

	//sstrcpy(msg, getPlayerName(sender));  // name
	//sstrcat(msg, " : ");                  // seperator
	sstrcpy(msg, newmsg);

	//Display the message and make the script callback
	displayAIMessage(msg, sender, receiver);

	//Received a console message from a player callback
	//store and call later
	//-------------------------------------------------
	if(!msgStackPush(CALL_AI_MSG, sender, receiver, msg, -1, -1, NULL))
	{
		debug(LOG_ERROR, "recvTextMessageAI() - msgStackPush - stack failed");
		return false;
	}

	return true;
}

bool recvPageSig(void)
{
	/*When we are beeped/paged.*/
	uint32_t PagedPlayer, Pager;

	NETbeginDecode(NET_PAGEPLAYER);
	NETuint32_t(&PagedPlayer);
	NETuint32_t(&Pager);

	if (PagedPlayer == selectedPlayer)
	{
		char TmpBuf[MAX_CONSOLE_STRING_LENGTH];

		snprintf(TmpBuf, MAX_CONSOLE_STRING_LENGTH, "Your attention is wanted by %s", NetPlay.players[Pager].name);

		addConsoleMessage(TmpBuf, DEFAULT_JUSTIFY, SYSTEM_MESSAGE);

		audio_QueueTrack(ID_SOUND_BUILD_FAIL);
		audio_QueueTrack(ID_SOUND_BUILD_FAIL);
		audio_QueueTrack(ID_SOUND_BUILD_FAIL);
	}
	else
	{
		debug(LOG_ERROR, "Player %d sent us a page signal for the wrong player. We are %d but they sent to %d.", Pager, selectedPlayer, PagedPlayer);
	}

	return NETend();
}

// ////////////////////////////////////////////////////////////////////////////
// Templates

// send a newly created template to other players
BOOL sendTemplate(DROID_TEMPLATE *pTempl)
{
	int i;
	uint8_t player = selectedPlayer;

	ASSERT(pTempl != NULL, "sendTemplate: Old Pumpkin bug");
	if (!pTempl)
	{
		return true;    /* hack */
	}

	NETbeginEncode(NET_TEMPLATE, NET_ALL_PLAYERS);
	NETuint8_t(&player);
	NETuint32_t(&pTempl->ref);
	NETstring(pTempl->aName, sizeof(pTempl->aName));
	NETuint8_t(&pTempl->NameVersion);

	for (i = 0; i < ARRAY_SIZE(pTempl->asParts); ++i)
	{
		// signed, but sent as a bunch of bits...
		NETint32_t(&pTempl->asParts[i]);
	}

	NETuint32_t(&pTempl->buildPoints);
	NETuint32_t(&pTempl->powerPoints);
	NETuint32_t(&pTempl->storeCount);
	NETuint32_t(&pTempl->numWeaps);

	for (i = 0; i < DROID_MAXWEAPS; i++)
	{
		NETuint32_t(&pTempl->asWeaps[i]);
	}

	NETuint32_t((uint32_t *)&pTempl->droidType);
	NETuint32_t(&pTempl->multiPlayerID);

	return NETend();
}

// receive a template created by another player
BOOL recvTemplate()
{
	uint8_t			player;
	DROID_TEMPLATE	*psTempl;
	DROID_TEMPLATE	t, *pT = &t;
	int				i;

	NETbeginDecode(NET_TEMPLATE);
	NETuint8_t(&player);
	ASSERT(player < MAX_PLAYERS, "recvtemplate: invalid player size: %d", player);

	NETuint32_t(&pT->ref);
	NETstring(pT->aName, sizeof(pT->aName));
	NETuint8_t(&pT->NameVersion);

	for (i = 0; i < ARRAY_SIZE(pT->asParts); ++i)
	{
		// signed, but sent as a bunch of bits...
		NETint32_t(&pT->asParts[i]);
	}

	NETuint32_t(&pT->buildPoints);
	NETuint32_t(&pT->powerPoints);
	NETuint32_t(&pT->storeCount);
	NETuint32_t(&pT->numWeaps);

	for (i = 0; i < DROID_MAXWEAPS; i++)
	{
		NETuint32_t(&pT->asWeaps[i]);
	}

	NETuint32_t((uint32_t *)&pT->droidType);
	NETuint32_t(&pT->multiPlayerID);
	NETend();

	t.psNext = NULL;
	t.pName = NULL;
	t.prefab = false;

	psTempl = IdToTemplate(t.multiPlayerID, player);

	// Already exists
	if (psTempl)
	{
		t.psNext = psTempl->psNext;
		memcpy(psTempl, &t, sizeof(DROID_TEMPLATE));
		debug(LOG_SYNC, "Updating MP template %d", (int)t.multiPlayerID);
	}
	else
	{
		addTemplate(player, &t);
		apsDroidTemplates[player]->ref = REF_TEMPLATE_START;
		debug(LOG_SYNC, "Creating MP template %d", (int)t.multiPlayerID);
	}

	return true;
}


// ////////////////////////////////////////////////////////////////////////////
// inform others that you no longer have a template

BOOL SendDestroyTemplate(DROID_TEMPLATE *t)
{
	uint8_t player = selectedPlayer;

	NETbeginEncode(NET_TEMPLATEDEST, NET_ALL_PLAYERS);
	NETuint8_t(&player);
	NETuint32_t(&t->multiPlayerID);
	NETend();

	return true;
}

// acknowledge another player no longer has a template
static BOOL recvDestroyTemplate()
{
	uint8_t			player;
	uint32_t		templateID;
	DROID_TEMPLATE	*psTempl, *psTempPrev = NULL;

	NETbeginDecode(NET_TEMPLATEDEST);
	NETuint8_t(&player);
	NETuint32_t(&templateID);
	NETend();

	// Find the template in the list
	for (psTempl = apsDroidTemplates[player]; psTempl; psTempl = psTempl->psNext)
	{
		if (psTempl->multiPlayerID == templateID)
		{
			break;
		}

		psTempPrev = psTempl;
	}

	// If we found it then delete it
	if (psTempl)
	{
		// Update the linked list
		if (psTempPrev)
		{
			psTempPrev->psNext = psTempl->psNext;
		}
		else
		{
			apsDroidTemplates[player] = psTempl->psNext;
		}

		// Delete the template
		free(psTempl);
	}

	return true;
}


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Features

// send a destruct feature message.
BOOL SendDestroyFeature(FEATURE *pF)
{
	NETbeginEncode(NET_FEATUREDEST, NET_ALL_PLAYERS);
	NETuint32_t(&pF->id);
	return NETend();
}

// process a destroy feature msg.
BOOL recvDestroyFeature()
{
	FEATURE *pF;
	uint32_t	id;

	NETbeginDecode(NET_FEATUREDEST);
	NETuint32_t(&id);
	NETend();

	pF = IdToFeature(id, ANYPLAYER);
	if (pF == NULL)
	{
		debug(LOG_FEATURE, "feature id %d not found (probably already destroyed)", id);
		return false;
	}

	debug(LOG_FEATURE, "p%d feature id %d destroyed (%s)", pF->player, pF->id, pF->psStats->pName);
	// Remove the feature locally
	turnOffMultiMsg(true);
	removeFeature(pF);
	turnOffMultiMsg(false);

	return true;
}

// ////////////////////////////////////////////////////////////////////////////
// Network File packet processor.
BOOL recvMapFileRequested()
{
	char mapStr[2][256], mapName[256];
	uint32_t player;

	PHYSFS_sint64 fileSize_64;
	PHYSFS_file	*pFileHandle;

	if(!NetPlay.isHost)				// only host should act
	{
		ASSERT(false, "Host only routine detected for client!");
		return false;
	}

	//	Check to see who wants the file
	NETbeginDecode(NET_FILE_REQUESTED);
	NETuint32_t(&player);
	NETend();

	if (!NetPlay.players[player].wzFile.isSending)
	{
		BOOL Classic = false;
		
		NetPlay.players[player].needFile = true;
		NetPlay.players[player].wzFile.isCancelled = false;
		NetPlay.players[player].wzFile.isSending = true;


		memset(mapStr, 0, 256);
		memset(mapName, 0, 256);

		addConsoleMessage("Map was requested: SENDING MAP!", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);

		sstrcpy(mapName, game.map);
		if (	strstr(mapName, "-T1") != 0
				|| strstr(mapName, "-T2") != 0
				|| strstr(mapName, "-T3") != 0)
		{
			// chop off the -T1 *only when needed!*
			mapName[strlen(game.map) - 3] = 0;		// chop off the -T1 etc..
		}
		// chop off the sk- if required.
		if(strncmp(mapName, "Sk-", 3) == 0)
		{
			char Tmp[sizeof mapName];
			sstrcpy(Tmp, mapName + 3);
			sstrcpy(mapName, Tmp);
		}

		/*One for Legacy maps, one for classic WZ maps.*/
		snprintf(mapStr[0], sizeof(mapStr[0]), "maps/%dc-%s.wzl", game.maxPlayers, mapName);
		snprintf(mapStr[1], sizeof(mapStr[1]), "maps/%dc-%s.wz", game.maxPlayers, mapName);

		debug(LOG_NET, "Map was requested. Looking for %dc-%s", game.maxPlayers, mapName);

		// Checking to see if file is available...
		if (!(pFileHandle = PHYSFS_openRead(mapStr[0])) && !(Classic = true, pFileHandle = PHYSFS_openRead(mapStr[1])))
		{
			debug(LOG_ERROR, "Failed to open %s for reading: %dc-%s, error %s", mapStr[Classic], game.maxPlayers, mapName, PHYSFS_getLastError());
			debug(LOG_FATAL, "You have a map (%dc-%s) that can't be located.\n\n"
				  "Make sure it is in the correct directory and or format! (No map packs!)", game.maxPlayers, mapName);
			// NOTE: if we get here, then the game is basically over, The host can't send the file for whatever reason...
			// Which also means, that we can't continue.
			debug(LOG_NET, "***Host has a file issue, and is being forced to quit!***");
			NETbeginEncode(NET_HOST_DROPPED, NET_ALL_PLAYERS);
			NETend();
			abort();
		}

		// get the file's size.
		fileSize_64 = PHYSFS_fileLength(pFileHandle);
		debug(LOG_NET, "File is valid, sending [directory: %s] %s to client %u", PHYSFS_getRealDir(mapStr[Classic]),
			  mapStr[Classic], player);

		NetPlay.players[player].wzFile.pFileHandle = pFileHandle;
		NetPlay.players[player].wzFile.fileSize_32 = (int32_t) fileSize_64;		//we don't support 64bit int nettypes.
		NetPlay.players[player].wzFile.currPos = 0;

		NETsendFile(mapStr[Classic], player);
	}
	return true;
}

// continue sending the map
void sendMap(void)
{
	int i = 0;
	uint8_t done;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (NetPlay.players[i].wzFile.isSending)
		{
			done = NETsendFile(game.map, i);
			if (done == 100)
			{
				addConsoleMessage("MAP SENT!", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);
				debug(LOG_NET, "=== File has been sent to player %d ===", i);
				NetPlay.players[i].wzFile.isSending = false;
				NetPlay.players[i].needFile = false;
			}
		}
	}
}

// Another player is broadcasting a map, recv a chunk. Returns false if not yet done.
BOOL recvMapFileData()
{
	mapDownloadProgress = NETrecvFile();
	if (mapDownloadProgress == 100)
	{
		char TBuf[128];

		addConsoleMessage("MAP DOWNLOADED!", DEFAULT_JUSTIFY, SYSTEM_MESSAGE);

		snprintf(TBuf, 128, "%s: MAP DOWNLOADED!", getPlayerName(selectedPlayer));
		sendTextMessage(TBuf, true);					//send

		debug(LOG_NET, "=== File has been received. ===");

		// clear out the old level list.
		levShutDown();
		levInitialise();
		rebuildSearchPath(mod_multiplay, true);	// MUST rebuild search path for the new maps we just got!
		if (!buildMapList())
		{
			return false;
		}
		return true;
	}

	return false;
}


//------------------------------------------------------------------------------------------------//

/* multiplayer message stack */
void msgStackReset(void)
{
	msgStackPos = -1;		//Beginning of the stack
}

uint32_t msgStackPush(int32_t CBtype, int32_t plFrom, int32_t plTo, const char *tStr, int32_t x, int32_t y, DROID *psDroid)
{
	debug(LOG_WZ, "msgStackPush: pushing message type %d to pos %d", CBtype, msgStackPos + 1);

	if (msgStackPos >= MAX_MSG_STACK)
	{
		debug(LOG_ERROR, "msgStackPush() - stack full");
		return false;
	}

	//make point to the last valid element
	msgStackPos++;

	//remember values
	msgPlFrom[msgStackPos] = plFrom;
	msgPlTo[msgStackPos] = plTo;

	callbackType[msgStackPos] = CBtype;
	locx[msgStackPos] = x;
	locy[msgStackPos] = y;

	strcpy(msgStr[msgStackPos], tStr);

	msgDroid[msgStackPos] = psDroid;

	return true;
}

BOOL isMsgStackEmpty(void)
{
	if(msgStackPos <= (-1))
	{
		return true;
	}
	return false;
}

BOOL msgStackGetFrom(int32_t  *psVal)
{
	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackGetFrom: msgStackPos < 0");
		return false;
	}

	*psVal = msgPlFrom[0];

	return true;
}

BOOL msgStackGetTo(int32_t  *psVal)
{
	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackGetTo: msgStackPos < 0");
		return false;
	}

	*psVal = msgPlTo[0];

	return true;
}

static BOOL msgStackGetCallbackType(int32_t  *psVal)
{
	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackGetCallbackType: msgStackPos < 0");
		return false;
	}

	*psVal = callbackType[0];

	return true;
}

static BOOL msgStackGetXY(int32_t  *psValx, int32_t  *psValy)
{
	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackGetXY: msgStackPos < 0");
		return false;
	}

	*psValx = locx[0];
	*psValy = locy[0];

	return true;
}


BOOL msgStackGetMsg(char  *psVal)
{
	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackGetMsg: msgStackPos < 0");
		return false;
	}

	strcpy(psVal, msgStr[0]);
	//*psVal = msgPlTo[msgStackPos];

	return true;
}

static BOOL msgStackSort(void)
{
	int32_t i;

	//go through all-1 elements (bottom-top)
	for(i = 0; i < msgStackPos; i++)
	{
		msgPlFrom[i] = msgPlFrom[i + 1];
		msgPlTo[i] = msgPlTo[i + 1];

		callbackType[i] = callbackType[i + 1];
		locx[i] = locx[i + 1];
		locy[i] = locy[i + 1];

		strcpy(msgStr[i], msgStr[i + 1]);
	}

	//erase top element
	msgPlFrom[msgStackPos] = -2;
	msgPlTo[msgStackPos] = -2;

	callbackType[msgStackPos] = -2;
	locx[msgStackPos] = -2;
	locy[msgStackPos] = -2;

	strcpy(msgStr[msgStackPos], "ERROR char!!!!!!!!");

	msgStackPos--;		//since removed the top element

	return true;
}

BOOL msgStackPop(void)
{
	debug(LOG_WZ, "msgStackPop: stack size %d", msgStackPos);

	if(msgStackPos < 0 || msgStackPos > MAX_MSG_STACK)
	{
		debug(LOG_ERROR, "msgStackPop: wrong msgStackPos index: %d", msgStackPos);
		return false;
	}

	return msgStackSort();		//move all elements 1 pos lower
}

BOOL msgStackGetDroid(DROID **ppsDroid)
{
	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackGetDroid: msgStackPos < 0");
		return false;
	}

	*ppsDroid = msgDroid[0];

	return true;
}

int32_t msgStackGetCount(void)
{
	return msgStackPos + 1;
}

BOOL msgStackFireTop(void)
{
	int32_t		_callbackType;
	char		msg[255];

	if(msgStackPos < 0)
	{
		debug(LOG_ERROR, "msgStackFireTop: msgStackPos < 0");
		return false;
	}

	if(!msgStackGetCallbackType(&_callbackType))
	{
		return false;
	}

	switch(_callbackType)
	{
		case CALL_VIDEO_QUIT:
			debug(LOG_SCRIPT, "msgStackFireTop: popped CALL_VIDEO_QUIT");
			eventFireCallbackTrigger((TRIGGER_TYPE)CALL_VIDEO_QUIT);
			break;

		case CALL_DORDER_STOP:
			ASSERT(false, "CALL_DORDER_STOP is currently disabled");

			debug(LOG_SCRIPT, "msgStackFireTop: popped CALL_DORDER_STOP");

			if(!msgStackGetDroid(&psScrCBOrderDroid))
			{
				return false;
			}

			eventFireCallbackTrigger((TRIGGER_TYPE)CALL_DORDER_STOP);
			break;

		case CALL_BEACON:

			if(!msgStackGetXY(&beaconX, &beaconY))
			{
				return false;
			}

			if(!msgStackGetFrom(&MultiMsgPlayerFrom))
			{
				return false;
			}

			if(!msgStackGetTo(&MultiMsgPlayerTo))
			{
				return false;
			}

			if(!msgStackGetMsg(msg))
			{
				return false;
			}

			strcpy(MultiplayMsg, msg);

			eventFireCallbackTrigger((TRIGGER_TYPE)CALL_BEACON);
			break;

		case CALL_AI_MSG:
			if(!msgStackGetFrom(&MultiMsgPlayerFrom))
			{
				return false;
			}

			if(!msgStackGetTo(&MultiMsgPlayerTo))
			{
				return false;
			}

			if(!msgStackGetMsg(msg))
			{
				return false;
			}

			strcpy(MultiplayMsg, msg);

			eventFireCallbackTrigger((TRIGGER_TYPE)CALL_AI_MSG);
			break;

		default:
			debug(LOG_ERROR, "msgStackFireTop: unknown callback type");
			return false;
			break;
	}

	if(!msgStackPop())
	{
		return false;
	}

	return true;
}

static BOOL recvBeacon(void)
{
	int32_t sender, receiver, locX, locY;
	char    msg[MAX_CONSOLE_STRING_LENGTH];

	NETbeginDecode(NET_BEACONMSG);
	NETint32_t(&sender);            // the actual sender
	NETint32_t(&receiver);          // the actual receiver (might not be the same as the one we are actually sending to, in case of AIs)
	NETint32_t(&locX);
	NETint32_t(&locY);
	NETstring(msg, sizeof(msg));    // Receive the actual message
	NETend();

	debug(LOG_WZ, "Received beacon for player: %d, from: %d", receiver, sender);

	sstrcat(msg, NetPlay.players[sender].name);    // name
	sstrcpy(beaconReceiveMsg[sender], msg);

	return addBeaconBlip(locX, locY, receiver, sender, beaconReceiveMsg[sender]);
}

const char *getPlayerColourName(unsigned int player)
{
	static const char *playerColors[] =
	{
		N_("Green"),
		N_("Orange"),
		N_("Grey"),
		N_("Black"),
		N_("Red"),
		N_("Blue"),
		N_("Pink"),
		N_("Cyan")
	};

	ASSERT(player < ARRAY_SIZE(playerColors), "player number (%d) exceeds maximum (%lu)", player, (unsigned long) ARRAY_SIZE(playerColors));

	if (player >= ARRAY_SIZE(playerColors))
	{
		return "";
	}

	return gettext(playerColors[getPlayerColour(player)]);
}

/* Reset ready status for all players */
void resetReadyStatus(bool bSendOptions)
{
	unsigned int player;

	for(player = 0; player < MAX_PLAYERS; player++)
	{
		bPlayerReadyGUI[player] = false;
	}

	// notify all clients if needed
	if(bSendOptions)
	{
		sendOptions();
	}
	netPlayersUpdated = true;
}
