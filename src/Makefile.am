AM_CPPFLAGS = -DCATDOC_VERSION=\"$(VERSION)\" -DCHARSETPATH=\"$(datadir)/catdoc\" \
              -DSYSTEMRC=\"$(sysconfdir)/catdocrc\" -DUSERRC=\".catdocrc\"

bin_PROGRAMS = catdoc xls2csv catppt

if BUILD_WORDVIEW
bin_SCRIPTS = wordview
endif

COMMONOBJ = charsets.c substmap.c fileutil.c confutil.c numutils.c ole.c

catdoc_SOURCES = catdoc.c reader.c writer.c analyze.c rtfread.c $(COMMONOBJ)

xls2csv_SOURCES = xls2csv.c sheet.c xlsparse.c $(COMMONOBJ)
xls2csv_LDADD = -lm

catppt_SOURCES = catppt.c pptparse.c $(COMMONOBJ)
catppt_LDADD = -lm

EXTRA_DIST = wordview.tcl catdoc.h ole.h ppt.h ppttypes.h xls.h xltypes.h

wordview: $(srcdir)/wordview.tcl
	echo "#! $(WISH)" > wordview
	echo 'set charset_lib "$(datadir)/catdoc"' >> wordview
	cat $(srcdir)/wordview.tcl >> wordview
	chmod +x wordview

CLEANFILES = wordview