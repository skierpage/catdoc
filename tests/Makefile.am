# Test framework using GNU Automake's parallel test harness
# Each test file gets its own .log file and runs in parallel with make -j

# File-based tests (each .expected file is a test)
FILE_TESTS = \
	basic.doc.expected \
	basic.ppt.expected \
	basic.xls.expected \
	hungarian.xls.expected \
	test_LO_file.doc.expected \
	test_LO_file.ppt.expected \
	unicode_MS.xls.expected

# Custom tests (scripts that don't follow the file pattern)
CUSTOM_TESTS = test-charsetpath.test \
	test-empty-charsetpath.test \
	test-memory-leaks.test \
	cve-2023-31979.test \
	cve-2018-20451.test \
	cve-2018-20453.test \
	asan_failures/vbwagner-issue-6.test \
	asan_failures/vbwagner-issue-7.test \
	asan_failures/vbwagner-issue-8.test \
	asan_failures/vbwagner-issue-9.test \
	asan_failures/vbwagner-issue-10.test \
	asan_failures/vbwagner-issue-11.test \
	asan_failures/vbwagner-issue-12.test \
	asan_failures/vbwagner-issue-13.test

# All tests
TESTS = $(FILE_TESTS) $(CUSTOM_TESTS)

# Set up test environment - CHARSETPATH so tests can find charset files
# Disable leak detection in ASAN builds - we focus on crashes/corruption, not leaks
AM_TESTS_ENVIRONMENT = \
	CHARSETPATH='$(abs_top_srcdir)/charsets'; export CHARSETPATH; \
	ASAN_OPTIONS=detect_leaks=0; export ASAN_OPTIONS;

# Test extensions - tells Automake how to handle different test types
TEST_EXTENSIONS = .expected .test

# Test driver for .expected files - uses run-single-test.sh
EXPECTED_LOG_COMPILER = $(srcdir)/run-single-test.sh

# .test files are executable scripts, no compiler needed
TEST_LOG_COMPILER =

# Expected failures (from xfail.list)
# CVE-2023-31979, CVE-2018-20451, and CVE-2018-20453 are now fixed
# vbwagner issues #6, #7, #8, #10, #12, and #13 are not yet fixed
XFAIL_TESTS = test_LO_file.ppt.expected \
	asan_failures/vbwagner-issue-6.test \
	asan_failures/vbwagner-issue-7.test \
	asan_failures/vbwagner-issue-8.test \
	asan_failures/vbwagner-issue-10.test \
	asan_failures/vbwagner-issue-12.test \
	asan_failures/vbwagner-issue-13.test

# Scripts needed for testing
check_SCRIPTS = run-single-test.sh \
	test-charsetpath.test \
	test-empty-charsetpath.test test-memory-leaks.test \
	cve-2023-31979.test cve-2018-20451.test cve-2018-20453.test \
	asan_failures/vbwagner-issue-6.test \
	asan_failures/vbwagner-issue-7.test \
	asan_failures/vbwagner-issue-8.test \
	asan_failures/vbwagner-issue-9.test \
	asan_failures/vbwagner-issue-10.test \
	asan_failures/vbwagner-issue-11.test \
	asan_failures/vbwagner-issue-12.test \
	asan_failures/vbwagner-issue-13.test

# Include test scripts and test data files in distribution
EXTRA_DIST = \
	run-single-test.sh \
	asan_failures/test-asan-bug.sh \
	asan_failures/vbwagner-issue-6.test \
	asan_failures/vbwagner-issue-7.test \
	asan_failures/vbwagner-issue-8.test \
	asan_failures/vbwagner-issue-9.test \
	asan_failures/vbwagner-issue-10.test \
	asan_failures/vbwagner-issue-11.test \
	asan_failures/vbwagner-issue-12.test \
	asan_failures/vbwagner-issue-13.test \
	test-charsetpath.test \
	test-empty-charsetpath.test \
	test-memory-leaks.test \
	cve-2023-31979.test \
	cve-2018-20451.test \
	cve-2018-20453.test \
	global-buffer-overflow \
	libdoc_reader_process_file_203.overflow \
	libdoc_numutils_getlong_22.overflow \
	test_charsets \
	asan_failures/vbwagner_issue_6/1 \
	asan_failures/vbwagner_issue_7/2 \
	asan_failures/vbwagner_issue_8/3 \
	asan_failures/vbwagner_issue_9/4 \
	asan_failures/vbwagner_issue_10/5 \
	asan_failures/vbwagner_issue_11/6 \
	asan_failures/vbwagner_issue_12/7 \
	asan_failures/vbwagner_issue_13/8 \
	$(wildcard *.doc) \
	$(wildcard *.ppt) \
	$(wildcard *.xls) \
	$(wildcard *.expected) \
	xfail.list

# Clean up test logs (Automake creates .log and .trs files)
clean-local:
	rm -f *.log *.trs test-suite.log

.PHONY: clean-local
