/*
 * Kata: UTF-8 Encoder/Decoder
 *
 * Build a UTF-8 encoder and decoder from scratch using the bit patterns.
 *
 *   - utf8_encode(codepoint, out)    — encode a Unicode code point as UTF-8 bytes
 *   - utf8_decode(in, codepoint)     — decode one UTF-8 character to a code point
 *   - utf8_strlen(s)                 — count codepoints (not bytes!) in a string
 *   - utf8_validate(s, len)          — check if a byte buffer is valid UTF-8
 *
 * 16 tests total.
 *
 * UTF-8 bit patterns:
 *
 *   Codepoint range          Byte 1     Byte 2     Byte 3     Byte 4
 *   U+0000  .. U+007F        0xxxxxxx
 *   U+0080  .. U+07FF        110xxxxx   10xxxxxx
 *   U+0800  .. U+FFFF        1110xxxx   10xxxxxx   10xxxxxx
 *   U+10000 .. U+10FFFF      11110xxx   10xxxxxx   10xxxxxx   10xxxxxx
 *
 * Hints:
 *   - Use unsigned char for all byte manipulation
 *   - Use bitwise AND (&) to mask bits, OR (|) to set tag bits
 *   - Use right shift (>>) to extract high bits
 *   - The number of leading 1s in the first byte tells you the byte count
 *   - Continuation bytes always match the pattern 10xxxxxx (mask: 0xC0 == 0x80)
 *   - Reject overlong encodings (e.g., 2 bytes for a value < 0x80)
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ── utf8_encode ─────────────────────────────────────────────────
 * Encode a Unicode code point as UTF-8.
 *
 * Parameters:
 *   codepoint — the Unicode code point (e.g., 0x00F1 for ñ)
 *   out       — buffer to write UTF-8 bytes into (must have room for 4)
 *
 * Returns: number of bytes written (1-4), or 0 on invalid codepoint.
 *
 * Algorithm:
 *   1. Determine the range the codepoint falls in
 *   2. Use the corresponding bit pattern
 *   3. Shift and mask the codepoint bits into the pattern
 */
int utf8_encode(uint32_t codepoint, unsigned char *out)
{
    (void)out; /* suppress unused warning — remove when you implement */
    if (codepoint <= 0x7F) {
        /* TODO: 1-byte encoding */
        /* The byte is just the codepoint value directly */
        return 0; /* placeholder */
    } else if (codepoint <= 0x7FF) {
        /* TODO: 2-byte encoding */
        /* Byte 1: 0xC0 | (codepoint >> 6) */
        /* Byte 2: 0x80 | (codepoint & 0x3F) */
        return 0; /* placeholder */
    } else if (codepoint <= 0xFFFF) {
        /* TODO: 3-byte encoding */
        /* Byte 1: 0xE0 | (codepoint >> 12) */
        /* Byte 2: 0x80 | ((codepoint >> 6) & 0x3F) */
        /* Byte 3: 0x80 | (codepoint & 0x3F) */
        return 0; /* placeholder */
    } else if (codepoint <= 0x10FFFF) {
        /* TODO: 4-byte encoding */
        /* Byte 1: 0xF0 | (codepoint >> 18) */
        /* Byte 2: 0x80 | ((codepoint >> 12) & 0x3F) */
        /* Byte 3: 0x80 | ((codepoint >> 6) & 0x3F) */
        /* Byte 4: 0x80 | (codepoint & 0x3F) */
        return 0; /* placeholder */
    }
    return 0; /* invalid codepoint (above U+10FFFF) */
}

/* ── utf8_decode ─────────────────────────────────────────────────
 * Decode one UTF-8 character from a byte stream.
 *
 * Parameters:
 *   in        — pointer to UTF-8 bytes
 *   codepoint — output: the decoded code point
 *
 * Returns: number of bytes consumed (1-4), or 0 on invalid sequence.
 *
 * Algorithm:
 *   1. Look at the first byte to determine the sequence length
 *   2. Check that continuation bytes start with 10xxxxxx
 *   3. Extract and combine the data bits
 *   4. Reject overlong encodings
 */
