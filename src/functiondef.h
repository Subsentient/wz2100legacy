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
/** \file
 *  Definitions for functions.
 */

#ifndef __INCLUDED_FUNCTIONDEF_H__
#define __INCLUDED_FUNCTIONDEF_H__

#include "statsdef.h"

enum FUNCTION_TYPES
{
	PRODUCTION_TYPE,
	PRODUCTION_UPGRADE_TYPE,
	RESEARCH_TYPE,
	RESEARCH_UPGRADE_TYPE,
	POWER_GEN_TYPE,
	RESOURCE_TYPE,
	REPAIR_DROID_TYPE,
	WEAPON_UPGRADE_TYPE,
	WALL_TYPE,
	STRUCTURE_UPGRADE_TYPE,
	WALLDEFENCE_UPGRADE_TYPE,
	POWER_UPGRADE_TYPE,
	REPAIR_UPGRADE_TYPE,
	DROIDREPAIR_UPGRADE_TYPE,
	DROIDECM_UPGRADE_TYPE,
	DROIDBODY_UPGRADE_TYPE,
	DROIDSENSOR_UPGRADE_TYPE,
	DROIDCONST_UPGRADE_TYPE,
	REARM_TYPE,
	REARM_UPGRADE_TYPE,
	//DEFENSIVE_STRUCTURE_TYPE,
	//RADAR_MAP_TYPE,
	//POWER_REG_TYPE,
	//POWER_RELAY_TYPE,
	//ARMOUR_UPGRADE_TYPE,
	//REPAIR_UPGRADE_TYPE,
	//RESISTANCE_UPGRADE_TYPE,
	//DROID_DESIGN_TYPE,
	//MAP_MARKER_TYPE,
	//SKY_DOME_MAP_TYPE,
	//BODY_UPGRADE_TYPE,
	//HQ_TYPE,

	/* The number of function types */
	NUMFUNCTIONS,
};

/*Common stats for all Structure Functions*/

#define FUNCTION_STATS \
	uint32_t		ref;			/* Unique ID of the item */ \
	char*		pName;			/* Text name of the component */ \
	uint8_t		type			/* The type of Function */

/*Common struct for all functions*/
typedef struct _function
{
	FUNCTION_STATS;
} FUNCTION;


/*To repair droids that enter the repair facility*/
typedef struct _repair_droid_function
{
	//common stats
	FUNCTION_STATS;

	uint32_t			repairPoints;	/*The number of repair points used to reduce
									  damage to the droid. These repair points can
									  restore even destroyed droid components*/
} REPAIR_DROID_FUNCTION;

/*To generate and supply power to other structures*/
typedef struct _power_gen_function
{
	//common stats
	FUNCTION_STATS;

	uint32_t		powerOutput;		/*How much power is generated per power cycle*/
	uint32_t		powerMultiplier;	/*Multiplies the output - upgradeable*/
	uint32_t		criticalMassChance;	/*The % chance of an explosion when the power
									  generator has taken damage*/
	uint32_t		criticalMassRadius;	/*The primary blast radius*/
	uint32_t		criticalMassDamage;	/*The base amount of damage applied to targets
									  within the primary blast area*/
	uint32_t		radiationDecayTime;	/*How long the radiation lasts n time cycles*/
} POWER_GEN_FUNCTION;

/*function used by walls to define which corner to use*/
typedef struct _wall_function
{
	//common stats
	FUNCTION_STATS;
	char						*pStructName;		//storage space for the name so can work out
	//which stat when structs are loaded in
	struct _structure_stats		*pCornerStat;		//pointer to which stat to use as a corner wall
} WALL_FUNCTION;

/*function used by Resource Extractor to indicate how much resource is available*/
typedef struct _resource_function
{
	//common stats
	FUNCTION_STATS;

	uint32_t		maxPower;			/*The max amount output from the resource*/
} RESOURCE_FUNCTION;

