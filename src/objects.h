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
 *  A header file that groups together all the object header files
 */

#ifndef __INCLUDED_SRC_OBJECTS_H__
#define __INCLUDED_SRC_OBJECTS_H__

#include "objectdef.h"
#include "droid.h"
#include "structure.h"
#include "feature.h"
#include "objmem.h"

#include "ai.h"
#include "move.h"
#include "function.h"
#include "stats.h"

/* Initialise the object system */
extern BOOL objInitialise(void);

/* Shutdown the object system */
extern BOOL objShutdown(void);

/*goes thru' the list passed in reversing the order so the first entry becomes
the last and the last entry becomes the first!*/
extern void reverseObjectList(BASE_OBJECT **ppsList);

/** Output an informative string about this object. For debugging. */
const char *objInfo(const BASE_OBJECT *psObj);

#endif // __INCLUDED_SRC_OBJECTS_H__
