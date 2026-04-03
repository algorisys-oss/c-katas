/*
 * mini_uniq.c — A simplified clone of the Unix `uniq` tool
 *
 * Usage:
 *   ./mini_uniq [flags] [file]    Remove adjacent duplicate lines
 *   ./mini_uniq --test            Run the built-in test suite
 *
 * Flags:
 *   -c    Prefix each line with the count of consecutive occurrences
 *   -d    Only print lines that are duplicated (appear 2+ times)
 *
 * If no file is given, reads from stdin.
 *
 * What you will learn:
 *   - Comparing the current line with the previous line (state tracking)
 *   - The "previous value" pattern: keep a buffer of the last thing you saw
 *   - strcmp() for exact string comparison
 *   - Flushing state at the end of input (the last group needs printing too)
 *
 * Hints:
 *   - Keep two buffers: `prev_line` and `curr_line`.
 *   - When curr_line == prev_line: increment a counter.
 *   - When curr_line != prev_line: output prev_line (maybe with count),
 *     then copy curr_line into prev_line and reset counter.
 *   - After the loop ends, you must output the LAST group (it never got
 *     a "different line" to trigger output). This is a common bug!
 *   - strcmp(a, b) returns 0 when strings are equal.
 *   - fgets keeps the '\n' at the end of each line. This is fine —
 *     strcmp will compare including the '\n', so lines match correctly.
 */

#define _GNU_SOURCE  /* for fmemopen */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

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

/* -- Options Structure ---------------------------------------------------- */

typedef struct {
    int show_count;      /* -c flag */
    int duplicates_only; /* -d flag */
} uniq_opts;

/* -- Core Functions ------------------------------------------------------- */

/*
 * output_group — Decide whether to print a group and in what format.
 *
 * A "group" is one unique line and the count of consecutive times it appeared.
 *
 *   line:  the text of the line (including trailing '\n')
 *   count: how many consecutive times this line appeared
 *   opts:  flags controlling output format
 *   out:   the FILE* to write to
 *
 * Rules:
 *   - If duplicates_only (-d) and count < 2: do not print.
 *   - If show_count (-c): print "  COUNT LINE"
 *   - Otherwise: just print the line.
 */
void output_group(const char *line, int count, uniq_opts opts, FILE *out) {
    if (opts.duplicates_only && count < 2)
        return;

    if (opts.show_count)
        fprintf(out, "%7d %s", count, line);
    else
        fprintf(out, "%s", line);
}

/*
 * uniq_stream — Remove adjacent duplicate lines from fp, write to out.
 *
 * Algorithm:
 *   1. Read the first line into prev_line. Set count = 1.
 *   2. For each subsequent line:
 *      a. If it equals prev_line: count++
 *      b. If it differs: output the group (prev_line, count),
 *         copy curr_line to prev_line, reset count to 1.
 *   3. After the loop: output the final group.
 *
 * Returns the number of unique groups found.
 */
int uniq_stream(FILE *fp, uniq_opts opts, FILE *out) {
    char prev_line[MAX_LINE];
    char curr_line[MAX_LINE];
    int count = 0;
    int groups = 0;

    /* Read the first line */
    if (fgets(prev_line, sizeof(prev_line), fp) == NULL)
        return 0;  /* empty input */
    count = 1;
    groups = 1;

    /* Process remaining lines */
    while (fgets(curr_line, sizeof(curr_line), fp) != NULL) {
        if (strcmp(curr_line, prev_line) == 0) {
            count++;
        } else {
            output_group(prev_line, count, opts, out);
            strcpy(prev_line, curr_line);
            count = 1;
            groups++;
        }
    }

    /* Don't forget the last group! */
    output_group(prev_line, count, opts, out);

    return groups;
}

/*
 * uniq_string — Helper for testing: run uniq on a string, capture output.
 *
 * Returns the number of unique groups.
 */
