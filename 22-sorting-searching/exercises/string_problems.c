/*
 * Kata: String Problems
 *
 * Classic string problems that use patterns from earlier exercises:
 * two-pointer reversal, frequency counting, and careful parsing.
 *
 * Exercises:
 *   - reverse_words   (two-pointer + reverse-twice trick)
 *   - is_pangram      (frequency array)
 *   - validate_ipv6   (string parsing)
 *
 * 14 tests total.
 *
 * Hints:
 *   - reverse_words: reverse the ENTIRE string, then reverse each word.
 *     This is the "reverse twice" trick from the two-pointer section.
 *   - is_pangram: use a boolean array of 26 slots, one per letter.
 *   - validate_ipv6: split on ':', check each group has 1-4 hex digits.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ── Exercise 1: Reverse Words in a String ─────────────────────── *
 * Reverse the order of words in a string. Write the result into
 * the output buffer.
 *
 * Also: trim leading/trailing spaces and collapse multiple spaces
 * between words to a single space.
 *
 * The "reverse twice" trick (in-place approach):
 *   1. Copy input to output, trimming/collapsing spaces
 *   2. Reverse the entire string
 *   3. Reverse each individual word
 *
 *   "the sky is blue"
 *    → reverse all: "eulb si yks eht"
 *    → reverse each word: "blue is sky the"
 *
 * Examples:
 *   "the sky is blue"   → "blue is sky the"
 *   "  hello world  "   → "world hello"
 *   "single"            → "single"
 *
 * output buffer is guaranteed to be large enough.
 */
void reverse_words(const char *input, char *output)
{
    /* TODO: Implement using the reverse-twice trick */
    (void)input;
    output[0] = '\0';
}

/* ── Exercise 2: Is Pangram ────────────────────────────────────── *
 * A pangram is a sentence that contains every letter of the alphabet
 * at least once. Return 1 if pangram, 0 otherwise.
 * Case-insensitive.
 *
 * Use a boolean array: int seen[26] = {0};
 * For each letter, mark seen[letter - 'a'] = 1.
 * Then check if all 26 slots are marked.
 *
 * Example:
 *   "The quick brown fox jumps over the lazy dog" → true (has all 26)
 *   "Hello World" → false (missing many letters)
 */
int is_pangram(const char *s)
{
    /* TODO: Use a frequency/boolean array of 26 slots */
    (void)s;
    return 0;
}

/* ── Exercise 3: Validate IPv6 Address ─────────────────────────── *
 * Validate a simplified IPv6 address string.
 * Rules (simplified — no :: shorthand):
 *   - Exactly 8 groups separated by ':'
 *   - Each group has 1-4 hexadecimal digits (0-9, a-f, A-F)
 *   - No leading/trailing colons
 *   - No empty groups
 *
 * Return 1 if valid, 0 if invalid.
 *
 * Examples:
 *   "2001:0db8:85a3:0000:0000:8a2e:0370:7334" → valid
 *   "2001:db8:85a3:0:0:8a2e:370:7334"         → valid (1-4 digits ok)
 *   "2001:db8:85a3::8a2e:370:7334"            → invalid (empty group)
 *   "2001:0db8:85a3:0000:0000:8a2e:0370:733g" → invalid ('g' not hex)
 *   "2001:0db8:85a3:0000:0000:8a2e:0370"      → invalid (only 7 groups)
 */
int validate_ipv6(const char *s)
{
    /* TODO: Parse groups separated by ':', validate each */
    (void)s;
    return 0;
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
    printf("=== String Problems ===\n\n");

    /* --- reverse_words --- */
    printf("reverse_words:\n");
    {
        char out[64];
        reverse_words("the sky is blue", out);
        TEST("\"the sky is blue\"") EXPECT(strcmp(out, "blue is sky the") == 0);
    }
    {
        char out[64];
        reverse_words("  hello world  ", out);
        TEST("trim + reverse") EXPECT(strcmp(out, "world hello") == 0);
    }
    {
        char out[64];
        reverse_words("single", out);
        TEST("single word") EXPECT(strcmp(out, "single") == 0);
    }
    {
        char out[64];
        reverse_words("  a   b   c  ", out);
        TEST("collapse multiple spaces") EXPECT(strcmp(out, "c b a") == 0);
    }

    /* --- is_pangram --- */
    printf("\nis_pangram:\n");
    {
        TEST("classic pangram") EXPECT(is_pangram("The quick brown fox jumps over the lazy dog"));
    }
    {
        TEST("Hello World — not pangram") EXPECT(!is_pangram("Hello World"));
    }
    {
        TEST("all lowercase pangram") EXPECT(is_pangram("abcdefghijklmnopqrstuvwxyz"));
    }
    {
        TEST("missing 'z'") EXPECT(!is_pangram("abcdefghijklmnopqrstuvwxy"));
    }

    /* --- validate_ipv6 --- */
    printf("\nvalidate_ipv6:\n");
    {
        TEST("valid full address") EXPECT(validate_ipv6("2001:0db8:85a3:0000:0000:8a2e:0370:7334"));
    }
    {
        TEST("valid short groups") EXPECT(validate_ipv6("2001:db8:85a3:0:0:8a2e:370:7334"));
    }
    {
        TEST("invalid — empty group (::)") EXPECT(!validate_ipv6("2001:db8:85a3::8a2e:370:7334"));
    }
    {
        TEST("invalid — non-hex char 'g'") EXPECT(!validate_ipv6("2001:0db8:85a3:0000:0000:8a2e:0370:733g"));
    }
    {
        TEST("invalid — only 7 groups") EXPECT(!validate_ipv6("2001:0db8:85a3:0000:0000:8a2e:0370"));
    }
    {
        TEST("invalid — group too long") EXPECT(!validate_ipv6("2001:0db8:85a3:00000:0000:8a2e:0370:7334"));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
