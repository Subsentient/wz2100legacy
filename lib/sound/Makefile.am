AM_CPPFLAGS = $(SDL_CFLAGS) $(OGGVORBIS_CFLAGS) $(OPENAL_CFLAGS) $(WZ_CPPFLAGS)
AM_CFLAGS = $(WZ_CFLAGS)
AM_CXXFLAGS = $(WZ_CXXFLAGS)

noinst_LIBRARIES = libsound.a
noinst_HEADERS = \
	aud.h \
	audio.h \
	audio_id.h \
	cdaudio.h \
	mixer.h \
	playlist.h \
	oggvorbis.h \
	openal_error.h \
	track.h \
	tracklib.h

libsound_a_SOURCES = \
	audio.c \
	audio_id.c \
	cdaudio.c \
	oggvorbis.c \
	openal_error.c \
	openal_track.c \
	playlist.c \
	track.c