/*To increase a production facilities output*/
typedef struct _production_upgrade_function
{
	//common stats
	FUNCTION_STATS;

	uint8_t		outputModifier;		/*The amount added to a facility's Output*/

	uint8_t		factory;			/*flag to indicate upgrades standard factories*/
	uint8_t		cyborgFactory;		/*flag to indicate upgrades cyborg factories*/
	uint8_t		vtolFactory;		/*flag to indicate upgrades vtol factories*/

} PRODUCTION_UPGRADE_FUNCTION;

/*To manufacture droids designed previously*/
typedef struct _production_function
{
	//common stats
	FUNCTION_STATS;

	uint8_t                                   capacity;               // The max size of body the factory can produce
	uint16_t					productionOutput;	/*Droid Build Points Produced Per
												  Build Cycle*/
} PRODUCTION_FUNCTION;

/*To research topics available*/
typedef struct _research_function
{
	//common stats
	FUNCTION_STATS;

	uint32_t			researchPoints;	/*The number of research points added per
									  research cycle*/
} RESEARCH_FUNCTION;

/*To rearm VTOLs*/
typedef struct _rearm_function
{
	//common stats
	FUNCTION_STATS;

	uint32_t			reArmPoints;	/*The number of reArm points added per cycle*/
} REARM_FUNCTION;

/*Generic upgrade function*/
#define UPGRADE_FUNCTION_STATS \
	FUNCTION_STATS;						/*common stats*/ \
    uint16_t			upgradePoints	/*The % to add to the action points*/
//uint8_t			upgradePoints	/*The % to add to the action points*/

typedef struct _upgrade_function
{
	UPGRADE_FUNCTION_STATS;
} UPGRADE_FUNCTION;

typedef UPGRADE_FUNCTION	RESEARCH_UPGRADE_FUNCTION;
typedef UPGRADE_FUNCTION	REPAIR_UPGRADE_FUNCTION;
typedef UPGRADE_FUNCTION	POWER_UPGRADE_FUNCTION;
typedef UPGRADE_FUNCTION	REARM_UPGRADE_FUNCTION;

/*Upgrade the weapon ROF and accuracy for the weapons of a particular class*/
typedef struct _weapon_upgrade_function
{
	//common stats
	FUNCTION_STATS;

// GNU C complains about this...
//	WEAPON_CLASS            subClass;		/*which weapons are affected */
// So need to do it this way...
	WEAPON_SUBCLASS		subClass;			/*which weapons are affected */
	uint8_t			firePause;			/*The % to decrease the fire pause or reload time */
	uint16_t			shortHit;			/*The % to increase the  short range accuracy */
	uint16_t			longHit;			/*The % to increase the long range accuracy */
	uint16_t			damage;				/*The % to increase the damage*/
	uint16_t			radiusDamage;		/*The % to increase the radius damage*/
	uint16_t			incenDamage;		/*The % to increase the incendiary damage*/
	uint16_t			radiusHit;			/*The % to increase the chance to hit in blast radius*/

} WEAPON_UPGRADE_FUNCTION;

/*Upgrade the structure stats for all non wall and defence structures*/
typedef struct _structure_upgrade_function
{
	//common stats
	FUNCTION_STATS;

	uint16_t			armour;			/*The % to increase the armour value*/
	uint16_t			body;			/*The % to increase the body points*/
	uint16_t			resistance;		/*The % to increase the resistance*/

} STRUCTURE_UPGRADE_FUNCTION;

/*Upgrade the structure stats for all wall and defence structures*/
typedef struct _wallDefence_upgrade_function
{
	//common stats
	FUNCTION_STATS;

	uint16_t			armour;			/*The % to increase the armour value*/
	uint16_t			body;			/*The % to increase the body points*/

} WALLDEFENCE_UPGRADE_FUNCTION;

typedef UPGRADE_FUNCTION	DROIDREPAIR_UPGRADE_FUNCTION;
typedef UPGRADE_FUNCTION	DROIDECM_UPGRADE_FUNCTION;
typedef UPGRADE_FUNCTION	DROIDCONSTR_UPGRADE_FUNCTION;

typedef struct _droidBody_upgrade_function
{
	UPGRADE_FUNCTION_STATS;
	uint16_t					body;		//The % to increase the whole vehicle body points by*/
	uint16_t					armourValue[WC_NUM_WEAPON_CLASSES];
	uint8_t					cyborg;		//flag to specify the upgrade is valid for cyborgs
	uint8_t					droid;		/*flag to specify the upgrade is valid
										  for droids (non cyborgs!)*/
} DROIDBODY_UPGRADE_FUNCTION;

