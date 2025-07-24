#!/bin/bash
# Test script for catdoc tools
# This runs the actual catdoc, catppt, and xls2csv tools on test files

set -e

# Directory for test outputs
TEST_OUTPUT_DIR="test-output"

# Tools to test (use built versions in ../src/)
DOC_TOOL="../src/catdoc"
PPT_TOOL="../src/catppt"
XLS_TOOL="../src/xls2csv"

# Check if tools exist
for tool in "$DOC_TOOL" "$PPT_TOOL" "$XLS_TOOL"; do
    if [ ! -x "$tool" ]; then
        echo "ERROR: Required tool not found: $tool"
        echo "Did you run 'make' first?"
        exit 1
    fi
done

# Create test output directory
mkdir -p "$TEST_OUTPUT_DIR"

# List of test files that are expected to fail
XFAIL_FILE="xfail.list"
if [ -f "$XFAIL_FILE" ]; then
    XFAIL_TESTS=$(cat "$XFAIL_FILE")
else
    XFAIL_TESTS=""
fi

# Find test files
DOC_TESTS=$(ls *.doc 2>/dev/null || true)
PPT_TESTS=$(ls *.ppt 2>/dev/null || true)
XLS_TESTS=$(ls *.xls 2>/dev/null || true)

ALL_TESTS="$DOC_TESTS $PPT_TESTS $XLS_TESTS"

if [ -z "$ALL_TESTS" ]; then
    echo "No test files found (*.doc, *.ppt, *.xls)"
    exit 0
fi

echo "Running catdoc test suite..."

# Function to run a single test
run_test() {
    local testfile="$1"
    local basename="${testfile%.*}"
    local extension="${testfile##*.}"
    local outfile="$TEST_OUTPUT_DIR/${testfile}.out"
    local resultfile="$TEST_OUTPUT_DIR/${testfile}.result"
    local expectedfile="${testfile}.expected"
    
    # Choose the right tool
    case "$extension" in
        doc) TOOL="$DOC_TOOL" ;;
        ppt) TOOL="$PPT_TOOL" ;;
        xls) TOOL="$XLS_TOOL" ;;
        *) echo "Unknown file type: $testfile"; return 1 ;;
    esac
    
    echo "Testing $testfile..."
    
    # Run the tool and capture output
    if "$TOOL" "$testfile" > "$outfile" 2> "$outfile.err"; then
        # Tool succeeded
        if [ -f "$expectedfile" ]; then
            # Compare with expected output
            if diff -u --ignore-blank-lines --ignore-trailing-space "$expectedfile" "$outfile" > "$resultfile.diff" 2>&1; then
                echo "PASS: $basename" > "$resultfile"
                rm -f "$resultfile.diff"
            else
                # Check if this is an expected failure
                if echo "$XFAIL_TESTS" | grep -q "$basename"; then
                    echo "XFAIL: $basename (expected failure)" > "$resultfile"
                else
                    echo "FAIL: $basename" > "$resultfile"
                    echo "See $resultfile.diff for details"
                fi
            fi
        else
            echo "SKIP: $basename (no expected file)" > "$resultfile"
        fi
    else
        # Tool failed
        status=$?
        if echo "$XFAIL_TESTS" | grep -q "$basename"; then
            echo "XFAIL: $basename (expected failure - exit status $status)" > "$resultfile"
        else
            echo "FAIL: $basename (exit status $status)" > "$resultfile"
            echo "See $outfile.err for details"
        fi
    fi
    
    # Display result
    cat "$resultfile"
}

# Run all tests
for testfile in $ALL_TESTS; do
    if [ -f "$testfile" ]; then
        run_test "$testfile"
    fi
done

# Generate summary
echo ""
echo "=== Test Summary ==="

if ls "$TEST_OUTPUT_DIR"/*.result >/dev/null 2>&1; then
    pass_count=$(grep -l '^PASS' "$TEST_OUTPUT_DIR"/*.result 2>/dev/null | wc -l)
    fail_count=$(grep -l '^FAIL' "$TEST_OUTPUT_DIR"/*.result 2>/dev/null | wc -l)
    xfail_count=$(grep -l '^XFAIL' "$TEST_OUTPUT_DIR"/*.result 2>/dev/null | wc -l)
    skip_count=$(grep -l '^SKIP' "$TEST_OUTPUT_DIR"/*.result 2>/dev/null | wc -l)
    
    echo "Passed: $pass_count"
    echo "Failed: $fail_count" 
    echo "Expected failures: $xfail_count"
    echo "Skipped: $skip_count"
    
    if [ "$fail_count" -gt 0 ]; then
        echo ""
        echo "=== Failed Tests ==="
        grep -l '^FAIL' "$TEST_OUTPUT_DIR"/*.result 2>/dev/null | while read resultfile; do
            basename=$(basename "$resultfile" .result)
            echo "  $basename"
        done
        exit 1
    else
        echo ""
        echo "=== All tests passed or failed as expected ==="
    fi
else
    echo "No test results found."
fi
