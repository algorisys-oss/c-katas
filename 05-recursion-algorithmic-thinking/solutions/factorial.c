#include <stdio.h>

/* Returns n! using recursion */
int factorial_recursive(int n)
{
    if (n == 0) {
        return 1;
    }
    return n * factorial_recursive(n - 1);
}

/* Returns n! using a loop */
int factorial_iterative(int n)
{
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

/* ---- Test harness ---- */

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
