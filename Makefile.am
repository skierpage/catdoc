AUTOMAKE_OPTIONS = foreign

SUBDIRS = src doc charsets tests

EXTRA_DIST = CODING.STD CREDITS INSTALL.dos \
             acconfig.h \
             compat \
             doc/*.txt doc/*.ps

# Custom doc target that is referenced in the dist target
docdist:
	$(MAKE) -C doc dosdoc

# Use automake's dist-hook to handle any custom distribution steps
dist-hook:
	$(MAKE) docdist
	cp -p doc/*.txt doc/*.ps $(distdir)/doc/