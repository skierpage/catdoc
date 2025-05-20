# Man pages to install
man_MANS = catdoc.1 xls2csv.1 catppt.1 wordview.1
if INSTALL_WORDVIEW
man_MANS += wordview
endif

# Documentation files to distribute but not install
EXTRA_DIST = $(man_MANS) *.txt *.ps

# Target for DOS documentation (referenced in top-level Makefile.am)
dosdoc:
	# Add commands here to generate DOS documentation
	# This likely involves converting man pages to text format
	# Example: groff -man -Tascii catdoc.1 > catdoc.txt
