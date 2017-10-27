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

#ifndef __INCLUDED_SRC_WARCAM_H__
#define __INCLUDED_SRC_WARCAM_H__

#include "lib/ivis_common/pietypes.h"
#include "objectdef.h"

#define X_UPDATE 0x1
#define Y_UPDATE 0x2
#define Z_UPDATE 0x4

#define CAM_X_AND_Y	(X_UPDATE + Y_UPDATE)

#define CAM_ALL (X_UPDATE + Y_UPDATE + Z_UPDATE)

#define ACCEL_CONSTANT 12.0f
#define VELOCITY_CONSTANT 4.0f
#define ROT_ACCEL_CONSTANT 4.0f
#define ROT_VELOCITY_CONSTANT 2.5f

#define CAM_X_SHIFT	((VISIBLE_XTILES/2)*128)
#define CAM_Z_SHIFT	((VISIBLE_YTILES/2)*128)

/* The different tracking states */
enum
{
	CAM_INACTIVE,
	CAM_REQUEST,
	CAM_TRACKING,
	CAM_RESET,
	CAM_TRACK_OBJECT,
	CAM_TRACK_LOCATION
};

/* Storage for old viewnagles etc */
typedef struct _warcam
{
	uint32_t	status;
	uint32_t	trackClass;
	uint32_t	lastUpdate;
	iView	oldView;

	Vector3f	acceleration;
	Vector3f	velocity;
	Vector3f	position;

	Vector3f	rotation;
	Vector3f	rotVel;
	Vector3f	rotAccel;

	uint32_t	oldDistance;
	BASE_OBJECT *target;
} WARCAM;

/* Externally referenced functions */
extern void	initWarCam			( void );
extern void	setWarCamActive		( BOOL status );
extern BOOL	getWarCamStatus		( void );
extern void camToggleStatus		( void );
extern BOOL processWarCam		( void );
extern void	camToggleInfo		( void );
extern void	requestRadarTrack	( int32_t x, int32_t y );
extern BOOL	getRadarTrackingStatus( void );
extern void	toggleRadarAllignment( void );
extern void	camInformOfRotation ( Vector3i *rotation );
extern BASE_OBJECT *camFindDroidTarget(void);
extern DROID *getTrackingDroid( void );
extern int32_t	getPresAngle( void );
extern uint32_t	getNumDroidsSelected( void );
extern void	camAllignWithTarget(BASE_OBJECT *psTarget);

#endif // __INCLUDED_SRC_WARCAM_H__