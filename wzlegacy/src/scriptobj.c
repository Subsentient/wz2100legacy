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
 * ScriptObj.c
 *
 * Object access functions for the script library
 *
 */
#include <string.h>

#include "lib/framework/frame.h"
#include "lib/framework/endian_hack.h"
#include "lib/framework/strres.h"
#include "objects.h"

#include "lib/script/script.h"
#include "scripttabs.h"
#include "scriptobj.h"
#include "group.h"
#include "lib/gamelib/gtime.h"
#include "cluster.h"
#include "messagedef.h"
#include "message.h"
#include "researchdef.h"
#include "lib/sound/audio.h"
#include "lib/sound/audio_id.h"

#include "multiplay.h"

#include "text.h"
#include "levels.h"
#include "scriptvals.h"
#include "research.h"

// Marks a NULL pointer for the script value save/load routines
static const int UNALLOCATED_OBJECT = -1;

static INTERP_VAL	scrFunctionResult;	//function return value to be pushed to stack

// Get values from a base object
BOOL scrBaseObjGet(uint32_t index)
{
    INTERP_TYPE		type = 0;
    BASE_OBJECT		*psObj;
    DROID			*psDroid;
    STRUCTURE		*psStruct;
    FEATURE			*psFeature;

    if (!stackPopParams(1, ST_BASEOBJECT, &psObj))
    {
        debug(LOG_ERROR, "scrBaseObjGet: stackPopParams failed");
        return false;
    }

    // Check this is a valid pointer
    if (psObj == NULL )
    {
        debug(LOG_ERROR, "scrBaseObjGet: was passed an invalid pointer");
        return false;
    }
    // Check this is a valid pointer
    if (psObj->type != OBJ_DROID && psObj->type != OBJ_STRUCTURE && psObj->type != OBJ_FEATURE)
    {
        debug(LOG_ERROR, "scrBaseObjGet: invalid object");
        return false;
    }

    // set the type and return value
    switch (index)
    {
        case OBJID_POSX:
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)psObj->pos.x;
            break;
        case OBJID_POSY:
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)psObj->pos.y;
            break;
        case OBJID_POSZ:
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)psObj->pos.z;
            break;
        case OBJID_ID:
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)psObj->id;
            break;
        case OBJID_PLAYER:
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)psObj->player;
            break;
        case OBJID_TYPE:
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)psObj->type;
            break;
        case OBJID_ORDER:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: order only valid for a droid");
                return false;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->order;
            if (scrFunctionResult.v.ival == DORDER_GUARD && ((DROID *)psObj)->psTarget == NULL)
            {
                scrFunctionResult.v.ival = DORDER_NONE;
            }
            break;
            //new member variable
        case OBJID_ACTION:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: action only valid for a droid");
                return false;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->action;
            break;
            //new member variable - if droid is selected (humans only)
        case OBJID_SELECTED:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: selected only valid for a droid");
                return false;
            }
            type = VAL_BOOL;
            scrFunctionResult.v.bval = (int32_t)((DROID *)psObj)->selected;
            break;

        case OBJID_STRUCTSTATTYPE:
            if (psObj->type == OBJ_STRUCTURE)
            {
                type = VAL_INT;
                scrFunctionResult.v.ival = ((STRUCTURE *)psObj)->pStructureType->type;
            }
            else
            {
                debug(LOG_ERROR, ".stattype is only supported by Structures");
                return false;
            }
            break;

        case OBJID_ORDERX:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: order only valid for a droid");
                return false;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->orderX;
            break;
        case OBJID_ORDERY:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: order only valid for a droid");
                return false;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->orderY;
            break;
        case OBJID_DROIDTYPE:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: droidType only valid for a droid");
                return false;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->droidType;
            break;
        case OBJID_CLUSTERID:
            if (psObj->type == OBJ_FEATURE)
            {
                debug(LOG_ERROR, "scrBaseObjGet: clusterID not valid for features");
                return false;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = clustGetClusterID(psObj);
            break;
        case OBJID_HEALTH:
            switch (psObj->type)
            {
                case OBJ_DROID:
                    psDroid = (DROID *)psObj;
                    type = VAL_INT;
                    scrFunctionResult.v.ival = psDroid->body * 100 / psDroid->originalBody;
                    break;
                case OBJ_FEATURE:
                    psFeature = (FEATURE *)psObj;
                    type = VAL_INT;
                    if (psFeature->psStats->damageable)
                    {
                        scrFunctionResult.v.ival = psFeature->body * 100 / psFeature->psStats->body;
                    }
                    else
                    {
                        scrFunctionResult.v.ival = 100;
                    }
                    break;
                case OBJ_STRUCTURE:
                    psStruct = (STRUCTURE *)psObj;
                    type = VAL_INT;
                    //val = psStruct->body * 100 / psStruct->baseBodyPoints;
                    scrFunctionResult.v.ival = psStruct->body * 100 / structureBody(psStruct);
                    break;
                default:
                    break;
            }
            break;
        case OBJID_BODY:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: body only valid for a droid");
                return false;
            }
            type = (INTERP_TYPE)ST_BODY;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->asBits[COMP_BODY].nStat;
            break;
        case OBJID_PROPULSION:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: propulsion only valid for a droid");
                return false;
            }
            type = (INTERP_TYPE)ST_PROPULSION;
            scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->asBits[COMP_PROPULSION].nStat;
            break;
        case OBJID_WEAPON:		//TODO: only returns first weapon now
            type = (INTERP_TYPE)ST_WEAPON;
            switch (psObj->type)
            {
                case OBJ_DROID:
                    if (((DROID *)psObj)->asWeaps[0].nStat == 0)
                    {
                        scrFunctionResult.v.ival = 0;
                    }
                    else
                    {
                        scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->asWeaps[0].nStat;
                    }
                    break;
                case OBJ_STRUCTURE:
                    if (((STRUCTURE *)psObj)->numWeaps == 0 || ((STRUCTURE *)psObj)->asWeaps[0].nStat == 0)
                    {
                        scrFunctionResult.v.ival = 0;
                    }
                    else
                    {
                        scrFunctionResult.v.ival = (int32_t)((STRUCTURE *)psObj)->asWeaps[0].nStat;
                    }
                    break;
                default:		//only droids and structures can have a weapon
                    debug(LOG_ERROR, "scrBaseObjGet: weapon only valid for droids and structures" );
                    return false;
                    break;
            }

            break;

        case OBJID_STRUCTSTAT:
            //droid.stat - now returns the type of structure a truck is building for droids
            if (psObj->type == OBJ_STRUCTURE)
            {
                type = (INTERP_TYPE)ST_STRUCTURESTAT;
                scrFunctionResult.v.ival = ((STRUCTURE *)psObj)->pStructureType - asStructureStats;
            }
            else if (psObj->type == OBJ_DROID)
            {
                type = (INTERP_TYPE)ST_STRUCTURESTAT;
                scrFunctionResult.v.ival = (int32_t)((STRUCTURE_STATS *)(((DROID *)psObj)->psTarStats) - asStructureStats);
            }
            else		//Nothing else supported
            {
                debug(LOG_ERROR, "scrBaseObjGet(): .stat only valid for structures and droids");
                return false;
            }

            break;

        case OBJID_TARGET:
            //added object->psTarget
            if (psObj->type == OBJ_STRUCTURE)
            {
                type = (INTERP_TYPE)ST_BASEOBJECT;
                scrFunctionResult.v.oval = ((STRUCTURE *)psObj)->psTarget[0];
            }
            else if (psObj->type == OBJ_DROID)
            {
                type = (INTERP_TYPE)ST_BASEOBJECT;
                scrFunctionResult.v.oval = ((DROID *)psObj)->psTarget;
            }
            else		//Nothing else supported
            {
                debug(LOG_ERROR, "scrBaseObjGet(): .target only valid for structures and droids");
                return false;
            }

            break;
        case OBJID_GROUP:
            if (psObj->type != OBJ_DROID)
            {
                debug(LOG_ERROR, "scrBaseObjGet: group only valid for a droid");
                return false;
            }

            type = (INTERP_TYPE)ST_GROUP;
            scrFunctionResult.v.oval = ((DROID *)psObj)->psGroup;
            break;

        case OBJID_HITPOINTS:

            type = VAL_INT;

            switch (psObj->type)
            {
                case OBJ_DROID:
                    scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->body;
                    break;

                case OBJ_STRUCTURE:
                    scrFunctionResult.v.ival = (int32_t)((STRUCTURE *)psObj)->body;
                    break;

                case OBJ_FEATURE:
                    scrFunctionResult.v.ival = (int32_t)((FEATURE *)psObj)->body;
                    break;

                default:
                    debug(LOG_ERROR, "scrBaseObjGet: unknown object type");
                    return false;
                    break;
            }

            break;
        case OBJID_ORIG_HITPOINTS:
            type = VAL_INT;

            switch (psObj->type)
            {
                case OBJ_DROID:
                    scrFunctionResult.v.ival = (int32_t)((DROID *)psObj)->originalBody;
                    break;

                case OBJ_STRUCTURE:
                    scrFunctionResult.v.ival = (int32_t)structureBody((STRUCTURE *)psObj);
                    break;

                case OBJ_FEATURE:
                    scrFunctionResult.v.ival = ((FEATURE *)psObj)->psStats->body;
                    break;

                default:
                    debug(LOG_ERROR, "scrBaseObjGet: unknown object type");
                    return false;
                    break;
            }

            break;
        default:
            debug(LOG_ERROR, "scrBaseObjGet: unknown variable index");
            return false;
            break;
    }

    // Return the value
    if (!stackPushResult(type, &scrFunctionResult))
    {
        debug(LOG_ERROR, "scrBaseObjGet: stackPushResult() failed");
        return false;
    }

    return true;
}


