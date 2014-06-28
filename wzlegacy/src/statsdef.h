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
*  Definitions for the stats system.
*/
#ifndef __INCLUDED_STATSDEF_H__
#define __INCLUDED_STATSDEF_H__

#include "lib/ivis_common/ivisdef.h"

/*
if any types are added BEFORE 'COMP_BODY' - then Save/Load Game will have to be
altered since it loops through the components from 1->MAX_COMP
*/
typedef enum COMPONENT_TYPE
{
	COMP_UNKNOWN,
	COMP_BODY,
	COMP_BRAIN,
	COMP_PROPULSION,
	COMP_REPAIRUNIT,
	COMP_ECM,
	COMP_SENSOR,
	COMP_CONSTRUCT,
	COMP_WEAPON,
	COMP_NUMCOMPONENTS,			/** The number of enumerators in this enum.	 */
} COMPONENT_TYPE;

/**
 * LOC used for holding locations for Sensors and ECM's
 */
typedef enum LOC
{
	LOC_DEFAULT,
	LOC_TURRET,
} LOC;

/**
 * SIZE used for specifying body size
 */
typedef enum BODY_SIZE
{
	SIZE_LIGHT,
	SIZE_MEDIUM,
	SIZE_HEAVY,
	SIZE_SUPER_HEAVY,
} BODY_SIZE;

/**
 * only using KINETIC and HEAT for now
 */
typedef enum WEAPON_CLASS
{
	WC_KINETIC,					///< bullets etc
	//WC_EXPLOSIVE,				///< rockets etc - classed as WC_KINETIC now to save space in DROID
	WC_HEAT,					///< laser etc
	//WC_MISC					///< others we haven't thought of! - classed as WC_HEAT now to save space in DROID
	WC_NUM_WEAPON_CLASSES		/** The number of enumerators in this enum.	 */
} WEAPON_CLASS;

/**
 * weapon subclasses used to define which weapons are affected by weapon upgrade
 * functions
 *
 * Watermelon:added a new subclass to do some tests
 */
typedef enum WEAPON_SUBCLASS
{
	WSC_MGUN,
	WSC_CANNON,
	//WSC_ARTILLARY,
	WSC_MORTARS,
	WSC_MISSILE,
	WSC_ROCKET,
	WSC_ENERGY,
	WSC_GAUSS,
	WSC_FLAME,
	//WSC_CLOSECOMBAT,
	WSC_HOWITZERS,
	WSC_ELECTRONIC,
	WSC_AAGUN,
	WSC_SLOWMISSILE,
	WSC_SLOWROCKET,
	WSC_LAS_SAT,
	WSC_BOMB,
	WSC_COMMAND,
	WSC_EMP,
	WSC_COUNTER,				// Counter missile
	WSC_NUM_WEAPON_SUBCLASSES,	/** The number of enumerators in this enum.	 */
} WEAPON_SUBCLASS;

/**
 * Used to define which projectile model to use for the weapon.
 */
typedef enum MOVEMENT_MODEL
{
	MM_DIRECT,
	MM_INDIRECT,
	MM_HOMINGDIRECT,
	MM_HOMINGINDIRECT,
	MM_ERRATICDIRECT,
	MM_SWEEP,
	NUM_MOVEMENT_MODEL,			/**  The number of enumerators in this enum. */
} MOVEMENT_MODEL;

/**
 * Used to modify the damage to a propuslion type (or structure) based on
 * weapon.
 */
typedef enum WEAPON_EFFECT
{
	WE_ANTI_PERSONNEL,
	WE_ANTI_TANK,
	WE_BUNKER_BUSTER,
	WE_ARTILLERY_ROUND,
	WE_FLAMER,
	WE_ANTI_AIRCRAFT,
	WE_NUMEFFECTS,			/**  The number of enumerators in this enum. */
} WEAPON_EFFECT;

/**
 * Sides used for droid impact
 */
