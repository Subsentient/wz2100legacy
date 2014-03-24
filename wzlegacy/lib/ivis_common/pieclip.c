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
#include "pieclip.h"
#include "ivi.h"

static uint32_t videoBufferDepth = 32, videoBufferWidth = 0, videoBufferHeight = 0;

BOOL pie_SetVideoBufferDepth(uint32_t depth)
{
    videoBufferDepth = depth;
    return(true);
}

BOOL pie_SetVideoBufferWidth(uint32_t width)
{
    videoBufferWidth = width;
    return(true);
}

BOOL pie_SetVideoBufferHeight(uint32_t height)
{
    videoBufferHeight = height;
    return(true);
}

uint32_t pie_GetVideoBufferDepth(void)
{
    return(videoBufferDepth);
}

uint32_t pie_GetVideoBufferWidth(void)
{
    return(videoBufferWidth);
}

uint32_t pie_GetVideoBufferHeight(void)
{
    return(videoBufferHeight);
}
