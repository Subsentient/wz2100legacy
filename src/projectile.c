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
/***************************************************************************/
/*
 * Projectile functions
 *
 */
/***************************************************************************/
#include <string.h>

#include "lib/framework/frame.h"
#include "lib/framework/trig.h"
#include "lib/framework/math_ext.h"

#include "lib/gamelib/gtime.h"
#include "objects.h"
#include "move.h"
#include "action.h"
#include "combat.h"
#include "effects.h"
#include "map.h"
#include "lib/sound/audio_id.h"
#include "lib/sound/audio.h"
#include "anim_id.h"
#include "projectile.h"
#include "visibility.h"
#include "lib/script/script.h"
#include "scripttabs.h"
#include "scriptcb.h"
#include "group.h"
#include "cmddroid.h"
#include "feature.h"
#include "lib/ivis_common/piestate.h"
#include "loop.h"
// FIXME Direct iVis implementation include!
#include "lib/ivis_opengl/piematrix.h"

#include "scores.h"

#include "display3d.h"
#include "display.h"
#include "multiplay.h"
#include "multistat.h"
#include "mapgrid.h"

#define	PROJ_MAX_PITCH			30
#define	DIRECT_PROJ_SPEED		500
#define VTOL_HITBOX_MODIFICATOR 100

typedef struct _interval
{
	int begin, end;  // Time 1 = 0, time 2 = 1024. Or begin >= end if empty.
} INTERVAL;

// Watermelon:they are from droid.c
/* The range for neighbouring objects */
#define PROJ_NAYBOR_RANGE		(TILE_UNITS*4)
// used to create a specific ID for projectile objects to facilitate tracking them.
static const uint32_t ProjectileTrackerID =	0xdead0000;
// Watermelon:neighbour global info ripped from droid.c
static PROJ_NAYBOR_INFO	asProjNaybors[MAX_NAYBORS];
static uint32_t		numProjNaybors = 0;

static BASE_OBJECT	*CurrentProjNaybors = NULL;
static uint32_t	projnayborTime = 0;

/* The list of projectiles in play */
static PROJECTILE *psProjectileList = NULL;

/* The next projectile to give out in the proj_First / proj_Next methods */
static PROJECTILE *psProjectileNext = NULL;

/***************************************************************************/

// the last unit that did damage - used by script functions
BASE_OBJECT		*g_pProjLastAttacker;

/***************************************************************************/

static uint32_t	establishTargetRadius( BASE_OBJECT *psTarget );
static uint32_t	establishTargetHeight( BASE_OBJECT *psTarget );
static void	proj_ImpactFunc( PROJECTILE *psObj );
static void	proj_PostImpactFunc( PROJECTILE *psObj );
static void	proj_checkBurnDamage( BASE_OBJECT *apsList, PROJECTILE *psProj);
static void	proj_Free(PROJECTILE *psObj);

static float objectDamage(BASE_OBJECT *psObj, uint32_t damage, uint32_t weaponClass, uint32_t weaponSubClass, HIT_SIDE impactSide);
static HIT_SIDE getHitSide (PROJECTILE *psObj, BASE_OBJECT *psTarget);

static void projGetNaybors(PROJECTILE *psObj);


/***************************************************************************/
BOOL gfxVisible(PROJECTILE *psObj)
{
	// Already know it is visible
	if (psObj->bVisible)
	{
		return true;
	}

	// You fired it
	if (psObj->player == selectedPlayer)
	{
		return true;
	}

	// Someone elses structure firing at something you can't see
	if (psObj->psSource != NULL
			&& !psObj->psSource->died
			&& psObj->psSource->type == OBJ_STRUCTURE
			&& psObj->psSource->player != selectedPlayer
			&& (psObj->psDest == NULL
				|| psObj->psDest->died
				|| !psObj->psDest->visible[selectedPlayer]))
	{
		return false;
	}

	// Something you cannot see firing at a structure that isn't yours
	if (psObj->psDest != NULL
			&& !psObj->psDest->died
			&& psObj->psDest->type == OBJ_STRUCTURE
			&& psObj->psDest->player != selectedPlayer
			&& (psObj->psSource == NULL
				|| !psObj->psSource->visible[selectedPlayer]))
	{
		return false;
	}

	// You can see the source
	if (psObj->psSource != NULL
			&& !psObj->psSource->died
			&& psObj->psSource->visible[selectedPlayer])
	{
		return true;
	}

	// You can see the destination
	if (psObj->psDest != NULL
			&& !psObj->psDest->died
			&& psObj->psDest->visible[selectedPlayer])
	{
		return true;
	}

	return false;
}

/***************************************************************************/

BOOL
proj_InitSystem( void )
{
	psProjectileList = NULL;
	psProjectileNext = NULL;

	return true;
}

/***************************************************************************/

// Clean out all projectiles from the system, and properly decrement
// all reference counts.
void
proj_FreeAllProjectiles( void )
{
	PROJECTILE *psCurr = psProjectileList, *psPrev = NULL;

	while (psCurr)
	{
		psPrev = psCurr;
		psCurr = psCurr->psNext;
		proj_Free(psPrev);
	}

	psProjectileList = NULL;
	psProjectileNext = NULL;
}

/***************************************************************************/

BOOL
proj_Shutdown( void )
{
	proj_FreeAllProjectiles();

	return true;
}

/***************************************************************************/

// Free the memory held by a projectile, and decrement its reference counts,
// if any. Do not call directly on a projectile in a list, because then the
// list will be broken!
static void proj_Free(PROJECTILE *psObj)
{
	/* Decrement any reference counts the projectile may have increased */
	free(psObj->psDamaged);
	setProjectileSource(psObj, NULL);
	setProjectileDestination(psObj, NULL);

	free(psObj);
}

/***************************************************************************/

// Reset the first/next methods, and give out the first projectile in the list.
PROJECTILE *
proj_GetFirst( void )
{
	psProjectileNext = psProjectileList;
	return psProjectileList;
}

/***************************************************************************/

// Get the next projectile
PROJECTILE *
proj_GetNext( void )
{
	psProjectileNext = psProjectileNext->psNext;
	return psProjectileNext;
}

/***************************************************************************/

/*
 * Relates the quality of the attacker to the quality of the victim.
 * The value returned satisfies the following inequality: 0.5 <= ret <= 2.0
 */
static float QualityFactor(DROID *psAttacker, DROID *psVictim)
{
	float powerRatio = calcDroidPower(psVictim) / calcDroidPower(psAttacker);
	float pointsRatio = calcDroidPoints(psVictim) / calcDroidPoints(psAttacker);

	CLIP(powerRatio, 0.5, 2.0);
	CLIP(pointsRatio, 0.5, 2.0);

	return (powerRatio + pointsRatio) / 2.0;
}

// update the kills after a target is damaged/destroyed
static void proj_UpdateKills(PROJECTILE *psObj, float experienceInc)
{
	DROID	        *psDroid;
	BASE_OBJECT     *psSensor;

	CHECK_PROJECTILE(psObj);

	if (psObj->psSource == NULL || (psObj->psDest && psObj->psDest->type == OBJ_FEATURE)
			|| (psObj->psDest && psObj->psSource->player == psObj->psDest->player))	// no exp for friendly fire
	{
		return;
	}

	// If experienceInc is negative then the target was killed
	if (bMultiPlayer && experienceInc < 0.0f)
	{
		updateMultiStatsKills(psObj->psDest, psObj->psSource->player);
	}

	// Since we are no longer interested if it was killed or not, abs it
	experienceInc = fabs(experienceInc);

	if (psObj->psSource->type == OBJ_DROID)			/* update droid kills */
	{
		psDroid = (DROID *) psObj->psSource;

		// If it is 'droid-on-droid' then modify the experience by the Quality factor
		// Only do this in MP so to not un-balance the campaign
		if (psObj->psDest != NULL
				&& psObj->psDest->type == OBJ_DROID
				&& bMultiPlayer)
		{
			// Modify the experience gained by the 'quality factor' of the units
			experienceInc *= QualityFactor(psDroid, (DROID *) psObj->psDest);
		}

		ASSERT_OR_RETURN(, -0.1 < experienceInc && experienceInc < 2.1, "Experience increase out of range");

		psDroid->experience += experienceInc;
		cmdDroidUpdateKills(psDroid, experienceInc);

		psSensor = orderStateObj(psDroid, DORDER_FIRESUPPORT);
		if (psSensor
				&& psSensor->type == OBJ_DROID)
		{
			((DROID *) psSensor)->experience += experienceInc;
		}
	}
	else if (psObj->psSource->type == OBJ_STRUCTURE)
	{
		ASSERT_OR_RETURN(, -0.1 < experienceInc && experienceInc < 2.1, "Experience increase out of range");

		// See if there was a command droid designating this target
		psDroid = cmdDroidGetDesignator(psObj->psSource->player);

		if (psDroid != NULL
				&& psDroid->action == DACTION_ATTACK
				&& psDroid->psActionTarget[0] == psObj->psDest)
		{
			psDroid->experience += experienceInc;
		}
	}
}

