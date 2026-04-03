/*
 * Kata: SQL Tokenizer
 *
 * Build a tokenizer (lexer) that breaks SQL strings into tokens.
 * This is the first stage of any language processor.
 *
 * The tokenizer reads characters one at a time and groups them into
 * meaningful chunks: keywords, identifiers, numbers, strings, operators.
 *
 * Functions to implement:
 *   - tokenize(input, tokens[], max_tokens) — returns token count
 *
 * Supported token types:
 *   TOK_KEYWORD   — SELECT, INSERT, INTO, FROM, WHERE, CREATE, TABLE,
 *                   VALUES, AND, OR
 *   TOK_IDENT     — identifiers (column names, table names)
 *   TOK_NUMBER    — integer literals (42, 100)
 *   TOK_STRING    — string literals ('hello')
 *   TOK_OPERATOR  — =, >, <, >=, <=, !=
 *   TOK_COMMA     — ,
 *   TOK_STAR      — *
 *   TOK_SEMICOLON — ;
 *   TOK_LPAREN    — (
 *   TOK_RPAREN    — )
 *
 * 12 tests total.
 *
 * Hints:
 *   - Process one character at a time with a position index
 *   - For letters: read the whole word, then check if it's a keyword
 *   - For digits: read all consecutive digits
 *   - For quotes: read until the closing quote
 *   - For >, <, !, =: peek at next char for two-character operators
 *   - Use toupper() or strcasecmp() so keywords are case-insensitive
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

/* ── Token types ──────────────────────────────────────────────────── */

typedef enum {
    TOK_KEYWORD,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_STRING,
    TOK_OPERATOR,
    TOK_COMMA,
    TOK_STAR,
    TOK_SEMICOLON,
    TOK_LPAREN,
    TOK_RPAREN
} TokenType;

typedef struct {
    TokenType type;
    char value[64];
} Token;

/* ── Helper: check if a word is a SQL keyword ─────────────────────
 * Keywords: SELECT, INSERT, INTO, FROM, WHERE, CREATE, TABLE,
 *           VALUES, AND, OR
 *
 * Compare case-insensitively. Return 1 if keyword, 0 otherwise.
 */
static int is_keyword(const char *word)
{
    /* TODO: Define an array of keyword strings, loop through them,
     * compare case-insensitively (use strcasecmp or manual comparison).
     */
    (void)word;
    return 0;
}

/* ── Main tokenizer ──────────────────────────────────────────────── */

/*
 * tokenize — break input string into tokens.
 *
 * Parameters:
 *   input      — the SQL string to tokenize
 *   tokens     — output array to fill with tokens
 *   max_tokens — maximum number of tokens to store
 *
 * Returns: the number of tokens produced.
 *
 * Algorithm:
 *   pos = 0
 *   while input[pos] != '\0' and count < max_tokens:
 *     - skip whitespace
 *     - if letter or '_': read word, check keyword vs identifier
 *     - if digit: read number
 *     - if '\'': read string literal (without quotes in value)
 *     - if '>', '<', '!', '=': read operator (check for two-char)
 *     - if ',', '*', ';', '(', ')': single-char token
 *     - else: skip unknown character
 */
int tokenize(const char *input, Token tokens[], int max_tokens)
{
    /* TODO: Implement the tokenizer.
     *
     * Use an int pos to track position in input.
     * Use an int count to track number of tokens produced.
     *
     * For each token:
     *   1. Set tokens[count].type
     *   2. Copy the text into tokens[count].value
     *   3. Increment count
     *
     * Return count.
     */
    (void)input;
    (void)tokens;
    (void)max_tokens;
    return 0;
}

/* ── Test harness ─────────────────────────────────────────────────── */

static int tests_total = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  %d. %-50s ", tests_total, name); \
} while (0);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); }

int main(void)
{
    Token tokens[64];
    int n;

    printf("=== SQL Tokenizer Tests ===\n\n");

    /* Test 1: Simple SELECT */
    n = tokenize("SELECT name FROM users;", tokens, 64);
    TEST("SELECT name FROM users — token count")
        EXPECT(n == 5);

    /* Test 2: Token types for simple SELECT */
    TEST("SELECT is KEYWORD")
        EXPECT(tokens[0].type == TOK_KEYWORD &&
               strcmp(tokens[0].value, "SELECT") == 0);

    /* Test 3: Identifier token */
    TEST("name is IDENT")
        EXPECT(tokens[1].type == TOK_IDENT &&
               strcmp(tokens[1].value, "name") == 0);

    /* Test 4: Semicolon token */
    TEST("trailing semicolon")
        EXPECT(tokens[4].type == TOK_SEMICOLON);

    /* Test 5: SELECT with star */
    n = tokenize("SELECT * FROM users;", tokens, 64);
    TEST("star token in SELECT *")
        EXPECT(n == 5 && tokens[1].type == TOK_STAR);

    /* Test 6: INSERT with VALUES */
    n = tokenize("INSERT INTO users VALUES ('Alice', 30);", tokens, 64);
    TEST("INSERT INTO — token count")
        EXPECT(n == 10);

    /* Test 7: String literal */
    TEST("string literal 'Alice'")
        EXPECT(tokens[5].type == TOK_STRING &&
               strcmp(tokens[5].value, "Alice") == 0);

    /* Test 8: Number literal */
    TEST("number literal 30")
        EXPECT(tokens[7].type == TOK_NUMBER &&
               strcmp(tokens[7].value, "30") == 0);

    /* Test 9: Parentheses */
    TEST("LPAREN and RPAREN present")
        EXPECT(tokens[4].type == TOK_LPAREN &&
               tokens[8].type == TOK_RPAREN);

    /* Test 10: WHERE with operator */
    n = tokenize("SELECT name FROM users WHERE age > 18;", tokens, 64);
    TEST("WHERE clause — token count")
        EXPECT(n == 9);

    /* Test 11: Operator token */
    TEST("operator > recognized")
        EXPECT(tokens[5].type == TOK_IDENT &&
               strcmp(tokens[5].value, "age") == 0 &&
               tokens[6].type == TOK_OPERATOR &&
               strcmp(tokens[6].value, ">") == 0);

    /* Test 12: Two-char operators */
    n = tokenize("SELECT * FROM t WHERE x >= 10 AND y != 5;", tokens, 64);
    TEST("two-char operators >= and !=")
        EXPECT(tokens[6].type == TOK_OPERATOR &&
               strcmp(tokens[6].value, ">=") == 0 &&
               tokens[10].type == TOK_OPERATOR &&
               strcmp(tokens[10].value, "!=") == 0);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
