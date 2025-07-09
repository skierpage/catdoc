# TODO

## Drop DOS support.
This will simplify the build process.
- [ ] Remove DOS files
- [ ] Rename `.rpl` and `.spc` files in `charsets` to full length
- [ ] Remove special `make install` handling
- [ ] So `make check` tests don't need `make install`
- [ ] .github/workflows/c-cpp.yml can be simplified and may not need `make install

## Investigate additional CVEs:

- [ ] [CVE-2018-20451](https://nvd.nist.gov/vuln/detail/CVE-2018-20451), "The process_file function in reader.c in libdoc through 2017-10-23 has a heap-based buffer over-read"

     This is in the libdoc library based on catdoc, [libdoc issue #2](/uvoteam/libdoc/issues/2) I think fixed in [commit 83e4bd51fa4](/uvoteam/libdoc/commit/83e4bd51fa4). `catdoc` does not report a buffer overflow and has a different check for `nLen` overflow.

- [ ] [CVE-2018-20453](https://nvd.nist.gov/vuln/detail/CVE-2018-20453), "The getlong function in numutils.c in libdoc through 2017-10-23 has a heap-based buffer over-read"

     This is also in the libdoc library based on catdoc, [libdoc issue #1](/uvoteam/libdoc/issues/1). The failure seems to require reading the garbled POC file with a version of `catdoc` built with address sanitizer. I think it's fixed by the same [commit 83e4bd51fa4](/uvoteam/libdoc/commit/83e4bd51fa4)

- [ ] [CVE-2023-31979](https://nvd.nist.gov/vuln/detail/CVE-2023-31979), "Catdoc v0.95 was discovered to contain a global buffer overflow via the function process_file at /src/reader.c."

     This is another error that requires reading the garbled POC file with a version of `catdoc` built with address sanitizer.

- [ ] [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633), "Catdoc v0.95 was discovered to contain a NULL pointer dereference via the component xls2csv at src/fileutil.c."
  - [ ] Contact rycbar77 and ask for their POC.

## Recreate autotools build with autoconf
- [ ] Use full GNU autoconf to create Makefiles.

     This will make it easier to build with Address Sanitizer (`-{f,l}asan`) to reproduce some of the above CVEs.

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

