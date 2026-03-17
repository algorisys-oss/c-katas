/*
 * Kata: Sorting Algorithms
 *
 * Implement five classic sorting algorithms, each returning the number
 * of comparisons made:
 *   - bubble_sort
 *   - selection_sort
 *   - insertion_sort
 *   - merge_sort
 *   - quick_sort
 *
 * 15 tests total.
 *
 * Hints:
 *   - A "comparison" is any time you compare two array elements
 *   - For merge sort, use malloc for the temporary buffer and free it
 *   - For quicksort, use the last element as the pivot (Lomuto partition)
 *   - Each function sorts arr[] in-place and returns the comparison count
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ── Helper: swap two ints ──────────────────────────────────────── */
static void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ── Helper: check if array is sorted ───────────────────────────── */
static int is_sorted(const int *arr, int n)
{
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) {
            return 0;
        }
    }
    return 1;
}

/* ── Exercise 1: Bubble Sort ────────────────────────────────────── *
 * Walk through the array repeatedly, swapping adjacent elements
 * that are out of order. Stop early if a pass makes no swaps.
 * Return the total number of comparisons.
 */
long bubble_sort(int *arr, int n)
{
    long comparisons = 0;
    /* TODO: Implement bubble sort with early-exit optimization */
    return comparisons;
}

/* ── Exercise 2: Selection Sort ─────────────────────────────────── *
 * For each position i, find the minimum in arr[i..n-1] and swap it
 * into position i.
 * Return the total number of comparisons.
 */
long selection_sort(int *arr, int n)
{
    long comparisons = 0;
    /* TODO: Implement selection sort */
    return comparisons;
}

/* ── Exercise 3: Insertion Sort ─────────────────────────────────── *
 * For each element, slide it left into its correct position in the
 * already-sorted prefix.
 * Return the total number of comparisons.
 */
long insertion_sort(int *arr, int n)
{
    long comparisons = 0;
    /* TODO: Implement insertion sort */
    return comparisons;
}

/* ── Exercise 4: Merge Sort ─────────────────────────────────────── *
 * Split the array in half, recursively sort each half, then merge.
 * Use malloc for the temporary buffer.
 *
 * Hint: write a helper merge_sort_helper(arr, lo, hi, tmp, &comps)
 *       that sorts arr[lo..hi) using tmp as scratch space.
 */

/* TODO: Write the merge helper function */

/* TODO: Write the merge_sort_helper recursive function */

long merge_sort(int *arr, int n)
{
    long comparisons = 0;
    /* TODO: Allocate tmp buffer, call helper, free buffer */
    return comparisons;
}

/* ── Exercise 5: Quicksort ──────────────────────────────────────── *
 * Pick the last element as pivot (Lomuto partition scheme).
 * Partition so everything <= pivot is on the left.
 * Recursively sort each side.
 *
 * Hint: write partition(arr, lo, hi, &comps) that returns pivot index,
 *       and quick_sort_helper(arr, lo, hi, &comps).
 */

/* TODO: Write the partition function (Lomuto scheme) */

/* TODO: Write the quick_sort_helper recursive function */

long quick_sort(int *arr, int n)
{
    long comparisons = 0;
    /* TODO: Call helper */
    return comparisons;
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

/* Helper: copy an array so each sort gets fresh input */
static void copy_arr(int *dst, const int *src, int n)
{
    memcpy(dst, src, (size_t)n * sizeof(int));
}

int main(void)
{
    printf("=== Sorting Algorithms ===\n\n");

    int base[] = {5, 3, 8, 1, 2, 7, 4, 6};
    int n = 8;
    int arr[8];

    /* --- Bubble sort --- */
    printf("bubble_sort:\n");
    {
        copy_arr(arr, base, n);
        long c = bubble_sort(arr, n);
        TEST("sorts correctly") EXPECT(is_sorted(arr, n));
        TEST("comparison count is reasonable") EXPECT(c > 0 && c <= (long)n * n);
    }
    {
        int sorted[] = {1, 2, 3, 4, 5};
        long c = bubble_sort(sorted, 5);
        TEST("already sorted — early exit") EXPECT(is_sorted(sorted, 5) && c <= 5);
    }

    /* --- Selection sort --- */
    printf("\nselection_sort:\n");
    {
        copy_arr(arr, base, n);
        long c = selection_sort(arr, n);
        TEST("sorts correctly") EXPECT(is_sorted(arr, n));
        /* Selection sort always does n*(n-1)/2 comparisons = 28 for n=8 */
        TEST("always n*(n-1)/2 comparisons") EXPECT(c == 28);
    }
    {
        int single[] = {42};
        long c = selection_sort(single, 1);
        TEST("single element") EXPECT(is_sorted(single, 1) && c == 0);
    }

    /* --- Insertion sort --- */
    printf("\ninsertion_sort:\n");
    {
        copy_arr(arr, base, n);
        long c = insertion_sort(arr, n);
        TEST("sorts correctly") EXPECT(is_sorted(arr, n));
        TEST("comparison count is reasonable") EXPECT(c > 0 && c <= (long)n * n);
    }
    {
        int sorted[] = {1, 2, 3, 4, 5};
        long c = insertion_sort(sorted, 5);
        TEST("already sorted — O(n)") EXPECT(is_sorted(sorted, 5) && c == 4);
    }

    /* --- Merge sort --- */
    printf("\nmerge_sort:\n");
    {
        copy_arr(arr, base, n);
        long c = merge_sort(arr, n);
        TEST("sorts correctly") EXPECT(is_sorted(arr, n));
        /* Merge sort: comparisons should be around n*log2(n) ≈ 24 */
        TEST("O(n log n) comparisons") EXPECT(c >= 10 && c <= 30);
    }
    {
        int reverse[] = {8, 7, 6, 5, 4, 3, 2, 1};
        long c = merge_sort(reverse, 8);
        TEST("reverse sorted input") EXPECT(is_sorted(reverse, 8) && c >= 10 && c <= 30);
    }

    /* --- Quicksort --- */
    printf("\nquick_sort:\n");
    {
        copy_arr(arr, base, n);
        long c = quick_sort(arr, n);
        TEST("sorts correctly") EXPECT(is_sorted(arr, n));
        TEST("comparison count is reasonable") EXPECT(c > 0 && c <= (long)n * n);
    }
    {
        int dups[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        long c = quick_sort(dups, 10);
        TEST("handles duplicates") EXPECT(is_sorted(dups, 10) && c > 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
