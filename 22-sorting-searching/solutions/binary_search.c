/*
 * Solution: Binary Search Variations
 *
 * Binary search and its common variations on sorted arrays.
 *
 * 13 tests total.
 */

#include <stdio.h>
#include <assert.h>

/* ── basic binary search ────────────────────────────────────────── */
int binary_search(const int *arr, int n, int target)
{
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return -1;
}

/* ── find first occurrence ──────────────────────────────────────── */
int binary_search_first(const int *arr, int n, int target)
{
    int lo = 0, hi = n;
    int result = -1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;       /* Record this find */
            hi = mid;           /* Keep searching left */
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return result;
}

/* ── find last occurrence ───────────────────────────────────────── */
int binary_search_last(const int *arr, int n, int target)
{
    int lo = 0, hi = n;
    int result = -1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;       /* Record this find */
            lo = mid + 1;       /* Keep searching right */
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return result;
}

/* ── find insert position ───────────────────────────────────────── */
int binary_search_insert_pos(const int *arr, int n, int target)
{
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
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
    printf("=== Binary Search Variations ===\n\n");

    /* --- binary_search --- */
    printf("binary_search:\n");
    {
        int arr[] = {1, 3, 5, 7, 9, 11, 13};
        TEST("find 7 in array") EXPECT(binary_search(arr, 7, 7) == 3);
        TEST("find 1 (first element)") EXPECT(binary_search(arr, 7, 1) == 0);
        TEST("find 13 (last element)") EXPECT(binary_search(arr, 7, 13) == 6);
        TEST("target not present") EXPECT(binary_search(arr, 7, 4) == -1);
    }

    /* --- binary_search_first --- */
    printf("\nbinary_search_first:\n");
    {
        int arr[] = {1, 3, 5, 5, 5, 7, 9};
        TEST("first 5 in [1,3,5,5,5,7,9]") EXPECT(binary_search_first(arr, 7, 5) == 2);
        TEST("first 1 (unique, at start)") EXPECT(binary_search_first(arr, 7, 1) == 0);
        TEST("target not present") EXPECT(binary_search_first(arr, 7, 4) == -1);
    }

    /* --- binary_search_last --- */
    printf("\nbinary_search_last:\n");
    {
        int arr[] = {1, 3, 5, 5, 5, 7, 9};
        TEST("last 5 in [1,3,5,5,5,7,9]") EXPECT(binary_search_last(arr, 7, 5) == 4);
        TEST("last 9 (unique, at end)") EXPECT(binary_search_last(arr, 7, 9) == 6);
        TEST("target not present") EXPECT(binary_search_last(arr, 7, 6) == -1);
    }

    /* --- binary_search_insert_pos --- */
    printf("\nbinary_search_insert_pos:\n");
    {
        int arr[] = {1, 3, 5, 7, 9};
        TEST("insert 4 → index 2") EXPECT(binary_search_insert_pos(arr, 5, 4) == 2);
        TEST("insert 0 → index 0 (before all)") EXPECT(binary_search_insert_pos(arr, 5, 0) == 0);
        TEST("insert 10 → index 5 (after all)") EXPECT(binary_search_insert_pos(arr, 5, 10) == 5);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
