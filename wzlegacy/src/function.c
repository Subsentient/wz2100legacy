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
 * Function.c
 *
 * Store function stats for the Structures etc.
 *
 */
#include "lib/framework/frame.h"
#include "lib/framework/strres.h"

#include "function.h"
#include "stats.h"
#include "structure.h"
#include "text.h"
#include "research.h"
#include "droid.h"
#include "group.h"

#include "multiplay.h"


//holder for all functions
FUNCTION **asFunctions;
uint32_t numFunctions;


typedef BOOL (*LoadFunction)(const char *pData);


/*Returns the Function type based on the string - used for reading in data */
static uint32_t functionType(const char *pType)
{
    if (!strcmp(pType, "Production"))
    {
        return PRODUCTION_TYPE;
    }
    if (!strcmp(pType, "Production Upgrade"))
    {
        return PRODUCTION_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "Research"))
    {
        return RESEARCH_TYPE;
    }
    if (!strcmp(pType, "Research Upgrade"))
    {
        return RESEARCH_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "Power Generator"))
    {
        return POWER_GEN_TYPE;
    }
    if (!strcmp(pType, "Resource"))
    {
        return RESOURCE_TYPE;
    }
    if (!strcmp(pType, "Repair Droid"))
    {
        return REPAIR_DROID_TYPE;
    }
    if (!strcmp(pType, "Weapon Upgrade"))
    {
        return WEAPON_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "Wall Function"))
    {
        return WALL_TYPE;
    }
    if (!strcmp(pType, "Structure Upgrade"))
    {
        return STRUCTURE_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "WallDefence Upgrade"))
    {
        return WALLDEFENCE_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "Power Upgrade"))
    {
        return POWER_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "Repair Upgrade"))
    {
        return REPAIR_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "VehicleRepair Upgrade"))
    {
        return DROIDREPAIR_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "VehicleECM Upgrade"))
    {
        return DROIDECM_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "VehicleConst Upgrade"))
    {
        return DROIDCONST_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "VehicleBody Upgrade"))
    {
        return DROIDBODY_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "VehicleSensor Upgrade"))
    {
        return DROIDSENSOR_UPGRADE_TYPE;
    }
    if (!strcmp(pType, "ReArm"))
    {
        return REARM_TYPE;
    }
    if (!strcmp(pType, "ReArm Upgrade"))
    {
        return REARM_UPGRADE_TYPE;
    }

    ASSERT( false, "Unknown Function Type: %s", pType );
    return 0;
}

// Allocate storage for the name
static BOOL storeName(FUNCTION *pFunction, const char *pNameToStore)
{
    pFunction->pName = strdup(pNameToStore);
    if (pFunction->pName == NULL)
    {
        debug( LOG_FATAL, "Function Name - Out of memory" );
        abort();
        return false;
    }

    return true;
}


