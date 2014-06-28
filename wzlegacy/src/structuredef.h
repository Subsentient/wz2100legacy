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
 *  Definitions for structures.
 */

#ifndef __INCLUDED_STRUCTUREDEF_H__
#define __INCLUDED_STRUCTUREDEF_H__

#include "lib/gamelib/animobj.h"
#include "positiondef.h"
#include "basedef.h"
#include "statsdef.h"
#include "weapondef.h"

#define NUM_FACTORY_MODULES	2
#define NUM_RESEARCH_MODULES 4
#define NUM_POWER_MODULES 4

#define	REF_ANY	255	// Used to indicate any kind of building when calling intGotoNextStructureType()

/* Defines for indexing an appropriate IMD object given a buildings purpose. */
typedef enum _structure_type
{
	REF_HQ,
	REF_FACTORY,
	REF_FACTORY_MODULE,//draw as factory 2
	REF_POWER_GEN,
	REF_POWER_MODULE,
	REF_RESOURCE_EXTRACTOR,
	REF_DEFENSE,
	REF_WALL,
	REF_WALLCORNER,				//corner wall - no gun
	REF_BLASTDOOR,
	REF_RESEARCH,
	REF_RESEARCH_MODULE,
	REF_REPAIR_FACILITY,
	REF_COMMAND_CONTROL,		//control centre for command droids
	REF_BRIDGE,			//NOT USED, but removing it would change savegames
	REF_DEMOLISH,			//the demolish structure type - should only be one stat with this type
	REF_CYBORG_FACTORY,
	REF_VTOL_FACTORY,
	REF_LAB,
	REF_REARM_PAD,
	REF_MISSILE_SILO,
	REF_SAT_UPLINK,         //added for updates - AB 8/6/99
	NUM_DIFF_BUILDINGS,		//need to keep a count of how many types for IMD loading
} STRUCTURE_TYPE;

typedef struct _flag_position
{
	POSITION_OBJ;
	Vector3i		coords;							//the world coords of the Position
	uint8_t		factoryInc;						//indicates whether the first, second etc factory
	uint8_t		factoryType;					//indicates whether standard, cyborg or vtol factory
//	uint8_t		factorySub;						//sub value. needed to order production points.
//	uint8_t		primary;
	struct _flag_position	*psNext;
} FLAG_POSITION;


#ifdef DEMO
#define NUM_DEMO_STRUCTS	12
#endif

//only allowed one weapon per structure (more memory for Tim)
//Watermelon:only allowed 4 weapons per structure(sorry Tim...)
#define STRUCT_MAXWEAPS		4

typedef enum _struct_strength
{
	STRENGTH_SOFT,
	STRENGTH_MEDIUM,
	STRENGTH_HARD,
	STRENGTH_BUNKER,

	NUM_STRUCT_STRENGTH,
} STRUCT_STRENGTH;

#define INVALID_STRENGTH	(NUM_STRUCT_STRENGTH + 1)

typedef uint16_t STRUCTSTRENGTH_MODIFIER;

//this structure is used to hold the permenant stats for each type of building
typedef struct _structure_stats
{
	STATS_BASE;						/* basic stats */
	STRUCTURE_TYPE	type;				/* the type of structure */
	STRUCT_STRENGTH	strength;		/* strength against the weapon effects */
	uint32_t		terrainType;		/*The type of terrain the structure has to be
									  built next to - may be none*/
	uint32_t		baseWidth;			/*The width of the base in tiles*/
	uint32_t		baseBreadth;		/*The breadth of the base in tiles*/
	uint32_t		foundationType;		/*The type of foundation for the structure*/
	uint32_t		buildPoints;		/*The number of build points required to build
									  the structure*/
	uint32_t		height;				/*The height above/below the terrain - negative
									  values denote below the terrain*/
	uint32_t		armourValue;		/*The armour value for the structure - can be
									  upgraded */
	uint32_t		bodyPoints;			/*The structure's body points - A structure goes
									  off-line when 50% of its body points are lost*/
	uint32_t		repairSystem;		/*The repair system points are added to the body
									  points until fully restored . The points are
									  then added to the Armour Points*/
	uint32_t		powerToBuild;		/*How much power the structure requires to build*/
	uint32_t		resistance;			/*The number used to determine whether a
									  structure can resist an enemy takeover -
									  0 = cannot be attacked electrically*/
	uint32_t		sizeModifier;		/*The larger the target, the easier to hit*/
	iIMDShape	*pIMD;		/*The IMD to draw for this structure */
	iIMDShape	*pBaseIMD;	/*The base IMD to draw for this structure */
	struct ECM_STATS	*pECM;		/*Which ECM is standard for the structure -
									  if any*/
	struct SENSOR_STATS *pSensor;	/*Which Sensor is standard for the structure -
									  if any*/
	uint32_t		weaponSlots;		/*Number of weapons that can be attached to the
									  building*/
	uint32_t		numWeaps;			/*Number of weapons for default */

	struct WEAPON_STATS    *psWeapStat[STRUCT_MAXWEAPS];

	uint32_t		numFuncs;			/*Number of functions for default*/
	int32_t		defaultFunc;		/*The default function*/
	struct _function	**asFuncList;		/*List of pointers to allowable functions -
									  unalterable*/
} WZ_DECL_MAY_ALIAS STRUCTURE_STATS;