int uniq_string(const char *text, uniq_opts opts,
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

    FILE *output = fmemopen(result_buf, (size_t)result_buf_size, "w");
    if (output == NULL) {
        fclose(input);
        result_buf[0] = '\0';
        return 0;
    }

    int groups = uniq_stream(input, opts, output);

    fflush(output);
    fclose(output);
    fclose(input);
    return groups;
}

/* -- Test Suite ----------------------------------------------------------- */

static void run_tests(void) {
    printf("mini_uniq tests:\n");

    char buf[4096];
    uniq_opts opts;

    /* Test 1: Basic deduplication */
    TEST("removes adjacent duplicate lines");
        opts = (uniq_opts){0, 0};
        uniq_string("aaa\naaa\nbbb\nbbb\nccc\n",
                     opts, buf, sizeof(buf));
    EXPECT(strcmp(buf, "aaa\nbbb\nccc\n") == 0);

    /* Test 2: No duplicates — output unchanged */
    TEST("unique lines pass through unchanged");
        opts = (uniq_opts){0, 0};
        uniq_string("one\ntwo\nthree\n",
                     opts, buf, sizeof(buf));
    EXPECT(strcmp(buf, "one\ntwo\nthree\n") == 0);

    /* Test 3: All lines identical */
    TEST("all identical lines collapse to one");
        opts = (uniq_opts){0, 0};
        uniq_string("same\nsame\nsame\nsame\n",
                     opts, buf, sizeof(buf));
    EXPECT(strcmp(buf, "same\n") == 0);

    /* Test 4: -c flag shows counts */
    TEST("-c flag prefixes each line with occurrence count");
        opts = (uniq_opts){1, 0};
        uniq_string("a\na\nb\nc\nc\nc\n",
                     opts, buf, sizeof(buf));
    EXPECT(strstr(buf, "2 a\n") != NULL
           && strstr(buf, "1 b\n") != NULL
           && strstr(buf, "3 c\n") != NULL);

    /* Test 5: -d flag only prints duplicated lines */
    TEST("-d flag prints only duplicated lines");
        opts = (uniq_opts){0, 1};
        uniq_string("aaa\nbbb\nbbb\nccc\n",
                     opts, buf, sizeof(buf));
    EXPECT(strstr(buf, "bbb") != NULL
           && strstr(buf, "aaa") == NULL
           && strstr(buf, "ccc") == NULL);

    /* Test 6: Empty input */
    TEST("empty input produces no output");
        opts = (uniq_opts){0, 0};
        memset(buf, 0, sizeof(buf));
        int g6 = uniq_string("", opts, buf, sizeof(buf));
    EXPECT(g6 == 0);

    /* Test 7: Single line */
    TEST("single line passes through");
        opts = (uniq_opts){0, 0};
        uniq_string("only\n", opts, buf, sizeof(buf));
    EXPECT(strcmp(buf, "only\n") == 0);

    /* Test 8: Non-adjacent duplicates are NOT removed */
    TEST("non-adjacent duplicates are kept (matches real uniq)");
        opts = (uniq_opts){0, 0};
        uniq_string("a\nb\na\nb\n",
                     opts, buf, sizeof(buf));
    EXPECT(strcmp(buf, "a\nb\na\nb\n") == 0);

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
}

/* -- Main ----------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    /* Run tests if --test flag is given */
    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        run_tests();
        return (tests_passed == tests_total) ? 0 : 1;
    }

    uniq_opts opts = {0, 0};
    int i = 1;

    while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0') {
        if (strcmp(argv[i], "-c") == 0) opts.show_count = 1;
        else if (strcmp(argv[i], "-d") == 0) opts.duplicates_only = 1;
        else {
            fprintf(stderr, "mini_uniq: unknown flag '%s'\n", argv[i]);
            return 2;
        }
        i++;
    }

    FILE *input = stdin;
    if (i < argc) {
        input = fopen(argv[i], "r");
        if (input == NULL) {
            fprintf(stderr, "mini_uniq: cannot open '%s'\n", argv[i]);
            return 1;
        }
    }

    uniq_stream(input, opts, stdout);
    if (input != stdin) fclose(input);
    return 0;
}
