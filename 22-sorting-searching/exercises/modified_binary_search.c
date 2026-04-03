/*
 * Kata: Modified Binary Search
 *
 * These are binary search variations that go beyond the basics in
 * binary_search.c. Each one modifies the standard binary search
 * by changing what "which half to search" means.
 *
 * The core invariant: at every step, we KNOW which half contains
 * the answer. The trick is figuring out the right condition.
 *
 * ASCII art — rotated sorted array:
 *
 *   Original sorted: [0, 1, 2, 4, 5, 6, 7]
 *
 *   Rotated by 4:    [4, 5, 6, 7, 0, 1, 2]
 *                     ─────────  ────────
 *                     sorted      sorted
 *                              ^
 *                         rotation point
 *
 *   Key insight: when you pick mid, ONE half is always sorted.
 *   Check if target is in the sorted half. If yes, search there.
 *   If no, search the other half.
 *
 * ASCII art — peak element:
 *
 *   Array: [1, 3, 8, 12, 4, 2]
 *
 *   Values:    12
 *             /  \
 *           8      4
 *          /        \
 *        3            2
 *       /
 *     1
 *
 *   Peak = 12 (greater than both neighbors 8 and 4)
 *   Binary search: if mid < mid+1, peak is to the RIGHT
 *                  if mid > mid+1, peak is to the LEFT (or at mid)
 *
 * ASCII art — 2D matrix search:
 *
 *   Matrix (rows sorted, first element of each row > last of previous):
 *   ┌────┬────┬────┬────┐
 *   │  1 │  3 │  5 │  7 │  row 0
 *   ├────┼────┼────┼────┤
 *   │ 10 │ 11 │ 16 │ 20 │  row 1
 *   ├────┼────┼────┼────┤
 *   │ 23 │ 30 │ 34 │ 50 │  row 2
 *   └────┴────┴────┴────┘
 *
 *   Treat as flat sorted array of 12 elements:
 *   [1, 3, 5, 7, 10, 11, 16, 20, 23, 30, 34, 50]
 *
 *   Index 7 → row = 7/4 = 1, col = 7%4 = 3 → matrix[1][3] = 20
 *
 * ASCII art — first/last with duplicates:
 *
 *   Array: [1, 3, 5, 5, 5, 5, 5, 8, 9]
 *                  ^           ^
 *                first(5)=2  last(5)=6
 *
 *   Two binary searches:
 *     - "Leftmost": when you find target, keep searching LEFT
 *     - "Rightmost": when you find target, keep searching RIGHT
 *
 * Exercises:
 *   - search_rotated    (rotated sorted array)
 *   - find_peak         (peak element)
 *   - search_2d_matrix  (2D matrix as flat array)
 *   - find_first_last   (first and last position of target)
 *
 * 17 tests total.
 *
 * Hints:
 *   - Always use lo + (hi - lo) / 2 to compute mid
 *   - Think carefully about which half is guaranteed to be sorted
 *   - For peak: the array is guaranteed to have a peak (no plateau)
 *   - For 2D: convert flat index to row/col with division and modulo
 */

#include <stdio.h>

/* ── Exercise 1: Search in Rotated Sorted Array ────────────────── *
 * A sorted array has been rotated at some pivot:
 *   [0,1,2,4,5,6,7] rotated by 4 → [4,5,6,7,0,1,2]
 *
 * Search for target. Return its index, or -1 if not found.
 * All elements are unique.
 *
 * Key insight: pick mid. One of [lo..mid] or [mid..hi] is sorted.
 *   - If the LEFT half is sorted (arr[lo] <= arr[mid]):
 *       - Is target in [arr[lo], arr[mid])? → search left
 *       - Otherwise → search right
 *   - If the RIGHT half is sorted:
 *       - Is target in (arr[mid], arr[hi]]? → search right
 *       - Otherwise → search left
 */
int search_rotated(const int *arr, int n, int target)
{
    /* TODO: Modified binary search on a rotated array */
    (void)arr; (void)n; (void)target;
    return -1;
}

/* ── Exercise 2: Find Peak Element ─────────────────────────────── *
 * A peak element is greater than its neighbors.
 * arr[-1] and arr[n] are considered -infinity (so edges can be peaks).
 * Return the INDEX of any peak element.
 *
 * Binary search approach:
 *   - If arr[mid] < arr[mid + 1]: peak is to the right → lo = mid + 1
 *   - Otherwise: peak is at mid or to the left → hi = mid
 *
 * Why this works: if mid+1 is bigger, there MUST be a peak to the
 * right (the values either keep increasing to the edge, or eventually
 * decrease — either way, a peak exists on that side).
 */
int find_peak(const int *arr, int n)
{
    /* TODO: Binary search for a peak */
    (void)arr; (void)n;
    return 0;
}

/* ── Exercise 3: Search a 2D Matrix ────────────────────────────── *
 * Given an m x n matrix where:
 *   - Each row is sorted left to right
 *   - First element of each row > last element of previous row
 *
 * Search for target. Return 1 if found, 0 if not.
 *
 * Treat the matrix as a flat sorted array of m*n elements:
 *   flat_index → row = flat_index / cols, col = flat_index % cols
 *
 * The matrix is passed as a 1D array in row-major order.
 */