/***************************************************************************/

BOOL proj_SendProjectile(WEAPON *psWeap, BASE_OBJECT *psAttacker, int player, Vector3i target, BASE_OBJECT *psTarget, BOOL bVisible, int weapon_slot)
{
	PROJECTILE		*psProj = malloc(sizeof(PROJECTILE));
	int32_t			tarHeight, srcHeight, iMinSq;
	int32_t			altChange, dx, dy, dz, iVelSq, iVel;
	double          fR, fA, fS, fT, fC;
	Vector3f muzzle;
	int32_t			iRadSq, iPitchLow, iPitchHigh, iTemp;
	WEAPON_STATS *psStats = &asWeaponStats[psWeap->nStat];

	ASSERT_OR_RETURN( false, psWeap->nStat < numWeaponStats, "Invalid range referenced for numWeaponStats, %d > %d", psWeap->nStat, numWeaponStats);
	ASSERT_OR_RETURN( false, psStats != NULL, "Invalid weapon stats" );
	ASSERT_OR_RETURN( false, psTarget == NULL || !psTarget->died, "Aiming at dead target!" );

	/* get muzzle offset */
	if (psAttacker == NULL)
	{
		// if there isn't an attacker just start at the target position
		// NB this is for the script function to fire the las sats
		muzzle = Vector3f_Init(target.x, target.y, target.z);
	}
	else if (psAttacker->type == OBJ_DROID && weapon_slot >= 0)
	{
		calcDroidMuzzleLocation( (DROID *) psAttacker, &muzzle, weapon_slot);
		/*update attack runs for VTOL droid's each time a shot is fired*/
		updateVtolAttackRun((DROID *)psAttacker, weapon_slot);
	}
	else if (psAttacker->type == OBJ_STRUCTURE && weapon_slot >= 0)
	{
		calcStructureMuzzleLocation( (STRUCTURE *) psAttacker, &muzzle, weapon_slot);
	}
	else // incase anything wants a projectile
	{
		// FIXME HACK Needed since we got those ugly Vector3uw floating around in BASE_OBJECT...
		muzzle = Vector3uw_To3f(psAttacker->pos);
	}

	/* Initialise the structure */
	psProj->id			= ProjectileTrackerID | (gameTime2 >> 4);		// make unique id
	psProj->type		    = OBJ_PROJECTILE;
	psProj->psWStats		= psStats;

	psProj->pos = Vector3f_To3uw(muzzle);
	psProj->startX		= muzzle.x;
	psProj->startY		= muzzle.y;
	psProj->tarX			= target.x;
	psProj->tarY			= target.y;

	psProj->player = player;
	psProj->bVisible = false;

	psProj->died = 0;

	setProjectileDestination(psProj, psTarget);

	/*
	When we have been created by penetration (spawned from another projectile),
	we shall live no longer than the original projectile may have lived
	*/
	if (psAttacker && psAttacker->type == OBJ_PROJECTILE)
	{
		PROJECTILE *psOldProjectile = (PROJECTILE *)psAttacker;
		psProj->born = psOldProjectile->born;

		setProjectileSource(psProj, psOldProjectile->psSource);
		psProj->psDamaged = (BASE_OBJECT **)malloc(psOldProjectile->psNumDamaged * sizeof(BASE_OBJECT *));
		psProj->psNumDamaged = psOldProjectile->psNumDamaged;
		memcpy(psProj->psDamaged, psOldProjectile->psDamaged, psOldProjectile->psNumDamaged * sizeof(BASE_OBJECT *));
	}
	else
	{
		psProj->born = gameTime;

		setProjectileSource(psProj, psAttacker);
		psProj->psDamaged = NULL;
		psProj->psNumDamaged = 0;
	}

	if (psTarget)
	{
		const float maxHeight = establishTargetHeight(psTarget);
		unsigned int heightVariance = frandom() * maxHeight;

		scoreUpdateVar(WD_SHOTS_ON_TARGET);

		tarHeight = psTarget->pos.z + heightVariance;
	}
	else
	{
		tarHeight = target.z;
		scoreUpdateVar(WD_SHOTS_OFF_TARGET);
	}

	srcHeight			= muzzle.z;
	altChange			= tarHeight - srcHeight;

	psProj->srcHeight	= srcHeight;
	psProj->altChange	= altChange;

	dx = ((int32_t)psProj->tarX) - muzzle.x;
	dy = ((int32_t)psProj->tarY) - muzzle.y;
	dz = tarHeight - muzzle.z;

	/* roll never set */
	psProj->roll = 0;

	fR = atan2(dx, dy);
	if ( fR < 0.0 )
	{
		fR += 2.0 * M_PI;
	}
	psProj->direction = RAD_TO_DEG(fR);


	/* get target distance */
	iRadSq = dx * dx + dy * dy + dz * dz;
	fR = trigIntSqrt( iRadSq );
	iMinSq = psStats->minRange * psStats->minRange;

	if ( proj_Direct(psStats) ||
			( !proj_Direct(psStats) && (iRadSq <= iMinSq) ) )
	{
		fR = atan2(dz, fR);
		if ( fR < 0.0 )
		{
			fR += 2.0 * M_PI;
		}
		psProj->pitch = (int16_t)( RAD_TO_DEG(fR) );
		psProj->state = PROJ_INFLIGHTDIRECT;
	}
	else
	{
		/* indirect */
		iVelSq = psStats->flightSpeed * psStats->flightSpeed;

		fA = ACC_GRAVITY * (double)iRadSq / (2.0 * iVelSq);
		fC = 4.0 * fA * (dz + fA);
		fS = (double)iRadSq - fC;

		/* target out of range - increase velocity to hit target */
		if ( fS < 0.0 )
		{
			/* set optimal pitch */
			psProj->pitch = PROJ_MAX_PITCH;

			fS = trigSin(PROJ_MAX_PITCH);
			fC = trigCos(PROJ_MAX_PITCH);
			fT = fS / fC;
			fS = ACC_GRAVITY * (1. + fT * fT);
			fS = fS / (2.0 * (fR * fT - dz));
			{
				iVel = trigIntSqrt(fS * (fR * fR));
			}
		}
		else
		{
			/* set velocity to stats value */
			iVel = psStats->flightSpeed;

			/* get floating point square root */
			fS = trigIntSqrt(fS);

			fT = atan2(fR + fS, 2.0 * fA);

			/* make sure angle positive */
			if ( fT < 0.0 )
			{
				fT += 2.0 * M_PI;
			}
			iPitchLow = RAD_TO_DEG(fT);

			fT = atan2(fR - fS, 2.0 * fA);
			/* make sure angle positive */
			if ( fT < 0.0 )
			{
				fT += 2.0 * M_PI;
			}
			iPitchHigh = RAD_TO_DEG(fT);

			/* swap pitches if wrong way round */
			if ( iPitchLow > iPitchHigh )
			{
				iTemp = iPitchHigh;
				iPitchLow = iPitchHigh;
				iPitchHigh = iTemp;
			}

			/* chooselow pitch unless -ve */
			if ( iPitchLow > 0 )
			{
				psProj->pitch = (int16_t)iPitchLow;
			}
			else
			{
				psProj->pitch = (int16_t)iPitchHigh;
			}
		}

		/* if droid set muzzle pitch */
		//Watermelon:fix turret pitch for more turrets
		if (psAttacker != NULL && weapon_slot >= 0)
		{
			if (psAttacker->type == OBJ_DROID)
			{
				((DROID *) psAttacker)->asWeaps[weapon_slot].pitch = psProj->pitch;
			}
			else if (psAttacker->type == OBJ_STRUCTURE)
			{
				((STRUCTURE *) psAttacker)->asWeaps[weapon_slot].pitch = psProj->pitch;
			}
		}

		psProj->vXY = iVel * trigCos(psProj->pitch);
		psProj->vZ  = iVel * trigSin(psProj->pitch);

		psProj->state = PROJ_INFLIGHTINDIRECT;
	}

	/* put the projectile object first in the global list */
	psProj->psNext = psProjectileList;
	psProjectileList = psProj;

	/* play firing audio */
	// only play if either object is visible, i know it's a bit of a hack, but it avoids the problem
	// of having to calculate real visibility values for each projectile.
	if ( bVisible || gfxVisible(psProj) )
	{
		// note that the projectile is visible
		psProj->bVisible = true;

		if ( psStats->iAudioFireID != NO_SOUND )
		{

			if ( psProj->psSource )
			{
				/* firing sound emitted from source */
				audio_PlayObjDynamicTrack( (BASE_OBJECT *) psProj->psSource,
										   psStats->iAudioFireID, NULL );
				/* GJ HACK: move howitzer sound with shell */
				if ( psStats->weaponSubClass == WSC_HOWITZERS )
				{
					audio_PlayObjDynamicTrack( (BASE_OBJECT *) psProj,
											   ID_SOUND_HOWITZ_FLIGHT, NULL );
				}
			}
			//don't play the sound for a LasSat in multiPlayer
			else if (!(bMultiPlayer && psStats->weaponSubClass == WSC_LAS_SAT))
			{
				audio_PlayObjStaticTrack(psProj, psStats->iAudioFireID);
			}
		}
	}

	if ((psAttacker != NULL) && !proj_Direct(psStats))
	{
		//check for Counter Battery Sensor in range of target
		counterBatteryFire(psAttacker, psTarget);
	}

	CHECK_PROJECTILE(psProj);

	return true;
}

