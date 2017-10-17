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
 *  Interface to the common stats module
 */
#ifndef __INCLUDED_SRC_STATS_H__
#define __INCLUDED_SRC_STATS_H__

#include "objectdef.h"
/**************************************************************************************
 *
 * Function prototypes and data storage for the stats
 */

/* The stores for the different stats */
extern BODY_STATS			*asBodyStats;
extern BRAIN_STATS			*asBrainStats;
extern PROPULSION_STATS		*asPropulsionStats;
extern SENSOR_STATS			*asSensorStats;
extern ECM_STATS			*asECMStats;
//extern ARMOUR_STATS			*asArmourStats;
extern REPAIR_STATS			*asRepairStats;
extern WEAPON_STATS			*asWeaponStats;
extern CONSTRUCT_STATS		*asConstructStats;
extern PROPULSION_TYPES		*asPropulsionTypes;

//used to hold the modifiers cross refd by weapon effect and propulsion type
extern WEAPON_MODIFIER		asWeaponModifier[WE_NUMEFFECTS][PROPULSION_TYPE_NUM];

//used to hold the current upgrade level per player per weapon subclass
extern WEAPON_UPGRADE		asWeaponUpgrade[MAX_PLAYERS][WSC_NUM_WEAPON_SUBCLASSES];
extern SENSOR_UPGRADE		asSensorUpgrade[MAX_PLAYERS];
extern ECM_UPGRADE			asECMUpgrade[MAX_PLAYERS];
extern REPAIR_UPGRADE		asRepairUpgrade[MAX_PLAYERS];
extern CONSTRUCTOR_UPGRADE	asConstUpgrade[MAX_PLAYERS];
//body upgrades are possible for droids and/or cyborgs
#define		DROID_BODY_UPGRADE	0
#define		CYBORG_BODY_UPGRADE	1
#define		BODY_TYPE		2
extern BODY_UPGRADE			asBodyUpgrade[MAX_PLAYERS][BODY_TYPE];

/* The number of different stats stored */
extern uint32_t		numBodyStats;
extern uint32_t		numBrainStats;
extern uint32_t		numPropulsionStats;
extern uint32_t		numSensorStats;
extern uint32_t		numECMStats;
extern uint32_t		numRepairStats;
extern uint32_t		numWeaponStats;
extern uint32_t		numConstructStats;
extern uint32_t		numTerrainTypes;

/* What number the ref numbers start at for each type of stat */
#define REF_BODY_START			0x010000
#define REF_BRAIN_START			0x020000
//#define REF_POWER_START			0x030000
#define REF_PROPULSION_START	0x040000
#define REF_SENSOR_START		0x050000
#define REF_ECM_START			0x060000
//#define REF_ARMOUR_START		0x070000
#define REF_REPAIR_START		0x080000
#define REF_WEAPON_START		0x0a0000
#define REF_RESEARCH_START		0x0b0000
#define REF_TEMPLATE_START		0x0c0000
#define REF_STRUCTURE_START		0x0d0000
#define REF_FUNCTION_START		0x0e0000
#define REF_CONSTRUCT_START		0x0f0000
#define REF_FEATURE_START		0x100000

/* The maximum number of refs for a type of stat */
#define REF_RANGE				0x010000


//stores for each players component states - see below
extern uint8_t		*apCompLists[MAX_PLAYERS][COMP_NUMCOMPONENTS];

//store for each players Structure states
extern uint8_t		*apStructTypeLists[MAX_PLAYERS];

//flags to fill apCompLists and apStructTypeLists
#define AVAILABLE				0x01		//this item can be used to design droids
#define UNAVAILABLE				0x02		//the player does not know about this item
#define FOUND					0x04		//this item has been found, but is unresearched
#define REDUNDANT				0x0A		//the player no longer needs this item

/*******************************************************************************
*		Allocate stats functions
*******************************************************************************/
/* Allocate Weapon stats */
extern BOOL statsAllocWeapons(uint32_t numEntries);

