/*
 * Solution: String Problems
 *
 * Three exercises: reverse words, pangram check, IPv6 validation.
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ── Helper: reverse a portion of a char array in-place ─────────── */
static void reverse_chars(char *s, int left, int right)
{
    while (left < right) {
        char tmp = s[left];
        s[left] = s[right];
        s[right] = tmp;
        left++;
        right--;
    }
}

/* ── Reverse Words ─────────────────────────────────────────────── */
void reverse_words(const char *input, char *output)
{
    int len = (int)strlen(input);

    /* Step 1: copy to output, trimming leading/trailing spaces
     * and collapsing multiple spaces to one */
    int out_len = 0;
    int i = 0;

    /* Skip leading spaces */
    while (i < len && input[i] == ' ') {
        i++;
    }

    while (i < len) {
        if (input[i] == ' ') {
            /* Skip extra spaces, add exactly one if not at end */
            while (i < len && input[i] == ' ') {
                i++;
            }
            if (i < len) {
                output[out_len++] = ' ';
            }
        } else {
            output[out_len++] = input[i];
            i++;
        }
    }
    output[out_len] = '\0';

    /* Step 2: reverse the entire string */
    reverse_chars(output, 0, out_len - 1);

    /* Step 3: reverse each individual word */
    int start = 0;
    for (int j = 0; j <= out_len; j++) {
        if (j == out_len || output[j] == ' ') {
            reverse_chars(output, start, j - 1);
            start = j + 1;
        }
    }
}

/* ── Is Pangram ────────────────────────────────────────────────── */
int is_pangram(const char *s)
{
    int seen[26] = {0};
    for (int i = 0; s[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)s[i]);
        if (c >= 'a' && c <= 'z') {
            seen[c - 'a'] = 1;
        }
    }
    for (int i = 0; i < 26; i++) {
        if (!seen[i]) {
            return 0;
        }
    }
    return 1;
}

/* ── Validate IPv6 ─────────────────────────────────────────────── */
static int is_hex_digit(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

int validate_ipv6(const char *s)
{
    int groups = 0;
    int digits_in_group = 0;
    int i = 0;

    while (s[i] != '\0') {
        if (s[i] == ':') {
            /* A group just ended */
            if (digits_in_group == 0) {
                return 0;  /* Empty group */
            }
            if (digits_in_group > 4) {
                return 0;  /* Too many digits */
            }
            groups++;
            digits_in_group = 0;
        } else if (is_hex_digit(s[i])) {
            digits_in_group++;
            if (digits_in_group > 4) {
                return 0;  /* Too many digits in this group */
            }
        } else {
            return 0;  /* Invalid character */
        }
        i++;
    }

    /* Count the last group (no trailing colon) */
    if (digits_in_group == 0) {
        return 0;  /* Trailing colon or empty string */
    }
    groups++;

    return groups == 8;
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
