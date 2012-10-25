# Makefile for PO directory in any package using GNU gettext.
# Copyright (C) 1995-1997, 2000-2007 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU General Public
# License but which still want to provide support for the GNU gettext
# functionality.
# Please note that the actual code of GNU gettext is covered by the GNU
# General Public License and is *not* in the public domain.
#
# Origin: gettext-0.17
GETTEXT_MACRO_VERSION = 0.17

PACKAGE = warzone2100legacy
VERSION = microwave_popping
PACKAGE_BUGREPORT = http://universe2.us/wzlegacy

SHELL = /bin/sh


srcdir = .
top_srcdir = ..


builddir = .
top_builddir = ..

prefix = /usr/local
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
datadir = ${datarootdir}
localedir = ${datarootdir}/locale
gettextsrcdir = $(datadir)/gettext/po

INSTALL = /bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

# We use $(mkdir_p).
# In automake <= 1.9.x, $(mkdir_p) is defined either as "mkdir -p --" or as
# "$(mkinstalldirs)" or as "$(install_sh) -d". For these automake versions,
# ${SHELL} /home/ben/wz2100legacy/wzlegacy/install-sh does not start with $(SHELL), so we add it.
# In automake >= 1.10, /bin/mkdir -p is derived from ${MKDIR_P}, which is defined
# either as "/path/to/mkdir -p" or ".../install-sh -c -d". For these automake
# versions, $(mkinstalldirs) and $(install_sh) are unused.
mkinstalldirs = $(SHELL) ${SHELL} /home/ben/wz2100legacy/wzlegacy/install-sh -d
install_sh = $(SHELL) ${SHELL} /home/ben/wz2100legacy/wzlegacy/install-sh
MKDIR_P = /bin/mkdir -p
mkdir_p = /bin/mkdir -p

GMSGFMT_ = /bin/msgfmt
GMSGFMT_no = /bin/msgfmt
GMSGFMT_yes = /bin/msgfmt
GMSGFMT = $(GMSGFMT_$(USE_MSGCTXT))
MSGFMT_ = /bin/msgfmt
MSGFMT_no = /bin/msgfmt
MSGFMT_yes = /bin/msgfmt
MSGFMT = $(MSGFMT_$(USE_MSGCTXT))
XGETTEXT_ = /bin/xgettext
XGETTEXT_no = /bin/xgettext
XGETTEXT_yes = /bin/xgettext
XGETTEXT = $(XGETTEXT_$(USE_MSGCTXT))
MSGMERGE = msgmerge
MSGMERGE_UPDATE = /bin/msgmerge --update
MSGINIT = msginit
MSGCONV = msgconv
MSGFILTER = msgfilter

POFILES =  ca_ES.po cs.po da.po de.po en_GB.po es.po et_EE.po fi.po fr.po fy.po ga.po hr.po hu.po it.po ko.po la.po lt.po nb.po nl.po pl.po pt.po pt_BR.po ro.po ru.po sk.po sl.po tr.po uk_UA.po zh_CN.po zh_TW.po
GMOFILES =  ca_ES.gmo cs.gmo da.gmo de.gmo en_GB.gmo es.gmo et_EE.gmo fi.gmo fr.gmo fy.gmo ga.gmo hr.gmo hu.gmo it.gmo ko.gmo la.gmo lt.gmo nb.gmo nl.gmo pl.gmo pt.gmo pt_BR.gmo ro.gmo ru.gmo sk.gmo sl.gmo tr.gmo uk_UA.gmo zh_CN.gmo zh_TW.gmo
UPDATEPOFILES =  ca_ES.po-update cs.po-update da.po-update de.po-update en_GB.po-update es.po-update et_EE.po-update fi.po-update fr.po-update fy.po-update ga.po-update hr.po-update hu.po-update it.po-update ko.po-update la.po-update lt.po-update nb.po-update nl.po-update pl.po-update pt.po-update pt_BR.po-update ro.po-update ru.po-update sk.po-update sl.po-update tr.po-update uk_UA.po-update zh_CN.po-update zh_TW.po-update
DUMMYPOFILES =  ca_ES.nop cs.nop da.nop de.nop en_GB.nop es.nop et_EE.nop fi.nop fr.nop fy.nop ga.nop hr.nop hu.nop it.nop ko.nop la.nop lt.nop nb.nop nl.nop pl.nop pt.nop pt_BR.nop ro.nop ru.nop sk.nop sl.nop tr.nop uk_UA.nop zh_CN.nop zh_TW.nop
DISTFILES.common = Makefile.in.in remove-potcdate.sin \
$(DISTFILES.common.extra1) $(DISTFILES.common.extra2) $(DISTFILES.common.extra3)
DISTFILES = $(DISTFILES.common) Makevars POTFILES.in update-po.sh \
$(POFILES) $(GMOFILES) \
$(DISTFILES.extra1) $(DISTFILES.extra2) $(DISTFILES.extra3)

