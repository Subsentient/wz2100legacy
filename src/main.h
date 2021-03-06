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

#ifndef __INCLUDED_SRC_MAIN_H__
#define __INCLUDED_SRC_MAIN_H__

typedef enum
{
	GS_TITLE_SCREEN,
	GS_NORMAL,
	GS_SAVEGAMELOAD
} GS_GAMEMODE;

//flag to indicate when initialisation is complete
extern BOOL gameInitialised;
extern BOOL bDisableLobby;
extern bool customDebugfile;
extern GS_GAMEMODE GetGameMode(void) WZ_DECL_PURE;
extern void SetGameMode(GS_GAMEMODE status);

extern char SaveGamePath[PATH_MAX];
extern char datadir[PATH_MAX];
extern char configdir[PATH_MAX];
extern char KeyMapPath[PATH_MAX];
extern char MultiPlayersPath[PATH_MAX];

#define MAX_MODS 100

extern char *global_mods[MAX_MODS];
extern char *campaign_mods[MAX_MODS];
extern char *multiplay_mods[MAX_MODS];

extern char *override_mods[MAX_MODS];
extern char *override_mod_list;
extern bool use_override_mods;

void setCurrentMap(char *map, int maxPlayers);
extern char *current_map[3];

#endif // __INCLUDED_SRC_MAIN_H__
