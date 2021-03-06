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
 *  All game variable access functions for the scripts
 */

#ifndef __INCLUDED_SRC_SCRIPTEXTERN_H__
#define __INCLUDED_SRC_SCRIPTEXTERN_H__

// current game level
extern int32_t	scrGameLevel;
// whether the tutorial is active
extern BOOL		bInTutorial;
// whether any additional special case victory/failure conditions have been met
extern BOOL		bExtraVictoryFlag;
extern BOOL		bExtraFailFlag;
extern BOOL		bTrackTransporter;


// ID numbers for external variables
enum _externids
{
	EXTID_MAPWIDTH,
	EXTID_MAPHEIGHT,
	EXTID_GAMEINIT,
	EXTID_SELECTEDPLAYER,
	EXTID_GAMELEVEL,
	EXTID_GAMETIME,
	EXTID_TUTORIAL,
	EXTID_MULTIGAMETYPE,
	EXTID_MULTIGAMEHUMANMAX,
	EXTID_MULTIGAMEBASETYPE,
	EXTID_CURSOR,
	EXTID_INTMODE,
	EXTID_TARGETTYPE,				// IHATESCRIPTSANDEVERYTHINGTHEYSTANDFOR(ESPECIALLYONSUNDAYS)
	EXTID_EXTRAVICTORYFLAG,
	EXTID_EXTRAFAILFLAG,
	EXTID_TRACKTRANSPORTER,
	EXTID_MULTIGAMEALLIANCESTYPE,
};

// reset the script externals for a new level
extern void scrExternReset(void);

// General function to get some basic game values
extern BOOL scrGenExternGet(uint32_t index);

// General function to set some basic game values
extern BOOL scrGenExternSet(uint32_t index);

#endif // __INCLUDED_SRC_SCRIPTEXTERN_H__