POTFILES = \
     ../data/base/messages/messages.rmsg \
     ../data/base/messages/resmessages12.rmsg \
     ../data/base/messages/resmessages1.rmsg \
     ../data/base/messages/resmessages23.rmsg \
     ../data/base/messages/resmessages2.rmsg \
     ../data/base/messages/resmessages3.rmsg \
     ../data/base/messages/resmessagesall.rmsg \
     ../data/base/messages/strings/cam1strings.txt \
     ../data/base/messages/strings/cam2strings.txt \
     ../data/base/messages/strings/cam3strings.txt \
     ../data/base/messages/strings/genstrings.txt \
     ../data/base/messages/strings/names.txt \
     ../data/base/messages/strings/resstrings.txt \
     ../data/base/messages/strings/scrstrings.txt \
     ../data/base/script/fastplay/fastdemo.slo \
     ../data/base/script/fastplay/start.slo \
     ../data/base/script/rules.js \
     ../data/base/script/text/basic.slo \
     ../data/base/script/text/brief.slo \
     ../data/base/script/text/cam1-1-ai.slo \
     ../data/base/script/text/cam1-1.slo \
     ../data/base/script/text/cam1-1s.slo \
     ../data/base/script/text/cam1-2-ai.slo \
     ../data/base/script/text/cam1-2.slo \
     ../data/base/script/text/cam1-2s.slo \
     ../data/base/script/text/cam1-3.slo \
     ../data/base/script/text/cam1-3s.slo \
     ../data/base/script/text/cam1-4a.slo \
     ../data/base/script/text/cam1-4as.slo \
     ../data/base/script/text/cam1-5.slo \
     ../data/base/script/text/cam1-5s.slo \
     ../data/base/script/text/cam1-7.slo \
     ../data/base/script/text/cam1-7s.slo \
     ../data/base/script/text/cam1a-ai.slo \
     ../data/base/script/text/cam1a-c.slo \
     ../data/base/script/text/cam1a.slo \
     ../data/base/script/text/cam1b-ai.slo \
     ../data/base/script/text/cam1b.slo \
     ../data/base/script/text/cam1bx.slo \
     ../data/base/script/text/cam1ca.slo \
     ../data/base/script/text/cam1c.slo \
     ../data/base/script/text/cam1daynight.slo \
     ../data/base/script/text/cam1end.slo \
     ../data/base/script/text/cam2-1x.slo \
     ../data/base/script/text/cam2ax.slo \
     ../data/base/script/text/cam2bx.slo \
     ../data/base/script/text/cam2cx.slo \
     ../data/base/script/text/cam2daynight.slo \
     ../data/base/script/text/cam2dx.slo \
     ../data/base/script/text/cam2endx.slo \
     ../data/base/script/text/cam3-1x.slo \
     ../data/base/script/text/cam3-2x.slo \
     ../data/base/script/text/cam3-4sx.slo \
     ../data/base/script/text/cam3-4x.slo \
     ../data/base/script/text/cam3a-bx.slo \
     ../data/base/script/text/cam3a-d1x.slo \
     ../data/base/script/text/cam3a-d2x.slo \
     ../data/base/script/text/cam3ax.slo \
     ../data/base/script/text/cam3bx.slo \
     ../data/base/script/text/cam3cx.slo \
     ../data/base/script/text/cam3daynight.slo \
     ../data/base/script/text/countdown.slo \
     ../data/base/script/text/genbet.slo \
     ../data/base/script/text/genexp.slo \
     ../data/base/script/text/reinforcement.slo \
     ../data/base/script/text/resetid.slo \
     ../data/base/script/text/setdisk.slo \
     ../data/base/script/text/tech.slo \
     ../data/base/script/text/tutorial3.slo \
     ../data/base/script/text/vtols2.slo \
     ../data/base/script/text/vtols.slo \
     ../data/base/sequenceaudio/cam1/c001end.txt \
     ../data/base/sequenceaudio/cam1/c001.txa \
     ../data/base/sequenceaudio/cam1/c001.txt \
     ../data/base/sequenceaudio/cam1/cam1ccom.txt \
     ../data/base/sequenceaudio/cam1/cam1cnp.txt \
     ../data/base/sequenceaudio/cam1/cam1dnp.txt \
     ../data/base/sequenceaudio/cam1/cam1dp2.txt \
     ../data/base/sequenceaudio/cam1/cam1out2.txt \
     ../data/base/sequenceaudio/cam1/cam1out3.txt \
     ../data/base/sequenceaudio/cam1/cam1out4.txt \
     ../data/base/sequenceaudio/cam1/cam1out5.txt \
     ../data/base/sequenceaudio/cam1/map1a_c.txt \
     ../data/base/sequenceaudio/cam1/sub13np1.txt \
     ../data/base/sequenceaudio/cam1/sub13np2.txt \
     ../data/base/sequenceaudio/cam1/sub14anp.txt \
     ../data/base/sequenceaudio/cam1/sub1_4bn.txt \
     ../data/base/sequenceaudio/cam1/sub1_5pl.txt \
     ../data/base/sequenceaudio/cam1/sub17fmv.txt \
     ../data/base/sequenceaudio/cam2/c002.txa \
     ../data/base/sequenceaudio/cam2/c2diif2.txt \
     ../data/base/sequenceaudio/cam2/cam22fmv.txt \
     ../data/base/sequenceaudio/cam2/cam2_2n2.txt \
     ../data/base/sequenceaudio/cam2/cam2_2n.txt \
     ../data/base/sequenceaudio/cam2/cam22pl2.txt \
     ../data/base/sequenceaudio/cam2/cam22pl3.txt \
     ../data/base/sequenceaudio/cam2/cam26afm.txt \
     ../data/base/sequenceaudio/cam2/cam26bfm.txt \
     ../data/base/sequenceaudio/cam2/cam26pl1.txt \
     ../data/base/sequenceaudio/cam2/cam27fmv.txt \
     ../data/base/sequenceaudio/cam2/cam2acol.txt \
     ../data/base/sequenceaudio/cam2/cam2bcol.txt \
     ../data/base/sequenceaudio/cam2/cam2dico.txt \
     ../data/base/sequenceaudio/cam2/cam2diin.txt \
     ../data/base/sequenceaudio/cam2/cam2dipl.txt \
     ../data/base/sequenceaudio/cam3/c003.txa \
     ../data/base/sequenceaudio/cam3/c003.txt \
     ../data/base/sequenceaudio/cam3/c3ad2n2.txt \
     ../data/base/sequenceaudio/cam3/c3ad2pl3.txt \
     ../data/base/sequenceaudio/cam3/cam3_1bn.txt \
     ../data/base/sequenceaudio/cam3/cam32a2.txt \
     ../data/base/sequenceaudio/cam3/cam32alp.txt \
     ../data/base/sequenceaudio/cam3/cam32pl2.txt \
     ../data/base/sequenceaudio/cam3/cam34mu1.txt \
     ../data/base/sequenceaudio/cam3/cam34mu2.txt \
     ../data/base/sequenceaudio/cam3/cam3abf.txt \
     ../data/base/sequenceaudio/cam3/cam3abpl.txt \
     ../data/base/sequenceaudio/cam3/cam3ad2n.txt \
     ../data/base/sequenceaudio/cam3/cam3bg.txt \
     ../data/base/sequenceaudio/cam3/cam3_bn.txt \
     ../data/base/sequenceaudio/cam3/cam3_cn.txt \
     ../data/base/sequenceaudio/devastation.txa \
     ../data/base/sequenceaudio/devastation.txt \
     ../data/base/sequenceaudio/outro.txt \
     ../data/mp/messages/resmessages12.rmsg \
     ../data/mp/messages/resmessages1.rmsg \
     ../data/mp/messages/resmessages23.rmsg \
     ../data/mp/messages/resmessages2.rmsg \
     ../data/mp/messages/resmessages3.rmsg \
     ../data/mp/messages/resmessagesall.rmsg \
     ../data/mp/messages/strings/names.txt \
     ../data/mp/messages/strings/resstrings.txt \
     ../data/mp/multiplay/script/camtech.slo \
     ../data/mp/multiplay/script/multilim.slo \
     ../data/mp/multiplay/script/scavfact.js \
     ../data/mp/multiplay/skirmish/nexus.slo \
     ../data/mp/multiplay/skirmish/nullbot-1-35-ge.inc.js \
     ../data/mp/multiplay/skirmish/nullbot-1-35-ge.js \
     ../data/mp/multiplay/skirmish/nullbot-1-35-head.inc.js \
     ../data/mp/multiplay/skirmish/nullbot-1-35-tail.inc.js \
     ../data/mp/multiplay/skirmish/rules.js \
     ../data/mp/multiplay/skirmish/semperfi.js \
     ../data/mp/multiplay/skirmish/semperfi.slo \
     ../lib/exceptionhandler/dumpinfo.cpp \
     ../lib/exceptionhandler/exceptionhandler.cpp \
     ../lib/exceptionhandler/exchndl.cpp \
     ../lib/framework/crc.cpp \
     ../lib/framework/debug.cpp \
     ../lib/framework/frame.cpp \
     ../lib/framework/frameresource.cpp \
     ../lib/framework/geometry.cpp \
     ../lib/framework/i18n.cpp \
     ../lib/framework/lexer_input.cpp \
     ../lib/framework/stdio_ext.cpp \
     ../lib/framework/strres.cpp \
     ../lib/framework/treap.cpp \
     ../lib/framework/trig.cpp \
     ../lib/framework/utf.cpp \
     ../lib/framework/wzconfig.cpp \
     ../lib/gamelib/anim.cpp \
     ../lib/gamelib/animobj.cpp \
     ../lib/gamelib/gtime.cpp \
     ../lib/gamelib/hashtable.cpp \
     ../lib/ivis_opengl/bitimage.cpp \
     ../lib/ivis_opengl/imd.cpp \
     ../lib/ivis_opengl/imdload.cpp \
     ../lib/ivis_opengl/ivi.cpp \
     ../lib/ivis_opengl/jpeg_encoder.cpp \
     ../lib/ivis_opengl/pieblitfunc.cpp \
     ../lib/ivis_opengl/pieclip.cpp \
     ../lib/ivis_opengl/piedraw.cpp \
     ../lib/ivis_opengl/piefunc.cpp \
     ../lib/ivis_opengl/piematrix.cpp \
     ../lib/ivis_opengl/piemode.cpp \
     ../lib/ivis_opengl/piepalette.cpp \
     ../lib/ivis_opengl/piestate.cpp \
     ../lib/ivis_opengl/png_util.cpp \
     ../lib/ivis_opengl/rendmode.cpp \
     ../lib/ivis_opengl/screen.cpp \
     ../lib/ivis_opengl/tex.cpp \
     ../lib/ivis_opengl/textdraw.cpp \
     ../lib/netplay/netjoin_stub.cpp \
     ../lib/netplay/netlog.cpp \
     ../lib/netplay/netplay.cpp \
     ../lib/netplay/netqueue.cpp \
     ../lib/netplay/netsocket.cpp \
     ../lib/netplay/nettypes.cpp \
     ../lib/backend/macosx_screen_resolutions.cpp \
     ../lib/backend/backend_main.cpp \
     ../lib/backend/backend_game.cpp \
     ../lib/backend/swapinterval.cpp \
     ../lib/backend/wzapp_backend.cpp \
     ../lib/script/codeprint.cpp \
     ../lib/script/event.cpp \
     ../lib/script/eventsave.cpp \
     ../lib/script/interpreter.cpp \
     ../lib/script/script.cpp \
     ../lib/script/stack.cpp \
     ../lib/sequence/sequence.cpp \
     ../lib/sequence/timer.cpp \
     ../lib/sound/audio.cpp \
     ../lib/sound/audio_id.cpp \
     ../lib/sound/cdaudio.cpp \
     ../lib/sound/oggvorbis.cpp \
     ../lib/sound/openal_error.cpp \
     ../lib/sound/openal_track.cpp \
     ../lib/sound/playlist.cpp \
     ../lib/sound/track.cpp \
     ../lib/widget/bar.cpp \
     ../lib/widget/button.cpp \
     ../lib/widget/editbox.cpp \
     ../lib/widget/form.cpp \
     ../lib/widget/label.cpp \
     ../lib/widget/slider.cpp \
     ../lib/widget/tip.cpp \
     ../lib/widget/widget.cpp \
     ../po/custom/mac-infoplist.txt \
     ../po/custom/warzone2100.desktop.txt \
     ../src/action.cpp \
     ../src/advvis.cpp \
     ../src/ai.cpp \
     ../src/astar.cpp \
     ../src/atmos.cpp \
     ../src/aud.cpp \
     ../src/baseobject.cpp \
     ../src/bridge.cpp \
     ../src/bucket3d.cpp \
     ../src/challenge.cpp \
     ../src/cheat.cpp \
     ../src/clparse.cpp \
     ../src/cluster.cpp \
     ../src/cmddroid.cpp \
     ../src/combat.cpp \
     ../src/component.cpp \
     ../src/configuration.cpp \
     ../src/console.cpp \
     ../src/data.cpp \
     ../src/design.cpp \
     ../src/difficulty.cpp \
     ../src/display3d.cpp \
     ../src/display.cpp \
     ../src/drive.cpp \
     ../src/droid.cpp \
     ../src/e3demo.cpp \
     ../src/edit3d.cpp \
     ../src/effects.cpp \
     ../src/feature.cpp \
     ../src/fpath.cpp \
     ../src/frontend.cpp \
     ../src/function.cpp \
     ../src/game.cpp \
     ../src/gateway.cpp \
     ../src/geometry.cpp \
     ../src/group.cpp \
     ../src/hci.cpp \
     ../src/ingameop.cpp \
     ../src/init.cpp \
     ../src/intdisplay.cpp \
     ../src/intelmap.cpp \
     ../src/intimage.cpp \
     ../src/intorder.cpp \
     ../src/keybind.cpp \
     ../src/keyedit.cpp \
     ../src/keymap.cpp \
     ../src/levels.cpp \
     ../src/lighting.cpp \
     ../src/loadsave.cpp \
     ../src/loop.cpp \
     ../src/main.cpp \
     ../src/map.cpp \
     ../src/mapdisplay.cpp \
     ../src/mapgrid.cpp \
     ../src/mechanics.cpp \
     ../src/message.cpp \
     ../src/miscimd.cpp \
     ../src/mission.cpp \
     ../src/move.cpp \
     ../src/multibot.cpp \
     ../src/multigifts.cpp \
     ../src/multiint.cpp \
     ../src/multijoin.cpp \
     ../src/multilimit.cpp \
     ../src/multimenu.cpp \
     ../src/multiopt.cpp \
     ../src/multiplay.cpp \
     ../src/multistat.cpp \
     ../src/multistruct.cpp \
     ../src/multisync.cpp \
     ../src/objects.cpp \
     ../src/objmem.cpp \
     ../src/oprint.cpp \
     ../src/order.cpp \
     ../src/pointtree.cpp \
     ../src/power.cpp \
     ../src/projectile.cpp \
     ../src/qtscript.cpp \
     ../src/qtscriptfuncs.cpp \
     ../src/radar.cpp \
     ../src/random.cpp \
     ../src/raycast.cpp \
     ../src/research.cpp \
     ../src/scores.cpp \
     ../src/scriptai.cpp \
     ../src/scriptcb.cpp \
     ../src/scriptextern.cpp \
     ../src/scriptfuncs.cpp \
     ../src/scriptobj.cpp \
     ../src/scripttabs.cpp \
     ../src/scriptvals.cpp \
     ../src/selection.cpp \
     ../src/seqdisp.cpp \
     ../src/stats.cpp \
     ../src/structure.cpp \
     ../src/template.cpp \
     ../src/terrain.cpp \
     ../src/text.cpp \
     ../src/texture.cpp \
     ../src/transporter.cpp \
     ../src/version.cpp \
     ../src/visibility.cpp \
     ../src/warcam.cpp \
     ../src/warzoneconfig.cpp \
     ../src/wavecast.cpp \
     ../src/wrappers.cpp

