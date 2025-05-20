# Programs to build
bin_PROGRAMS = catdoc xls2csv catppt wordview

# Conditionally add wordview if enabled
if INSTALL_WORDVIEW
bin_PROGRAMS += wordview
endif

# Source files for catdoc
catdoc_SOURCES = catdoc.c charsets.c substmap.c fileutil.c numutils.c ole.c \
                unicode.c rtf.c writer.c read_config.c analyze.c

# Source files for xls2csv
xls2csv_SOURCES = xls2csv.c charsets.c substmap.c fileutil.c numutils.c ole.c \
                 unicode.c sheet.c xls.c

# Source files for catppt
catppt_SOURCES = catppt.c charsets.c substmap.c fileutil.c numutils.c ole.c \
                unicode.c ppt.c

# Source files for wordview
wordview_SOURCES = wordview.c

# Common header files
noinst_HEADERS = catdoc.h ole.h rtf.h unicode.h xls.h

# Make sure we compile with our configuration
# Substitute variables from configure.ac
AM_CPPFLAGS = -I$(top_srcdir)/src -DSOURCE_CHARSET=\"@sourcecharset@\" -DTARGET_CHARSET=\"@targetcharset@\"
