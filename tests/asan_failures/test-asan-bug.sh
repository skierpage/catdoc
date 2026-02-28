#!/bin/bash
# Generic test runner for ASAN bug test cases
# Usage: test-asan-bug.sh <issue_id> <tool> <poc_file> [description]
#
# Example: test-asan-bug.sh vbwagner_6 xls2csv vbwagner_issue_6/1 "NULL deref in ole.c"

set -e

if [ $# -lt 3 ]; then
    echo "Usage: $0 <issue_id> <tool> <poc_file> [description]" >&2
    echo "Example: $0 vbwagner_6 xls2csv vbwagner_issue_6/1 'NULL deref in ole.c'" >&2
    exit 2
fi

ISSUE_ID="$1"
TOOL="$2"
POC_FILE="$3"
DESCRIPTION="${4:-Issue $ISSUE_ID}"

# Determine paths based on where script is run from
if [ -f "../src/$TOOL" ]; then
    # Running from tests/ directory
    TOOL_PATH="../src/$TOOL"
    CHARSETPATH="../charsets"
    POC_PATH="asan_failures/$POC_FILE"
elif [ -f "../../src/$TOOL" ]; then
    # Running from tests/asan_failures/ directory
    TOOL_PATH="../../src/$TOOL"
    CHARSETPATH="../../charsets"
    POC_PATH="$POC_FILE"
elif [ -f "../../../src/$TOOL" ]; then
    # Running from tests/asan_failures/vbwagner_issue_X/ directory
    TOOL_PATH="../../../src/$TOOL"
    CHARSETPATH="../../../charsets"
    POC_PATH="$(basename $POC_FILE)"
else
    echo "ERROR: Cannot find $TOOL (tried ../src, ../../src, ../../../src)" >&2
    exit 1
fi

# Check if tool exists and is executable
if [ ! -x "$TOOL_PATH" ]; then
    echo "ERROR: $TOOL not found or not executable: $TOOL_PATH" >&2
    echo "Did you run 'make' first?" >&2
    exit 1
fi

# Check if built with Address Sanitizer
if ! ldd "$TOOL_PATH" 2>/dev/null | grep -q libasan; then
    echo "SKIP: Test requires Address Sanitizer (configure with --enable-asan)"
    exit 77
fi

# Check if POC file exists
if [ ! -f "$POC_PATH" ]; then
    echo "ERROR: POC file not found: $POC_PATH" >&2
    exit 1
fi

# Run the tool on the POC file
# If built with ASan and vulnerable, ASan will report errors to stderr
# If fixed, the tool may reject invalid input (exit non-zero) but without ASan errors
# Suppress LeakSanitizer since we're testing for specific bugs, not leaks
export CHARSETPATH
OUTPUT=$(ASAN_OPTIONS=detect_leaks=0 "$TOOL_PATH" "$POC_PATH" 2>&1) || true

# Check if ASan detected any errors (look for ASan error patterns in output)
if echo "$OUTPUT" | grep -qE "(AddressSanitizer|SEGV|heap-buffer-overflow|global-buffer-overflow|stack-buffer-overflow|use-after-free|heap-use-after-free|stack-use-after-free)"; then
    echo "FAIL: $ISSUE_ID - $DESCRIPTION - Address Sanitizer detected memory access error" >&2
    echo "$OUTPUT" >&2
    exit 1
else
    # No ASan errors - bug is fixed (tool may still exit non-zero for invalid input, which is correct)
    exit 0
fi
