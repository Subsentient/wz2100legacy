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
/*
 * EvntSave.h
 *
 * Save the state of the event system.
 *
 */

#ifndef _evntsave_h
#define _evntsave_h

// Save the state of the event system
extern BOOL eventSaveState(SDWORD version, char **ppBuffer, UDWORD *pFileSize);

// Load the state of the event system
extern BOOL eventLoadState(char *pBuffer, UDWORD fileSize, BOOL bHashed);

#endif


