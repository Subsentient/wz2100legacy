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

#ifndef __INCLUDED_SRC_VISIBILITY__
#define __INCLUDED_SRC_VISIBILITY__

#include "objectdef.h"
#include "raycast.h"

// initialise the visibility stuff
extern BOOL visInitialise(void);

extern BOOL visTilesPending(BASE_OBJECT *psObj);

/* The terrain revealing ray callback */
extern bool rayTerrainCallback(Vector3i pos, int dist, void *data);

/* Ray callback for scripts */
extern bool scrRayTerrainCallback(Vector3i pos, int dist, void *data);

/* Check which tiles can be seen by an object */
extern void visTilesUpdate(BASE_OBJECT *psObj, RAY_CALLBACK callback);

/* Check whether psViewer can see psTarget
 * psViewer should be an object that has some form of sensor,
 * currently droids and structures.
 * psTarget can be any type of BASE_OBJECT (e.g. a tree).
 */
extern bool visibleObject(const BASE_OBJECT *psViewer, const BASE_OBJECT *psTarget, bool wallsBlock);

/** Can shooter hit target with direct fire weapon? */
bool lineOfFire(const BASE_OBJECT *psViewer, const BASE_OBJECT *psTarget, bool wallsBlock);

// Find the wall that is blocking LOS to a target (if any)
extern STRUCTURE *visGetBlockingWall(const BASE_OBJECT *psViewer, const BASE_OBJECT *psTarget);

extern void processVisibility(BASE_OBJECT *psCurr);

// update the visibility reduction
extern void visUpdateLevel(void);

extern void setUnderTilesVis(BASE_OBJECT *psObj, uint32_t player);

void visRemoveVisibility(BASE_OBJECT *psObj);
void visRemoveVisibilityOffWorld(BASE_OBJECT *psObj);

extern bool scrTileIsVisible(int32_t player, int32_t x, int32_t y);
extern void scrResetPlayerTileVisibility(int32_t player);

// fast test for whether obj2 is in range of obj1
static inline BOOL visObjInRange(BASE_OBJECT *psObj1, BASE_OBJECT *psObj2, int32_t range)
{
	int32_t	xdiff, ydiff, distSq, rangeSq;

	xdiff = (int32_t)psObj1->pos.x - (int32_t)psObj2->pos.x;
	if (xdiff < 0)
	{
		xdiff = -xdiff;
	}
	if (xdiff > range)
	{
		// too far away, reject
		return false;
	}

	ydiff = (int32_t)psObj1->pos.y - (int32_t)psObj2->pos.y;
	if (ydiff < 0)
	{
		ydiff = -ydiff;
	}
	if (ydiff > range)
	{
		// too far away, reject
		return false;
	}

	distSq = xdiff * xdiff + ydiff * ydiff;
	rangeSq = range * range;
	if (distSq > rangeSq)
	{
		// too far away, reject
		return false;
	}

	return true;
}

static inline int objSensorRange(const BASE_OBJECT *psObj)
{
	return psObj->sensorRange;
}

static inline int objSensorPower(const BASE_OBJECT *psObj)
{
	return psObj->sensorPower;
}

static inline int objJammerPower(WZ_DECL_UNUSED const BASE_OBJECT *psObj)
{
	return 0;
}

static inline int objJammerRange(WZ_DECL_UNUSED const BASE_OBJECT *psObj)
{
	return 0;
}

static inline int objConcealment(const BASE_OBJECT *psObj)
{
	return psObj->ECMMod;
}

#endif // __INCLUDED_SRC_VISIBILITY__