CATALOGS =  ca_ES.gmo cs.gmo da.gmo de.gmo en_GB.gmo es.gmo et_EE.gmo fi.gmo fr.gmo fy.gmo ga.gmo hr.gmo hu.gmo it.gmo ko.gmo la.gmo lt.gmo nb.gmo nl.gmo pl.gmo pt.gmo pt_BR.gmo ro.gmo ru.gmo sk.gmo sl.gmo tr.gmo uk_UA.gmo zh_CN.gmo zh_TW.gmo

# Makevars gets inserted here. (Don't remove this line!)
# Makefile variables for PO directory in any package using GNU gettext.

# Usually the message domain is the same as the package name.
DOMAIN = $(PACKAGE)

# These two variables depend on the location of this directory.
subdir = po
#top_builddir = ..

# These options get passed to xgettext.
XGETTEXT_OPTIONS = --from-code=UTF-8 --no-wrap --width=1 --keyword=_ --keyword=N_ --keyword=P_:1c,2 --keyword=NP_:1c,2
# --nowrap and --width=1 are needed so that msges are not broken up,
#   but there is only one source reference on each line (while gettext would have more on one for greater widths)

MSGMERGE_OPTIONS = --no-wrap --width=1

# This is the copyright holder that gets inserted into the header of the
# $(DOMAIN).pot file.  Set this to the copyright holder of the surrounding
# package.  (Note that the msgstr strings, extracted from the package's
# sources, belong to the copyright holder of the package.)  Translators are
# expected to transfer the copyright for their translations to this person
# or entity, or to disclaim their copyright.  The empty string stands for
# the public domain; in this case the translators are expected to disclaim
# their copyright.
COPYRIGHT_HOLDER = Subsentient

