/*
 * Solution: String Pattern Matching Algorithms
 *
 * Four exercises: brute force, KMP table, KMP search, Rabin-Karp.
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <string.h>

/* ── Brute Force String Search ────────────────────────────────────── */
int brute_force_search(const char *text, const char *pattern)
{
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == m) {
            return i;  /* All m characters matched */
        }
    }
    return -1;
}

/* ── Build KMP Prefix Table ───────────────────────────────────────── */
void build_kmp_table(const char *pattern, int *table)
{
    int m = (int)strlen(pattern);
    table[0] = 0;
    int len = 0;  /* Length of the previous longest matching prefix-suffix */

    int j = 1;
    while (j < m) {
        if (pattern[j] == pattern[len]) {
            len++;
            table[j] = len;
            j++;
        } else if (len > 0) {
            /* Fall back to the previous matching prefix */
            len = table[len - 1];
            /* Don't advance j — retry with shorter prefix */
        } else {
            table[j] = 0;
            j++;
        }
    }
}

/* ── KMP Search ───────────────────────────────────────────────────── */
int kmp_search(const char *text, const char *pattern)
{
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    int table[1024];  /* Prefix table — large enough for our tests */
    build_kmp_table(pattern, table);

    int i = 0;  /* Index in text */
    int j = 0;  /* Index in pattern */

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }
        if (j == m) {
            return i - j;  /* Found a match */
        }
        if (i < n && text[i] != pattern[j]) {
            if (j > 0) {
                /* Use the prefix table to skip ahead */
                j = table[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1;
}

/* ── Rabin-Karp Search ────────────────────────────────────────────── */
int rabin_karp_search(const char *text, const char *pattern)
{
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    long base = 256;
    long prime = 101;

    /* Compute h = base^(m-1) % prime — used to remove leading character */
    long h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * base) % prime;
    }

    /* Compute initial hashes */
    long pattern_hash = 0;
    long text_hash = 0;
    for (int i = 0; i < m; i++) {
        pattern_hash = (pattern_hash * base + pattern[i]) % prime;
        text_hash = (text_hash * base + text[i]) % prime;
    }

    /* Slide the window */
    for (int i = 0; i <= n - m; i++) {
        if (pattern_hash == text_hash) {
            /* Hashes match — verify character by character to avoid
             * false positives from hash collisions */
            int match = 1;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) return i;
        }

        /* Update hash: remove text[i], add text[i + m] */
        if (i < n - m) {
            text_hash = ((text_hash - text[i] * h) * base + text[i + m]) % prime;
            /* Ensure non-negative */
            if (text_hash < 0) {
                text_hash += prime;
            }
        }
    }

    return -1;
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

static int arr_eq(const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void)
{
    printf("=== String Pattern Matching ===\n\n");

    /* --- brute_force_search --- */
    printf("brute_force_search:\n");
    {
        TEST("find \"BCD\" in \"ABCDEF\"")
            EXPECT(brute_force_search("ABCDEF", "BCD") == 1);
    }
    {
        TEST("pattern not found")
            EXPECT(brute_force_search("ABCDEF", "XYZ") == -1);
    }
    {
        TEST("pattern at start")
            EXPECT(brute_force_search("HELLO", "HEL") == 0);
    }

    /* --- build_kmp_table --- */
    printf("\nbuild_kmp_table:\n");
    {
        int table[6];
        int exp[] = {0, 0, 1, 2, 3, 0};
        build_kmp_table("ABABAC", table);
        TEST("table for \"ABABAC\"")
            EXPECT(arr_eq(table, exp, 6));
    }
    {
        int table[7];
        int exp[] = {0, 0, 0, 0, 1, 2, 3};
        build_kmp_table("ABCDABC", table);
        TEST("table for \"ABCDABC\"")
            EXPECT(arr_eq(table, exp, 7));
    }
    {
        int table[5];
        int exp[] = {0, 1, 2, 3, 4};
        build_kmp_table("AAAAA", table);
        TEST("table for \"AAAAA\"")
            EXPECT(arr_eq(table, exp, 5));
    }

    /* --- kmp_search --- */
    printf("\nkmp_search:\n");
    {
        TEST("find \"ABABAC\" in text")
            EXPECT(kmp_search("ABABABACABC", "ABABAC") == 2);
    }
    {
        TEST("pattern not found")
            EXPECT(kmp_search("ABCDEFGH", "XYZ") == -1);
    }
    {
        TEST("multiple occurrences (find first)")
            EXPECT(kmp_search("AAAAAA", "AA") == 0);
    }
    {
        TEST("pattern longer than text")
            EXPECT(kmp_search("AB", "ABCD") == -1);
    }

    /* --- rabin_karp_search --- */
    printf("\nrabin_karp_search:\n");
    {
        TEST("find \"BCD\" in \"ABCDEF\"")
            EXPECT(rabin_karp_search("ABCDEF", "BCD") == 1);
    }
    {
        TEST("pattern not found")
            EXPECT(rabin_karp_search("ABCDEF", "XYZ") == -1);
    }
    {
        TEST("pattern at end")
            EXPECT(rabin_karp_search("ABCDEF", "DEF") == 3);
    }
    {
        TEST("entire text is pattern")
            EXPECT(rabin_karp_search("HELLO", "HELLO") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
