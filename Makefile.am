SUBDIRS = src tests

EXTRA_DIST = charsets doc/catdoc.1.in doc/catppt.1.in doc/xls2csv.1.in
if BUILD_WORDVIEW
EXTRA_DIST += doc/wordview.1.in
endif

charsetdir = $(datadir)/catdoc

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(charsetdir)
	$(INSTALL_DATA) $(srcdir)/charsets/* $(DESTDIR)$(charsetdir)

uninstall-local:
	for file in $(srcdir)/charsets/*; do \
		rm -f $(DESTDIR)$(charsetdir)/`basename $$file`; \
	done
	-rmdir $(DESTDIR)$(charsetdir)


EXTRA_DIST += CODING.STD CREDITS NEWS README.md TODO.md
