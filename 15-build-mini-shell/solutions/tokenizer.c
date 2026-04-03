/*
 * Kata: Command-Line Tokenizer
 *
 * Build a tokenizer that splits shell input into tokens.
 *
 *   - tokenize(input, tokens, max_tokens) — split input string into tokens
 *
 * Rules:
 *   - Spaces and tabs are delimiters (multiple consecutive = same as one)
 *   - Quoted strings ("hello world") are a single token (quotes removed)
 *   - Pipe '|' is always a separate token
 *   - Redirect operators '>', '<', '>>' are separate tokens
 *   - Trailing newline is stripped
 *   - Returns the number of tokens found
 *   - tokens[count] is set to NULL (required by execvp)
 *
 * 14 tests total.
 *
 * Hints:
 *   - Walk the input character by character
 *   - Use a state machine: NORMAL, IN_QUOTES
 *   - When you see '"', toggle the in_quotes state
 *   - When you see '|', '>', '<', finish current token, emit operator token
 *   - Check for '>>' (two characters) before single '>'
 *   - Null-terminate each token by writing '\0' into a buffer
 *   - Use a scratch buffer to build tokens, then strdup() each one
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS    64
#define MAX_TOKEN_LEN 256

/* ── Test harness ───────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ── tokenize ───────────────────────────────────────────────────────
 * Split 'input' into tokens, storing pointers in 'tokens[]'.
 *
 * Each token is a freshly malloc'd string (caller must free).
 * tokens[return_value] is set to NULL.
 *
 * Returns: number of tokens, or -1 on error.
 *
 * Algorithm:
 *   - Strip trailing newline from input
 *   - Walk character by character:
 *     - If whitespace (and not in quotes): finish current token
 *     - If '"': toggle in_quotes flag (don't include the quote char)
 *     - If '|': finish current token, emit "|" as its own token
 *     - If '>': check next char for '>' (emit ">>" or ">")
 *     - If '<': finish current token, emit "<" as its own token
 *     - Otherwise: append character to current token buffer
 *   - After the loop, finish any remaining token
 */
int tokenize(char *input, char **tokens, int max_tokens)
{
    char buf[MAX_TOKEN_LEN];
    int buf_len = 0;
    int count = 0;
    int in_quotes = 0;
    int i;
    int len;

    /* Helper macro: flush buf as a token if non-empty */
    #define FLUSH_TOKEN() do { \
        if (buf_len > 0 && count < max_tokens - 1) { \
            buf[buf_len] = '\0'; \
            tokens[count++] = strdup(buf); \
            buf_len = 0; \
        } \
    } while (0)

    #define EMIT_TOKEN(s) do { \
        if (count < max_tokens - 1) { \
            tokens[count++] = strdup(s); \
        } \
    } while (0)

    /* Step 1: Strip trailing newline */
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') input[--len] = '\0';

    /* Step 2: Walk character by character */
    for (i = 0; i < len; i++) {
        /* a) Quote toggle */
        if (input[i] == '"') {
            in_quotes = !in_quotes;
            continue;
        }

        /* b) Inside quotes: everything is literal */
        if (in_quotes) {
            if (buf_len < MAX_TOKEN_LEN - 1)
                buf[buf_len++] = input[i];
            continue;
        }

        /* c) Whitespace: finish current token */
        if (isspace((unsigned char)input[i])) {
            FLUSH_TOKEN();
            continue;
        }

        /* d) Pipe: finish current token, emit "|" */
        if (input[i] == '|') {
            FLUSH_TOKEN();
            EMIT_TOKEN("|");
            continue;
        }

        /* e) Output redirect: check for >> or > */
        if (input[i] == '>') {
            FLUSH_TOKEN();
            if (i + 1 < len && input[i + 1] == '>') {
                EMIT_TOKEN(">>");
                i++; /* skip next '>' */
            } else {
                EMIT_TOKEN(">");
            }
            continue;
        }

        /* f) Input redirect: finish current token, emit "<" */
        if (input[i] == '<') {
            FLUSH_TOKEN();
            EMIT_TOKEN("<");
            continue;
        }

        /* g) Regular character: append to buffer */
        if (buf_len < MAX_TOKEN_LEN - 1)
            buf[buf_len++] = input[i];
    }

    /* Step 3: Flush remaining token */
    FLUSH_TOKEN();

    #undef FLUSH_TOKEN
    #undef EMIT_TOKEN

    /* Step 4: Null-terminate and return */
    tokens[count] = NULL;
    return count;
}

/* ── Helper: free tokens ────────────────────────────────────────── */
static void free_tokens(char **tokens, int count)
{
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
        tokens[i] = NULL;
    }
}

/* ── Tests ──────────────────────────────────────────────────────── */

