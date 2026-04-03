/*
 * Kata: Bit Manipulation Tricks
 *
 * Bit tricks are the foundation for Fenwick trees and many efficient
 * algorithms. You learned binary numbers in Module 00 and two's complement
 * in Module 02. Now you'll learn to manipulate individual bits.
 *
 * Quick review of two's complement (for 8-bit):
 *   -n is computed as: flip all bits, add 1
 *    12 = 0000 1100
 *   -12 = 1111 0011 + 1 = 1111 0100
 *
 * Key binary operations:
 *   AND (&):  1 only if both bits are 1
 *   OR  (|):  1 if either bit is 1
 *   XOR (^):  1 if bits are different
 *   NOT (~):  flip every bit
 *   <<  :     shift bits left (multiply by 2)
 *   >>  :     shift bits right (divide by 2)
 *
 * Why n & (-n) gives the lowest set bit:
 *
 *     n  = 12 = 0000 1100
 *    -n  =      1111 0100      (flip + 1)
 *   n&-n =      0000 0100 = 4  (only the lowest set bit survives!)
 *
 *   This works because flipping clears everything below the lowest 1,
 *   and adding 1 sets exactly that bit back. Everything above cancels.
 *
 * Brian Kernighan's trick: n & (n-1) clears the lowest set bit
 *
 *     n   = 12 = 1100
 *     n-1 = 11 = 1011     (borrows from lowest set bit, flips it + below)
 *   n&n-1 = 8  = 1000     (lowest set bit gone!)
 *
 *     n   =  8 = 1000
 *     n-1 =  7 = 0111
 *   n&n-1 =  0 = 0000     (was power of 2, now zero)
 *
 * XOR truth table:
 *     a | b | a^b
 *    ---+---+----
 *     0 | 0 |  0
 *     0 | 1 |  1
 *     1 | 0 |  1
 *     1 | 1 |  0
 *
 *   Properties: a ^ a = 0, a ^ 0 = a, a ^ b ^ a = b
 *
 * 17 tests total.
 */

#include <stdio.h>

/* ── Exercise 1: lowest_set_bit ──────────────────────────────────────
 * Return the value of the lowest set bit in n.
 *
 * Formula: n & (-n)
 *
 * Examples:
 *   12  (1100)  → 4 (0100)
 *   10  (1010)  → 2 (0010)
 *   1   (0001)  → 1 (0001)
 *   0          → 0 (no bits set)
 */
int lowest_set_bit(int n)
{
    /* TODO: Return n & (-n) */
    (void)n; /* suppress unused warning */
    return 0; /* placeholder */
}

/* ── Exercise 2: count_set_bits ──────────────────────────────────────
 * Count the number of 1-bits in n (population count / Hamming weight).
 *
 * Use Brian Kernighan's trick: n = n & (n-1) clears the lowest set
 * bit. Count how many times you can do this before n becomes 0.
 *
 * Example for n = 13 (1101):
 *   1101 & 1100 = 1100  (count=1, cleared bit 0)
 *   1100 & 1011 = 1000  (count=2, cleared bit 2)
 *   1000 & 0111 = 0000  (count=3, cleared bit 3)
 *   n is 0, done → 3 set bits
 */
int count_set_bits(int n)
{
    /* TODO: Loop: while n != 0, do n = n & (n-1), count++ */
    (void)n; /* suppress unused warning */
    return 0; /* placeholder */
}

/* ── Exercise 3: is_power_of_two ─────────────────────────────────────
 * Return 1 if n is a power of 2, 0 otherwise.
 *
 * A power of 2 has exactly one set bit:
 *   1 = 0001, 2 = 0010, 4 = 0100, 8 = 1000
 *
 * So: n > 0 && (n & (n-1)) == 0
 *
 * Why? n & (n-1) clears the lowest set bit. If there's only one bit,
 * the result is 0.
 */
int is_power_of_two(int n)
{
    /* TODO: Return n > 0 && (n & (n-1)) == 0 */
    (void)n; /* suppress unused warning */
    return 0; /* placeholder */
}