/*Allocate Armour stats*/
//extern BOOL statsAllocArmour(uint32_t numEntries);

/*Allocate Body stats*/
extern BOOL statsAllocBody(uint32_t numEntries);

/*Allocate Brain stats*/
extern BOOL statsAllocBrain(uint32_t numEntries);

/*Allocate Power stats*/
//extern BOOL statsAllocPower(uint32_t numEntries);

/*Allocate Propulsion stats*/
extern BOOL statsAllocPropulsion(uint32_t numEntries);

/*Allocate Sensor stats*/
extern BOOL statsAllocSensor(uint32_t numEntries);

/*Allocate Ecm Stats*/
extern BOOL statsAllocECM(uint32_t numEntries);

/*Allocate Repair Stats*/
extern BOOL statsAllocRepair(uint32_t numEntries);

/*Allocate Construct Stats*/
extern BOOL statsAllocConstruct(uint32_t numEntries);

extern uint16_t weaponROF(WEAPON_STATS *psStat, int8_t player);

/*******************************************************************************
*		Load stats functions
*******************************************************************************/
/* Return the number of newlines in a file buffer */
extern uint32_t numCR(const char *pFileBuffer, uint32_t fileSize);

/*Load the weapon stats from the file exported from Access*/
extern BOOL loadWeaponStats(const char *pWeaponData, uint32_t bufferSize);

/*Load the armour stats from the file exported from Access*/
//extern BOOL loadArmourStats(void);

/*Load the body stats from the file exported from Access*/
extern BOOL loadBodyStats(const char *pBodyData, uint32_t bufferSize);

/*Load the brain stats from the file exported from Access*/
extern BOOL loadBrainStats(const char *pBrainData, uint32_t bufferSize);

/*Load the power stats from the file exported from Access*/
//extern BOOL loadPowerStats(void);

/*Load the propulsion stats from the file exported from Access*/
extern BOOL loadPropulsionStats(const char *pPropulsionData, uint32_t bufferSize);

/*Load the sensor stats from the file exported from Access*/
extern BOOL loadSensorStats(const char *pSensorData, uint32_t bufferSize);

/*Load the ecm stats from the file exported from Access*/
extern BOOL loadECMStats(const char *pECMData, uint32_t bufferSize);

/*Load the repair stats from the file exported from Access*/
extern BOOL loadRepairStats(const char *pRepairData, uint32_t bufferSize);

/*Load the construct stats from the file exported from Access*/
extern BOOL loadConstructStats(const char *pConstructData, uint32_t bufferSize);

/*Load the Propulsion Types from the file exported from Access*/
extern BOOL loadPropulsionTypes(const char *pPropTypeData, uint32_t bufferSize);

/*Load the propulsion sounds from the file exported from Access*/
extern BOOL loadPropulsionSounds(const char *pSoundData, uint32_t bufferSize);

/*Load the Terrain Table from the file exported from Access*/
extern BOOL loadTerrainTable(const char *pTerrainTableData, uint32_t bufferSize);

/*Load the Special Ability stats from the file exported from Access*/
extern BOOL loadSpecialAbility(const char *pSAbilityData, uint32_t bufferSize);

/* load the IMDs to use for each body-propulsion combination */
extern BOOL loadBodyPropulsionIMDs(const char *pData, uint32_t bufferSize);

/*Load the weapon sounds from the file exported from Access*/
extern BOOL loadWeaponSounds(const char *pSoundData, uint32_t bufferSize);

/*Load the Weapon Effect Modifiers from the file exported from Access*/
extern BOOL loadWeaponModifiers(const char *pWeapModData, uint32_t bufferSize);
/*******************************************************************************
*		Set stats functions
*******************************************************************************/
/* Set the stats for a particular weapon type
 * The function uses the ref number in the stats structure to
 * index the correct array entry
 */
extern void statsSetWeapon(WEAPON_STATS	*psStats, uint32_t index);

