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
 *  Definitions for the weapons.
 */

#ifndef __INCLUDED_WEAPONDEF_H__
#define __INCLUDED_WEAPONDEF_H__

typedef struct _weapon
{
	/**
	 * Index into the global @c asWeaponStats array; thus a "reference" of
	 * some kind to the associated stats.
	 */
	unsigned int    nStat;

	uint32_t          ammo;

	/**
	 * @c gameTime when this weapon was last fired.
	 */
	uint32_t          lastFired;
	uint32_t          recoilValue;
	uint16_t		pitch;
	uint16_t		rotation;
} WEAPON;

#endif // __INCLUDED_WEAPONDEF_H__