typedef enum HIT_SIDE
{
	HIT_SIDE_FRONT,
	HIT_SIDE_REAR,
	HIT_SIDE_LEFT,
	HIT_SIDE_RIGHT,
	HIT_SIDE_TOP,
	HIT_SIDE_BOTTOM,
	HIT_SIDE_PIERCE, // ignore armor
	NUM_HIT_SIDES,			/**  The number of enumerators in this enum. */
} HIT_SIDE;

/**
 * Defines the left and right sides for propulsion IMDs
 */
typedef enum PROP_SIDE
{
	LEFT_PROP,
	RIGHT_PROP,
	NUM_PROP_SIDES,			/**  The number of enumerators in this enum. */
} PROP_SIDE;

typedef enum PROPULSION_TYPE
{
	PROPULSION_TYPE_WHEELED,
	PROPULSION_TYPE_TRACKED,
	PROPULSION_TYPE_LEGGED,
	PROPULSION_TYPE_HOVER,
	PROPULSION_TYPE_SKI,
	PROPULSION_TYPE_LIFT,
	PROPULSION_TYPE_PROPELLOR,
	PROPULSION_TYPE_HALF_TRACKED,
	PROPULSION_TYPE_JUMP,
	PROPULSION_TYPE_NUM,	/**  The number of enumerators in this enum. */
} PROPULSION_TYPE;

typedef enum SENSOR_TYPE
{
	STANDARD_SENSOR,
	INDIRECT_CB_SENSOR,
	VTOL_CB_SENSOR,
	VTOL_INTERCEPT_SENSOR,
	SUPER_SENSOR,			///< works as all of the above together! - new for updates
	RADAR_DETECTOR_SENSOR,
} SENSOR_TYPE;

typedef enum FIREONMOVE
{
	FOM_NO,			///< no capability - droid must stop
	FOM_PARTIAL,	///< partial capability - droid has 50% chance to hit
	FOM_YES,		///< full capability - droid fires normally on move
} FIREONMOVE;

typedef enum TRAVEL_MEDIUM
{
	GROUND,
	AIR,
} TRAVEL_MEDIUM;

/*
* Stats structures type definitions
*/

/* Elements common to all stats structures */

/* Stats common to all stats structs */
typedef struct BASE_STATS
{
	uint32_t	ref;	/**< Unique ID of the item */
	char	*pName; /**< pointer to the text id name (i.e. short language-independant name) */
} WZ_DECL_MAY_ALIAS BASE_STATS;

#define STATS_BASE \
	uint32_t ref; /**< Unique ID of the item */ \
	char *pName /**< pointer to the text id name (i.e. short language-independant name) */

/* Stats common to all droid components */
typedef struct COMPONENT_STATS
{
	uint32_t		ref;				/**< Unique ID of the item */
	char		*pName;				/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t		buildPower;			/**< Power required to build the component */
	uint32_t		buildPoints;		/**< Time required to build the component */
	uint32_t		weight;				/**< Component's weight */
	uint32_t		body;				/**< Component's body points */
	bool		designable;			/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape	*pIMD;				/**< The IMD to draw for this component */
} WZ_DECL_MAY_ALIAS COMPONENT_STATS;

#define STATS_COMPONENT \
 \
	uint32_t		ref;				/**< Unique ID of the item */ \
	char		*pName;				/**< pointer to the text id name (i.e. short language-independant name) */ \
	uint32_t		buildPower;			/**< Power required to build the component */ \
	uint32_t		buildPoints;		/**< Time required to build the component */ \
	uint32_t		weight;				/**< Component's weight */ \
	uint32_t		body;				/**< Component's body points */ \
	bool		designable;			/**< flag to indicate whether this component can be used in the design screen */ \
	iIMDShape	*pIMD				/**< The IMD to draw for this component */

typedef struct PROPULSION_STATS
{

	///< Common stats
	uint32_t			ref;			/**< Unique ID of the item */
	char			*pName;			/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t			buildPower;		/**< Power required to build the component */
	uint32_t			buildPoints;	/**< Time required to build the component */
	uint32_t			weight;			/**< Component's weight */
	uint32_t			body;			/**< Component's body points */
	bool			designable;		/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape		*pIMD;			/**< The IMD to draw for this component */

	uint32_t			maxSpeed;		///< Max speed for the droid
	PROPULSION_TYPE propulsionType; ///< Type of propulsion used - index into PropulsionTable
} WZ_DECL_MAY_ALIAS PROPULSION_STATS;

