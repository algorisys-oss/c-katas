/*
 * bit_operations.c — Implement fundamental bit manipulation functions
 *
 * In this kata you will implement four functions that operate on
 * individual bits within an unsigned integer:
 *
 *   set_bit(value, bit)    -- set bit N to 1
 *   clear_bit(value, bit)  -- set bit N to 0
 *   toggle_bit(value, bit) -- flip bit N (0->1, 1->0)
 *   check_bit(value, bit)  -- return 1 if bit N is set, 0 otherwise
 *
 * Bit numbering: bit 0 is the rightmost (least significant) bit.
 *
 *   bit:    7  6  5  4  3  2  1  0
 *         +--+--+--+--+--+--+--+--+
 *   value | 0| 1| 0| 0| 1| 0| 1| 0|  =  0x4A  =  74
 *         +--+--+--+--+--+--+--+--+
 *
 * Hints:
 *   - To create a mask with only bit N set:     (1U << n)
 *   - To set a bit, use OR:                     value | mask
 *   - To clear a bit, use AND with inverted:    value & ~mask
 *   - To toggle a bit, use XOR:                 value ^ mask
 *   - To check a bit, use AND and compare:      (value & mask) != 0
 *
 * Compile: gcc -Wall -Wextra -pedantic -std=c99 -o bit_operations bit_operations.c
 * Run:     ./bit_operations
 */

#include <stdio.h>

/* ================================================================
 * Helper: print an unsigned int as binary (provided for you)
 * ================================================================ */
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

/* ================================================================
 * TODO: Implement set_bit
 *
 * Set bit number 'bit' of 'value' to 1 and return the result.
 *
 * Example: set_bit(0b00000000, 3) = 0b00001000
 * ================================================================ */
unsigned int set_bit(unsigned int value, int bit)
{
    /* TODO: implement this function */
    (void)value;
    (void)bit;
    return 0;
}

/* ================================================================
 * TODO: Implement clear_bit
 *
 * Set bit number 'bit' of 'value' to 0 and return the result.
 *
 * Example: clear_bit(0b11111111, 3) = 0b11110111
 * ================================================================ */
unsigned int clear_bit(unsigned int value, int bit)
{
    /* TODO: implement this function */
    (void)value;
    (void)bit;
    return 0;
}

/* ================================================================
 * TODO: Implement toggle_bit
 *
 * Flip bit number 'bit' of 'value' and return the result.
 *
 * Example: toggle_bit(0b00001000, 3) = 0b00000000
 *          toggle_bit(0b00000000, 3) = 0b00001000
 * ================================================================ */
unsigned int toggle_bit(unsigned int value, int bit)
{
    /* TODO: implement this function */
    (void)value;
    (void)bit;
    return 0;
}

/* ================================================================
 * TODO: Implement check_bit
 *
 * Return 1 if bit number 'bit' of 'value' is set, 0 otherwise.
 *
 * Example: check_bit(0b00001000, 3) = 1
 *          check_bit(0b00001000, 2) = 0
 * ================================================================ */
int check_bit(unsigned int value, int bit)
{
    /* TODO: implement this function */
    (void)value;
    (void)bit;
    return 0;
}

/* ================================================================
 * Test harness (provided — do not modify)
 * ================================================================ */

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
