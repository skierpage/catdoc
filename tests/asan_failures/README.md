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

All 8 yangzao issues have been downloaded, tested, and integrated into the test suite:
- **2 issues fixed**: #9, #11
- **6 issues unfixed**: #6, #7, #8, #10, #12, #13

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
[vbwagner #13](https://github.com/vbwagner/catdoc/issues/13) | vbwagner-issue-13.test | vbwagner_issue_13/8 | xls2csv | ✅ PASS | (this commit) | global-buffer-overflow(write) at xlsparse.c:716 |