/*Set the stats for a particular armour type*/
//extern void statsSetArmour(ARMOUR_STATS	*psStats, uint32_t index);

/*Set the stats for a particular body type*/
extern void statsSetBody(BODY_STATS	*psStats, uint32_t index);

/*Set the stats for a particular brain type*/
extern void statsSetBrain(BRAIN_STATS	*psStats, uint32_t index);

/*Set the stats for a particular propulsion type*/
extern void statsSetPropulsion(PROPULSION_STATS	*psStats, uint32_t index);

/*Set the stats for a particular sensor type*/
extern void statsSetSensor(SENSOR_STATS	*psStats, uint32_t index);

/*Set the stats for a particular ecm type*/
extern void statsSetECM(ECM_STATS	*psStats, uint32_t index);

/*Set the stats for a particular repair type*/
extern void statsSetRepair(REPAIR_STATS	*psStats, uint32_t index);

/*Set the stats for a particular construct type*/
extern void statsSetConstruct(CONSTRUCT_STATS	*psStats, uint32_t index);

/*******************************************************************************
*		Get stats functions
*******************************************************************************/
extern WEAPON_STATS *statsGetWeapon(uint32_t ref);
//extern ARMOUR_STATS *statsGetArmour(uint32_t ref);
extern BODY_STATS *statsGetBody(uint32_t ref);
extern BRAIN_STATS *statsGetBrain(uint32_t ref);
extern PROPULSION_STATS *statsGetPropulsion(uint32_t ref);
extern SENSOR_STATS *statsGetSensor(uint32_t ref);
extern ECM_STATS *statsGetECM(uint32_t ref);
extern REPAIR_STATS *statsGetRepair(uint32_t ref);
extern CONSTRUCT_STATS *statsGetConstruct(uint32_t ref);

/*******************************************************************************
*		Generic stats functions
*******************************************************************************/

/*calls the STATS_DEALLOC macro for each set of stats*/
extern BOOL statsShutDown(void);

/*Deallocate the stats passed in as parameter */
extern void statsDealloc(COMPONENT_STATS *pStats, uint32_t listSize,
						 uint32_t structureSize);

extern void deallocPropulsionTypes(void);
extern void deallocTerrainTypes(void);
extern void deallocTerrainTable(void);
extern void deallocSpecialAbility(void);

extern void storeSpeedFactor(uint32_t terrainType, uint32_t propulsionType, uint32_t speedFactor);
extern uint32_t getSpeedFactor(uint32_t terrainType, uint32_t propulsionType);
//return the type of stat this ref refers to!
extern uint32_t statType(uint32_t ref);
//return the REF_START value of this type of stat
extern uint32_t statRefStart(uint32_t stat);
/*Returns the component type based on the string - used for reading in data */
extern uint32_t componentType(const char *pType);
//get the component Inc for a stat based on the name
extern int32_t getCompFromName(uint32_t compType, const char *pName);
//get the component Inc for a stat based on the Resource name held in Names.txt
extern int32_t getCompFromResName(uint32_t compType, const char *pName);
/*returns the weapon sub class based on the string name passed in */
extern bool getWeaponSubClass(const char *subClass, WEAPON_SUBCLASS *wclass);
/*either gets the name associated with the resource (if one) or allocates space and copies pName*/
extern char *allocateName(const char *name);
/*return the name to display for the interface - valid for OBJECTS and STATS*/
extern const char *getName(const char *pNameID);
/*sets the store to the body size based on the name passed in - returns false
if doesn't compare with any*/
extern BOOL getBodySize(const char *pSize, uint8_t *pStore);

// Pass in a stat and get its name
extern const char *getStatName(const void *pStat);

/**
 * Determines the propulsion type indicated by the @c typeName string passed
 * in.
 *
 * @param typeName  name of the propulsion type to determine the enumerated
 *                  constant for.
 * @param[out] type Will contain an enumerated constant representing the given
 *                  propulsion type, if successful (as indicated by the return
 *                  value).
 *
 * @return true if successful, false otherwise. If successful, @c *type will
 *         contain a valid propulsion type enumerator, otherwise its value will
 *         be left unchanged.
 */