/***************************************************************************/

static INTERVAL intervalIntersection(INTERVAL i1, INTERVAL i2)
{
	INTERVAL ret = {MAX(i1.begin, i2.begin), MIN(i1.end, i2.end)};
	return ret;
}

static bool intervalEmpty(INTERVAL i)
{
	return i.begin >= i.end;
}

static INTERVAL collisionZ(int32_t z1, int32_t z2, int32_t height)
{
	INTERVAL ret = { -1, -1};
	if (z1 > z2)
	{
		z1 *= -1;
		z2 *= -1;
	}

	if (z1 > height || z2 < -height)
	{
		return ret;    // No collision between time 1 and time 2.
	}

	if (z1 == z2)
	{
		if (z1 >= -height && z1 <= height)
		{
			ret.begin = 0;
			ret.end = 1024;
		}
		return ret;
	}

	ret.begin = 1024 * (-height - z1) / (z2 - z1);
	ret.end   = 1024 * ( height - z1) / (z2 - z1);
	return ret;
}

static INTERVAL collisionXY(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t radius)
{
	// Solve (1 - t)v1 + t v2 = r.
	int32_t dx = x2 - x1, dy = y2 - y1;
	int32_t a = dx * dx + dy * dy;              // a = (v2 - v1)²
	float   b = x1 * dx + y1 * dy;              // b = v1(v2 - v1)
	float   c = x1 * x1 + y1 * y1 - radius * radius; // c = v1² - r²
	// Equation to solve is now a t^2 + 2 b t + c = 0.
	float   d = b * b - a * c;                  // d = b² - a c
	float sd;
	// Solution is (-b ± √d)/a.
	INTERVAL empty = { -1, -1};
	INTERVAL full = {0, 1024};
	INTERVAL ret;
	if (d < 0)
	{
		return empty;  // Missed.
	}
	if (a == 0)
	{
		return c < 0 ? full : empty;  // Not moving. See if inside the target.
	}

	sd = sqrtf(d);
	ret.begin = MAX(   0, 1024 * (-b - sd) / a);
	ret.end   = MIN(1024, 1024 * (-b + sd) / a);
	return ret;
}

static int32_t collisionXYZ(Vector3i v1, Vector3i v2, int32_t radius, int32_t height)
{
	INTERVAL iz = collisionZ(v1.z, v2.z, height);
	if (!intervalEmpty(iz))  // Don't bother checking x and y unless z passes.
	{
		INTERVAL i = intervalIntersection(iz, collisionXY(v1.x, v1.y, v2.x, v2.y, radius));
		if (!intervalEmpty(i))
		{
			return MAX(0, i.begin);
		}
	}
	return -1;
}

