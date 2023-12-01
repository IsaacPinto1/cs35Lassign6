#!/bin/bash

# Function to run a test case
run_test() {
    echo "Running Test: $1"
    if eval "$2" 2>/dev/null; then
        return 0
    else
        return 1
    fi
}

# Generate random sizes
SIZE=$((RANDOM % 100 + 1))
OUTPUT_SIZE=$((RANDOM % 200 + 1))

# Track failed tests
failed_tests=""

# Test Case 1: Invalid input for ./randall N
run_test "1" "./randall 2 abc > /dev/null; [ \$? -eq 1 ]" || failed_tests+="1 "

# Test Case 2: Invalid file path for -i /F
run_test "2" "./randall -i /nonexistent 2 >/dev/null; [ \$? -eq 1 ]" || failed_tests+="2 "

# Test Case 3: Invalid number of bytes for -o N
run_test "3" "./randall -o abc 2 >/dev/null; [ \$? -eq 1 ]" || failed_tests+="3 "

# Test Case 4: Invalid -i or -o inputs
run_test "4" "./randall -x -y 2 >/dev/null; [ \$? -eq 1 ]" || failed_tests+="4 "

# Test Case 5: ./randall N should output N bytes
run_test "5" "[ \$(./randall $SIZE | wc -c) -eq $SIZE ]" || failed_tests+="5 "

# Test Case 6: ./randall -o N X should have ceil(X/N) write calls
run_test "6" "[ \$(strace -f -e write ./randall -o $SIZE $OUTPUT_SIZE 2>&1 | grep -c write) -eq \$((($OUTPUT_SIZE + $SIZE - 1) / $SIZE)) ]" || failed_tests+="6 "

# Test Case 7: Test -i Options
run_test "7" "./randall -i rdrand 2 >/dev/null; [ \$? -eq 0 ]" || failed_tests+="7 "
run_test "8" "./randall -i lrand48_r 2 >/dev/null; [ \$? -eq 0 ]" || failed_tests+="8 "
run_test "9" "./randall -i /dev/urandom 2 >/dev/null; [ \$? -eq 0 ]" || failed_tests+="9 "

# Test Case 8: Test Combinations of -i and -o Options
run_test "10" "./randall -i rdrand -o 100 2 >/dev/null; [ \$? -eq 0 ]" || failed_tests+="10 "
run_test "11" "./randall -i lrand48_r -o 50 2 >/dev/null; [ \$? -eq 0 ]" || failed_tests+="11 "
run_test "12" "./randall -i /dev/urandom -o 200 2 >/dev/null; [ \$? -eq 0 ]" || failed_tests+="12 "

# Test Case 9: Test Invalid Combinations of -i and -o Options
run_test "13" "./randall -i rdrand -o invalid_option 2 >/dev/null; [ \$? -eq 1 ]" || failed_tests+="13 "
run_test "14" "./randall -i invalid_option -o 100 2 >/dev/null; [ \$? -eq 1 ]" || failed_tests+="14 "

# Check if any tests failed
if [ -z "$failed_tests" ]; then
    echo "All tests passed"
else
    echo "Tests failed: $failed_tests"
fi