/*
 * Kata: Top-K and K-Way Merge (Heap Applications)
 *
 * These are the most common heap application patterns.  The core idea:
 * when you need the "k best/worst" of something, think heap.
 *
 * Why min-heap for "top k largest"?
 *   The root of a min-heap of size k is the SMALLEST of the k largest
 *   elements seen so far.  Any new element bigger than the root deserves
 *   a spot — pop the root and push the new element.
 *
 * 15 tests total.
 *
 * Hints:
 *   - Use the provided MinHeap / MaxHeap helpers (fixed-capacity, inline)
 *   - For top_k_frequent: first count frequencies, then use a min-heap
 *   - For kth_largest: min-heap of size k, root is the answer
 *   - For merge_k_sorted: min-heap of size k, track which array each came from
 *   - For find_median_stream: two heaps — max-heap (lower half), min-heap (upper half)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Simple MinHeap helper ───────────────────────────────────────── */

#define HEAP_CAP 1024

typedef struct {
    int data[HEAP_CAP];
    int size;
} MinHeap;

static void minheap_init(MinHeap *h) { h->size = 0; }

static void minheap_push(MinHeap *h, int val)
{
    h->data[h->size] = val;
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i] < h->data[p]) {
            int tmp = h->data[i]; h->data[i] = h->data[p]; h->data[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static int minheap_pop(MinHeap *h)
{
    int val = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < h->size && h->data[l] < h->data[s]) s = l;
        if (r < h->size && h->data[r] < h->data[s]) s = r;
        if (s != i) {
            int tmp = h->data[i]; h->data[i] = h->data[s]; h->data[s] = tmp;
            i = s;
        } else {
            break;
        }
    }
    return val;
}

static int minheap_peek(MinHeap *h) { return h->data[0]; }

/* ── Simple MaxHeap helper ───────────────────────────────────────── */

typedef struct {
    int data[HEAP_CAP];
    int size;
} MaxHeap;

static void maxheap_init(MaxHeap *h) { h->size = 0; }

static void maxheap_push(MaxHeap *h, int val)
{
    h->data[h->size] = val;
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i] > h->data[p]) {
            int tmp = h->data[i]; h->data[i] = h->data[p]; h->data[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static int maxheap_pop(MaxHeap *h)
{
    int val = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < h->size && h->data[l] > h->data[s]) s = l;
        if (r < h->size && h->data[r] > h->data[s]) s = r;
        if (s != i) {
            int tmp = h->data[i]; h->data[i] = h->data[s]; h->data[s] = tmp;
            i = s;
        } else {
            break;
        }
    }
    return val;
}

static int maxheap_peek(MaxHeap *h) { return h->data[0]; }

/* ── Exercise 1: top_k_frequent ──────────────────────────────────
 * Given an array of integers and k, return the k most frequent elements
 * in the result array.
 *
 * Strategy:
 *   1. Count frequencies.  Since values can be arbitrary, use a simple
 *      approach: sort a copy, then count runs (or use a small hash).
 *      For simplicity here, assume values are in range [0, 999].
 *   2. Use a min-heap of size k to track the top k frequent elements.
 *      Each heap entry stores a frequency.  If a new element's frequency
 *      is greater than the heap's root frequency, replace it.
 *
 * Parameters:
 *   - arr, n: input array and its size
 *   - k: how many top frequent elements to return
 *   - result: output array (must hold k elements)
 *
 * Return: number of elements written to result (should be k).
 *
 * Note: result order doesn't matter for the tests.
 */
#define MAX_VAL 1000

int top_k_frequent(int *arr, int n, int k, int *result)
{
    /* TODO:
     * 1. Count frequencies in a freq[MAX_VAL] array
     * 2. Use a min-heap of size k (track both freq and value)
     *    - Push first k unique elements
     *    - For remaining: if freq > heap root's freq, replace
     * 3. Pop all k elements into result
     */
    (void)arr; (void)n; (void)k; (void)result;
    return 0; /* placeholder */
}