// convert a base object to a droid if it is the right type
BOOL scrObjToDroid(void)
{
    BASE_OBJECT		*psObj;

    if (!stackPopParams(1, ST_BASEOBJECT, &psObj))
    {
        return false;
    }

    // return NULL if not a droid
    if (psObj->type != OBJ_DROID)
    {
        psObj = NULL;
    }


    scrFunctionResult.v.oval = psObj;
    if (!stackPushResult((INTERP_TYPE)ST_DROID, &scrFunctionResult))
    {
        return false;
    }

    return true;
}


// convert a base object to a structure if it is the right type
BOOL scrObjToStructure(void)
{
    BASE_OBJECT		*psObj;

    if (!stackPopParams(1, ST_BASEOBJECT, &psObj))
    {
        return false;
    }

    // return NULL if not a droid
    if (psObj->type != OBJ_STRUCTURE)
    {
        psObj = NULL;
    }

    scrFunctionResult.v.oval = psObj;
    if (!stackPushResult((INTERP_TYPE)ST_STRUCTURE, &scrFunctionResult))
    {
        return false;
    }

    return true;
}


// convert a base object to a feature if it is the right type
BOOL scrObjToFeature(void)
{
    BASE_OBJECT		*psObj;

    if (!stackPopParams(1, ST_BASEOBJECT, &psObj))
    {
        return false;
    }

    // return NULL if not a droid
    if (psObj->type != OBJ_FEATURE)
    {
        psObj = NULL;
    }

    scrFunctionResult.v.oval = psObj;
    if (!stackPushResult((INTERP_TYPE)ST_FEATURE, &scrFunctionResult))
    {
        return false;
    }

    return true;
}