typedef struct _droidsensor_upgrade_function
{
	UPGRADE_FUNCTION_STATS;
	uint16_t					range;		// % to increase range by
} DROIDSENSOR_UPGRADE_FUNCTION;


#if(0)
typedef struct _function_upgrade
{
	uint32_t		functionInc;			/*The index of the function in asFunctions */
	BOOL		available;				/*Flag to indicate whether this Upgrade is available*/
} FUNCTION_UPGRADE;
#endif
/*function used by HQ to input power values*/
//typedef struct _hq_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	uint32_t			power;				/*The power value of the HQ*/
//} HQ_FUNCTION;


/*upgrade the armour that can be applied to a droid/structure*/
//typedef struct _armour_upgrade_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	ARMOUR_STATS*	pArmour;		/*The armour to upgrade to*/
//	uint32_t			buildPoints;	/*The number of build points required to upgrade
//									  the structure*/
//	uint32_t			powerRequired;	/*The amount of power required to upgrade*/
//	uint32_t			armourPoints;	/*The percentage to increase the armour points by*/
//} ARMOUR_UPGRADE_FUNCTION;

/*To regulate power flows*/
//typedef struct _power_reg_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	uint32_t		maxPower;			/*The maximum amount of power output the
//									  regulator can handle*/
//} POWER_REG_FUNCTION;

/*To transfer power across the map*/
//typedef struct _power_relay_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	uint32_t		powerRelayType;		/*Broadcast=0 cable=1*/
//	uint32_t		powerRelayRange;	/*The range in map distances that the power
//									  can be relayed*/
//} POWER_RELAY_FUNCTION;

/*To display the radar map*/
//typedef struct _radar_map_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	uint32_t		radarDecayRate;		/*How fast the droids out of LOS decay on the
//									  radar*/
//	uint32_t		radarRadius;		/*How far a radar building can see with 100%
//									  accuracy*/
//} RADAR_MAP_FUNCTION;

/*Upgrade the repair points that can be obtained from a repair facitity*/
//typedef struct _repair_upgrade_function
//{
//common stats
//	FUNCTION_STATS;

//	struct REPAIR_STATS*	pRepair;		/*The repair unit to be upgraded*/
//	uint32_t			repairPoints;	/*The percentage to increase the repair points by*/
//	uint32_t			buildPoints;	/*The number of build points required to upgrade
//									  the structure*/
//	uint32_t			powerRequired;	/*The amount of power required to upgrade*/
//} REPAIR_UPGRADE_FUNCTION;

/*Upgrade the body points of the structure*/
//typedef struct _body_upgrade_function
//{
//	//common stats
//	FUNCTION_STATS;
//
///	uint32_t			bodyPoints;		/*The percentage to increase the body points by*/
//} BODY_UPGRADE_FUNCTION;

/*Upgrade the resistance*/
//typedef struct _resistance_upgrade_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	uint32_t			resistanceUpgrade;	/*This is unknown at the moment!!27/02/97*/
//	uint32_t			buildPoints;		/*The number of build points required to
//										  upgrade the structure*/
//	uint32_t			powerRequired;		/*The amount of power required to upgrade*/
//	uint32_t			resistancePoints;	/*The percentage to increase the resistance points by*/
//} RESISTANCE_UPGRADE_FUNCTION;
/*blocks LOS and restricts movement*/
//typedef struct _defensive_structure_function
//{
//	//common stats
//	FUNCTION_STATS;
//
//	struct SENSOR_STATS*	pSensor;		/*The Sensor fitted, if any*/
//	struct ECM_STATS*		pECM;			/*The ECM fitted, if any*/
//	uint32_t			weaponCapacity;	/*The size of weapon in system points that may
//									  be added. 0 = no weapons can be added*/
//} DEFENSIVE_STRUCTURE_FUNCTION;

#endif // __INCLUDED_FUNCTIONDEF_H__
