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
Edit3D.c - to ultimately contain the map editing functions -
they are presently scattered in various files .
Alex McLean, Pumpkin Studios, EIDOS Interactive, 1997
*/

#include "lib/framework/frame.h"
#include "map.h"
#include "edit3d.h"
#include "display3d.h"
#include "objects.h"
#include "display.h"
#include "hci.h"

/*
Definition of a tile to highlight - presently more than is required
but means that we can highlight any individual tile in future. An
x coordinate that is greater than mapWidth implies that the highlight
is invalid (not currently being used)
*/

uint32_t	buildState = BUILD3D_NONE;
BUILDDETAILS	sBuildDetails;
HIGHLIGHT		buildSite;
int brushSize = 1;
bool editMode = false;
bool quickQueueMode = false;

// Initialisation function for statis & globals in this module.
//
void Edit3DInitVars(void)
{
    buildState = BUILD3D_NONE;
    brushSize = 1;
}

/* Raises a tile by a #defined height */
void raiseTile(int tile3dX, int tile3dY)
{
    int i, j;

    if (tile3dX < 0 || tile3dX > mapWidth - 1 || tile3dY < 0 || tile3dY > mapHeight - 1)
    {
        return;
    }
    for (i = tile3dX; i <= MIN(mapWidth - 1, tile3dX + brushSize); i++)
    {
        for (j = tile3dY; j <= MIN(mapHeight - 1, tile3dY + brushSize); j++)
        {
            adjustTileHeight(mapTile(i, j), TILE_RAISE);
        }
    }
}

/* Lowers a tile by a #defined height */
void lowerTile(int tile3dX, int tile3dY)
{
    int i, j;

    if (tile3dX < 0 || tile3dX > mapWidth - 1 || tile3dY < 0 || tile3dY > mapHeight - 1)
    {
        return;
    }
    for (i = tile3dX; i <= MIN(mapWidth - 1, tile3dX + brushSize); i++)
    {
        for (j = tile3dY; j <= MIN(mapHeight - 1, tile3dY + brushSize); j++)
        {
            adjustTileHeight(mapTile(i, j), TILE_LOWER);
        }
    }
}

/* Ensures any adjustment to tile elevation is within allowed ranges */
void	adjustTileHeight(MAPTILE *psTile, int32_t adjust)
{
    int32_t	newHeight;

    newHeight = psTile->height + adjust;
    if (newHeight>=MIN_TILE_HEIGHT && newHeight<=MAX_TILE_HEIGHT)
    {
        psTile->height=(unsigned char) newHeight;
    }
}

BOOL	inHighlight(uint32_t realX, uint32_t realY)
{
    BOOL	retVal = false;

    if (realX>=buildSite.xTL && realX<=buildSite.xBR)
    {
        if (realY>=buildSite.yTL && realY<=buildSite.yBR)
        {
            retVal = true;
        }
    }

    return(retVal);
}

void init3DBuilding(BASE_STATS *psStats,BUILDCALLBACK CallBack,void *UserData)
{
    ASSERT(psStats, "Bad parameter");
    if (!psStats)
    {
        return;
    }

    buildState = BUILD3D_POS;

    sBuildDetails.CallBack = CallBack;
    sBuildDetails.UserData = UserData;
    sBuildDetails.x = mouseTileX;
    sBuildDetails.y = mouseTileY;

    if (psStats->ref >= REF_STRUCTURE_START &&
            psStats->ref < (REF_STRUCTURE_START + REF_RANGE))
    {
        sBuildDetails.width = ((STRUCTURE_STATS *)psStats)->baseWidth;
        sBuildDetails.height = ((STRUCTURE_STATS *)psStats)->baseBreadth;
        sBuildDetails.psStats = psStats;

        // hack to increase the size of repair facilities
        if (((STRUCTURE_STATS *)psStats)->type == REF_REPAIR_FACILITY)
        {
            sBuildDetails.width += 2;
            sBuildDetails.height += 2;
        }
    }
    else if (psStats->ref >= REF_FEATURE_START &&
             psStats->ref < (REF_FEATURE_START + REF_RANGE))
    {
        sBuildDetails.width = ((FEATURE_STATS *)psStats)->baseWidth;
        sBuildDetails.height = ((FEATURE_STATS *)psStats)->baseBreadth;
        sBuildDetails.psStats = psStats;
    }
    else /*if (psStats->ref >= REF_TEMPLATE_START &&
			 psStats->ref < (REF_TEMPLATE_START + REF_RANGE))*/
    {
        sBuildDetails.width = 1;
        sBuildDetails.height = 1;
        sBuildDetails.psStats = psStats;
    }
}

void	kill3DBuilding		( void )
{
    CancelDeliveryRepos();
    //cancel the drag boxes
    dragBox3D.status = DRAG_INACTIVE;
    wallDrag.status = DRAG_INACTIVE;
    buildState = BUILD3D_NONE;
}


