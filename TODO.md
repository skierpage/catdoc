# TODO

## catdoc issues

### Make the programs sandbox themselves with Landlock
- [ ] The catdoc programs parse untrusted files in unsafe C, yet need very little access (see
  "Sandboxing the binaries" in README.md). Running them under an external sandbox such as
  `bwrap` protects the user, but every user or calling program has to set that up. Instead the
  programs could restrict themselves, so that a hostile input file that exploits a parser bug
  gains almost nothing, on by default and with nothing for users or distros to configure.

  On Linux, use [Landlock](https://docs.kernel.org/userspace-api/landlock.html) (kernel 5.13+,
  needs no privileges). After parsing options and before reading any input, `catdoc`,
  `xls2csv` and `catppt` would call `landlock_create_ruleset()`, `landlock_add_rule()` and
  `landlock_restrict_self()` to allow only:
  - reading the input files named on the command line
  - reading the charset directories. This has to be the directories, not already-open files,
    because `rtfread.c` and `xlsparse.c` load a charset in mid-parse, once the file declares
    its codepage
  - creating the temporary file that `ole_init()` makes when the input is not seekable (or
    create it before restricting)
  - nothing else: no other file access, and with Landlock ABI 4 (kernel 6.7+) no TCP
    networking.

  The configuration files and the substitution maps are read before this point, so they
  need no rule. Notes:
  - detect `<linux/landlock.h>` in `configure.ac`, and carry on silently if the running kernel
    lacks Landlock or supports only an older ABI
  - put the code in one function shared by the three programs
  - add a test that the restricted programs still produce the expected output, and that
    reading a file outside the allowed set fails
  - on OpenBSD, `pledge()` and `unveil()` would do the same job in a few lines
  - this does not help someone who does not trust the catdoc binary itself; for that only
    an external sandbox works, hence the `bwrap` example in README.md.

### Fix compiler warnings
- [ ] Fix the compiler warnings in the build (e.g. the `-Wdiscarded-qualifiers` warning in
  `fileutil.c`).

### Create a test .doc that triggers the mini-FAT (SBD) chain loop
- [ ] `tests/ole-cycle.doc` only exercises the directory chain loop in `ole_init()`. Fabricate a
  .doc (e.g. patch a valid one with a script) whose header SBD start sector (offset 0x3c) is
  > 0 and whose FAT entry for that sector points to itself, and add a test like
  `tests/ole-cycle.test`, to cover the SBD loop bound.

### Create test .xls files for the stale `saved_reference` fixes in xlsparse.c
- [ ] `tests/xls-uaf.xls` only covers a STRING record after a BOF that frees the sheet. Fabricate
  .xls files (patch a valid one) for the other cases, run under ASan:
  - a string FORMULA, then EOF (MSEOF frees the sheet), then a STRING record;
  - a string FORMULA, then an INTEGER_CELL, BLANK or MULBLANK whose `alloc_cell()` call
    reallocs and moves the cell arrays, then a STRING record.
  These fixes are currently untested beyond reading the code.

### Fix the charset name leak in check_charset()
- [ ] `check_charset()` in `src/fileutil.c` `strdup()`s the charset name into `source_csname` or
  `dest_csname` without freeing the previous value, so e.g. `xls2csv -d mac-roman` leaks a few
  bytes. It can't simply `free()` the old value, because that may be a string literal (the
  initial values in `src/charsets.c`), may be the `charset` argument itself (e.g.
  `check_charset(&dest_csname,dest_csname)`), or may still be referenced by `locale_charset` in
  `src/confutil.c`. Once fixed, remove `leak:check_charset` from `tests/lsan.supp`.

## catppt issues

### low priority: catppt slide separator (form feed) inconsistency
- [ ] `basic.ppt` (MS PowerPoint format) outputs a form feed between each slide and after
  the last slide. `test_LO_file.ppt` (LibreOffice Impress format) only outputs a form
  feed after the last slide. Root cause: MS PPT stores text in `SlideListWithText` with
  `SlidePersistAtom` records that trigger `slide_state = START_SLIDE`, while LO PPT
  stores text in `PPDrawing`/Escher `ClientTextbox` records where there is no equivalent
  per-slide state transition. Fix would require emitting slide separators when entering
  each `Slide` container, not just when processing `SlidePersistAtom`.

### medium priority: catppt doesn't extract text from chart object

tests/file_example_PPT_1MB.ppt has a chart object on slide 2. catppt does not output the text in the Data Table is not output.

### low priority: "IRM protected" in protection_warning.ppt sample file.

The sample PowerPoint I found and used as a basic .ppt sample test file doesn't show any text in LibreOffice besides "This presentation has been IRM protected by policy." This makes it a bad test file. Maybe it's worth figuring out if there's other text in the file and if this is a special protected PowerPoint feature.

## CI

### Run the Address Sanitizer tests in GitHub CI
- [ ] Add a second GitHub CI workflow job that runs `./configure --enable-asan` and `make check`.
  The current job in `.github/workflows/c-cpp.yml` uses a plain `./configure`, so the ASan tests
  (tests/asan_failures and e.g. `tests/xls-uaf.test`) are skipped and the README's CI badge
  reports "passing" without running them.

### Fedora RPM automation
- [ ] Check that the next release triggers the Copr build by itself. For v0.97.3 Packit never
  handled the GitHub release event (no entry under "Releases Handled", cause unknown), and the
  build only started after commenting `/packit build --release v0.97.3` on the tagged commit.
  If it doesn't fire, ask the Packit team (https://github.com/packit/packit-service/issues).
