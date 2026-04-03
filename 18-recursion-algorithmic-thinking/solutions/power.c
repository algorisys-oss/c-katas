#include <stdio.h>

/* Naive O(n) power — multiply x by itself n times */
long long power_naive(long long x, int n)
{
    long long result = 1;
    for (int i = 0; i < n; i++) {
        result *= x;
    }
    return result;
}

/* Fast O(log n) power — exponentiation by squaring */
long long power_fast(long long x, int n)
{
    if (n == 0) {
        return 1;
    }
    long long half = power_fast(x, n / 2);
    if (n % 2 == 0) {
        return half * half;
    }
    return x * half * half;
}

/* ---- Test harness ---- */

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