static BOOL loadProduction(const char *pData)
{
    PRODUCTION_FUNCTION	*psFunction;
    //PROPULSION_TYPE propType;
    char					functionName[MAX_STR_LENGTH], bodySize[MAX_STR_LENGTH];
    uint32_t					productionOutput;
    //char					propulsionType[MAX_STR_LENGTH];
    //PROPULSION_TYPES*		pPropulsionType;
    //allocate storage

    psFunction = (PRODUCTION_FUNCTION *)malloc(sizeof(PRODUCTION_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Production Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(PRODUCTION_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = PRODUCTION_TYPE;

    //read the data in
    functionName[0] = '\0';
    //propulsionType[0] = '\0';
    bodySize[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%255[^,'\r\n],%d", functionName, bodySize,
           &productionOutput);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    //get the propulsion stats pointer
    /*	pPropulsionType = asPropulsionTypes;
    	psFunction->propulsionType = 0;
    	for (i=0; i < numPropulsionTypes; i++)
    	{
    		//compare the names
    		if (!strcmp(propulsionType, pPropulsionType->pName))
    		{
    			psFunction->propulsionType = pPropulsionType;
    			break;
    		}
    		pPropulsionType++;
    	}
    	//if haven't found the propulsion type, then problem
    	if (!psFunction->propulsionType)
    	{
    		DBERROR(("Unknown Propulsion Type"));
    		return false;
    	}
    */
#if 0
    if (!getPropulsionType(propulsionType, &propType))
    {
        ASSERT(!"Unknown propulsion type", "Unknown Propulsion Type: %s", propulsionType);
        return false;
    }
    psFunction->propulsionType = propType;
#endif

    if (!getBodySize(bodySize, &psFunction->capacity))
    {

        ASSERT( false, "loadProduction: unknown body size for %s",psFunction->pName );

        return false;
    }

    //check prod output < uint16_t_MAX
    if (productionOutput < uint16_t_MAX)
    {
        psFunction->productionOutput = (uint16_t)productionOutput;
    }
    else
    {

        ASSERT( false, "loadProduction: production Output too big for %s",psFunction->pName );

        psFunction->productionOutput = 0;
    }

    return true;
}

static BOOL loadProductionUpgradeFunction(const char *pData)
{
    PRODUCTION_UPGRADE_FUNCTION	*psFunction;
    char							functionName[MAX_STR_LENGTH];
    uint32_t							factory, cyborg, vtol;
    uint32_t outputModifier;

    //allocate storage
    psFunction = (PRODUCTION_UPGRADE_FUNCTION *)malloc(sizeof
                 (PRODUCTION_UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Production Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(PRODUCTION_UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = PRODUCTION_UPGRADE_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d,%d,%d,%d", functionName, &factory,
           &cyborg, &vtol,&outputModifier);


    psFunction->outputModifier=(uint8_t)outputModifier;
    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    //set the factory flags
    if (factory)
    {
        psFunction->factory = true;
    }
    else
    {
        psFunction->factory = false;
    }
    if (cyborg)
    {
        psFunction->cyborgFactory = true;
    }
    else
    {
        psFunction->cyborgFactory = false;
    }
    if (vtol)
    {
        psFunction->vtolFactory = true;
    }
    else
    {
        psFunction->vtolFactory = false;
    }

    //increment the number of upgrades
    //numProductionUpgrades++;
    return true;
}

static BOOL loadResearchFunction(const char *pData)
{
    RESEARCH_FUNCTION			*psFunction;
    char						functionName[MAX_STR_LENGTH];

    //allocate storage
    psFunction = (RESEARCH_FUNCTION *)malloc(sizeof(RESEARCH_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Research Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(RESEARCH_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = RESEARCH_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d", functionName, &psFunction->researchPoints);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    return true;
}

static BOOL loadReArmFunction(const char *pData)
{
    REARM_FUNCTION				*psFunction;
    char						functionName[MAX_STR_LENGTH];

    //allocate storage
    psFunction = (REARM_FUNCTION *)malloc(sizeof(REARM_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "ReArm Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(REARM_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = REARM_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d", functionName, &psFunction->reArmPoints);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    return true;
}


//generic load function for upgrade type
static BOOL loadUpgradeFunction(const char *pData, uint8_t type)
{
    char						functionName[MAX_STR_LENGTH];
    uint32_t						modifier;
    UPGRADE_FUNCTION			*psFunction;

    //allocate storage
    psFunction = (UPGRADE_FUNCTION *)malloc(sizeof(UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = type;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d", functionName, &modifier);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    if (modifier > uint16_t_MAX)
    {
        ASSERT( false, "loadUpgradeFunction: modifier too great for %s", functionName );
        return false;
    }

    //store the % upgrade
    psFunction->upgradePoints = (uint16_t)modifier;

    return true;
}



static BOOL loadResearchUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, RESEARCH_UPGRADE_TYPE);
}

static BOOL loadPowerUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, POWER_UPGRADE_TYPE);
}

static BOOL loadRepairUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, REPAIR_UPGRADE_TYPE);
}

static BOOL loadDroidRepairUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, DROIDREPAIR_UPGRADE_TYPE);
}

static BOOL loadDroidECMUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, DROIDECM_UPGRADE_TYPE);
}

static BOOL loadDroidConstUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, DROIDCONST_UPGRADE_TYPE);
}

static BOOL loadReArmUpgradeFunction(const char *pData)
{
    return loadUpgradeFunction(pData, REARM_UPGRADE_TYPE);
}