# This is the email address or URL to which the translators shall report
# bugs in the untranslated strings:
# - Strings which are not entire sentences, see the maintainer guidelines
#   in the GNU gettext documentation, section 'Preparing Strings'.
# - Strings which use unclear terms or require additional context to be
#   understood.
# - Strings which make invalid assumptions about notation of date, time or
#   money.
# - Pluralisation problems.
# - Incorrect English spelling.
# - Incorrect formatting.
# It can be your email address, or a mailing list address where translators
# can write to without being subscribed, or the URL of a web page through
# which the translators can contact you.
MSGID_BUGS_ADDRESS = thinkingrodent@gmail.com

# This is the list of locale categories, beyond LC_MESSAGES, for which the
# message catalogs shall be used.  It is usually empty.
EXTRA_LOCALE_CATEGORIES =

.SUFFIXES:
.SUFFIXES: .po .gmo .mo .sed .sin .nop .po-create .po-update

.po.mo:
	@echo "$(MSGFMT) -c -o $@ $<"; \
	$(MSGFMT) -c -o t-$@ $< && mv t-$@ $@

.po.gmo:
	@lang=`echo $* | sed -e 's,.*/,,'`; \
	test "$(builddir)" = . && cdcmd="" || cdcmd="cd $(builddir) && "; \
	echo "cd $(srcdir) && rm -f $${lang}.gmo && $(GMSGFMT) -c --statistics -o $${lang}.gmo $${lang}.po"; \
	cd $(srcdir) && rm -f $${lang}.gmo && $(GMSGFMT) -c --statistics -o t-$${lang}.gmo $${lang}.po && mv t-$${lang}.gmo $${lang}.gmo

