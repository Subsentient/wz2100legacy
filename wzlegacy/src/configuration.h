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
 *  load and save favourites to the config file
 */

#ifndef __INCLUDED_SRC_CONFIGURATION_H__
#define __INCLUDED_SRC_CONFIGURATION_H__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "version.h" /*This smells like a bad idea for obvious reasons, but I think it's worth it.*/

    /*Configuration paths and whatnot.*/
#define CONFIG_FILENAME "legacyconfig.txt"
#if defined(WZ_OS_WIN)
#  undef WZ_DATADIR
#endif

#ifndef WZ_DATADIR
#define WZ_DATADIR "data"
#endif

#ifdef WZ_OS_WIN
# define WZ_WRITEDIR "Warzone 2100 Legacy " VERSIONBIG
#elif defined(WZ_OS_MAC)
# include <CoreServices/CoreServices.h>
# include <unistd.h>
# define WZ_WRITEDIR "Warzone 2100 Legacy " VERSIONBIG
#else
# define WZ_WRITEDIR ".wz2100legacy-" VERSIONBIG
#endif

    /*Functions.*/

    BOOL loadConfig(void);
    BOOL loadRenderMode(void);
    BOOL saveConfig(void);
    BOOL reloadMPConfig(void);
    void closeConfig( void );
    void setDefaultFrameRateLimit(void);

/// Default map for Skirmish
    static const char DEFAULTSKIRMISHMAP[] = "Sk-Rush";

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INCLUDED_SRC_CONFIGURATION_H__
