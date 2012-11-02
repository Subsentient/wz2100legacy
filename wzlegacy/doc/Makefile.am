if A2X_AVAILABLE

nobase_dist_doc_DATA = \
	Readme.en

EXTRA_DIST = \
	wz2100legacy.7.asciidoc

DISTCLEANFILES = \
	wz2100legacy.7 \
	wz21100legacy.7.xml

dist_man7_MANS = \
	wz2100legacy.7

wz2100legacy.7: wz2100legacy.7.asciidoc
	$(A2X) -f manpage -D $(srcdir) $(srcdir)/wz2100legacy.7.asciidoc

manpage: wz2100legacy.7

dist-hook: manpage

endif

if INSTALLER

all: manpage

endif
