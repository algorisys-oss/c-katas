/*
 * Solution: Min-Heap & Heap Sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ── Heap definition ──────────────────────────────────────────────── */

typedef struct {
    int *data;
    int size;
    int capacity;
} Heap;

/* ── heap_create ──────────────────────────────────────────────────── */

Heap *heap_create(int capacity)
{
    Heap *h = malloc(sizeof(Heap));
    if (h == NULL) {
        return NULL;
    }
    h->data = malloc(sizeof(int) * capacity);
    if (h->data == NULL) {
        free(h);
        return NULL;
    }
    h->size = 0;
    h->capacity = capacity;
    return h;
}

/* ── heap_destroy ─────────────────────────────────────────────────── */

void heap_destroy(Heap *h)
{
    if (h == NULL) {
        return;
    }
    free(h->data);
    free(h);
}

/* ── swap helper ──────────────────────────────────────────────────── */

static void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ── heap_insert ──────────────────────────────────────────────────── */

void heap_insert(Heap *h, int value)
{
    assert(h->size < h->capacity);

    /* Place at end */
    h->data[h->size] = value;
    h->size++;

    /* Bubble up */
    int i = h->size - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[i] < h->data[parent]) {
            swap(&h->data[i], &h->data[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

/* ── heap_extract_min ─────────────────────────────────────────────── */

int heap_extract_min(Heap *h)
{
    assert(h->size > 0);

    int min_val = h->data[0];

    /* Move last element to root */
    h->size--;
    h->data[0] = h->data[h->size];

    /* Bubble down */
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < h->size && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }
        if (smallest != i) {
            swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }

    return min_val;
}

/* ── heap_peek ────────────────────────────────────────────────────── */

int heap_peek(Heap *h)
{
    assert(h->size > 0);
    return h->data[0];
}

/* ── heap_size ────────────────────────────────────────────────────── */

int heap_size(Heap *h)
{
    return h->size;
}

/* ── heap_sort (max-heap based, ascending order) ──────────────────── */

static void sift_down_max(int *arr, int n, int i)
{
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }
        if (largest != i) {
            swap(&arr[i], &arr[largest]);
            i = largest;
        } else {
            break;
        }
    }
}

void heap_sort(int *arr, int n)
{
    /* Build max-heap (heapify from last non-leaf down to 0) */
    for (int i = n / 2 - 1; i >= 0; i--) {
        sift_down_max(arr, n, i);
    }

    /* Extract elements one by one */
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        sift_down_max(arr, i, 0);
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
