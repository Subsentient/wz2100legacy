SUBDIRS = music

stamp:
	touch stamp

%.wzl: $(abs_srcdir)/% stamp
	(cd $(srcdir)/$(notdir $<) && $(ZIP) -r0 $(ZIP_UPDATE) $(abs_builddir)/$@ $(filter-out stamp,$(filter-out $<,$(^:$</%=%))) -x '*svn*' -x '*Makefile*' -x '*.svg' || [ $$? -eq 12 ] && true) # zip returns 12 on "nothing to do"
	$(ZIP) -T $@
	rm -f stamp

BASELIST = \
	palette.txt \
	anims \
	audio \
	components \
	effects \
	features \
	gamedesc.lev \
	images \
	messages \
	misc \
	script \
	sequenceaudio \
	sequences \
	shaders\
	stats \
	structs \
	texpages \
	tileset \
	wrf

PATCHLIST = \
	addon.lev \
	anims \
	challenges \
	components \
	effects \
	messages \
	multiplay \
	stats \
	structs \
	wrf

SITREPLIST = \
	audio \
	multiplayer \
	wrf

BASEARCHIVE = base.wzl
PATCHARCHIVE = mp.wzl
SITREPARCHIVE = EnhancedSitRep.wzl

CLEANFILES = $(BASEARCHIVE) $(PATCHARCHIVE) $(SITREPARCHIVE)

$(BASEARCHIVE): $(BASELIST:%=$(abs_srcdir)/$(basename $(BASEARCHIVE))/%)
$(PATCHARCHIVE): $(PATCHLIST:%=$(abs_srcdir)/$(basename $(PATCHARCHIVE))/%)
$(SITREPARCHIVE): $(PATCHLIST:%=$(abs_srcdir)/$(basename $(SITREPARCHIVE))/%)

nodist_pkgdata_DATA =  $(BASEARCHIVE) $(PATCHARCHIVE) $(SITREPARCHIVE)

dist-hook: $(BASEARCHIVE) $(PATCHARCHIVE) $(SITREPARCHIVE)
	$(UNZIP) -u $(BASEARCHIVE) -d $(DESTDIR)$(distdir)/$(basename $(BASEARCHIVE))
	$(UNZIP) -u $(PATCHARCHIVE) -d $(DESTDIR)$(distdir)/$(basename $(PATCHARCHIVE))
	$(UNZIP) -u $(SITREPARCHIVE) -d $(DESTDIR)$(distdir)/$(basename $(SITREPARCHIVE))
