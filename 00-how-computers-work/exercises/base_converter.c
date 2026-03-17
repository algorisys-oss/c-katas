/*
 * base_converter.c — Convert numbers between decimal, binary, and hexadecimal
 *
 * YOUR MISSION:
 *   Fill in the functions marked with TODO to build a working base converter.
 *   The main() function at the bottom will test your work.
 *
 * CONCEPTS YOU'LL USE:
 *   - The "repeated division" algorithm for converting between bases
 *   - Modulo (%) to get remainders
 *   - Integer division (/) to reduce the number
 *   - Arrays to store digits
 *   - The idea that '0'-'9' and 'A'-'F' are just characters with numeric values
 *
 * COMPILE:
 *   gcc -Wall -Wextra -pedantic -std=c99 -o base_converter base_converter.c
 *
 * RUN:
 *   ./base_converter
 */

#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 64  /* enough bits for very large numbers */

/*
 * decimal_to_binary_str
 *
 * Convert an unsigned integer to its binary string representation.
 *
 * Parameters:
 *   value  - the number to convert
 *   result - character array to store the binary string (must be at least
 *            MAX_DIGITS + 1 bytes)
 *
 * Example:
 *   decimal_to_binary_str(42, result)  =>  result = "101010"
 *   decimal_to_binary_str(0, result)   =>  result = "0"
 *
 * Algorithm:
 *   1. Special-case: if value is 0, the result is "0".
 *   2. Repeatedly divide by 2. Each remainder is a binary digit.
 *   3. The remainders come out in reverse order (least significant first),
 *      so you'll need to reverse the string at the end.
 *
 * Hint: remainder of dividing by 2 is either 0 or 1.
 *       '0' + 0 = '0',  '0' + 1 = '1'
 */
void decimal_to_binary_str(unsigned int value, char result[])
{
    /* TODO: Handle the special case where value is 0 */

    /* TODO: Use a loop to repeatedly divide by 2.
     *       Store each remainder as a character ('0' or '1') in result[].
     *       Keep a counter for how many digits you've stored. */

    /* TODO: Null-terminate the string. */

    /* TODO: Reverse the string (the digits came out backwards).
     *       You can swap characters from both ends moving inward. */
}

/*
 * decimal_to_hex_str
 *
 * Convert an unsigned integer to its hexadecimal string representation.
 *
 * Parameters:
 *   value  - the number to convert
 *   result - character array to store the hex string (must be at least
 *            MAX_DIGITS + 1 bytes)
 *
 * Example:
 *   decimal_to_hex_str(255, result)  =>  result = "FF"
 *   decimal_to_hex_str(0, result)    =>  result = "0"
 *   decimal_to_hex_str(42, result)   =>  result = "2A"
 *
 * Algorithm:
 *   Same as binary, but divide by 16 instead of 2.
 *   Remainders 0-9 map to '0'-'9', and 10-15 map to 'A'-'F'.
 *
 * Hint: You can use a lookup string: "0123456789ABCDEF"
 *       Then hex_chars[remainder] gives you the right character.
 */
void decimal_to_hex_str(unsigned int value, char result[])
{
    const char hex_chars[] = "0123456789ABCDEF";

    /* TODO: Handle the special case where value is 0 */

    /* TODO: Use a loop to repeatedly divide by 16.
     *       Use hex_chars[remainder] to get the character.
     *       Store each character in result[]. */

    /* TODO: Null-terminate and reverse the string (same idea as binary). */
}

/*
 * binary_str_to_decimal
 *
 * Convert a binary string (e.g., "101010") to an unsigned integer.
 *
 * Parameters:
 *   binary_str - a null-terminated string of '0' and '1' characters
 *
 * Returns:
 *   The decimal value of the binary number.
 *
 * Example:
 *   binary_str_to_decimal("101010")  =>  42
 *   binary_str_to_decimal("0")       =>  0
 *   binary_str_to_decimal("11111111") => 255
 *
 * Algorithm:
 *   Walk through the string left to right. For each character:
 *     result = result * 2 + (current digit)
 *   This works because shifting left in binary is multiplying by 2.
 *
 * Hint: The digit value is (binary_str[i] - '0'), which turns '0' into 0
 *       and '1' into 1.
 */
unsigned int binary_str_to_decimal(const char binary_str[])
{
    unsigned int result = 0;

    /* TODO: Loop through each character in binary_str.
     *       For each character, multiply result by 2 and add the digit. */

    return result;
}