typedef struct SENSOR_STATS
{

	///< Common stats
	uint32_t		ref;			/**< Unique ID of the item */
	char		*pName;			/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t		buildPower;		/**< Power required to build the component */
	uint32_t		buildPoints;	/**< Time required to build the component */
	uint32_t		weight;			/**< Component's weight */
	uint32_t		body;			/**< Component's body points */
	bool		designable;		/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape	*pIMD;			/**< The IMD to draw for this component */

	uint32_t		range;			///< Sensor range
	uint32_t		power;			///< Sensor power (put against ecm power)
	uint32_t		location;		///< specifies whether the Sensor is default or for the Turret
	SENSOR_TYPE type;			///< used for combat
	uint32_t		time;			///< time delay before associated weapon droids 'know' where the attack is from
	iIMDShape	*pMountGraphic; ///< The turret mount to use
} WZ_DECL_MAY_ALIAS SENSOR_STATS;

typedef struct ECM_STATS
{

	///< Common stats
	uint32_t		ref;			/**< Unique ID of the item */
	char		*pName;			/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t		buildPower;		/**< Power required to build the component */
	uint32_t		buildPoints;	/**< Time required to build the component */
	uint32_t		weight;			/**< Component's weight */
	uint32_t		body;			/**< Component's body points */
	bool		designable;		/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape	*pIMD;			/**< The IMD to draw for this component */

	uint32_t		range;			///< ECM range
	uint32_t		power;			///< ECM power (put against sensor power)
	uint32_t		location;		///< specifies whether the ECM is default or for the Turret
	iIMDShape	*pMountGraphic; ///< The turret mount to use
} WZ_DECL_MAY_ALIAS ECM_STATS;

typedef struct REPAIR_STATS
{

	///< Common stats
	uint32_t		ref;			/**< Unique ID of the item */
	char		*pName;			/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t		buildPower;		/**< Power required to build the component */
	uint32_t		buildPoints;	/**< Time required to build the component */
	uint32_t		weight;			/**< Component's weight */
	uint32_t		body;			/**< Component's body points */
	bool		designable;		/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape	*pIMD;			/**< The IMD to draw for this component */

	uint32_t		repairPoints;	///< How much damage is restored to Body Points and armour each Repair Cycle
	bool		repairArmour;	///< whether armour can be repaired or not
	uint32_t		location;		///< specifies whether the Repair is default or for the Turret
	uint32_t		time;			///< time delay for repair cycle
	iIMDShape	*pMountGraphic; ///< The turret mount to use
} WZ_DECL_MAY_ALIAS REPAIR_STATS;

