# TODO

- [x] Use full GNU autoconf to create Makefiles.
     This will make it easier to build with Address Sanitizer (`-{f,l}asan`) to reproduce some of the above CVEs.
- [ ] .github/workflows/c-cpp.yml can be simplified and may not need `make install
- [ ] Check if copr build (using.packit.yaml) of Fedora RPM works.

## mac-roman charset test
Add a test for https://github.com/vbwagner/catdoc/issues/14 "-d mac-roman crashes catdoc and catppt"
Without the fix
  % xls2csv -d mac-roman tests/hungarian.xls 
dumps core and outputs nothing.
With the fix, it outputs something very close to tests/hungarian.xls.expected, except for
    7c7
    < "Születési hely és Id?",,,
    ---
    > "Születési hely és Idő",,,

The trick is how to have a test that reuses .expected but I can specify "-d mac-roman"

## Investigate additional CVEs:

- [ ] [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633), "Catdoc v0.95 was discovered to contain a NULL pointer dereference via the component xls2csv at src/fileutil.c."
  - [ ] Contact rycbar77 and ask for their POC.

## Investigate fixes committed to libdoc

- [ ] Review [commits](https://github.com/uvoteam/libdoc/commits/master/) made to the similar C source code in [libdoc](https://github.com/uvoteam/libdoc/) that is based on catdoc.

Note that libdoc [issue #1(https://github.com/uvoteam/libdoc/issues/1) (CVE-2018-20453) and [issue #2](https://github.com/uvoteam/libdoc/issues/2) (CVE-2018-20451) seem to be fixed by catdoc commit 12ab5099c7da32. This is confusing since user kasha13 claimed these fixed in libdoc yet it seems to have different source code changes.

## Investigate @yangzao's overflow and access violation issues
- [ ] Investigate all issues @yangzao reported at https://github.com/vbwagner/catdoc/issues

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
