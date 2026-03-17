/*
 * Kata: Min-Heap & Heap Sort
 *
 * Implement a min-heap (priority queue) and heap sort:
 *   - Create/destroy a heap with a given capacity
 *   - Insert values (bubble up to maintain heap property)
 *   - Extract the minimum (bubble down to maintain heap property)
 *   - Peek at the minimum without removing
 *   - In-place heap sort
 *
 * 14 tests total.
 *
 * Hints:
 *   - For node at index i:  left = 2*i+1, right = 2*i+2, parent = (i-1)/2
 *   - Bubble up: compare with parent, swap if smaller
 *   - Bubble down: compare with both children, swap with smaller child
 *   - Heap sort uses a MAX-heap to sort ascending (build max-heap, then
 *     repeatedly swap root with last element and sift down)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ── Heap definition ──────────────────────────────────────────────── */

typedef struct {
    int *data;      /* dynamic array of elements */
    int size;       /* current number of elements */
    int capacity;   /* maximum capacity */
} Heap;

/* ── Exercise 1: heap_create ──────────────────────────────────────
 * Allocate a Heap struct and its internal array with given capacity.
 * Initialize size to 0.
 * Return the pointer to the new Heap (or NULL if malloc fails).
 */
Heap *heap_create(int capacity)
{
    /* TODO: malloc Heap, malloc data array, set size=0, capacity */
    return NULL; /* placeholder */
}

/* ── Exercise 2: heap_destroy ─────────────────────────────────────
 * Free the internal array and the Heap struct.
 */
void heap_destroy(Heap *h)
{
    /* TODO: free h->data, then free h */
}

/* ── Exercise 3: heap_insert ──────────────────────────────────────
 * Insert a value into the min-heap.
 *
 * Steps:
 *   1. Place the value at index h->size, then increment size.
 *   2. Bubble up: while the new element is smaller than its parent,
 *      swap them.
 */
void heap_insert(Heap *h, int value)
{
    /* TODO: Add at end, bubble up */
}

/* ── Exercise 4: heap_extract_min ─────────────────────────────────
 * Remove and return the minimum element (root).
 *
 * Steps:
 *   1. Save the root value.
 *   2. Move the last element to the root, decrement size.
 *   3. Bubble down: while the element is larger than either child,
 *      swap with the smaller child.
 *
 * Precondition: h->size > 0 (assert this).
 */
int heap_extract_min(Heap *h)
{
    /* TODO: Save root, move last to root, bubble down, return saved value */
    return 0; /* placeholder */
}

/* ── Exercise 5: heap_peek ────────────────────────────────────────
 * Return the minimum element without removing it.
 * Precondition: h->size > 0 (assert this).
 */
int heap_peek(Heap *h)
{
    /* TODO: Return h->data[0] */
    return 0; /* placeholder */
}

/* ── Exercise 6: heap_size ────────────────────────────────────────
 * Return the current number of elements in the heap.
 */
int heap_size(Heap *h)
{
    /* TODO: Return h->size */
    return 0; /* placeholder */
}

/* ── Exercise 7: heap_sort ────────────────────────────────────────
 * Sort an array in ascending order using heap sort (in-place).
 *
 * Algorithm (uses a MAX-heap):
 *   1. Build a max-heap from the array (heapify from the last
 *      non-leaf node down to index 0).
 *   2. Repeatedly swap arr[0] (max) with the last unsorted element,
 *      shrink the heap by 1, and sift down the new root.
 *
 * Helper needed: a sift_down function that takes (arr, heap_size, index)
 * and sifts the element at `index` down to its correct position
 * using MAX-heap comparisons (parent >= children).
 */
void heap_sort(int *arr, int n)
{
    /* TODO: Build max-heap, then extract elements from end */
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
    printf("=== Min-Heap & Heap Sort ===\n\n");

    /* --- Create & basic operations --- */
    printf("create & basic ops:\n");
    Heap *h = heap_create(16);
    TEST("heap created with size 0")
        EXPECT(h != NULL && heap_size(h) == 0);

    /* --- Insert tests --- */
    printf("\ninsert:\n");
    heap_insert(h, 50);
    heap_insert(h, 30);
    heap_insert(h, 70);
    heap_insert(h, 10);
    heap_insert(h, 40);

    TEST("after 5 inserts, size is 5")
        EXPECT(heap_size(h) == 5);

    TEST("peek returns minimum (10)")
        EXPECT(heap_peek(h) == 10);

    /* --- Extract min tests --- */
    printf("\nextract_min:\n");
    {
        int v1 = heap_extract_min(h);
        TEST("first extract gives 10")
            EXPECT(v1 == 10);

        int v2 = heap_extract_min(h);
        TEST("second extract gives 30")
            EXPECT(v2 == 30);

        TEST("size after 2 extracts is 3")
            EXPECT(heap_size(h) == 3);

        TEST("peek after 2 extracts is 40")
            EXPECT(heap_peek(h) == 40);
    }

    /* --- Extract remaining elements in order --- */
    printf("\nextract all remaining:\n");
    {
        int v3 = heap_extract_min(h);
        int v4 = heap_extract_min(h);
        int v5 = heap_extract_min(h);
        TEST("remaining extracted in order: 40, 50, 70")
            EXPECT(v3 == 40 && v4 == 50 && v5 == 70);
        TEST("heap is empty after extracting all")
            EXPECT(heap_size(h) == 0);
    }

    /* --- Insert after empty --- */
    printf("\ninsert after empty:\n");
    heap_insert(h, 5);
    heap_insert(h, 3);
    heap_insert(h, 8);
    TEST("insert into emptied heap: peek is 3")
        EXPECT(heap_peek(h) == 3);
    TEST("insert into emptied heap: size is 3")
        EXPECT(heap_size(h) == 3);

    heap_destroy(h);

    /* --- Heap sort tests --- */
    printf("\nheap_sort:\n");
    {
        int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
        int n = 9;
        heap_sort(arr, n);
        int sorted = 1;
        for (int i = 1; i < n; i++) {
            if (arr[i] < arr[i - 1]) {
                sorted = 0;
                break;
            }
        }
        TEST("heap sort 9 elements")
            EXPECT(sorted && arr[0] == 1 && arr[8] == 9);
    }
    {
        int arr[] = {1};
        heap_sort(arr, 1);
        TEST("heap sort single element")
            EXPECT(arr[0] == 1);
    }
    {
        int arr[] = {5, 5, 3, 3, 1, 1};
        heap_sort(arr, 6);
        TEST("heap sort with duplicates")
            EXPECT(arr[0] == 1 && arr[1] == 1 && arr[2] == 3 &&
                   arr[3] == 3 && arr[4] == 5 && arr[5] == 5);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
