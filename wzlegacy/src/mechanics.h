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
 * Game world mechanics.
 */

#ifndef __INCLUDED_SRC_MECHANICS_H__
#define __INCLUDED_SRC_MECHANICS_H__

#include "statsdef.h"

/* Shutdown the mechanics system */
bool mechanicsShutdown(void);

// Allocate the list for a component
BOOL allocComponentList(COMPONENT_TYPE	type, SDWORD number);

// release all the component lists
void freeComponentLists(void);

//allocate the space for the Players' structure lists
BOOL allocStructLists(void);

// release the structure lists
void freeStructureLists(void);

//TEST FUNCTION - MAKE EVERYTHING AVAILABLE
void makeAllAvailable(void);

#endif // __INCLUDED_SRC_MECHANICS_H__