int utf8_decode(const unsigned char *in, uint32_t *codepoint)
{
    unsigned char b0 = in[0];
    (void)codepoint; /* suppress unused warning — remove when you implement */

    if (b0 <= 0x7F) {
        /* TODO: 1-byte character */
        /* The codepoint is just the byte value */
        return 0; /* placeholder */
    } else if ((b0 & 0xE0) == 0xC0) {
        /* TODO: 2-byte character */
        /* Check that in[1] is a valid continuation byte: (in[1] & 0xC0) == 0x80 */
        /* Combine: ((b0 & 0x1F) << 6) | (in[1] & 0x3F) */
        /* Reject overlong: result must be >= 0x80 */
        return 0; /* placeholder */
    } else if ((b0 & 0xF0) == 0xE0) {
        /* TODO: 3-byte character */
        /* Check continuation bytes in[1] and in[2] */
        /* Combine: ((b0 & 0x0F) << 12) | ((in[1] & 0x3F) << 6) | (in[2] & 0x3F) */
        /* Reject overlong: result must be >= 0x800 */
        return 0; /* placeholder */
    } else if ((b0 & 0xF8) == 0xF0) {
        /* TODO: 4-byte character */
        /* Check continuation bytes in[1], in[2], in[3] */
        /* Combine all four bytes' data bits */
        /* Reject overlong: result must be >= 0x10000 */
        /* Reject above max: result must be <= 0x10FFFF */
        return 0; /* placeholder */
    }
    return 0; /* invalid lead byte */
}

/* ── utf8_strlen ─────────────────────────────────────────────────
 * Count the number of Unicode code points in a null-terminated
 * UTF-8 string.  This is the "character count", NOT the byte count.
 *
 * Returns: number of codepoints, or -1 if invalid UTF-8 is found.
 *
 * Algorithm:
 *   Walk through the string, decoding one character at a time.
 *   Count each successful decode.  Stop at the null terminator.
 */
int utf8_strlen(const unsigned char *s)
{
    /* TODO: loop while *s is not 0 */
    /* TODO: call utf8_decode to get the byte length of the current character */
    /* TODO: if decode returns 0, return -1 (invalid) */
    /* TODO: increment count, advance s by the decoded byte length */
    (void)s; /* suppress unused warning — remove when you implement */
    return 0; /* placeholder */
}

/* ── utf8_validate ───────────────────────────────────────────────
 * Check if a byte buffer of given length is valid UTF-8.
 *
 * Parameters:
 *   s   — the byte buffer (not necessarily null-terminated)
 *   len — number of bytes to check
 *
 * Returns: 1 if valid, 0 if invalid.
 *
 * Must check:
 *   - Valid lead bytes (correct bit pattern)
 *   - Correct number of continuation bytes
 *   - Continuation bytes match 10xxxxxx
 *   - No overlong encodings
 *   - No codepoints above U+10FFFF
 *   - No truncated sequences (enough bytes remaining)
 */