// cache all the possible values for the last group to try
// to speed up access
static int32_t		lgX,lgY, lgMembers, lgHealth;

// Get values from a weapon
BOOL scrWeaponObjGet(uint32_t index)
{
    INTERP_TYPE		type;
    int32_t			weapIndex;

    if (!stackPopParams(1, ST_WEAPON, &weapIndex))
    {
        return false;
    }

    switch (index)
    {
        case WEAPID_SHORT_RANGE:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].shortRange;

            break;
        case WEAPID_LONG_RANGE:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].longRange;

            break;
        case WEAPID_SHORT_HIT:
            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].shortHit;

            break;
        case WEAPID_LONG_HIT:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].longHit;

            break;
        case WEAPID_DAMAGE:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].damage;

            break;
        case WEAPID_FIRE_PAUSE:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].firePause;

            break;
        case WEAPID_RELOAD_TIME:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].reloadTime;

            break;
        case WEAPID_NUM_ROUNDS:

            type = VAL_INT;

            scrFunctionResult.v.ival = asWeaponStats[weapIndex].numRounds;

            break;
        default:
            ASSERT( false, "unknown variable index" );
            return false;
            break;
    }

    // Return the value
    if (!stackPushResult(type, &scrFunctionResult))
    {
        return false;
    }

    return true;
}

