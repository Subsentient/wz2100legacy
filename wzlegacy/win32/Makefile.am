.rc.o:
	$(WINDRES) -DVERSION="\"$(VERSION)\"" -o$@ $<

dist_noinst_DATA = wz2100legacy.rc

if MINGW32
nodist_noinst_DATA = \
		wz2100legacy.o
CLEANFILES = $(nodist_noinst_DATA)
endif
