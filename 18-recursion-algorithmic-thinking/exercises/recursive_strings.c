/*
 * Kata: Recursive Strings & Arrays
 *
 * Two exercises in one file:
 *
 * 1. reverse_string(str) — reverse a string in place, recursively
 *    Strategy: swap the first and last characters, then recurse on the
 *    substring in between.
 *
 *    "hello"  ->  swap 'h' and 'o'  ->  "oellh"
 *                 recurse on "ell"  ->  "lle"
 *                 result: "olleh"
 *
 * 2. sum_array(arr, n) — return the sum of an array, recursively
 *    Strategy: the sum of n elements is arr[0] + sum of remaining (n-1) elements.
 *    Base case: empty array (n == 0) -> sum is 0.
 *
 * Hints:
 *   - For reverse_string, you'll need a helper that takes start and end indices
 *   - strlen() from <string.h> gives you the string length
 *   - Swapping two chars: use a temporary variable
 */

#include <stdio.h>
#include <string.h>

/* Helper: recursively reverse str between indices 'start' and 'end' */
void reverse_helper(char str[], int start, int end)
{
    /* TODO: Base case — if start >= end, nothing left to swap, return */

    /* TODO: Swap str[start] and str[end] using a temp variable */

    /* TODO: Recurse on the inner substring: start+1, end-1 */
}

/* Reverse a string in place using recursion */
void reverse_string(char str[])
{
    int len = strlen(str);
    if (len <= 1) {
        return;
    }
    reverse_helper(str, 0, len - 1);
}

/* Return the sum of the first n elements of arr, recursively */
int sum_array(int arr[], int n)
{
    /* TODO: Base case — if n == 0, return 0 (empty array has sum 0) */

    /* TODO: Recursive case — return arr[n-1] + sum_array(arr, n-1)
     *       (the last element plus the sum of everything before it) */

    return 0;  /* placeholder */
}

/* ---- Test harness (do not modify) ---- */

int main(void)
{
    int passed = 0;
    int total = 0;

    printf("=== Recursive String Reverse ===\n\n");

    /* String reverse tests */
    struct {
        char input[64];
        const char *expected;
    } str_tests[] = {
        {"hello",    "olleh"},
        {"abcd",     "dcba"},
        {"a",        "a"},
        {"",         ""},
        {"racecar",  "racecar"},
        {"ab",       "ba"},
        {"12345",    "54321"},
    };
    int num_str = sizeof(str_tests) / sizeof(str_tests[0]);

    for (int i = 0; i < num_str; i++) {
        char buf[64];
        strcpy(buf, str_tests[i].input);
        reverse_string(buf);
        int ok = (strcmp(buf, str_tests[i].expected) == 0);
        printf("reverse(\"%s\") = \"%s\" %s\n",
               str_tests[i].input, buf, ok ? "PASS" : "FAIL");
        if (ok) passed++;
        total++;
    }

    printf("\n=== Recursive Sum of Array ===\n\n");

    /* Sum array tests */
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {10, 20, 30};
    int arr3[] = {-1, -2, -3, -4};
    int arr4[] = {42};
    int arr5[] = {0};  /* dummy for empty test */

    struct {
        int *arr;
        int n;
        int expected;
    } sum_tests[] = {
        {arr1, 5, 15},
        {arr2, 3, 60},
        {arr3, 4, -10},
        {arr4, 1, 42},
        {arr5, 0, 0},
    };
    int num_sum = sizeof(sum_tests) / sizeof(sum_tests[0]);

    for (int i = 0; i < num_sum; i++) {
        int result = sum_array(sum_tests[i].arr, sum_tests[i].n);
        int ok = (result == sum_tests[i].expected);
        printf("sum_array(arr, %d) = %d (expected %d) %s\n",
               sum_tests[i].n, result, sum_tests[i].expected,
               ok ? "PASS" : "FAIL");
        if (ok) passed++;
        total++;
    }

    printf("\n%d/%d tests passed.\n", passed, total);

    if (passed == total) {
        printf("All tests passed!\n");
    }

    return (passed == total) ? 0 : 1;
}
