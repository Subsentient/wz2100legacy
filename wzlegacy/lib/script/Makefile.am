
AM_CPPFLAGS = -DYY_NO_INPUT $(SDL_CFLAGS) $(WZ_CPPFLAGS)
AM_CFLAGS = $(WZ_CFLAGS)
AM_CXXFLAGS = $(WZ_CXXFLAGS)
AM_LFLAGS = $(FLEX_FLAGS)
AM_YFLAGS = -d


EXTRA_DIST = \
	chat_lexer.l chat_parser.y \
	script_lexer.l script_parser.y

noinst_LIBRARIES = libscript.a
noinst_HEADERS = \
	chat_parser.h \
	script_parser.h \
	chat_processing.h \
	codeprint.h \
	event.h \
	eventsave.h \
	interpreter.h \
	parse.h \
	script.h \
	stack.h

libscript_a_SOURCES = \
	chat_lexer.c \
	chat_parser.c \
	codeprint.c \
	event.c \
	eventsave.c \
	interpreter.c \
	script.c \
	script_lexer.c \
	script_parser.c \
	stack.c
