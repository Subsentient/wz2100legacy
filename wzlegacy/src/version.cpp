/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.*/
/*
	This file is part of Warzone 2100.
	Copyright (C) 2007  Giel van Schijndel
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

#include "lib/framework/frame.h"
#include "lib/framework/debug.h"
#include "lib/framework/string_ext.h"
#include "lib/framework/stdio_ext.h"
#include "version.h"
#include "stringdef.h"

const char* legacyVersion = "microwave_popping"; //Set the version of the game.
//We removed all that version control stuff, since Legacy should be able to be comfortably developed with none whatsoever.

const char* version_getBuildDate()
{
	return __DATE__;
}

const char* version_getBuildTime()
{
	return __TIME__;
}

const char* version_getFormattedVersionString()
{
	static char versionString[MAX_STR_LENGTH] = {'\0'};

	if (versionString[0] == '\0')
	{
		// Compose the working copy state string
		//Subsentient changed the version string layout and such.
		// Compose the build type string
#ifdef DEBUG
		const char* build_type = _(" (debug build)");
#else
		const char* build_type = "";
#endif

		const char* build_date = NULL;

		sasprintf((char**)&build_date, _(" - Compiled on %s"), version_getBuildDate());

		snprintf(versionString, MAX_STR_LENGTH, _("Warzone 2100 Legacy Version: %s%s%s"), 
		legacyVersion, build_date, build_type);
	}

	return versionString;
}
