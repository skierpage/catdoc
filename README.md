# catdoc version 0.97 in development

`catdoc` is a program which reads MS-Office Word `.doc` files and prints their
content as readable ASCII text to stdout.  It can also produce correct
escape sequences if some UNICODE characters have to be represented
specially in your typesetting system such as (La)TeX.

The catdoc package also includes
- `catppt`, which reads MS PowerPoint `.ppt` files and prints their
  content.
- `xls2csv`, which reads MS Excel `.xls` files and prints their content as
  rows of comma-separated values.
- `wordview`, which displays `catdoc` output in a window.

See [INSTALL](INSTALL) for information about compiling and installing the
catdoc programs on Linux and Mac OS.
Several Linux distributions build the catdoc package, though as of
December 2025 most build the old version 0.95.

The KDE project's ["baloo"](https://community.kde.org/Baloo) file indexing and
search framework uses these programs (via the KFileMetadata library) to index
the text of old MS-Office files.

`catdoc` features runtime configuration, proper charset handling,
user-definable output formats and support for Word97 files, which contain
UNICODE internally.

## version 0.97 in development

This in-development next release of the catdoc programs incorporates the Debian
patches for the vulnerabilities
[CVE-2024-54028](https://nvd.nist.gov/vuln/detail/CVE-2024-54028),
[CVE-2024-52035](https://nvd.nist.gov/vuln/detail/CVE-2024-52035),
and
[CVE-2024-48877](https://nvd.nist.gov/vuln/detail/CVE-2024-48877)
identified and addressed by the Cisco Talos team.
The patched source code _no longer compiles_ in Borland Turbo C.
So v0.96 is likely the last release of the catdoc programs that build and run
in 16-bit DOS. If anyone cares about DOS support, get in touch!

## File format specifications

Microsoft publishes the file format specifications:
- [Word 97-2003 (.doc) binary file format](https://learn.microsoft.com/en-us/openspecs/office_file_formats/ms-doc/ccd7b486-7881-484c-a137-51170af7cc22)
- [PowerPoint 97-2003 (.ppt) binary file format](https://learn.microsoft.com/en-us/openspecs/office_file_formats/ms-ppt/6be79dde-33c1-4c1b-8ccc-4b2301c08662)
- [Excel 97-2003 binary file format (.xls) structure](https://learn.microsoft.com/en-us/openspecs/office_file_formats/ms-xls/cd03cb5f-ca02-4934-a391-bb674cb8aa06)

## Limitations

Since 0.93.0 catdoc parses OLE structure and extracts the WordDocument
stream, but doesn't parse internal structure of it.

This rough approach inevitable results in some garbage in the output,
especially near the end of file and if the file contains embedded OLE objects
such as pictures or equations.

[ALTERNATIVES](ALTERNATIVES.md) discusses alternate ways to extract text
from Office 97-2003 files, or convert them to other formats.

## Vulnerabilities

The catdoc programs are unsafe C code that parse old files. Unexpected or
garbled file content will cause them to crash and running them on a
specially-crafted file may allow an attacker to interfere with the operation
of your computer. There are other unpatched known vulnerabilities in the
programs:
[CVE-2018-20451](https://nvd.nist.gov/vuln/detail/CVE-2018-20451),
[CVE-2018-20453](https://nvd.nist.gov/vuln/detail/CVE-2018-20453),
[CVE-2023-31979](https://nvd.nist.gov/vuln/detail/CVE-2023-31979).
and
[CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633),

## Documentation, bugs, more information

Catdoc is distributed under GNU Public License version 2 or above, see
[COPYING](COPYING).

The catdoc programs are documented in their UNIX-style manual pages. For
those who don't have man command (such as MS-DOS users), plain text and
PostScript versions of the man pages are in the doc directory.

Your bug reports and suggestions are welcome, as are code contributions;
[TODO](TODO) is an incomplete list of things to work on.  In particular, if
you have old MS-Office files from which the catdoc text extraction programs do
not produce correct output, please file an issue and attach a small test file. 

See the [CREDITS](CREDITS) file and git log for contributors. Special thanks
to Victor Wagner <vitus@45.free.net> for working on this project and managing
releases for over a decade.
