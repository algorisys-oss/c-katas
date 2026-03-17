#include <stdio.h>

/* Returns the nth Fibonacci number using naive recursion */
int fib_recursive(int n)
{
    if (n <= 1) {
        return n;
    }
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

/* Returns the nth Fibonacci number using iteration */
int fib_iterative(int n)
{
    if (n <= 1) {
        return n;
    }
    int prev = 0;
    int curr = 1;
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

/* Returns the nth Fibonacci number recursively AND counts total calls */
int fib_recursive_count(int n, int *count)
{
    (*count)++;
    if (n <= 1) {
        return n;
    }
    return fib_recursive_count(n - 1, count)
         + fib_recursive_count(n - 2, count);
}

/* ---- Test harness ---- */

int main(void)
{
    int test_n[]    = {0, 1, 2, 3, 4, 5, 10, 15, 20};
    int expected[]  = {0, 1, 1, 2, 3, 5, 55, 610, 6765};
    int num_tests = sizeof(test_n) / sizeof(test_n[0]);
    int passed = 0;

    printf("=== Fibonacci Tests ===\n\n");

    for (int i = 0; i < num_tests; i++) {
        int n = test_n[i];
        int r = fib_recursive(n);
        int it = fib_iterative(n);
        int exp = expected[i];

        int ok_r = (r == exp);
        int ok_i = (it == exp);

        printf("fib(%2d): recursive=%6d %s  iterative=%6d %s\n",
               n, r, ok_r ? "PASS" : "FAIL",
               it, ok_i ? "PASS" : "FAIL");

        if (ok_r && ok_i) {
            passed++;
        }
    }

    printf("\n%d/%d tests passed.\n\n", passed, num_tests);

    /* Show the explosion of calls */
    printf("=== Call Count (recursive) ===\n\n");
    printf("  n  |  fib(n)  |  total calls\n");
    printf("-----+----------+-------------\n");

    for (int n = 0; n <= 30; n += 5) {
        int count = 0;
        int result = fib_recursive_count(n, &count);
        printf("  %2d |  %6d  |  %10d\n", n, result, count);
    }

    printf("\nNotice how the call count roughly doubles every time n increases by 1.\n");
    printf("This is O(2^n) — exponential growth.\n");

    return (passed == num_tests) ? 0 : 1;
}
