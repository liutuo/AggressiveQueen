#!/bin/bash
gcc -o find_aq_seq find_aq_seq.c -lm
declare -A test_cases
test_cases["3 1 1 0"]="2"
test_cases["4 0 1 0"]="4"
test_cases["4 1 0 1"]="4"
results=0
failed=0
for params in "3 1 1 0" "4 0 1 0" "4 1 0 1"; do
    expected=${test_cases[$params]}
    echo "Running test with params: $params"
    output=$(./find_aq_seq $params | head -n 1)
    echo "Output: $output"
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
rm find_aq_seq
