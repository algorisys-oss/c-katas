/*
 * Solution: Modified Binary Search
 *
 * Four exercises: rotated array search, peak finding, 2D matrix search,
 * first/last position with duplicates.
 *
 * 17 tests total.
 */

#include <stdio.h>

/* ── Search in Rotated Sorted Array ────────────────────────────── */
int search_rotated(const int *arr, int n, int target)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        /* Determine which half is sorted */
        if (arr[lo] <= arr[mid]) {
            /* Left half [lo..mid] is sorted */
            if (target >= arr[lo] && target < arr[mid]) {
                hi = mid - 1;  /* Target is in the sorted left half */
            } else {
                lo = mid + 1;  /* Target is in the right half */
            }
        } else {
            /* Right half [mid..hi] is sorted */
            if (target > arr[mid] && target <= arr[hi]) {
                lo = mid + 1;  /* Target is in the sorted right half */
            } else {
                hi = mid - 1;  /* Target is in the left half */
            }
        }
    }
    return -1;
}

/* ── Find Peak Element ─────────────────────────────────────────── */
int find_peak(const int *arr, int n)
{
    int lo = 0, hi = n - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < arr[mid + 1]) {
            /* Rising slope: peak must be to the right */
            lo = mid + 1;
        } else {
            /* Falling slope: peak is at mid or to the left */
            hi = mid;
        }
    }
    return lo;  /* lo == hi, this is the peak */
}

/* ── Search a 2D Matrix ────────────────────────────────────────── */
int search_2d_matrix(const int *matrix, int rows, int cols, int target)
{
    int lo = 0, hi = rows * cols - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        /* Convert flat index to 2D coordinates */
        int val = matrix[mid / cols * cols + mid % cols];
        if (val == target) {
            return 1;
        } else if (val < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return 0;
}

/* ── Find First and Last Position ──────────────────────────────── */
int find_first_last(const int *arr, int n, int target, int *first, int *last)
{
    /* Find first (leftmost) occurrence */
    int lo = 0, hi = n;
    int f = -1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            f = mid;
            hi = mid;       /* Keep searching left */
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    if (f == -1) {
        return 0;  /* Not found */
    }

    /* Find last (rightmost) occurrence */
    lo = 0;
    hi = n;
    int l = -1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            l = mid;
            lo = mid + 1;   /* Keep searching right */
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    *first = f;
    *last = l;
    return 1;
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
        TEST("peak in [1,3,8,12,4,2]") EXPECT(p >= 0 && p < 6 &&
            (p == 0 || arr[p] > arr[p - 1]) &&
            (p == 5 || arr[p] > arr[p + 1]));
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        int p = find_peak(arr, 5);
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
