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
 * Mechanics.c
 *
 * Game world mechanics.
 *
 */

#include "lib/framework/frame.h"

#include "basedef.h"
#include "droid.h"
#include "feature.h"
#include "mechanics.h"
#include "objmem.h"
#include "research.h"
#include "structure.h"

/* Shutdown the mechanics system */
bool mechanicsShutdown(void)
{
    BASE_OBJECT *psObj, *psNext;

    for(psObj = psDestroyedObj; psObj != NULL; psObj = psNext)
    {
        psNext = psObj->psNext;
        switch (psObj->type)
        {
            case OBJ_DROID:
                droidRelease((DROID *)psObj);
                break;

            case OBJ_STRUCTURE:
                structureRelease((STRUCTURE *)psObj);
                break;

            case OBJ_FEATURE:
                featureRelease((FEATURE *)psObj);
                break;

            default:
                ASSERT(!"unknown object type", "unknown object type in destroyed object list");
        }
        free(psObj);
    }
    psDestroyedObj = NULL;

    return true;
}


// Allocate the list for a component
BOOL allocComponentList(COMPONENT_TYPE	type, int32_t number)
{
    int32_t	inc, comp;

    //allocate the space for the Players' component lists
    for (inc=0; inc < MAX_PLAYERS; inc++)
    {
        if (apCompLists[inc][type])
        {
            free(apCompLists[inc][type]);
        }

        apCompLists[inc][type] = (uint8_t *) malloc(sizeof(uint8_t) * number);
        if (apCompLists[inc][type] == NULL)
        {
            debug( LOG_FATAL, "Out of memory assigning Player Component Lists" );
            abort();
            return false;
        }

        //initialise the players' lists
        for (comp=0; comp < number; comp++)
        {
            apCompLists[inc][type][comp] = UNAVAILABLE;
        }
    }

    return true;
}

// release all the component lists
void freeComponentLists(void)
{
    uint32_t	inc;

    for (inc=0; inc < MAX_PLAYERS; inc++)
    {
        //free the component lists
        if (apCompLists[inc][COMP_BODY])
        {
            free(apCompLists[inc][COMP_BODY]);
            apCompLists[inc][COMP_BODY] = NULL;
        }
        if (apCompLists[inc][COMP_BRAIN])
        {
            free(apCompLists[inc][COMP_BRAIN]);
            apCompLists[inc][COMP_BRAIN] = NULL;
        }
        if (apCompLists[inc][COMP_PROPULSION])
        {
            free(apCompLists[inc][COMP_PROPULSION]);
            apCompLists[inc][COMP_PROPULSION] = NULL;
        }
        if (apCompLists[inc][COMP_SENSOR])
        {
            free(apCompLists[inc][COMP_SENSOR]);
            apCompLists[inc][COMP_SENSOR] = NULL;
        }
        if (apCompLists[inc][COMP_ECM])
        {
            free(apCompLists[inc][COMP_ECM]);
            apCompLists[inc][COMP_ECM] = NULL;
        }
        if (apCompLists[inc][COMP_REPAIRUNIT])
        {
            free(apCompLists[inc][COMP_REPAIRUNIT]);
            apCompLists[inc][COMP_REPAIRUNIT] = NULL;
        }
        if (apCompLists[inc][COMP_CONSTRUCT])
        {
            free(apCompLists[inc][COMP_CONSTRUCT]);
            apCompLists[inc][COMP_CONSTRUCT] = NULL;
        }
        if (apCompLists[inc][COMP_WEAPON])
        {
            free(apCompLists[inc][COMP_WEAPON]);
            apCompLists[inc][COMP_WEAPON] = NULL;
        }
    }
}

//allocate the space for the Players' structure lists
BOOL allocStructLists(void)
{
    int32_t	inc, stat;

    for (inc=0; inc < MAX_PLAYERS; inc++)
    {
        if(numStructureStats)
        {
            apStructTypeLists[inc] = (uint8_t *) malloc(sizeof(uint8_t) *
                                     numStructureStats);
            if (apStructTypeLists[inc] == NULL)
            {
                debug( LOG_FATAL, "Out of memory assigning Player Structure Lists" );
                abort();
                return false;
            }
            for (stat = 0; stat < (int32_t)numStructureStats; stat++)
            {
                apStructTypeLists[inc][stat] = UNAVAILABLE;
            }
        }
        else
        {
            apStructTypeLists[inc] = NULL;
        }
    }

    return true;
}


// release the structure lists
void freeStructureLists(void)
{
    uint32_t	inc;

    for (inc=0; inc < MAX_PLAYERS; inc++)
    {
        //free the structure lists
        if(apStructTypeLists[inc])
        {
            free(apStructTypeLists[inc]);
            apStructTypeLists[inc] = NULL;
        }
    }
}


//TEST FUNCTION - MAKE EVERYTHING AVAILABLE
void makeAllAvailable(void)
{
    uint32_t	comp,i;

    for(i=0; i<MAX_PLAYERS; i++)
    {
        for (comp=0; comp <numWeaponStats; comp++)
        {
            apCompLists[i][COMP_WEAPON][comp] = AVAILABLE;
        }
        for (comp=0; comp <numBodyStats; comp++)
        {
            apCompLists[i][COMP_BODY][comp] = AVAILABLE;
        }
        for (comp=0; comp <numPropulsionStats; comp++)
        {
            apCompLists[i][COMP_PROPULSION][comp] = AVAILABLE;
        }
        for (comp=0; comp <numSensorStats; comp++)
        {
            apCompLists[i][COMP_SENSOR][comp] = AVAILABLE;
        }
        for (comp=0; comp <numECMStats; comp++)
        {
            apCompLists[i][COMP_ECM][comp] = AVAILABLE;
        }
        for (comp=0; comp <numConstructStats; comp++)
        {
            apCompLists[i][COMP_CONSTRUCT][comp] = AVAILABLE;
        }
        for (comp=0; comp <numBrainStats; comp++)
        {
            apCompLists[i][COMP_BRAIN][comp] = AVAILABLE;
        }
        for (comp=0; comp <numRepairStats; comp++)
        {
            apCompLists[i][COMP_REPAIRUNIT][comp] = AVAILABLE;
        }

        //make all the structures available
        for (comp=0; comp < numStructureStats; comp++)
        {
            apStructTypeLists[i][comp] = AVAILABLE;
        }
        //make all research availble to be performed
        for (comp = 0; comp < numResearch; comp++)
        {
            enableResearch(&asResearch[comp], i);
        }
    }
}

