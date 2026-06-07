#!/bin/bash

cd "$(dirname "$0")"

BUILD_TYPE="${BUILD_TYPE:-Debug}"
BUILD_DIR="build"
TEST_EXE="$BUILD_DIR/tests/test_main"
LOG_FILE="$BUILD_DIR/test_output.log"

if [ "$1" = "clean" ]; then
    rm -rf "$BUILD_DIR"
    shift
fi

command -v lcov >/dev/null 2>&1 || { echo "ERROR: lcov not installed"; exit 1; }
command -v gcov >/dev/null 2>&1 || { echo "ERROR: gcov not installed"; exit 1; }
command -v cmake >/dev/null 2>&1 || { echo "ERROR: cmake not installed"; exit 1; }

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DENABLE_COVERAGE=ON || { echo "ERROR: cmake configure failed"; exit 1; }
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" -j || { echo "ERROR: build failed"; exit 1; }

echo "=== Running tests ==="
"$TEST_EXE" --log_level=test_suite > "$LOG_FILE" 2>&1
TEST_EXIT=$?
cat "$LOG_FILE"

grep -q "Leaving test case" "$LOG_FILE" || { echo "ERROR: no test cases ran"; exit 1; }

PASSED=$(grep -cE "Leaving test case" "$LOG_FILE")
FAILED=$(grep -cE "test .* FAILED" "$LOG_FILE")
TOTAL=$((PASSED + FAILED))

echo "==============================="
echo "Test Summary:"
echo "  Total:  $TOTAL"
echo "  Passed: $PASSED"
echo "  Failed: $FAILED"
echo "==============================="

[ "$FAILED" -eq 0 ] || { echo "ERROR: $FAILED test(s) failed"; exit 1; }
[ "$TEST_EXIT" -eq 0 ] || { echo "ERROR: test executable exit code $TEST_EXIT"; exit 1; }

echo "=== Generating coverage report ==="
COV_DIR="$BUILD_DIR/coverage"
mkdir -p "$COV_DIR"
lcov --capture --directory "$BUILD_DIR" --output-file "$COV_DIR/coverage.info" \
    --rc lcov_branch_coverage=1 \
    --rc geninfo_unexecuted_blocks=1 \
    --ignore-errors negative,gcov,unused,empty \
    --ignore-errors mismatch \
    || { echo "ERROR: lcov capture failed"; exit 1; }
lcov --remove "$COV_DIR/coverage.info" '/usr/*' '*/_deps/*' '*/tests/*' \
    --output-file "$COV_DIR/coverage.info" \
    --ignore-errors unused \
    || { echo "ERROR: lcov remove failed"; exit 1; }
[ -f "$COV_DIR/coverage.info" ] || { echo "ERROR: coverage.info not generated"; exit 1; }
lcov --list "$COV_DIR/coverage.info" || { echo "ERROR: lcov list failed"; exit 1; }

command -v genhtml >/dev/null 2>&1 || { echo "ERROR: genhtml not installed"; exit 1; }
genhtml "$COV_DIR/coverage.info" --output-directory "$COV_DIR/html" || { echo "ERROR: genhtml failed"; exit 1; }
echo "HTML report: $COV_DIR/html/index.html"

exit 0