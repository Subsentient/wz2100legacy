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
#ifndef __INCLUDED_INTIMAGE__
#define __INCLUDED_INTIMAGE__

#include "intfac.h" // Interface image id's.

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define FILLRED 16
#define FILLGREEN 16
#define FILLBLUE 128
#define FILLTRANS 128

/** Frame type */
typedef enum
{
	FRAME_NORMAL, FRAME_RADAR
}
FRAMETYPE;

typedef struct
{
	int16_t MajorUp;			//< Index of image to use for tab not pressed.
	int16_t MajorDown;		//< Index of image to use for tab pressed.
	int16_t MajorHilight;		//< Index of image to use for tab hilighted by mouse.
	int16_t MajorSelected;		//< Index of image to use for tab selected (same as pressed).

	int16_t MinorUp;			//< As above but for minor tabs.
	int16_t MinorDown;
	int16_t MinorHilight;
	int16_t MinorSelected;
} TABDEF;

extern IMAGEFILE *IntImages;	//< All the 2d graphics for the user interface.

// A few useful defined tabs.
extern TABDEF StandardTab;
extern TABDEF SmallTab;

BOOL imageInitBitmaps(void);

/** Draws a transparent window. */
void RenderWindowFrame(FRAMETYPE frame, uint32_t x, uint32_t y, uint32_t Width, uint32_t Height);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