.sin.sed:
	sed -e '/^#/d' $< > t-$@
	mv t-$@ $@


all: check-macro-version all-yes

all-yes:
all-no:

# Ensure that the gettext macros and this Makefile.in.in are in sync.
check-macro-version:
	@test "$(GETTEXT_MACRO_VERSION)" = "0.17" \
	  || { echo "*** error: gettext infrastructure mismatch: using a Makefile.in.in from gettext version $(GETTEXT_MACRO_VERSION) but the autoconf macros are from gettext version 0.17" 1>&2; \
	       exit 1; \
	     }

# $(srcdir)/$(DOMAIN).pot is only created when needed. When xgettext finds no
# internationalized messages, no $(srcdir)/$(DOMAIN).pot is created (because
# we don't want to bother translators with empty POT files). We assume that
# LINGUAS is empty in this case, i.e. $(POFILES) and $(GMOFILES) are empty.
# In this case, stamp-po is a nop (i.e. a phony target).

# stamp-po is a timestamp denoting the last time at which the CATALOGS have
# been loosely updated. Its purpose is that when a developer or translator
# checks out the package via CVS, and the $(DOMAIN).pot file is not in CVS,
# "make" will update the $(DOMAIN).pot and the $(CATALOGS), but subsequent
# invocations of "make" will do nothing. This timestamp would not be necessary
# if updating the $(CATALOGS) would always touch them; however, the rule for
# $(POFILES) has been designed to not touch files that don't need to be
# changed.
stamp-po: $(srcdir)/$(DOMAIN).pot
	test ! -f $(srcdir)/$(DOMAIN).pot || \
	  test -z "$(GMOFILES)" || $(MAKE) $(GMOFILES)
	@test ! -f $(srcdir)/$(DOMAIN).pot || { \
	  echo "touch stamp-po" && \
	  echo timestamp > stamp-poT && \
	  mv stamp-poT stamp-po; \
	}