int search_2d_matrix(const int *matrix, int rows, int cols, int target)
{
    /* TODO: Binary search treating matrix as flat sorted array */
    (void)matrix; (void)rows; (void)cols; (void)target;
    return 0;
}

/* ── Exercise 4: Find First and Last Position ──────────────────── *
 * Given a sorted array with duplicates, find the first and last
 * index of target. Set *first and *last. Return 1 if found, 0 if not.
 *
 * Run two binary searches:
 *   1. Find FIRST (leftmost): when you find target, set hi = mid
 *   2. Find LAST (rightmost): when you find target, set lo = mid + 1
 *
 * You did this individually in binary_search.c — now combine them.
 */
int find_first_last(const int *arr, int n, int target, int *first, int *last)
{
    /* TODO: Two binary searches — one biased left, one biased right */
    (void)arr; (void)n; (void)target; (void)first; (void)last;
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
    printf("=== Modified Binary Search ===\n\n");

    /* --- search_rotated --- */
    printf("search_rotated:\n");
    {
        int arr[] = {4, 5, 6, 7, 0, 1, 2};
        TEST("find 0 in [4,5,6,7,0,1,2]") EXPECT(search_rotated(arr, 7, 0) == 4);
    }
    {
        int arr[] = {4, 5, 6, 7, 0, 1, 2};
        TEST("find 5 in [4,5,6,7,0,1,2]") EXPECT(search_rotated(arr, 7, 5) == 1);
    }
    {
        int arr[] = {4, 5, 6, 7, 0, 1, 2};
        TEST("find 3 (not present)") EXPECT(search_rotated(arr, 7, 3) == -1);
    }
    {
        int arr[] = {1};
        TEST("single element found") EXPECT(search_rotated(arr, 1, 1) == 0);
    }
    {
        int arr[] = {2, 1};
        TEST("two elements, find 1") EXPECT(search_rotated(arr, 2, 1) == 1);
    }

    /* --- find_peak --- */
    printf("\nfind_peak:\n");
    {
        int arr[] = {1, 3, 8, 12, 4, 2};
        int p = find_peak(arr, 6);
        /* Peak must be greater than both neighbors */
        TEST("peak in [1,3,8,12,4,2]") EXPECT(p >= 0 && p < 6 &&
            (p == 0 || arr[p] > arr[p - 1]) &&
            (p == 5 || arr[p] > arr[p + 1]));
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        int p = find_peak(arr, 5);
        /* Edge element can be a peak (arr[n] = -infinity) */
        TEST("ascending array → peak at end") EXPECT(p == 4);
    }
    {
        int arr[] = {5, 4, 3, 2, 1};
        int p = find_peak(arr, 5);
        TEST("descending array → peak at start") EXPECT(p == 0);
    }
    {
        int arr[] = {42};
        TEST("single element is peak") EXPECT(find_peak(arr, 1) == 0);
    }

    /* --- search_2d_matrix --- */
    printf("\nsearch_2d_matrix:\n");
    {
        /* 3x4 matrix in row-major order */
        int m[] = {1,  3,  5,  7,
                   10, 11, 16, 20,
                   23, 30, 34, 50};
        TEST("find 16 in 3x4 matrix") EXPECT(search_2d_matrix(m, 3, 4, 16));
    }
    {
        int m[] = {1,  3,  5,  7,
                   10, 11, 16, 20,
                   23, 30, 34, 50};
        TEST("find 13 (not present)") EXPECT(!search_2d_matrix(m, 3, 4, 13));
    }
    {
        int m[] = {1,  3,  5,  7,
                   10, 11, 16, 20,
                   23, 30, 34, 50};
        TEST("find 1 (top-left corner)") EXPECT(search_2d_matrix(m, 3, 4, 1));
    }
    {
        int m[] = {1,  3,  5,  7,
                   10, 11, 16, 20,
                   23, 30, 34, 50};
        TEST("find 50 (bottom-right corner)") EXPECT(search_2d_matrix(m, 3, 4, 50));
    }

    /* --- find_first_last --- */
    printf("\nfind_first_last:\n");
    {
        int arr[] = {1, 3, 5, 5, 5, 5, 5, 8, 9};
        int first, last;
        int found = find_first_last(arr, 9, 5, &first, &last);
        TEST("5 in [1,3,5,5,5,5,5,8,9]") EXPECT(found && first == 2 && last == 6);
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        int first, last;
        int found = find_first_last(arr, 5, 3, &first, &last);
        TEST("unique element → first == last") EXPECT(found && first == 2 && last == 2);
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        int first, last;
        int found = find_first_last(arr, 5, 6, &first, &last);
        TEST("target not present") EXPECT(!found);
    }
    {
        int arr[] = {5, 5, 5, 5, 5};
        int first, last;
        int found = find_first_last(arr, 5, 5, &first, &last);
        TEST("all same → first=0, last=4") EXPECT(found && first == 0 && last == 4);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
