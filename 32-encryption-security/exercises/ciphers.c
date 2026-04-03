/*
 * Kata: Classical Ciphers
 *
 * Implement fundamental encryption algorithms:
 *   - Caesar cipher (encrypt, decrypt, and crack)
 *   - XOR cipher (encrypt = decrypt)
 *   - Vigenère cipher (encrypt and decrypt)
 *
 * 14 tests total.
 *
 * Hints:
 *   - Caesar: shift each letter by a fixed amount, wrapping around
 *   - XOR: apply byte-by-byte XOR with a repeating key
 *   - Vigenère: like Caesar, but each letter gets a different shift
 *   - For cracking Caesar, use English letter frequencies (e ≈ 13%, t ≈ 9%, a ≈ 8%)
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ── Exercise 1: caesar_encrypt ───────────────────────────────────
 * Encrypt plaintext using a Caesar cipher with the given shift.
 *
 * - Only shift letters (a-z, A-Z); leave other characters unchanged.
 * - Preserve case: 'A' shifted by 3 → 'D', 'a' shifted by 3 → 'd'.
 * - Shift wraps around: 'Z' shifted by 1 → 'A'.
 * - Write the result into `out` (null-terminated).
 */
void caesar_encrypt(const char *text, int shift, char *out)
{
    /* TODO: Loop through each character of text.
     *       If it's a letter, shift it by `shift` positions (mod 26).
     *       Copy the result to `out`. Null-terminate. */
}

/* ── Exercise 2: caesar_decrypt ───────────────────────────────────
 * Decrypt ciphertext that was encrypted with the given shift.
 *
 * Hint: Decryption is just encryption with (26 - shift).
 */
void caesar_decrypt(const char *text, int shift, char *out)
{
    /* TODO: Decrypt by shifting in the opposite direction. */
}

/* ── Exercise 3: caesar_crack ─────────────────────────────────────
 * Crack a Caesar cipher by brute force with frequency analysis.
 *
 * Strategy:
 *   1. Try all 26 shifts (0-25).
 *   2. For each shift, decrypt the text and score it.
 *   3. Score = count of 'e' + 't' + 'a' (case-insensitive) in decrypted text.
 *   4. The shift with the highest score wins.
 *   5. Write the best decryption into `out`.
 */
void caesar_crack(const char *ciphertext, char *out)
{
    /* TODO: Try all 26 shifts. For each, decrypt into a temp buffer,
     *       count occurrences of 'e', 't', 'a'. Keep the best. */
}

/* ── Exercise 4: xor_encrypt ─────────────────────────────────────
 * XOR-encrypt (or decrypt) `data` of length `len` using `key` of
 * length `key_len`. Write the result into `out`.
 *
 * XOR each byte of data with the corresponding byte of key,
 * cycling the key if data is longer than key.
 *
 *   out[i] = data[i] ^ key[i % key_len]
 *
 * Note: This works on raw bytes, not just text. The same function
 * encrypts AND decrypts (XOR is its own inverse).
 */
void xor_encrypt(const unsigned char *data, int len,
                 const unsigned char *key, int key_len,
                 unsigned char *out)
{
    /* TODO: XOR each byte of data with the repeating key. */
}

/* ── Exercise 5: vigenere_encrypt ─────────────────────────────────
 * Encrypt plaintext using the Vigenère cipher.
 *
 * - Key is a word (e.g., "LEMON"). Use only its letters.
 * - For each letter in text, shift it by the corresponding key letter
 *   (A=0, B=1, ..., Z=25).
 * - Only advance the key position when encrypting a letter.
 * - Non-letters pass through unchanged.
 * - Preserve case. Key is case-insensitive.
 */
void vigenere_encrypt(const char *text, const char *key, char *out)
{
    /* TODO: Loop through text. For each letter, shift by key[ki % key_len].
     *       Only advance ki when you process a letter. */
}

/* ── Exercise 6: vigenere_decrypt ─────────────────────────────────
 * Decrypt ciphertext that was encrypted with the Vigenère cipher.
 *
 * Hint: Subtract the key shift instead of adding it.
 */
