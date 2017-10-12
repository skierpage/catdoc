Automatic: install following INSTALL instructions, then run `make check`.

Manual: Run each test file through its converter (catdoc for Microsoft Office
.doc files, pptdoc for .ppt, and xls2csv for .xls) and compare the output the
corresponding .expected file.

For example,

    catdoc basic.doc | diff - basic.doc.expected

## Test files include:
unicode_MS.xls  
reproduces Debian bug 878330 - [catdoc] invalid heap memory read and garbage output when parsing XLS files with international characters

test_LO_file.doc  
reproduces Debian bug 874048, Red Hat bug 2150140 - catdoc extracts no text from LibreOffice .doc files

hungarian.xls  
reproduces  Debian bug 878334 - wrong charset conversion in xls2csv

## Expected failing files
xfail.list lists test files that are expected to not succesfully be converted
due to known bugs:
