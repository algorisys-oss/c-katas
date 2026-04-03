/*
 * Kata: String Tokenizer
 *
 * Build your own string tokenizer — a function that splits a string
 * into pieces (tokens) separated by a delimiter character.
 *
 * This is similar to strtok() from <string.h>, but you'll build it
 * from scratch to understand how it works.
 *
 * You'll implement:
 *   1. count_tokens   — count how many tokens a string has
 *   2. get_token      — extract the nth token into a buffer
 *   3. split_string   — split into an array of token pointers
 *
 * 12 tests total.
 *
 * Example: "Hello,World,Foo" with delimiter ',' has 3 tokens:
 *   token 0: "Hello"
 *   token 1: "World"
 *   token 2: "Foo"
 */

#include <stdio.h>
#include <assert.h>

/* ── Exercise 1: count_tokens ───────────────────────────────────────
 * Count the number of tokens in str separated by delim.
 *
 * Examples:
 *   count_tokens("a,b,c", ',')     → 3
 *   count_tokens("hello", ',')     → 1  (no delimiter → 1 token)
 *   count_tokens(",,", ',')        → 3  (empty tokens count!)
 *   count_tokens("", ',')          → 1  (empty string is 1 empty token)
 *
 * Hint: tokens = number of delimiters + 1
 */
int count_tokens(const char *str, char delim)
{
    /* TODO: Count occurrences of delim, return that + 1 */
    return 0; /* placeholder */
}

/* ── Exercise 2: get_token ──────────────────────────────────────────
 * Extract the nth token (0-indexed) from str into buf.
 * Null-terminate the result.
 * buf_size is the maximum size of buf (including '\0').
 *
 * Return 1 on success, 0 if token index is out of range.
 *
 * Example:
 *   get_token("Hello,World,Foo", ',', 1, buf, 32) → buf = "World"
 *
 * Hint:
 *   1. Skip past the first n delimiters to find the start
 *   2. Copy characters until the next delimiter or end of string
 *   3. Don't forget the null terminator and buf_size limit
 */
int get_token(const char *str, char delim, int index, char *buf, int buf_size)
{
    /* TODO: Find the nth token and copy it into buf */
    return 0; /* placeholder */
}

/* ── Exercise 3: split_string ───────────────────────────────────────
 * Split str by delim into an array of null-terminated strings.
 *
 * Parameters:
 *   str   — the string to split (will NOT be modified)
 *   delim — the delimiter character
 *   out   — array of char pointers (each points to a buffer you fill)
 *   bufs  — 2D array of buffers: bufs[i] has buf_size bytes
 *   max_tokens — maximum number of tokens (size of out[] and bufs[])
 *   buf_size   — size of each buffer in bufs
 *
 * Return the number of tokens found (up to max_tokens).
 *
 * Example:
 *   char bufs[10][32];
 *   char *out[10];
 *   int n = split_string("a,bb,ccc", ',', out, bufs, 10, 32);
 *   // n == 3, out[0] → "a", out[1] → "bb", out[2] → "ccc"
 */
int split_string(const char *str, char delim, char **out,
                 char bufs[][32], int max_tokens, int buf_size)
{
    /* TODO: Split str and fill bufs/out arrays */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* Helper to compare strings without <string.h> */
static int str_eq(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

int main(void)
{
    printf("=== String Tokenizer ===\n\n");

    /* --- count_tokens tests --- */
    printf("count_tokens:\n");
    TEST("\"a,b,c\" with ',' → 3")        EXPECT(count_tokens("a,b,c", ',') == 3);
    TEST("\"hello\" with ',' → 1")        EXPECT(count_tokens("hello", ',') == 1);
    TEST("\",,\" with ',' → 3")           EXPECT(count_tokens(",,", ',') == 3);
    TEST("\"a::b::c\" with ':' → 5")      EXPECT(count_tokens("a::b::c", ':') == 5);

    /* --- get_token tests --- */
    printf("\nget_token:\n");
    {
        char buf[32];

        get_token("Hello,World,Foo", ',', 0, buf, 32);
        TEST("token 0 of \"Hello,World,Foo\" → \"Hello\"")
            EXPECT(str_eq(buf, "Hello"));

        get_token("Hello,World,Foo", ',', 1, buf, 32);
        TEST("token 1 → \"World\"")
            EXPECT(str_eq(buf, "World"));

        get_token("Hello,World,Foo", ',', 2, buf, 32);
        TEST("token 2 → \"Foo\"")
            EXPECT(str_eq(buf, "Foo"));

        int ok = get_token("Hello,World", ',', 5, buf, 32);
        TEST("out of range token → returns 0")
            EXPECT(ok == 0);
    }

    /* --- split_string tests --- */
    printf("\nsplit_string:\n");
    {
        char bufs[10][32];
        char *out[10];

        int n = split_string("one,two,three", ',', out, bufs, 10, 32);
        TEST("split \"one,two,three\" → 3 tokens")
            EXPECT(n == 3);
        TEST("token 0 → \"one\"")
            EXPECT(str_eq(out[0], "one"));
        TEST("token 1 → \"two\"")
            EXPECT(str_eq(out[1], "two"));
        TEST("token 2 → \"three\"")
            EXPECT(str_eq(out[2], "three"));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
