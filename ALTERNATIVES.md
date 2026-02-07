The best programs to view and edit the Word, PowerPoint, and Excel file
formats used in Microsoft Office 97-2004 are those in the
[LibreOffice](https://www.libreoffice.org/) office suite.

## Alternative text extraction and indexing tools

There are various relevant textual elements of an Office 97-2003 file
that a "contents printing" utility might output:
- latest contents
- metadata such as Title, Subject, Keywords, etc.
- comments
- revision history

catdoc, catppt, and xls2csv focus on the first one.

Maybe other programs can handle some or all of these better

### LibreOffice
This was suggested for KDE KFileMetadata indexing in [KDE bug 438455](https://bugs.kde.org/show_bug.cgi?id=438455#c34]).

#### LibreOffice --convert-to txt option
```
  $ flatpak run org.libreoffice.LibreOffice --invisible --convert-to txt --outdir $HOME /path/to/basic.doc
```
This works for a simple Word .doc file. Attempting with a PowerPoint .ppt file fails with
```
Error: no export filter for /path/to/myfile.txt found, aborting.
```

#### LibreOffice --cat option
LibreOffice obviously has robust support for importing MS-office 97-2003 files.
A human will probably get the best results by opening a file in LibreOffice, then select all and copy, then paste as text.

You can automate this from the command line with

```
  $ flatpak run org.libreoffice.LibreOffice --cat /path/to/myfile.doc
```

**--cat** only works with text file (including .doc files), not spreadsheets and slides ([LibreOffice bug 70625 comment 15](https://bugs.documentfoundation.org/show_bug.cgi?id=70625#c15]).

This doesn't work well if other instances of LibreOffice are running, you get various Gdk-CRITICAL errors about screens and displays.

#### LibreOffice mso-dumper ppt-dump utility
The LibreOffice mso-dumper repository has a [ppt-dump](https://github.com/LibreOffice/mso-dumper/blob/master/man/ppt-dump.py.1) script that can dump the text of a PowerPoint 97-2007 file:
``` sh
ppt\-dump.py \-\-no\-struct\-output \-\-dump\-text myfile.ppt
```

### Linux `file` utility

This will dump out several of the metadata fields, e.g.

| Attribute | Value |
| --- | --- |
| basic.doc | Composite Document File V2 Document |
| | Little Endian |
| Os | Windows |
| Version 6.2 |
| Code page | 1252 |
| Author | John Doe |
| Template | Normal.dotm |
| Last Saved By | John Doe |
| Revision Number | 2 |
| Name of Creating Application | Microsoft Office Word |
| Create Time/Date | Fri Oct 26 13:16:00 2018 |
| Last Saved Time/Date | Fri Oct 26 13:16:00 2018 |
| Number of Pages | 1 |
| Number of Words | 242 |
| Number of Characters | 1380 |
| Security | 0 |

### Gnome tracker-miner (renamed localsearch) 

    % tracker3-extract /path/to/myfile.doc

produces json or RDF output. It parses some of the Office document properties, but not all, e.g.:
``` sgml
<file:///path/to/xls-spreadsheet-1.xls> rdf:type nfo:PaginatedTextDocument, n>
  nie:subject "sums" ;
  nie:keyword "sums", "math", "genius!" ;
  nie:title "spreadsheet" ;
  nie:contentCreated "2024-04-23T11:54:07Z" ;
  nie:license "http://example.com" ;
  nie:comment "wheee" ;
  nie:plainTextContent "Foo Bar The quick brown fox yadda yadda Lorem ipsum dolor sit amet, co
```

### Antiword

Can read .doc files, maybe even earlier versions.
https://github.com/grobian/antiword

### Word2x
Unclear if this is for Office 2007 Word or earlier Word formats.
https://word2x.sourceforge.net/

https://github.com/pld-linux/word2x has patches

### Rust possibilities
https://github.com/tafia/calamine claims to read any spreadsheet file, including .xls and .ods

[litchi](https://github.com/DevExzh/litchi) is a high-performance Rust library for parsing Microsoft Office file formats (OLE2 and OOXML) and many other formats.
It can extract text and properties.
It has an example program that prints metadata and file contents.

The [Rust cfb crate](https://github.com/mdsteele/rust-cfb) reads Microsoft OLE documents/Microsoft Compound File Binary file formats.