# Note: Target 'all' must not depend on target '$(DOMAIN).pot-update',
# otherwise packages like GCC can not be built if only parts of the source
# have been downloaded.

# This target rebuilds $(DOMAIN).pot; it is an expensive operation.
# Note that $(DOMAIN).pot is not touched if it doesn't need to be changed.
$(DOMAIN).pot-update: $(POTFILES) $(srcdir)/POTFILES.in remove-potcdate.sed
	if LC_ALL=C grep 'GNU warzone2100legacy' $(top_srcdir)/* 2>/dev/null | grep -v 'libtool:' >/dev/null; then \
	  package_gnu='GNU '; \
	else \
	  package_gnu=''; \
	fi; \
	if test -n '$(MSGID_BUGS_ADDRESS)' || test '$(PACKAGE_BUGREPORT)' = '@'PACKAGE_BUGREPORT'@'; then \
	  msgid_bugs_address='$(MSGID_BUGS_ADDRESS)'; \
	else \
	  msgid_bugs_address='$(PACKAGE_BUGREPORT)'; \
	fi; \
	case `$(XGETTEXT) --version | sed 1q | sed -e 's,^[^0-9]*,,'` in \
	  '' | 0.[0-9] | 0.[0-9].* | 0.1[0-5] | 0.1[0-5].* | 0.16 | 0.16.[0-1]*) \
	    $(XGETTEXT) --default-domain=$(DOMAIN) --directory=$(top_srcdir) \
	      --add-comments=TRANSLATORS: $(XGETTEXT_OPTIONS)  \
	      --files-from=$(srcdir)/POTFILES.in \
	      --copyright-holder='$(COPYRIGHT_HOLDER)' \
	      --msgid-bugs-address="$$msgid_bugs_address" \
	    ;; \
	  *) \
	    $(XGETTEXT) --default-domain=$(DOMAIN) --directory=$(top_srcdir) \
	      --add-comments=TRANSLATORS: $(XGETTEXT_OPTIONS)  \
	      --files-from=$(srcdir)/POTFILES.in \
	      --copyright-holder='$(COPYRIGHT_HOLDER)' \
	      --package-name="$${package_gnu}warzone2100legacy" \
	      --package-version='microwave_popping' \
	      --msgid-bugs-address="$$msgid_bugs_address" \
	    ;; \
	esac
	test ! -f $(DOMAIN).po || { \
	  if test -f $(srcdir)/$(DOMAIN).pot; then \
	    sed -f remove-potcdate.sed < $(srcdir)/$(DOMAIN).pot > $(DOMAIN).1po && \
	    sed -f remove-potcdate.sed < $(DOMAIN).po > $(DOMAIN).2po && \
	    if cmp $(DOMAIN).1po $(DOMAIN).2po >/dev/null 2>&1; then \
	      rm -f $(DOMAIN).1po $(DOMAIN).2po $(DOMAIN).po; \
	    else \
	      rm -f $(DOMAIN).1po $(DOMAIN).2po $(srcdir)/$(DOMAIN).pot && \
	      mv $(DOMAIN).po $(srcdir)/$(DOMAIN).pot; \
	    fi; \
	  else \
	    mv $(DOMAIN).po $(srcdir)/$(DOMAIN).pot; \
	  fi; \
	}

# This rule has no dependencies: we don't need to update $(DOMAIN).pot at
# every "make" invocation, only create it when it is missing.
# Only "make $(DOMAIN).pot-update" or "make dist" will force an update.
$(srcdir)/$(DOMAIN).pot:
	$(MAKE) $(DOMAIN).pot-update

# This target rebuilds a PO file if $(DOMAIN).pot has changed.
# Note that a PO file is not touched if it doesn't need to be changed.
$(POFILES): $(srcdir)/$(DOMAIN).pot
	@lang=`echo $@ | sed -e 's,.*/,,' -e 's/\.po$$//'`; \
	if test -f "$(srcdir)/$${lang}.po"; then \
	  test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	  echo "$${cdcmd}$(MSGMERGE_UPDATE) $(MSGMERGE_OPTIONS) $${lang}.po $(DOMAIN).pot"; \
	  cd $(srcdir) && $(MSGMERGE_UPDATE) $(MSGMERGE_OPTIONS) $${lang}.po $(DOMAIN).pot; \
	else \
	  $(MAKE) $${lang}.po-create; \
	fi


