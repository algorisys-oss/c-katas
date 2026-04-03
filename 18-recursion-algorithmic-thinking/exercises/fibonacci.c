/*
 * Kata: Fibonacci
 *
 * Implement the Fibonacci sequence three ways:
 *   1. fib_recursive(n)       — naive recursive O(2^n)
 *   2. fib_iterative(n)       — iterative O(n)
 *   3. fib_recursive_count(n, count) — recursive, but also counts total calls
 *
 * The Fibonacci sequence:
 *   fib(0) = 0
 *   fib(1) = 1
 *   fib(n) = fib(n-1) + fib(n-2)   for n >= 2
 *
 *   Sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, ...
 *
 * After implementing all three, the test harness will show you the
 * explosive growth of function calls in the recursive version.
 *
 * Hints:
 *   - Recursive: two base cases (n==0 returns 0, n==1 returns 1)
 *   - Iterative: track 'prev' and 'curr', walk forward in a loop
 *   - Count version: increment *count each time the function is entered
 */

#include <stdio.h>

/* Returns the nth Fibonacci number using naive recursion */
int fib_recursive(int n)
{
    /* TODO: Base cases — return n if n <= 1 */

    /* TODO: Recursive case — return fib_recursive(n-1) + fib_recursive(n-2) */

    return 0;  /* placeholder */
}

/* Returns the nth Fibonacci number using iteration */
int fib_iterative(int n)
{
    /* TODO: Handle the base case (n <= 1) */

    /* TODO: Create two variables: prev = 0, curr = 1 */

    /* TODO: Loop from 2 to n:
     *   - compute next = prev + curr
     *   - shift: prev = curr, curr = next
     */

    /* TODO: Return curr */

    return 0;  /* placeholder */
}

/* Returns the nth Fibonacci number recursively AND counts total calls.
 * Before doing anything else, increment *count. */
int fib_recursive_count(int n, int *count)
{
    /* TODO: Increment *count (dereference the pointer and add 1) */

    /* TODO: Base cases — return n if n <= 1 */

    /* TODO: Recursive case — call fib_recursive_count for (n-1) and (n-2),
     *       passing count through so it accumulates */

    return 0;  /* placeholder */
}

/* ---- Test harness (do not modify) ---- */

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
