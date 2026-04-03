/*
 * Kata: Advanced Dynamic Programming
 *
 * In memoization.c you learned the two key conditions for DP:
 *   1. OVERLAPPING SUBPROBLEMS — same subproblem appears multiple times
 *   2. OPTIMAL SUBSTRUCTURE — optimal solution contains optimal sub-solutions
 *
 * You solved four problems: Fibonacci, stair climbing, coin change, and LCS.
 * Now we tackle five classic DP patterns that appear in interviews everywhere.
 *
 * The five patterns:
 *
 *   1. 0/1 KNAPSACK — for each item, choose to take it or skip it.
 *      2D table: dp[i][w] = best value using items 0..i-1 with capacity w.
 *
 *   2. SEQUENCE DP — dp[i] depends on all dp[j] where j < i.
 *      Example: Longest Increasing Subsequence.
 *
 *   3. TWO-STRING DP — 2D table on two strings (extends LCS from memoization.c).
 *      Example: Edit Distance (Levenshtein distance).
 *
 *   4. INTERVAL DP — dp[i][j] on a substring/subarray range.
 *      Example: Longest Palindromic Subsequence.
 *
 *   5. STRING PARTITION DP — can I split s[0..i] into valid pieces?
 *      Example: Word Break.
 *
 * Edit distance DP table for "kit" -> "sit":
 *
 *          ""  s   i   t
 *      ""   0  1   2   3      <- converting "" to "sit" costs 3 inserts
 *      k    1  1   2   3      <- k!=s: 1+min(0,1,1) = 1
 *      i    2  2   1   2      <- i==i: copy diagonal = 1
 *      t    3  3   2   1      <- t==t: copy diagonal = 1
 *                        ^
 *                   answer = 1 (replace k with s)
 *
 * Exercises:
 *   1. knapsack_01(weights, values, n, cap)  — 0/1 Knapsack
 *   2. longest_increasing_subseq(arr, n)     — LIS (O(n^2))
 *   3. edit_distance(a, b)                   — Levenshtein distance
 *   4. longest_palindromic_subseq(s)         — LPS
 *   5. word_break(s, dict, dict_size)        — string segmentation
 *
 * 19 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_N 1000
#define MAX_STR 100

/* ── Exercise 1: knapsack_01 ──────────────────────────────────────────
 *
 * You have n items. Item i has weight weights[i] and value values[i].
 * Your bag can hold at most 'capacity' total weight.
 * Maximize the total value you can carry. Each item is taken or not (0 or 1).
 *
 * Build a 2D table dp[i][w]:
 *   dp[i][w] = max value achievable using items 0..i-1 with capacity w
 *
 * Filling the table:
 *   For each item i (1 to n) and capacity w (0 to capacity):
 *     Option 1: DON'T take item i → dp[i][w] = dp[i-1][w]
 *     Option 2: TAKE item i (if it fits) → dp[i-1][w - weight_i] + value_i
 *     Take the max.
 *
 * The answer is dp[n][capacity].
 *
 * DP table for weights=[2,3], values=[3,4], capacity=4:
 *
 *        cap:  0   1   2   3   4
 *   item 0:    0   0   0   0   0     <- no items
 *   item 1:    0   0   3   3   3     <- can take item 0 (w=2,v=3) if cap>=2
 *   item 2:    0   0   3   4   4     <- can take item 1 (w=3,v=4) if cap>=3
 *                                         at cap=4: max(3, dp[1][4-3]+4) = max(3,4) = 4
 *                              ^
 *                         but wait: at cap=5 we'd take BOTH for 7.
 *                         With cap=4, best is item 1 alone (value 4).
 */
int knapsack_01(int weights[], int values[], int n, int capacity)
{
    /* TODO: Create dp[n+1][capacity+1] and zero it */

    /* TODO: For each item i from 1 to n:
     *         For each weight w from 0 to capacity:
     *           dp[i][w] = dp[i-1][w]  (don't take item i)
     *           if weights[i-1] <= w:
     *             with_item = dp[i-1][w - weights[i-1]] + values[i-1]
     *             if with_item > dp[i][w]: dp[i][w] = with_item
     */

    /* TODO: Return dp[n][capacity] */

    return 0;  /* placeholder */
}

/* ── Exercise 2: longest_increasing_subseq ────────────────────────────
 *
 * Find the length of the longest strictly increasing subsequence.
 * A subsequence doesn't have to be contiguous — you can skip elements.
 *
 * Example: [10, 9, 2, 5, 3, 7, 101, 18]
 *   LIS: 2, 3, 7, 101 → length 4
 *   (also: 2, 5, 7, 101 → same length)
 *
 * O(n^2) approach:
 *   dp[i] = length of LIS ending at index i
 *   dp[i] = 1 + max(dp[j]) for all j < i where arr[j] < arr[i]
 *   (every element alone is a subsequence of length 1)
 *
 * Steps:
 *   1. Initialize all dp[i] = 1
 *   2. For each i from 1 to n-1:
 *        For each j from 0 to i-1:
 *          if arr[j] < arr[i] and dp[j] + 1 > dp[i]:
 *            dp[i] = dp[j] + 1
 *   3. Return the maximum value in dp[]
 *
 * Building dp[] for [2, 5, 3, 7]:
 *   dp[0] = 1          (just [2])
 *   dp[1] = 2          (2 < 5, so dp[0]+1 = 2 → [2,5])
 *   dp[2] = 2          (2 < 3, so dp[0]+1 = 2 → [2,3])
 *   dp[3] = 3          (2<7→dp[0]+1=2, 5<7→dp[1]+1=3, 3<7→dp[2]+1=3 → [2,5,7])
 *   Answer: max(1,2,2,3) = 3
 */
