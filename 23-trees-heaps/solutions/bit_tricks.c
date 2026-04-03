/*
 * Solution: Bit Manipulation Tricks
 */

#include <stdio.h>

/* ── lowest_set_bit ──────────────────────────────────────────────────── */

int lowest_set_bit(int n)
{
    return n & (-n);
}

/* ── count_set_bits ──────────────────────────────────────────────────── */

int count_set_bits(int n)
{
    int count = 0;
    while (n != 0) {
        n = n & (n - 1);  /* clear lowest set bit */
        count++;
    }
    return count;
}

/* ── is_power_of_two ─────────────────────────────────────────────────── */

int is_power_of_two(int n)
{
    return n > 0 && (n & (n - 1)) == 0;
}

/* ── swap_without_temp ───────────────────────────────────────────────── */

void swap_without_temp(int *a, int *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

/* ── next_power_of_two ───────────────────────────────────────────────── */

int next_power_of_two(int n)
{
    if (n <= 0) {
        return 1;
    }
    if (is_power_of_two(n)) {
        return n;
    }
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
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
