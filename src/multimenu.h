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
 *  Definition for in game,multiplayer, interface.
 */

#ifndef __INCLUDED_SRC_MULTIMENU__
#define __INCLUDED_SRC_MULTIMENU__

#include "lib/widget/widgbase.h"
#include "stringdef.h"

// requester
extern void		addMultiRequest(const char *searchDir, const char *fileExtension, uint32_t id, uint8_t mapCam, uint8_t numPlayers);
extern BOOL		multiRequestUp;
extern W_SCREEN *psRScreen;			// requester stuff.
extern BOOL		runMultiRequester(uint32_t id, uint32_t *contextmode, char *chosen, uint32_t *chosenValue, short *isHoverPreview);
extern void		displayRequestOption(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

// multimenu
extern void		intProcessMultiMenu		(uint32_t id);
extern BOOL		intRunMultiMenu			(void);
extern BOOL		intCloseMultiMenu		(void);
extern void		intCloseMultiMenuNoAnim	(void);
extern BOOL		intAddMultiMenu			(void);

extern BOOL		addDebugMenu			(BOOL bAdd);
extern void		intCloseDebugMenuNoAnim	(void);
extern void		setDebugMenuEntry(char *entry, int32_t index);
extern bool autoCompleteName(const char *InStream, char *OutStream);

extern BOOL		MultiMenuUp;
extern BOOL		ClosingMultiMenu;

extern BOOL		DebugMenuUp;

extern uint32_t		current_numplayers;
extern uint32_t		current_tech;

#define MULTIMENU				10600
#define MULTIMENU_FORM			MULTIMENU

#define	DEBUGMENU				106000
#define	DEBUGMENU_CLOSE			(DEBUGMENU+1)
#define	DEBUGMENU_MAX_ENTRIES	10
#define	DEBUGMENU_BUTTON		(DEBUGMENU_CLOSE + DEBUGMENU_MAX_ENTRIES)

extern char		debugMenuEntry[DEBUGMENU_MAX_ENTRIES][MAX_STR_LENGTH];

#endif // __INCLUDED_SRC_MULTIMENU__
