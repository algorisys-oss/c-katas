/*
 * Solution: SQL Tokenizer
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

/* ── Helper: check if a word is a SQL keyword ───────────────────── */

static int is_keyword(const char *word)
{
    static const char *keywords[] = {
        "SELECT", "INSERT", "INTO", "FROM", "WHERE",
        "CREATE", "TABLE", "VALUES", "AND", "OR"
    };
    int num_keywords = (int)(sizeof(keywords) / sizeof(keywords[0]));

    for (int i = 0; i < num_keywords; i++) {
        if (strcasecmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* ── Main tokenizer ──────────────────────────────────────────────── */

int tokenize(const char *input, Token tokens[], int max_tokens)
{
    int pos = 0;
    int count = 0;
    int len = (int)strlen(input);

    while (pos < len && count < max_tokens) {
        /* Skip whitespace */
        while (pos < len && isspace((unsigned char)input[pos])) {
            pos++;
        }
        if (pos >= len) {
            break;
        }

        char c = input[pos];

        /* Identifiers and keywords */
        if (isalpha((unsigned char)c) || c == '_') {
            int start = pos;
            while (pos < len && (isalnum((unsigned char)input[pos]) || input[pos] == '_')) {
                pos++;
            }
            int word_len = pos - start;
            if (word_len >= 64) {
                word_len = 63;
            }
            memcpy(tokens[count].value, input + start, word_len);
            tokens[count].value[word_len] = '\0';

            /* Convert to uppercase for keyword check */
            char upper[64];
            for (int i = 0; i < word_len; i++) {
                upper[i] = toupper((unsigned char)tokens[count].value[i]);
            }
            upper[word_len] = '\0';

            if (is_keyword(upper)) {
                tokens[count].type = TOK_KEYWORD;
                /* Store keywords in uppercase */
                memcpy(tokens[count].value, upper, word_len + 1);
            } else {
                tokens[count].type = TOK_IDENT;
            }
            count++;
        }
        /* Numbers */
        else if (isdigit((unsigned char)c)) {
            int start = pos;
            while (pos < len && isdigit((unsigned char)input[pos])) {
                pos++;
            }
            int num_len = pos - start;
            if (num_len >= 64) {
                num_len = 63;
            }
            memcpy(tokens[count].value, input + start, num_len);
            tokens[count].value[num_len] = '\0';
            tokens[count].type = TOK_NUMBER;
            count++;
        }
        /* String literals */
        else if (c == '\'') {
            pos++;  /* skip opening quote */
            int start = pos;
            while (pos < len && input[pos] != '\'') {
                pos++;
            }
            int str_len = pos - start;
            if (str_len >= 64) {
                str_len = 63;
            }
            memcpy(tokens[count].value, input + start, str_len);
            tokens[count].value[str_len] = '\0';
            tokens[count].type = TOK_STRING;
            if (pos < len) {
                pos++;  /* skip closing quote */
            }
            count++;
        }
        /* Operators: >, <, >=, <=, !=, = */
        else if (c == '>' || c == '<' || c == '!' || c == '=') {
            tokens[count].value[0] = c;
            pos++;
            if (pos < len && input[pos] == '=') {
                tokens[count].value[1] = '=';
                tokens[count].value[2] = '\0';
                pos++;
            } else {
                tokens[count].value[1] = '\0';
            }
            tokens[count].type = TOK_OPERATOR;
            count++;
        }
        /* Single-character tokens */
        else if (c == ',') {
            tokens[count].type = TOK_COMMA;
            tokens[count].value[0] = ',';
            tokens[count].value[1] = '\0';
            pos++;
            count++;
        }
        else if (c == '*') {
            tokens[count].type = TOK_STAR;
            tokens[count].value[0] = '*';
            tokens[count].value[1] = '\0';
            pos++;
            count++;
        }
        else if (c == ';') {
            tokens[count].type = TOK_SEMICOLON;
            tokens[count].value[0] = ';';
            tokens[count].value[1] = '\0';
            pos++;
            count++;
        }
        else if (c == '(') {
            tokens[count].type = TOK_LPAREN;
            tokens[count].value[0] = '(';
            tokens[count].value[1] = '\0';
            pos++;
            count++;
        }
        else if (c == ')') {
            tokens[count].type = TOK_RPAREN;
            tokens[count].value[0] = ')';
            tokens[count].value[1] = '\0';
            pos++;
            count++;
        }
        else {
            /* Skip unknown characters */
            pos++;
        }
    }

    return count;
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