static BOOL loadDroidBodyUpgradeFunction(const char *pData)
{
    DROIDBODY_UPGRADE_FUNCTION		*psFunction;
    char							functionName[MAX_STR_LENGTH];
    uint32_t							modifier, armourKinetic, armourHeat,
                                    body, droid, cyborg;

    //allocate storage
    psFunction = (DROIDBODY_UPGRADE_FUNCTION *)malloc(
                     sizeof(DROIDBODY_UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "UnitBody Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(DROIDBODY_UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = DROIDBODY_UPGRADE_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d,%d,%d,%d,%d,%d", functionName, &modifier,
           &body, &armourKinetic,	&armourHeat, &droid, &cyborg);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    if (modifier > uint16_t_MAX || armourKinetic > uint16_t_MAX ||
            armourHeat > uint16_t_MAX || body > uint16_t_MAX)
    {
        ASSERT( false,
                "loadUnitBodyUpgradeFunction: one or more modifiers too great" );
        return false;
    }

    //store the % upgrades
    psFunction->upgradePoints = (uint16_t)modifier;
    psFunction->body = (uint16_t)body;
    psFunction->armourValue[WC_KINETIC] = (uint16_t)armourKinetic;
    psFunction->armourValue[WC_HEAT] = (uint16_t)armourHeat;
    if (droid)
    {
        psFunction->droid = true;
    }
    else
    {
        psFunction->droid = false;
    }
    if (cyborg)
    {
        psFunction->cyborg = true;
    }
    else
    {
        psFunction->cyborg = false;
    }

    return true;
}

static BOOL loadDroidSensorUpgradeFunction(const char *pData)
{
    DROIDSENSOR_UPGRADE_FUNCTION	*psFunction;
    char							functionName[MAX_STR_LENGTH];
    uint32_t							modifier, range;

    //allocate storage
    psFunction = (DROIDSENSOR_UPGRADE_FUNCTION *)malloc(
                     sizeof(DROIDSENSOR_UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "UnitSensor Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(DROIDSENSOR_UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = DROIDSENSOR_UPGRADE_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d,%d", functionName, &modifier, &range);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    if (modifier > uint16_t_MAX || range > uint16_t_MAX)
    {
        ASSERT( false,
                "loadUnitSensorUpgradeFunction: one or more modifiers too great" );
        return false;
    }

    //store the % upgrades
    psFunction->upgradePoints = (uint16_t)modifier;
    psFunction->range = (uint16_t)range;

    return true;
}

static BOOL loadWeaponUpgradeFunction(const char *pData)
{
    WEAPON_UPGRADE_FUNCTION	*psFunction;
    char						functionName[MAX_STR_LENGTH],
                                weaponSubClass[MAX_STR_LENGTH];
    uint32_t						firePause, shortHit, longHit, damage,
                                radiusDamage, incenDamage, radiusHit;

    //allocate storage
    psFunction = (WEAPON_UPGRADE_FUNCTION *)malloc(sizeof
                 (WEAPON_UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Weapon Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(WEAPON_UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = WEAPON_UPGRADE_TYPE;

    //read the data in
    functionName[0] = '\0';
    weaponSubClass[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%255[^,'\r\n],%d,%d,%d,%d,%d,%d,%d", functionName,
           weaponSubClass, &firePause, &shortHit, &longHit, &damage, &radiusDamage,
           &incenDamage, &radiusHit);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    if (!getWeaponSubClass(weaponSubClass, &psFunction->subClass))
    {
        return false;
    }

    //check none of the %increases are over uint8_t max
    if (firePause > uint8_t_MAX ||
            shortHit > uint16_t_MAX ||
            longHit > uint16_t_MAX ||
            damage > uint16_t_MAX ||
            radiusDamage > uint16_t_MAX ||
            incenDamage > uint16_t_MAX ||
            radiusHit > uint16_t_MAX)
    {
        debug( LOG_ERROR, "A percentage increase for Weapon Upgrade function is too large" );

        return false;
    }

    //copy the data across
    psFunction->firePause = (uint8_t)firePause;
    psFunction->shortHit = (uint16_t)shortHit;
    psFunction->longHit = (uint16_t)longHit;
    psFunction->damage = (uint16_t)damage;
    psFunction->radiusDamage = (uint16_t)radiusDamage;
    psFunction->incenDamage = (uint16_t)incenDamage;
    psFunction->radiusHit = (uint16_t)radiusHit;

    //increment the number of upgrades
    //numWeaponUpgrades++;

    return true;
}

static BOOL loadStructureUpgradeFunction(const char *pData)
{
    STRUCTURE_UPGRADE_FUNCTION  *psFunction;
    char						functionName[MAX_STR_LENGTH];
    uint32_t						armour, body, resistance;

    //allocate storage
    psFunction = (STRUCTURE_UPGRADE_FUNCTION *)malloc(sizeof
                 (STRUCTURE_UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Structure Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(STRUCTURE_UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = STRUCTURE_UPGRADE_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d,%d,%d", functionName, &armour, &body, &resistance);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    //check none of the %increases are over uint16_t max
    if (armour > uint16_t_MAX ||
            body > uint16_t_MAX ||
            resistance > uint16_t_MAX)
    {
        debug( LOG_ERROR, "A percentage increase for Structure Upgrade function is too large" );

        return false;
    }

    //copy the data across
    psFunction->armour = (uint16_t)armour;
    psFunction->body = (uint16_t)body;
    psFunction->resistance = (uint16_t)resistance;

    return true;
}

static BOOL loadWallDefenceUpgradeFunction(const char *pData)
{
    WALLDEFENCE_UPGRADE_FUNCTION  *psFunction;
    char						functionName[MAX_STR_LENGTH];
    uint32_t						armour, body;

    //allocate storage
    psFunction = (WALLDEFENCE_UPGRADE_FUNCTION *)malloc(sizeof
                 (WALLDEFENCE_UPGRADE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "WallDefence Upgrade Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(WALLDEFENCE_UPGRADE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = WALLDEFENCE_UPGRADE_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d,%d", functionName, &armour, &body);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    //check none of the %increases are over uint16_t max
    if (armour > uint16_t_MAX ||
            body > uint16_t_MAX)
    {
        debug( LOG_ERROR, "A percentage increase for WallDefence Upgrade function is too large" );

        return false;
    }

    //copy the data across
    psFunction->armour = (uint16_t)armour;
    psFunction->body = (uint16_t)body;

    return true;
}


static BOOL loadPowerGenFunction(const char *pData)
{
    POWER_GEN_FUNCTION			*psFunction;
    char						functionName[MAX_STR_LENGTH];

    //allocate storage
    psFunction = (POWER_GEN_FUNCTION *)malloc(sizeof
                 (POWER_GEN_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Power Gen Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(POWER_GEN_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = POWER_GEN_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d,%d,%d,%d,%d,%d", functionName,
           &psFunction->powerOutput, &psFunction->powerMultiplier,
           &psFunction->criticalMassChance, &psFunction->criticalMassRadius,
           &psFunction->criticalMassDamage, &psFunction->radiationDecayTime);


//	if(bMultiPlayer)
//	{
//		modifyResources(psFunction);
//	}


    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    return true;
}

static BOOL loadResourceFunction(const char *pData)
{
    RESOURCE_FUNCTION			*psFunction;
    char						functionName[MAX_STR_LENGTH];

    //allocate storage
    psFunction = (RESOURCE_FUNCTION *)malloc(sizeof
                 (RESOURCE_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Resource Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(RESOURCE_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = RESOURCE_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d", functionName, &psFunction->maxPower);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    return true;
}


static BOOL loadRepairDroidFunction(const char *pData)
{
    REPAIR_DROID_FUNCTION		*psFunction;
    char						functionName[MAX_STR_LENGTH];

    //allocate storage
    psFunction = (REPAIR_DROID_FUNCTION *)malloc(sizeof
                 (REPAIR_DROID_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Repair Droid Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(REPAIR_DROID_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = REPAIR_DROID_TYPE;

    //read the data in
    functionName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%d", functionName,
           &psFunction->repairPoints);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    return true;
}


/*loads the corner stat to use for a particular wall stat */
static BOOL loadWallFunction(const char *pData)
{
    WALL_FUNCTION			*psFunction;
//	uint32_t					i;
    char					functionName[MAX_STR_LENGTH];
    char					structureName[MAX_STR_LENGTH];
//	STRUCTURE_STATS			*pStructStat;

    //allocate storage
    psFunction = (WALL_FUNCTION *)malloc(sizeof(WALL_FUNCTION));
    if (psFunction == NULL)
    {
        debug( LOG_FATAL, "Wall Function - Out of memory" );
        abort();
        return false;
    }
    memset(psFunction, 0, sizeof(WALL_FUNCTION));

    //store the pointer in the Function Array
    *asFunctions = (FUNCTION *)psFunction;
    psFunction->ref = REF_FUNCTION_START + numFunctions;
    numFunctions++;
    asFunctions++;

    //set the type of function
    psFunction->type = WALL_TYPE;

    //read the data in
    functionName[0] = '\0';
    structureName[0] = '\0';
    sscanf(pData, "%255[^,'\r\n],%255[^,'\r\n],%*d", functionName, structureName);

    //allocate storage for the name
    storeName((FUNCTION *)psFunction, functionName);

    //store the structure name - cannot set the stat pointer here because structures
    //haven't been loaded in yet!
    psFunction->pStructName = allocateName(structureName);
    if (!psFunction->pStructName)
    {
        debug( LOG_ERROR, "Structure Stats Invalid for function - %s", functionName );

        return false;
    }
    psFunction->pCornerStat = NULL;

    return true;
}

void productionUpgrade(FUNCTION *pFunction, uint8_t player)
{
    PRODUCTION_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (PRODUCTION_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values
    if (pUpgrade->factory)
    {
        if (asProductionUpgrade[player][FACTORY_FLAG].modifier <
                pUpgrade->outputModifier)
        {
            asProductionUpgrade[player][FACTORY_FLAG].modifier =
                pUpgrade->outputModifier;
        }
    }
    if (pUpgrade->cyborgFactory)
    {
        if (asProductionUpgrade[player][CYBORG_FLAG].modifier <
                pUpgrade->outputModifier)
        {
            asProductionUpgrade[player][CYBORG_FLAG].modifier =
                pUpgrade->outputModifier;
        }
    }
    if (pUpgrade->vtolFactory)
    {
        if (asProductionUpgrade[player][VTOL_FLAG].modifier <
                pUpgrade->outputModifier)
        {
            asProductionUpgrade[player][VTOL_FLAG].modifier =
                pUpgrade->outputModifier;
        }
    }
}

void researchUpgrade(FUNCTION *pFunction, uint8_t player)
{
    RESEARCH_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (RESEARCH_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values
    if (asResearchUpgrade[player].modifier < pUpgrade->upgradePoints)
    {
        asResearchUpgrade[player].modifier = pUpgrade->upgradePoints;
    }
}

void repairFacUpgrade(FUNCTION *pFunction, uint8_t player)
{
    REPAIR_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (REPAIR_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values
    if (asRepairFacUpgrade[player].modifier < pUpgrade->upgradePoints)
    {
        asRepairFacUpgrade[player].modifier = pUpgrade->upgradePoints;
    }
}

void powerUpgrade(FUNCTION *pFunction, uint8_t player)
{
    POWER_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (POWER_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values
    if (asPowerUpgrade[player].modifier < pUpgrade->upgradePoints)
    {
        asPowerUpgrade[player].modifier = pUpgrade->upgradePoints;
    }
}

void reArmUpgrade(FUNCTION *pFunction, uint8_t player)
{
    REARM_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (REARM_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values
    if (asReArmUpgrade[player].modifier < pUpgrade->upgradePoints)
    {
        asReArmUpgrade[player].modifier = pUpgrade->upgradePoints;
    }
}

void structureBodyUpgrade(FUNCTION *pFunction, STRUCTURE *psBuilding)
{
    uint16_t	increase, prevBaseBody, newBaseBody;

    switch(psBuilding->pStructureType->type)
    {
        case REF_WALL:
        case REF_WALLCORNER:
        case REF_DEFENSE:
        case REF_BLASTDOOR:
            increase = ((WALLDEFENCE_UPGRADE_FUNCTION *)pFunction)->body;
            break;
        default:
            increase = ((STRUCTURE_UPGRADE_FUNCTION *)pFunction)->body;
            break;
    }

    prevBaseBody = (uint16_t)structureBody(psBuilding);
    //newBaseBody = (uint16_t)(psBuilding->pStructureType->bodyPoints + (psBuilding->
    //	pStructureType->bodyPoints * increase) / 100);
    newBaseBody = (uint16_t)(structureBaseBody(psBuilding) +
                          (structureBaseBody(psBuilding) * increase) / 100);

    if (newBaseBody > prevBaseBody)
    {
        psBuilding->body = (uint16_t)((psBuilding->body * newBaseBody) / prevBaseBody);
        //psBuilding->baseBodyPoints = newBaseBody;
    }
}

void structureArmourUpgrade(FUNCTION *pFunction, STRUCTURE *psBuilding)
{
    uint16_t	increase, prevBaseArmour, newBaseArmour, i, j;

    switch(psBuilding->pStructureType->type)
    {
        case REF_WALL:
        case REF_WALLCORNER:
        case REF_DEFENSE:
        case REF_BLASTDOOR:
            increase = ((WALLDEFENCE_UPGRADE_FUNCTION *)pFunction)->armour;
            break;
        default:
            increase = ((STRUCTURE_UPGRADE_FUNCTION *)pFunction)->armour;
            break;
    }

    prevBaseArmour = (uint16_t)structureArmour(psBuilding->pStructureType, psBuilding->player);
    newBaseArmour = (uint16_t)(psBuilding->pStructureType->armourValue + (psBuilding->
                            pStructureType->armourValue * increase) / 100);

    if (newBaseArmour > prevBaseArmour)
    {
        // TODO: support advanced armour system
        for (i = 0; i < NUM_HIT_SIDES; i++)
        {
            for (j = 0; j < WC_NUM_WEAPON_CLASSES; j++)
            {
                psBuilding->armour[i][j] = (uint16_t)((psBuilding->armour[i][j] * newBaseArmour) / prevBaseArmour);
            }
        }
    }
}

void structureResistanceUpgrade(FUNCTION *pFunction, STRUCTURE *psBuilding)
{
    uint16_t	increase, prevBaseResistance, newBaseResistance;

    increase = ((STRUCTURE_UPGRADE_FUNCTION *)pFunction)->resistance;

    prevBaseResistance = (uint16_t)structureResistance(psBuilding->pStructureType,
                         psBuilding->player);
    newBaseResistance = (uint16_t)(psBuilding->pStructureType->resistance + (psBuilding
                                ->pStructureType->resistance * increase) / 100);

    if (newBaseResistance > prevBaseResistance)
    {
        psBuilding->resistance = (uint16_t)((psBuilding->resistance * newBaseResistance) /
                                         prevBaseResistance);
    }
}

void structureProductionUpgrade(STRUCTURE *psBuilding)
{
    FACTORY						*pFact;
    PRODUCTION_FUNCTION			*pFactFunc;
    uint32_t						type, baseOutput, i;
    STRUCTURE_STATS             *psStat;

    switch(psBuilding->pStructureType->type)
    {
        case REF_FACTORY:
            type = FACTORY_FLAG;
            break;
        case REF_CYBORG_FACTORY:
            type = CYBORG_FLAG;
            break;
        case REF_VTOL_FACTORY:
            type = VTOL_FLAG;
            break;
        default:
            ASSERT(!"invalid or not a factory type", "structureProductionUpgrade: Invalid factory type");
            return;
    }

    //upgrade the Output
    pFact = &psBuilding->pFunctionality->factory;
    ASSERT( pFact != NULL,
            "structureProductionUpgrade: invalid Factory pointer" );

    pFactFunc = (PRODUCTION_FUNCTION *)psBuilding->pStructureType->asFuncList[0];
    ASSERT( pFactFunc != NULL,
            "structureProductionUpgrade: invalid Function pointer" );

    //current base value depends on whether there are modules attached to the structure
    baseOutput = pFactFunc->productionOutput;
    psStat = getModuleStat(psBuilding);
    if (psStat)
    {
        for (i = 0; i < pFact->capacity; i++)
        {
            baseOutput += ((PRODUCTION_FUNCTION *)psStat->asFuncList[0])->productionOutput;
        }
    }

    pFact->productionOutput = (uint8_t)(baseOutput + (pFactFunc->productionOutput *
                                      asProductionUpgrade[psBuilding->player][type].modifier) / 100);
}

void structureResearchUpgrade(STRUCTURE *psBuilding)
{
    RESEARCH_FACILITY			*pRes = &psBuilding->pFunctionality->researchFacility;
    RESEARCH_FUNCTION			*pResFunc;
    uint32_t                       baseOutput;
    STRUCTURE_STATS             *psStat;

    //upgrade the research points
    ASSERT(pRes != NULL, "structureResearchUpgrade: invalid Research pointer");

    pResFunc = (RESEARCH_FUNCTION *)psBuilding->pStructureType->asFuncList[0];
    ASSERT( pResFunc != NULL,
            "structureResearchUpgrade: invalid Function pointer" );

    //current base value depends on whether there are modules attached to the structure
    baseOutput = pResFunc->researchPoints;
    psStat = getModuleStat(psBuilding);
    if (psStat)
    {
        if (pRes->capacity)
        {
            baseOutput += ((RESEARCH_FUNCTION *)psStat->asFuncList[0])->researchPoints;
        }
    }
    pRes->researchPoints = baseOutput + (pResFunc->researchPoints *
                                         asResearchUpgrade[psBuilding->player].modifier) / 100;
}

void structureReArmUpgrade(STRUCTURE *psBuilding)
{
    REARM_PAD					*pPad = &psBuilding->pFunctionality->rearmPad;
    REARM_FUNCTION				*pPadFunc;

    //upgrade the reArm points
    ASSERT(pPad != NULL, "structureReArmUpgrade: invalid ReArm pointer");

    pPadFunc = (REARM_FUNCTION *)psBuilding->pStructureType->asFuncList[0];
    ASSERT( pPadFunc != NULL,
            "structureReArmUpgrade: invalid Function pointer" );

    pPad->reArmPoints = pPadFunc->reArmPoints + (pPadFunc->reArmPoints *
                        asReArmUpgrade[psBuilding->player].modifier) / 100;
}

void structurePowerUpgrade(STRUCTURE *psBuilding)
{
    POWER_GEN		*pPowerGen = &psBuilding->pFunctionality->powerGenerator;
    POWER_GEN_FUNCTION	*pPGFunc = (POWER_GEN_FUNCTION *)psBuilding->pStructureType->asFuncList[0];
    uint32_t			multiplier;
    STRUCTURE_STATS		*psStat;

    ASSERT(pPowerGen != NULL, "Invalid Power Gen pointer");
    ASSERT(pPGFunc != NULL, "Invalid function pointer");

    // Current base value depends on whether there are modules attached to the structure
    multiplier = pPGFunc->powerMultiplier;
    psStat = getModuleStat(psBuilding);
    if (psStat)
    {
        if (pPowerGen->capacity)
        {
            multiplier += ((POWER_GEN_FUNCTION *)psStat->asFuncList[0])->powerMultiplier;
        }
    }
    pPowerGen->multiplier = multiplier + (pPGFunc->powerMultiplier * asPowerUpgrade[psBuilding->player].modifier) / 100;
}

void structureRepairUpgrade(STRUCTURE *psBuilding)
{
    REPAIR_FACILITY			*pRepair = &psBuilding->pFunctionality->repairFacility;
    REPAIR_DROID_FUNCTION	*pRepairFunc;

    //upgrade the research points
    ASSERT(pRepair != NULL, "structureRepairUpgrade: invalid Repair pointer");

    pRepairFunc = (REPAIR_DROID_FUNCTION *)psBuilding->pStructureType->asFuncList[0];
    ASSERT( pRepairFunc != NULL,
            "structureRepairUpgrade: invalid Function pointer" );

    pRepair->power = pRepairFunc->repairPoints + (pRepairFunc->repairPoints *
                     asRepairFacUpgrade[psBuilding->player].modifier) / 100;
}

void structureSensorUpgrade(STRUCTURE *psBuilding)
{
    //reallocate the sensor range and power since the upgrade
    if (psBuilding->pStructureType->pSensor)
    {
        psBuilding->sensorRange = (uint16_t)sensorRange(psBuilding->pStructureType->
                                  pSensor, psBuilding->player);
        psBuilding->sensorPower = (uint16_t)sensorPower(psBuilding->pStructureType->
                                  pSensor, psBuilding->player);
    }
    else
    {
        //give them the default sensor for droids if not
        psBuilding->sensorRange = (uint16_t)sensorRange(asSensorStats +
                                  aDefaultSensor[psBuilding->player], psBuilding->player);
        psBuilding->sensorPower = (uint16_t)sensorPower(asSensorStats +
                                  aDefaultSensor[psBuilding->player], psBuilding->player);
    }
}

void structureECMUpgrade(STRUCTURE *psBuilding)
{
    //reallocate the sensor range and power since the upgrade
    if (psBuilding->pStructureType->pECM)
    {
        psBuilding->ECMMod = (uint16_t)ecmPower(psBuilding->pStructureType->pECM, psBuilding->player);
    }
    else
    {
        psBuilding->ECMMod = 0;
    }
}

void droidSensorUpgrade(DROID *psDroid)
{
    //reallocate the sensor range and power since the upgrade
    psDroid->sensorRange = sensorRange((asSensorStats + psDroid->asBits
                                        [COMP_SENSOR].nStat), psDroid->player);
    psDroid->sensorPower = sensorPower((asSensorStats + psDroid->asBits
                                        [COMP_SENSOR].nStat), psDroid->player);
}

void droidECMUpgrade(DROID *psDroid)
{
    //reallocate the ecm power since the upgrade
    psDroid->ECMMod = ecmPower((asECMStats + psDroid->asBits[COMP_ECM].nStat),
                               psDroid->player);
}

void droidBodyUpgrade(FUNCTION *pFunction, DROID *psDroid)
{
    uint32_t	increase, prevBaseBody, newBaseBody, base;
    DROID   *psCurr;

    increase = ((DROIDBODY_UPGRADE_FUNCTION *)pFunction)->body;

    prevBaseBody = psDroid->originalBody;
    base = calcDroidBaseBody(psDroid);
    newBaseBody =  base + (base * increase) / 100;

    if (newBaseBody > prevBaseBody)
    {
        psDroid->body = (psDroid->body * newBaseBody) / prevBaseBody;
        psDroid->originalBody = newBaseBody;
    }
    //if a transporter droid then need to upgrade the contents
    if (psDroid->droidType == DROID_TRANSPORTER)
    {
        for (psCurr = psDroid->psGroup->psList; psCurr != NULL; psCurr =
                    psCurr->psGrpNext)
        {
            if (psCurr != psDroid)
            {
                droidBodyUpgrade(pFunction, psCurr);
            }
        }
    }
}

//upgrade the weapon stats for the correct subclass
void weaponUpgrade(FUNCTION *pFunction, uint8_t player)
{
    WEAPON_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (WEAPON_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asWeaponUpgrade[player][pUpgrade->subClass].firePause < pUpgrade->firePause)
    {
        //make sure don't go less than 100%
        if (pUpgrade->firePause > 100)
        {
            pUpgrade->firePause = 100;
        }
        asWeaponUpgrade[player][pUpgrade->subClass].firePause = pUpgrade->firePause;
    }
    if (asWeaponUpgrade[player][pUpgrade->subClass].shortHit < pUpgrade->shortHit)
    {
        asWeaponUpgrade[player][pUpgrade->subClass].shortHit = pUpgrade->shortHit;
    }
    if (asWeaponUpgrade[player][pUpgrade->subClass].longHit < pUpgrade->longHit)
    {
        asWeaponUpgrade[player][pUpgrade->subClass].longHit = pUpgrade->longHit;
    }
    if (asWeaponUpgrade[player][pUpgrade->subClass].damage < pUpgrade->damage)
    {
        asWeaponUpgrade[player][pUpgrade->subClass].damage = pUpgrade->damage;
    }
    if (asWeaponUpgrade[player][pUpgrade->subClass].radiusDamage < pUpgrade->radiusDamage)
    {
        asWeaponUpgrade[player][pUpgrade->subClass].radiusDamage = pUpgrade->radiusDamage;
    }
    if (asWeaponUpgrade[player][pUpgrade->subClass].incenDamage < pUpgrade->incenDamage)
    {
        asWeaponUpgrade[player][pUpgrade->subClass].incenDamage = pUpgrade->incenDamage;
    }
    if (asWeaponUpgrade[player][pUpgrade->subClass].radiusHit < pUpgrade->radiusHit)
    {
        asWeaponUpgrade[player][pUpgrade->subClass].radiusHit = pUpgrade->radiusHit;
    }
}

//upgrade the sensor stats
void sensorUpgrade(FUNCTION *pFunction, uint8_t player)
{
    DROIDSENSOR_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (DROIDSENSOR_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asSensorUpgrade[player].range < pUpgrade->range)
    {
        asSensorUpgrade[player].range = pUpgrade->range;
    }
    if (asSensorUpgrade[player].power < pUpgrade->upgradePoints)
    {
        asSensorUpgrade[player].power = pUpgrade->upgradePoints;
    }
}

//upgrade the repair stats
void repairUpgrade(FUNCTION *pFunction, uint8_t player)
{
    DROIDREPAIR_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (DROIDREPAIR_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asRepairUpgrade[player].repairPoints < pUpgrade->upgradePoints)
    {
        asRepairUpgrade[player].repairPoints = pUpgrade->upgradePoints;
    }
}

//upgrade the repair stats
void ecmUpgrade(FUNCTION *pFunction, uint8_t player)
{
    DROIDECM_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (DROIDECM_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asECMUpgrade[player].power < pUpgrade->upgradePoints)
    {
        asECMUpgrade[player].power = pUpgrade->upgradePoints;
    }
}

//upgrade the repair stats
void constructorUpgrade(FUNCTION *pFunction, uint8_t player)
{
    DROIDCONSTR_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (DROIDCONSTR_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asConstUpgrade[player].constructPoints < pUpgrade->upgradePoints)
    {
        asConstUpgrade[player].constructPoints = pUpgrade->upgradePoints;
    }
}

//upgrade the body stats
void bodyUpgrade(FUNCTION *pFunction, uint8_t player)
{
    DROIDBODY_UPGRADE_FUNCTION		*pUpgrade;
    uint8_t							inc;

    pUpgrade = (DROIDBODY_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (pUpgrade->droid)
    {
        if (asBodyUpgrade[player][DROID_BODY_UPGRADE].powerOutput <
                pUpgrade->upgradePoints)
        {
            asBodyUpgrade[player][DROID_BODY_UPGRADE].powerOutput =
                pUpgrade->upgradePoints;
        }
        if (asBodyUpgrade[player][DROID_BODY_UPGRADE].body <
                pUpgrade->body)
        {
            asBodyUpgrade[player][DROID_BODY_UPGRADE].body =
                pUpgrade->body;
        }
        for (inc=0; inc < WC_NUM_WEAPON_CLASSES; inc++)
        {
            if (asBodyUpgrade[player][DROID_BODY_UPGRADE].armourValue[inc] <
                    pUpgrade->armourValue[inc])
            {
                asBodyUpgrade[player][DROID_BODY_UPGRADE].armourValue[inc] =
                    pUpgrade->armourValue[inc];
            }
        }
    }
    if (pUpgrade->cyborg)
    {
        if (asBodyUpgrade[player][CYBORG_BODY_UPGRADE].powerOutput <
                pUpgrade->upgradePoints)
        {
            asBodyUpgrade[player][CYBORG_BODY_UPGRADE].powerOutput =
                pUpgrade->upgradePoints;
        }
        if (asBodyUpgrade[player][CYBORG_BODY_UPGRADE].body <
                pUpgrade->body)
        {
            asBodyUpgrade[player][CYBORG_BODY_UPGRADE].body =
                pUpgrade->body;
        }
        for (inc=0; inc < WC_NUM_WEAPON_CLASSES; inc++)
        {
            if (asBodyUpgrade[player][CYBORG_BODY_UPGRADE].armourValue[inc] <
                    pUpgrade->armourValue[inc])
            {
                asBodyUpgrade[player][CYBORG_BODY_UPGRADE].armourValue[inc] =
                    pUpgrade->armourValue[inc];
            }
        }
    }
}

//upgrade the structure stats for the correct player
void structureUpgrade(FUNCTION *pFunction, uint8_t player)
{
    STRUCTURE_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (STRUCTURE_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asStructureUpgrade[player].armour < pUpgrade->armour)
    {
        asStructureUpgrade[player].armour = pUpgrade->armour;
    }
    if (asStructureUpgrade[player].body < pUpgrade->body)
    {
        asStructureUpgrade[player].body = pUpgrade->body;
    }
    if (asStructureUpgrade[player].resistance < pUpgrade->resistance)
    {
        asStructureUpgrade[player].resistance = pUpgrade->resistance;
    }
}

//upgrade the wall/Defence structure stats for the correct player
void wallDefenceUpgrade(FUNCTION *pFunction, uint8_t player)
{
    WALLDEFENCE_UPGRADE_FUNCTION		*pUpgrade;

    pUpgrade = (WALLDEFENCE_UPGRADE_FUNCTION *)pFunction;

    //check upgrades increase all values!
    if (asWallDefenceUpgrade[player].armour < pUpgrade->armour)
    {
        asWallDefenceUpgrade[player].armour = pUpgrade->armour;
    }
    if (asWallDefenceUpgrade[player].body < pUpgrade->body)
    {
        asWallDefenceUpgrade[player].body = pUpgrade->body;
    }
}

/*upgrades the droids inside a Transporter uwith the appropriate upgrade function*/
void upgradeTransporterDroids(DROID *psTransporter,
                              void(*pUpgradeFunction)(DROID *psDroid))
{
    DROID   *psCurr;

    ASSERT( psTransporter->droidType == DROID_TRANSPORTER,
            "upgradeTransporterUnits: invalid unit type" );

    //loop thru' each unit in the Transporter
    for (psCurr = psTransporter->psGroup->psList; psCurr != NULL; psCurr =
                psCurr->psGrpNext)
    {
        if (psCurr != psTransporter)
        {
            //apply upgrade if not the transporter itself
            pUpgradeFunction(psCurr);
        }
    }
}

BOOL FunctionShutDown(void)
{
    uint32_t		inc;//, player;
    FUNCTION	*pFunction, **pStartList = asFunctions;

    for (inc=0; inc < numFunctions; inc++)
    {
        pFunction = *asFunctions;
        free(pFunction->pName);
        free(pFunction);
        asFunctions++;
    }
    free(pStartList);

    return true;
}

BOOL loadFunctionStats(const char *pFunctionData, uint32_t bufferSize)
{
    //array of functions pointers for each load function
    static const LoadFunction pLoadFunction[NUMFUNCTIONS] =
    {
        loadProduction,
        loadProductionUpgradeFunction,
        loadResearchFunction,
        loadResearchUpgradeFunction,
        loadPowerGenFunction,
        loadResourceFunction,
        loadRepairDroidFunction,
        loadWeaponUpgradeFunction,
        loadWallFunction,
        loadStructureUpgradeFunction,
        loadWallDefenceUpgradeFunction,
        loadPowerUpgradeFunction,
        loadRepairUpgradeFunction,
        loadDroidRepairUpgradeFunction,
        loadDroidECMUpgradeFunction,
        loadDroidBodyUpgradeFunction,
        loadDroidSensorUpgradeFunction,
        loadDroidConstUpgradeFunction,
        loadReArmFunction,
        loadReArmUpgradeFunction,
    };

    const unsigned int totalFunctions = numCR(pFunctionData, bufferSize);
    uint32_t		i, type;
    char		FunctionType[MAX_STR_LENGTH];
    FUNCTION	**pStartList;

    //allocate storage for the Function pointer array
    asFunctions = (FUNCTION **) malloc(totalFunctions*sizeof(FUNCTION *));
    if (!asFunctions)
    {
        debug( LOG_FATAL, "Out of memory" );
        abort();
        return false;
    }
    pStartList = asFunctions;
    //initialise the storage
    memset(asFunctions, 0, totalFunctions*sizeof(FUNCTION *));
    numFunctions = 0;
    //numProductionUpgrades =	numResearchUpgrades = 0;//numArmourUpgrades =
    //numRepairUpgrades = numResistanceUpgrades = numBodyUpgrades =
    //numWeaponUpgrades = 0;

    for (i=0; i < totalFunctions; i++)
    {
        //read the data into the storage - the data is delimeted using comma's
        FunctionType[0] = '\0';
        sscanf(pFunctionData, "%255[^,'\r\n]", FunctionType);
        type = functionType(FunctionType);
        pFunctionData += (strlen(FunctionType)+1);

        if (!(pLoadFunction[type](pFunctionData)))
        {
            return false;
        }
        //increment the pointer to the start of the next record
        pFunctionData = strchr(pFunctionData,'\n') + 1;
    }
    //set the function list pointer to the start
    asFunctions = pStartList;

    return true;
}
