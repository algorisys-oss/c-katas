#include <stdio.h>

int is_prime(int n)
{
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

/*
 * Test harness: verifies is_prime against known values.
 */
int main(void)
{
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    int num_primes = sizeof(primes) / sizeof(primes[0]);

    int non_primes[] = {0, 1, 4, 6, 8, 9, 10, 12, 15, 20, 25, 49, 100};
    int num_non_primes = sizeof(non_primes) / sizeof(non_primes[0]);

    int passed = 0;
    int failed = 0;

    for (int i = 0; i < num_primes; i++) {
        if (is_prime(primes[i])) {
            passed++;
        } else {
            printf("FAIL: is_prime(%d) returned 0, expected 1\n", primes[i]);
            failed++;
        }
    }

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
