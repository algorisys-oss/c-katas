#include <stdio.h>
#include <string.h>

#define MAX_N 1000
#define MAX_STR 100

/* ── Exercise 1: knapsack_01 ──────────────────────────────────────── */

int knapsack_01(int weights[], int values[], int n, int capacity)
{
    /* dp[i][w] = max value using first i items with capacity w */
    int dp[n + 1][capacity + 1];
    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            /* Option 1: don't take item i */
            dp[i][w] = dp[i - 1][w];
            /* Option 2: take item i (if it fits) */
            if (weights[i - 1] <= w) {
                int with_item = dp[i - 1][w - weights[i - 1]] + values[i - 1];
                if (with_item > dp[i][w]) {
                    dp[i][w] = with_item;
                }
            }
        }
    }
    return dp[n][capacity];
}

/* ── Exercise 2: longest_increasing_subseq ────────────────────────── */

int longest_increasing_subseq(int arr[], int n)
{
    if (n == 0) return 0;

    int dp[n];
    for (int i = 0; i < n; i++) {
        dp[i] = 1;  /* every element is a subsequence of length 1 */
    }

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
    }

    int max_len = 1;
    for (int i = 0; i < n; i++) {
        if (dp[i] > max_len) max_len = dp[i];
    }
    return max_len;
}

/* ── Exercise 3: edit_distance ────────────────────────────────────── */

static int min3(int a, int b, int c)
{
    int m = a < b ? a : b;
    return m < c ? m : c;
}

int edit_distance(const char *a, const char *b)
{
    int m = (int)strlen(a);
    int n = (int)strlen(b);
    int dp[MAX_STR + 1][MAX_STR + 1];

    /* Base cases: transforming empty string */
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];  /* characters match — no cost */
            } else {
                dp[i][j] = 1 + min3(
                    dp[i - 1][j],      /* delete from a */
                    dp[i][j - 1],      /* insert into a */
                    dp[i - 1][j - 1]   /* replace in a */
                );
            }
        }
    }
    return dp[m][n];
}

/* ── Exercise 4: longest_palindromic_subseq ───────────────────────── */

int longest_palindromic_subseq(const char *s)
{
    int n = (int)strlen(s);
    if (n == 0) return 0;

    /* dp[i][j] = length of LPS in s[i..j] */
    int dp[MAX_STR][MAX_STR];
    memset(dp, 0, sizeof(dp));

    /* Every single character is a palindrome of length 1 */
    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    /* Fill for increasing substring lengths */
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            if (s[i] == s[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            } else {
                dp[i][j] = dp[i + 1][j] > dp[i][j - 1]
                          ? dp[i + 1][j]
                          : dp[i][j - 1];
            }
        }
    }
    return dp[0][n - 1];
}

/* ── Exercise 5: word_break ───────────────────────────────────────── */

int word_break(const char *s, const char *dict[], int dict_size)
{
    int n = (int)strlen(s);
    /* dp[i] = 1 if s[0..i-1] can be segmented into dictionary words */
    int dp[MAX_N + 1];
    memset(dp, 0, sizeof(dp));
    dp[0] = 1;  /* empty string is always valid */

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w < dict_size; w++) {
            int wlen = (int)strlen(dict[w]);
            if (wlen <= i && dp[i - wlen]) {
                /* Check if s[i-wlen .. i-1] matches dict[w] */
                if (strncmp(s + i - wlen, dict[w], (size_t)wlen) == 0) {
                    dp[i] = 1;
                    break;  /* no need to check more words */
                }
            }
        }
    }
    return dp[n];
}

/* ── Test Harness ────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Advanced Dynamic Programming ===\n\n");

    /* ── knapsack_01 tests ───────────────────────────────────── */
    printf("knapsack_01:\n");
    {
        int w1[] = {2, 3, 4, 5};
        int v1[] = {3, 4, 5, 6};
        TEST("weights[2,3,4,5] vals[3,4,5,6] cap=8 -> 10")
            EXPECT(knapsack_01(w1, v1, 4, 8) == 10);

        int w2[] = {1, 2, 3};
        int v2[] = {6, 10, 12};
        TEST("weights[1,2,3] vals[6,10,12] cap=5 -> 22")
            EXPECT(knapsack_01(w2, v2, 3, 5) == 22);

        TEST("capacity 0 -> 0")
            EXPECT(knapsack_01(w1, v1, 4, 0) == 0);

        int w3[] = {10};
        int v3[] = {100};
        TEST("single item too heavy -> 0")
            EXPECT(knapsack_01(w3, v3, 1, 5) == 0);
    }

    /* ── longest_increasing_subseq tests ──────────────────────── */
    printf("\nlongest_increasing_subseq:\n");
    {
        int a1[] = {10, 9, 2, 5, 3, 7, 101, 18};
        TEST("[10,9,2,5,3,7,101,18] -> 4")
            EXPECT(longest_increasing_subseq(a1, 8) == 4);

        int a2[] = {0, 1, 0, 3, 2, 3};
        TEST("[0,1,0,3,2,3] -> 4")
            EXPECT(longest_increasing_subseq(a2, 6) == 4);

        int a3[] = {7, 7, 7, 7};
        TEST("[7,7,7,7] -> 1 (strictly increasing)")
            EXPECT(longest_increasing_subseq(a3, 4) == 1);

        int a4[] = {1, 2, 3, 4, 5};
        TEST("[1,2,3,4,5] -> 5 (already sorted)")
            EXPECT(longest_increasing_subseq(a4, 5) == 5);
    }

    /* ── edit_distance tests ──────────────────────────────────── */
    printf("\nedit_distance:\n");
    {
        TEST("'kitten' -> 'sitting' = 3")
            EXPECT(edit_distance("kitten", "sitting") == 3);
        TEST("'' -> 'abc' = 3")
            EXPECT(edit_distance("", "abc") == 3);
        TEST("'abc' -> 'abc' = 0")
            EXPECT(edit_distance("abc", "abc") == 0);
        TEST("'horse' -> 'ros' = 3")
            EXPECT(edit_distance("horse", "ros") == 3);
    }

    /* ── longest_palindromic_subseq tests ──────────────────────── */
    printf("\nlongest_palindromic_subseq:\n");
    {
        TEST("'bbbab' -> 4")
            EXPECT(longest_palindromic_subseq("bbbab") == 4);
        TEST("'cbbd' -> 2")
            EXPECT(longest_palindromic_subseq("cbbd") == 2);
        TEST("'a' -> 1")
            EXPECT(longest_palindromic_subseq("a") == 1);
    }

    /* ── word_break tests ─────────────────────────────────────── */
    printf("\nword_break:\n");
    {
        const char *d1[] = {"leet", "code"};
        TEST("'leetcode' with [leet,code] -> true")
            EXPECT(word_break("leetcode", d1, 2) == 1);

        const char *d2[] = {"apple", "pen"};
        TEST("'applepenapple' with [apple,pen] -> true")
            EXPECT(word_break("applepenapple", d2, 2) == 1);

        const char *d3[] = {"cats", "dog", "sand", "and", "cat"};
        TEST("'catsandog' with [cats,dog,sand,and,cat] -> false")
            EXPECT(word_break("catsandog", d3, 5) == 0);

        const char *d4[] = {"a", "b"};
        TEST("'ab' with [a,b] -> true")
            EXPECT(word_break("ab", d4, 2) == 1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
