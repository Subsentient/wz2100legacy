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
/** \file
 *  Functions for the base object type.
 */

#ifndef __INCLUDED_BASEOBJECT_H__
#define __INCLUDED_BASEOBJECT_H__

#include "basedef.h"

static const unsigned int max_check_object_recursion = 4;

void checkObject(const BASE_OBJECT* psObject, const char * const location_description, const char * function, const int recurse);

/* assert if object is bad */
#define CHECK_OBJECT(object) checkObject((object), AT_MACRO, __FUNCTION__, max_check_object_recursion)

#endif // __INCLUDED_BASEOBJECT_H__
