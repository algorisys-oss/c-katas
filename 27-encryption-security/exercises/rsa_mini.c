/*
 * Kata: Mini RSA
 *
 * Implement RSA encryption from first principles using small primes.
 * All math uses long long to avoid overflow with small prime products.
 *
 *   - Modular exponentiation (repeated squaring)
 *   - Greatest common divisor (Euclidean algorithm)
 *   - Modular inverse (extended Euclidean algorithm)
 *   - RSA key generation, encryption, and decryption
 *
 * 10 tests total.
 *
 * Hints:
 *   - mod_pow: square and multiply, reduce mod m at each step
 *   - gcd: Euclidean algorithm — gcd(a, b) = gcd(b, a % b)
 *   - mod_inverse: find d such that (e * d) % phi == 1
 *     Use extended Euclidean algorithm or brute-force search (phi is small)
 *   - RSA: c = msg^e mod n, msg = c^d mod n
 */

#include <stdio.h>

/* ── Exercise 1: mod_pow ──────────────────────────────────────────
 * Compute (base^exp) mod mod using repeated squaring.
 *
 * Algorithm:
 *   result = 1
 *   base = base % mod
 *   while exp > 0:
 *     if exp is odd: result = (result * base) % mod
 *     exp = exp >> 1  (divide by 2)
 *     base = (base * base) % mod
 *   return result
 *
 * This runs in O(log exp) multiplications.
 */
long long mod_pow(long long base, long long exp, long long mod)
{
    /* TODO: Implement repeated squaring.
     *       Be careful: use long long for intermediate products. */
    return 0;
}

/* ── Exercise 2: gcd ─────────────────────────────────────────────
 * Compute the greatest common divisor of a and b.
 *
 * Euclidean algorithm:
 *   while b != 0:
 *     temp = b
 *     b = a % b
 *     a = temp
 *   return a
 */
long long gcd(long long a, long long b)
{
    /* TODO: Implement the Euclidean algorithm. */
    return 0;
}

/* ── Exercise 3: mod_inverse ──────────────────────────────────────
 * Find d such that (e * d) % phi == 1.
 *
 * This is the modular multiplicative inverse of e modulo phi.
 *
 * For small phi, you can brute-force: try d = 1, 2, 3, ...
 * until (e * d) % phi == 1.
 *
 * For a more elegant solution, use the extended Euclidean algorithm.
 *
 * Returns d, or -1 if no inverse exists (when gcd(e, phi) != 1).
 */
long long mod_inverse(long long e, long long phi)
{
    /* TODO: Find d such that (e * d) % phi == 1.
     *       Return -1 if gcd(e, phi) != 1. */
    return -1;
}

/* ── Exercise 4: rsa_keygen ───────────────────────────────────────
 * Generate RSA key pair from two primes p and q.
 *
 * Steps:
 *   1. n = p * q
 *   2. phi = (p - 1) * (q - 1)
 *   3. Choose e = 17 (a common small public exponent; must be coprime to phi)
 *   4. Compute d = mod_inverse(e, phi)
 *
 * Store n, e, d through the output pointers.
 * Returns 0 on success, -1 if e and phi are not coprime.
 */
int rsa_keygen(long long p, long long q,
               long long *n, long long *e, long long *d)
{
    /* TODO: Compute n, phi, set e=17, compute d. Check gcd(e,phi)==1. */
    return -1;
}

/* ── Exercise 5: rsa_encrypt ──────────────────────────────────────
 * Encrypt a message: ciphertext = msg^e mod n
 *
 * msg must be less than n.
 */
long long rsa_encrypt(long long msg, long long e, long long n)
{
    /* TODO: One line using mod_pow. */
    return 0;
}

/* ── Exercise 6: rsa_decrypt ──────────────────────────────────────
 * Decrypt a ciphertext: plaintext = cipher^d mod n
 */
long long rsa_decrypt(long long cipher, long long d, long long n)
{
    /* TODO: One line using mod_pow. */
    return 0;
}


/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Mini RSA ===\n\n");

    /* --- mod_pow --- */
    printf("-- Modular Exponentiation --\n");

    TEST("mod_pow(2, 10, 1000) = 24") EXPECT(mod_pow(2, 10, 1000) == 24);
    TEST("mod_pow(3, 13, 7) = 3") EXPECT(mod_pow(3, 13, 7) == 3);
    TEST("mod_pow(5, 0, 7) = 1") EXPECT(mod_pow(5, 0, 7) == 1);

    /* --- gcd --- */
    printf("\n-- Greatest Common Divisor --\n");

    TEST("gcd(48, 18) = 6") EXPECT(gcd(48, 18) == 6);
    TEST("gcd(17, 3120) = 1 (coprime)") EXPECT(gcd(17, 3120) == 1);

    /* --- mod_inverse --- */
    printf("\n-- Modular Inverse --\n");

    TEST("mod_inverse(17, 3120) = 2753") EXPECT(mod_inverse(17, 3120) == 2753);
    TEST("mod_inverse(3, 7) = 5 (3*5=15, 15%%7=1)") EXPECT(mod_inverse(3, 7) == 5);

    /* --- RSA full pipeline --- */
    printf("\n-- RSA Key Generation & Encryption --\n");

    {
        long long n, e, d;
        int ok = rsa_keygen(61, 53, &n, &e, &d);
        TEST("rsa_keygen(61, 53) succeeds") EXPECT(ok == 0 && n == 3233 && e == 17 && d == 2753);

        long long msg = 65;
        long long cipher = rsa_encrypt(msg, e, n);
        long long decrypted = rsa_decrypt(cipher, d, n);
        TEST("RSA encrypt/decrypt round-trip (msg=65)") EXPECT(decrypted == msg);

        /* Test with several messages */
        int all_ok = 1;
        long long test_msgs[] = {42, 100, 1000, 3000, 1, 0};
        for (int i = 0; test_msgs[i] >= 0 && i < 6; i++) {
            if (test_msgs[i] >= n) continue;
            long long c = rsa_encrypt(test_msgs[i], e, n);
            long long m = rsa_decrypt(c, d, n);
            if (m != test_msgs[i]) { all_ok = 0; break; }
        }
        TEST("RSA round-trip for multiple messages") EXPECT(all_ok);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
