/*
 * Solution: Text Statistics
 *
 * Analyses plain text: word counting, line counting, case conversion,
 * and longest-word detection.  Works on ASCII characters and is safe
 * for UTF-8 strings (non-ASCII bytes are left untouched).
 */

#include <stdio.h>
#include <string.h>

/* ── helper: is_whitespace ───────────────────────────────────────
 * Returns 1 if ch is a whitespace character, 0 otherwise.
 * We check for space, tab, newline, carriage return, form feed,
 * and vertical tab — the same set as isspace() from <ctype.h>.
 */
static int is_whitespace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n'
        || ch == '\r' || ch == '\f' || ch == '\v';
}

/* ── count_words ─────────────────────────────────────────────────
 * Count whitespace-separated words.  A "word" is a contiguous run
 * of non-whitespace characters.
 */
int count_words(const char *str)
{
    int count = 0;
    int in_word = 0;
    while (*str) {
        if (is_whitespace(*str)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}

/* ── count_lines ─────────────────────────────────────────────────
 * Count lines.  Empty string = 0.  Non-empty string with no '\n' = 1.
 * Each '\n' adds one line.
 */
int count_lines(const char *str)
{
    if (str[0] == '\0') return 0;
    int count = 1;
    const char *p = str;
    while (*p) {
        if (*p == '\n' && *(p + 1) != '\0') count++;
        p++;
    }
    return count;
}

/* ── to_uppercase ────────────────────────────────────────────────
 * Convert a-z to A-Z in place.  Other bytes unchanged.
 */
void to_uppercase(char *str)
{
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32;
        }
        str++;
    }
}

/* ── to_lowercase ────────────────────────────────────────────────
 * Convert A-Z to a-z in place.  Other bytes unchanged.
 */
void to_lowercase(char *str)
{
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 32;
        }
        str++;
    }
}

/* ── longest_word_length ─────────────────────────────────────────
 * Return the byte-length of the longest whitespace-separated word.
 */
int longest_word_length(const char *str)
{
    int max_len = 0;
    int cur_len = 0;
    while (*str) {
        if (is_whitespace(*str)) {
            if (cur_len > max_len) max_len = cur_len;
            cur_len = 0;
        } else {
            cur_len++;
        }
        str++;
    }
    /* Check the last word (string might not end with whitespace) */
    if (cur_len > max_len) max_len = cur_len;
    return max_len;
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
    printf("=== Text Statistics ===\n\n");

    /* ── count_words tests ────────────────────────────────────── */
    printf("count_words:\n");

    TEST("'hello world' -> 2 words")
        EXPECT(count_words("hello world") == 2);

    TEST("'  hello  world  ' -> 2 words (leading/trailing)")
        EXPECT(count_words("  hello  world  ") == 2);

    TEST("'one' -> 1 word")
        EXPECT(count_words("one") == 1);

    TEST("empty string -> 0 words")
        EXPECT(count_words("") == 0);

    TEST("only spaces -> 0 words")
        EXPECT(count_words("   ") == 0);

    TEST("tabs and newlines as separators -> 3 words")
        EXPECT(count_words("one\ttwo\nthree") == 3);

    /* ── count_lines tests ────────────────────────────────────── */
    printf("\ncount_lines:\n");

    TEST("empty string -> 0 lines")
        EXPECT(count_lines("") == 0);

    TEST("'hello' (no newline) -> 1 line")
        EXPECT(count_lines("hello") == 1);

    TEST("'a\\nb\\nc' -> 3 lines")
        EXPECT(count_lines("a\nb\nc") == 3);

    TEST("'a\\nb\\n' (trailing newline) -> 2 lines")
        EXPECT(count_lines("a\nb\n") == 2);

    /* ── to_uppercase tests ───────────────────────────────────── */
    printf("\nto_uppercase:\n");

    {
        char s1[] = "hello";
        to_uppercase(s1);
        TEST("'hello' -> 'HELLO'")
            EXPECT(strcmp(s1, "HELLO") == 0);
    }

    {
        char s2[] = "Hello World 123!";
        to_uppercase(s2);
        TEST("'Hello World 123!' -> 'HELLO WORLD 123!'")
            EXPECT(strcmp(s2, "HELLO WORLD 123!") == 0);
    }

    /* ── to_lowercase tests ───────────────────────────────────── */
    printf("\nto_lowercase:\n");

    {
        char s3[] = "HELLO";
        to_lowercase(s3);
        TEST("'HELLO' -> 'hello'")
            EXPECT(strcmp(s3, "hello") == 0);
    }

    {
        char s4[] = "Hello World 123!";
        to_lowercase(s4);
        TEST("'Hello World 123!' -> 'hello world 123!'")
            EXPECT(strcmp(s4, "hello world 123!") == 0);
    }

    /* ── longest_word_length tests ────────────────────────────── */
    printf("\nlongest_word_length:\n");

    TEST("'hi there world' -> 5")
        EXPECT(longest_word_length("hi there world") == 5);

    TEST("'a' -> 1")
        EXPECT(longest_word_length("a") == 1);

    TEST("empty string -> 0")
        EXPECT(longest_word_length("") == 0);

    TEST("'programming is fun' -> 11")
        EXPECT(longest_word_length("programming is fun") == 11);

    /* ── Summary ──────────────────────────────────────────────── */
    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
