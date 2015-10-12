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

#ifndef __INCLUDED_SRC_SELECTION_H__
#define __INCLUDED_SRC_SELECTION_H__

typedef enum _selection_class
{
	DS_ALL_UNITS,
	DS_BY_TYPE
} SELECTION_CLASS;

typedef enum _selectiontype
{
	DST_UNUSED,
	DST_VTOL,
	DST_HOVER,
	DST_WHEELED,
	DST_TRACKED,
	DST_HALF_TRACKED,
	DST_ALL_COMBAT,
	DST_ALL_DAMAGED,
	DST_ALL_SAME,
	DST_CONSTRUCTORS,
	DST_ALL_GROUND_COMBAT
} SELECTIONTYPE;

// EXTERNALLY REFERENCED FUNCTIONS
extern uint32_t	selDroidSelection( uint32_t	player, SELECTION_CLASS droidClass,
								   SELECTIONTYPE droidType, BOOL bOnScreen );
extern uint32_t	selDroidDeselect		( uint32_t player );
extern uint32_t	selNumSelected			( uint32_t player );
extern void	selNextRepairUnit			( void );
extern void selNextUnassignedUnit		( void );
extern void	selNextSpecifiedBuilding	( uint32_t structType );
extern	void	selNextSpecifiedUnit	(uint32_t unitType);
// select the n'th command droid
extern void selCommander(int32_t n);

#endif // __INCLUDED_SRC_SELECTION_H__
