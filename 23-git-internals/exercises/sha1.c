/*
 * Kata: SHA-1 Hashing & Git Blob Format
 *
 * Learn how content-addressable storage works by implementing:
 *   - A simple hash function (educational, demonstrates the concept)
 *   - Hex string conversion
 *   - Git's blob hashing format ("blob <size>\0<content>")
 *
 * Git identifies every object by hashing its content. Same content
 * always produces the same hash. This is the foundation of Git's
 * entire data model.
 *
 * 12 tests total.
 *
 * Hints:
 *   - Git blob format: "blob <decimal_length>\0<content>"
 *   - The \0 is a literal null byte, not two characters
 *   - sprintf() can format "blob %zu" for the header
 *   - Each byte of hash becomes two hex characters (use "%02x")
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define HASH_SIZE 20   /* 160-bit hash → 20 bytes */
#define HEX_SIZE  41   /* 40 hex chars + null terminator */

/* ── Exercise 1: simple_hash ──────────────────────────────────────
 * Compute a 160-bit (20-byte) hash of arbitrary data.
 *
 * This is a simplified hash for educational purposes — NOT
 * cryptographically secure, but it demonstrates the key properties:
 *   1. Deterministic: same input → same output
 *   2. Avalanche: small input change → big output change
 *   3. Fixed size: any input length → exactly 20 bytes out
 *
 * Algorithm (simplified hash based on mixing):
 *   - Initialize state[0..4] with five seed values:
 *       0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0
 *     (These are the same initial values SHA-1 uses)
 *   - For each byte of input data[i]:
 *       - Pick a state slot: idx = i % 5
 *       - Mix: state[idx] = state[idx] * 31 + data[i]
 *       - Cross-mix with neighbor: state[(idx+1) % 5] ^= (state[idx] << 5) | (state[idx] >> 27)
 *   - Run 3 finalization rounds over all 5 state words:
 *       for (r = 0; r < 3; r++)
 *         for (j = 0; j < 5; j++)
 *           state[j] += (state[(j+1)%5] << 7) | (state[(j+1)%5] >> 25)
 *   - Write state[0..4] to out_hash as 20 bytes in big-endian order:
 *       for each state[i], write bytes: (state[i]>>24), (state[i]>>16)&0xFF,
 *                                       (state[i]>>8)&0xFF, state[i]&0xFF
 *
 * Parameters:
 *   data     — pointer to input bytes
 *   len      — number of input bytes
 *   out_hash — 20-byte output buffer
 */
void simple_hash(const uint8_t *data, size_t len, uint8_t out_hash[HASH_SIZE])
{
    /* TODO: Implement the simplified hash algorithm described above */
    (void)data; (void)len; (void)out_hash;
}

/* ── Exercise 2: hash_to_hex ──────────────────────────────────────
 * Convert a 20-byte hash to a 40-character hexadecimal string.
 *
 * Example: byte 0xAF → "af"
 *
 * Use sprintf(out + i*2, "%02x", hash[i]) for each byte.
 * Don't forget to null-terminate the string.
 *
 * Parameters:
 *   hash    — 20-byte hash
 *   out_hex — 41-byte output buffer (40 hex chars + '\0')
 */
void hash_to_hex(const uint8_t hash[HASH_SIZE], char out_hex[HEX_SIZE])
{
    /* TODO: Convert each byte to two hex characters */
    (void)hash; (void)out_hex;
}

/* ── Exercise 3: git_blob_hash ────────────────────────────────────
 * Compute the hash of a git blob object.
 *
 * Git doesn't just hash the file content — it prepends a header:
 *   "blob <content_length>\0<content>"
 *
 * For example, the string "hello" (5 bytes) becomes:
 *   "blob 5\0hello"
 * which is then hashed.
 *
 * Steps:
 *   1. Create header: sprintf(header, "blob %zu", content_len)
 *   2. Build full blob: header + '\0' + content
 *      (use a buffer large enough, or compute in parts)
 *   3. Hash the full blob with simple_hash()
 *   4. Convert to hex with hash_to_hex()
 *
 * Parameters:
 *   content  — file content bytes
 *   len      — content length
 *   out_hex  — 41-byte output buffer for hex hash string
 */
void git_blob_hash(const char *content, size_t len, char out_hex[HEX_SIZE])
{
    /* TODO: Build "blob <len>\0<content>" and hash it */
    (void)content; (void)len; (void)out_hex;
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
        /* blob hash differs from raw hash because of "blob <len>\0" prefix */
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
