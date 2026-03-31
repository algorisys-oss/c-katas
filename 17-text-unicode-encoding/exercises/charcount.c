/*
 * Kata: UTF-8 Character Counter
 *
 * Build functions that correctly count and navigate UTF-8 characters.
 * Remember: in UTF-8, one character can be 1 to 4 bytes.  C's strlen()
 * counts bytes, not characters — so we need our own tools.
 *
 *   - utf8_char_width(first_byte)       — how many bytes does this char use?
 *   - utf8_char_count(str)              — count characters (not bytes)
 *   - utf8_byte_count(str, char_index)  — byte offset of the Nth character
 *   - is_ascii(str)                     — is the entire string pure ASCII?
 *
 * 12 tests total.
 *
 * UTF-8 first-byte patterns:
 *
 *   First byte      Char width
 *   0xxxxxxx        1 byte  (ASCII)
 *   110xxxxx        2 bytes
 *   1110xxxx        3 bytes
 *   11110xxx        4 bytes
 *   10xxxxxx        continuation byte (NOT a valid first byte)
 *
 * Hints:
 *   - Use unsigned char for byte inspection
 *   - Mask the high bits of the first byte to determine the width
 *   - A continuation byte (10xxxxxx) is never a valid start byte
 *   - is_ascii: every byte must be <= 0x7F
 */

#include <stdio.h>
#include <string.h>

/* ── utf8_char_width ─────────────────────────────────────────────
 * Given the first byte of a UTF-8 character, return how many
 * bytes the full character occupies (1-4).
 *
 * Returns: 1, 2, 3, or 4 on valid first byte; 0 on invalid.
 *
 * Algorithm:
 *   Check the high bits of first_byte:
 *   - 0xxxxxxx (0x00-0x7F) → 1
 *   - 110xxxxx (0xC0-0xDF) → 2
 *   - 1110xxxx (0xE0-0xEF) → 3
 *   - 11110xxx (0xF0-0xF7) → 4
 *   - anything else (continuation byte 10xxxxxx) → 0 (invalid)
 */
int utf8_char_width(unsigned char first_byte)
{
    /* TODO: check if first_byte <= 0x7F → return 1 */
    /* TODO: check if (first_byte & 0xE0) == 0xC0 → return 2 */
    /* TODO: check if (first_byte & 0xF0) == 0xE0 → return 3 */
    /* TODO: check if (first_byte & 0xF8) == 0xF0 → return 4 */
    (void)first_byte; /* suppress unused warning — remove when you implement */
    return 0; /* placeholder — invalid first byte */
}

/* ── utf8_char_count ─────────────────────────────────────────────
 * Count the number of Unicode characters in a null-terminated
 * UTF-8 string.  This is NOT the same as strlen() which counts bytes.
 *
 * Returns: character count, or -1 if invalid UTF-8 is encountered.
 *
 * Algorithm:
 *   Walk through the string.  At each position, use utf8_char_width()
 *   to find how many bytes the current character uses.  Advance by
 *   that many bytes.  Count each character.
 */
int utf8_char_count(const char *str)
{
    /* TODO: cast str bytes to unsigned char for inspection */
    /* TODO: loop while current byte is not 0 */
    /* TODO: call utf8_char_width on the current byte */
    /* TODO: if width is 0, return -1 (invalid) */
    /* TODO: increment count, advance pointer by width */
    (void)str; /* suppress unused warning — remove when you implement */
    return 0; /* placeholder */
}

/* ── utf8_byte_count ─────────────────────────────────────────────
 * Return the byte offset of the char_index-th character in a
 * null-terminated UTF-8 string.
 *
 * Parameters:
 *   str        — UTF-8 string
 *   char_index — 0-based character index
 *
 * Returns: byte offset, or -1 if char_index is out of range or
 *          invalid UTF-8 is encountered.
 *
 * Example:
 *   "Hé!" (H=1 byte, é=2 bytes, !=1 byte)
 *   char_index 0 → byte offset 0 (H)
 *   char_index 1 → byte offset 1 (é starts at byte 1)
 *   char_index 2 → byte offset 3 (! starts at byte 3)
 */
int utf8_byte_count(const char *str, int char_index)
{
    /* TODO: walk the string character by character */
    /* TODO: for each character, use utf8_char_width to get its size */
    /* TODO: count characters; when count == char_index, return current offset */
    /* TODO: if you hit the null terminator before reaching char_index, return -1 */
    (void)str; /* suppress unused warning — remove when you implement */
    (void)char_index;
    return -1; /* placeholder */
}

/* ── is_ascii ────────────────────────────────────────────────────
 * Check if a null-terminated string contains only ASCII characters
 * (every byte is 0x00-0x7F).
 *
 * Returns: 1 if pure ASCII, 0 if any byte has the high bit set.
 */
int is_ascii(const char *str)
{
    /* TODO: loop through each byte */
    /* TODO: if any byte has value > 0x7F (i.e., high bit set), return 0 */
    /* TODO: if you reach the end without finding non-ASCII, return 1 */
    (void)str; /* suppress unused warning — remove when you implement */
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