static void proj_InFlightFunc(PROJECTILE *psProj, bool bIndirect)
{
	/* we want a delay between Las-Sats firing and actually hitting in multiPlayer
	magic number but that's how long the audio countdown message lasts! */
	const unsigned int LAS_SAT_DELAY = 4;
	int timeSoFar, nextPosZ;
	int distancePercent; /* How far we are 0..100 */
	float distanceRatio; /* How far we are, 1.0==at target */
	float distanceExtensionFactor; /* Extended lifespan */
	Vector3i move;
	unsigned int i, j;
	// Projectile is missile:
	bool bMissile = false;
	WEAPON_STATS *psStats;
	Vector3uw prevPos, nextPos;
	unsigned int targetDistance, currentDistance;
	int32_t closestCollision = 1 << 30;
	BASE_OBJECT *closestCollisionObject = NULL;
	Vector3uw closestCollisionPos;

	CHECK_PROJECTILE(psProj);

	timeSoFar = gameTime - psProj->born;

	psStats = psProj->psWStats;
	ASSERT_OR_RETURN( , psStats != NULL, "Invalid weapon stats pointer");

	/* we want a delay between Las-Sats firing and actually hitting in multiPlayer
	magic number but that's how long the audio countdown message lasts! */
	if (bMultiPlayer && psStats->weaponSubClass == WSC_LAS_SAT &&
			timeSoFar < LAS_SAT_DELAY * GAME_TICKS_PER_SEC)
	{
		return;
	}

	/* Calculate extended lifespan where appropriate */
	switch (psStats->weaponSubClass)
	{
		case WSC_MGUN:
		case WSC_COMMAND:
			distanceExtensionFactor = 1.2f;
			break;
		case WSC_CANNON:
		case WSC_BOMB:
		case WSC_ELECTRONIC:
		case WSC_EMP:
		case WSC_FLAME:
		case WSC_ENERGY:
		case WSC_GAUSS:
			distanceExtensionFactor = 1.5f;
			break;
		case WSC_AAGUN: // No extended distance
			distanceExtensionFactor = 1.0f;
			break;
		case WSC_ROCKET:
		case WSC_MISSILE:
		case WSC_SLOWROCKET:
		case WSC_SLOWMISSILE:
			bMissile = true; // Take the same extended targetDistance as artillery
		case WSC_COUNTER:
		case WSC_MORTARS:
		case WSC_HOWITZERS:
		case WSC_LAS_SAT:
			distanceExtensionFactor = 1.5f;
			break;
		default:
			// WSC_NUM_WEAPON_SUBCLASSES
			/* Uninitialized "marker", this can be used as a
			 * condition to assert on (i.e. it shouldn't occur).
			 */
			distanceExtensionFactor = 0.f;
			break;
	}

	/* Calculate movement vector: */
	if (psStats->movementModel == MM_HOMINGDIRECT && psProj->psDest)
	{
		/* If it's homing and it has a target (not a miss)... */
		move.x = psProj->psDest->pos.x - psProj->startX;
		move.y = psProj->psDest->pos.y - psProj->startY;
		move.z = psProj->psDest->pos.z + establishTargetHeight(psProj->psDest) / 2 - psProj->srcHeight;
	}
	else
	{
		move.x = psProj->tarX - psProj->startX;
		move.y = psProj->tarY - psProj->startY;
		// LASSAT doesn't have a z
		if(psStats->weaponSubClass == WSC_LAS_SAT)
		{
			move.z = 0;
		}
		else if (!bIndirect)
		{
			move.z = psProj->altChange;
		}
		else
		{
			move.z = (psProj->vZ - (timeSoFar * ACC_GRAVITY / (GAME_TICKS_PER_SEC * 2))) * timeSoFar / GAME_TICKS_PER_SEC; // '2' because we reach our highest point in the mid of flight, when "vZ is 0".
		}
	}

	targetDistance = sqrtf(move.x * move.x + move.y * move.y);
	if (!bIndirect)
	{
		currentDistance = timeSoFar * psStats->flightSpeed / GAME_TICKS_PER_SEC;
	}
	else
	{
		currentDistance = timeSoFar * psProj->vXY / GAME_TICKS_PER_SEC;
	}

	// Prevent div by zero:
	if (targetDistance == 0)
	{
		targetDistance = 1;
	}

	distanceRatio = (float)currentDistance / targetDistance;
	distancePercent = PERCENT(currentDistance, targetDistance);

	/* Calculate next position */
	nextPos.x = psProj->startX + (distanceRatio * move.x);
	nextPos.y = psProj->startY + (distanceRatio * move.y);
	if (!bIndirect)
	{
		nextPosZ = (int32_t)(psProj->srcHeight + (distanceRatio * move.z));  // Save unclamped nextPos.z value.
	}
	else
	{
		nextPosZ = (int32_t)(psProj->srcHeight + move.z);  // Save unclamped nextPos.z value.
	}
	nextPos.z = MAX(0, nextPosZ);  // nextPos.z is unsigned.

	/* impact if about to go off map else update coordinates */
	if (!worldOnMap(nextPos.x, nextPos.y))
	{
		psProj->state = PROJ_IMPACT;
		psProj->tarX = psProj->pos.x;
		psProj->tarY = psProj->pos.y;
		setProjectileDestination(psProj, NULL);
		debug(LOG_NEVER, "**** projectile(%i) off map - removed ****\n", psProj->id);
		return;
	}

	/* Update position */
	prevPos = psProj->pos;
	psProj->pos = nextPos;

	if (bIndirect)
	{
		/* Update pitch */
		psProj->pitch = rad2degf(atan2f(psProj->vZ - (timeSoFar * ACC_GRAVITY / GAME_TICKS_PER_SEC), psProj->vXY));
	}

	/* Check nearby objects for possible collisions */
	for (i = 0; i < numProjNaybors; i++)
	{
		BASE_OBJECT *psTempObj = asProjNaybors[i].psObj;
		bool alreadyDamaged = false;

		CHECK_OBJECT(psTempObj);

		for (j = 0; j != psProj->psNumDamaged; ++j)
		{
			if (psTempObj == psProj->psDamaged[j])
			{
				alreadyDamaged = true;
				break;
			}
		}
		if (alreadyDamaged)
		{
			// Dont damage one target twice
			continue;
		}

		if (psTempObj->died)
		{
			// Do not damage dead objects further
			continue;
		}

		if (psTempObj->type == OBJ_PROJECTILE &&
				!(bMissile || ((PROJECTILE *)psTempObj)->psWStats->weaponSubClass == WSC_COUNTER))
		{
			// A projectile should not collide with another projectile unless it's a counter-missile weapon
			continue;
		}

		if (psTempObj->type == OBJ_FEATURE &&
				!((FEATURE *)psTempObj)->psStats->damageable)
		{
			// Ignore oil resources, artifacts and other pickups
			continue;
		}

		if (aiCheckAlliances(psTempObj->player, psProj->player)
				&& psTempObj != psProj->psDest)
		{
			// No friendly fire unless intentional
			continue;
		}

		if (psStats->surfaceToAir == SHOOT_IN_AIR &&
				(psTempObj->type == OBJ_STRUCTURE ||
				 psTempObj->type == OBJ_FEATURE ||
				 (psTempObj->type == OBJ_DROID && !isFlying((DROID *)psTempObj))
				))
		{
			// AA weapons should not hit buildings and non-vtol droids
			continue;
		}

		// Actual collision test.
		{
			// FIXME HACK Needed since we got those ugly Vector3uw floating around in BASE_OBJECT...
			Vector3i
			posProj = {psProj->pos.x, psProj->pos.y, nextPosZ},  // HACK psProj->pos.z may have been set to 0, since psProj->pos.z can't be negative. So can't use Vector3uw_To3i.
			prevPosProj = Vector3uw_To3i(prevPos),
			posTemp = Vector3uw_To3i(psTempObj->pos);

			Vector3i diff = Vector3i_Sub(posProj, posTemp);
			Vector3i prevDiff = Vector3i_Sub(prevPosProj, posTemp);  // HACK Ignore that target might be moving. The projectile is probably moving faster, so it's better than nothing...

			unsigned int targetHeight = establishTargetHeight(psTempObj);
			unsigned int targetRadius = establishTargetRadius(psTempObj);

			int32_t collision = collisionXYZ(prevDiff, diff, targetRadius, targetHeight);

			if (collision >= 0 && collision < closestCollision)
			{
				// We hit!
				//exactHitPos = prevPosProj + (posProj - prevPosProj)*collision/1024;
				Vector3i exactHitPos = Vector3i_Add(prevPosProj, Vector3i_Div(Vector3i_Mult(Vector3i_Sub(posProj, prevPosProj), collision), 1024));
				exactHitPos.z = MAX(0, exactHitPos.z);  // Clamp before casting to unsigned.
				closestCollisionPos = Vector3i_To3uw(exactHitPos);

				closestCollision = collision;
				closestCollisionObject = psTempObj;

				// Keep testing for more collisions, in case there was a closer target.
			}
		}
	}

	if (closestCollisionObject != NULL)
	{
		// We hit!
		psProj->pos = closestCollisionPos;
		setProjectileDestination(psProj, closestCollisionObject);  // We hit something.

		/* Buildings cannot be penetrated and we need a penetrating weapon */
		if (closestCollisionObject->type == OBJ_DROID && psStats->penetrate)
		{
			WEAPON asWeap = {psStats - asWeaponStats, 0, 0, 0, 0, 0};
			// Determine position to fire a missile at
			// (must be at least 0 because we don't use signed integers
			//  this shouldn't be larger than the height and width of the map either)
			Vector3i newDest =
			{
				psProj->startX + move.x * distanceExtensionFactor,
				psProj->startY + move.y * distanceExtensionFactor,
				psProj->srcHeight + move.z *distanceExtensionFactor
			};

			ASSERT(distanceExtensionFactor != 0.f, "Unitialized variable used! distanceExtensionFactor is not initialized.");

			newDest.x = clip(newDest.x, 0, world_coord(mapWidth - 1));
			newDest.y = clip(newDest.y, 0, world_coord(mapHeight - 1));

			// Assume we damaged the chosen target
			setProjectileDamaged(psProj, closestCollisionObject);

			proj_SendProjectile(&asWeap, (BASE_OBJECT *)psProj, psProj->player, newDest, NULL, true, -1);
		}

		psProj->state = PROJ_IMPACT;

		return;
	}

	ASSERT(distanceExtensionFactor != 0.f, "Unitialized variable used! distanceExtensionFactor is not initialized.");

	if (distanceRatio > distanceExtensionFactor || /* We've traveled our maximum range */
			!mapObjIsAboveGround((BASE_OBJECT *)psProj)) /* trying to travel through terrain */
	{
		/* Miss due to range or height */
		psProj->state = PROJ_IMPACT;
		setProjectileDestination(psProj, NULL); /* miss registered if NULL target */
		return;
	}

	/* Paint effects if visible */
	if (gfxVisible(psProj))
	{
		switch (psStats->weaponSubClass)
		{
			case WSC_FLAME:
			{
				Vector3i pos = {psProj->pos.x, psProj->pos.z - 8, psProj->pos.y};
				effectGiveAuxVar(distancePercent);
				addEffect(&pos, EFFECT_EXPLOSION, EXPLOSION_TYPE_FLAMETHROWER, false, NULL, 0);
			}
			break;
			case WSC_COMMAND:
			case WSC_ELECTRONIC:
			case WSC_EMP:
			{
				Vector3i pos = {psProj->pos.x, psProj->pos.z - 8, psProj->pos.y};
				effectGiveAuxVar(distancePercent / 2);
				addEffect(&pos, EFFECT_EXPLOSION, EXPLOSION_TYPE_LASER, false, NULL, 0);
			}
			break;
			case WSC_ROCKET:
			case WSC_MISSILE:
			case WSC_SLOWROCKET:
			case WSC_SLOWMISSILE:
			{
				Vector3i pos = {psProj->pos.x, psProj->pos.z + 8, psProj->pos.y};
				addEffect(&pos, EFFECT_SMOKE, SMOKE_TYPE_TRAIL, false, NULL, 0);
			}
			break;
			default:
				// Add smoke trail to indirect weapons, even if firing directly.
				if (!proj_Direct(psStats))
				{
					Vector3i pos = {psProj->pos.x, psProj->pos.z + 4, psProj->pos.y};
					addEffect(&pos, EFFECT_SMOKE, SMOKE_TYPE_TRAIL, false, NULL, 0);
				}
				// Otherwise no effect.
				break;
		}
	}
}

/***************************************************************************/

