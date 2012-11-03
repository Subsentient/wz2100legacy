dist_noinst_DATA = \
	wz2100legacy.desktop \
	wz2100legacy.ico \
	wz2100legacy.png \
	wz2100legacy.splash.png \
	wz2100l_header.bmp \
	wz2100l_welcome.bmp

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(icondir)
	$(MKDIR_P) $(DESTDIR)$(applicationdir)
	$(INSTALL_DATA) $(srcdir)/wz2100legacy.png $(DESTDIR)$(icondir)/wz2100legacy.png
	$(INSTALL_DATA) $(srcdir)/wz2100legacy.desktop $(DESTDIR)$(applicationdir)/wz2100legacy.desktop

uninstall-local:
	$(RM) $(DESTDIR)$(icondir)/wz2100legacy.png
	$(RM) $(DESTDIR)$(applicationdir)/wz2100legacy.desktop