void vigenere_decrypt(const char *text, const char *key, char *out)
{
    /* TODO: Same as encrypt, but subtract the key shift (mod 26). */
}


/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
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
    char buf[256];

    printf("=== Classical Ciphers ===\n\n");

    /* --- Caesar encrypt --- */
    printf("-- Caesar Encrypt --\n");

    caesar_encrypt("HELLO", 3, buf);
    TEST("caesar_encrypt(HELLO, 3) → KHOOR") EXPECT(strcmp(buf, "KHOOR") == 0);

    caesar_encrypt("abc xyz", 1, buf);
    TEST("caesar_encrypt(abc xyz, 1) → bcd yza") EXPECT(strcmp(buf, "bcd yza") == 0);

    caesar_encrypt("Zoo!", 25, buf);
    TEST("caesar_encrypt(Zoo!, 25) → Ynn!") EXPECT(strcmp(buf, "Ynn!") == 0);

    /* --- Caesar decrypt --- */
    printf("\n-- Caesar Decrypt --\n");

    caesar_decrypt("KHOOR", 3, buf);
    TEST("caesar_decrypt(KHOOR, 3) → HELLO") EXPECT(strcmp(buf, "HELLO") == 0);

    caesar_encrypt("The quick brown fox", 7, buf);
    char encrypted[256];
    strcpy(encrypted, buf);
    caesar_decrypt(encrypted, 7, buf);
    TEST("encrypt then decrypt round-trip") EXPECT(strcmp(buf, "The quick brown fox") == 0);

    /* --- Caesar crack --- */
    printf("\n-- Caesar Crack --\n");

    caesar_encrypt("the treasure is buried under the old oak tree", 13, encrypted);
    caesar_crack(encrypted, buf);
    TEST("crack shift-13 of 'the treasure...'")
        EXPECT(strcmp(buf, "the treasure is buried under the old oak tree") == 0);

    /* --- XOR cipher --- */
    printf("\n-- XOR Cipher --\n");

    {
        unsigned char plain[] = "Hello, World!";
        unsigned char key[] = {0x4B};
        int len = (int)strlen((char *)plain);
        unsigned char cipher[256];
        unsigned char decrypted[256];

        xor_encrypt(plain, len, key, 1, cipher);
        TEST("XOR encrypt changes data") EXPECT(memcmp(cipher, plain, (size_t)len) != 0);

        xor_encrypt(cipher, len, key, 1, decrypted);
        decrypted[len] = '\0';
        TEST("XOR decrypt recovers original") EXPECT(strcmp((char *)decrypted, "Hello, World!") == 0);
    }

    {
        unsigned char plain[] = "Secret message";
        unsigned char key[] = {0xDE, 0xAD, 0xBE, 0xEF};
        int len = (int)strlen((char *)plain);
        unsigned char cipher[256];
        unsigned char decrypted[256];

        xor_encrypt(plain, len, key, 4, cipher);
        xor_encrypt(cipher, len, key, 4, decrypted);
        decrypted[len] = '\0';
        TEST("XOR multi-byte key round-trip") EXPECT(strcmp((char *)decrypted, "Secret message") == 0);
    }

    /* --- Vigenère encrypt --- */
    printf("\n-- Vigenere Cipher --\n");

    vigenere_encrypt("ATTACKATDAWN", "LEMON", buf);
    TEST("vigenere_encrypt(ATTACKATDAWN, LEMON)") EXPECT(strcmp(buf, "LXFOPVEFRNHR") == 0);

    vigenere_decrypt("LXFOPVEFRNHR", "LEMON", buf);
    TEST("vigenere_decrypt(LXFOPVEFRNHR, LEMON)") EXPECT(strcmp(buf, "ATTACKATDAWN") == 0);

    vigenere_encrypt("Hello, World!", "key", buf);
    char vig_cipher[256];
    strcpy(vig_cipher, buf);
    vigenere_decrypt(vig_cipher, "key", buf);
    TEST("vigenere round-trip with punctuation") EXPECT(strcmp(buf, "Hello, World!") == 0);

    vigenere_encrypt("aaa", "ABC", buf);
    TEST("vigenere_encrypt(aaa, ABC) → abc") EXPECT(strcmp(buf, "abc") == 0);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
