/*
 * Solution: Sliding Window Technique
 *
 * Three exercises: fixed window, variable window with frequency array,
 * variable window with sum condition.
 *
 * 13 tests total.
 */

#include <stdio.h>
#include <string.h>

/* ── Maximum Sum Subarray of Size k ────────────────────────────── */
int max_sum_subarray_fixed(const int *arr, int n, int k)
{
    /* Compute the sum of the first window */
    int window_sum = 0;
    for (int i = 0; i < k; i++) {
        window_sum += arr[i];
    }
    int max_sum = window_sum;

    /* Slide: remove the element leaving, add the element entering */
    for (int i = k; i < n; i++) {
        window_sum += arr[i] - arr[i - k];
        if (window_sum > max_sum) {
            max_sum = window_sum;
        }
    }
    return max_sum;
}

/* ── Longest Substring Without Repeating Characters ────────────── */
int longest_substr_no_repeat(const char *s)
{
    int freq[128] = {0};
    int max_len = 0;
    int left = 0;
    int n = (int)strlen(s);

    for (int right = 0; right < n; right++) {
        freq[(unsigned char)s[right]]++;

        /* Shrink window from left while we have a duplicate */
        while (freq[(unsigned char)s[right]] > 1) {
            freq[(unsigned char)s[left]]--;
            left++;
        }

        /* Update max length */
        int window_len = right - left + 1;
        if (window_len > max_len) {
            max_len = window_len;
        }
    }
    return max_len;
}

/* ── Minimum Length Subarray with Sum >= Target ────────────────── */
int min_subarray_sum(const int *arr, int n, int target)
{
    int left = 0;
    int sum = 0;
    int min_len = n + 1;  /* Impossible large value as sentinel */

    for (int right = 0; right < n; right++) {
        sum += arr[right];

        /* Shrink window from left while sum is sufficient */
        while (sum >= target) {
            int window_len = right - left + 1;
            if (window_len < min_len) {
                min_len = window_len;
            }
            sum -= arr[left];
            left++;
        }
    }
    return min_len <= n ? min_len : 0;
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
