# catdoc

[![C/C++ CI](https://github.com/skierpage/catdoc/actions/workflows/c-cpp.yml/badge.svg?branch=master)](https://github.com/skierpage/catdoc/actions/workflows/c-cpp.yml)
[![Copr build status](https://copr.fedorainfracloud.org/coprs/skierpage/catdoc/package/catdoc/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/skierpage/catdoc/package/catdoc/)

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

## Installing

Several Linux distributions ship the catdoc package, though as of
December 2025 most build the old version 0.95.

You can compile and install the catdoc programs yourself on Linux and Mac OS from the source in git
or from a source tarball, see [INSTALL](INSTALL).

In addition, RPM packages of recent releases for recent Fedora versions on the
x86_64 architecture are built automatically in the
[skierpage/catdoc Copr project](https://copr.fedorainfracloud.org/coprs/skierpage/catdoc/).
These are not part of Fedora itself. To add the project as a DNF repository,
install the package, and get upgrades to future releases:
```
sudo dnf copr enable skierpage/catdoc
sudo dnf install catdoc
sudo dnf upgrade catdoc
```
(The `wordview` GUI is in a separate Fedora package, `catdoc-wordview`.)

## version 0.97.3

This fixes an endless loop on OLE files with a cyclic sector chain and a
use-after-free in `xls2csv`, both found by DicksonWu654, and gives clear
errors for directories and too-small input files.

## version 0.97.2

This extracts text from many more PowerPoint .ppt files

## version 0.97

This release of the catdoc programs addresses numerous vulnerabilities
described [below](#vulnerabilities). To do so it has updated autoconf/automake tooling to make it
easier to build with Address Sanitizer, and an automake test harness to check
for memory errors. The steps to build it from source changed slightly, see
[INSTALL](INSTALL).

[vbwagner's upstream]() came back to life after a 9 year absence with a couple
of fixes in November 2025; this fork incorporates them. 

See [NEWS](NEWS) and `git log` for changes in earlier releases.

### End of DOS support

The patched source code _no longer compiles_ in Borland Turbo C;
v0.96 is the last release of the catdoc programs that builds and runs
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
specially-crafted file may allow an attacker to interfere with the operation of
your computer. Version 0.97 fixes many memory access errors and Common
Vulnerabilities and Exposures reported against various forks and distribution
packages of catdoc over the years:
- it incorporates the Debian patches for the vulnerabilities
[CVE-2024-54028](https://nvd.nist.gov/vuln/detail/CVE-2024-54028),
[CVE-2024-52035](https://nvd.nist.gov/vuln/detail/CVE-2024-52035),
and
[CVE-2024-48877](https://nvd.nist.gov/vuln/detail/CVE-2024-48877)
identified and addressed by the Cisco Talos team
- it fixes the memory access errors [reported by yangzao against vbwagner's catdoc upstream](/vbwagner/catdoc/issues?q=is%3Aissue%20author%3Ayangzao)
- it fixes the buffer overflow [CVE-2023-31979](https://nvd.nist.gov/vuln/detail/CVE-2023-31979) reported by randomssr against petewarden's outdated copy of catdoc; this also fixes the heap buffer over-reads in [CVE-2018-20451](https://nvd.nist.gov/vuln/detail/CVE-2018-20451) and [CVE-2018-20453](https://nvd.nist.gov/vuln/detail/CVE-2018-20453) reported against libuvdoc (based on catdoc)
- it probably fixes [CVE-2023-46345](https://nvd.nist.gov/vuln/detail/CVE-2023-46345) and [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633) reported by rycbar77
- it fixes the out-of-bounds reads [CVE-2026-111126](https://www.cve.org/CVERecord?id=CVE-2026-111126) and [CVE-2026-111127](https://www.cve.org/CVERecord?id=CVE-2026-111127) in `xls2csv` reported by HaiND in 2026 (version 0.97.4 fixes some similar ones)
- together, these fixes address all the [many bugs detected by Dean Pierce in 2015](https://seclists.org/oss-sec/2015/q1/835)

Some were detected by Address Sanitizer tools,
see [tests/asan_failures/README.md](tests/asan_failures/README.md) for more details.

## Sandboxing the binaries

Since a malicious file might exploit a remaining bug (see above), it is prudent
to run these programs in a sandbox, especially on files you don't trust.
`catdoc`, `xls2csv` and `catppt` need very little access:
- they read the input file(s) named on the command line, or standard input
- they read the charset and character-substitution files in the catdoc data
  directory (usually `/usr/share/catdoc`, or the directories in `$CHARSETPATH`)
- they read the optional configuration files `/etc/catdocrc` and `~/.catdocrc`
- they write only to standard output and standard error
- they create a temporary file in `/tmp` only when the input is a pipe
- they never use the network or run other programs.

(This does not apply to `wordview`, a Tcl/Tk GUI that runs `catdoc`.)

For example, [bubblewrap](https://github.com/containers/bubblewrap) can run
a packaged `catdoc` on `untrusted.doc` so that it sees nothing but a read-only
`/usr` and that one file:

```sh
bwrap --unshare-all --die-with-parent --new-session \
      --ro-bind /usr /usr --symlink usr/lib /lib --symlink usr/lib64 /lib64 \
      --ro-bind untrusted.doc /input \
      /usr/bin/catdoc /input
```

- `--unshare-all` puts the program in new namespaces, so it has no network
  access and cannot see other processes
- `--die-with-parent` kills the program if the shell that started it exits
- `--new-session` detaches the program from your terminal so it cannot inject
  keystrokes into it
- `--ro-bind /usr /usr` and the two `--symlink` options make the programs,
  shared libraries, locale data and catdoc's data directory visible, read-only.
  This assumes a distribution where `/lib` and `/lib64` are symbolic links into
  `/usr`
- `--ro-bind untrusted.doc /input` makes the file visible, read-only, as
  `/input`. Nothing else, including your home directory and `/etc`, exists in
  the sandbox, so the configuration files are not read.

Use `/usr/bin/xls2csv` or `/usr/bin/catppt` the same way. To pipe a file into
the program instead, drop the `--ro-bind untrusted.doc /input` line and the
`/input` argument and add `--tmpfs /tmp` for the temporary file. (Before
version 0.97.4, `xls2csv` and `catppt` crash when reading standard input.)

## Documentation, bugs, more information

Catdoc is distributed under GNU Public License version 2 or above, see
[COPYING](COPYING).

The catdoc programs are documented in their UNIX-style manual pages. For
those who don't have man command (such as MS-DOS users), plain text and
PostScript versions of the man pages are in the doc directory.

Your bug reports and suggestions are welcome, as are code contributions;
[TODO.md](TODO.md) is an incomplete list of things to work on.  In particular, if
you have old MS-Office files from which the catdoc text extraction programs do
not produce correct output, please file an issue and attach a small test file. 

See the [CREDITS](CREDITS) file and git log for contributors. Special thanks
to Victor Wagner <vitus@45.free.net> for working on this project and managing
releases for over a decade.
