/*
 * Kata: Fast Power (Exponentiation by Squaring)
 *
 * Implement power(x, n) which computes x^n.
 *
 * The naive approach multiplies x by itself n times — O(n).
 * The fast approach uses a divide-and-conquer trick — O(log n):
 *
 *   x^0 = 1                          (base case)
 *   x^n = (x^(n/2))^2       if n is even
 *   x^n = x * (x^(n/2))^2   if n is odd
 *
 * Why does this work?
 *   x^8 = x * x * x * x * x * x * x * x     (8 multiplications)
 *   x^8 = (x^4)^2 = ((x^2)^2)^2              (3 multiplications!)
 *
 * Examples:
 *   power(2, 0)  = 1
 *   power(2, 10) = 1024
 *   power(3, 5)  = 243
 *   power(5, 3)  = 125
 *
 * Hints:
 *   - Use long long to avoid overflow for larger results
 *   - n/2 in integer division drops the remainder (that is fine)
 *   - "if n is odd" means n % 2 != 0
 */

#include <stdio.h>

/* Naive O(n) power — multiply x by itself n times */
long long power_naive(long long x, int n)
{
    /* TODO: Create a variable 'result' initialized to 1 */

    /* TODO: Loop n times, multiplying result by x each iteration */

    /* TODO: Return result */

    return 0;  /* placeholder */
}

/* Fast O(log n) power — exponentiation by squaring */
long long power_fast(long long x, int n)
{
    /* TODO: Base case — x^0 = 1 */

    /* TODO: Compute half = power_fast(x, n / 2) */

    /* TODO: If n is even, return half * half */

    /* TODO: If n is odd, return x * half * half */

    return 0;  /* placeholder */
}

/* ---- Test harness (do not modify) ---- */

int main(void)
{
    struct {
        long long x;
        int n;
        long long expected;
    } tests[] = {
        {2,  0,  1},
        {2,  1,  2},
        {2,  10, 1024},
        {3,  5,  243},
        {5,  3,  125},
        {7,  4,  2401},
        {2,  20, 1048576},
        {1,  100, 1},
        {10, 6,  1000000},
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;

    printf("=== Power Function Tests ===\n\n");

    for (int i = 0; i < num_tests; i++) {
        long long x = tests[i].x;
        int n = tests[i].n;
        long long exp = tests[i].expected;

        long long naive = power_naive(x, n);
        long long fast = power_fast(x, n);

        int ok_n = (naive == exp);
        int ok_f = (fast == exp);

        printf("power(%lld, %2d) = %10lld  naive:%s  fast:%s\n",
               x, n, exp,
               ok_n ? "PASS" : "FAIL",
               ok_f ? "PASS" : "FAIL");

        if (ok_n && ok_f) {
            passed++;
        }
    }

    printf("\n%d/%d tests passed.\n", passed, num_tests);

    if (passed == num_tests) {
        printf("All tests passed!\n");
    }

    return (passed == num_tests) ? 0 : 1;
}