int longest_increasing_subseq(int arr[], int n)
{
    /* TODO: Handle n == 0 edge case */

    /* TODO: Create dp[] of size n, initialize all to 1 */

    /* TODO: For each i from 1 to n-1:
     *         For each j from 0 to i-1:
     *           if arr[j] < arr[i] and dp[j]+1 > dp[i]:
     *             dp[i] = dp[j] + 1
     */

    /* TODO: Find and return the maximum value in dp[] */

    return 0;  /* placeholder */
}

/* ── Exercise 3: edit_distance ────────────────────────────────────────
 *
 * Given two strings a and b, find the minimum number of operations
 * to convert a into b. Allowed operations:
 *   - INSERT a character
 *   - DELETE a character
 *   - REPLACE a character
 *
 * Example: "kitten" → "sitting"
 *   1. Replace k with s → "sitten"
 *   2. Replace e with i → "sittin"
 *   3. Insert g at end  → "sitting"
 *   Total: 3 operations
 *
 * 2D DP table: dp[i][j] = edit distance of a[0..i-1] and b[0..j-1]
 *
 * Rules:
 *   Base cases:
 *     dp[i][0] = i  (delete all i characters from a)
 *     dp[0][j] = j  (insert all j characters into empty a)
 *   If a[i-1] == b[j-1]:
 *     dp[i][j] = dp[i-1][j-1]      (characters match — no operation needed)
 *   Else:
 *     dp[i][j] = 1 + min(dp[i-1][j],      ← delete from a
 *                         dp[i][j-1],      ← insert into a
 *                         dp[i-1][j-1])    ← replace in a
 *
 * Hint: write a helper min3(a, b, c) that returns the minimum of three ints.
 * Strings are limited to MAX_STR (100) characters.
 */

/* TODO: Write min3(a, b, c) helper */

int edit_distance(const char *a, const char *b)
{
    /* TODO: Get string lengths m and n */

    /* TODO: Create dp[MAX_STR+1][MAX_STR+1] */

    /* TODO: Fill base cases: dp[i][0] = i, dp[0][j] = j */

    /* TODO: Fill table using the rules above */

    /* TODO: Return dp[m][n] */

    return 0;  /* placeholder */
}

/* ── Exercise 4: longest_palindromic_subseq ───────────────────────────
 *
 * Find the length of the longest palindromic subsequence in string s.
 * A palindromic subsequence reads the same forwards and backwards.
 *
 * Example: "bbbab" → "bbbb" (skip the 'a') → length 4
 *
 * Interval DP: dp[i][j] = length of LPS in s[i..j]
 *
 * Rules:
 *   Base: dp[i][i] = 1 (single character is a palindrome)
 *   If s[i] == s[j]: dp[i][j] = dp[i+1][j-1] + 2
 *     (both endpoints match — they extend the inner palindrome)
 *   Else: dp[i][j] = max(dp[i+1][j], dp[i][j-1])
 *     (skip left endpoint, or skip right endpoint — take the better)
 *
 * Fill order: increasing substring length (len = 2, 3, ..., n).
 * For each length, i goes from 0 to n-len, and j = i + len - 1.
 *
 * Answer: dp[0][n-1].
 */
int longest_palindromic_subseq(const char *s)
{
    /* TODO: Get string length n, handle n == 0 */

    /* TODO: Create dp[MAX_STR][MAX_STR] and zero it */

    /* TODO: Base case: dp[i][i] = 1 for all i */

    /* TODO: Fill for increasing lengths (len from 2 to n):
     *         for i from 0 to n-len:
     *           j = i + len - 1
     *           if s[i] == s[j]: dp[i][j] = dp[i+1][j-1] + 2
     *           else: dp[i][j] = max(dp[i+1][j], dp[i][j-1])
     */

    /* TODO: Return dp[0][n-1] */

    return 0;  /* placeholder */
}

/* ── Exercise 5: word_break ───────────────────────────────────────────
 *
 * Given a string s and a dictionary of words (array of strings),
 * return 1 if s can be segmented into a sequence of dictionary words.
 *
 * Example: s = "leetcode", dict = ["leet", "code"]
 *   "leet" + "code" = "leetcode" → return 1
 *
 * Example: s = "catsandog", dict = ["cats", "dog", "sand", "and", "cat"]
 *   "cats" + "and" + "og" — "og" not in dict
 *   "cat" + "sand" + "og" — "og" not in dict
 *   No valid segmentation → return 0
 *
 * 1D DP: dp[i] = 1 if s[0..i-1] can be segmented into dictionary words.
 *
 * dp[0] = 1 (empty prefix is always valid — our starting point).
 * For i from 1 to n:
 *   For each word w in dict:
 *     if len(w) <= i AND dp[i - len(w)] == 1
 *       AND s[i-len(w) .. i-1] matches w:
 *         dp[i] = 1; break
 *
 * The idea: dp[i] is true if there's a word w that ends at position i,
 * AND the prefix before that word (dp[i - len(w)]) is also valid.
 *
 * Use strncmp to compare substrings.
 *
 * Return dp[n] (1 if entire string can be segmented, 0 otherwise).
 */
int word_break(const char *s, const char *dict[], int dict_size)
{
    /* TODO: Get string length n */

    /* TODO: Create dp[MAX_N + 1], zero it, set dp[0] = 1 */

    /* TODO: For i from 1 to n:
     *         For each word w in dict:
     *           wlen = strlen(w)
     *           if wlen <= i AND dp[i - wlen] == 1
     *             AND strncmp(s + i - wlen, w, wlen) == 0:
     *               dp[i] = 1; break
     */

    /* TODO: Return dp[n] */

    return 0;  /* placeholder */
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
