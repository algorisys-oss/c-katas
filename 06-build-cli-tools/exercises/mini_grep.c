/*
 * mini_grep.c — A simplified clone of the Unix `grep` tool
 *
 * Usage:
 *   ./mini_grep [flags] pattern [file]
 *   ./mini_grep --test              Run the built-in test suite
 *
 * Flags:
 *   -n    Prefix each matching line with its line number
 *   -i    Case-insensitive matching
 *   -c    Print only the count of matching lines (not the lines themselves)
 *
 * If no file is given, reads from stdin.
 *
 * What you will learn:
 *   - Parsing multiple flags from argv
 *   - String searching with strstr()
 *   - Case-insensitive comparison (tolower each character)
 *   - Combining flags to change output behavior
 *
 * Hints:
 *   - strstr(haystack, needle) returns a pointer to the first occurrence
 *     of needle in haystack, or NULL if not found.
 *   - For case-insensitive search, convert BOTH the line and the pattern
 *     to lowercase, then use strstr on the lowercase copies.
 *   - Use strcpy + tolower in a loop to make a lowercase copy.
 */

#define _GNU_SOURCE  /* for fmemopen */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 4096

/* ── Test Harness ───────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ── Options Structure ──────────────────────────────────────────────── */

typedef struct {
    int show_line_numbers;  /* -n flag */
    int case_insensitive;   /* -i flag */
    int count_only;         /* -c flag */
} grep_opts;

/* ── Core Functions ─────────────────────────────────────────────────── */

/*
 * to_lower_str — Convert a string to lowercase in-place.
 *
 * Walk each character of str and replace it with tolower(c).
 * This modifies the string directly — only call on a copy!
 */
void to_lower_str(char *str) {
    /* TODO: Implement.
     *
     * for (int i = 0; str[i] != '\0'; i++) {
     *     str[i] = (char)tolower((unsigned char)str[i]);
     * }
     */
    (void)str;
}

/*
 * line_matches — Check if a line contains the pattern.
 *
 * If case_insensitive is true, compare lowercase copies of both.
 * Returns 1 if the pattern is found, 0 otherwise.
 */
int line_matches(const char *line, const char *pattern, int case_insensitive) {
    /* TODO: Implement.
     *
     * If case_insensitive:
     *   1. Make lowercase copies of both line and pattern
     *      char lower_line[MAX_LINE];
     *      char lower_pat[MAX_LINE];
     *      strcpy(lower_line, line);
     *      strcpy(lower_pat, pattern);
     *      to_lower_str(lower_line);
     *      to_lower_str(lower_pat);
     *   2. return strstr(lower_line, lower_pat) != NULL;
     *
     * If case_sensitive:
     *   return strstr(line, pattern) != NULL;
     */
    (void)line;
    (void)pattern;
    (void)case_insensitive;
    return 0;
}

/*
 * grep_stream — Search for pattern in an open FILE stream.
 *
 * Read line by line with fgets. For each line that matches:
 *   - If count_only: just increment a counter
 *   - If show_line_numbers: print "linenum:line"
 *   - Otherwise: print the line
 *
 * Returns the number of matching lines (used as exit code: 0 if matches
 * found, 1 if no matches).
 */
int grep_stream(FILE *fp, const char *pattern, grep_opts opts) {
    /* TODO: Implement.
     *
     * int match_count = 0;
     * int line_num = 0;
     * char line[MAX_LINE];
     *
     * while (fgets(line, sizeof(line), fp) != NULL) {
     *     line_num++;
     *     if (line_matches(line, pattern, opts.case_insensitive)) {
     *         match_count++;
     *         if (!opts.count_only) {
     *             if (opts.show_line_numbers)
     *                 printf("%d:", line_num);
     *             printf("%s", line);
     *         }
     *     }
     * }
     *
     * if (opts.count_only)
     *     printf("%d\n", match_count);
     *
     * return match_count;
     */
    (void)fp;
    (void)pattern;
    (void)opts;
    return 0;
}

/*
 * grep_string — Helper for testing: run grep on a string in memory.
 *
 * Captures stdout output into result_buf so tests can check it.
 * Returns the match count.
 */
int grep_string(const char *text, const char *pattern, grep_opts opts,
                char *result_buf, int result_buf_size) {
    size_t text_len = strlen(text);
    if (text_len == 0) {
        result_buf[0] = '\0';
        return 0;
    }

    FILE *input = fmemopen((void *)text, text_len, "r");
    if (input == NULL) {
        result_buf[0] = '\0';
        return 0;
    }

    /* Redirect stdout to a memory buffer to capture output */
    FILE *capture = fmemopen(result_buf, (size_t)result_buf_size, "w");
    if (capture == NULL) {
        fclose(input);
        result_buf[0] = '\0';
        return 0;
    }

    /* Save real stdout, replace with capture buffer */
    FILE *saved_stdout = stdout;
    stdout = capture;

    int count = grep_stream(input, pattern, opts);

    /* Restore stdout */
    fflush(capture);
    stdout = saved_stdout;

    fclose(capture);
    fclose(input);
    return count;
}

/* ── Test Suite ─────────────────────────────────────────────────────── */

