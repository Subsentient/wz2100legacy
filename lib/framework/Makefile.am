
AM_CPPFLAGS = -DYY_NO_INPUT $(SDL_CFLAGS) $(WZ_CPPFLAGS)
AM_CFLAGS = $(WZ_CFLAGS)
AM_CXXFLAGS = $(WZ_CXXFLAGS)
AM_LFLAGS = $(FLEX_FLAGS)
AM_YFLAGS = -d

EXTRA_DIST = \
	resource_lexer.l resource_parser.y \
	strres_lexer.l strres_parser.y

noinst_LIBRARIES = libframework.a
noinst_HEADERS = \
	strres_parser.h \
	resource_parser.h \
	configfile.h \
	cursors.h \
	debug.h \
	file.h \
	fixedpoint.h \
	frame.h \
	frameint.h \
	frameresource.h \
	i18n.h \
	input.h \
	lexer_input.h \
	listmacs.h \
	math_ext.h \
	physfs_ext.h \
	resly.h \
	stdio_ext.h \
	string_ext.h \
	strres.h \
	strresly.h \
	tagfile.h \
	treap.h \
	trig.h \
	types.h \
	utf.h \
	vector.h \
	config-macosx.h \
	endian_hack.h \
	gettext.h \
	macros.h \
	SDL_framerate.h \
	windowicon.h \
	wzglobal.h

libframework_a_SOURCES = \
	resource_lexer.c \
	resource_parser.c \
	strres_lexer.c \
	strres_parser.c \
	configfile.c \
	cursors16.c \
	cursors32.c \
	cursors.c \
	debug.c \
	frame.c \
	frameresource.c \
	i18n.c \
	input.c \
	lexer_input.c \
	SDL_framerate.c \
	stdio_ext.c \
	strres.c \
	tagfile.c \
	treap.c \
	trig.c \
	utf.c

if MOTIF_AVAILABLE
libframework_a_SOURCES += \
	x-motif-messagebox.c
noinst_HEADERS += \
	x-motif-messagebox.h
endif