int main(void)
{
    char *tokens[MAX_TOKENS];
    int count;

    printf("=== Tokenizer Tests ===\n\n");

    /* Test 1: simple command, no arguments */
    {
        char input[] = "ls\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("simple command: 'ls'");
        EXPECT(count == 1 && strcmp(tokens[0], "ls") == 0
               && tokens[1] == NULL);
        free_tokens(tokens, count);
    }

    /* Test 2: command with arguments */
    {
        char input[] = "ls -la /home\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("command with args: 'ls -la /home'");
        EXPECT(count == 3
               && strcmp(tokens[0], "ls") == 0
               && strcmp(tokens[1], "-la") == 0
               && strcmp(tokens[2], "/home") == 0
               && tokens[3] == NULL);
        free_tokens(tokens, count);
    }

    /* Test 3: extra whitespace between tokens */
    {
        char input[] = "  ls   -la    /home  \n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("extra whitespace: '  ls   -la    /home  '");
        EXPECT(count == 3
               && strcmp(tokens[0], "ls") == 0
               && strcmp(tokens[1], "-la") == 0
               && strcmp(tokens[2], "/home") == 0);
        free_tokens(tokens, count);
    }

    /* Test 4: tabs as delimiters */
    {
        char input[] = "echo\thello\tworld\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("tabs as delimiters");
        EXPECT(count == 3
               && strcmp(tokens[0], "echo") == 0
               && strcmp(tokens[1], "hello") == 0
               && strcmp(tokens[2], "world") == 0);
        free_tokens(tokens, count);
    }

    /* Test 5: quoted string becomes one token */
    {
        char input[] = "echo \"hello world\"\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("quoted string: 'echo \"hello world\"'");
        EXPECT(count == 2
               && strcmp(tokens[0], "echo") == 0
               && strcmp(tokens[1], "hello world") == 0);
        free_tokens(tokens, count);
    }

    /* Test 6: pipe as separate token */
    {
        char input[] = "ls | grep .c\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("pipe: 'ls | grep .c'");
        EXPECT(count == 4
               && strcmp(tokens[0], "ls") == 0
               && strcmp(tokens[1], "|") == 0
               && strcmp(tokens[2], "grep") == 0
               && strcmp(tokens[3], ".c") == 0);
        free_tokens(tokens, count);
    }

    /* Test 7: pipe without spaces */
    {
        char input[] = "ls|grep .c\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("pipe no spaces: 'ls|grep .c'");
        EXPECT(count == 4
               && strcmp(tokens[0], "ls") == 0
               && strcmp(tokens[1], "|") == 0
               && strcmp(tokens[2], "grep") == 0
               && strcmp(tokens[3], ".c") == 0);
        free_tokens(tokens, count);
    }

    /* Test 8: output redirect */
    {
        char input[] = "ls > files.txt\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("output redirect: 'ls > files.txt'");
        EXPECT(count == 3
               && strcmp(tokens[0], "ls") == 0
               && strcmp(tokens[1], ">") == 0
               && strcmp(tokens[2], "files.txt") == 0);
        free_tokens(tokens, count);
    }

    /* Test 9: append redirect */
    {
        char input[] = "echo hi >> log.txt\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("append redirect: 'echo hi >> log.txt'");
        EXPECT(count == 4
               && strcmp(tokens[0], "echo") == 0
               && strcmp(tokens[1], "hi") == 0
               && strcmp(tokens[2], ">>") == 0
               && strcmp(tokens[3], "log.txt") == 0);
        free_tokens(tokens, count);
    }

    /* Test 10: input redirect */
    {
        char input[] = "sort < data.txt\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("input redirect: 'sort < data.txt'");
        EXPECT(count == 3
               && strcmp(tokens[0], "sort") == 0
               && strcmp(tokens[1], "<") == 0
               && strcmp(tokens[2], "data.txt") == 0);
        free_tokens(tokens, count);
    }

    /* Test 11: empty input */
    {
        char input[] = "\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("empty input");
        EXPECT(count == 0 && tokens[0] == NULL);
        free_tokens(tokens, count);
    }

    /* Test 12: only whitespace */
    {
        char input[] = "   \t  \n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("only whitespace");
        EXPECT(count == 0 && tokens[0] == NULL);
        free_tokens(tokens, count);
    }

    /* Test 13: multiple pipes */
    {
        char input[] = "cat file | grep foo | wc -l\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("multiple pipes: 'cat file | grep foo | wc -l'");
        EXPECT(count == 8
               && strcmp(tokens[0], "cat") == 0
               && strcmp(tokens[1], "file") == 0
               && strcmp(tokens[2], "|") == 0
               && strcmp(tokens[3], "grep") == 0
               && strcmp(tokens[4], "foo") == 0
               && strcmp(tokens[5], "|") == 0
               && strcmp(tokens[6], "wc") == 0
               && strcmp(tokens[7], "-l") == 0);
        free_tokens(tokens, count);
    }

    /* Test 14: combined — pipe + redirect + quotes */
    {
        char input[] = "grep \"hello world\" < input.txt | sort > output.txt\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("combined: pipe + redirect + quotes");
        EXPECT(count == 8
               && strcmp(tokens[0], "grep") == 0
               && strcmp(tokens[1], "hello world") == 0
               && strcmp(tokens[2], "<") == 0
               && strcmp(tokens[3], "input.txt") == 0
               && strcmp(tokens[4], "|") == 0
               && strcmp(tokens[5], "sort") == 0
               && strcmp(tokens[6], ">") == 0
               && strcmp(tokens[7], "output.txt") == 0);
        free_tokens(tokens, count);
    }

    /* ── Summary ─────────────────────────────────────────────────── */
    printf("\n  Passed: %d / %d\n", tests_passed, tests_total);
    return (tests_passed == tests_total) ? 0 : 1;
}