static void proj_ImpactFunc( PROJECTILE *psObj )
{
	WEAPON_STATS	*psStats;
	int32_t			i, iAudioImpactID;
	float			relativeDamage;
	Vector3i position, scatter;
	iIMDShape       *imd;
	HIT_SIDE	impactSide = HIT_SIDE_FRONT;

	CHECK_PROJECTILE(psObj);

	psStats = psObj->psWStats;
	ASSERT( psStats != NULL,
			"proj_ImpactFunc: Invalid weapon stats pointer" );

	// note the attacker if any
	g_pProjLastAttacker = psObj->psSource;

	/* play impact audio */
	if (gfxVisible(psObj))
	{
		if (psStats->iAudioImpactID == NO_SOUND)
		{
			/* play richochet if MG */
			if (psObj->psDest != NULL && psObj->psWStats->weaponSubClass == WSC_MGUN
					&& ONEINTHREE)
			{
				iAudioImpactID = ID_SOUND_RICOCHET_1 + (rand() % 3);
				audio_PlayStaticTrack(psObj->psDest->pos.x, psObj->psDest->pos.y, iAudioImpactID);
			}
		}
		else
		{
			audio_PlayStaticTrack(psObj->pos.x, psObj->pos.y, psStats->iAudioImpactID);
		}

		/* Shouldn't need to do this check but the stats aren't all at a value yet... */ // FIXME
		if (psStats->incenRadius && psStats->incenTime)
		{
			position.x = psObj->pos.x;
			position.z = psObj->pos.y; // z = y [sic] intentional
			position.y = map_Height(position.x, position.z);
			effectGiveAuxVar(psStats->incenRadius);
			effectGiveAuxVarSec(psStats->incenTime);
			addEffect(&position, EFFECT_FIRE, FIRE_TYPE_LOCALISED, false, NULL, 0);
		}

		// may want to add both a fire effect and the las sat effect
		if (psStats->weaponSubClass == WSC_LAS_SAT)
		{
			position.x = psObj->tarX;
			position.z = psObj->tarY;
			position.y = map_Height(position.x, position.z);
			addEffect(&position, EFFECT_SAT_LASER, SAT_LASER_STANDARD, false, NULL, 0);
			if (clipXY(psObj->tarX, psObj->tarY))
			{
				shakeStart();
			}
		}
	}

	// Set the effects position and radius
	position.x = psObj->pos.x;
	position.z = psObj->pos.y; // z = y [sic] intentional
	position.y = psObj->pos.z; // y = z [sic] intentional
	scatter.x = psStats->radius;
	scatter.y = 0;
	scatter.z = psStats->radius;

	// If the projectile missed its target (or the target died)
	if (psObj->psDest == NULL)
	{
		if (gfxVisible(psObj))
		{
			// Get if we are facing or not
			EFFECT_TYPE facing = (psStats->facePlayer ? EXPLOSION_TYPE_SPECIFIED : EXPLOSION_TYPE_NOT_FACING);

			// The graphic to show depends on if we hit water or not
			if (terrainType(mapTile(map_coord(psObj->pos.x), map_coord(psObj->pos.y))) == TER_WATER)
			{
				imd = psStats->pWaterHitGraphic;
			}
			// We did not hit water, the regular miss graphic will do the trick
			else
			{
				imd = psStats->pTargetMissGraphic;
			}

			addMultiEffect(&position, &scatter, EFFECT_EXPLOSION, facing, true, imd, psStats->numExplosions, psStats->lightWorld, psStats->effectSize);

			// If the target was a VTOL hit in the air add smoke
			if ((psStats->surfaceToAir & SHOOT_IN_AIR)
					&& !(psStats->surfaceToAir & SHOOT_ON_GROUND))
			{
				addMultiEffect(&position, &scatter, EFFECT_SMOKE, SMOKE_TYPE_DRIFTING, false, NULL, 3, 0, 0);
			}
		}
	}
	// The projectile hit its intended target
	else
	{
		CHECK_OBJECT(psObj->psDest);

		if (psObj->psDest->type == OBJ_FEATURE
				&& ((FEATURE *)psObj->psDest)->psStats->damageable == 0)
		{
			debug(LOG_NEVER, "proj_ImpactFunc: trying to damage non-damageable target,projectile removed");
			psObj->died = gameTime;
			return;
		}

		if (gfxVisible(psObj))
		{
			// Get if we are facing or not
			EFFECT_TYPE facing = (psStats->facePlayer ? EXPLOSION_TYPE_SPECIFIED : EXPLOSION_TYPE_NOT_FACING);

			// If we hit a VTOL with an AA gun use the miss graphic and add some smoke
			if ((psStats->surfaceToAir & SHOOT_IN_AIR)
					&& !(psStats->surfaceToAir & SHOOT_ON_GROUND)
					&& psStats->weaponSubClass == WSC_AAGUN)
			{
				imd = psStats->pTargetMissGraphic;
				addMultiEffect(&position, &scatter, EFFECT_SMOKE, SMOKE_TYPE_DRIFTING, false, NULL, 3, 0, 0);
			}
			// Otherwise we just hit it plain and simple
			else
			{
				imd = psStats->pTargetHitGraphic;
			}

			addMultiEffect(&position, &scatter, EFFECT_EXPLOSION, facing, true, imd, psStats->numExplosions, psStats->lightWorld, psStats->effectSize);
		}

		// Check for electronic warfare damage where we know the subclass and source
		if (proj_Direct(psStats)
				&& psStats->weaponSubClass == WSC_ELECTRONIC
				&& psObj->psSource)
		{
			// If we did enough `damage' to capture the target
			if (electronicDamage(psObj->psDest,
								 calcDamage(weaponDamage(psStats, psObj->player), psStats->weaponEffect, psObj->psDest),
								 psObj->player))
			{
				switch (psObj->psSource->type)
				{
					case OBJ_DROID:
						((DROID *) psObj->psSource)->order = DORDER_NONE;
						actionDroid((DROID *) (psObj->psSource), DACTION_NONE);
						break;

					case OBJ_STRUCTURE:
						((STRUCTURE *) psObj->psSource)->psTarget[0] = NULL;
						break;

					// This is only here to prevent the compiler from producing
					// warnings for unhandled enumeration values
					default:
						break;
				}
			}
		}
		// Else it is just a regular weapon (direct or indirect)
		else
		{
			// Calculate the damage the weapon does to its target
			unsigned int damage = calcDamage(weaponDamage(psStats, psObj->player), psStats->weaponEffect, psObj->psDest);

			// If we are in a multi-player game and the attacker is our responsibility
			if (bMultiPlayer && psObj->psSource && myResponsibility(psObj->psSource->player))
			{
				updateMultiStatsDamage(psObj->psSource->player, psObj->psDest->player, damage);
			}

			debug(LOG_NEVER, "Damage to object %d, player %d\n",
				  psObj->psDest->id, psObj->psDest->player);

			// If the target is a droid work out the side of it we hit
			if (psObj->psDest->type == OBJ_DROID)
			{
				// For indirect weapons (e.g. artillery) just assume the side as HIT_SIDE_TOP
				impactSide = proj_Direct(psStats) ? getHitSide(psObj, psObj->psDest) : HIT_SIDE_TOP;
			}

			// Damage the object
			relativeDamage = objectDamage(psObj->psDest, damage , psStats->weaponClass, psStats->weaponSubClass, impactSide);

			proj_UpdateKills(psObj, relativeDamage);

			if (relativeDamage >= 0)	// So long as the target wasn't killed
			{
				setProjectileDamaged(psObj, psObj->psDest);
			}
		}
	}

	// If the projectile does no splash damage and does not set fire to things
	if ((psStats->radius == 0) && (psStats->incenTime == 0) )
	{
		psObj->died = gameTime;
		return;
	}

	if (psStats->radius != 0)
	{
		FEATURE *psCurrF, *psNextF;

		/* An area effect bullet */
		psObj->state = PROJ_POSTIMPACT;

		/* Note when it exploded for the explosion effect */
		psObj->born = gameTime;

		for (i = 0; i < MAX_PLAYERS; i++)
		{
			DROID *psCurrD, *psNextD;

			for (psCurrD = apsDroidLists[i]; psCurrD; psCurrD = psNextD)
			{
				/* have to store the next pointer as psCurrD could be destroyed */
				psNextD = psCurrD->psNext;

				/* see if psCurrD is hit (don't hit main target twice) */
				if ((BASE_OBJECT *)psCurrD != psObj->psDest)
				{
					bool bTargetInAir = (asPropulsionTypes[asPropulsionStats[psCurrD->asBits[COMP_PROPULSION].nStat].propulsionType].travel == AIR && ((DROID *)psCurrD)->sMove.Status != MOVEINACTIVE);

					// Check whether we can hit it and it is in hit radius
					if (!((psStats->surfaceToAir == SHOOT_IN_AIR && !bTargetInAir) ||
							(psStats->surfaceToAir == SHOOT_ON_GROUND && bTargetInAir)) &&
							Vector3i_InSphere(Vector3uw_To3i(psCurrD->pos), Vector3uw_To3i(psObj->pos), psStats->radius))
					{
						int dice;
						HIT_ROLL(dice);
						if (dice < weaponRadiusHit(psStats, psObj->player))
						{
							unsigned int damage = calcDamage(
													  weaponRadDamage(psStats, psObj->player),
													  psStats->weaponEffect, (BASE_OBJECT *)psCurrD);

							debug(LOG_NEVER, "Damage to object %d, player %d\n",
								  psCurrD->id, psCurrD->player);

							if (bMultiPlayer)
							{
								if (psObj->psSource && myResponsibility(psObj->psSource->player))
								{
									updateMultiStatsDamage(psObj->psSource->player, psCurrD->player, damage);
								}
								turnOffMultiMsg(true);
							}

							//Watermelon:uses a slightly different check for angle,
							// since fragment of a project is from the explosion spot not from the projectile start position
							impactSide = getHitSide(psObj, (BASE_OBJECT *)psCurrD);
							relativeDamage = droidDamage(psCurrD, damage, psStats->weaponClass, psStats->weaponSubClass, impactSide);

							turnOffMultiMsg(false);	// multiplay msgs back on.

							proj_UpdateKills(psObj, relativeDamage);
						}
					}
				}
			}

			// FIXME Check whether we hit above maximum structure height, to skip unnecessary calculations!
			if (psStats->surfaceToAir != SHOOT_IN_AIR)
			{
				STRUCTURE *psCurrS, *psNextS;

				for (psCurrS = apsStructLists[i]; psCurrS; psCurrS = psNextS)
				{
					/* have to store the next pointer as psCurrD could be destroyed */
					psNextS = psCurrS->psNext;

					/* see if psCurrS is hit (don't hit main target twice) */
					if ((BASE_OBJECT *)psCurrS != psObj->psDest)
					{
						// Check whether it is in hit radius
						if (Vector3i_InCircle(Vector3uw_To3i(psCurrS->pos), Vector3uw_To3i(psObj->pos), psStats->radius))
						{
							int dice;
							HIT_ROLL(dice);
							if (dice < weaponRadiusHit(psStats, psObj->player))
							{
								unsigned int damage = calcDamage(weaponRadDamage(psStats, psObj->player), psStats->weaponEffect, (BASE_OBJECT *)psCurrS);

								if (bMultiPlayer)
								{
									if (psObj->psSource && myResponsibility(psObj->psSource->player))
									{
										updateMultiStatsDamage(psObj->psSource->player,	psCurrS->player, damage);
									}
								}

								//Watermelon:uses a slightly different check for angle,
								// since fragment of a project is from the explosion spot not from the projectile start position
								impactSide = getHitSide(psObj, (BASE_OBJECT *)psCurrS);

								relativeDamage = structureDamage(psCurrS,
																 damage,
																 psStats->weaponClass,
																 psStats->weaponSubClass, impactSide);

								proj_UpdateKills(psObj, relativeDamage);
							}
						}
					}
				}
			}
		}

		for (psCurrF = apsFeatureLists[0]; psCurrF; psCurrF = psNextF)
		{
			/* have to store the next pointer as psCurrD could be destroyed */
			psNextF = psCurrF->psNext;

			//ignore features that are not damageable
			if(!psCurrF->psStats->damageable)
			{
				continue;
			}
			/* see if psCurrS is hit (don't hit main target twice) */
			if ((BASE_OBJECT *)psCurrF != psObj->psDest)
			{
				// Check whether it is in hit radius
				if (Vector3i_InCircle(Vector3uw_To3i(psCurrF->pos), Vector3uw_To3i(psObj->pos), psStats->radius))
				{
					int dice;
					HIT_ROLL(dice);
					if (dice < weaponRadiusHit(psStats, psObj->player))
					{
						debug(LOG_NEVER, "Damage to object %d, player %d\n",
							  psCurrF->id, psCurrF->player);

						// Watermelon:uses a slightly different check for angle,
						// since fragment of a project is from the explosion spot not from the projectile start position
						impactSide = getHitSide(psObj, (BASE_OBJECT *)psCurrF);

						relativeDamage = featureDamage(psCurrF,
													   calcDamage(weaponRadDamage(psStats, psObj->player),
															   psStats->weaponEffect,
															   (BASE_OBJECT *)psCurrF),
													   psStats->weaponClass,
													   psStats->weaponSubClass, impactSide);

						proj_UpdateKills(psObj, relativeDamage);
					}
				}
			}
		}
	}

	if (psStats->incenTime != 0)
	{
		/* Incendiary round */
		/* Incendiary damage gets done in the bullet update routine */
		/* Just note when the incendiary started burning            */
		psObj->state = PROJ_POSTIMPACT;
		psObj->born = gameTime;
	}
	/* Something was blown up */
}

