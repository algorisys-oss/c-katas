/*
 * bit_operations.c — Solution
 *
 * Compile: gcc -Wall -Wextra -pedantic -std=c99 -o bit_operations bit_operations.c
 * Run:     ./bit_operations
 */

#include <stdio.h>

/* Helper: print an unsigned int as binary (provided) */
void print_binary(unsigned int value, int bits)
{
    int i;
    for (i = bits - 1; i >= 0; i--) {
        printf("%u", (value >> i) & 1U);
        if (i > 0 && i % 4 == 0) {
            printf(" ");
        }
    }
}

/* Set bit number 'bit' of 'value' to 1 and return the result. */
unsigned int set_bit(unsigned int value, int bit)
{
    return value | (1U << bit);
}

/* Set bit number 'bit' of 'value' to 0 and return the result. */
unsigned int clear_bit(unsigned int value, int bit)
{
    return value & ~(1U << bit);
}

/* Flip bit number 'bit' of 'value' and return the result. */
unsigned int toggle_bit(unsigned int value, int bit)
{
    return value ^ (1U << bit);
}

/* Return 1 if bit number 'bit' of 'value' is set, 0 otherwise. */
int check_bit(unsigned int value, int bit)
{
    return (value & (1U << bit)) != 0;
}

/* Test harness */

static int tests_passed = 0;
static int tests_failed = 0;

void assert_equal(const char *test_name, unsigned int expected,
                  unsigned int actual)
{
    if (expected == actual) {
        printf("  PASS: %s\n", test_name);
        tests_passed++;
    } else {
        printf("  FAIL: %s (expected ", test_name);
        print_binary(expected, 8);
        printf(" [%u], got ", expected);
        print_binary(actual, 8);
        printf(" [%u])\n", actual);
        tests_failed++;
    }
}

int main(void)
{
    printf("=== Testing set_bit ===\n");
    assert_equal("set bit 0 of 0", 1, set_bit(0, 0));
    assert_equal("set bit 3 of 0", 8, set_bit(0, 3));
    assert_equal("set bit 7 of 0", 128, set_bit(0, 7));
    assert_equal("set bit 1 of 5", 7, set_bit(5, 1));
    assert_equal("set bit already set", 5, set_bit(5, 0));

    printf("\n=== Testing clear_bit ===\n");
    assert_equal("clear bit 0 of 1", 0, clear_bit(1, 0));
    assert_equal("clear bit 3 of 255", 247, clear_bit(255, 3));
    assert_equal("clear bit 7 of 255", 127, clear_bit(255, 7));
    assert_equal("clear bit already clear", 6, clear_bit(6, 0));

    printf("\n=== Testing toggle_bit ===\n");
    assert_equal("toggle bit 0 of 0 (0->1)", 1, toggle_bit(0, 0));
    assert_equal("toggle bit 0 of 1 (1->0)", 0, toggle_bit(1, 0));
    assert_equal("toggle bit 3 of 0", 8, toggle_bit(0, 3));
    assert_equal("toggle bit 3 of 8", 0, toggle_bit(8, 3));

    printf("\n=== Testing check_bit ===\n");
    assert_equal("check bit 0 of 1", 1, (unsigned)check_bit(1, 0));
    assert_equal("check bit 1 of 1", 0, (unsigned)check_bit(1, 1));
    assert_equal("check bit 3 of 8", 1, (unsigned)check_bit(8, 3));
    assert_equal("check bit 7 of 128", 1, (unsigned)check_bit(128, 7));
    assert_equal("check bit 7 of 127", 0, (unsigned)check_bit(127, 7));

    printf("\n=== Results: %d passed, %d failed ===\n",
           tests_passed, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
