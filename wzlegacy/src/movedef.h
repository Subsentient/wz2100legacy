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
 *  Definitions for movement tracking.
 */

#ifndef __INCLUDED_MOVEDEF_H__
#define __INCLUDED_MOVEDEF_H__

//Watermelon:num of VTOL weapons should be same as DROID_MAXWEAPS
#define VTOL_MAXWEAPS		3

typedef struct _move_control
{
	uint8_t	Status;						// Inactive, Navigating or moving point to point status
	uint8_t	Position;	   				// Position in asPath
	uint8_t	numPoints;					// number of points in asPath
	Vector2i *asPath;					// Pointer to list of block X,Y map coordinates.
	int32_t	DestinationX, DestinationY;			// World coordinates of movement destination
	int32_t	srcX, srcY, targetX, targetY;

	/* Stuff for John's movement update */
	float	fx, fy;						// droid location as a fract
	float	speed;						// Speed of motion
	int16_t	boundX, boundY;				// Vector for the end of path boundary

	float	moveDir;						// direction of motion (not the direction the droid is facing)
	int16_t	bumpDir;					// direction at last bump
	uint32_t	bumpTime;					// time of first bump with something
	uint16_t	lastBump;					// time of last bump with a droid - relative to bumpTime
	uint16_t	pauseTime;					// when MOVEPAUSE started - relative to bumpTime
	uint16_t	bumpX, bumpY;				// position of last bump

	uint32_t	shuffleStart;				// when a shuffle started

	struct _formation	*psFormation;			// formation the droid is currently a member of

	/* vtol movement - GJ */
	int16_t	iVertSpeed;

	// iAttackRuns tracks the amount of ammunition a VTOL has remaining for each weapon
	uint32_t	iAttackRuns[VTOL_MAXWEAPS];

	// added for vtol movement
	float	fz;
} MOVE_CONTROL;

#endif // __INCLUDED_MOVEDEF_H__