/***************************************************************************/

static void proj_PostImpactFunc( PROJECTILE *psObj )
{
	WEAPON_STATS	*psStats;
	int32_t			i, age;

	CHECK_PROJECTILE(psObj);

	psStats = psObj->psWStats;
	ASSERT( psStats != NULL,
			"proj_PostImpactFunc: Invalid weapon stats pointer" );

	age = (int32_t)gameTime - (int32_t)psObj->born;

	/* Time to finish postimpact effect? */
	if (age > (int32_t)psStats->radiusLife && age > (int32_t)psStats->incenTime)
	{
		psObj->died = gameTime;
		return;
	}

	/* Burning effect */
	if (psStats->incenTime > 0)
	{
		/* See if anything is in the fire and burn it */
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			/* Don't damage your own droids - unrealistic, but better */
			if(i != psObj->player)
			{
				proj_checkBurnDamage((BASE_OBJECT *)apsDroidLists[i], psObj);
				proj_checkBurnDamage((BASE_OBJECT *)apsStructLists[i], psObj);
			}
		}
	}
}

/***************************************************************************/

static void proj_Update(PROJECTILE *psObj)
{
	unsigned n, m;
	CHECK_PROJECTILE(psObj);

	/* See if any of the stored objects have died
	 * since the projectile was created
	 */
	if (psObj->psSource && psObj->psSource->died)
	{
		setProjectileSource(psObj, NULL);
	}
	if (psObj->psDest && psObj->psDest->died)
	{
		setProjectileDestination(psObj, NULL);
	}
	for (n = m = 0; n != psObj->psNumDamaged; ++n)
	{
		if (!psObj->psDamaged[n]->died)
		{
			psObj->psDamaged[m++] = psObj->psDamaged[n];
		}
	}
	psObj->psNumDamaged = m;

	// This extra check fixes a crash in cam2, mission1
	if (worldOnMap(psObj->pos.x, psObj->pos.y) == false)
	{
		psObj->died = true;
		return;
	}

	projGetNaybors((PROJECTILE *)psObj);

	switch (psObj->state)
	{
		case PROJ_INFLIGHTDIRECT:
			proj_InFlightFunc(psObj, false);
			break;

		case PROJ_INFLIGHTINDIRECT:
			proj_InFlightFunc(psObj, true);
			break;

		case PROJ_IMPACT:
			proj_ImpactFunc( psObj );
			break;

		case PROJ_POSTIMPACT:
			proj_PostImpactFunc( psObj );
			break;
	}
}

/***************************************************************************/

// iterate through all projectiles and update their status
void proj_UpdateAll()
{
	PROJECTILE	*psObj, *psPrev;

	for (psObj = psProjectileList; psObj != NULL; psObj = psObj->psNext)
	{
		proj_Update( psObj );
	}

	// Now delete any dead projectiles
	psObj = psProjectileList;

	// is the first node dead?
	while (psObj && psObj == psProjectileList && psObj->died)
	{
		psProjectileList = psObj->psNext;
		proj_Free(psObj);
		psObj = psProjectileList;
	}

	// first object is now either NULL or not dead, so we have time to set this below
	psPrev = NULL;

	// are any in the list dead?
	while (psObj)
	{
		if (psObj->died)
		{
			psPrev->psNext = psObj->psNext;
			proj_Free(psObj);
			psObj = psPrev->psNext;
		}
		else
		{
			psPrev = psObj;
			psObj = psObj->psNext;
		}
	}
}

/***************************************************************************/

static void proj_checkBurnDamage( BASE_OBJECT *apsList, PROJECTILE *psProj)
{
	BASE_OBJECT		*psCurr, *psNext;
	int32_t			xDiff, yDiff;
	WEAPON_STATS	*psStats;
	uint32_t			radSquared;
	float			relativeDamage;

	CHECK_PROJECTILE(psProj);

	// note the attacker if any
	g_pProjLastAttacker = psProj->psSource;

	psStats = psProj->psWStats;
	radSquared = psStats->incenRadius * psStats->incenRadius;

	for (psCurr = apsList; psCurr; psCurr = psNext)
	{
		/* have to store the next pointer as psCurr could be destroyed */
		psNext = psCurr->psNext;

		if ((psCurr->type == OBJ_DROID) &&
				isVtolDroid((DROID *)psCurr) &&
				((DROID *)psCurr)->sMove.Status != MOVEINACTIVE)
		{
			// can't set flying vtols on fire
			continue;
		}

		/* Within the bounding box, now check the radius */
		xDiff = psCurr->pos.x - psProj->pos.x;
		yDiff = psCurr->pos.y - psProj->pos.y;
		if (xDiff * xDiff + yDiff * yDiff <= radSquared)
		{
			/* The object is in the fire */
			psCurr->inFire |= IN_FIRE;

			if ( (psCurr->burnStart == 0) ||
					(psCurr->inFire & BURNING) )
			{
				/* This is the first turn the object is in the fire */
				psCurr->burnStart = gameTime;
				psCurr->burnDamage = 0;
			}
			else
			{
				/* Calculate how much damage should have
				   been done up till now */
				const int timeburned = gameTime - psCurr->burnStart;
				const int weapondamage = weaponIncenDamage(psStats, psProj->player);
				const int armourreduceddamage = weapondamage - psCurr->armour[HIT_SIDE_FRONT][WC_HEAT];
				const int minimaldamage = weapondamage / 3;
				const int realdamage = MAX(armourreduceddamage, minimaldamage);

				const int damageSoFar = realdamage * timeburned / GAME_TICKS_PER_SEC;
				int damageToDo = damageSoFar - psCurr->burnDamage;

				if (damageToDo > 20) // maximum DR becomes 95% instead of previous 0%
				{
					damageToDo -= (damageToDo % 20); // make deterministic

					debug(LOG_NEVER, "Burn damage of %d to object %d, player %d\n",
						  damageToDo, psCurr->id, psCurr->player);

					// HIT_SIDE_PIERCE because armor from burn effects is handled externally
					// To be consistent with vehicle burn, all burn damage is thermal flame damage
					relativeDamage = objectDamage(psCurr, damageToDo, WC_HEAT, WSC_FLAME, HIT_SIDE_PIERCE);

					psCurr->burnDamage += damageToDo;

					proj_UpdateKills(psProj, relativeDamage);
				}
				/* The damage could be negative if the object
				   is being burn't by another fire
				   with a higher burn damage */
			}
		}
	}
}