static void run_tests(void) {
    printf("mini_grep tests:\n");

    char buf[4096];
    grep_opts opts;

    /* Test 1: Basic match */
    TEST("finds a line containing the pattern");
        opts = (grep_opts){0, 0, 0};
        int c1 = grep_string("hello world\ngoodbye world\n",
                              "hello", opts, buf, sizeof(buf));
    EXPECT(c1 == 1 && strstr(buf, "hello world") != NULL);

    /* Test 2: No match */
    TEST("returns 0 when pattern is not found");
        opts = (grep_opts){0, 0, 0};
        int c2 = grep_string("apple\nbanana\ncherry\n",
                              "grape", opts, buf, sizeof(buf));
    EXPECT(c2 == 0);

    /* Test 3: Multiple matches */
    TEST("finds multiple matching lines");
        opts = (grep_opts){0, 0, 0};
        int c3 = grep_string("error: file not found\nok\nerror: timeout\n",
                              "error", opts, buf, sizeof(buf));
    EXPECT(c3 == 2);

    /* Test 4: Line numbers with -n */
    TEST("-n flag prefixes matching lines with line number");
        opts = (grep_opts){1, 0, 0};
        grep_string("aaa\nbbb\nccc\nbbb\n",
                     "bbb", opts, buf, sizeof(buf));
    EXPECT(strstr(buf, "2:") != NULL && strstr(buf, "4:") != NULL);

    /* Test 5: Case insensitive with -i */
    TEST("-i flag matches regardless of case");
        opts = (grep_opts){0, 1, 0};
        int c5 = grep_string("Hello World\nhello world\nHELLO WORLD\n",
                              "hello", opts, buf, sizeof(buf));
    EXPECT(c5 == 3);

    /* Test 6: Count only with -c */
    TEST("-c flag prints count instead of lines");
        opts = (grep_opts){0, 0, 1};
        grep_string("yes\nno\nyes\nno\nyes\n",
                     "yes", opts, buf, sizeof(buf));
    EXPECT(strstr(buf, "3") != NULL);

    /* Test 7: Empty input */
    TEST("empty input returns 0 matches");
        opts = (grep_opts){0, 0, 0};
        int c7 = grep_string("", "pattern", opts, buf, sizeof(buf));
    EXPECT(c7 == 0);

    /* Test 8: Pattern at start of line */
    TEST("matches pattern at start of line");
        opts = (grep_opts){0, 0, 0};
        int c8 = grep_string("prefix_word\nno match\n",
                              "prefix", opts, buf, sizeof(buf));
    EXPECT(c8 == 1);

    /* Test 9: Pattern at end of line */
    TEST("matches pattern at end of line");
        opts = (grep_opts){0, 0, 0};
        int c9 = grep_string("word_suffix\nno match\n",
                              "suffix", opts, buf, sizeof(buf));
    EXPECT(c9 == 1);

    /* Test 10: Case insensitive with mixed input */
    TEST("-i handles mixed case patterns");
        opts = (grep_opts){0, 1, 0};
        int c10 = grep_string("The Quick Brown Fox\nthe slow red dog\n",
                               "THE", opts, buf, sizeof(buf));
    EXPECT(c10 == 2);

    /* Test 11: -c with -n still prints only the count */
    TEST("-c with -n still prints only the count");
        opts = (grep_opts){1, 0, 1};
        grep_string("aaa\nbbb\naaa\n",
                     "aaa", opts, buf, sizeof(buf));
    EXPECT(strstr(buf, "2") != NULL && strstr(buf, "1:") == NULL);

    /* Test 12: Single character pattern */
    TEST("single character pattern works");
        opts = (grep_opts){0, 0, 0};
        int c12 = grep_string("axe\nbow\ncat\n",
                               "a", opts, buf, sizeof(buf));
    EXPECT(c12 == 2);

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
}

/* ── Main ───────────────────────────────────────────────────────────── */

int main(int argc, char *argv[]) {
    /* Run tests if --test flag is given */
    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        run_tests();
        return (tests_passed == tests_total) ? 0 : 1;
    }

    /* TODO: Implement argument parsing and main logic.
     *
     * 1. Parse flags: loop through argv starting at index 1.
     *    While argv[i] starts with '-', check for -n, -i, -c.
     *    Store them in a grep_opts struct.
     *
     * 2. After flags, the next argument is the pattern.
     *    If no pattern is given, print usage to stderr and return 2.
     *
     * 3. The argument after the pattern (if any) is the filename.
     *    Open it with fopen. If it fails, print error to stderr and return 1.
     *    If no filename, use stdin.
     *
     * 4. Call grep_stream() with the input, pattern, and options.
     *
     * 5. Return 0 if matches were found, 1 if no matches.
     *    (This matches real grep behavior.)
     *
     * Skeleton:
     *
     * grep_opts opts = {0, 0, 0};
     * int i = 1;
     *
     * while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0') {
     *     if (strcmp(argv[i], "-n") == 0) opts.show_line_numbers = 1;
     *     else if (strcmp(argv[i], "-i") == 0) opts.case_insensitive = 1;
     *     else if (strcmp(argv[i], "-c") == 0) opts.count_only = 1;
     *     else {
     *         fprintf(stderr, "mini_grep: unknown flag '%s'\n", argv[i]);
     *         return 2;
     *     }
     *     i++;
     * }
     *
     * if (i >= argc) {
     *     fprintf(stderr, "Usage: mini_grep [-n] [-i] [-c] pattern [file]\n");
     *     return 2;
     * }
     * const char *pattern = argv[i++];
     *
     * FILE *input = stdin;
     * if (i < argc) {
     *     input = fopen(argv[i], "r");
     *     if (input == NULL) {
     *         fprintf(stderr, "mini_grep: cannot open '%s'\n", argv[i]);
     *         return 1;
     *     }
     * }
     *
     * int matches = grep_stream(input, pattern, opts);
     * if (input != stdin) fclose(input);
     * return (matches > 0) ? 0 : 1;
     */

    (void)argc;
    (void)argv;
    return 0;
}
