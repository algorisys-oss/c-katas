/*
 * Kata: Hex Dump
 *
 * Build a hex dump tool that displays data in hex and ASCII,
 * like the real `hexdump -C` command.
 *
 * Output format for each 8-byte row:
 *   OFFSET  HH HH HH HH HH HH HH HH  |ASCII...|
 *
 * Functions to implement:
 *   - is_printable:      check if a byte is printable ASCII
 *   - hexdump_format_row: format one row (up to 8 bytes) into a string
 *   - hexdump_to_string:  format an entire buffer into a string
 *
 * 10 tests total.
 *
 * Hints:
 *   - Use sprintf to build each piece of the output
 *   - Printable ASCII characters are 0x20 (space) through 0x7E (~)
 *   - Non-printable bytes show as '.' in the ASCII column
 *   - Each row shows at most 8 bytes (BYTES_PER_ROW)
 *   - The offset is in hexadecimal, 8 digits wide, zero-padded
 *   - Pad with spaces when a row has fewer than 8 bytes
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_OUTPUT 4096
#define BYTES_PER_ROW 8

/* ── Helper: is_printable ────────────────────────────────────────────
 * Return 1 if the byte is a printable ASCII character (0x20..0x7E),
 * 0 otherwise.
 */
int is_printable(unsigned char c)
{
    /* TODO: check if c is in printable ASCII range */
    return 0; /* placeholder */
}

/* ── Exercise 1: hexdump_format_row ──────────────────────────────────
 * Format one row of hex dump output into `out`.
 *
 * Parameters:
 *   data   - pointer to the bytes for this row
 *   len    - number of bytes in this row (1..8)
 *   offset - the byte offset of this row in the original data
 *   out    - output buffer (must be large enough, ~80 chars)
 *
 * Format: "OOOOOOOO  HH HH HH HH HH HH HH HH  |AAAAAAAA|\n"
 *   - OOOOOOOO = offset in hex, 8 digits, zero-padded
 *   - HH = each byte in hex, 2 digits, zero-padded, lowercase
 *   - If fewer than 8 bytes, pad with spaces (3 spaces per missing byte)
 *   - AAAA = ASCII representation (printable chars or '.')
 *
 * Returns the number of characters written to `out`.
 */
int hexdump_format_row(const unsigned char *data, int len, int offset, char *out)
{
    /* TODO: format the offset */
    /* TODO: format each hex byte with spacing */
    /* TODO: pad remaining space if len < 8 */
    /* TODO: format the ASCII column */
    return 0; /* placeholder */
}

/* ── Exercise 2: hexdump_to_string ───────────────────────────────────
 * Format an entire buffer as a hex dump into `out`.
 *
 * Parameters:
 *   data     - the input data to dump
 *   data_len - number of bytes in data
 *   out      - output buffer
 *   out_size - size of the output buffer
 *
 * Process the data in chunks of BYTES_PER_ROW (8) bytes,
 * calling hexdump_format_row for each chunk.
 *
 * Returns the total number of characters written to `out`.
 */
int hexdump_to_string(const unsigned char *data, int data_len, char *out, int out_size)
{
    /* TODO: loop through data in BYTES_PER_ROW chunks */
    /* TODO: call hexdump_format_row for each chunk */
    /* TODO: accumulate output, respecting out_size */
    return 0; /* placeholder */
}

/* ── Test harness ────────────────────────────────────────────────── */

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
    char out[MAX_OUTPUT];
    int n;

    printf("=== Hex Dump ===\n\n");

    /* --- is_printable tests --- */
    printf("is_printable:\n");

    TEST("printable letter 'A'")
        EXPECT(is_printable('A') == 1);

    TEST("printable space 0x20")
        EXPECT(is_printable(0x20) == 1);

    TEST("non-printable null 0x00")
        EXPECT(is_printable(0x00) == 0);

    TEST("non-printable DEL 0x7F")
        EXPECT(is_printable(0x7F) == 0);

    /* --- hexdump_format_row tests --- */
    printf("\nhexdump_format_row:\n");

    hexdump_format_row((const unsigned char *)"Hello Wo", 8, 0, out);
    TEST("full 8-byte row 'Hello Wo'")
        EXPECT(strcmp(out, "00000000  48 65 6c 6c 6f 20 57 6f  |Hello Wo|\n") == 0);

    hexdump_format_row((const unsigned char *)"\x01\x02\x03", 3, 0x10, out);
    TEST("short row 3 bytes at offset 0x10")
        EXPECT(strcmp(out, "00000010  01 02 03                 |...|\n") == 0);

    hexdump_format_row((const unsigned char *)"\x00\x41\xff\x0a", 4, 0, out);
    TEST("row with non-printable bytes")
        EXPECT(strcmp(out, "00000000  00 41 ff 0a              |.A..|\n") == 0);

    /* --- hexdump_to_string tests --- */
    printf("\nhexdump_to_string:\n");

    n = hexdump_to_string((const unsigned char *)"", 0, out, sizeof(out));
    TEST("empty data produces empty output")
        EXPECT(n == 0 && out[0] == '\0');

    n = hexdump_to_string((const unsigned char *)"Hi", 2, out, sizeof(out));
    TEST("short string 'Hi'")
        EXPECT(n > 0 && strcmp(out, "00000000  48 69                    |Hi|\n") == 0);

    n = hexdump_to_string((const unsigned char *)"ABCDEFGH", 8, out, sizeof(out));
    TEST("exact 8 bytes = one row")
        EXPECT(n > 0 && strcmp(out, "00000000  41 42 43 44 45 46 47 48  |ABCDEFGH|\n") == 0);

    /* --- Summary --- */
    printf("\n%d / %d tests passed\n", tests_passed, tests_total);

    return (tests_passed == tests_total) ? 0 : 1;
}
