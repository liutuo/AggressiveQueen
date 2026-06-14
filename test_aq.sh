#!/bin/bash

# Compile the sequential version
gcc -o findAQ_seq findAQ_seq.c -lm

# Define test cases: "n k l w" -> "expected_queen_count"
declare -A test_cases
test_cases["3 1 1 0"]="2"
test_cases["4 0 1 0"]="4"
test_cases["4 1 0 1"]="4"

results=0
failed=0

for params in "3 1 1 0" "4 0 1 0" "4 1 0 1"; do
    expected=${test_cases[$params]}
    echo "Running test with params: $params"
    output=$(./findAQ_seq $params | head -n 1)
    echo "Output: $output"

    # Extract queen count from output (format: n,k:count:)
    actual=$(echo $output | cut -d':' -f2)

    if [ "$actual" == "$expected" ]; then
        echo "Test PASSED"
    else
        echo "Test FAILED (Expected $expected, got $actual)"
        failed=$((failed + 1))
    fi
    echo "----------------------------"
    results=$((results + 1))
done

if [ $failed -eq 0 ]; then
    echo "All $results tests passed!"
else
    echo "$failed out of $results tests failed."
fi
rm findAQ_seq
