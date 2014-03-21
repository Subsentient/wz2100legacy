

AM_CPPFLAGS = -DYY_NO_INPUT $(SDL_CFLAGS) $(WZ_CPPFLAGS)
AM_CFLAGS = $(WZ_CFLAGS)
AM_CXXFLAGS = $(WZ_CXXFLAGS)
AM_LFLAGS = $(FLEX_FLAGS)
AM_YFLAGS = -d


EXTRA_DIST = \
	audp_lexer.l audp_parser.y

noinst_LIBRARIES = libgamelib.a
noinst_HEADERS = \
	audp_parser.h \
	anim.h \
	animobj.h \
	gtime.h \
	hashtable.h \
	parser.h

libgamelib_a_SOURCES = \
	anim.c \
	animobj.c \
	audp_lexer.c \
	audp_parser.c \
	gtime.c \
	hashtable.c
