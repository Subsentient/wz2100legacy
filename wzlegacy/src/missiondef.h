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
 *  Definitions for missions.
 */

#ifndef __INCLUDED_MISSIONDEF_H__
#define __INCLUDED_MISSIONDEF_H__

#include "map.h"
#include "structuredef.h"
#include "droiddef.h"
#include "featuredef.h"
#include "power.h"


//mission types

//used to set the reinforcement time on hold whilst the Transporter is unable to land
//hopefully they'll never need to set it this high for other reasons!
#define SCR_LZ_COMPROMISED_TIME     999999
//this is used to compare the value passed in from the scripts with which is multiplied by 100
#define LZ_COMPROMISED_TIME         99999900

typedef struct _landing_zone
{
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
} LANDING_ZONE;

//storage structure for values that need to be kept between missions
typedef struct _mission
{
    //MISSION_TYPE		type;							//defines which start and end functions to use
    uint32_t				type;							//defines which start and end functions to use - see levels_type in levels.h
    MAPTILE				*psMapTiles;					//the original mapTiles
    uint32_t				mapWidth;						//the original mapWidth
    uint32_t				mapHeight;						//the original mapHeight
    struct _gateway		*psGateways;					//the gateway list
    uint8_t				**apRLEZones;					//the RLE map zones
    int32_t				gwNumZones;						//the number of map zones
    uint8_t				*aNumEquiv;						//zone equivalence data
    uint8_t				**apEquivZones;
    uint8_t				*aZoneReachable;
    uint32_t				scrollMinX;						//scroll coords for original map
    uint32_t				scrollMinY;
    uint32_t				scrollMaxX;
    uint32_t				scrollMaxY;
    STRUCTURE					*apsStructLists[MAX_PLAYERS];	//original object lists
    DROID						*apsDroidLists[MAX_PLAYERS];
    FEATURE						*apsFeatureLists[MAX_PLAYERS];
    BASE_OBJECT			*apsSensorList[1];
    //struct _proximity_display	*apsProxDisp[MAX_PLAYERS];
    FLAG_POSITION				*apsFlagPosLists[MAX_PLAYERS];
    PLAYER_POWER				asPower[MAX_PLAYERS];

    uint32_t				startTime;			//time the mission started
    int32_t				time;				//how long the mission can last
    // < 0 = no limit
    int32_t				ETA;				//time taken for reinforcements to arrive
    // < 0 = none allowed
    uint32_t				cheatTime;			//time the cheating started (mission time-wise!)

    //LANDING_ZONE		homeLZ;
    uint16_t               homeLZ_X;           //selectedPlayer's LZ x and y
    uint16_t               homeLZ_Y;
    int32_t				playerX;			//original view position
    int32_t				playerY;

    /* transporter entry/exit tiles */
    uint16_t				iTranspEntryTileX[MAX_PLAYERS];
    uint16_t				iTranspEntryTileY[MAX_PLAYERS];
    uint16_t				iTranspExitTileX[MAX_PLAYERS];
    uint16_t				iTranspExitTileY[MAX_PLAYERS];

} MISSION;

#endif // __INCLUDED_MISSIONDEF_H__