/*
 * hex_str_to_decimal
 *
 * Convert a hexadecimal string (e.g., "FF") to an unsigned integer.
 *
 * Parameters:
 *   hex_str - a null-terminated string of hex characters (0-9, A-F, a-f)
 *
 * Returns:
 *   The decimal value of the hex number.
 *
 * Example:
 *   hex_str_to_decimal("FF")  =>  255
 *   hex_str_to_decimal("2A")  =>  42
 *   hex_str_to_decimal("0")   =>  0
 *
 * Algorithm:
 *   Same idea as binary, but multiply by 16 instead of 2.
 *   You need to handle three ranges of characters:
 *     '0'-'9' => value 0-9
 *     'A'-'F' => value 10-15
 *     'a'-'f' => value 10-15
 *
 * Hint: 'A' - 'A' + 10 = 10,  'F' - 'A' + 10 = 15
 */
unsigned int hex_str_to_decimal(const char hex_str[])
{
    unsigned int result = 0;

    /* TODO: Loop through each character in hex_str.
     *       For each character, figure out its numeric value (0-15),
     *       then do: result = result * 16 + digit_value */

    return result;
}


/* ======================================================================
 * TEST HARNESS — DO NOT MODIFY BELOW THIS LINE
 * ====================================================================== */

static int tests_passed = 0;
static int tests_failed = 0;

static void check_str(const char *label, const char *expected, const char *got)
{
    if (strcmp(expected, got) == 0) {
        printf("  PASS: %s => \"%s\"\n", label, got);
        tests_passed++;
    } else {
        printf("  FAIL: %s => expected \"%s\", got \"%s\"\n",
               label, expected, got);
        tests_failed++;
    }
}

static void check_uint(const char *label, unsigned int expected, unsigned int got)
{
    if (expected == got) {
        printf("  PASS: %s => %u\n", label, got);
        tests_passed++;
    } else {
        printf("  FAIL: %s => expected %u, got %u\n", label, expected, got);
        tests_failed++;
    }
}

int main(void)
{
    char buf[MAX_DIGITS + 1];

    printf("=== Decimal to Binary ===\n");
    decimal_to_binary_str(0, buf);
    check_str("0 -> binary", "0", buf);

    decimal_to_binary_str(1, buf);
    check_str("1 -> binary", "1", buf);

    decimal_to_binary_str(42, buf);
    check_str("42 -> binary", "101010", buf);

    decimal_to_binary_str(255, buf);
    check_str("255 -> binary", "11111111", buf);

    decimal_to_binary_str(1024, buf);
    check_str("1024 -> binary", "10000000000", buf);

    printf("\n=== Decimal to Hex ===\n");
    decimal_to_hex_str(0, buf);
    check_str("0 -> hex", "0", buf);

    decimal_to_hex_str(42, buf);
    check_str("42 -> hex", "2A", buf);

    decimal_to_hex_str(255, buf);
    check_str("255 -> hex", "FF", buf);

    decimal_to_hex_str(1024, buf);
    check_str("1024 -> hex", "400", buf);

    decimal_to_hex_str(65535, buf);
    check_str("65535 -> hex", "FFFF", buf);

    printf("\n=== Binary String to Decimal ===\n");
    check_uint("\"0\" -> decimal", 0, binary_str_to_decimal("0"));
    check_uint("\"1\" -> decimal", 1, binary_str_to_decimal("1"));
    check_uint("\"101010\" -> decimal", 42, binary_str_to_decimal("101010"));
    check_uint("\"11111111\" -> decimal", 255, binary_str_to_decimal("11111111"));
    check_uint("\"10000000000\" -> decimal", 1024, binary_str_to_decimal("10000000000"));

    printf("\n=== Hex String to Decimal ===\n");
    check_uint("\"0\" -> decimal", 0, hex_str_to_decimal("0"));
    check_uint("\"2A\" -> decimal", 42, hex_str_to_decimal("2A"));
    check_uint("\"ff\" -> decimal", 255, hex_str_to_decimal("ff"));
    check_uint("\"FF\" -> decimal", 255, hex_str_to_decimal("FF"));
    check_uint("\"400\" -> decimal", 1024, hex_str_to_decimal("400"));
    check_uint("\"FFFF\" -> decimal", 65535, hex_str_to_decimal("FFFF"));

    printf("\n=== Results ===\n");
    printf("Passed: %d, Failed: %d\n", tests_passed, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