- Fixed in 0.97.3: release builds in https://copr.fedorainfracloud.org/coprs/skierpage/catdoc/
  needed all of
  - `github.com/skierpage/catdoc` in the Copr project's "Packit allowed forge projects" setting;
  - the Copr project's chroots matching the `fedora-stable` target in .packit.yaml (currently
    fedora-43, 44, 45 x86_64; add a new chroot by hand when a Fedora release appears, or grant
    Packit `admin`);
  - approving Packit's `builder` permission request on the Copr permissions page.
- Pull request builds go to temporary `packit/skierpage-catdoc-N` Copr projects, not to
  skierpage/catdoc, so only releases reach the `dnf copr enable skierpage/catdoc` repository.

## Project management
This project is a fork of https://github.com/vbwagner/catdoc . Because that project went ~9 years without an update (2016–2025), there are bug reports and patches floating all over GitHub forks, CVE reports, and the bug and patch trackers of various Linux distros. I _think_ I have analyzed and addressed most of these up to early 2026, but it is exhausting having to check all of these for updates.

### Repos, bug trackers, and patch lists to consult
| Name/URL | Comment | Last checked |
| --- | --- | --- |
| [vbwagner's catdoc](https://github.com/vbwagner/catdoc) | Upstream. Was inactive 2016–2025; a few fixes appeared Nov 2025. Has open issues from yangzao and others. Also has a git mirror at http://wagner.pp.ru/git/oss/catdoc | 2026-03 |
| [petewarden's catdoc](https://github.com/petewarden/catdoc) | Early out-of-date fork with multiple bug reports; most have been incorporated | 2026-03 |
| [uvoteam/libdoc](https://github.com/uvoteam/libdoc) | Based on catdoc source; has CVE-related issues #1 (CVE-2018-20453) and #2 (CVE-2018-20451) | 2025 |
| [Debian package tracker](https://tracker.debian.org/pkg/catdoc) | Central Debian page; patches live in [Salsa git](https://salsa.debian.org/debian/catdoc) | 2026-03 |
| [Debian bug tracker](https://bugs.debian.org/cgi-bin/pkgreport.cgi?pkg=catdoc) | Individual Debian bug reports; several old ones (327905, 363541, 383576, 878330, 878334, 904291+) already applied | 2026-03 |
| [Fedora package source](https://src.fedoraproject.org/rpms/catdoc) | Fedora/RHEL RPM spec and any downstream patches | 2025 |
| [Fedora bugzilla](https://bugzilla.redhat.com/buglist.cgi?product=Fedora&component=catdoc) | Fedora-specific bug reports | never |
| [OpenSUSE OBS package](https://build.opensuse.org/package/show/openSUSE:Factory/catdoc) | Has patches (e.g. CVE-2017-11110 patch already applied) | 2025 |
| [Arch AUR](https://aur.archlinux.org/packages/catdoc) | Community package; check for downstream patches | never |
| [Victor Wagner's site](https://www.wagner.pp.ru/~vitus/software/catdoc/) | Original author's page with notes not yet in README | never |

### Approach to scanning for changes

The table above has too many URLs to check manually on any regular basis. Options:

- **GitHub repos** (vbwagner, petewarden, libdoc): Use `gh api` to check for new issues since a given date:
  ```sh
  gh api 'repos/vbwagner/catdoc/issues?state=open&since=2026-01-01' --jq '.[].html_url'
  ```
  Could wrap all three repos in a script `local/check-upstreams.sh`.

- **Distro trackers** (Debian, Fedora, openSUSE, Arch): These don't have a convenient API. Best approach is a Claude `/schedule` agent that runs `WebFetch` on each URL monthly and summarizes changes, or manually bump "Last checked" dates after a periodic review.

- **Automated option**: Add a GitHub Actions cron workflow that runs the `gh api` checks weekly and opens an issue in this repo if new upstream issues are found.

## Research: find more Office 2007 test files

- [ ] Do the Gnome LocalSearch (formerly Tracker), KFileMetadata, and LibreOffice projects have test
  documents? In particular I'm looking for other encodings.
- [ ] Is there an online Word 97-to-Office 2007 emulator or in-browser
  tool that lets you make your own?

## Research: consider alternative approaches

This is old C code.
[ALTERNATIVES](ALTERNATIVES.md) discusses alternate ways to extract text.

## Confirm some CVE fixes

v0.97 fixed nearly all outstanding CVEs and memory access errors, see (NEWS)[NEWS] for details.
But there are a few loose ends

- [ ] Contact rycbar77 and ask for their POC for [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633), "Catdoc v0.95 was discovered to contain a NULL pointer dereference via the component xls2csv at src/fileutil.c.". This may be [skierpage/catdoc issue #8](/skierpage/catdoc/issues/8), if so it's fixed by commit e91fef7.

- [ ] [CVE-2023-46345](https://nvd.nist.gov/vuln/detail/CVE-2023-46345) (references [rycbar77's gist](https://gist.github.com/rycbar77/d747b2c37b544ece30b2353a65ab41f9)), **Strftime-Nullptr-Dereference** "NULL pointer dereference via the component xls2csv at src/xlsparse.c". One of the [commits to src/slxparse.c](commits/master/src/xlsparse.c) in this fork in February 2026 may have fixed this

  - Need the POC file in order to reproduce.

## Investigate fixes committed to libdoc

- [ ] Review [commits](https://github.com/uvoteam/libdoc/commits/master/) made to the similar C source code in [libdoc](https://github.com/uvoteam/libdoc/) that is based on catdoc.

    Note that libdoc [issue #1](https://github.com/uvoteam/libdoc/issues/1) (CVE-2018-20453) and [issue #2](https://github.com/uvoteam/libdoc/issues/2) (CVE-2018-20451) seem to be fixed by catdoc commit 12ab509. This is confusing since user kasha13 claimed these were fixed in libdoc with different source code changes.

## MISC
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