/***************************************************************************/

// return whether a weapon is direct or indirect
bool proj_Direct(const WEAPON_STATS *psStats)
{
	ASSERT(psStats != NULL, "proj_Direct: called with NULL weapon!");
	if (!psStats)
	{
		return true; // arbitrary value in no-debug case
	}
	ASSERT(psStats->movementModel < NUM_MOVEMENT_MODEL, "proj_Direct: invalid weapon stat");

	switch (psStats->movementModel)
	{
		case MM_DIRECT:
		case MM_HOMINGDIRECT:
		case MM_ERRATICDIRECT:
		case MM_SWEEP:
			return true;
			break;
		case MM_INDIRECT:
		case MM_HOMINGINDIRECT:
			return false;
			break;
		case NUM_MOVEMENT_MODEL:
			break; // error checking in assert above; this is for no-debug case
	}

	return true; // just to satisfy compiler
}

/***************************************************************************/

// return the maximum range for a weapon
int32_t proj_GetLongRange(const WEAPON_STATS *psStats)
{
	return psStats->longRange;
}


/***************************************************************************/
static uint32_t	establishTargetRadius(BASE_OBJECT *psTarget)
{
	uint32_t		radius;
	STRUCTURE	*psStructure;
	FEATURE		*psFeat;

	CHECK_OBJECT(psTarget);
	radius = 0;

	switch(psTarget->type)
	{
		case OBJ_DROID:
			switch(((DROID *)psTarget)->droidType)
			{
				case DROID_WEAPON:
				case DROID_SENSOR:
				case DROID_ECM:
				case DROID_CONSTRUCT:
				case DROID_COMMAND:
				case DROID_REPAIR:
				case DROID_PERSON:
				case DROID_CYBORG:
				case DROID_CYBORG_CONSTRUCT:
				case DROID_CYBORG_REPAIR:
				case DROID_CYBORG_SUPER:
					//Watermelon:'hitbox' size is now based on imd size
					radius = abs(psTarget->sDisplay.imd->radius) * 2;
					break;
				case DROID_DEFAULT:
				case DROID_TRANSPORTER:
				default:
					radius = TILE_UNITS / 4;	// how will we arrive at this?
			}
			break;
		case OBJ_STRUCTURE:
			psStructure = (STRUCTURE *)psTarget;
			radius = (MAX(psStructure->pStructureType->baseBreadth, psStructure->pStructureType->baseWidth) * TILE_UNITS) / 2;
			break;
		case OBJ_FEATURE:
//			radius = TILE_UNITS/4;	// how will we arrive at this?
			psFeat = (FEATURE *)psTarget;
			radius = (MAX(psFeat->psStats->baseBreadth, psFeat->psStats->baseWidth) * TILE_UNITS) / 2;
			break;
		case OBJ_PROJECTILE:
			//Watermelon 1/2 radius of a droid?
			radius = TILE_UNITS / 8;
		default:
			break;
	}

	return(radius);
}
/***************************************************************************/

/*the damage depends on the weapon effect and the target propulsion type or
structure strength*/
uint32_t	calcDamage(uint32_t baseDamage, WEAPON_EFFECT weaponEffect, BASE_OBJECT *psTarget)
{
	uint32_t	damage;

	if (psTarget->type == OBJ_STRUCTURE)
	{
		damage = baseDamage * asStructStrengthModifier[weaponEffect][((
					 STRUCTURE *)psTarget)->pStructureType->strength] / 100;
	}
	else if (psTarget->type == OBJ_DROID)
	{
		damage = baseDamage * asWeaponModifier[weaponEffect][(
					 asPropulsionStats + ((DROID *)psTarget)->asBits[COMP_PROPULSION].
					 nStat)->propulsionType] / 100;
	}
	// Default value
	else
	{
		damage = baseDamage;
	}

	// A little fail safe!
	if (damage == 0 && baseDamage != 0)
	{
		damage = 1;
	}

	return damage;
}

/*
 * A quick explanation about hown this function works:
 *  - It returns an integer between 0 and 100 (see note for exceptions);
 *  - this represents the amount of damage inflicted on the droid by the weapon
 *    in relation to its original health.
 *  - e.g. If 100 points of (*actual*) damage were done to a unit who started
 *    off (when first produced) with 400 points then .25 would be returned.
 *  - If the actual damage done to a unit is greater than its remaining points
 *    then the actual damage is clipped: so if we did 200 actual points of
 *    damage to a cyborg with 150 points left the actual damage would be taken
 *    as 150.
 *  - Should sufficient damage be done to destroy/kill a unit then the value is
 *    multiplied by -1, resulting in a negative number.
 */
static float objectDamage(BASE_OBJECT *psObj, uint32_t damage, uint32_t weaponClass, uint32_t weaponSubClass, HIT_SIDE impactSide)
{
	switch (psObj->type)
	{
		case OBJ_DROID:
			return droidDamage((DROID *)psObj, damage, weaponClass, weaponSubClass, impactSide);
			break;

		case OBJ_STRUCTURE:
			return structureDamage((STRUCTURE *)psObj, damage, weaponClass, weaponSubClass, impactSide);
			break;

		case OBJ_FEATURE:
			return featureDamage((FEATURE *)psObj, damage, weaponClass, weaponSubClass, impactSide);
			break;

		case OBJ_PROJECTILE:
			ASSERT(!"invalid object type: bullet", "invalid object type: OBJ_PROJECTILE (id=%d)", psObj->id);
			break;

		case OBJ_TARGET:
			ASSERT(!"invalid object type: target", "invalid object type: OBJ_TARGET (id=%d)", psObj->id);
			break;

		default:
			ASSERT(!"unknown object type", "unknown object type %d, id=%d", psObj->type, psObj->id );
	}

	return 0;
}

/**
 * This function will calculate which side of the droid psTarget the projectile
 * psObj hit. Although it is possible to extract the target from psObj it is
 * only the `direct' target of the projectile. Since impact sides also apply for
 * any splash damage a projectile might do the exact target is needed.
 */
static HIT_SIDE getHitSide(PROJECTILE *psObj, BASE_OBJECT *psTarget)
{
	int deltaX, deltaY;
	int impactAngle;

	// If we hit the top of the droid
	if (psObj->altChange > 300)
	{
		return HIT_SIDE_TOP;
	}
	// If the height difference between us and the target is > 50
	else if (psObj->pos.z < (psTarget->pos.z - 50))
	{
		return HIT_SIDE_BOTTOM;
	}
	// We hit an actual `side'
	else
	{
		deltaX = psObj->startX - psTarget->pos.x;
		deltaY = psObj->startY - psTarget->pos.y;

		/*
		 * Work out the impact angle. It is easiest to understand if you
		 * model the target droid as a circle, divided up into 360 pieces.
		 */
		impactAngle = abs(psTarget->direction - (180 * atan2f(deltaX, deltaY) / M_PI));

		impactAngle = wrap(impactAngle, 360);

		// Use the impact angle to work out the side hit
		// Right
		if (impactAngle > 45 && impactAngle < 135)
		{
			return HIT_SIDE_RIGHT;
		}
		// Rear
		else if (impactAngle >= 135 && impactAngle <= 225)
		{
			return HIT_SIDE_REAR;
		}
		// Left
		else if (impactAngle > 225 && impactAngle < 315)
		{
			return HIT_SIDE_LEFT;
		}
		// Front - default
		else //if (impactAngle <= 45 || impactAngle >= 315)
		{
			return HIT_SIDE_FRONT;
		}
	}
}

/* Returns true if an object has just been hit by an electronic warfare weapon*/
static BOOL	justBeenHitByEW(BASE_OBJECT *psObj)
{
	DROID		*psDroid;
	FEATURE		*psFeature;
	STRUCTURE	*psStructure;

	if(gamePaused())
	{
		return(false);	// Don't shake when paused...!
	}

	switch(psObj->type)
	{
		case OBJ_DROID:
			psDroid = (DROID *)psObj;
			if ((gameTime - psDroid->timeLastHit) < ELEC_DAMAGE_DURATION
					&& psDroid->lastHitWeapon == WSC_ELECTRONIC)
			{
				return(true);
			}
			break;

		case OBJ_FEATURE:
			psFeature = (FEATURE *)psObj;
			if ((gameTime - psFeature->timeLastHit) < ELEC_DAMAGE_DURATION)
			{
				return(true);
			}
			break;

		case OBJ_STRUCTURE:
			psStructure = (STRUCTURE *)psObj;
			if ((gameTime - psStructure->timeLastHit) < ELEC_DAMAGE_DURATION
					&& psStructure->lastHitWeapon == WSC_ELECTRONIC)
			{
				return true;
			}
			break;

		case OBJ_PROJECTILE:
			ASSERT(!"invalid object type: bullet", "justBeenHitByEW: invalid object type: OBJ_PROJECTILE");
			abort();
			break;

		case OBJ_TARGET:
			ASSERT(!"invalid object type: target", "justBeenHitByEW: invalid object type: OBJ_TARGET");
			abort();
			break;

		default:
			ASSERT(!"unknown object type", "justBeenHitByEW: unknown object type");
			abort();
	}

	return false;
}