// Get values from a group
BOOL scrGroupObjGet(uint32_t index)
{
    INTERP_TYPE		type;
    DROID_GROUP		*psGroup;
    DROID			*psCurr;

    if (!stackPopParams(1, ST_GROUP, &psGroup))
    {
        return false;
    }

    switch (index)
    {
        case GROUPID_POSX:
            lgX = 0;
            lgMembers = 0;
            for(psCurr = psGroup->psList; psCurr; psCurr = psCurr->psGrpNext)
            {
                lgMembers += 1;
                lgX += (int32_t)psCurr->pos.x;
            }

            if (lgMembers > 0)
            {
                lgX = lgX / lgMembers;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = lgX;
            break;
        case GROUPID_POSY:
            lgY = 0;
            lgMembers = 0;
            for(psCurr = psGroup->psList; psCurr; psCurr = psCurr->psGrpNext)
            {
                lgMembers += 1;
                lgY += (int32_t)psCurr->pos.y;
            }

            if (lgMembers > 0)
            {
                lgY = lgY / lgMembers;
            }

            type = VAL_INT;
            scrFunctionResult.v.ival = lgY;
            break;
        case GROUPID_MEMBERS:
            lgMembers = 0;
            for(psCurr = psGroup->psList; psCurr; psCurr = psCurr->psGrpNext)
            {
                lgMembers += 1;
            }

            type = VAL_INT;
            scrFunctionResult.v.ival = lgMembers;
            break;
        case GROUPID_HEALTH:
            lgHealth = 0;
            lgMembers = 0;
            for(psCurr = psGroup->psList; psCurr; psCurr = psCurr->psGrpNext)
            {
                lgMembers += 1;
                lgHealth += (int32_t)((100 * psCurr->body)/psCurr->originalBody);
            }

            if (lgMembers > 0)
            {
                lgHealth = lgHealth / lgMembers;
            }
            type = VAL_INT;
            scrFunctionResult.v.ival = lgHealth;
            break;
        case GROUPID_TYPE:
            type = VAL_INT;
            scrFunctionResult.v.ival = psGroup->type;
            break;
        case GROUPID_CMD:
            type = (INTERP_TYPE)ST_DROID;
            scrFunctionResult.v.oval = psGroup->psCommander;
            break;
        default:
            ASSERT( false, "scrGroupObjGet: unknown variable index" );
            return false;
            break;
    }

    // Return the value
    if (!stackPushResult(type, &scrFunctionResult))
    {
        return false;
    }

    return true;
}


// get the name from a stat pointer
static char *scrGetStatName(INTERP_TYPE type, uint32_t data)
{
    char	*pName = NULL;

    switch (type)
    {
        case ST_STRUCTURESTAT:
            if (data < numStructureStats)
            {
                pName = asStructureStats[data].pName;
            }
            break;
        case ST_FEATURESTAT:
            if (data < numFeatureStats)
            {
                pName = asFeatureStats[data].pName;
            }
            break;
        case ST_BODY:
            if (data < numBodyStats)
            {
                pName = asBodyStats[data].pName;
            }
            break;
        case ST_PROPULSION:
            if (data < numPropulsionStats)
            {
                pName = asPropulsionStats[data].pName;
            }
            break;
        case ST_ECM:
            if (data < numECMStats)
            {
                pName = asECMStats[data].pName;
            }
            break;
        case ST_SENSOR:
            if (data < numSensorStats)
            {
                pName = asSensorStats[data].pName;
            }
            break;
        case ST_CONSTRUCT:
            if (data < numConstructStats)
            {
                pName = asConstructStats[data].pName;
            }
            break;
        case ST_WEAPON:
            if (data < numWeaponStats)
            {
                pName = asWeaponStats[data].pName;
            }
            break;
        case ST_REPAIR:
            if (data < numRepairStats)
            {
                pName = asRepairStats[data].pName;
            }
            break;
        case ST_BRAIN:
            if (data < numBrainStats)
            {
                pName = asBrainStats[data].pName;
            }
            break;
        case ST_BASESTATS:
        case ST_COMPONENT:
            // should never have variables of this type
            break;
        default:
            break;
    }

    if (pName == NULL)
    {
        debug( LOG_FATAL, "scrGetStatName: cannot get name for a base stat" );
        abort();
    }

    return pName;
}

// default value save routine
//TODO: use union
BOOL scrValDefSave(INTERP_VAL *psVal, char *pBuffer, uint32_t *pSize)
{
    VIEWDATA	*psIntMessage;
    const char	*pName;
    RESEARCH	*psResearch;
    char		*pPos;
    DROID		*psCDroid;

    switch (psVal->type)
    {
        case ST_INTMESSAGE:
            // save the name
            psIntMessage = (VIEWDATA *)psVal->v.oval;
            if (psIntMessage != NULL)
            {
                if (pBuffer)
                {
                    strcpy(pBuffer, psIntMessage->pName);
                }
                *pSize = strlen(psIntMessage->pName)+1;
            }
            else
            {
                if (pBuffer)
                {
                    *pBuffer = '\0';
                }
                *pSize = 1;
            }
            break;
        case ST_BASEOBJECT:
        case ST_DROID:
        case ST_STRUCTURE:
        case ST_FEATURE:
            // just save the id
            if (pBuffer)
            {
                if (psVal->v.oval == NULL || ((BASE_OBJECT *)psVal->v.oval)->died > 1 || !getBaseObjFromId(((BASE_OBJECT *)psVal->v.oval)->id))
                {
                    *((uint32_t *)pBuffer) = uint32_t_MAX;
                }
                else
                {
                    *((uint32_t *)pBuffer) = ((BASE_OBJECT *)psVal->v.oval)->id;
                }
                endian_udword((uint32_t *)pBuffer);
            }
            *pSize = sizeof(uint32_t);
            break;
        case ST_BASESTATS:
        case ST_COMPONENT:
        case ST_FEATURESTAT:
        case ST_STRUCTURESTAT:
        case ST_BODY:
        case ST_PROPULSION:
        case ST_ECM:
        case ST_SENSOR:
        case ST_CONSTRUCT:
        case ST_WEAPON:
        case ST_REPAIR:
        case ST_BRAIN:
            pName = scrGetStatName(psVal->type, psVal->v.ival);
            if (pName != NULL)
            {
                if (pBuffer)
                {
                    strcpy(pBuffer, pName);
                }
                *pSize = strlen(pName) + 1;
            }
            else
            {
                return false;
            }
            break;
        case ST_TEMPLATE:
            if (pBuffer)
            {
                if (psVal->v.oval == NULL)
                {
                    *((uint32_t *)pBuffer) = uint32_t_MAX;
                }
                else
                {
                    *((uint32_t *)pBuffer) = ((DROID_TEMPLATE *)psVal->v.oval)->multiPlayerID;
                }
                endian_udword((uint32_t *)pBuffer);
            }
            *pSize = sizeof(uint32_t);
            break;
        case ST_TEXTSTRING:
            {
                const char *const idStr = psVal->v.sval ? strresGetIDfromString(psStringRes, psVal->v.sval) : NULL;
                uint16_t len = idStr ? strlen(idStr) + 1 : 0;

                if (pBuffer)
                {
                    *((uint16_t *)pBuffer) = len;
                    endian_uword((uint16_t *)pBuffer);

                    memcpy(pBuffer + sizeof(len), idStr, len);
                }
                *pSize = sizeof(len) + len;
                break;
            }
        case ST_LEVEL:
            if (psVal->v.sval != NULL)
            {
                if (pBuffer)
                {
                    strcpy(pBuffer, psVal->v.sval);
                }
                *pSize = strlen(psVal->v.sval)+1;
            }
            else
            {
                if (pBuffer)
                {
                    *pBuffer = '\0';
                }
                *pSize = 1;
            }
            break;
        case ST_RESEARCH:
            psResearch = (RESEARCH *)psVal->v.oval;
            if (psResearch != NULL)
            {
                if (pBuffer)
                {
                    strcpy(pBuffer, psResearch->pName);
                }
                *pSize = strlen(psResearch->pName)+1;
            }
            else
            {
                if (pBuffer)
                {
                    *pBuffer = '\0';
                }
                *pSize = 1;
            }
            break;
        case ST_GROUP:
            {
                DROID_GROUP *const psGroup = (DROID_GROUP *)psVal->v.oval;
                const int members = psGroup ? grpNumMembers(psGroup) : UNALLOCATED_OBJECT;

                if (pBuffer)
                {
                    pPos = pBuffer;

                    *((int32_t *)pPos) = members;
                    endian_sdword((int32_t *)pPos);
                    pPos += sizeof(int32_t);

                    if (psGroup)
                    {
                        // store the run data
                        *((int32_t *)pPos) = psGroup->sRunData.sPos.x;
                        endian_sdword((int32_t *)pPos);
                        pPos += sizeof(int32_t);
                        *((int32_t *)pPos) = psGroup->sRunData.sPos.y;
                        endian_sdword((int32_t *)pPos);
                        pPos += sizeof(int32_t);
                        *((int32_t *)pPos) = psGroup->sRunData.forceLevel;
                        endian_sdword((int32_t *)pPos);
                        pPos += sizeof(int32_t);
                        *((int32_t *)pPos) = psGroup->sRunData.leadership;
                        endian_sdword((int32_t *)pPos);
                        pPos += sizeof(int32_t);
                        *((int32_t *)pPos) = psGroup->sRunData.healthLevel;
                        endian_sdword((int32_t *)pPos);
                        pPos += sizeof(int32_t);

                        // now store the droids
                        for (psCDroid = psGroup->psList; psCDroid; psCDroid = psCDroid->psGrpNext)
                        {
                            checkValidId(psCDroid->id);

                            *((uint32_t *)pPos) = psCDroid->id;
                            endian_udword((uint32_t *)pPos);

                            pPos += sizeof(uint32_t);
                        }
                    }
                }

                if (!psGroup)
                {
                    *pSize = sizeof(int32_t);
                }
                else
                {
                    *pSize = sizeof(int32_t) + sizeof(uint32_t) * members + sizeof(int32_t) * 5;	// members + runData
                }
                break;
            }
        case ST_SOUND:
            if(psVal->v.ival)
            {
                // can also return NULL
                pName = sound_GetTrackName((uint32_t)psVal->v.ival);
            }
            else
            {
                pName = NULL;
            }
            if (pName == NULL)
            {
                debug(LOG_WARNING, "scrValDefSave: couldn't get sound track name");
                // just save an empty string
                pName = "";
            }
            if (pBuffer)
            {
                strcpy(pBuffer, pName);
            }
            *pSize = strlen(pName) + 1;
            break;
        case ST_STRUCTUREID:
        case ST_DROIDID:
            // just save the variable contents directly
            if (pBuffer)
            {
                *((uint32_t *)pBuffer) = psVal->v.ival;
                endian_udword((uint32_t *)pBuffer);
            }
            *pSize = sizeof(uint32_t);
            break;
        default:
            ASSERT( false, "scrValDefSave: unknown script variable type for save" );
            break;
    }
    return true;
}

/// default value load routine
BOOL scrValDefLoad(int32_t version, INTERP_VAL *psVal, char *pBuffer, uint32_t size)
{
    char			*pPos;
    DROID			*psCDroid;
    int32_t			index, members, savedMembers;
    uint32_t			id;
    LEVEL_DATASET	*psLevel;
    DROID_GROUP		*psGroup = NULL;
    const char              *pName;
    BOOL			bObjectDefined;

    switch (psVal->type)
    {
        case ST_INTMESSAGE:
            if ((size == 1) &&
                    (*pBuffer == 0))
            {
                psVal->v.oval = NULL;
            }
            else
            {
                psVal->v.oval = (void *)getViewData(pBuffer);
                if (psVal->v.oval == NULL)
                {
                    return false;
                }
            }
            break;
        case ST_BASEOBJECT:
        case ST_DROID:
        case ST_STRUCTURE:
        case ST_FEATURE:
            id = *((uint32_t *)pBuffer);
            endian_udword(&id);

            if (id == uint32_t_MAX)
            {
                psVal->v.oval = NULL;
            }
            else
            {
                psVal->v.oval = (void *)getBaseObjFromId(id);
                if (!psVal->v.oval)
                {
                    // apparently, this is a non-fatal error.
                    debug( LOG_INFO, "Couldn't find object id %u type %d", id, psVal->type );
                }
            }
            break;
        case ST_BASESTATS:
        case ST_COMPONENT:
            break;
        case ST_STRUCTURESTAT:
            index = getStructStatFromName(pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find structure stat %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_FEATURESTAT:
            index = getFeatureStatFromName(pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find feature stat %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_BODY:
            index = getCompFromResName(COMP_BODY, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find body component %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_PROPULSION:
            index = getCompFromResName(COMP_PROPULSION, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find propulsion component %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_ECM:
            index = getCompFromResName(COMP_ECM, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find ECM component %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_SENSOR:
            index = getCompFromResName(COMP_SENSOR, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find sensor component %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_CONSTRUCT:
            index = getCompFromResName(COMP_CONSTRUCT, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find constructor component %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_WEAPON:
            index = getCompFromResName(COMP_WEAPON, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find weapon %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_REPAIR:
            index = getCompFromResName(COMP_REPAIRUNIT, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find repair component %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_BRAIN:
            index = getCompFromResName(COMP_BRAIN, pBuffer);
            if (index == -1)
            {
                debug( LOG_FATAL, "scrValDefLoad: couldn't find repair brain %s", pBuffer );
                abort();
                index = 0;
            }
            psVal->v.ival = index;
            break;
        case ST_TEMPLATE:
            id = *((uint32_t *)pBuffer);
            endian_udword(&id);

            if (id == uint32_t_MAX)
            {
                psVal->v.oval = NULL;
            }
            else
            {
                psVal->v.oval = (void *)IdToTemplate(id, ANYPLAYER);
                if ((DROID_TEMPLATE *)(psVal->v.oval) == NULL)
                {
                    debug( LOG_FATAL, "scrValDefLoad: couldn't find template id %d", id );
                    abort();
                }
            }
            break;
        case ST_TEXTSTRING:
            if (version < 4)
            {
                if (size != sizeof(uint32_t))
                {
                    debug(LOG_ERROR, "Data size is too small, %u is expected, but %u is provided", (unsigned int)(sizeof(uint32_t)), (unsigned int)size);
                    return false;
                }

                id = *((uint32_t *)pBuffer);
                endian_udword(&id);

                if (id == uint32_t_MAX)
                {
                    psVal->v.sval = NULL;
                }
                else
                {
                    /* This code is commented out, because it depends on assigning the
                     * id-th loaded string from the string resources. And from version
                     * 4 of this file format onward, we do not count strings anymore.
                     *
                     * Thus loading of these strings is practically impossible.
                     */
                    debug(LOG_ERROR, "Incompatible savegame format version %u, should be at least version 4", (unsigned int)version);
                    return false;
                }
            }
            else
            {
                const char *str;
                char *idStr;
                uint16_t len;

                if (size < sizeof(len))
                {
                    debug(LOG_ERROR, "Data size is too small, %u is expected, but %u is provided", (unsigned int)(sizeof(len)), (unsigned int)size);
                    return false;
                }

                len = *((uint16_t *)pBuffer);
                endian_uword(&len);

                if (size < sizeof(len) + len)
                {
                    debug(LOG_ERROR, "Data size is too small, %u is expected, but %u is provided", (unsigned int)(sizeof(len) + len), (unsigned int)size);
                    return false;
                }

                if (len == 0)
                {
                    psVal->v.sval = NULL;
                    return true;
                }

                idStr = malloc(len);
                if (!idStr)
                {
                    debug(LOG_ERROR, "Out of memory (tried to allocate %u bytes)", (unsigned int)len);
                    // Don't abort() here, as this might be the result from a bad "len" field in the data
                    return false;
                }

                memcpy(idStr, pBuffer + sizeof(len), len);

                if (idStr[len - 1] != '\0')
                {
                    debug(LOG_WARNING, "Non-NUL terminated string encountered!");
                }
                idStr[len - 1] = '\0';

                str = strresGetString(psStringRes, idStr);
                if (!str)
                {
                    debug(LOG_ERROR, "Couldn't find string with id \"%s\"", idStr);
                    free(idStr);
                    return false;
                }
                free(idStr);

                psVal->v.sval = strdup(str);
                if (!psVal->v.sval)
                {
                    debug(LOG_FATAL, "Out of memory");
                    abort();
                    return false;
                }
            }
            break;
        case ST_LEVEL:
            if ((size == 1) &&
                    (*pBuffer == 0))
            {
                psVal->v.sval = '\0';
            }
            else
            {
                psLevel = levFindDataSet(pBuffer);
                if (psLevel == NULL)
                {
                    debug( LOG_FATAL, "scrValDefLoad: couldn't find level dataset %s", pBuffer );
                    abort();
                }
                psVal->v.sval = psLevel->pName;
            }
            break;
        case ST_RESEARCH:
            if ((size == 1) &&
                    (*pBuffer == 0))
            {
                psVal->v.oval = NULL;
            }
            else
            {
                psVal->v.oval = (void *)getResearch(pBuffer, true);
                if (psVal->v.oval == NULL)
                {
                    debug( LOG_FATAL, "scrValDefLoad: couldn't find research %s", pBuffer );
                    abort();
                }
            }
            break;
        case ST_GROUP:
            bObjectDefined = true;

            if (psVal->v.oval == NULL)
            {
                if (!grpCreate((DROID_GROUP **)&(psVal->v.oval)))
                {
                    debug( LOG_FATAL, "scrValDefLoad: out of memory" );
                    abort();
                    break;
                }
                grpJoin((DROID_GROUP *)(psVal->v.oval), NULL);
            }

            pPos = pBuffer;

            if (version < 2)
            {
                members = size / sizeof(uint32_t);
            }
            else if (version < 3)
            {
                members = (size - sizeof(int32_t)*4) / sizeof(uint32_t);
            }
            else
            {
                members = (size - sizeof(int32_t)*6) / sizeof(uint32_t);

                // get saved group member count/nullpointer flag
                endian_sdword((int32_t *)pPos);
                bObjectDefined = ( *((int32_t *)pPos) != UNALLOCATED_OBJECT );

                if(bObjectDefined)
                {
                    savedMembers = *((int32_t *)pPos);	// get number of saved group members

                    ASSERT(savedMembers == members, "scrValDefLoad: calculated and saved group member count did not match." );
                }
                pPos += sizeof(int32_t);
            }

            // make sure group was allocated when it was saved (relevant starting from version 3)
            if( version < 3 || bObjectDefined )
            {
                if (version >= 2)
                {
                    // load the retreat data
                    psGroup = (DROID_GROUP *)(psVal->v.oval);
                    endian_sdword((int32_t *)pPos);
                    psGroup->sRunData.sPos.x = *((int32_t *)pPos);
                    pPos += sizeof(int32_t);
                    endian_sdword((int32_t *)pPos);
                    psGroup->sRunData.sPos.y = *((int32_t *)pPos);
                    pPos += sizeof(int32_t);
                    endian_sdword((int32_t *)pPos);
                    psGroup->sRunData.forceLevel = (uint8_t)(*((int32_t *)pPos));
                    pPos += sizeof(int32_t);
                    endian_sdword((int32_t *)pPos);
                    psGroup->sRunData.leadership = (uint8_t)(*((int32_t *)pPos));
                    pPos += sizeof(int32_t);
                }
                if (version >= 3)
                {
                    endian_sdword((int32_t *)pPos);
                    psGroup->sRunData.healthLevel = (uint8_t)(*((int32_t *)pPos));
                    pPos += sizeof(int32_t);
                }

                // load the droids
                while (members > 0)
                {
                    endian_udword((uint32_t *)pPos);
                    id = *((uint32_t *) pPos);
                    psCDroid = (DROID *)getBaseObjFromId(id);
                    if (!psCDroid)
                    {
                        debug(LOG_INFO, "Could not find object id %d", id);
                    }
                    else
                    {
                        grpJoin((DROID_GROUP *)(psVal->v.oval), psCDroid);
                    }
                    pPos += sizeof(uint32_t);
                    members -= 1;
                }
            }
            else		// a group var was unallocated during saving
            {
                pPos += sizeof(uint16_t);
            }
            break;
        case ST_SOUND:
            // find audio id

            // don't use sound if it's disabled
            if (audio_Disabled())
            {
                psVal->v.ival = NO_SOUND;
                break;
            }

            pName = pBuffer;
            index = audio_GetTrackID( pName );
            if (index == SAMPLE_NOT_FOUND)
            {
                // find empty id and set track vals
                index = audio_SetTrackVals(pName, false, 100, 1800);
                if (!index)			//this is a NON fatal error.
                {
                    // We can't find filename of the sound for some reason.
                    debug(LOG_INFO, "Sound ID not available %s not found", pName);
                    break;
                }
            }
            psVal->v.ival = index;
            break;
        case ST_STRUCTUREID:
        case ST_DROIDID:
        default:
            // just set the contents directly
            psVal->v.ival = *((int32_t *)pBuffer);
            endian_sdword(&psVal->v.ival);
            break;
    }

    return true;
}