extern bool getPropulsionType(const char *typeName, PROPULSION_TYPE *type);

/**
 * Determines the weapon effect indicated by the @c weaponEffect string passed
 * in.
 *
 * @param weaponEffect name of the weapon effect to determine the enumerated
 *                     constant for.
 * @param[out] effect  Will contain an enumerated constant representing the
 *                     given weapon effect, if successful (as indicated by the
 *                     return value).
 *
 * @return true if successful, false otherwise. If successful, @c *effect will
 *         contain a valid weapon effect enumerator, otherwise its value will
 *         be left unchanged.
 */
extern bool getWeaponEffect(const char *weaponEffect, WEAPON_EFFECT *effect);

extern uint16_t weaponROF(WEAPON_STATS *psStat, int8_t player);
/*Access functions for the upgradeable stats of a weapon*/
extern uint32_t	weaponFirePause(const WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponReloadTime(WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponShortHit(const WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponLongHit(const WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponDamage(const WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponRadDamage(WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponIncenDamage(WEAPON_STATS *psStats, uint8_t player);
extern uint32_t	weaponRadiusHit(WEAPON_STATS *psStats, uint8_t player);
/*Access functions for the upgradeable stats of a sensor*/
extern uint32_t	sensorPower(SENSOR_STATS *psStats, uint8_t player);
extern uint32_t	sensorRange(SENSOR_STATS *psStats, uint8_t player);
/*Access functions for the upgradeable stats of a ECM*/
extern uint32_t	ecmPower(ECM_STATS *psStats, uint8_t player);
extern uint32_t	ecmRange(ECM_STATS *psStats, uint8_t player);
/*Access functions for the upgradeable stats of a repair*/
extern uint32_t	repairPoints(REPAIR_STATS *psStats, uint8_t player);
/*Access functions for the upgradeable stats of a constructor*/
extern uint32_t	constructorPoints(CONSTRUCT_STATS *psStats, uint8_t player);
/*Access functions for the upgradeable stats of a body*/
extern uint32_t	bodyPower(BODY_STATS *psStats, uint8_t player, uint8_t bodyType);
extern uint32_t	bodyArmour(BODY_STATS *psStats, uint8_t player, uint8_t bodyType,
						   WEAPON_CLASS weaponClass, int side);

/*dummy function for John*/
extern void brainAvailable(BRAIN_STATS *psStat);

extern void adjustMaxDesignStats(void);

//Access functions for the max values to be used in the Design Screen
extern uint32_t getMaxComponentWeight(void);
extern uint32_t getMaxBodyArmour(void);
extern uint32_t getMaxBodyPower(void);
extern uint32_t getMaxBodyPoints(void);
extern uint32_t getMaxSensorRange(void);
extern uint32_t getMaxSensorPower(void);
extern uint32_t getMaxECMPower(void);
extern uint32_t getMaxECMRange(void);
extern uint32_t getMaxConstPoints(void);
extern uint32_t getMaxRepairPoints(void);
extern uint32_t getMaxWeaponRange(void);
extern uint32_t getMaxWeaponDamage(void);
extern uint32_t getMaxWeaponROF(void);
extern uint32_t getMaxPropulsionSpeed(void);

extern BOOL objHasWeapon(BASE_OBJECT *psObj);

extern void statsInitVars(void);

/* Wrappers */

/** If object is an active radar (has sensor turret), then return a pointer to its sensor stats. If not, return NULL. */
SENSOR_STATS *objActiveRadar(BASE_OBJECT *psObj);

/** Returns whether object has a radar detector sensor. */
bool objRadarDetector(BASE_OBJECT *psObj);

#endif // __INCLUDED_SRC_STATS_H__
