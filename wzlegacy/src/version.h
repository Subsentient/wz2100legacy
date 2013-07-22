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

#ifndef __INCLUDED_VERSION_H__
#define __INCLUDED_VERSION_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define VERSIONBIG "microwave_popping"
#define VERSIONSMALL ""
#define VERSIONNUM VERSIONBIG VERSIONSMALL
#define PROJECTNAME "Warzone 2100 Legacy"
#define PROJECTURL "http://warzonelegacy.org/"
#define PROJECTURL_BUGS "http://forums.warzonelegacy.org/"

    extern char *GetVersionInfo(void);

#if defined(__cplusplus)
}
#endif

#endif
