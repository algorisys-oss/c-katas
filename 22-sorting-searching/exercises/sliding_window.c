/*
 * Kata: Sliding Window Technique
 *
 * The sliding window technique maintains a "window" (a contiguous
 * subarray or substring) and slides it across the data. Instead of
 * recomputing from scratch for each position, we update incrementally:
 * remove the element leaving the window, add the element entering.
 *
 * Two types:
 *   1. Fixed-size window: window always has exactly k elements
 *   2. Variable-size window: window grows and shrinks based on a condition
 *
 * ASCII art — fixed window sliding across an array:
 *
 *   Array: [ 2 | 1 | 5 | 1 | 3 | 2 ]     k = 3
 *
 *   Window 1: [2, 1, 5]  sum = 8
 *              ─────────
 *   Window 2:    [1, 5, 1]  sum = 8 - 2 + 1 = 7
 *                 ─────────
 *   Window 3:       [5, 1, 3]  sum = 7 - 1 + 3 = 9  ← max!
 *                    ─────────
 *   Window 4:          [1, 3, 2]  sum = 9 - 5 + 2 = 6
 *                       ─────────
 *
 *   Key: we never recompute the full sum. We subtract what leaves
 *   and add what enters. O(n) instead of O(n*k).
 *
 * ASCII art — variable window for longest substring without repeats:
 *
 *   String: "abcabcbb"
 *   freq[128] tracks character counts in the current window.
 *
 *   Step 1: left=0, right=0  window="a"     freq[a]=1  len=1
 *   Step 2: left=0, right=1  window="ab"    freq[b]=1  len=2
 *   Step 3: left=0, right=2  window="abc"   freq[c]=1  len=3
 *   Step 4: left=0, right=3  window="abca"  freq[a]=2  DUPLICATE!
 *           → shrink: remove arr[left], left++
 *           left=1, right=3  window="bca"   freq[a]=1  len=3
 *   Step 5: left=1, right=4  window="bcab"  freq[b]=2  DUPLICATE!
 *           → shrink: remove arr[left], left++
 *           left=2, right=4  window="cab"   freq[b]=1  len=3
 *   ...and so on. Maximum length seen = 3.
 *
 * Exercises:
 *   - max_sum_subarray_fixed   (fixed window)
 *   - longest_substr_no_repeat (variable window)
 *   - min_subarray_sum         (variable window)
 *
 * 13 tests total.
 *
 * Hints:
 *   - For fixed window: compute initial window sum, then slide
 *   - For variable window: expand right until condition breaks,
 *     then shrink left until condition is restored
 *   - Use int freq[128] for character frequency (covers all ASCII)
 */

#include <stdio.h>
#include <string.h>

/* ── Exercise 1: Maximum Sum Subarray of Size k ────────────────── *
 * Find the maximum sum of any contiguous subarray of exactly k
 * elements.
 *
 * Approach:
 *   1. Compute the sum of the first k elements
 *   2. Slide the window: subtract arr[i-k], add arr[i]
 *   3. Track the maximum sum seen
 *
 * Example: [2, 1, 5, 1, 3, 2], k=3 → 9 (subarray [5, 1, 3])
 */
int max_sum_subarray_fixed(const int *arr, int n, int k)
{
    /* TODO: Implement fixed-size sliding window */
    (void)arr; (void)n; (void)k;
    return 0;
}

/* ── Exercise 2: Longest Substring Without Repeating Characters ── *
 * Given a string, find the length of the longest substring that
 * contains no repeated characters.
 *
 * Use a variable-size window with a frequency array:
 *   - int freq[128] = {0};  (one slot per ASCII character)
 *   - Expand right: add s[right] to freq
 *   - If freq[s[right]] > 1: shrink from left until no duplicates
 *   - Track the maximum window size
 *
 * Examples:
 *   "abcabcbb" → 3 ("abc")
 *   "bbbbb"    → 1 ("b")
 *   "pwwkew"   → 3 ("wke")
 */
int longest_substr_no_repeat(const char *s)
{
    /* TODO: Implement variable-size sliding window with freq array */
    (void)s;
    return 0;
}

/* ── Exercise 3: Minimum Length Subarray with Sum >= Target ─────── *
 * Find the minimum length of a contiguous subarray whose sum is
 * greater than or equal to target. Return 0 if no such subarray.
 *
 * Use a variable-size window:
 *   - Expand right to increase the sum
 *   - Once sum >= target, shrink from left to minimize length
 *   - Track the minimum length seen
 *
 * Example: target=7, [2, 3, 1, 2, 4, 3] → 2 (subarray [4, 3])
 */
int min_subarray_sum(const int *arr, int n, int target)
{
    /* TODO: Implement variable-size sliding window */
    (void)arr; (void)n; (void)target;
    return 0;
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

int main(void)
{
    printf("=== Sliding Window Technique ===\n\n");

    /* --- max_sum_subarray_fixed --- */
    printf("max_sum_subarray_fixed:\n");
    {
        int arr[] = {2, 1, 5, 1, 3, 2};
        TEST("k=3 in [2,1,5,1,3,2] → 9") EXPECT(max_sum_subarray_fixed(arr, 6, 3) == 9);
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        TEST("k=2 in [1,2,3,4,5] → 9") EXPECT(max_sum_subarray_fixed(arr, 5, 2) == 9);
    }
    {
        int arr[] = {5, 5, 5, 5};
        TEST("k=1 in [5,5,5,5] → 5") EXPECT(max_sum_subarray_fixed(arr, 4, 1) == 5);
    }
    {
        int arr[] = {3, -1, 4, -1, 5, 9};
        TEST("k=4 with negatives → 17") EXPECT(max_sum_subarray_fixed(arr, 6, 4) == 17);
    }

    /* --- longest_substr_no_repeat --- */
    printf("\nlongest_substr_no_repeat:\n");
    {
        TEST("\"abcabcbb\" → 3") EXPECT(longest_substr_no_repeat("abcabcbb") == 3);
    }
    {
        TEST("\"bbbbb\" → 1") EXPECT(longest_substr_no_repeat("bbbbb") == 1);
    }
    {
        TEST("\"pwwkew\" → 3") EXPECT(longest_substr_no_repeat("pwwkew") == 3);
    }
    {
        TEST("\"\" (empty) → 0") EXPECT(longest_substr_no_repeat("") == 0);
    }
    {
        TEST("\"abcdef\" (all unique) → 6") EXPECT(longest_substr_no_repeat("abcdef") == 6);
    }

    /* --- min_subarray_sum --- */
    printf("\nmin_subarray_sum:\n");
    {
        int arr[] = {2, 3, 1, 2, 4, 3};
        TEST("target=7 → 2 ([4,3])") EXPECT(min_subarray_sum(arr, 6, 7) == 2);
    }
    {
        int arr[] = {1, 1, 1, 1, 1};
        TEST("target=100 impossible → 0") EXPECT(min_subarray_sum(arr, 5, 100) == 0);
    }
    {
        int arr[] = {1, 4, 4};
        TEST("target=4 → 1 (single element)") EXPECT(min_subarray_sum(arr, 3, 4) == 1);
    }
    {
        int arr[] = {10, 2, 3};
        TEST("target=10 → 1") EXPECT(min_subarray_sum(arr, 3, 10) == 1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
