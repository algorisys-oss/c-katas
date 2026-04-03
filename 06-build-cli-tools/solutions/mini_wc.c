/*
 * mini_wc.c — A simplified clone of the Unix `wc` (word count) tool
 *
 * Usage:
 *   ./mini_wc [file]         Count lines, words, and characters
 *   ./mini_wc --test         Run the built-in test suite
 *
 * If no file is given, reads from stdin.
 *
 * Output format (matches real wc):
 *   <lines> <words> <chars> <filename>
 *
 * What you will learn:
 *   - Reading from a file or stdin using the same loop
 *   - Parsing argc/argv to get the filename
 *   - Counting things character by character
 *   - The "in a word / not in a word" state machine pattern
 *
 * Hints:
 *   - A "word" is any sequence of non-whitespace characters.
 *   - Use isspace() from <ctype.h> to check for whitespace.
 *   - Track whether you are currently "in a word" using a flag.
 *     When you transition from whitespace to non-whitespace, that is a new word.
 *   - A "line" is counted by counting '\n' characters.
 *   - Every character you read counts toward the char total (including '\n').
 */

#define _GNU_SOURCE  /* for fmemopen */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -- Test Harness --------------------------------------------------------- */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* -- Data Structure ------------------------------------------------------- */

typedef struct {
    int lines;
    int words;
    int chars;
} wc_result;

/* -- Core Functions ------------------------------------------------------- */

/*
 * count_stream — Count lines, words, and characters from an open FILE.
 *
 * Read the stream character by character using fgetc().
 * Track three counters and use a boolean flag (in_word) to detect
 * word boundaries.
 *
 * Algorithm:
 *   in_word = 0
 *   for each character c:
 *     chars++
 *     if c == '\n': lines++
 *     if c is whitespace:
 *       in_word = 0
 *     else:
 *       if not in_word: words++, in_word = 1
 */
wc_result count_stream(FILE *fp) {
    wc_result result = {0, 0, 0};

    int c;
    int in_word = 0;
    while ((c = fgetc(fp)) != EOF) {
        result.chars++;
        if (c == '\n') result.lines++;
        if (isspace(c)) {
            in_word = 0;
        } else {
            if (!in_word) { result.words++; in_word = 1; }
        }
    }

    return result;
}

/*
 * count_string — Count lines, words, and characters in a string.
 *
 * This is a helper for testing. It wraps the string in a temporary file
 * stream using fmemopen (a POSIX function that creates a FILE* from a
 * memory buffer).
 */
wc_result count_string(const char *text) {
    size_t len = strlen(text);
    if (len == 0) {
        wc_result empty = {0, 0, 0};
        return empty;
    }
    FILE *fp = fmemopen((void *)text, len, "r");
    if (fp == NULL) {
        wc_result empty = {0, 0, 0};
        return empty;
    }
    wc_result result = count_stream(fp);
    fclose(fp);
    return result;
}

/*
 * print_result — Print the counts in wc format.
 *
 * Format: "%8d %8d %8d %s\n"
 * If filename is NULL, omit it (stdin mode).
 */
void print_result(wc_result r, const char *filename) {
    if (filename != NULL)
        printf("%8d %8d %8d %s\n", r.lines, r.words, r.chars, filename);
    else
        printf("%8d %8d %8d\n", r.lines, r.words, r.chars);
}

/* -- Test Suite ----------------------------------------------------------- */

static void run_tests(void) {
    printf("mini_wc tests:\n");

    /* Test 1: Empty string */
    TEST("empty string has zero counts");
        wc_result r1 = count_string("");
    EXPECT(r1.lines == 0 && r1.words == 0 && r1.chars == 0);

    /* Test 2: Single word, no newline */
    TEST("single word without newline");
        wc_result r2 = count_string("hello");
    EXPECT(r2.lines == 0 && r2.words == 1 && r2.chars == 5);

    /* Test 3: Single line with newline */
    TEST("single line ending with newline");
        wc_result r3 = count_string("hello world\n");
    EXPECT(r3.lines == 1 && r3.words == 2 && r3.chars == 12);

    /* Test 4: Multiple lines */
    TEST("multiple lines");
        wc_result r4 = count_string("one\ntwo\nthree\n");
    EXPECT(r4.lines == 3 && r4.words == 3 && r4.chars == 14);

    /* Test 5: Only whitespace */
    TEST("only whitespace characters");
        wc_result r5 = count_string("   \n  \n\n");
    EXPECT(r5.lines == 3 && r5.words == 0 && r5.chars == 8);

    /* Test 6: Multiple words per line */
    TEST("multiple words on multiple lines");
        wc_result r6 = count_string("the quick brown\nfox jumps over\n");
    EXPECT(r6.lines == 2 && r6.words == 6 && r6.chars == 31);

    /* Test 7: Tabs and mixed whitespace */
    TEST("tabs count as whitespace word separators");
        wc_result r7 = count_string("one\ttwo\tthree\n");
    EXPECT(r7.lines == 1 && r7.words == 3 && r7.chars == 14);

    /* Test 8: Leading and trailing whitespace */
    TEST("leading and trailing spaces");
        wc_result r8 = count_string("  hello  \n");
    EXPECT(r8.lines == 1 && r8.words == 1 && r8.chars == 10);

    /* Test 9: Single character */
    TEST("single character no newline");
        wc_result r9 = count_string("x");
    EXPECT(r9.lines == 0 && r9.words == 1 && r9.chars == 1);

    /* Test 10: Only newlines */
    TEST("only newlines count as lines with zero words");
        wc_result r10 = count_string("\n\n\n");
    EXPECT(r10.lines == 3 && r10.words == 0 && r10.chars == 3);

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
}

/* -- Main ----------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    /* Run tests if --test flag is given */
    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        run_tests();
        return (tests_passed == tests_total) ? 0 : 1;
    }

    FILE *input = stdin;
    const char *filename = NULL;

    if (argc > 1) {
        filename = argv[1];
        input = fopen(filename, "r");
        if (input == NULL) {
            fprintf(stderr, "mini_wc: cannot open '%s'\n", filename);
            return 1;
        }
    }

    wc_result r = count_stream(input);
    print_result(r, filename);

    if (input != stdin) fclose(input);
    return 0;
}