/* ── Exercise 2: kth_largest ─────────────────────────────────────
 * Find the kth largest element in an unsorted array.
 *
 * Strategy: maintain a min-heap of size k.
 *   - Insert the first k elements.
 *   - For each remaining element: if it's larger than the heap root
 *     (the smallest of the k largest), pop the root and push the new one.
 *   - After processing all elements, the root is the kth largest.
 */
int kth_largest(int *arr, int n, int k)
{
    /* TODO: min-heap of size k, process all elements, return root */
    (void)arr; (void)n; (void)k;
    return 0; /* placeholder */
}

/* ── Exercise 3: merge_k_sorted ──────────────────────────────────
 * Merge k sorted arrays into one sorted array.
 *
 * Strategy: use a min-heap of size k.
 *   - Initially push the first element of each array into the heap.
 *     Each heap entry needs: value, which array it came from, and
 *     the index of the next element in that array.
 *   - Repeatedly pop the smallest, add it to the output, and push
 *     the next element from the same array (if any remain).
 *
 * Parameters:
 *   - arrays: array of k pointers to sorted arrays
 *   - sizes: size of each array
 *   - k: number of arrays
 *   - output: pre-allocated array large enough for all elements
 *
 * Return: total number of elements written.
 *
 * Hint: you'll need a custom heap entry struct with (value, array_idx,
 *       elem_idx).  You can write a small specialized heap or adapt MinHeap.
 */

typedef struct {
    int value;
    int array_idx;
    int elem_idx;
} MergeEntry;

int merge_k_sorted(int **arrays, int *sizes, int k, int *output)
{
    /* TODO: min-heap of MergeEntry, merge all arrays */
    (void)arrays; (void)sizes; (void)k; (void)output;
    return 0; /* placeholder */
}

/* ── Exercise 4: find_median_stream ──────────────────────────────
 * Find the median after each insertion in a stream of numbers.
 *
 * Strategy: use TWO heaps:
 *   - max-heap (lower half): stores the smaller half of numbers
 *   - min-heap (upper half): stores the larger half of numbers
 *
 * Invariants to maintain:
 *   - Every element in max-heap <= every element in min-heap
 *   - Size difference between heaps is at most 1
 *   - max-heap may have one extra element (when total count is odd)
 *
 * To add a number:
 *   1. If it's <= max-heap root (or max-heap is empty), push to max-heap.
 *      Otherwise push to min-heap.
 *   2. Rebalance: if one heap has 2+ more elements than the other,
 *      move the root of the larger heap to the smaller one.
 *
 * Median:
 *   - If heaps are same size: average of both roots
 *   - If max-heap is larger: max-heap root
 *   - If min-heap is larger: min-heap root
 *
 * Parameters:
 *   - stream: array of numbers arriving one at a time
 *   - n: total numbers in stream
 *   - medians: output array where medians[i] = median * 10
 *              (multiply by 10 to avoid floating point: 4.0 → 40, 4.5 → 45)
 */
