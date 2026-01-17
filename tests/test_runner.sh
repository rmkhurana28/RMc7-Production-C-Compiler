#!/bin/bash
#rm129
# Test Runner Script for RMc7 Compiler Unit Tests
# Purpose: Automate running unit tests and compare outputs
# Usage: ./test_runner.sh [test_category]
# Categories: lexer, declarator, all

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

COMPILER="./rmc7"
TEST_DIR="tests/unit"
OUTPUT_DIR="tests/test_outputs"
EXPECTED_DIR="tests/expected_outputs"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

echo "=================================="
echo "RMc7 Compiler Unit Test Runner"
echo "=================================="
echo ""

# Counter for test results
PASSED=0
FAILED=0
TOTAL=0

# Function to run a single test
run_test() {
    local test_file=$1
    local test_name=$(basename "$test_file" .c)
    
    echo -n "Running $test_name ... "
    TOTAL=$((TOTAL + 1))
    
    # Run compiler on test file
    if $COMPILER "$test_file" > "$OUTPUT_DIR/${test_name}.out" 2>&1; then
        # Check if expected output exists
        if [ -f "$EXPECTED_DIR/${test_name}.expected" ]; then
            # Compare output with expected
            if diff -q "$OUTPUT_DIR/${test_name}.out" "$EXPECTED_DIR/${test_name}.expected" > /dev/null 2>&1; then
                echo -e "${GREEN}PASSED${NC}"
                PASSED=$((PASSED + 1))
            else
                echo -e "${RED}FAILED${NC} (output mismatch)"
                FAILED=$((FAILED + 1))
            fi
        else
            echo -e "${YELLOW}NO EXPECTED OUTPUT${NC}"
        fi
    else
        echo -e "${RED}FAILED${NC} (compilation error)"
        FAILED=$((FAILED + 1))
    fi
}

# Determine which tests to run
CATEGORY=${1:-all}

case $CATEGORY in
    lexer)
        echo "Running LEXER tests..."
        for test in "$TEST_DIR"/test_lexer*.c; do
            [ -f "$test" ] && run_test "$test"
        done
        ;;
    declarator)
        echo "Running DECLARATOR tests..."
        for test in "$TEST_DIR"/test_declarator*.c; do
            [ -f "$test" ] && run_test "$test"
        done
        ;;
    all)
        echo "Running ALL tests..."
        for test in "$TEST_DIR"/*.c; do
            [ -f "$test" ] && run_test "$test"
        done
        ;;
    *)
        echo "Unknown category: $CATEGORY"
        echo "Usage: ./test_runner.sh [lexer|declarator|all]"
        exit 1
        ;;
esac

# Print summary
echo ""
echo "=================================="
echo "Test Summary"
echo "=================================="
echo -e "Total:  $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
