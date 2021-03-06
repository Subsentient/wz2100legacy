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

#ifndef __INCLUDED_LIB_SOUND_TRACK_H__
#define __INCLUDED_LIB_SOUND_TRACK_H__

#include "lib/framework/frame.h"
#include <physfs.h>

#ifndef WZ_NOSOUND
# ifdef WZ_OS_MAC
#  include <OpenAL/al.h>
# else
#  include <AL/al.h>
# endif
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

#define ATTENUATION_FACTOR	0.0003f

#define	SAMPLE_NOT_ALLOCATED	-1
#define	SAMPLE_NOT_FOUND		-3
#define	SAMPLE_COORD_INVALID	-5

#define	AUDIO_VOL_MAX			100L

/* typedefs
 */

typedef BOOL (* AUDIO_CALLBACK)  ( void *psObj );
typedef struct __audio_stream AUDIO_STREAM;

/* structs */

typedef struct AUDIO_SAMPLE
{
	int32_t                  iTrack;         // ID number identifying a specific sound; currently (r1182) mapped in audio_id.c
#ifndef WZ_NOSOUND
	ALuint                  iSample;        // OpenAL name of the sound source
#ifdef DEBUG	// only used for debugging
	ALboolean			isLooping;		// if	sample loops
	ALboolean			is3d;			// if	sample is 3d (as opposed to 2d)
	char				filename[256];	// actual filename of sample
#endif
#endif
	int32_t                  x, y, z;
	float                   fVol;           // computed volume of sample
	BOOL                    bFinishedPlaying;
	AUDIO_CALLBACK          pCallback;
	void                    *psObj;
	struct AUDIO_SAMPLE     *psPrev;
	struct AUDIO_SAMPLE     *psNext;
} AUDIO_SAMPLE;

typedef struct TRACK
{
	BOOL            bLoop;
	int32_t          iVol;
	int32_t          iAudibleRadius;
	int32_t          iTime;                  // duration in milliseconds
	uint32_t          iTimeLastFinished;      // time last finished in ms
	uint32_t          iNumPlaying;
#ifndef WZ_NOSOUND
	ALuint          iBufferName;            // OpenAL name of the buffer
#endif
	const char     *fileName;
} TRACK;

/* functions
 */

BOOL	sound_Init(void);
BOOL	sound_Shutdown(void);

TRACK 	*sound_LoadTrackFromFile(const char *fileName);
unsigned int sound_SetTrackVals(const char *fileName, BOOL loop, unsigned int volume, unsigned int audibleRadius);
void	sound_ReleaseTrack( TRACK *psTrack );

void	sound_StopTrack( AUDIO_SAMPLE *psSample );
void	sound_PauseTrack( AUDIO_SAMPLE *psSample );
void	sound_UpdateSample( AUDIO_SAMPLE *psSample );
void	sound_CheckAllUnloaded( void );
void sound_RemoveActiveSample( AUDIO_SAMPLE *psSample );
BOOL	sound_CheckTrack( int32_t iTrack );

int32_t	sound_GetTrackTime( int32_t iTrack );
int32_t	sound_GetTrackAudibleRadius( int32_t iTrack );
int32_t	sound_GetTrackVolume( int32_t iTrack );
const char 	*sound_GetTrackName( int32_t iTrack );

BOOL	sound_TrackLooped( int32_t iTrack );
void	sound_SetCallbackFunction( void *fn );

BOOL	sound_Play2DTrack( AUDIO_SAMPLE *psSample, BOOL bQueued );
BOOL	sound_Play3DTrack( AUDIO_SAMPLE *psSample );
void	sound_PlayWithCallback( AUDIO_SAMPLE *psSample, int32_t iCurTime, AUDIO_CALLBACK pDoneFunc );
void	sound_FinishedCallback( AUDIO_SAMPLE *psSample );

BOOL	sound_GetSystemActive( void );
int32_t	sound_GetTrackID( TRACK *psTrack );
int32_t	sound_GetAvailableID( void );
int32_t	sound_GetNumPlaying( int32_t iTrack );

int32_t	sound_GetGlobalVolume( void );
void	sound_SetGlobalVolume( int32_t iVol );

void	sound_SetStoppedCallback( AUDIO_CALLBACK pStopTrackCallback );

uint32_t	sound_GetTrackTimeLastFinished( int32_t iTrack );
void	sound_SetTrackTimeLastFinished( int32_t iTrack, uint32_t iTime );

extern BOOL sound_isStreamPlaying(AUDIO_STREAM *stream);
extern void sound_StopStream(AUDIO_STREAM *stream);
extern void sound_PauseStream(AUDIO_STREAM *stream);
extern void sound_ResumeStream(AUDIO_STREAM *stream);
extern AUDIO_STREAM *sound_PlayStreamWithBuf(PHYSFS_file *fileHandle, float volume, void (*onFinished)(void *), void *user_data, size_t streamBufferSize, unsigned int buffer_count);
extern float sound_GetStreamVolume(const AUDIO_STREAM *stream);
extern void sound_SetStreamVolume(AUDIO_STREAM *stream, float volume);

#if defined(__cplusplus)
}
#endif

void soundTest(void);

#endif	// __INCLUDED_LIB_SOUND_TRACK_H__
