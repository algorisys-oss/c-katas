/*
 * Solution: UTF-8 Character Counter
 *
 * Correctly counts and navigates UTF-8 characters by inspecting
 * the bit patterns of the first byte in each sequence.
 */

#include <stdio.h>
#include <string.h>

/* ── utf8_char_width ─────────────────────────────────────────────
 * Given the first byte of a UTF-8 character, return how many
 * bytes the full character occupies (1-4).
 *
 * Returns: 1, 2, 3, or 4 on valid first byte; 0 on invalid.
 */
int utf8_char_width(unsigned char first_byte)
{
    if (first_byte <= 0x7F) {
        return 1;
    } else if ((first_byte & 0xE0) == 0xC0) {
        return 2;
    } else if ((first_byte & 0xF0) == 0xE0) {
        return 3;
    } else if ((first_byte & 0xF8) == 0xF0) {
        return 4;
    }
    return 0; /* continuation byte or invalid — not a valid start */
}

/* ── utf8_char_count ─────────────────────────────────────────────
 * Count the number of Unicode characters in a null-terminated
 * UTF-8 string.  This is NOT the same as strlen() which counts bytes.
 *
 * Returns: character count, or -1 if invalid UTF-8 is encountered.
 */
int utf8_char_count(const char *str)
{
    const unsigned char *s = (const unsigned char *)str;
    int count = 0;
    while (*s) {
        int width = utf8_char_width(*s);
        if (width == 0) return -1; /* invalid start byte */
        count++;
        s += width;
    }
    return count;
}

/* ── utf8_byte_count ─────────────────────────────────────────────
 * Return the byte offset of the char_index-th character in a
 * null-terminated UTF-8 string.
 *
 * Returns: byte offset, or -1 if char_index is out of range or
 *          invalid UTF-8 is encountered.
 */
int utf8_byte_count(const char *str, int char_index)
{
    const unsigned char *s = (const unsigned char *)str;
    int offset = 0;
    int i = 0;
    while (*s) {
        if (i == char_index) return offset;
        int width = utf8_char_width(*s);
        if (width == 0) return -1; /* invalid */
        offset += width;
        s += width;
        i++;
    }
    /* char_index might point exactly at the end (one past last char) */
    if (i == char_index) return offset;
    return -1; /* out of range */
}

/* ── is_ascii ────────────────────────────────────────────────────
 * Check if a null-terminated string contains only ASCII characters
 * (every byte is 0x00-0x7F).
 *
 * Returns: 1 if pure ASCII, 0 if any byte has the high bit set.
 */
int is_ascii(const char *str)
{
    const unsigned char *s = (const unsigned char *)str;
    while (*s) {
        if (*s > 0x7F) return 0;
        s++;
    }
    return 1;
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
    printf("=== UTF-8 Character Counter ===\n\n");

    /* ── utf8_char_width tests ────────────────────────────────── */
    printf("utf8_char_width:\n");

    TEST("ASCII byte 0x41 ('A') -> width 1")
        EXPECT(utf8_char_width(0x41) == 1);

    TEST("2-byte lead 0xC3 -> width 2")
        EXPECT(utf8_char_width(0xC3) == 2);

    TEST("3-byte lead 0xE4 -> width 3")
        EXPECT(utf8_char_width(0xE4) == 3);

    TEST("4-byte lead 0xF0 -> width 4")
        EXPECT(utf8_char_width(0xF0) == 4);

    TEST("continuation byte 0x80 -> invalid (0)")
        EXPECT(utf8_char_width(0x80) == 0);

    /* ── utf8_char_count tests ────────────────────────────────── */
    printf("\nutf8_char_count:\n");

    /* Pure ASCII */
    TEST("'Hello' -> 5 characters")
        EXPECT(utf8_char_count("Hello") == 5);

    /* 2-byte chars: "éñ" = 0xC3 0xA9, 0xC3 0xB1 (4 bytes, 2 chars) */
    {
        char s[] = {(char)0xC3, (char)0xA9, (char)0xC3, (char)0xB1, 0x00};
        TEST("2-byte chars (4 bytes, 2 chars)")
            EXPECT(utf8_char_count(s) == 2);
    }

    /* 3-byte chars: "中€" = E4 B8 AD, E2 82 AC (6 bytes, 2 chars) */
    {
        char s[] = {(char)0xE4, (char)0xB8, (char)0xAD,
                    (char)0xE2, (char)0x82, (char)0xAC, 0x00};
        TEST("3-byte chars (6 bytes, 2 chars)")
            EXPECT(utf8_char_count(s) == 2);
    }

    /* 4-byte char: U+1F600 = F0 9F 98 80 (4 bytes, 1 char) */
    {
        char s[] = {(char)0xF0, (char)0x9F, (char)0x98, (char)0x80, 0x00};
        TEST("4-byte emoji (4 bytes, 1 char)")
            EXPECT(utf8_char_count(s) == 1);
    }

    /* Mixed: "Hé世!" = H(1) + é(2) + 世(3) + !(1) = 7 bytes, 4 chars */
    {
        char s[] = {'H',
                    (char)0xC3, (char)0xA9,
                    (char)0xE4, (char)0xB8, (char)0x96,
                    '!', 0x00};
        TEST("mixed ASCII/2-byte/3-byte (7 bytes, 4 chars)")
            EXPECT(utf8_char_count(s) == 4);
    }

    /* Empty string */
    TEST("empty string -> 0 characters")
        EXPECT(utf8_char_count("") == 0);

    /* ── utf8_byte_count tests ────────────────────────────────── */
    printf("\nutf8_byte_count:\n");

    /* "Hé!" — H at byte 0, é at byte 1, ! at byte 3 */
    {
        char s[] = {'H', (char)0xC3, (char)0xA9, '!', 0x00};
        TEST("'H(e-acute)!' char 0 -> byte 0")
            EXPECT(utf8_byte_count(s, 0) == 0);

        TEST("'H(e-acute)!' char 1 -> byte 1")
            EXPECT(utf8_byte_count(s, 1) == 1);

        TEST("'H(e-acute)!' char 2 -> byte 3")
            EXPECT(utf8_byte_count(s, 2) == 3);

        TEST("'H(e-acute)!' char 5 -> out of range (-1)")
            EXPECT(utf8_byte_count(s, 5) == -1);
    }

    /* ── is_ascii tests ───────────────────────────────────────── */
    printf("\nis_ascii:\n");

    TEST("'Hello' is ASCII")
        EXPECT(is_ascii("Hello") == 1);

    {
        char s[] = {(char)0xC3, (char)0xA9, 0x00};
        TEST("UTF-8 string is NOT ASCII")
            EXPECT(is_ascii(s) == 0);
    }

    TEST("empty string is ASCII")
        EXPECT(is_ascii("") == 1);

    /* ── Summary ──────────────────────────────────────────────── */
    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
