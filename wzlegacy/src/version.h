/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.*/
/*
	This file is part of Warzone 2100.
	Copyright (C) 2004  Giel van Schijndel
	Copyright (C) 2007-2012  Warzone 2100 Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef __INCLUDED_VERSION_H__
#define __INCLUDED_VERSION_H__

#include "lib/framework/types.h"

/*Variables*/

extern const char* legacyVersion; //Version of the game.

/*End of variables.*/

extern const char* version_getBuildDate(void);

/** Retrieves the time at which this build was compiled.
 *  \return the time at which this build was made (uses __TIME__)
 */
extern const char* version_getBuildTime(void);


/* Shows a well printed version piece containing all of Legacy's important version info. */
extern const char* version_getFormattedVersionString(void);

#endif // __INCLUDED_VERSION_H__
