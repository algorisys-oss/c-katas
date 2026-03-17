/*
 * Kata: Prime Number Checker
 *
 * Implement a function that checks if a number is prime.
 * A prime number is a number greater than 1 that has no divisors
 * other than 1 and itself.
 *
 * Algorithm (pseudocode):
 *   1. If n < 2, return NOT prime
 *   2. If n is 2, return prime (2 is the only even prime)
 *   3. If n is even, return NOT prime
 *   4. Check odd divisors from 3 to sqrt(n):
 *      - If any divides n evenly, return NOT prime
 *   5. If no divisor found, return prime
 *
 * Why only up to sqrt(n)?
 *   If n = a * b, then at least one of a or b must be <= sqrt(n).
 *   So if no number up to sqrt(n) divides n, nothing will.
 *
 * Hints:
 *   - n % i == 0 means i divides n
 *   - You can skip even numbers after checking 2 (step by 2 in the loop)
 *   - i * i <= n is equivalent to i <= sqrt(n) but avoids floating point
 *   - Return 1 for prime, 0 for not prime
 */

#include <stdio.h>

int is_prime(int n)
{
    /* TODO: Handle edge cases:
     *   - If n < 2, return 0 (not prime)
     *   - If n == 2, return 1 (prime)
     *   - If n is even (n % 2 == 0), return 0 (not prime)
     */

    /* TODO: Loop through odd numbers from 3 to sqrt(n):
     *   - Use: for (int i = 3; i * i <= n; i += 2)
     *   - If n % i == 0, return 0 (found a divisor)
     */

    /* TODO: If we get here, no divisor was found. Return 1 (prime). */

    (void)n;   /* suppress unused-parameter warning until you implement */
    return 0;  /* placeholder -- remove this when you implement the function */
}

/*
 * Test harness: verifies your is_prime function against known values.
 * Do not modify this function.
 */
int main(void)
{
    /* Known primes under 50 */
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    int num_primes = sizeof(primes) / sizeof(primes[0]);

    /* Known non-primes to test */
    int non_primes[] = {0, 1, 4, 6, 8, 9, 10, 12, 15, 20, 25, 49, 100};
    int num_non_primes = sizeof(non_primes) / sizeof(non_primes[0]);

    int passed = 0;
    int failed = 0;

    /* Test primes */
    for (int i = 0; i < num_primes; i++) {
        if (is_prime(primes[i])) {
            passed++;
        } else {
            printf("FAIL: is_prime(%d) returned 0, expected 1\n", primes[i]);
            failed++;
        }
    }

    /* Test non-primes */
    for (int i = 0; i < num_non_primes; i++) {
        if (!is_prime(non_primes[i])) {
            passed++;
        } else {
            printf("FAIL: is_prime(%d) returned 1, expected 0\n",
                   non_primes[i]);
            failed++;
        }
    }

    printf("\nResults: %d passed, %d failed out of %d tests.\n",
           passed, failed, passed + failed);

    if (failed == 0) {
        printf("All tests passed!\n");
    }

    return failed > 0 ? 1 : 0;
}