install: $(CATALOGS) install-exec install-data
install-exec:
install-data: install-data-yes
	if test "$(PACKAGE)" = "gettext-tools"; then \
	  $(mkdir_p) $(DESTDIR)$(gettextsrcdir); \
	  for file in $(DISTFILES.common) Makevars.template; do \
	    $(INSTALL_DATA) $(srcdir)/$$file \
			    $(DESTDIR)$(gettextsrcdir)/$$file; \
	  done; \
	  for file in Makevars; do \
	    rm -f $(DESTDIR)$(gettextsrcdir)/$$file; \
	  done; \
	else \
	  : ; \
	fi
install-data-no: all
install-data-yes: all
	$(mkdir_p) $(DESTDIR)$(datadir)
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed -e 's/\.gmo$$//'`; \
	  dir=$(localedir)/$$lang/LC_MESSAGES; \
	  $(mkdir_p) $(DESTDIR)$$dir; \
	  if test -r $$cat; then realcat=$$cat; else realcat=$(srcdir)/$$cat; fi; \
	  $(INSTALL_DATA) $$realcat $(DESTDIR)$$dir/$(DOMAIN).mo; \
	  echo "installing $$realcat as $(DESTDIR)$$dir/$(DOMAIN).mo"; \
	  for lc in '' $(EXTRA_LOCALE_CATEGORIES); do \
	    if test -n "$$lc"; then \
	      if (cd $(DESTDIR)$(localedir)/$$lang && LC_ALL=C ls -l -d $$lc 2>/dev/null) | grep ' -> ' >/dev/null; then \
	        link=`cd $(DESTDIR)$(localedir)/$$lang && LC_ALL=C ls -l -d $$lc | sed -e 's/^.* -> //'`; \
	        mv $(DESTDIR)$(localedir)/$$lang/$$lc $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	        mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	        (cd $(DESTDIR)$(localedir)/$$lang/$$lc.old && \
	         for file in *; do \
	           if test -f $$file; then \
	             ln -s ../$$link/$$file $(DESTDIR)$(localedir)/$$lang/$$lc/$$file; \
	           fi; \
	         done); \
	        rm -f $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	      else \
	        if test -d $(DESTDIR)$(localedir)/$$lang/$$lc; then \
	          :; \
	        else \
	          rm -f $(DESTDIR)$(localedir)/$$lang/$$lc; \
	          mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	        fi; \
	      fi; \
	      rm -f $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo; \
	      ln -s ../LC_MESSAGES/$(DOMAIN).mo $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo 2>/dev/null || \
	      ln $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(DOMAIN).mo $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo 2>/dev/null || \
	      cp -p $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(DOMAIN).mo $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo; \
	      echo "installing $$realcat link as $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo"; \
	    fi; \
	  done; \
	done

install-strip: install

installdirs: installdirs-exec installdirs-data
installdirs-exec:
installdirs-data: installdirs-data-yes
	if test "$(PACKAGE)" = "gettext-tools"; then \
	  $(mkdir_p) $(DESTDIR)$(gettextsrcdir); \
	else \
	  : ; \
	fi
installdirs-data-no:
installdirs-data-yes:
	$(mkdir_p) $(DESTDIR)$(datadir)
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed -e 's/\.gmo$$//'`; \
	  dir=$(localedir)/$$lang/LC_MESSAGES; \
	  $(mkdir_p) $(DESTDIR)$$dir; \
	  for lc in '' $(EXTRA_LOCALE_CATEGORIES); do \
	    if test -n "$$lc"; then \
	      if (cd $(DESTDIR)$(localedir)/$$lang && LC_ALL=C ls -l -d $$lc 2>/dev/null) | grep ' -> ' >/dev/null; then \
	        link=`cd $(DESTDIR)$(localedir)/$$lang && LC_ALL=C ls -l -d $$lc | sed -e 's/^.* -> //'`; \
	        mv $(DESTDIR)$(localedir)/$$lang/$$lc $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	        mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	        (cd $(DESTDIR)$(localedir)/$$lang/$$lc.old && \
	         for file in *; do \
	           if test -f $$file; then \
	             ln -s ../$$link/$$file $(DESTDIR)$(localedir)/$$lang/$$lc/$$file; \
	           fi; \
	         done); \
	        rm -f $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	      else \
	        if test -d $(DESTDIR)$(localedir)/$$lang/$$lc; then \
	          :; \
	        else \
	          rm -f $(DESTDIR)$(localedir)/$$lang/$$lc; \
	          mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	        fi; \
	      fi; \
	    fi; \
	  done; \
	done

