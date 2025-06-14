# catdoc version 0.96

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

The KDE project's ["baloo"](https://community.kde.org/Baloo) file indexing and
search framework uses these programs (via the KFileMetadata library) to index
the text of old MS-Office files.

`catdoc` features runtime configuration, proper charset handling,
user-definable output formats and support for Word97 files, which contain
UNICODE internally.

## Version 0.96

This release of the catdoc programs incorporates patches from around the 'net,
many from Debian. See [NEWS](NEWS).

## Alternatives

Since 0.93.0 catdoc parses OLE structure and extracts the WordDocument
stream, but doesn't parse internal structure of it.

This rough approach inevitable results in some garbage in output file,
especially near the end of file and if file contains embedded OLE objects,
such as pictures or equations.

So, if you are looking for a purely automatic way to convert Word to LaTeX,
you can better investigate word2x, wvware or LAOLA. The best programs to view
and edit these Word, PowerPoint, and Excel file formats are those in the
[LibreOffice](https://www.libreoffice.org/) office suite.

See [INSTALL](INSTALL) for information about compiling and installing the
catdoc programs on Linux and Mac OS. See [INSTALL.dos](INSTALL.dos) for
compiling catdoc for DOS.

## Upcoming changes and vulnerabilities

Serious vulnerabilities were discovered in the catdoc programs, see
[CVE-2024-54028](https://nvd.nist.gov/vuln/detail/CVE-2024-54028),
[CVE-2024-52035](https://nvd.nist.gov/vuln/detail/CVE-2024-52035),
and
[CVE-2024-48877](https://nvd.nist.gov/vuln/detail/CVE-2024-48877).
The Debian catdoc package release 1:0.95-6 incorporates patches for these. The
Talos_CVEs branch in this repository incorporates those patches (and its
history describes them in more detail) and they will be in the next version of
catdoc.

_Note_ that the next version of catdoc will very likely drop 16-bit DOS support.

The catdoc programs are unsafe C code that parse old files. Unexpected or
garbled file content will cause them to crash and running them on a
specially-crafted file may allow an attacker to interfere with the operation
of your computer. There are other unpatched known vulnerabilities in the
programs:
[CVE-2018-20451](https://nvd.nist.gov/vuln/detail/CVE-2018-20451),
[CVE-2018-20453](https://nvd.nist.gov/vuln/detail/CVE-2018-20453),
[CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633),
and
[CVE-2023-31979](https://nvd.nist.gov/vuln/detail/CVE-2023-31979).

## Documentation, bugs, more information

Catdoc is distributed under GNU Public License version 2 or above, see
[COPYING](COPYING).

The catdoc programs are documented in their UNIX-style manual pages. For
those who don't have man command (such as MS-DOS users), plain text and
PostScript versions of the man pages are in the doc directory.

Your bug reports and suggestions are welcome, as are code contributions.

There is also major work to do, see [TODO](TODO).

See the [CREDITS](CREDITS) file and git log for contributors. Special thanks
to Victor Wagner <vitus@45.free.net> for working on this project and managing
releases for over a decade.