typedef struct WEAPON_STATS
{

	///< Common stats
	uint32_t			ref;		/**< Unique ID of the item */
	char			*pName;		/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t			buildPower; /**< Power required to build the component */
	uint32_t			buildPoints;			/**< Time required to build the component */
	uint32_t			weight;					/**< Component's weight */
	uint32_t			body;					/**< Component's body points */
	bool			designable;				/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape		*pIMD;					/**< The IMD to draw for this component */

	uint32_t			shortRange;				///< Max distance to target for	short	range	shot
	uint32_t			longRange;				///< Max distance to target for	long range shot
	uint32_t			minRange;				///< Min distance to target for	shot
	uint32_t			shortHit;				///< Chance to hit at short range
	uint32_t			longHit;				///< Chance to hit at long range
	uint32_t			firePause;				///< Time between each weapon fire
	uint32_t			numExplosions;			///< The number of explosions per shot
	uint8_t			numRounds;				///< The number of rounds	per salvo(magazine)
	uint32_t			reloadTime;				///< Time to reload	the round of ammo	(salvo fire)
	uint32_t			damage;					///< How much	damage the weapon	causes
	uint32_t			radius;					///< Basic blast radius of weapon
	uint32_t			radiusHit;				///< Chance to hit in the	blast	radius
	uint32_t			radiusDamage;			///< Damage done in	the blast radius
	uint32_t			incenTime;				///< How long	the round burns
	uint32_t			incenDamage;			///< Damage done each burn cycle
	uint32_t			incenRadius;			///< Burn radius of	the round
	uint32_t			flightSpeed;			///< speed ammo travels at
	uint32_t			indirectHeight;			///< how high	the ammo travels for indirect	fire
	FIREONMOVE		fireOnMove;				///< indicates whether the droid has to stop before firing
	WEAPON_CLASS	weaponClass;			///< the class of weapon
	WEAPON_SUBCLASS weaponSubClass;			///< the subclass to which the weapon	belongs
	MOVEMENT_MODEL	movementModel;			///< which projectile model to use for the bullet
	WEAPON_EFFECT	weaponEffect;			///< which type of warhead is associated with the	weapon
	uint32_t			recoilValue;			///< used to compare with	weight to see if recoils or not
	uint8_t			rotate;					///< amount the weapon(turret) can rotate 0	= none
	uint8_t			maxElevation;			///< max amount the	turret can be elevated up
	int8_t			minElevation;			///< min amount the	turret can be elevated down
	uint8_t			facePlayer;				///< flag to make the (explosion) effect face the	player when	drawn
	uint8_t			faceInFlight;			///< flag to make the inflight effect	face the player when drawn
	uint8_t			effectSize;				///< size of the effect 100 = normal,	50 = half etc
	bool			lightWorld;				///< flag to indicate whether the effect lights up the world
	uint8_t			surfaceToAir;			///< indicates how good in the air - SHOOT_ON_GROUND, SHOOT_IN_AIR or both
	uint8_t			vtolAttackRuns;			///< number of attack runs a VTOL droid can	do with this weapon
	bool			penetrate;				///< flag to indicate whether pentrate droid or not

	/* Graphics control stats */
	uint32_t			directLife;				///< How long a direct fire weapon is visible. Measured in 1/100 sec.
	uint32_t			radiusLife;				///< How long a blast radius is visible

	/* Graphics used for the weapon */
	iIMDShape		*pMountGraphic;			///< The turret mount to use
	iIMDShape		*pMuzzleGraphic;		///< The muzzle flash
	iIMDShape		*pInFlightGraphic;		///< The ammo in flight
	iIMDShape		*pTargetHitGraphic;		///< The ammo hitting a target
	iIMDShape		*pTargetMissGraphic;	///< The ammo missing a target
	iIMDShape		*pWaterHitGraphic;		///< The ammo hitting water
	iIMDShape		*pTrailGraphic;			///< The trail used for in flight

	/* Audio */
	int32_t			iAudioFireID;
	int32_t			iAudioImpactID;
} WZ_DECL_MAY_ALIAS WEAPON_STATS;

typedef struct CONSTRUCT_STATS
{

	///< Common stats
	uint32_t		ref;			/**< Unique ID of the item */
	char		*pName;			/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t		buildPower;		/**< Power required to build the component */
	uint32_t		buildPoints;	/**< Time required to build the component */
	uint32_t		weight;			/**< Component's weight */
	uint32_t		body;			/**< Component's body points */
	bool		designable;		/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape	*pIMD;			/**< The IMD to draw for this component */

	uint32_t		constructPoints;	///< The number of points contributed each cycle
	iIMDShape	*pMountGraphic;		///< The turret mount to use
} WZ_DECL_MAY_ALIAS CONSTRUCT_STATS;