void find_median_stream(int *stream, int n, int *medians)
{
    /* TODO: maintain two heaps, compute median after each insertion */
    (void)stream; (void)n; (void)medians;
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

/* Helper: check if result contains a value */
static int contains(int *arr, int n, int val)
{
    for (int i = 0; i < n; i++) {
        if (arr[i] == val) return 1;
    }
    return 0;
}

int main(void)
{
    printf("=== Top-K and K-Way Merge (Heap Applications) ===\n\n");

    /* --- top_k_frequent --- */
    printf("top_k_frequent:\n");
    {
        int arr[] = {1, 1, 1, 2, 2, 3};
        int result[2];
        int count = top_k_frequent(arr, 6, 2, result);
        TEST("top 2 frequent of [1,1,1,2,2,3]")
            EXPECT(count == 2 && contains(result, 2, 1) &&
                   contains(result, 2, 2));
    }
    {
        int arr[] = {1};
        int result[1];
        int count = top_k_frequent(arr, 1, 1, result);
        TEST("top 1 frequent of [1]")
            EXPECT(count == 1 && result[0] == 1);
    }
    {
        int arr[] = {4, 4, 4, 6, 6, 6, 2, 2, 9};
        int result[2];
        int count = top_k_frequent(arr, 9, 2, result);
        TEST("top 2 frequent of [4,4,4,6,6,6,2,2,9]")
            EXPECT(count == 2 && contains(result, 2, 4) &&
                   contains(result, 2, 6));
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        int result[3];
        int count = top_k_frequent(arr, 5, 3, result);
        TEST("top 3 frequent when all freq=1")
            EXPECT(count == 3);
    }

    /* --- kth_largest --- */
    printf("\nkth_largest:\n");
    {
        int arr[] = {3, 2, 1, 5, 6, 4};
        TEST("2nd largest of [3,2,1,5,6,4] = 5")
            EXPECT(kth_largest(arr, 6, 2) == 5);
    }
    {
        int arr[] = {3, 2, 3, 1, 2, 4, 5, 5, 6};
        TEST("4th largest of [3,2,3,1,2,4,5,5,6] = 4")
            EXPECT(kth_largest(arr, 9, 4) == 4);
    }
    {
        int arr[] = {7};
        TEST("1st largest of [7] = 7")
            EXPECT(kth_largest(arr, 1, 1) == 7);
    }
    {
        int arr[] = {1, 1, 1, 1};
        TEST("1st largest of [1,1,1,1] = 1")
            EXPECT(kth_largest(arr, 4, 1) == 1);
    }

    /* --- merge_k_sorted --- */
    printf("\nmerge_k_sorted:\n");
    {
        int a[] = {1, 4, 5};
        int b[] = {1, 3, 4};
        int c[] = {2, 6};
        int *arrays[] = {a, b, c};
        int sizes[] = {3, 3, 2};
        int output[8];
        int total = merge_k_sorted(arrays, sizes, 3, output);
        int expected[] = {1, 1, 2, 3, 4, 4, 5, 6};
        int ok = (total == 8);
        for (int i = 0; i < 8 && ok; i++) {
            if (output[i] != expected[i]) ok = 0;
        }
        TEST("merge [[1,4,5],[1,3,4],[2,6]]")
            EXPECT(ok);
    }
    {
        int a[] = {1};
        int *arrays[] = {a};
        int sizes[] = {1};
        int output[1];
        int total = merge_k_sorted(arrays, sizes, 1, output);
        TEST("merge single array [[1]]")
            EXPECT(total == 1 && output[0] == 1);
    }
    {
        int a[] = {1, 2, 3};
        int b[] = {4, 5, 6};
        int *arrays[] = {a, b};
        int sizes[] = {3, 3};
        int output[6];
        int total = merge_k_sorted(arrays, sizes, 2, output);
        int ok = (total == 6);
        for (int i = 0; i < 6 && ok; i++) {
            if (output[i] != i + 1) ok = 0;
        }
        TEST("merge [[1,2,3],[4,5,6]]")
            EXPECT(ok);
    }

    /* --- find_median_stream --- */
    printf("\nfind_median_stream:\n");
    {
        int stream[] = {5};
        int medians[1];
        find_median_stream(stream, 1, medians);
        TEST("stream [5] -> median 50 (5.0)")
            EXPECT(medians[0] == 50);
    }
    {
        int stream[] = {5, 3};
        int medians[2];
        find_median_stream(stream, 2, medians);
        TEST("stream [5,3] -> median 40 (4.0)")
            EXPECT(medians[1] == 40);
    }
    {
        int stream[] = {5, 3, 8};
        int medians[3];
        find_median_stream(stream, 3, medians);
        TEST("stream [5,3,8] -> median 50 (5.0)")
            EXPECT(medians[2] == 50);
    }
    {
        int stream[] = {5, 3, 8, 1};
        int medians[4];
        find_median_stream(stream, 4, medians);
        TEST("stream [5,3,8,1] -> median 40 (4.0)")
            EXPECT(medians[3] == 40);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
