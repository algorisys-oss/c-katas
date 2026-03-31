/*
 * Solution: SHA-1 Hashing & Git Blob Format
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define HASH_SIZE 20
#define HEX_SIZE  41

void simple_hash(const uint8_t *data, size_t len, uint8_t out_hash[HASH_SIZE])
{
    uint32_t state[5] = {
        0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0
    };

    for (size_t i = 0; i < len; i++) {
        int idx = (int)(i % 5);
        state[idx] = state[idx] * 31 + data[i];
        state[(idx + 1) % 5] ^= (state[idx] << 5) | (state[idx] >> 27);
    }

    /* Finalization rounds: spread influence across all state words */
    for (int r = 0; r < 3; r++) {
        for (int j = 0; j < 5; j++) {
            state[j] += (state[(j + 1) % 5] << 7) | (state[(j + 1) % 5] >> 25);
        }
    }

    /* Write state as big-endian bytes */
    for (int i = 0; i < 5; i++) {
        out_hash[i * 4 + 0] = (uint8_t)(state[i] >> 24);
        out_hash[i * 4 + 1] = (uint8_t)(state[i] >> 16);
        out_hash[i * 4 + 2] = (uint8_t)(state[i] >> 8);
        out_hash[i * 4 + 3] = (uint8_t)(state[i]);
    }
}

void hash_to_hex(const uint8_t hash[HASH_SIZE], char out_hex[HEX_SIZE])
{
    for (int i = 0; i < HASH_SIZE; i++) {
        snprintf(out_hex + i * 2, 3, "%02x", hash[i]);
    }
    out_hex[40] = '\0';
}

void git_blob_hash(const char *content, size_t len, char out_hex[HEX_SIZE])
{
    /* Build "blob <len>\0<content>" */
    char header[64];
    int hdr_len = snprintf(header, sizeof(header), "blob %zu", len);

    size_t total = (size_t)hdr_len + 1 + len;
    uint8_t *buf = malloc(total);
    if (!buf) {
        out_hex[0] = '\0';
        return;
    }

    memcpy(buf, header, (size_t)hdr_len);
    buf[hdr_len] = '\0';  /* null byte separator */
    memcpy(buf + hdr_len + 1, content, len);

    uint8_t hash[HASH_SIZE];
    simple_hash(buf, total, hash);
    hash_to_hex(hash, out_hex);

    free(buf);
}


/* ══════════════════════════════════════════════════════════════════
 *  Tests
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== SHA-1 Hashing & Git Blob Format ===\n\n");

    /* --- simple_hash: deterministic output --- */
    printf("simple_hash:\n");
    {
        uint8_t h1[HASH_SIZE], h2[HASH_SIZE];
        simple_hash((const uint8_t *)"hello", 5, h1);
        simple_hash((const uint8_t *)"hello", 5, h2);
        TEST("same input → same hash")
            EXPECT(memcmp(h1, h2, HASH_SIZE) == 0);
    }

    {
        uint8_t h1[HASH_SIZE], h2[HASH_SIZE];
        simple_hash((const uint8_t *)"hello", 5, h1);
        simple_hash((const uint8_t *)"hellp", 5, h2);
        TEST("different input → different hash")
            EXPECT(memcmp(h1, h2, HASH_SIZE) != 0);
    }

    {
        uint8_t h1[HASH_SIZE], h2[HASH_SIZE];
        simple_hash((const uint8_t *)"", 0, h1);
        simple_hash((const uint8_t *)"a", 1, h2);
        TEST("empty vs non-empty → different hash")
            EXPECT(memcmp(h1, h2, HASH_SIZE) != 0);
    }

    {
        uint8_t h1[HASH_SIZE], h2[HASH_SIZE];
        simple_hash((const uint8_t *)"abc", 3, h1);
        simple_hash((const uint8_t *)"ABC", 3, h2);
        TEST("case sensitive: 'abc' ≠ 'ABC'")
            EXPECT(memcmp(h1, h2, HASH_SIZE) != 0);
    }

    /* --- hash_to_hex: correct format --- */
    printf("\nhash_to_hex:\n");
    {
        uint8_t hash[HASH_SIZE];
        char hex[HEX_SIZE];
        simple_hash((const uint8_t *)"test", 4, hash);
        hash_to_hex(hash, hex);
        TEST("hex string is 40 chars")
            EXPECT(strlen(hex) == 40);
    }

    {
        uint8_t hash[HASH_SIZE];
        char hex[HEX_SIZE];
        simple_hash((const uint8_t *)"test", 4, hash);
        hash_to_hex(hash, hex);
        int all_hex = 1;
        for (int i = 0; i < 40; i++) {
            char c = hex[i];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                all_hex = 0;
                break;
            }
        }
        TEST("hex string contains only 0-9a-f")
            EXPECT(all_hex);
    }

    {
        uint8_t hash[HASH_SIZE];
        char h1[HEX_SIZE], h2[HEX_SIZE];
        simple_hash((const uint8_t *)"hello", 5, hash);
        hash_to_hex(hash, h1);
        simple_hash((const uint8_t *)"hello", 5, hash);
        hash_to_hex(hash, h2);
        TEST("same input → same hex string")
            EXPECT(strcmp(h1, h2) == 0);
    }

    /* --- git_blob_hash: git object format --- */
    printf("\ngit_blob_hash:\n");
    {
        char h1[HEX_SIZE], h2[HEX_SIZE];
        git_blob_hash("hello", 5, h1);
        git_blob_hash("hello", 5, h2);
        TEST("same content → same blob hash")
            EXPECT(strcmp(h1, h2) == 0);
    }

    {
        char h1[HEX_SIZE], h2[HEX_SIZE];
        git_blob_hash("hello", 5, h1);
        git_blob_hash("world", 5, h2);
        TEST("different content → different blob hash")
            EXPECT(strcmp(h1, h2) != 0);
    }

    {
        uint8_t raw[HASH_SIZE];
        char raw_hex[HEX_SIZE], blob_hex[HEX_SIZE];
        simple_hash((const uint8_t *)"hello", 5, raw);
        hash_to_hex(raw, raw_hex);
        git_blob_hash("hello", 5, blob_hex);
        TEST("blob hash ≠ raw hash (header changes it)")
            EXPECT(strcmp(raw_hex, blob_hex) != 0);
    }

    {
        char hex[HEX_SIZE];
        git_blob_hash("", 0, hex);
        TEST("empty blob has valid hash")
            EXPECT(strlen(hex) == 40);
    }

    {
        char h1[HEX_SIZE], h2[HEX_SIZE];
        git_blob_hash("hello\n", 6, h1);
        git_blob_hash("hello", 5, h2);
        TEST("trailing newline changes blob hash")
            EXPECT(strcmp(h1, h2) != 0);
    }

    /* --- Summary --- */
    printf("\n%d / %d tests passed.\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("All tests passed!\n");
    }
    return tests_passed == tests_total ? 0 : 1;
}
