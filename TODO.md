# TODO

- [ ] Fix "Cannod read" misspelling

## CI
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
This is old crappy C code.
[ALTERNATIVES](ALTERNATIVES.md) discusses alternate ways to extract text.

## low-priority: mac-roman charset test
Add a test for https://github.com/vbwagner/catdoc/issues/14, "-d mac-roman
crashes catdoc and catppt".  Without the fix (applied in commit 8866ca937
cherry-picked from vbwagner)

    % xls2csv -d mac-roman tests/hungarian.xls 
dumps core and outputs nothing.
With the fix, it should output macintosh encoding, see
tests/hungarian.txt.mac-roman.expected.
To see the characters, use a tool that support macintosh/mac-roman encoding, or
convert it back to utf-8 with

    % src/xls2csv -d mac-roman tests/hungarian.xls | iconv -f macintosh - -t utf-8
the output is close to tests/hungarian.xls.expected except for one character which I guess is missing from mac-roman?
    7c7
    < "Születési hely és Id?",,,
    ---
    > "Születési hely és Idő",,,

Maybe implement a test harness that reuses the .expected machinery but you
can specify options like "-d mac-roman"

## Investigate additional CVEs:

- [ ] Contact rycbar77 and ask for their POC for [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633), "Catdoc v0.95 was discovered to contain a NULL pointer dereference via the component xls2csv at src/fileutil.c.". This may [skierpage/catdoc issue #8](/skierpage/catdoc/issues/8), if so it's fixed by commit e91fef7.

- [ ] [CVE-2023-46345](https://nvd.nist.gov/vuln/detail/CVE-2023-46345) (references [rycbar77's gist](https://gist.github.com/rycbar77/d747b2c37b544ece30b2353a65ab41f9)), **Strftime-Nullptr-Dereference** "NULL pointer dereference via the component xls2csv at src/xlsparse.c"

## Investigate fixes committed to libdoc

- [ ] Review [commits](https://github.com/uvoteam/libdoc/commits/master/) made to the similar C source code in [libdoc](https://github.com/uvoteam/libdoc/) that is based on catdoc.

    Note that libdoc [issue #1](https://github.com/uvoteam/libdoc/issues/1) (CVE-2018-20453) and [issue #2](https://github.com/uvoteam/libdoc/issues/2) (CVE-2018-20451) seem to be fixed by catdoc commit 12ab509. This is confusing since user kasha13 claimed these were fixed in libdoc with different source code changes.

## Investigate issues reported against petewarden fork of catdoc
- [ ] Analyze [issues](https://github.com/petewarden/catdoc/issues) , including:
  - [ ] [Null pointer dereference in fileutil.c:75](https://github.com/petewarden/catdoc/issues/8)

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