# Define this as empty until I found a useful application.
installcheck:

uninstall: uninstall-exec uninstall-data
uninstall-exec:
uninstall-data: uninstall-data-yes
	if test "$(PACKAGE)" = "gettext-tools"; then \
	  for file in $(DISTFILES.common) Makevars.template; do \
	    rm -f $(DESTDIR)$(gettextsrcdir)/$$file; \
	  done; \
	else \
	  : ; \
	fi
uninstall-data-no:
uninstall-data-yes:
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed -e 's/\.gmo$$//'`; \
	  for lc in LC_MESSAGES $(EXTRA_LOCALE_CATEGORIES); do \
	    rm -f $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo; \
	  done; \
	done

check: all

info dvi ps pdf html tags TAGS ctags CTAGS ID:

mostlyclean:
	rm -f remove-potcdate.sed
	rm -f stamp-poT
	rm -f core core.* $(DOMAIN).po $(DOMAIN).1po $(DOMAIN).2po *.new.po
	rm -fr *.o

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES *.mo

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f stamp-po $(GMOFILES)

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir:
	$(MAKE) update-po
	@$(MAKE) dist2
# This is a separate target because 'update-po' must be executed before.
dist2: stamp-po $(DISTFILES)
	dists="$(DISTFILES)"; \
	if test "$(PACKAGE)" = "gettext-tools"; then \
	  dists="$$dists Makevars.template"; \
	fi; \
	if test -f $(srcdir)/$(DOMAIN).pot; then \
	  dists="$$dists $(DOMAIN).pot stamp-po"; \
	fi; \
	if test -f $(srcdir)/ChangeLog; then \
	  dists="$$dists ChangeLog"; \
	fi; \
	for i in 0 1 2 3 4 5 6 7 8 9; do \
	  if test -f $(srcdir)/ChangeLog.$$i; then \
	    dists="$$dists ChangeLog.$$i"; \
	  fi; \
	done; \
	if test -f $(srcdir)/LINGUAS; then dists="$$dists LINGUAS"; fi; \
	for file in $$dists; do \
	  if test -f $$file; then \
	    cp -p $$file $(distdir) || exit 1; \
	  else \
	    cp -p $(srcdir)/$$file $(distdir) || exit 1; \
	  fi; \
	done

POTFILES.in: $(srcdir)/update-po.sh
	(cd $(srcdir); ./update-po.sh)

update-po-custom: POTFILES.in

update-po: Makefile update-po-custom stamp-po
	$(MAKE) $(DOMAIN).pot-update
	test -z "$(UPDATEPOFILES)" || $(MAKE) $(UPDATEPOFILES)
	$(MAKE) update-gmo

update-custom:
	(cd $(srcdir); ./update-custom.py push; ./update-custom.py pull)

# General rule for creating PO files.

.nop.po-create:
	@lang=`echo $@ | sed -e 's/\.po-create$$//'`; \
	echo "File $$lang.po does not exist. If you are a translator, you can create it through 'msginit'." 1>&2; \
	exit 1

# General rule for updating PO files.

.nop.po-update:
	@lang=`echo $@ | sed -e 's/\.po-update$$//'`; \
	if test "$(PACKAGE)" = "gettext-tools"; then PATH=`pwd`/../src:$$PATH; fi; \
	tmpdir=`pwd`; \
	echo "$$lang:"; \
	test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	echo "$${cdcmd}$(MSGMERGE) $(MSGMERGE_OPTIONS) $$lang.po $(DOMAIN).pot -o $$lang.new.po"; \
	cd $(srcdir); \
	if $(MSGMERGE) $(MSGMERGE_OPTIONS) $$lang.po $(DOMAIN).pot -o $$tmpdir/$$lang.new.po; then \
	  if cmp $$lang.po $$tmpdir/$$lang.new.po >/dev/null 2>&1; then \
	    rm -f $$tmpdir/$$lang.new.po; \
	  else \
	    if mv -f $$tmpdir/$$lang.new.po $$lang.po; then \
	      :; \
	    else \
	      echo "msgmerge for $$lang.po failed: cannot move $$tmpdir/$$lang.new.po to $$lang.po" 1>&2; \
	      exit 1; \
	    fi; \
	  fi; \
	else \
	  echo "msgmerge for $$lang.po failed!" 1>&2; \
	  rm -f $$tmpdir/$$lang.new.po; \
	fi

$(DUMMYPOFILES):

update-gmo: Makefile $(GMOFILES)
	@:

Makefile: Makefile.in.in Makevars $(top_builddir)/config.status POTFILES.in LINGUAS
	cd $(top_builddir) \
	  && $(SHELL) ./config.status $(subdir)/$@.in po-directories

force:

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
