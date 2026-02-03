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
	cve-2023-31979.test \
	cve-2018-20451.test \
	cve-2018-20453.test

# All tests
TESTS = $(FILE_TESTS) $(CUSTOM_TESTS)

# Set up test environment - CHARSETPATH so tests can find charset files
AM_TESTS_ENVIRONMENT = \
	CHARSETPATH='$(abs_top_srcdir)/charsets'; export CHARSETPATH;

# Test extensions - tells Automake how to handle different test types
TEST_EXTENSIONS = .expected .test

# Test driver for .expected files - uses run-single-test.sh
EXPECTED_LOG_COMPILER = $(srcdir)/run-single-test.sh

# .test files are executable scripts, no compiler needed
TEST_LOG_COMPILER =

# Expected failures (from xfail.list)
# CVE-2023-31979, CVE-2018-20451, and CVE-2018-20453 are now fixed
XFAIL_TESTS = test_LO_file.ppt.expected

# Scripts needed for testing
check_SCRIPTS = run-single-test.sh test-charsetpath.test \
	test-empty-charsetpath.test \
	cve-2023-31979.test cve-2018-20451.test cve-2018-20453.test

# Include test scripts and test data files in distribution
EXTRA_DIST = \
	run-single-test.sh \
	test-charsetpath.test \
	test-empty-charsetpath.test \
	cve-2023-31979.test \
	cve-2018-20451.test \
	cve-2018-20453.test \
	global-buffer-overflow \
	libdoc_reader_process_file_203.overflow \
	libdoc_numutils_getlong_22.overflow \
	test_charsets \
	$(wildcard *.doc) \
	$(wildcard *.ppt) \
	$(wildcard *.xls) \
	$(wildcard *.expected) \
	xfail.list

# Clean up test logs (Automake creates .log and .trs files)
clean-local:
	rm -f *.log *.trs test-suite.log

.PHONY: clean-local
