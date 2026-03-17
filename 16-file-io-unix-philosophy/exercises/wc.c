/*
 * Kata: wc (Word Count)
 *
 * Build a wc clone that counts lines, words, and characters in a string.
 *
 * Function to implement:
 *   wc_count(const char *text, int *lines, int *words, int *chars)
 *
 * Definitions:
 *   - chars: total number of bytes in the string (strlen)
 *   - lines: number of newline characters '\n'
 *   - words: sequences of non-whitespace characters separated by whitespace
 *     (whitespace = space, tab, newline, carriage return)
 *
 * 10 tests total.
 *
 * Hints:
 *   - A "word" is any maximal sequence of non-whitespace characters
 *   - Track whether you're currently "in a word" or not
 *   - An empty string has 0 lines, 0 words, 0 chars
 *   - "  hello  " has 1 word (leading/trailing spaces don't create words)
 *   - Use isspace() from <ctype.h> to check for whitespace
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ── Exercise: wc_count ──────────────────────────────────────────────
 * Count lines, words, and characters in the given text.
 *
 * Parameters:
 *   text  - the input string (null-terminated)
 *   lines - pointer to store the line count
 *   words - pointer to store the word count
 *   chars - pointer to store the character count
 *
 * A line is counted for each '\n' character.
 * A word is a maximal sequence of non-whitespace characters.
 * Characters is simply the string length.
 */
void wc_count(const char *text, int *lines, int *words, int *chars)
{
    /* TODO: initialize counts to 0 */
    /* TODO: loop through each character */
    /* TODO: count '\n' for lines */
    /* TODO: track in_word state to count words */
    /* TODO: count total characters */
}

/* ── Test harness ────────────────────────────────────────────────── */

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
    int lines, words, chars;

    printf("=== wc (Word Count) ===\n\n");

    printf("basic counts:\n");

    wc_count("", &lines, &words, &chars);
    TEST("empty string")
        EXPECT(lines == 0 && words == 0 && chars == 0);

    wc_count("hello", &lines, &words, &chars);
    TEST("single word no newline")
        EXPECT(lines == 0 && words == 1 && chars == 5);

    wc_count("hello\n", &lines, &words, &chars);
    TEST("single line with newline")
        EXPECT(lines == 1 && words == 1 && chars == 6);

    wc_count("hello world\n", &lines, &words, &chars);
    TEST("two words on one line")
        EXPECT(lines == 1 && words == 2 && chars == 12);

    wc_count("line one\nline two\nline three\n", &lines, &words, &chars);
    TEST("multiple lines")
        EXPECT(lines == 3 && words == 6 && chars == 29);

    printf("\nedge cases:\n");

    wc_count("   \t\n  \n", &lines, &words, &chars);
    TEST("only whitespace")
        EXPECT(lines == 2 && words == 0 && chars == 8);

    wc_count("  hello  ", &lines, &words, &chars);
    TEST("leading and trailing spaces")
        EXPECT(lines == 0 && words == 1 && chars == 9);

    wc_count("one   two   three", &lines, &words, &chars);
    TEST("multiple spaces between words")
        EXPECT(lines == 0 && words == 3 && chars == 17);

    wc_count("a\tb\tc\n", &lines, &words, &chars);
    TEST("tabs as separators")
        EXPECT(lines == 1 && words == 3 && chars == 6);

    wc_count("first\nsecond", &lines, &words, &chars);
    TEST("no trailing newline multi-line")
        EXPECT(lines == 1 && words == 2 && chars == 12);

    /* --- Summary --- */
    printf("\n%d / %d tests passed\n", tests_passed, tests_total);

    return (tests_passed == tests_total) ? 0 : 1;
}
