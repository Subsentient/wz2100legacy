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
/** @file
 *  Definitions for the feature structures.
 */

#ifndef __INCLUDED_SRC_FEATURE_H__
#define __INCLUDED_SRC_FEATURE_H__

#include "objectdef.h"

/* The statistics for the features */
extern FEATURE_STATS	*asFeatureStats;
extern uint32_t			numFeatureStats;

//Value is stored for easy access to this feature in destroyDroid()/destroyStruct()
extern uint32_t			oilResFeature;

/* Load the feature stats */
extern BOOL loadFeatureStats(const char *pFeatureData, uint32_t bufferSize);

/* Release the feature stats memory */
extern void featureStatsShutDown(void);

/* Create a feature on the map */
extern FEATURE *buildFeature(FEATURE_STATS *psStats, uint32_t x, uint32_t y,BOOL FromSave);

/* Release the resources associated with a feature */
extern void featureRelease(FEATURE *psFeature);

/* Update routine for features */
extern void featureUpdate(FEATURE *psFeat);

// free up a feature with no visual effects
extern bool removeFeature(FEATURE *psDel);

/* Remove a Feature and free it's memory */
extern bool destroyFeature(FEATURE *psDel);

/* get a feature stat id from its name */
extern int32_t getFeatureStatFromName(const char *pName);

/*looks around the given droid to see if there is any building
wreckage to clear*/
extern FEATURE	 *checkForWreckage(DROID *psDroid);

extern float featureDamage(FEATURE *psFeature, uint32_t damage, uint32_t weaponClass, uint32_t weaponSubClass, HIT_SIDE impactSide);

extern void     featureInitVars(void);

#endif // __INCLUDED_SRC_FEATURE_H__
