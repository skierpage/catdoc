# TODO

## catdoc issues

### medium priority: incomplete/inconsistent input file checking

GitHub issue #9: catdoc doesn't error if you give it a directory to convert; catppt and xls2csv print "/usr is not OLE file or Error" (with no error, exit status is 0). They should consistently fail. Could also check

Unreadable file inconsistency:
  % catdoc -b  /tmp/unreadable
prints
    catdoc: Permission denied
but
  % catppt  /tmp/unreadable
prints the better
    /tmp/unreadable: Permission denied

Likewise for /tmp/nosuchfile


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

### Fedora RPM automation
- [ ] Trigger Fedora package builds for skierpage/catdoc Copr once packit permissions are sorted.
  - `trigger: release` in .packit.yaml targets `owner: skierpage, project: catdoc` but builds
    never happen: the release shows up in the Packit dashboard under "Releases Handled" with no
    build created, and https://copr.fedorainfracloud.org/coprs/skierpage/catdoc/builds/ stays empty.
  - Packit is installed as a GitHub App (Settings > GitHub Actions > Packit-as-a-service).
  - PR builds work fine (go to temporary `packit/skierpage-catdoc-N` Copr projects).
  - Latest Packit docs focus on `dist-git` integration (submitting to Fedora proper); the Copr
    release trigger workflow may need different configuration or explicit Copr permissions granted
    to the Packit service account.

## Test cleanup

- [ ] Maybe fix the memory leaks reported by asan so tests don't have to set ASAN_OPTIONS=detect_leaks=0

  Investigation shows xls2csv has memory leaks (154 bytes: 128 from rowptr[row].cells allocation
  at sheet.c:38, 26 from cell content at xlsparse.c:307). catdoc does not leak.

  free_sheet() IS being called and executes all free() calls for rows/cells, but ASan still
  reports the leak. This may be a complex issue with realloc/ASan interaction, or allocations
  happening in a different code path. The leaks are small and occur at program exit, so this
  may be "won't fix".

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

- [ ] Contact rycbar77 and ask for their POC for [CVE-2023-41633](https://nvd.nist.gov/vuln/detail/CVE-2023-41633), "Catdoc v0.95 was discovered to contain a NULL pointer dereference via the component xls2csv at src/fileutil.c.". This may [skierpage/catdoc issue #8](/skierpage/catdoc/issues/8), if so it's fixed by commit e91fef7.

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