void	objectShimmy(BASE_OBJECT *psObj)
{
	if(justBeenHitByEW(psObj))
	{
		iV_MatrixRotateX(SKY_SHIMMY);
		iV_MatrixRotateY(SKY_SHIMMY);
		iV_MatrixRotateZ(SKY_SHIMMY);
		if(psObj->type == OBJ_DROID)
		{
			iV_TRANSLATE(1 - rand() % 3, 0, 1 - rand() % 3);
		}
	}
}

// Watermelon:addProjNaybor ripped from droid.c
/* Add a new object to the projectile naybor list */
static void addProjNaybor(BASE_OBJECT *psObj, uint32_t distSqr)
{
	uint32_t	pos;

	if (numProjNaybors == 0)
	{
		// No objects in the list
		asProjNaybors[0].psObj = psObj;
		asProjNaybors[0].distSqr = distSqr;
		numProjNaybors++;
	}
	else if (distSqr >= asProjNaybors[numProjNaybors - 1].distSqr)
	{
		// Simple case - this is the most distant object
		asProjNaybors[numProjNaybors].psObj = psObj;
		asProjNaybors[numProjNaybors].distSqr = distSqr;
		numProjNaybors++;
	}
	else
	{
		// Move all the objects further away up the list
		pos = numProjNaybors;
		while (pos > 0 && asProjNaybors[pos - 1].distSqr > distSqr)
		{
			memcpy(asProjNaybors + pos, asProjNaybors + (pos - 1), sizeof(PROJ_NAYBOR_INFO));
			pos --;
		}

		// Insert the object at the correct position
		asProjNaybors[pos].psObj = psObj;
		asProjNaybors[pos].distSqr = distSqr;
		numProjNaybors++;
	}
}

//Watermelon: projGetNaybors ripped from droid.c
/* Find all the objects close to the projectile */
static void projGetNaybors(PROJECTILE *psObj)
{
	int32_t		xdiff, ydiff;
	uint32_t		distSqr;
	BASE_OBJECT	*psTempObj;

	CHECK_PROJECTILE(psObj);

	// Ensure only called max of once per droid per game cycle.
	if (CurrentProjNaybors == (BASE_OBJECT *)psObj && projnayborTime == gameTime)
	{
		return;
	}
	CurrentProjNaybors = (BASE_OBJECT *)psObj;
	projnayborTime = gameTime;

	// reset the naybor array
	numProjNaybors = 0;

	gridStartIterate(psObj->pos.x, psObj->pos.y);
	while ((psTempObj = gridIterate()) != NULL)
	{
		if (psTempObj != (BASE_OBJECT *)psObj && !psTempObj->died)
		{
			// See if an object is in NAYBOR_RANGE
			xdiff = (int32_t) psObj->pos.x - (int32_t) psTempObj->pos.x;
			ydiff = (int32_t) psObj->pos.y - (int32_t) psTempObj->pos.y;

			// Compute the distance squared
			distSqr = xdiff * xdiff + ydiff * ydiff;
			if (distSqr <= PROJ_NAYBOR_RANGE * PROJ_NAYBOR_RANGE)
			{
				// Add psTempObj as a naybor
				addProjNaybor(psTempObj, distSqr);

				// If the naybors array is full, break early
				if (numProjNaybors >= MAX_NAYBORS)
				{
					break;
				}
			}
		}
	}
}


#define BULLET_FLIGHT_HEIGHT 16


static uint32_t	establishTargetHeight(BASE_OBJECT *psTarget)
{
	if (psTarget == NULL)
	{
		return 0;
	}

	CHECK_OBJECT(psTarget);

	switch(psTarget->type)
	{
		case OBJ_DROID:
		{
			DROID *psDroid = (DROID *)psTarget;
			unsigned int height = asBodyStats[psDroid->asBits[COMP_BODY].nStat].pIMD->max.y - asBodyStats[psDroid->asBits[COMP_BODY].nStat].pIMD->min.y;
			unsigned int utilityHeight = 0, yMax = 0, yMin = 0; // Temporaries for addition of utility's height to total height

			// VTOL's don't have pIMD either it seems...
			if (isVtolDroid(psDroid))
			{
				return (height + VTOL_HITBOX_MODIFICATOR);
			}

			switch(psDroid->droidType)
			{
				case DROID_WEAPON:
					if ( psDroid->numWeaps > 0 )
					{
						// Don't do this for Barbarian Propulsions as they don't possess a turret (and thus have pIMD == NULL)
						if ((asWeaponStats[psDroid->asWeaps[0].nStat]).pIMD == NULL)
						{
							return height;
						}

						yMax = (asWeaponStats[psDroid->asWeaps[0].nStat]).pIMD->max.y;
						yMin = (asWeaponStats[psDroid->asWeaps[0].nStat]).pIMD->min.y;
					}
					break;

				case DROID_SENSOR:
					yMax = (asSensorStats[psDroid->asBits[COMP_SENSOR].nStat]).pIMD->max.y;
					yMin = (asSensorStats[psDroid->asBits[COMP_SENSOR].nStat]).pIMD->min.y;
					break;

				case DROID_ECM:
					yMax = (asECMStats[psDroid->asBits[COMP_ECM].nStat]).pIMD->max.y;
					yMin = (asECMStats[psDroid->asBits[COMP_ECM].nStat]).pIMD->min.y;
					break;

				case DROID_CONSTRUCT:
					yMax = (asConstructStats[psDroid->asBits[COMP_CONSTRUCT].nStat]).pIMD->max.y;
					yMin = (asConstructStats[psDroid->asBits[COMP_CONSTRUCT].nStat]).pIMD->min.y;
					break;

				case DROID_REPAIR:
					yMax = (asRepairStats[psDroid->asBits[COMP_REPAIRUNIT].nStat]).pIMD->max.y;
					yMin = (asRepairStats[psDroid->asBits[COMP_REPAIRUNIT].nStat]).pIMD->min.y;
					break;

				case DROID_PERSON:
				//TODO:add person 'state'checks here(stand, knee, crouch, prone etc)
				case DROID_CYBORG:
				case DROID_CYBORG_CONSTRUCT:
				case DROID_CYBORG_REPAIR:
				case DROID_CYBORG_SUPER:
				case DROID_DEFAULT:
				case DROID_TRANSPORTER:
				// Commanders don't have pIMD either
				case DROID_COMMAND:
				case DROID_ANY:
					return height;
			}

			utilityHeight = (yMax + yMin) / 2;

			return height + utilityHeight;
		}
		case OBJ_STRUCTURE:
		{
			STRUCTURE_STATS *psStructureStats = ((STRUCTURE *)psTarget)->pStructureType;
			return (psStructureStats->pIMD->max.y + psStructureStats->pIMD->min.y) / 2;
		}
		case OBJ_FEATURE:
			// Just use imd ymax+ymin
			return (psTarget->sDisplay.imd->max.y + psTarget->sDisplay.imd->min.y) / 2;
		case OBJ_PROJECTILE:
			return BULLET_FLIGHT_HEIGHT;
		default:
			return 0;
	}
}

void checkProjectile(const PROJECTILE *psProjectile, const char *const location_description, const char *function, const int recurse)
{
	unsigned n;
	if (recurse < 0)
	{
		return;
	}

	ASSERT_HELPER(psProjectile != NULL, location_description, function, "CHECK_PROJECTILE: NULL pointer");
	ASSERT_HELPER(psProjectile->psWStats != NULL, location_description, function, "CHECK_PROJECTILE");
	ASSERT_HELPER(psProjectile->type == OBJ_PROJECTILE, location_description, function, "CHECK_PROJECTILE");
	ASSERT_HELPER(psProjectile->player < MAX_PLAYERS, location_description, function, "CHECK_PROJECTILE: Out of bound owning player number (%u)", (unsigned int)psProjectile->player);
	ASSERT_HELPER(psProjectile->state == PROJ_INFLIGHTDIRECT
				  || psProjectile->state == PROJ_INFLIGHTINDIRECT
				  || psProjectile->state == PROJ_IMPACT
				  || psProjectile->state == PROJ_POSTIMPACT, location_description, function, "CHECK_PROJECTILE: invalid projectile state: %u", (unsigned int)psProjectile->state);
	ASSERT_HELPER(psProjectile->direction <= 360.0f && psProjectile->direction >= 0.0f, location_description, function, "CHECK_PROJECTILE: out of range direction (%f)", psProjectile->direction);

	if (psProjectile->psDest)
	{
		checkObject(psProjectile->psDest, location_description, function, recurse - 1);
	}

	if (psProjectile->psSource)
	{
		checkObject(psProjectile->psSource, location_description, function, recurse - 1);
	}

	for (n = 0; n != psProjectile->psNumDamaged; ++n)
	{
		checkObject(psProjectile->psDamaged[n], location_description, function, recurse - 1);
	}
}
