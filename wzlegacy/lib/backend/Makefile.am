AM_CPPFLAGS = $(WZ_CPPFLAGS) $(QT4_CFLAGS) $(GLEW_CFLAGS)
AM_CFLAGS = $(WZ_CFLAGS)
AM_CXXFLAGS = $(WZ_CXXFLAGS) $(QT4_CFLAGS)

MOCHEADER = \
	backend_game.h \
	wzapp_backend.h
MOCEDFILES = $(MOCHEADER:%.h=%_moc.cpp)

%_moc.cpp: %.h
	$(MOC4) -o $@ $<

CLEANFILES = $(MOCEDFILES)

EXTRA_DIST = \
	swapinterval.mm \
	macosx_screen_resolutions.h macosx_screen_resolutions.cpp

noinst_LIBRARIES = libbackend.a
noinst_HEADERS = \
	$(MOCHEADER) \
	backend_game.h \
	swapinterval.h

nodist_libbackend_a_SOURCES = \
	backend_game_moc.cpp \
	wzapp_backend_moc.cpp

libbackend_a_SOURCES = \
	backend_main.cpp \
	backend_game.cpp \
	swapinterval.cpp \
	wzapp_backend.cpp
