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
/***************************************************************************/
/*
 * pieclip.h
 *
 * clipping for all pumpkin image library functions.
 *
 */
/***************************************************************************/

#ifndef _pieclip_h
#define _pieclip_h

/***************************************************************************/

#include "lib/framework/frame.h"
#include "piedef.h"


/***************************************************************************/
/*
 *	Global Definitions
 */
/***************************************************************************/

typedef struct
{
    Vector3i pos;
    unsigned int u, v;
    PIELIGHT light;
} CLIP_VERTEX;

/***************************************************************************/
/*
 *	Global ProtoTypes
 */
/***************************************************************************/

extern BOOL pie_SetVideoBufferDepth(uint32_t depth);
extern BOOL pie_SetVideoBufferWidth(uint32_t width);
extern BOOL pie_SetVideoBufferHeight(uint32_t height);
extern uint32_t pie_GetVideoBufferDepth( void ) WZ_DECL_PURE;
extern uint32_t pie_GetVideoBufferWidth( void ) WZ_DECL_PURE;
extern uint32_t pie_GetVideoBufferHeight( void ) WZ_DECL_PURE;

#endif // _pieclip_h
