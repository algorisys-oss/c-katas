/*
 * Solution: String Tokenizer
 */

#include <stdio.h>
#include <assert.h>

int count_tokens(const char *str, char delim)
{
    int count = 1;
    while (*str) {
        if (*str == delim) count++;
        str++;
    }
    return count;
}

int get_token(const char *str, char delim, int index, char *buf, int buf_size)
{
    /* Skip past 'index' delimiters */
    int token = 0;
    while (token < index) {
        if (*str == '\0') return 0;     /* not enough tokens */
        if (*str == delim) token++;
        str++;
    }

    /* Copy characters until next delimiter or end */
    int i = 0;
    while (*str && *str != delim && i < buf_size - 1) {
        buf[i++] = *str++;
    }
    buf[i] = '\0';
    return 1;
}

int split_string(const char *str, char delim, char **out,
                 char bufs[][32], int max_tokens, int buf_size)
{
    int n = 0;
    int bi = 0;     /* index into current buffer */

    while (n < max_tokens) {
        bi = 0;
        /* Copy characters until delimiter or end */
        while (*str && *str != delim && bi < buf_size - 1) {
            bufs[n][bi++] = *str++;
        }
        bufs[n][bi] = '\0';
        out[n] = bufs[n];
        n++;

        if (*str == '\0') break;
        str++;  /* skip delimiter */
    }

    return n;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

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
    printf("=== String Tokenizer (Solution) ===\n\n");

    printf("count_tokens:\n");
    TEST("\"a,b,c\" with ',' → 3")        EXPECT(count_tokens("a,b,c", ',') == 3);
    TEST("\"hello\" with ',' → 1")        EXPECT(count_tokens("hello", ',') == 1);
    TEST("\",,\" with ',' → 3")           EXPECT(count_tokens(",,", ',') == 3);
    TEST("\"a::b::c\" with ':' → 5")      EXPECT(count_tokens("a::b::c", ':') == 5);

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
