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

#ifndef __INCLUDED_SRC_DISPLAY3D_H__
#define __INCLUDED_SRC_DISPLAY3D_H__

#include "display.h"
#include "display3ddef.h"	// This should be the only place including this file
#include "lib/ivis_common/pietypes.h"
#include "lib/ivis_common/piedef.h"
#include "objectdef.h"
#include "message.h"


/*!
 * Special tile types
 */
typedef enum
{
    RIVERBED_TILE = 5, //! Underwater ground
    WATER_TILE = 17, //! Water surface
    RUBBLE_TILE = 54, //! You can drive over these
    BLOCKING_RUBBLE_TILE = 67 //! You cannot drive over these
} TILE_ID;

typedef enum
{
    BAR_SELECTED,
    BAR_DROIDS,
    BAR_DROIDS_AND_STRUCTURES,
    BAR_LAST
} ENERGY_BAR;

extern bool showFPS;
extern bool showSAMPLES;
extern bool showORDERS;
extern bool showLevelName;
extern bool showTicker;

extern void	setViewAngle(int32_t angle);
extern uint32_t getViewDistance(void);
extern void	setViewDistance(uint32_t dist);
extern BOOL	radarOnScreen;
extern bool rangeOnScreen; // Added to get sensor/gun range on screen.  -Q 5-10-05
extern void	scaleMatrix( uint32_t percent );
extern void setViewPos( uint32_t x, uint32_t y, BOOL Pan);
extern void getPlayerPos(int32_t *px, int32_t *py);
extern void setPlayerPos(int32_t x, int32_t y);
extern void disp3d_setView(iView *newView);
extern void disp3d_resetView(void);
extern void disp3d_getView(iView *newView);

extern void draw3DScene (void);
extern void renderDroid					( DROID *psDroid );
extern void renderStructure				( STRUCTURE *psStructure);
extern void renderFeature				( FEATURE *psFeature );
extern void renderProximityMsg			( PROXIMITY_DISPLAY	*psProxDisp);
extern void renderProjectile			( PROJECTILE *psCurr);
extern void renderAnimComponent			( const COMPONENT_OBJECT *psObj );
extern void renderDeliveryPoint			( FLAG_POSITION *psPosition, BOOL blueprint );
extern void debugToggleSensorDisplay	( void );

extern void displayFeatures( void );
extern void displayStaticObjects( void );
extern void displayDynamicObjects( void );
extern void displayProximityMsgs( void );
extern void displayDelivPoints(void);
extern void calcScreenCoords(DROID *psDroid);
extern ENERGY_BAR toggleEnergyBars( void );

extern BOOL doWeDrawProximitys( void );
extern void setProximityDraw(BOOL val);
extern void renderShadow( DROID *psDroid, iIMDShape *psShadowIMD );

extern PIELIGHT getTileColour(int x, int y);
extern void setTileColour(int x, int y, PIELIGHT colour);

extern BOOL	clipXY ( int32_t x, int32_t y);

extern BOOL init3DView(void);
extern void initViewPosition(void);
extern iView player;
extern BOOL selectAttempt;
extern BOOL draggingTile;
extern iIMDShape *g_imd;
extern BOOL	droidSelected;
extern uint32_t terrainMidX,terrainMidY;

extern int32_t scrollSpeed;
//extern void	assignSensorTarget( DROID *psDroid );
extern void assignSensorTarget( BASE_OBJECT *psObj );
extern void assignDestTarget( void );
extern uint32_t getWaterTileNum( void);
extern void setUnderwaterTile(uint32_t num);
extern uint32_t getRubbleTileNum( void );
extern void setRubbleTile(uint32_t num);

extern int32_t	getCentreX( void );
extern int32_t	getCentreZ( void );

extern int32_t mouseTileX, mouseTileY;

extern BOOL bRender3DOnly;
extern BOOL showGateways;
extern BOOL showPath;
extern Vector2i visibleTiles;

/*returns the graphic ID for a droid rank*/
extern uint32_t  getDroidRankGraphic(DROID *psDroid);

/* Visualize radius at position */
extern void showRangeAtPos(int32_t centerX, int32_t centerY, int32_t radius);

#define	BASE_MUZZLE_FLASH_DURATION	(GAME_TICKS_PER_SEC/10)
#define	EFFECT_MUZZLE_ADDITIVE		128

#define BAR_FULL	0
#define BAR_BASIC	1
#define BAR_DOT		2
#define BAR_NONE	3

extern uint16_t barMode;
extern uint32_t geoOffset;

extern bool CauseCrash;

#endif // __INCLUDED_SRC_DISPLAY3D_H__
