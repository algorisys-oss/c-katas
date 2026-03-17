/*
 * Solution: Sorting Algorithms
 *
 * Five classic sorting algorithms, each returning the number of
 * comparisons made.
 *
 * 15 tests total.
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

/* ── Bubble Sort ────────────────────────────────────────────────── */
long bubble_sort(int *arr, int n)
{
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) {
            break;  /* Early exit: already sorted */
        }
    }
    return comparisons;
}

/* ── Selection Sort ─────────────────────────────────────────────── */
long selection_sort(int *arr, int n)
{
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
    return comparisons;
}

/* ── Insertion Sort ─────────────────────────────────────────────── */
long insertion_sort(int *arr, int n)
{
    long comparisons = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

/* ── Merge Sort ─────────────────────────────────────────────────── */

static void merge(int *arr, int lo, int mid, int hi, int *tmp, long *comps)
{
    /* Copy arr[lo..hi) into tmp */
    for (int i = lo; i < hi; i++) {
        tmp[i] = arr[i];
    }

    int i = lo;      /* pointer into left half */
    int j = mid;     /* pointer into right half */
    int k = lo;      /* write pointer into arr */

    while (i < mid && j < hi) {
        (*comps)++;
        if (tmp[i] <= tmp[j]) {
            arr[k++] = tmp[i++];
        } else {
            arr[k++] = tmp[j++];
        }
    }
    /* Copy remaining elements */
    while (i < mid) {
        arr[k++] = tmp[i++];
    }
    while (j < hi) {
        arr[k++] = tmp[j++];
    }
}

static void merge_sort_helper(int *arr, int lo, int hi, int *tmp, long *comps)
{
    if (hi - lo <= 1) {
        return;  /* Base case: 0 or 1 elements */
    }
    int mid = lo + (hi - lo) / 2;
    merge_sort_helper(arr, lo, mid, tmp, comps);
    merge_sort_helper(arr, mid, hi, tmp, comps);
    merge(arr, lo, mid, hi, tmp, comps);
}

long merge_sort(int *arr, int n)
{
    long comparisons = 0;
    if (n <= 1) {
        return 0;
    }
    int *tmp = malloc((size_t)n * sizeof(int));
    if (!tmp) {
        fprintf(stderr, "merge_sort: malloc failed\n");
        exit(1);
    }
    merge_sort_helper(arr, 0, n, tmp, &comparisons);
    free(tmp);
    return comparisons;
}

/* ── Quicksort ──────────────────────────────────────────────────── */

static int partition(int *arr, int lo, int hi, long *comps)
{
    int pivot = arr[hi];  /* Last element as pivot */
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        (*comps)++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[hi]);
    return i + 1;
}

static void quick_sort_helper(int *arr, int lo, int hi, long *comps)
{
    if (lo >= hi) {
        return;
    }
    int pivot_idx = partition(arr, lo, hi, comps);
    quick_sort_helper(arr, lo, pivot_idx - 1, comps);
    quick_sort_helper(arr, pivot_idx + 1, hi, comps);
}

long quick_sort(int *arr, int n)
{
    long comparisons = 0;
    if (n <= 1) {
        return 0;
    }
    quick_sort_helper(arr, 0, n - 1, &comparisons);
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
