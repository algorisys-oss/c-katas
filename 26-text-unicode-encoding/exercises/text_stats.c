/*
 * Kata: Text Statistics
 *
 * Build functions that analyse plain text: count words, count lines,
 * convert case, and find the longest word.  All functions work on
 * ASCII characters (letters a-z / A-Z) — they leave non-ASCII bytes
 * untouched, which is safe for UTF-8 strings.
 *
 *   - count_words(str)            — count whitespace-separated words
 *   - count_lines(str)            — count lines
 *   - to_uppercase(str)           — convert a-z to A-Z in place
 *   - to_lowercase(str)           — convert A-Z to a-z in place
 *   - longest_word_length(str)    — length of the longest word (in bytes)
 *
 * 14 tests total.
 *
 * Hints:
 *   - A "word" is a contiguous run of non-whitespace characters.
 *   - Use <ctype.h> functions like isspace() to detect whitespace,
 *     or check for ' ', '\t', '\n', '\r' manually.
 *   - count_lines: count '\n' characters.  An empty string has 0 lines.
 *     A non-empty string with no '\n' has 1 line.
 *   - Case conversion: 'a'-'z' differ from 'A'-'Z' by exactly 32.
 *     Only convert ASCII letters; leave everything else alone.
 */

#include <stdio.h>
#include <string.h>

/* ── count_words ─────────────────────────────────────────────────
 * Count the number of words in a null-terminated string.
 * Words are separated by whitespace (space, tab, newline, etc.).
 * Leading/trailing whitespace does NOT create extra words.
 *
 * Examples:
 *   "hello world"   → 2
 *   "  hello  "     → 1
 *   ""              → 0
 *   "   "           → 0
 *
 * Algorithm:
 *   Walk through the string.  Each time you transition from a
 *   whitespace character to a non-whitespace character, you've
 *   found the start of a new word.
 */
int count_words(const char *str)
{
    /* TODO: track whether you are "in a word" (boolean flag) */
    /* TODO: loop through each character */
    /* TODO: if current char is whitespace and in_word is true, set in_word false */
    /* TODO: if current char is not whitespace and in_word is false, increment count, set in_word true */
    (void)str; /* suppress unused warning — remove when you implement */
    return 0; /* placeholder */
}

/* ── count_lines ─────────────────────────────────────────────────
 * Count the number of lines in a null-terminated string.
 *
 * Rules:
 *   - Empty string → 0 lines
 *   - Non-empty string with no '\n' → 1 line
 *   - Each '\n' that is followed by more text adds a line.
 *   - A trailing '\n' does NOT add an extra line:
 *       "a\nb\n" → 2 lines (not 3)
 *
 * Algorithm:
 *   If the string is empty, return 0.
 *   Otherwise start at 1 and add 1 for every '\n' that is NOT
 *   the last character in the string.
 */
int count_lines(const char *str)
{
    /* TODO: if str[0] == '\0', return 0 */
    /* TODO: start count at 1 */
    /* TODO: for each '\n', increment count */
    (void)str; /* suppress unused warning — remove when you implement */
    return 0; /* placeholder */
}

/* ── to_uppercase ────────────────────────────────────────────────
 * Convert all lowercase ASCII letters (a-z) to uppercase (A-Z)
 * in place.  Leave all other characters (digits, punctuation,
 * non-ASCII bytes) unchanged.
 *
 * The difference between 'a' and 'A' is exactly 32:
 *   'a' = 97,  'A' = 65,  97 - 65 = 32
 *
 * So:  uppercase = ch - 32   (when ch is in 'a'..'z')
 */
void to_uppercase(char *str)
{
    /* TODO: loop through each character */
    /* TODO: if ch >= 'a' && ch <= 'z', subtract 32 */
    (void)str; /* suppress unused warning — remove when you implement */
}

/* ── to_lowercase ────────────────────────────────────────────────
 * Convert all uppercase ASCII letters (A-Z) to lowercase (a-z)
 * in place.  Leave all other characters unchanged.
 *
 * lowercase = ch + 32   (when ch is in 'A'..'Z')
 */
void to_lowercase(char *str)
{
    /* TODO: loop through each character */
    /* TODO: if ch >= 'A' && ch <= 'Z', add 32 */
    (void)str; /* suppress unused warning — remove when you implement */
}

/* ── longest_word_length ─────────────────────────────────────────
 * Find the length (in bytes) of the longest whitespace-separated
 * word in a null-terminated string.
 *
 * Examples:
 *   "hi there world"   → 5 ("world" or "there")
 *   ""                 → 0
 *   "a"                → 1
 *
 * Algorithm:
 *   Track the current word length as you walk the string.
 *   When you hit whitespace (or the end), compare current length
 *   to the max so far.
 */
int longest_word_length(const char *str)
{
    /* TODO: track current_len and max_len */
    /* TODO: for each character: if non-whitespace, current_len++ */
    /* TODO: if whitespace or end of string, compare current_len with max_len */
    /* TODO: reset current_len on whitespace */
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
