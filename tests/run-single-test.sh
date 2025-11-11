#!/bin/bash
# Single test runner for autotools - runs one test file
# Usage: run-single-test.sh basic.doc.expected

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 expectedfile" >&2
    exit 1
fi

EXPECTEDFILE="$1"
TESTFILE="${EXPECTEDFILE%.expected}"

# Tools to test (use built versions in ../src/)
DOC_TOOL="../src/catdoc"
PPT_TOOL="../src/catppt"
XLS_TOOL="../src/xls2csv"

# Choose the right tool based on extension
case "${TESTFILE##*.}" in
    doc) TOOL="$DOC_TOOL" ;;
    ppt) TOOL="$PPT_TOOL" ;;
    xls) TOOL="$XLS_TOOL" ;;
    *) echo "ERROR: Unknown file type: $TESTFILE" >&2; exit 1 ;;
esac

# Check if files exist
if [ ! -f "$TESTFILE" ]; then
    echo "ERROR: Test file not found: $TESTFILE" >&2
    exit 1
fi

if [ ! -f "$EXPECTEDFILE" ]; then
    echo "ERROR: Expected output file not found: $EXPECTEDFILE" >&2
    exit 1
fi

# Check if tool exists
if [ ! -x "$TOOL" ]; then
    echo "ERROR: Required tool not found: $TOOL" >&2
    echo "Did you run 'make' first?" >&2
    exit 1
fi

# Run the tool and compare with expected output
if "$TOOL" "$TESTFILE" | diff -u --ignore-blank-lines --ignore-trailing-space "$EXPECTEDFILE" - >/dev/null 2>&1; then
    exit 0
else
    echo "FAIL: $TESTFILE (output differs from expected)" >&2
    exit 1
fi