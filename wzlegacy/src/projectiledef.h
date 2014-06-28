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
 *  Definitions for projectiles.
 */

#ifndef __INCLUDED_PROJECTILEDEF_H__
#define __INCLUDED_PROJECTILEDEF_H__

#include "basedef.h"

typedef enum PROJ_STATE
{
	PROJ_INFLIGHTDIRECT,
	PROJ_INFLIGHTINDIRECT,
	PROJ_IMPACT,
	PROJ_POSTIMPACT
} PROJ_STATE;

typedef struct PROJECTILE
{
	/* Use only simple object elements */
	SIMPLE_ELEMENTS( struct PROJECTILE );

	uint8_t           state;                  ///< current projectile state

	uint8_t           player;                 ///< needed because damange and radDamage vary from base stat per player because of upgrades

	uint8_t           bVisible;               ///< whether the selected player should see the projectile

	WEAPON_STATS   *psWStats;               ///< firing weapon stats

	BASE_OBJECT    *psSource;               ///< what fired the projectile
	BASE_OBJECT    *psDest;                 ///< target of this projectile
	BASE_OBJECT   **psDamaged;              ///< the targets that have already been dealt damage to (don't damage the same target twice)
	unsigned        psNumDamaged;

	uint32_t          startX, startY;         ///< Where projectile started
	uint32_t          tarX, tarY;             ///< The target coordinates
	int32_t          vXY, vZ;                ///< axis velocities
	uint32_t          srcHeight;              ///< Height of origin
	int32_t          altChange;              ///< Change in altitude
	uint32_t          born;
	uint32_t          died;
} PROJECTILE;

#endif // __INCLUDED_PROJECTILEDEF_H__
