/*
 * Kata: Factorial
 *
 * Implement factorial two ways:
 *   1. Recursively:  factorial_recursive(n)
 *   2. Iteratively:  factorial_iterative(n)
 *
 * Definition:
 *   0! = 1
 *   n! = n * (n-1) * (n-2) * ... * 2 * 1
 *
 * Examples:
 *   factorial(0) = 1
 *   factorial(1) = 1
 *   factorial(5) = 120
 *   factorial(10) = 3628800
 *
 * The test harness below will verify both implementations produce the
 * same results.
 *
 * Hints:
 *   - Recursive: base case is n == 0 -> return 1
 *                recursive case: return n * factorial(n - 1)
 *   - Iterative: use a for loop, multiply a running product
 */

#include <stdio.h>

/* Returns n! using recursion */
int factorial_recursive(int n)
{
    /* TODO: Base case — what value of n should stop the recursion?
     *       Return 1 for that case. */

    /* TODO: Recursive case — return n * factorial_recursive(n - 1) */

    return 0;  /* placeholder — remove when you implement */
}

/* Returns n! using a loop */
int factorial_iterative(int n)
{
    /* TODO: Create a variable 'result' initialized to 1 */

    /* TODO: Loop from 1 to n, multiplying result by i each time */

    return 0;  /* placeholder — remove when you implement */
}

/* ---- Test harness (do not modify) ---- */

int main(void)
{
    int test_cases[] = {0, 1, 2, 3, 4, 5, 10, 12};
    int expected[]   = {1, 1, 2, 6, 24, 120, 3628800, 479001600};
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    int passed = 0;

    printf("=== Factorial Tests ===\n\n");

    for (int i = 0; i < num_tests; i++) {
        int n = test_cases[i];
        int r = factorial_recursive(n);
        int it = factorial_iterative(n);
        int exp = expected[i];

        int ok_r = (r == exp);
        int ok_i = (it == exp);

        printf("factorial(%2d): recursive=%10d %s  iterative=%10d %s\n",
               n, r, ok_r ? "PASS" : "FAIL",
               it, ok_i ? "PASS" : "FAIL");

        if (ok_r && ok_i) {
            passed++;
        }
    }

    printf("\n%d/%d tests passed.\n", passed, num_tests);

    if (passed == num_tests) {
        printf("All tests passed!\n");
    }

    return (passed == num_tests) ? 0 : 1;
}
