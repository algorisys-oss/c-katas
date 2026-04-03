/*
 * Solution: Hex Dump
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_OUTPUT 4096
#define BYTES_PER_ROW 8

/* ── is_printable ────────────────────────────────────────────────── */

int is_printable(unsigned char c)
{
    return (c >= 0x20 && c <= 0x7E) ? 1 : 0;
}

/* ── hexdump_format_row ──────────────────────────────────────────── */

int hexdump_format_row(const unsigned char *data, int len, int offset, char *out)
{
    int pos = 0;
    int i;

    /* Offset: 8 hex digits, zero-padded */
    pos += snprintf(out + pos, 12, "%08x  ", offset);

    /* Hex bytes */
    for (i = 0; i < len; i++) {
        pos += snprintf(out + pos, 4, "%02x ", data[i]);
    }

    /* Pad remaining space if fewer than BYTES_PER_ROW bytes */
    for (i = len; i < BYTES_PER_ROW; i++) {
        pos += snprintf(out + pos, 4, "   ");
    }

    /* ASCII column */
    pos += snprintf(out + pos, 3, " |");
    for (i = 0; i < len; i++) {
        out[pos++] = is_printable(data[i]) ? (char)data[i] : '.';
    }
    pos += snprintf(out + pos, 3, "|\n");

    return pos;
}

/* ── hexdump_to_string ───────────────────────────────────────────── */

int hexdump_to_string(const unsigned char *data, int data_len, char *out, int out_size)
{
    int total = 0;
    int offset = 0;

    out[0] = '\0';

    while (offset < data_len) {
        int chunk = data_len - offset;
        if (chunk > BYTES_PER_ROW) {
            chunk = BYTES_PER_ROW;
        }

        char row[128];
        int row_len = hexdump_format_row(data + offset, chunk, offset, row);

        if (total + row_len >= out_size) {
            break;
        }

        memcpy(out + total, row, row_len);
        total += row_len;
        offset += chunk;
    }

    out[total] = '\0';
    return total;
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
