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
 *  Audio wrapper functions
 */

#ifndef __INCLUDED_LIB_SOUND_AUD_H__
#define __INCLUDED_LIB_SOUND_AUD_H__

#include "lib/framework/vector.h"

#if defined(__cplusplus)
extern "C"
{
#endif

void	audio_GetObjectPos( void *psObj, int32_t *piX, int32_t *piY,
							int32_t *piZ );
void	audio_GetStaticPos( int32_t iWorldX, int32_t iWorldY,
							int32_t *piX, int32_t *piY, int32_t *piZ );
BOOL	audio_ObjectDead( void *psObj );
Vector3f audio_GetPlayerPos(void);
void audio_Get3DPlayerRotAboutVerticalAxis(float *angle);

#if defined(__cplusplus)
}
#endif

#endif // __INCLUDED_LIB_SOUND_AUD_H__