/* ── Exercise 4: swap_without_temp ───────────────────────────────────
 * Swap two integers using XOR (no temporary variable).
 *
 * Algorithm:
 *   a ^= b;   // a now holds a^b
 *   b ^= a;   // b = b ^ (a^b) = a  (because b^b cancels)
 *   a ^= b;   // a = (a^b) ^ a = b  (because a^a cancels)
 *
 * Note: this only works when a and b are different variables.
 * If &a == &b, it zeroes both (since a^a = 0).
 */
void swap_without_temp(int *a, int *b)
{
    /* TODO: XOR swap: *a ^= *b; *b ^= *a; *a ^= *b; */
    (void)a; (void)b; /* suppress unused warnings */
}

/* ── Exercise 5: next_power_of_two ───────────────────────────────────
 * Round n up to the next power of 2.
 * If n is already a power of 2, return n.
 *
 * Algorithm: subtract 1 (to handle exact powers), then set all bits
 * below the highest set bit, then add 1.
 *
 *   n = 5 (0101)
 *   n-1 = 4 (0100)
 *   Spread highest bit down:
 *     0100 | 0010 = 0110
 *     0110 | 0001 = 0111
 *   Add 1: 0111 + 1 = 1000 = 8
 *
 * Use: n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16;
 * Then n + 1.
 *
 * Special case: if n <= 0, return 1.
 */
int next_power_of_two(int n)
{
    /* TODO:
     * if (n <= 0) return 1;
     * if (is_power_of_two(n)) return n;
     * n--;
     * n |= n >> 1;
     * n |= n >> 2;
     * n |= n >> 4;
     * n |= n >> 8;
     * n |= n >> 16;
     * return n + 1;
     */
    (void)n; /* suppress unused warning */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Bit Manipulation Tricks ===\n\n");

    /* --- lowest_set_bit --- */
    printf("lowest_set_bit:\n");
    TEST("lowest set bit of 12 (1100) = 4")
        EXPECT(lowest_set_bit(12) == 4);
    TEST("lowest set bit of 10 (1010) = 2")
        EXPECT(lowest_set_bit(10) == 2);
    TEST("lowest set bit of 1 (0001) = 1")
        EXPECT(lowest_set_bit(1) == 1);
    TEST("lowest set bit of 0 = 0")
        EXPECT(lowest_set_bit(0) == 0);

    /* --- count_set_bits --- */
    printf("\ncount_set_bits:\n");
    TEST("set bits in 0 = 0")
        EXPECT(count_set_bits(0) == 0);
    TEST("set bits in 1 (0001) = 1")
        EXPECT(count_set_bits(1) == 1);
    TEST("set bits in 13 (1101) = 3")
        EXPECT(count_set_bits(13) == 3);
    TEST("set bits in 255 (1111 1111) = 8")
        EXPECT(count_set_bits(255) == 8);

    /* --- is_power_of_two --- */
    printf("\nis_power_of_two:\n");
    TEST("1 is power of 2")
        EXPECT(is_power_of_two(1) == 1);
    TEST("16 is power of 2")
        EXPECT(is_power_of_two(16) == 1);
    TEST("0 is NOT power of 2")
        EXPECT(is_power_of_two(0) == 0);
    TEST("6 is NOT power of 2")
        EXPECT(is_power_of_two(6) == 0);

    /* --- swap_without_temp --- */
    printf("\nswap_without_temp:\n");
    {
        int a = 42, b = 99;
        swap_without_temp(&a, &b);
        TEST("swap 42 and 99")
            EXPECT(a == 99 && b == 42);
    }
    {
        int a = -5, b = 10;
        swap_without_temp(&a, &b);
        TEST("swap -5 and 10")
            EXPECT(a == 10 && b == -5);
    }

    /* --- next_power_of_two --- */
    printf("\nnext_power_of_two:\n");
    TEST("next power of 2 for 5 = 8")
        EXPECT(next_power_of_two(5) == 8);
    TEST("next power of 2 for 16 = 16 (already power)")
        EXPECT(next_power_of_two(16) == 16);
    TEST("next power of 2 for 17 = 32")
        EXPECT(next_power_of_two(17) == 32);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
