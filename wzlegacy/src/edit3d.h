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

#ifndef __INCLUDED_SRC_EDIT3D_H__
#define __INCLUDED_SRC_EDIT3D_H__

#include "map.h"

#define TILE_RAISE	1
#define TILE_LOWER	-1
#define MAX_TILE_HEIGHT 255
#define MIN_TILE_HEIGHT	0

typedef void (*BUILDCALLBACK)(uint32_t xPos, uint32_t yPos, void *UserData);

extern void Edit3DInitVars(void);
extern	BOOL	found3DBuilding		( uint32_t *x, uint32_t *y );
extern  BOOL    found3DBuildLocTwo  ( uint32_t *px1, uint32_t *py1, uint32_t *px2, uint32_t *py2);
extern void init3DBuilding(BASE_STATS *psStats, BUILDCALLBACK CallBack, void *UserData);
extern	void	kill3DBuilding		( void );
extern BOOL process3DBuilding(void);

extern void	adjustTileHeight	( MAPTILE *psTile, int32_t adjust );
extern void	raiseTile(int tile3dX, int tile3dY);
extern void	lowerTile(int tile3dX, int tile3dY);
BOOL	inHighlight				( uint32_t realX, uint32_t realY );

typedef struct _highlight
{
	uint16_t	xTL, yTL;		// Top left of box to highlight
	uint16_t	xBR, yBR;		// Bottom right of box to highlight
} HIGHLIGHT;

extern HIGHLIGHT	buildSite;


#define BUILD3D_NONE		99
#define BUILD3D_POS			100
#define BUILD3D_FINISHED	101
#define BUILD3D_VALID		102


typedef struct _build_details
{
	BUILDCALLBACK	CallBack;
	void 			*UserData;  //this holds the OBJECT_POSITION pointer for a Deliv Point
	uint32_t			x, y;
	uint32_t			width, height;
	BASE_STATS		*psStats;
} BUILDDETAILS;

extern BUILDDETAILS	sBuildDetails;

extern uint32_t buildState;
extern uint32_t temp;
extern int brushSize;
extern bool editMode;
extern bool quickQueueMode;

/*returns true if the build state is not equal to BUILD3D_NONE*/
extern BOOL   tryingToGetLocation(void);

#endif // __INCLUDED_SRC_EDIT3D_H__
