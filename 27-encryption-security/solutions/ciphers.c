/*
 * Kata: Classical Ciphers — SOLUTION
 *
 * Implement fundamental encryption algorithms:
 *   - Caesar cipher (encrypt, decrypt, and crack)
 *   - XOR cipher (encrypt = decrypt)
 *   - Vigenère cipher (encrypt and decrypt)
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ── Exercise 1: caesar_encrypt ─────────────────────────────────── */
void caesar_encrypt(const char *text, int shift, char *out)
{
    /* Normalize shift to 0-25 range */
    shift = ((shift % 26) + 26) % 26;

    int i;
    for (i = 0; text[i] != '\0'; i++) {
        if (isupper((unsigned char)text[i])) {
            out[i] = (char)('A' + (text[i] - 'A' + shift) % 26);
        } else if (islower((unsigned char)text[i])) {
            out[i] = (char)('a' + (text[i] - 'a' + shift) % 26);
        } else {
            out[i] = text[i];
        }
    }
    out[i] = '\0';
}

/* ── Exercise 2: caesar_decrypt ─────────────────────────────────── */
void caesar_decrypt(const char *text, int shift, char *out)
{
    caesar_encrypt(text, 26 - (shift % 26), out);
}

/* ── Exercise 3: caesar_crack ───────────────────────────────────── */
void caesar_crack(const char *ciphertext, char *out)
{
    char buf[256];
    int best_score = -1;
    int best_shift = 0;

    for (int shift = 0; shift < 26; shift++) {
        caesar_decrypt(ciphertext, shift, buf);

        int score = 0;
        for (int j = 0; buf[j] != '\0'; j++) {
            char c = (char)tolower((unsigned char)buf[j]);
            if (c == 'e' || c == 't' || c == 'a') {
                score++;
            }
        }

        if (score > best_score) {
            best_score = score;
            best_shift = shift;
        }
    }

    caesar_decrypt(ciphertext, best_shift, out);
}

/* ── Exercise 4: xor_encrypt ────────────────────────────────────── */
void xor_encrypt(const unsigned char *data, int len,
                 const unsigned char *key, int key_len,
                 unsigned char *out)
{
    for (int i = 0; i < len; i++) {
        out[i] = data[i] ^ key[i % key_len];
    }
}

/* ── Exercise 5: vigenere_encrypt ───────────────────────────────── */
void vigenere_encrypt(const char *text, const char *key, char *out)
{
    int key_len = (int)strlen(key);
    int ki = 0;
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i])) {
            int shift = toupper((unsigned char)key[ki % key_len]) - 'A';
            if (isupper((unsigned char)text[i])) {
                out[i] = (char)('A' + (text[i] - 'A' + shift) % 26);
            } else {
                out[i] = (char)('a' + (text[i] - 'a' + shift) % 26);
            }
            ki++;
        } else {
            out[i] = text[i];
        }
    }
    out[i] = '\0';
}

/* ── Exercise 6: vigenere_decrypt ───────────────────────────────── */
void vigenere_decrypt(const char *text, const char *key, char *out)
{
    int key_len = (int)strlen(key);
    int ki = 0;
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i])) {
            int shift = toupper((unsigned char)key[ki % key_len]) - 'A';
            if (isupper((unsigned char)text[i])) {
                out[i] = (char)('A' + (text[i] - 'A' - shift + 26) % 26);
            } else {
                out[i] = (char)('a' + (text[i] - 'a' - shift + 26) % 26);
            }
            ki++;
        } else {
            out[i] = text[i];
        }
    }
    out[i] = '\0';
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
