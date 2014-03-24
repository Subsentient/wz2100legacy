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
 *  Definitions for the form functions.
 */

#ifndef __INCLUDED_LIB_WIDGET_FORM_H__
#define __INCLUDED_LIB_WIDGET_FORM_H__

#include "lib/widget/widget.h"

/* The basic form data */
#define FORM_BASE \
	WIDGET_BASE;				/* The common widget data */ \
	\
	BOOL		disableChildren;	/* Disable all child widgets if true */ \
	uint16_t		Ax0,Ay0,Ax1,Ay1; 	/* Working coords for animations. */ \
	uint32_t		animCount; 			/* Animation counter. */ \
	uint32_t		startTime;			/* Animation start time */ \
	PIELIGHT	aColours[WCOL_MAX];		/* Colours for the form and its widgets. signed since aColours -1 means use bitmap. */ \
	WIDGET		*psLastHiLite;	/* The last widget to be hilited */ \
								/* This is used to track when the mouse moves */ \
								/* off something */ \
	WIDGET		*psWidgets		/* The widgets on the form */


/* The standard form */
typedef struct _w_form
{
    /* The common form data */
    FORM_BASE;
} W_FORM;

/* Information for a minor tab */
typedef struct _w_minortab
{
    /* Graphics data for the tab will go here */
    WIDGET		*psWidgets;			// Widgets on the tab
    char		*pTip;				// Tool tip
} W_MINORTAB;

/* Information for a major tab */
typedef struct _w_majortab
{
    /* Graphics data for the tab will go here */
    uint16_t			lastMinor;					// Store which was the last selected minor tab
    uint16_t			numMinor;
    W_MINORTAB		asMinor[WFORM_MAXMINOR];	// Minor tab information
    char			*pTip;
} W_MAJORTAB;

/* The tabbed form data structure */
typedef struct _w_tabform
{
    /* The common form data */
    FORM_BASE;

    uint16_t		majorPos, minorPos;		// Position of the tabs on the form
    uint16_t		majorSize,minorSize;	// the size of tabs horizontally and vertically
    uint16_t		tabMajorThickness;			// The thickness of the tabs
    uint16_t		tabMinorThickness;			// The thickness of the tabs
    uint16_t		tabMajorGap;					// The gap between tabs
    uint16_t		tabMinorGap;					// The gap between tabs
    int16_t		tabVertOffset;				// Tab form overlap offset.
    int16_t		tabHorzOffset;				// Tab form overlap offset.
    int16_t		majorOffset;			// Tab start offset.
    int16_t		minorOffset;			// Tab start offset.
    uint16_t		majorT,minorT;			// which tab is selected
    uint16_t		state;					// Current state of the widget
    uint16_t		tabHiLite;				// which tab is hilited.
    /* NOTE: If tabHiLite is (uint16_t)(-1) then there is no hilite.  A bit of a hack I know */
    /*       but I don't really have the energy to change it.  (Don't design stuff after  */
    /*       beers at lunch-time :-)                                                      */

    uint16_t		numMajor;				// The number of major tabs
    int16_t		TabMultiplier;				//used to tell system we got lots of tabs to display
    uint16_t		numStats;				//# of 'stats' (items) in list
    uint16_t		numButtons;				//# of buttons per form
    W_MAJORTAB	asMajor[WFORM_MAXMAJOR];	// The major tab information
    TAB_DISPLAY pTabDisplay;			// Optional callback for display tabs.
} W_TABFORM;


/* Button states for a clickable form */
#define WCLICK_NORMAL		0x0000
#define WCLICK_DOWN			0x0001		// Button is down
#define WCLICK_GREY			0x0002		// Button is disabled
#define WCLICK_HILITE		0x0004		// Button is hilited
#define WCLICK_LOCKED		0x0008		// Button is locked down
#define WCLICK_CLICKLOCK	0x0010		// Button is locked but clickable
#define WCLICK_FLASH		0x0020		// Button flashing is enabled
#define WCLICK_FLASHON		0x0040		// Button is flashing

/* The clickable form data structure */
typedef struct _w_clickform
{
    /* The common form data */
    FORM_BASE;

    uint32_t		state;					// Button state of the form
    const char	*pTip;					// Tip for the form
    int16_t HilightAudioID;				// Audio ID for form clicked sound
    int16_t ClickedAudioID;				// Audio ID for form hilighted sound
    WIDGET_AUDIOCALLBACK AudioCallback;	// Pointer to audio callback function
} W_CLICKFORM;

extern void formClearFlash(W_FORM *psWidget);

/* Create a form widget data structure */
extern W_FORM *formCreate(const W_FORMINIT *psInit);

/* Free the memory used by a form */
extern void formFree(W_FORM *psWidget);

/* Add a widget to a form */
extern BOOL formAddWidget(W_FORM *psForm, WIDGET *psWidget, W_INIT *psInit);

/* Initialise a form widget before running it */
extern void formInitialise(W_FORM *psWidget);

/* Return the widgets currently displayed by a form */
extern WIDGET *formGetWidgets(W_FORM *psWidget);

/* Return the origin on the form from which button locations are calculated */
extern void formGetOrigin(W_FORM *psWidget, int32_t *pXOrigin, int32_t *pYOrigin);

/* Variables for the formGetAllWidgets functions */
typedef struct _w_formgetall
{
    WIDGET		*psGAWList;
    W_TABFORM	*psGAWForm;
    W_MAJORTAB	*psGAWMajor;
    uint32_t		GAWMajor, GAWMinor;
} W_FORMGETALL;

/* Initialise the formGetAllWidgets function */
extern void formInitGetAllWidgets(W_FORM *psWidget, W_FORMGETALL *psCtrl);

/* Repeated calls to this function will return widget lists
 * until all widgets in a form have been returned.
 * When a NULL list is returned, all widgets have been seen.
 */
extern WIDGET *formGetAllWidgets(W_FORMGETALL *psCtrl);

/* Get the button state of a click form */
extern uint32_t formGetClickState(W_CLICKFORM *psForm);

extern void formSetFlash(W_FORM *psWidget);

/* Set the button state of a click form */
extern void formSetClickState(W_CLICKFORM *psForm, uint32_t state);

/* Run a form widget */
extern void formRun(W_FORM *psWidget, W_CONTEXT *psContext);

/* Respond to a mouse click */
extern void formClicked(W_FORM *psWidget, uint32_t key);

/* Respond to a mouse form up */
extern void formReleased(W_FORM *psWidget, uint32_t key, W_CONTEXT *psContext);

/* Respond to a mouse moving over a form */
extern void formHiLite(W_FORM *psWidget, W_CONTEXT *psContext);

/* Respond to the mouse moving off a form */
extern void formHiLiteLost(W_FORM *psWidget, W_CONTEXT *psContext);

/* Display function prototypes */
extern void formDisplay(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);
extern void formDisplayClickable(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);
extern void formDisplayTabbed(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

#endif // __INCLUDED_LIB_WIDGET_FORM_H__
