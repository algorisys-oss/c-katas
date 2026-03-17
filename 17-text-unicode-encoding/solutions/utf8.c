/*
 * Solution: UTF-8 Encoder/Decoder
 *
 * Implements UTF-8 encoding, decoding, string length (in codepoints),
 * and validation — all from scratch using the bit patterns.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ══════════════════════════════════════════════════════════════════
 *                      UTF-8 BIT PATTERNS
 *
 *  Codepoint range          Byte 1     Byte 2     Byte 3     Byte 4
 *  U+0000  .. U+007F        0xxxxxxx
 *  U+0080  .. U+07FF        110xxxxx   10xxxxxx
 *  U+0800  .. U+FFFF        1110xxxx   10xxxxxx   10xxxxxx
 *  U+10000 .. U+10FFFF      11110xxx   10xxxxxx   10xxxxxx   10xxxxxx
 * ══════════════════════════════════════════════════════════════════ */

/* ── utf8_encode ─────────────────────────────────────────────────
 * Encode a Unicode code point as UTF-8.
 *
 * Parameters:
 *   codepoint — the Unicode code point (e.g., 0x00F1 for ñ)
 *   out       — buffer to write UTF-8 bytes into (must have room for 4)
 *
 * Returns: number of bytes written (1-4), or 0 on invalid codepoint.
 */
int utf8_encode(uint32_t codepoint, unsigned char *out)
{
    if (codepoint <= 0x7F) {
        /* 1-byte: 0xxxxxxx */
        out[0] = (unsigned char)codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        /* 2-byte: 110xxxxx 10xxxxxx */
        out[0] = (unsigned char)(0xC0 | (codepoint >> 6));
        out[1] = (unsigned char)(0x80 | (codepoint & 0x3F));
        return 2;
    } else if (codepoint <= 0xFFFF) {
        /* 3-byte: 1110xxxx 10xxxxxx 10xxxxxx */
        out[0] = (unsigned char)(0xE0 | (codepoint >> 12));
        out[1] = (unsigned char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[2] = (unsigned char)(0x80 | (codepoint & 0x3F));
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        /* 4-byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
        out[0] = (unsigned char)(0xF0 | (codepoint >> 18));
        out[1] = (unsigned char)(0x80 | ((codepoint >> 12) & 0x3F));
        out[2] = (unsigned char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[3] = (unsigned char)(0x80 | (codepoint & 0x3F));
        return 4;
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
 */
int utf8_decode(const unsigned char *in, uint32_t *codepoint)
{
    unsigned char b0 = in[0];

    if (b0 <= 0x7F) {
        /* 1-byte: 0xxxxxxx */
        *codepoint = b0;
        return 1;
    } else if ((b0 & 0xE0) == 0xC0) {
        /* 2-byte: 110xxxxx 10xxxxxx */
        if ((in[1] & 0xC0) != 0x80) return 0;
        *codepoint = ((uint32_t)(b0 & 0x1F) << 6)
                   | ((uint32_t)(in[1] & 0x3F));
        /* Reject overlong: must need 2 bytes (codepoint >= 0x80) */
        if (*codepoint < 0x80) return 0;
        return 2;
    } else if ((b0 & 0xF0) == 0xE0) {
        /* 3-byte: 1110xxxx 10xxxxxx 10xxxxxx */
        if ((in[1] & 0xC0) != 0x80) return 0;
        if ((in[2] & 0xC0) != 0x80) return 0;
        *codepoint = ((uint32_t)(b0 & 0x0F) << 12)
                   | ((uint32_t)(in[1] & 0x3F) << 6)
                   | ((uint32_t)(in[2] & 0x3F));
        /* Reject overlong: must need 3 bytes (codepoint >= 0x800) */
        if (*codepoint < 0x800) return 0;
        return 3;
    } else if ((b0 & 0xF8) == 0xF0) {
        /* 4-byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
        if ((in[1] & 0xC0) != 0x80) return 0;
        if ((in[2] & 0xC0) != 0x80) return 0;
        if ((in[3] & 0xC0) != 0x80) return 0;
        *codepoint = ((uint32_t)(b0 & 0x07) << 18)
                   | ((uint32_t)(in[1] & 0x3F) << 12)
                   | ((uint32_t)(in[2] & 0x3F) << 6)
                   | ((uint32_t)(in[3] & 0x3F));
        /* Reject overlong: must need 4 bytes (codepoint >= 0x10000) */
        if (*codepoint < 0x10000) return 0;
        /* Reject above max Unicode */
        if (*codepoint > 0x10FFFF) return 0;
        return 4;
    }
    return 0; /* invalid lead byte */
}

/* ── utf8_strlen ─────────────────────────────────────────────────
 * Count the number of Unicode code points in a null-terminated
 * UTF-8 string.  This is the "character count", NOT the byte count.
 *
 * Returns: number of codepoints, or -1 if invalid UTF-8 is found.
 */
int utf8_strlen(const unsigned char *s)
{
    int count = 0;
    while (*s) {
        uint32_t cp;
        int bytes = utf8_decode(s, &cp);
        if (bytes == 0) return -1; /* invalid */
        count++;
        s += bytes;
    }
    return count;
}

/* ── utf8_validate ───────────────────────────────────────────────
 * Check if a byte buffer of given length is valid UTF-8.
 *
 * Parameters:
 *   s   — the byte buffer (not necessarily null-terminated)
 *   len — number of bytes to check
 *
 * Returns: 1 if valid, 0 if invalid.
 */
int utf8_validate(const unsigned char *s, int len)
{
    int i = 0;
    while (i < len) {
        unsigned char b0 = s[i];
        int seq_len;

        if (b0 <= 0x7F) {
            seq_len = 1;
        } else if ((b0 & 0xE0) == 0xC0) {
            seq_len = 2;
        } else if ((b0 & 0xF0) == 0xE0) {
            seq_len = 3;
        } else if ((b0 & 0xF8) == 0xF0) {
            seq_len = 4;
        } else {
            return 0; /* invalid lead byte */
        }

        /* Check that we have enough bytes remaining */
        if (i + seq_len > len) return 0;

        /* Validate continuation bytes */
        for (int j = 1; j < seq_len; j++) {
            if ((s[i + j] & 0xC0) != 0x80) return 0;
        }

        /* Decode and check for overlong encodings and range */
        uint32_t cp;
        if (seq_len == 1) {
            cp = b0;
        } else if (seq_len == 2) {
            cp = ((uint32_t)(b0 & 0x1F) << 6)
               | ((uint32_t)(s[i + 1] & 0x3F));
            if (cp < 0x80) return 0;
        } else if (seq_len == 3) {
            cp = ((uint32_t)(b0 & 0x0F) << 12)
               | ((uint32_t)(s[i + 1] & 0x3F) << 6)
               | ((uint32_t)(s[i + 2] & 0x3F));
            if (cp < 0x800) return 0;
        } else {
            cp = ((uint32_t)(b0 & 0x07) << 18)
               | ((uint32_t)(s[i + 1] & 0x3F) << 12)
               | ((uint32_t)(s[i + 2] & 0x3F) << 6)
               | ((uint32_t)(s[i + 3] & 0x3F));
            if (cp < 0x10000) return 0;
            if (cp > 0x10FFFF) return 0;
        }

        i += seq_len;
        (void)cp; /* suppress unused warning for 1-byte case */
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
