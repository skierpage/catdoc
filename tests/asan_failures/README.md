This directory contains test documents crafted to demonstrate various memory errors that Address Sanitization detects.


# List of bugs

## yangzao bugs

GitHub user @yangzao reported several issues [in vbwagner's catdoc repository](https://github.com/vbwagner/catdoc/issues)

### Test Infrastructure

We now have a generic test framework for ASAN bug tests:

1. **Generic test script**: `test-asan-bug.sh`
   - Usage: `./test-asan-bug.sh <issue_id> <tool> <poc_file> [description]`
   - Example: `./test-asan-bug.sh vbwagner_6 xls2csv vbwagner_issue_6/1 "NULL deref"`

2. **Individual test wrappers**: `vbwagner-issue-N.test`
   - Minimal wrapper files (3-4 lines) that call test-asan-bug.sh
   - Each gets its own test report in `make check`
   - Listed in `tests/Makefile.am` CUSTOM_TESTS
   - Unfixed issues are marked in XFAIL_TESTS

3. **Adding a new test**:
   - Download POC to `tests/asan_failures/source_issue_N/`
   - Create wrapper: `tests/asan_failures/source-issue-N.test`
   - Add to Makefile.am: CUSTOM_TESTS, check_SCRIPTS, EXTRA_DIST
   - If unfixed, add to XFAIL_TESTS
   - Update status table below

### Summary

All 8 issues reported by @yangzao have been downloaded, tested, integrated into the test suite,
and subsequently fixed: #6, #7, #8, #9, #10, #11, #12, and #13.

Note: Issues #6 and #12 remain in XFAIL_TESTS because the POC files are malformed
and cause the program to exit with error codes, but they no longer trigger ASAN crashes.

## Status

GitHub issue | Test File | POC Location | Tool | Current Status | Fixed by | Description |
---- | ---- | ---- | ---- | ---- | ---- | ---- |
[vbwagner #6](https://github.com/vbwagner/catdoc/issues/6) | vbwagner-issue-6.test | vbwagner_issue_6/1 | xls2csv | ✅ PASS | d74d3ac | NULL deref in calcFileBlockOffset() - AccessViolation at ole.c:450/544 |
[vbwagner #7](https://github.com/vbwagner/catdoc/issues/7) | vbwagner-issue-7.test | vbwagner_issue_7/2 | xls2csv | ✅ PASS | 1aa8a2c | AccessViolation at xlsparse.c:679 (number2string) |
[vbwagner #8](https://github.com/vbwagner/catdoc/issues/8) | vbwagner-issue-8.test | vbwagner_issue_8/3 | xls2csv | ✅ PASS | 70d2bd1 | heap-buffer-overflow(read) at xlsparse.c:493 |
[vbwagner #9](https://github.com/vbwagner/catdoc/issues/9) | vbwagner-issue-9.test | vbwagner_issue_9/4 | xls2csv | ✅ PASS | 2c156ed | NULL deref in stradd() - AccessViolation at fileutil.c:124 |
[vbwagner #10](https://github.com/vbwagner/catdoc/issues/10) | vbwagner-issue-10.test | vbwagner_issue_10/5 | xls2csv | ✅ PASS | 70d2bd1 | AccessViolation at xlsparse.c:438 |
[vbwagner #11](https://github.com/vbwagner/catdoc/issues/11)/CVE-2017-11110 | vbwagner-issue-11.test | vbwagner_issue_11/6 | xls2csv | ✅ PASS | possibly 7c6fd7b | heap-buffer-overflow(read) at numutils.c:22 |
[vbwagner #12](https://github.com/vbwagner/catdoc/issues/12) | vbwagner-issue-12.test | vbwagner_issue_12/7 | xls2csv | ✅ PASS | 4c5e43b | global-buffer-overflow(write) at xlsparse.c:608 |
[vbwagner #13](https://github.com/vbwagner/catdoc/issues/13) | vbwagner-issue-13.test | vbwagner_issue_13/8 | xls2csv | ✅ PASS | 44daea3 | global-buffer-overflow(write) at xlsparse.c:716 |

---

## Dean Pierce 2015 AFL Fuzzing Campaign

In March 2015, Dean Pierce reported multiple bugs discovered through [AFL (American Fuzzy Lop)](https://en.wikipedia.org/wiki/American_Fuzzy_Lop_(software)) fuzzing to the [oss-sec mailing list](https://seclists.org/oss-sec/2015/q1/835). The crash samples were too large for the mailing list (>200KB), so they were hosted at [catdocbugs.neocities.org](https://catdocbugs.neocities.org/).

### Background

Pierce discovered **35 analyzed crash samples** (catdoc-crashes.tar.bz2) plus 27 additional crashes (catdoc-crashes-raw.tar.bz2) affecting multiple locations in catdoc:

- **substmap.c:151** - Segmentation faults (read violations)
- **numutils.c:22** - Mixed crashes and ASAN detections
- **ole.c:108** - ASAN memory corruption indicators
- **ole.c:315** - ASAN memory corruption indicators

Pierce noted these were "read violations, so non-trivial to exploit" and unlikely to warrant CVEs given the document parser context. However, they represented real memory safety issues that could lead to DoS or information disclosure.

### Test Files

Test files are located in:
- `crashes/` - 35 files with descriptive names (bug00-substmap.doc through bug34-ole-asan.doc)
  - Corresponds to crashes_raw/ id:000000 through id:000034
  - `original-filenames.txt` contains the mapping
- `crashes_raw/` - All 62 crash files with AFL's original naming (id:000000 through id:000061)
  - id:000000-034: The 35 analyzed crashes (mapped to crashes/)
  - id:000035-061: Additional 27 crashes from catdoc-crashes-raw.tar.bz2
  - Testing confirms all 27 additional crashes also trigger CVE-2023-31979 (same buffer overflow)

### Current Status

Testing with multiple binary versions reveals the evolution of fixes:

**Without `-b` flag (default behavior):**
- All ole/numutils bugs (03-34): Rejected with "Broken OLE file. Try using -b switch"
- Defensive behavior prevents processing of malformed files
- This early rejection prevents the underlying vulnerabilities from being triggered

**With `-b` flag (bypass broken OLE check):**

All bugs 03-34 expose the same underlying vulnerability in different ways:
- ❌ **2026-02-02 binary**: ALL trigger **global-buffer-overflow** in reader.c:178 (process_file)
- ✅ **2026-02-09+ binaries**: All process successfully
- **Fixed by:** 1a09fc5 "Fix buffer overflow CVEs" (2026-02-03)
  - This fixed **CVE-2023-31979**: global-buffer-overflow in process_file
  - Also addressed CVE-2018-20451 and CVE-2018-20453 from libdoc
  - Added SAFE_BUFFER_ADD macro with proper bounds checking

Substmap bugs (00-02, 10, 12-13, 15, 19):
- ✅ Current binary: Pass
- ❌ 2026-02-02 binary: Memory leaks (and global-buffer-overflow with `-b`)
- ✅ 2026-02-09+ binaries: Pass
- **Fixed by:** e89ff38 "Fix memory leak in character set handling" (2026-02-03)

### Testing

Use `test-all-afl-bugs.sh` to systematically test all 35 bugs against multiple binary versions:

```bash
cd tests/asan_failures
./test-all-afl-bugs.sh
```

This script tests with leak detection enabled and shows which binary versions pass/fail each test.

### Summary Table

Bug # | Module | POC File | Current Status | 2026-02-02 | 2026-02-02 with `-b` | Fixed By |
----- | ------ | -------- | -------------- | ---------- | -------------------- | -------- |
00 | substmap | bug00-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
01 | substmap | bug01-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
02 | substmap | bug02-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
03 | numutils | bug03-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
04 | numutils | bug04-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
05 | numutils | bug05-numutils.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
06 | ole | bug06-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
07 | ole | bug07-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
08 | ole | bug08-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
09 | ole | bug09-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
10 | substmap | bug10-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
11 | numutils | bug11-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
12 | substmap | bug12-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
13 | substmap | bug13-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
14 | numutils | bug14-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
15 | substmap | bug15-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
16 | ole | bug16-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
17 | numutils | bug17-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
18 | ole | bug18-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
19 | substmap | bug19-substmap.doc | ✅ Pass | ❌ Leak | ❌ Overflow | e89ff38 (leak), 1a09fc5 (overflow) |
20 | numutils | bug20-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
21 | ole | bug21-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
22 | ole | bug22-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
23 | ole | bug23-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
24 | numutils | bug24-numutils.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
25 | ole | bug25-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
26 | numutils | bug26-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
27 | ole | bug27-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
28 | numutils | bug28-numutils-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
29 | ole | bug29-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
30 | ole | bug30-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
31 | ole | bug31-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
32 | ole | bug32-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
33 | ole | bug33-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |
34 | ole | bug34-ole-asan.doc | ⚠️ Rejected | ⚠️ Rejected | ❌ Overflow | 1a09fc5 (CVE-2023-31979) |

**Legend:**
- ✅ **Pass**: File processes successfully without errors or leaks
- ❌ **Leak**: Memory leak detected by LeakSanitizer
- ❌ **Overflow**: Global-buffer-overflow in reader.c:178 (process_file)
- ⚠️ **Rejected**: File rejected with "Broken OLE file" error (defensive behavior)

**Key Finding:** All AFL-discovered bugs (both the 35 analyzed crashes and 27 additional unanalyzed crashes) expose the same underlying vulnerability: CVE-2023-31979, a global-buffer-overflow in the process_file function. The early "Broken OLE file" rejection provides defense-in-depth, but using `-b` flag on vulnerable binaries triggers the overflow.

Testing script for additional crashes: `test-additional-crashes.sh`
