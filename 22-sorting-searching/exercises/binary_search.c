/*
 * Kata: Binary Search Variations
 *
 * Implement binary search and its common variations on sorted arrays:
 *   - binary_search        — find target, return index or -1
 *   - binary_search_first  — find FIRST occurrence of target
 *   - binary_search_last   — find LAST occurrence of target
 *   - binary_search_insert_pos — find where target should be inserted
 *
 * 13 tests total.
 *
 * Hints:
 *   - Use lo + (hi - lo) / 2 to avoid integer overflow
 *   - Use half-open intervals: lo is inclusive, hi is exclusive
 *   - For "first" and "last", do NOT return immediately when you find
 *     the target — keep narrowing the range
 */

#include <stdio.h>
#include <assert.h>

/* ── Exercise 1: basic binary search ────────────────────────────── *
 * Search for target in sorted arr[0..n-1].
 * Return the index if found, or -1 if not present.
 */
int binary_search(const int *arr, int n, int target)
{
    /* TODO: Implement using lo/hi pointers */
    return -1;
}

/* ── Exercise 2: find first occurrence ──────────────────────────── *
 * Search for the FIRST (leftmost) occurrence of target.
 * Return its index, or -1 if not present.
 *
 * Key insight: when you find target, do NOT return — set hi = mid
 *              to keep searching the left half for an earlier one.
 */
int binary_search_first(const int *arr, int n, int target)
{
    /* TODO: Implement — track the best result found so far */
    return -1;
}

/* ── Exercise 3: find last occurrence ───────────────────────────── *
 * Search for the LAST (rightmost) occurrence of target.
 * Return its index, or -1 if not present.
 *
 * Key insight: when you find target, set lo = mid + 1 to keep
 *              searching the right half.
 */
int binary_search_last(const int *arr, int n, int target)
{
    /* TODO: Implement — track the best result found so far */
    return -1;
}

/* ── Exercise 4: find insert position ───────────────────────────── *
 * Find the index where target should be inserted to keep arr sorted.
 * This is the index of the first element >= target.
 * If target is larger than all elements, return n.
 *
 * Examples:
 *   arr = [1, 3, 5, 7], target = 4 → return 2 (insert before 5)
 *   arr = [1, 3, 5, 7], target = 5 → return 2 (insert before existing 5)
 *   arr = [1, 3, 5, 7], target = 8 → return 4 (append at end)
 */
int binary_search_insert_pos(const int *arr, int n, int target)
{
    /* TODO: Implement — find first element >= target */
    return n;
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