// Call once per frame to handle structure positioning and callbacks.
//
BOOL process3DBuilding(void)
{
    uint32_t	bX,bY;

    //if not trying to build ignore
    if (buildState == BUILD3D_NONE)
    {
        if (quickQueueMode && !ctrlShiftDown())
        {
            quickQueueMode = false;
            intDemolishCancel();
        }
        return true;
    }


    if (buildState != BUILD3D_FINISHED)// && buildState != BUILD3D_NONE)
    {
        bX = mouseTileX;
        bY = mouseTileY;
        // lovely hack to make the repair facility 3x3 - need to offset the position by 1
        if (((STRUCTURE_STATS *)sBuildDetails.psStats)->type == REF_REPAIR_FACILITY)
        {
            bX += 1;
            bY += 1;
        }

        if (validLocation(sBuildDetails.psStats, bX, bY, selectedPlayer, true))
        {
            buildState = BUILD3D_VALID;
        }
        else
        {
            buildState = BUILD3D_POS;
        }
    }

    /* Need to update the building locations if we're building */
    bX = mouseTileX;
    bY = mouseTileY;

    if(mouseTileX<2)
    {
        bX = 2;
    }
    else
    {
        bX = mouseTileX;
    }
    if(mouseTileX > (int32_t)(mapWidth-3))
    {
        bX = mapWidth-3;
    }
    else
    {
        bX = mouseTileX;
    }

    if(mouseTileY<2)
    {
        bY = 2;
    }
    else
    {
        bY = mouseTileY;
    }
    if(mouseTileY > (int32_t)(mapHeight-3))
    {
        bY = mapHeight-3;
    }
    else
    {
        bY = mouseTileY;
    }

    sBuildDetails.x = buildSite.xTL = (uint16_t)bX;
    sBuildDetails.y = buildSite.yTL = (uint16_t)bY;
    buildSite.xBR = (uint16_t)(buildSite.xTL+sBuildDetails.width-1);
    buildSite.yBR = (uint16_t)(buildSite.yTL+sBuildDetails.height-1);

    if( (buildState == BUILD3D_FINISHED) && (sBuildDetails.CallBack != NULL) )
    {
        sBuildDetails.CallBack(sBuildDetails.x,sBuildDetails.y,sBuildDetails.UserData);
        buildState = BUILD3D_NONE;
        return true;
    }
    if (quickQueueMode && !ctrlShiftDown())
    {
        buildState = BUILD3D_NONE;
        quickQueueMode = false;
    }

    return false;
}


/* See if a structure location has been found */
BOOL found3DBuilding(uint32_t *x, uint32_t *y)
{
    if (buildState != BUILD3D_FINISHED || x == NULL || y == NULL)
    {
        return false;
    }

    *x = sBuildDetails.x;
    *y = sBuildDetails.y;

    // lovely hack to make the repair facility 3x3 - need to offset the position by 1
    if (((STRUCTURE_STATS *)sBuildDetails.psStats)->type == REF_REPAIR_FACILITY)
    {
        *x += 1;
        *y += 1;
    }

    if (ctrlShiftDown())
    {
        quickQueueMode = true;
        init3DBuilding(sBuildDetails.psStats, NULL, NULL);
    }
    else
    {
        buildState = BUILD3D_NONE;
    }

    return true;
}

/* See if a second position for a build has been found */
BOOL found3DBuildLocTwo(uint32_t *px1, uint32_t *py1, uint32_t *px2, uint32_t *py2)
{
    if ( (((STRUCTURE_STATS *)sBuildDetails.psStats)->type != REF_WALL &&
            ((STRUCTURE_STATS *)sBuildDetails.psStats)->type != REF_DEFENSE &&
            ((STRUCTURE_STATS *)sBuildDetails.psStats)->type != REF_REARM_PAD &&
            ((STRUCTURE_STATS *)sBuildDetails.psStats)->type != REF_RESOURCE_EXTRACTOR) ||
            wallDrag.status != DRAG_RELEASED)
    {
        return false;
    }

    //whilst we're still looking for a valid location - return false
    if (buildState == BUILD3D_POS)
    {
        return false;
    }

    wallDrag.status = DRAG_INACTIVE;
    *px1 = wallDrag.x1;
    *py1 = wallDrag.y1;
    *px2 = wallDrag.x2;
    *py2 = wallDrag.y2;

    if (ctrlShiftDown())
    {
        quickQueueMode = true;
        init3DBuilding(sBuildDetails.psStats, NULL, NULL);
    }

    return true;
}

/*returns true if the build state is not equal to BUILD3D_NONE*/
BOOL tryingToGetLocation(void)
{
    if (buildState == BUILD3D_NONE)
    {
        return false;
    }
    else
    {
        return true;
    }
}
