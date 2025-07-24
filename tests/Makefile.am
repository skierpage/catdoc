# Real test framework that runs catdoc tools on test files
TESTS = run-tests.sh
check_SCRIPTS = run-tests.sh

# Include the test script and test data files in distribution
# (wildcard is a GNU make extension but simplifies adding tests).
EXTRA_DIST = run-tests.sh $(wildcard *.doc) $(wildcard *.ppt) $(wildcard *.xls) $(wildcard *.expected) xfail.list

# Clean up test output
clean-local:
	rm -rf test-output

.PHONY: check-local clean-local