typedef enum _struct_states
{
	SS_BEING_BUILT,
	SS_BUILT,
	SS_BEING_DEMOLISHED,
	SS_BLUEPRINT_VALID,
	SS_BLUEPRINT_INVALID,
	SS_BLUEPRINT_PLANNED,
	SS_BLUEPRINT_ALLY
} STRUCT_STATES;

typedef struct _research_facility
{
	struct BASE_STATS	*psSubject;		/* the subject the structure is working on*/
	uint32_t		capacity;				/* Number of upgrade modules added*/
	uint32_t		timeStarted;			/* The time the building started on the subject*/
	uint32_t		researchPoints;			/* Research Points produced per research cycle*/
	uint32_t		timeToResearch;			/* Time taken to research the topic*/
	struct BASE_STATS	*psBestTopic;	/* The topic with the most research points
										   that was last performed*/
	uint32_t		powerAccrued;			/* used to keep track of power before
										   researching a topic*/
	uint32_t		timeStartHold;		    /* The time the research facility was put on hold*/

} RESEARCH_FACILITY;

typedef struct _factory
{

	uint8_t				capacity;			/* The max size of body the factory
											   can produce*/
	uint8_t				quantity;			/* The number of droids to produce OR for
											   selectedPlayer, how many loops to perform*/
	uint8_t				loopsPerformed;		/* how many times the loop has been performed*/
	//struct _propulsion_types*	propulsionType;
	//uint8_t				propulsionType;		/* The type of propulsion the facility
	//										   can produce*/
	uint8_t				productionOutput;	/* Droid Build Points Produced Per
											   Build Cycle*/
	uint32_t				powerAccrued;		/* used to keep track of power before building a droid*/
	BASE_STATS			*psSubject;			/* the subject the structure is working on */
	uint32_t				timeStarted;		/* The time the building started on the subject*/
	uint32_t				timeToBuild;		/* Time taken to build one droid */
	uint32_t				timeStartHold;		/* The time the factory was put on hold*/
	FLAG_POSITION		*psAssemblyPoint;	/* Place for the new droids to assemble at */
	struct DROID		*psCommander;	    // command droid to produce droids for (if any)
	uint32_t              secondaryOrder;     // secondary order state for all units coming out of the factory
	// added AB 22/04/99

	//these are no longer required - yipee!
	// The group the droids produced by this factory belong to - used for Missions
	//struct _droid_group		*psGroup;
	//struct DROID			*psGrpNext;

} FACTORY;

typedef struct _res_extractor
{
	uint32_t				power;				/*The max amount of power that can be extracted*/
	uint32_t				timeLastUpdated;	/*time the Res Extr last got points*/
	BOOL				active;				/*indicates when the extractor is on ie digging up oil*/
	struct _structure	*psPowerGen;		/*owning power generator*/
} RES_EXTRACTOR;

typedef struct _power_gen
{
	uint32_t				power;				/*The max power that can be used - NOT USED 21/04/98*/
	uint32_t				multiplier;			/*Factor to multiply output by - percentage*/
	uint32_t				capacity;			/* Number of upgrade modules added*/

	//struct _structure	*apResExtractors[NUM_POWER_MODULES + 1];/*pointers to the res ext
	struct _structure	*apResExtractors[NUM_POWER_MODULES];/*pointers to the res ext
																associated with this gen*/
} POWER_GEN;

typedef struct REPAIR_FACILITY
{
	uint32_t				power;				/* Power used in repairing */
	uint32_t				timeStarted;		/* Time repair started on current object */
	BASE_OBJECT			*psObj;				/* Object being repaired */
	uint32_t				powerAccrued;		/* used to keep track of power before
											   repairing a droid */
	FLAG_POSITION		*psDeliveryPoint;	/* Place for the repaired droids to assemble
                                               at */
	uint32_t              currentPtsAdded;    /* stores the amount of body points added to the unit
                                               that is being worked on */

	// The group the droids to be repaired by this facility belong to
	struct _droid_group		*psGroup;
	struct DROID			*psGrpNext;
	int				droidQueue;		///< Last count of droid queue for this facility
} REPAIR_FACILITY;

