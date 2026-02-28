# TODO

- [ ] Fix "Cannod read" misspelling

## Test cleanup

- [ ] Maybe fix the memory leaks reported by asan so tests don't have to set ASAN_OPTIONS=detect_leaks=0

## Other CI issues

- [x] Use full GNU autoconf to create Makefiles.
     This will make it easier to build with Address Sanitizer (`-{f,l}asan`) to reproduce some of the above CVEs.
- [x] .github/workflows/c-cpp.yml can be simplified and may not need `make install
- [ ] Check if copr build (using.packit.yaml) of Fedora RPM works.


## Research: find more Office 2007 test files

- [ ] Do the Gnome LocalSearch (formerly Tracker), KFileMetadata, and LibreOffice projects have test
  documents? In particular I'm looking for other encodings.
- [ ] Is there an online Word 97-to-Office 2007 emulator or in-browser
  tool that lets you make your own?

## Research: consider alternative approaches
This is old C code.
[ALTERNATIVES](ALTERNATIVES.md) discusses alternate ways to extract text.

## Investigate additional CVEs:

- [ ] Contact rycbar77 and ask for their POC for [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633), "Catdoc v0.95 was discovered to contain a NULL pointer dereference via the component xls2csv at src/fileutil.c.". This may [skierpage/catdoc issue #8](/skierpage/catdoc/issues/8), if so it's fixed by commit e91fef7.

- [ ] [CVE-2023-46345](https://nvd.nist.gov/vuln/detail/CVE-2023-46345) (references [rycbar77's gist](https://gist.github.com/rycbar77/d747b2c37b544ece30b2353a65ab41f9)), **Strftime-Nullptr-Dereference** "NULL pointer dereference via the component xls2csv at src/xlsparse.c". One of the [commits to src/slxparse.c](commits/master/src/xlsparse.c) in this fork in February 2026 may have fixed this

  - Need the POC file in order to reproduce.

## Investigate fixes committed to libdoc

- [ ] Review [commits](https://github.com/uvoteam/libdoc/commits/master/) made to the similar C source code in [libdoc](https://github.com/uvoteam/libdoc/) that is based on catdoc.

    Note that libdoc [issue #1](https://github.com/uvoteam/libdoc/issues/1) (CVE-2018-20453) and [issue #2](https://github.com/uvoteam/libdoc/issues/2) (CVE-2018-20451) seem to be fixed by catdoc commit 12ab509. This is confusing since user kasha13 claimed these were fixed in libdoc with different source code changes.

## Investigate issues reported against petewarden fork of catdoc
[petewarden/catdoc](https://github.com/petewarden/catdoc) is a commit of version 0.93 of catdoc,
dating from around 2010
- [✅] Analyze [issues](https://github.com/petewarden/catdoc/issues) , including:
  - [✅] [Issue 3](https://github.com/petewarden/catdoc/issues/3), "change character ก to Ď during xls2csv -d utf-8 /source.xls > desination.csv" may be fixed in this fork, see comment on issue
  - [✅] [Issue 4](https://github.com/petewarden/catdoc/issues/4), "Heap-buffer-overflow in catdoc version 0.95 (numutils.c)" is fixed as part of Address Sanitizer fixes in this fork
  - [✅] [Issue 5](https://github.com/petewarden/catdoc/issues/5) "Heap-buffer-overflow in catdoc version 0.95 (numutils.c)" is removed
  - [✅] [Issue 6](https://github.com/petewarden/catdoc/issues/6) "Global-buffer-overflow in xls2csv" is removed
  - [✅] [Issue 7](https://github.com/petewarden/catdoc/issues/7) "Buffer overflow in xls2csv (xlsparse.c:716)" is fixed by commit 44daea395; it may have a Debian bug number.
  - [✅] [Issue 7](https://github.com/petewarden/catdoc/issues/7) "Buffer overflow in xls2csv (xlsparse.c:716)" is fixed by commit 44daea395; it may have a Debian bug number.
  - [✅] [Issue 9](https://github.com/petewarden/catdoc/issues/9), "catdoc global buffer overflow -- by misuse of the option "-b"" is fixed by commit 1a09fc5
  - [✅] [Issue 10](https://github.com/petewarden/catdoc/issues/10), "global-buffer-overflow on reader.c:177:20" is fixed in this fork

## MISC
- [ ] Build, or trigger, Fedora package builds.
- [ ] Incorporate Victor Wagner's notes at https://www.wagner.pp.ru/~vitus/software/catdoc/ into README.md

# BACKLOG

Victor Wagner's original TODO:

* [ ] support dual-byte (CJK) encodings as output
* [ ] Find a way to extract rowspan information from XLS.
* [ ] Make XLS2CSV to output sheet partially when memory exhausted
* [ ] Plain-text output method for XLS2CSV and its support in wordview
* [ ] textmode (ck) wordview
* [ ] Improve RTF support
* [ ] Extract text from Top Level OLE objects ???
* [ ] Write correct TeX commands for accented latin letters and most often
* [ ] used mathematical symbols (20xx-25xx) into tex.specchars file
* [ ] Add handling of tables & footnotes
* [ ] Fastsave support