typedef struct BRAIN_STATS
{
	///< Common stats
	uint32_t			ref;			/**< Unique ID of the item */
	char			*pName;			/**< pointer to the text id name (i.e. short language-independant name) */
	uint32_t			buildPower;		/**< Power required to build the component */
	uint32_t			buildPoints;	/**< Time required to build the component */
	uint32_t			weight;			/**< Component's weight */
	uint32_t			body;			/**< Component's body points */
	bool			designable;		/**< flag to indicate whether this component can be used in the design screen */
	iIMDShape		*pIMD;			/**< The IMD to draw for this component */

	uint32_t			progCap;		///< Program capacity
	WEAPON_STATS	*psWeaponStat;	///< weapon stats associated with this brain - for Command Droids
} WZ_DECL_MAY_ALIAS BRAIN_STATS;


#define NULL_COMP	(-1)
/*
 * Stats structures type definitions
 */
#define SHOOT_ON_GROUND 0x01
#define SHOOT_IN_AIR	0x02

//Special angles representing top or bottom hit
#define HIT_ANGLE_TOP		361
#define HIT_ANGLE_BOTTOM	362

typedef struct _body_stats
{
	STATS_COMPONENT;			///< Common stats

	uint8_t		size;			///< How big the body is - affects how hit
	uint32_t		weaponSlots;	///< The number of weapon slots on the body
	uint32_t		armourValue[NUM_HIT_SIDES][WC_NUM_WEAPON_CLASSES];	///< A measure of how much protection the armour provides. Cross referenced with the weapon types.

	// A measure of how much energy the power plant outputs
	uint32_t		powerOutput;	///< this is the engine output of the body
	iIMDShape	**ppIMDList;	///< list of IMDs to use for propulsion unit - up to numPropulsionStats
	iIMDShape	*pFlameIMD;		///< pointer to which flame graphic to use - for VTOLs only at the moment
} BODY_STATS;

/************************************************************************************
* Additional stats tables
************************************************************************************/
typedef struct _propulsion_types
{
	uint16_t	powerRatioMult; ///< Multiplier for the calculated power ratio of the droid
	uint32_t	travel;			///< Which medium the propulsion travels in
	int16_t	startID;		///< sound to play when this prop type starts
	int16_t	idleID;			///< sound to play when this prop type is idle
	int16_t	moveOffID;		///< sound to link moveID and idleID
	int16_t	moveID;			///< sound to play when this prop type is moving
	int16_t	hissID;			///< sound to link moveID and idleID
	int16_t	shutDownID;		///< sound to play when this prop type shuts down
} PROPULSION_TYPES;

typedef struct _terrain_table
{
	uint32_t	speedFactor;	///< factor to multiply the speed by depending on the method of propulsion and the terrain type - to be divided by 100 before use
} TERRAIN_TABLE;

typedef struct _special_ability
{
	char	*pName;			///< Text name of the component
} SPECIAL_ABILITY;

typedef uint16_t	WEAPON_MODIFIER;

/* weapon stats which can be upgraded by research*/
typedef struct _weapon_upgrade
{
	uint8_t	firePause;
	uint16_t	shortHit;
	uint16_t	longHit;
	uint16_t	damage;
	uint16_t	radiusDamage;
	uint16_t	incenDamage;
	uint16_t	radiusHit;
} WEAPON_UPGRADE;

/*sensor stats which can be upgraded by research*/
typedef struct _sensor_upgrade
{
	uint16_t	power;
	uint16_t	range;
} SENSOR_UPGRADE;

/*ECM stats which can be upgraded by research*/
typedef struct _ecm_upgrade
{
	uint16_t	power;
	uint32_t	range;
} ECM_UPGRADE;

/*repair stats which can be upgraded by research*/
typedef struct _repair_upgrade
{
	uint16_t	repairPoints;
} REPAIR_UPGRADE;

/*constructor stats which can be upgraded by research*/
typedef struct _constructor_upgrade
{
	uint16_t	constructPoints;
} CONSTRUCTOR_UPGRADE;

/*body stats which can be upgraded by research*/
typedef struct _body_upgrade
{
	uint16_t	powerOutput;
	uint16_t	body;
	uint16_t	armourValue[WC_NUM_WEAPON_CLASSES];
} BODY_UPGRADE;
#endif // __INCLUDED_STATSDEF_H__
