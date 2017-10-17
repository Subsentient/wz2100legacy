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
 * piefunc.h
 *
 * type defines for extended image library functions.
 *
 */
/***************************************************************************/

#ifndef _piefunc_h
#define _piefunc_h

#include "lib/framework/frame.h"
#include "lib/ivis_common/piedef.h"
#include "lib/ivis_common/pieclip.h"

extern uint8_t pie_ByteScale(uint8_t a, uint8_t b) WZ_DECL_CONST;
extern void pie_TransColouredTriangle(CLIP_VERTEX *vrt, PIELIGHT c);
extern void pie_DrawSkybox(float scale, int u, int v, int w, int h);
extern void pie_DrawFogBox(float left, float right, float front, float back, float height, float wider);
extern void pie_DrawViewingWindow( Vector3i *v, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, PIELIGHT colour);

void pie_ClipBegin(int x1, int y1, int x2, int y2);
void pie_ClipEnd(void);

#endif // _piedef_h
