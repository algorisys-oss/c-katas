/*
 * Kata: Mini RSA — SOLUTION
 *
 * Implement RSA encryption from first principles using small primes.
 * All math uses long long to avoid overflow with small prime products.
 *
 * 10 tests total.
 */

#include <stdio.h>

/* ── Exercise 1: mod_pow ──────────────────────────────────────────
 * Compute (base^exp) mod mod using repeated squaring.
 */
long long mod_pow(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

/* ── Exercise 2: gcd ─────────────────────────────────────────────
 * Compute the greatest common divisor using the Euclidean algorithm.
 */
long long gcd(long long a, long long b)
{
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* ── Exercise 3: mod_inverse ──────────────────────────────────────
 * Find d such that (e * d) % phi == 1.
 * Uses the extended Euclidean algorithm.
 */
long long mod_inverse(long long e, long long phi)
{
    if (gcd(e, phi) != 1) {
        return -1;
    }

    /* Extended Euclidean algorithm */
    long long a = e, b = phi;
    long long x0 = 1, x1 = 0;

    while (b != 0) {
        long long q = a / b;
        long long temp = b;
        b = a - q * b;
        a = temp;

        temp = x1;
        x1 = x0 - q * x1;
        x0 = temp;
    }

    /* Make sure result is positive */
    return ((x0 % phi) + phi) % phi;
}

/* ── Exercise 4: rsa_keygen ───────────────────────────────────────
 * Generate RSA key pair from two primes p and q.
 */
int rsa_keygen(long long p, long long q,
               long long *n, long long *e, long long *d)
{
    *n = p * q;
    long long phi = (p - 1) * (q - 1);
    *e = 17;

    if (gcd(*e, phi) != 1) {
        return -1;
    }

    *d = mod_inverse(*e, phi);
    return 0;
}

/* ── Exercise 5: rsa_encrypt ──────────────────────────────────────
 * Encrypt: ciphertext = msg^e mod n
 */
long long rsa_encrypt(long long msg, long long e, long long n)
{
    return mod_pow(msg, e, n);
}

/* ── Exercise 6: rsa_decrypt ──────────────────────────────────────
 * Decrypt: plaintext = cipher^d mod n
 */
long long rsa_decrypt(long long cipher, long long d, long long n)
{
    return mod_pow(cipher, d, n);
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
