/*
 * Solution: Top-K and K-Way Merge (Heap Applications)
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

/* ── top_k_frequent ──────────────────────────────────────────────── */

#define MAX_VAL 1000

/* Pair heap: min-heap ordered by frequency, storing (freq, value) */
typedef struct {
    int freq;
    int value;
} FreqPair;

typedef struct {
    FreqPair data[HEAP_CAP];
    int size;
} FreqMinHeap;

static void fheap_init(FreqMinHeap *h) { h->size = 0; }

static void fheap_push(FreqMinHeap *h, int freq, int value)
{
    h->data[h->size] = (FreqPair){freq, value};
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i].freq < h->data[p].freq) {
            FreqPair tmp = h->data[i];
            h->data[i] = h->data[p];
            h->data[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static FreqPair fheap_pop(FreqMinHeap *h)
{
    FreqPair val = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < h->size && h->data[l].freq < h->data[s].freq) s = l;
        if (r < h->size && h->data[r].freq < h->data[s].freq) s = r;
        if (s != i) {
            FreqPair tmp = h->data[i];
            h->data[i] = h->data[s];
            h->data[s] = tmp;
            i = s;
        } else {
            break;
        }
    }
    return val;
}

int top_k_frequent(int *arr, int n, int k, int *result)
{
    /* Step 1: count frequencies */
    int freq[MAX_VAL];
    memset(freq, 0, sizeof(freq));
    for (int i = 0; i < n; i++) {
        freq[arr[i]]++;
    }

    /* Step 2: use a min-heap of size k ordered by frequency */
    FreqMinHeap h;
    fheap_init(&h);

    for (int v = 0; v < MAX_VAL; v++) {
        if (freq[v] == 0) continue;
        if (h.size < k) {
            fheap_push(&h, freq[v], v);
        } else if (freq[v] > h.data[0].freq) {
            fheap_pop(&h);
            fheap_push(&h, freq[v], v);
        }
    }

    /* Step 3: extract results */
    int count = h.size;
    for (int i = 0; i < count; i++) {
        FreqPair p = fheap_pop(&h);
        result[i] = p.value;
    }
    return count;
}

/* ── kth_largest ─────────────────────────────────────────────────── */

int kth_largest(int *arr, int n, int k)
{
    MinHeap h;
    minheap_init(&h);

    /* Push first k elements */
    for (int i = 0; i < k; i++) {
        minheap_push(&h, arr[i]);
    }

    /* For remaining: if larger than root, replace */
    for (int i = k; i < n; i++) {
        if (arr[i] > minheap_peek(&h)) {
            minheap_pop(&h);
            minheap_push(&h, arr[i]);
        }
    }

    return minheap_peek(&h);
}

/* ── merge_k_sorted ──────────────────────────────────────────────── */

typedef struct {
    int value;
    int array_idx;
    int elem_idx;
} MergeEntry;

typedef struct {
    MergeEntry data[HEAP_CAP];
    int size;
} MergeHeap;

static void mheap_init(MergeHeap *h) { h->size = 0; }

static void mheap_push(MergeHeap *h, MergeEntry e)
{
    h->data[h->size] = e;
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i].value < h->data[p].value) {
            MergeEntry tmp = h->data[i];
            h->data[i] = h->data[p];
            h->data[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static MergeEntry mheap_pop(MergeHeap *h)
{
    MergeEntry val = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < h->size && h->data[l].value < h->data[s].value) s = l;
        if (r < h->size && h->data[r].value < h->data[s].value) s = r;
        if (s != i) {
            MergeEntry tmp = h->data[i];
            h->data[i] = h->data[s];
            h->data[s] = tmp;
            i = s;
        } else {
            break;
        }
    }
    return val;
}

int merge_k_sorted(int **arrays, int *sizes, int k, int *output)
{
    MergeHeap h;
    mheap_init(&h);

    /* Push the first element of each array */
    for (int i = 0; i < k; i++) {
        if (sizes[i] > 0) {
            MergeEntry e = {arrays[i][0], i, 0};
            mheap_push(&h, e);
        }
    }

    int out_idx = 0;
    while (h.size > 0) {
        MergeEntry e = mheap_pop(&h);
        output[out_idx++] = e.value;

        /* Push next element from the same array */
        int next = e.elem_idx + 1;
        if (next < sizes[e.array_idx]) {
            MergeEntry ne = {arrays[e.array_idx][next], e.array_idx, next};
            mheap_push(&h, ne);
        }
    }

    return out_idx;
}

/* ── find_median_stream ──────────────────────────────────────────── */

void find_median_stream(int *stream, int n, int *medians)
{
    MaxHeap lower;  /* max-heap: stores the smaller half */
    MinHeap upper;  /* min-heap: stores the larger half */
    maxheap_init(&lower);
    minheap_init(&upper);

    for (int i = 0; i < n; i++) {
        int val = stream[i];

        /* Decide which heap to push to */
        if (lower.size == 0 || val <= maxheap_peek(&lower)) {
            maxheap_push(&lower, val);
        } else {
            minheap_push(&upper, val);
        }

        /* Rebalance: lower can have at most 1 more element than upper */
        if (lower.size > upper.size + 1) {
            minheap_push(&upper, maxheap_pop(&lower));
        } else if (upper.size > lower.size) {
            maxheap_push(&lower, minheap_pop(&upper));
        }

        /* Compute median * 10 to avoid floating point */
        if (lower.size == upper.size) {
            medians[i] = (maxheap_peek(&lower) + minheap_peek(&upper)) * 10 / 2;
        } else {
            medians[i] = maxheap_peek(&lower) * 10;
        }
    }
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
