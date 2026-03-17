/*
 * base_converter.c — REFERENCE SOLUTION
 *
 * Convert numbers between decimal, binary, and hexadecimal.
 */

#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 64

void decimal_to_binary_str(unsigned int value, char result[])
{
    if (value == 0) {
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    int count = 0;
    while (value > 0) {
        result[count] = '0' + (value % 2);
        value /= 2;
        count++;
    }
    result[count] = '\0';

    /* Reverse the string */
    for (int i = 0, j = count - 1; i < j; i++, j--) {
        char tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
    }
}

void decimal_to_hex_str(unsigned int value, char result[])
{
    const char hex_chars[] = "0123456789ABCDEF";

    if (value == 0) {
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    int count = 0;
    while (value > 0) {
        result[count] = hex_chars[value % 16];
        value /= 16;
        count++;
    }
    result[count] = '\0';

    /* Reverse the string */
    for (int i = 0, j = count - 1; i < j; i++, j--) {
        char tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
    }
}

unsigned int binary_str_to_decimal(const char binary_str[])
{
    unsigned int result = 0;

    for (int i = 0; binary_str[i] != '\0'; i++) {
        result = result * 2 + (binary_str[i] - '0');
    }

    return result;
}

unsigned int hex_str_to_decimal(const char hex_str[])
{
    unsigned int result = 0;

    for (int i = 0; hex_str[i] != '\0'; i++) {
        unsigned int digit;
        char c = hex_str[i];

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else {
            digit = 0;  /* invalid character — skip */
        }

        result = result * 16 + digit;
    }

    return result;
}


/* ======================================================================
 * TEST HARNESS
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