typedef struct _rearm_pad
{
	uint32_t				reArmPoints;		/* rearm points per cycle				 */
	uint32_t				timeStarted;		/* Time reArm started on current object	 */
	BASE_OBJECT			*psObj;				/* Object being rearmed		             */
	uint32_t              timeLastUpdated;    /* Time rearm was last updated */
} REARM_PAD;

typedef union
{
	RESEARCH_FACILITY researchFacility;
	FACTORY           factory;
	RES_EXTRACTOR     resourceExtractor;
	POWER_GEN         powerGenerator;
	REPAIR_FACILITY   repairFacility;
	REARM_PAD         rearmPad;
} FUNCTIONALITY;

//this structure is used whenever an instance of a building is required in game
typedef struct _structure
{
	/* The common structure elements for all objects */
	BASE_ELEMENTS(struct _structure);

	STRUCTURE_STATS	*pStructureType;		/* pointer to the structure stats for this
											   type of building */
	uint8_t		status;						/* defines whether the structure is being
											   built, doing nothing or performing a function*/
	int16_t		currentBuildPts;			/* the build points currently assigned to this
											   structure */
	int16_t       currentPowerAccrued;        /* the power accrued for building this structure*/
	int16_t		resistance;					/* current resistance points
											   0 = cannot be attacked electrically*/
	uint32_t		lastResistance;				/* time the resistance was last increased*/

	/* The other structure data.  These are all derived from the functions
	 * but stored here for easy access - will need to add more for variable stuff!
	 */

	FUNCTIONALITY	*pFunctionality;		/* pointer to structure that contains fields
											   necessary for functionality */
	/* The weapons on the structure */
	uint16_t		numWeaps;
	uint8_t		targetted;
	WEAPON		asWeaps[STRUCT_MAXWEAPS];
	BASE_OBJECT	*psTarget[STRUCT_MAXWEAPS];

#ifdef DEBUG
	// these are to help tracking down dangling pointers
	char				targetFunc[STRUCT_MAXWEAPS][MAX_EVENT_NAME_LEN];
	int				targetLine[STRUCT_MAXWEAPS];
#endif

	/* anim data */
	ANIM_OBJECT	*psCurAnim;

} WZ_DECL_MAY_ALIAS STRUCTURE;

#define LOTS_OF	255						/*highest number the limit can be set to */
typedef struct _structure_limits
{
	uint8_t		limit;				/* the number allowed to be built */
	uint8_t		currentQuantity;	/* the number of the type currently
												   built per player*/

	uint8_t		globalLimit;		// multiplayer only. sets the max value selectable (limits changed by player)

} STRUCTURE_LIMITS;


//the three different types of factory (currently) - FACTORY, CYBORG_FACTORY, VTOL_FACTORY
// added repair facilities as they need an assebly point as well
#define NUM_FACTORY_TYPES	3
#define FACTORY_FLAG		0
#define CYBORG_FLAG			1
#define VTOL_FLAG			2
#define REPAIR_FLAG			3
//seperate the numfactory from numflag
#define NUM_FLAG_TYPES      4

//this is used for module graphics - factory and vtol factory
#define NUM_FACMOD_TYPES	2

typedef struct _production_run
{
	uint8_t						quantity;			//number to build
	uint8_t						built;				//number built on current run
	struct _droid_template		*psTemplate;		//template to build
} PRODUCTION_RUN;

/* structure stats which can be upgraded by research*/
typedef struct _structure_upgrade
{
	uint16_t			armour;
	uint16_t			body;
	uint16_t			resistance;
} STRUCTURE_UPGRADE;

/* wall/Defence structure stats which can be upgraded by research*/
typedef struct _wallDefence_upgrade
{
	uint16_t			armour;
	uint16_t			body;
} WALLDEFENCE_UPGRADE;

typedef struct _upgrade
{
	uint16_t		modifier;		//% to increase the stat by
} UPGRADE;

typedef UPGRADE		RESEARCH_UPGRADE;
typedef UPGRADE		PRODUCTION_UPGRADE;
typedef UPGRADE		REPAIR_FACILITY_UPGRADE;
typedef UPGRADE		POWER_UPGRADE;
typedef UPGRADE		REARM_UPGRADE;

#endif // __INCLUDED_STRUCTUREDEF_H__
