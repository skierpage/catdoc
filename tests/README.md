Run each test file through its converter (catdoc for Microsoft Office
.doc files, pptdoc for .ppt, and xls2csv for .xls) and compare with .expected.

For example,

    catdoc basic.doc | diff - basic.doc.expected

xfail.list lists test files that are expected to fail due to known bugs:

unicode_MS.xls  Debian bug 878330 - [catdoc] invalid heap memory read and garbage output when parsing XLS files with international characters

test_LO_file.doc    Debian bug 874048, Red Hat bug 2150140 - catdoc extracts no text from LibreOffice .doc files"
