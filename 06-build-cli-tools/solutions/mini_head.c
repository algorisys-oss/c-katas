/*
 * mini_head.c — A simplified clone of the Unix `head` tool
 *
 * Usage:
 *   ./mini_head [file]            Print the first 10 lines (default)
 *   ./mini_head -n NUM [file]     Print the first NUM lines
 *   ./mini_head --test            Run the built-in test suite
 *
 * If no file is given, reads from stdin.
 *
 * What you will learn:
 *   - Parsing a flag that takes a numeric argument (-n NUM)
 *   - Breaking out of a loop early (you stop before EOF)
 *   - atoi() for converting strings to integers
 *   - Default values: do the common thing when no flag is given
 *
 * Hints:
 *   - The key insight: you do NOT read the whole file. You read up to N
 *     lines and stop. This is what makes head fast on huge files.
 *   - Use a counter: read lines with fgets, increment counter, stop when
 *     counter reaches N.
 *   - atoi("42") returns the integer 42.
 */

#define _GNU_SOURCE  /* for fmemopen */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define DEFAULT_LINES 10

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

/* -- Core Functions ------------------------------------------------------- */

/*
 * head_stream — Print the first `max_lines` lines from fp.
 *
 * Reads line by line with fgets. Stops after printing max_lines lines
 * (or when the input ends, whichever comes first).
 *
 * Writes output to `out` (so tests can capture it).
 * Returns the number of lines actually printed.
 */
int head_stream(FILE *fp, int max_lines, FILE *out) {
    int count = 0;
    char line[MAX_LINE];

    while (count < max_lines && fgets(line, sizeof(line), fp) != NULL) {
        fprintf(out, "%s", line);
        count++;
    }

    return count;
}

/*
 * head_string — Helper for testing: run head on a string, capture output.
 *
 * Returns the number of lines printed.
 */
int head_string(const char *text, int max_lines,
                char *result_buf, int result_buf_size) {
    size_t text_len = strlen(text);
    if (text_len == 0) {
        result_buf[0] = '\0';
        return head_stream(stdin, 0, stdout);
        /* empty input special case: just return 0 */
    }

    FILE *input = fmemopen((void *)text, text_len, "r");
    if (input == NULL) {
        result_buf[0] = '\0';
        return 0;
    }

    FILE *output = fmemopen(result_buf, (size_t)result_buf_size, "w");
    if (output == NULL) {
        fclose(input);
        result_buf[0] = '\0';
        return 0;
    }

    int count = head_stream(input, max_lines, output);

    fflush(output);
    fclose(output);
    fclose(input);
    return count;
}

/*
 * count_newlines — Count the number of '\n' in a string.
 *
 * Helper for tests to verify output.
 */
static int count_newlines(const char *s) {
    int n = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '\n') n++;
    }
    return n;
}

/* -- Test Suite ----------------------------------------------------------- */

static void run_tests(void) {
    printf("mini_head tests:\n");

    char buf[4096];

    /* Test 1: Default 10 lines from a 15-line input */
    TEST("default prints first 10 lines");
        const char *t1 =
            "line 1\nline 2\nline 3\nline 4\nline 5\n"
            "line 6\nline 7\nline 8\nline 9\nline 10\n"
            "line 11\nline 12\nline 13\nline 14\nline 15\n";
        head_string(t1, DEFAULT_LINES, buf, sizeof(buf));
    EXPECT(count_newlines(buf) == 10 && strstr(buf, "line 11") == NULL);

    /* Test 2: Custom count -n 3 */
    TEST("-n 3 prints exactly 3 lines");
        int p2 = head_string("aaa\nbbb\nccc\nddd\neee\n", 3,
                              buf, sizeof(buf));
    EXPECT(p2 == 3 && count_newlines(buf) == 3);

    /* Test 3: Fewer lines than requested */
    TEST("input shorter than N prints all lines");
        int p3 = head_string("only\ntwo\n", 10, buf, sizeof(buf));
    EXPECT(p3 == 2 && count_newlines(buf) == 2);

    /* Test 4: Empty input */
    TEST("empty input prints nothing");
        memset(buf, 0, sizeof(buf));
        int p4 = head_string("", 10, buf, sizeof(buf));
    EXPECT(p4 == 0);

    /* Test 5: One line, request one */
    TEST("-n 1 prints exactly one line");
        int p5 = head_string("first\nsecond\nthird\n", 1,
                              buf, sizeof(buf));
    EXPECT(p5 == 1 && strstr(buf, "first") != NULL
           && strstr(buf, "second") == NULL);

    /* Test 6: Input without trailing newline */
    TEST("last line without newline is still counted");
        int p6 = head_string("aaa\nbbb\nccc", 5, buf, sizeof(buf));
    EXPECT(p6 == 3 && strstr(buf, "ccc") != NULL);

    /* Test 7: -n 0 prints nothing */
    TEST("-n 0 prints zero lines");
        int p7 = head_string("something\n", 0, buf, sizeof(buf));
    EXPECT(p7 == 0);

    /* Test 8: Preserves line content exactly */
    TEST("output preserves exact line content");
        head_string("  indented\ttabs\n", 1, buf, sizeof(buf));
    EXPECT(strstr(buf, "  indented\ttabs") != NULL);

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
}

/* -- Main ----------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    /* Run tests if --test flag is given */
    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        run_tests();
        return (tests_passed == tests_total) ? 0 : 1;
    }

    int max_lines = DEFAULT_LINES;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0) {
        if (i + 1 >= argc) {
            fprintf(stderr, "mini_head: -n requires an argument\n");
            return 2;
        }
        max_lines = atoi(argv[i + 1]);
        i += 2;
    }

    FILE *input = stdin;
    if (i < argc) {
        input = fopen(argv[i], "r");
        if (input == NULL) {
            fprintf(stderr, "mini_head: cannot open '%s'\n", argv[i]);
            return 1;
        }
    }

    head_stream(input, max_lines, stdout);
    if (input != stdin) fclose(input);
    return 0;
}
