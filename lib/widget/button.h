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
/** @file
 *  Definitions for edit box functions.
 */

#ifndef __INCLUDED_LIB_WIDGET_BUTTON_H__
#define __INCLUDED_LIB_WIDGET_BUTTON_H__

#include "widget.h"
#include "widgbase.h"
#include "lib/ivis_common/textdraw.h"

/* Button states */
#define WBUTS_NORMAL	0x0000
#define WBUTS_DOWN		0x0001		// Button is down
#define WBUTS_GREY		0x0002		// Button is disabled
#define WBUTS_HILITE	0x0004		// Button is hilited
#define WBUTS_LOCKED	0x0008		// Button is locked down
#define WBUTS_CLICKLOCK	0x0010		// Button is locked but clickable
#define WBUTS_FLASH		0x0020		// Button flashing is enabled
#define WBUTS_FLASHON	0x0040		// Button is flashing

typedef struct _w_button
{
	/* The common widget data */
	WIDGET_BASE;

	uint32_t		state;				// The current button state
	const char *pText;				// The text for the button
	const char *pTip;				// The tool tip for the button
	int16_t HilightAudioID;				// Audio ID for form clicked sound
	int16_t ClickedAudioID;				// Audio ID for form hilighted sound
	WIDGET_AUDIOCALLBACK AudioCallback;	// Pointer to audio callback function
	enum iV_fonts FontID;
} W_BUTTON;

/* Initialise the button module */
extern BOOL buttonStartUp(void);

/* Create a button widget data structure */
extern W_BUTTON *buttonCreate(const W_BUTINIT *psInit);

/* Free the memory used by a button */
extern void buttonFree(W_BUTTON *psWidget);

/* Initialise a button widget before running it */
extern void buttonInitialise(W_BUTTON *psWidget);

/* Run a button widget */
extern void buttonRun(W_BUTTON *psWidget);

/* Respond to a mouse click */
extern void buttonClicked(W_BUTTON *psWidget, uint32_t key);

/* Respond to a mouse button up */
extern void buttonReleased(W_SCREEN *psScreen, W_BUTTON *psWidget, uint32_t key);

/* Respond to a mouse moving over a button */
extern void buttonHiLite(W_BUTTON *psWidget, W_CONTEXT *psContext);

/* Respond to the mouse moving off a button */
extern void buttonHiLiteLost(W_BUTTON *psWidget);

/* Get a button's state */
extern uint32_t buttonGetState(W_BUTTON *psButton);

/* Set a button's state */
extern void buttonSetState(W_BUTTON *psWidget, uint32_t state);

extern void buttonSetFlash(W_BUTTON *psButton);
extern void buttonClearFlash(W_BUTTON *psButton);

/* The button display function */
extern void buttonDisplay(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

#endif // __INCLUDED_LIB_WIDGET_BUTTON_H__