int utf8_validate(const unsigned char *s, int len)
{
    /* TODO: walk through the buffer byte by byte (index i from 0 to len) */
    /* TODO: determine expected sequence length from the lead byte */
    /* TODO: check that i + seq_len <= len (not truncated) */
    /* TODO: check each continuation byte */
    /* TODO: decode and check for overlong / out-of-range */
    (void)s; /* suppress unused warning — remove when you implement */
    (void)len;
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
    printf("=== UTF-8 Encoder/Decoder ===\n\n");

    unsigned char buf[8];
    uint32_t cp;
    int n;

    /* ── Encode tests ─────────────────────────────────────────── */
    printf("encode:\n");

    /* 1-byte: ASCII 'A' = U+0041 */
    TEST("encode U+0041 (A) -> 1 byte")
        n = utf8_encode(0x0041, buf);
        EXPECT(n == 1 && buf[0] == 0x41);

    /* 2-byte: ñ = U+00F1 -> 0xC3 0xB1 */
    TEST("encode U+00F1 (n with tilde) -> 2 bytes")
        n = utf8_encode(0x00F1, buf);
        EXPECT(n == 2 && buf[0] == 0xC3 && buf[1] == 0xB1);

    /* 3-byte: 世 = U+4E16 -> 0xE4 0xB8 0x96 */
    TEST("encode U+4E16 (CJK 'world') -> 3 bytes")
        n = utf8_encode(0x4E16, buf);
        EXPECT(n == 3 && buf[0] == 0xE4 && buf[1] == 0xB8 && buf[2] == 0x96);

    /* 4-byte: 😀 = U+1F600 -> 0xF0 0x9F 0x98 0x80 */
    TEST("encode U+1F600 (grinning face) -> 4 bytes")
        n = utf8_encode(0x1F600, buf);
        EXPECT(n == 4 && buf[0] == 0xF0 && buf[1] == 0x9F
                      && buf[2] == 0x98 && buf[3] == 0x80);

    /* Invalid: above max Unicode */
    TEST("encode U+110000 (invalid) -> 0")
        n = utf8_encode(0x110000, buf);
        EXPECT(n == 0);

    /* ── Decode tests ─────────────────────────────────────────── */
    printf("\ndecode:\n");

    /* 1-byte: 0x41 -> U+0041 */
    {
        unsigned char ascii_a[] = {0x41};
        TEST("decode 0x41 -> U+0041 (A)")
            n = utf8_decode(ascii_a, &cp);
            EXPECT(n == 1 && cp == 0x0041);
    }

    /* 2-byte: 0xC3 0xB1 -> U+00F1 */
    {
        unsigned char ntilde[] = {0xC3, 0xB1};
        TEST("decode 0xC3 0xB1 -> U+00F1 (n with tilde)")
            n = utf8_decode(ntilde, &cp);
            EXPECT(n == 2 && cp == 0x00F1);
    }

    /* 3-byte: 0xE4 0xB8 0x96 -> U+4E16 */
    {
        unsigned char shi[] = {0xE4, 0xB8, 0x96};
        TEST("decode 0xE4 0xB8 0x96 -> U+4E16 (CJK 'world')")
            n = utf8_decode(shi, &cp);
            EXPECT(n == 3 && cp == 0x4E16);
    }

    /* 4-byte: 0xF0 0x9F 0x98 0x80 -> U+1F600 */
    {
        unsigned char grin[] = {0xF0, 0x9F, 0x98, 0x80};
        TEST("decode 0xF0 0x9F 0x98 0x80 -> U+1F600 (grinning)")
            n = utf8_decode(grin, &cp);
            EXPECT(n == 4 && cp == 0x1F600);
    }

    /* ── Round-trip tests ─────────────────────────────────────── */
    printf("\nround-trip:\n");

    {
        unsigned char rt_buf[8];
        uint32_t rt_cp;
        int enc_len, dec_len;

        TEST("round-trip U+00E9 (e with acute)")
            enc_len = utf8_encode(0x00E9, rt_buf);
            dec_len = utf8_decode(rt_buf, &rt_cp);
            EXPECT(enc_len == 2 && dec_len == 2 && rt_cp == 0x00E9);

        TEST("round-trip U+1F4A9 (pile of poo)")
            enc_len = utf8_encode(0x1F4A9, rt_buf);
            dec_len = utf8_decode(rt_buf, &rt_cp);
            EXPECT(enc_len == 4 && dec_len == 4 && rt_cp == 0x1F4A9);
    }

    /* ── utf8_strlen tests ────────────────────────────────────── */
    printf("\nutf8_strlen:\n");

    /* Pure ASCII */
    {
        unsigned char hello[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x00}; /* "Hello" */
        TEST("strlen 'Hello' = 5 codepoints")
            EXPECT(utf8_strlen(hello) == 5);
    }

    /* Mixed: "Hñ世" = 1 + 2 + 3 = 6 bytes, 3 codepoints */
    {
        unsigned char mixed[] = {0x48, 0xC3, 0xB1, 0xE4, 0xB8, 0x96, 0x00};
        int byte_len = (int)strlen((const char *)mixed);
        TEST("strlen mixed (6 bytes, 3 codepoints)")
            EXPECT(byte_len == 6 && utf8_strlen(mixed) == 3);
    }

    /* ── utf8_validate tests ──────────────────────────────────── */
    printf("\nutf8_validate:\n");

    /* Valid ASCII */
    {
        unsigned char valid_ascii[] = {0x48, 0x69, 0x21}; /* "Hi!" */
        TEST("validate valid ASCII")
            EXPECT(utf8_validate(valid_ascii, 3) == 1);
    }

    /* Valid multi-byte */
    {
        unsigned char valid_multi[] = {0xC3, 0xB1, 0xE4, 0xB8, 0x96};
        TEST("validate valid multi-byte sequence")
            EXPECT(utf8_validate(valid_multi, 5) == 1);
    }

    /* Invalid: bare continuation byte */
    {
        unsigned char bad_cont[] = {0x80};
        TEST("reject bare continuation byte 0x80")
            EXPECT(utf8_validate(bad_cont, 1) == 0);
    }

    /* Invalid: truncated 2-byte sequence */
    {
        unsigned char trunc2[] = {0xC3};
        TEST("reject truncated 2-byte sequence")
            EXPECT(utf8_validate(trunc2, 1) == 0);
    }

    /* ── Summary ──────────────────────────────────────────────── */
    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
