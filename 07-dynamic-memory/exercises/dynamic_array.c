/*
 * Kata: Dynamic Array
 *
 * Build a resizable integer array — the foundation of ArrayList (Java),
 * std::vector (C++), and Vec (Rust). This is one of the most commonly
 * used data structures in all of programming.
 *
 * Operations:
 *   - create/destroy: lifecycle management
 *   - push: add to end (grow if needed)
 *   - pop: remove from end
 *   - get/set: access by index with bounds checking
 *   - size: number of elements currently stored
 *
 * 18 tests total.
 *
 * Hints:
 *   - Start with capacity 4, double when full
 *   - Always bounds-check in get/set (return error for invalid index)
 *   - pop on an empty array should return an error code
 *   - Don't shrink capacity on pop (keep it simple)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DA_INITIAL_CAP 4

typedef struct {
    int *data;       /* heap-allocated buffer */
    int size;        /* number of elements currently stored */
    int capacity;    /* total slots available */
} DynArray;

/* ── Exercise 1: da_create ────────────────────────────────────────────
 * Create a new DynArray with initial capacity DA_INITIAL_CAP.
 *
 * - Allocate a buffer for DA_INITIAL_CAP ints
 * - Set size to 0
 * - Return the initialized struct
 */
DynArray da_create(void)
{
    DynArray da;
    /* TODO: malloc a buffer, set size=0, capacity=DA_INITIAL_CAP */
    da.data = NULL;
    da.size = 0;
    da.capacity = 0;
    return da;
}

/* ── Exercise 2: da_push ──────────────────────────────────────────────
 * Append a value to the end of the array.
 *
 * If the array is full (size == capacity), double the capacity with realloc.
 * Then store the value and increment size.
 *
 * Returns: 0 on success, -1 if realloc fails
 */
int da_push(DynArray *da, int value)
{
    /* TODO: Grow if full, store value at data[size], increment size */
    return -1; /* placeholder */
}

/* ── Exercise 3: da_pop ───────────────────────────────────────────────
 * Remove the last element and store it in *out_value.
 *
 * Returns: 0 on success, -1 if array is empty
 */
int da_pop(DynArray *da, int *out_value)
{
    /* TODO: Check if empty, decrement size, store removed value */
    return -1; /* placeholder */
}

/* ── Exercise 4: da_get ───────────────────────────────────────────────
 * Get the value at the given index and store it in *out_value.
 *
 * Returns: 0 on success, -1 if index is out of bounds
 */
int da_get(const DynArray *da, int index, int *out_value)
{
    /* TODO: Bounds check, then *out_value = da->data[index] */
    return -1; /* placeholder */
}

/* ── Exercise 5: da_set ───────────────────────────────────────────────
 * Set the value at the given index.
 *
 * Returns: 0 on success, -1 if index is out of bounds
 */
int da_set(DynArray *da, int index, int value)
{
    /* TODO: Bounds check, then da->data[index] = value */
    return -1; /* placeholder */
}

/* ── Exercise 6: da_destroy ───────────────────────────────────────────
 * Free the array's heap memory and reset all fields.
 */
void da_destroy(DynArray *da)
{
    /* TODO: free the buffer, set data=NULL, size=0, capacity=0 */
}

/* ── Test Harness ─────────────────────────────────────────────────── */

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
    printf("=== Dynamic Array ===\n\n");

    printf("da_create:\n");
    {
        DynArray da = da_create();
        TEST("initial size is 0") EXPECT(da.size == 0);
        TEST("initial capacity is 4") EXPECT(da.capacity == DA_INITIAL_CAP);
        TEST("data is not NULL") EXPECT(da.data != NULL);
        da_destroy(&da);
    }

    printf("\nda_push:\n");
    {
        DynArray da = da_create();
        da_push(&da, 10);
        da_push(&da, 20);
        da_push(&da, 30);
        TEST("push 3 items → size 3") EXPECT(da.size == 3);

        int val;
        da_get(&da, 0, &val); TEST("first element is 10") EXPECT(val == 10);
        da_get(&da, 2, &val); TEST("third element is 30") EXPECT(val == 30);

        /* Push past initial capacity to trigger realloc */
        da_push(&da, 40);
        da_push(&da, 50);
        TEST("push past capacity → size 5") EXPECT(da.size == 5);
        TEST("capacity doubled to 8") EXPECT(da.capacity == 8);
        da_get(&da, 4, &val); TEST("fifth element is 50") EXPECT(val == 50);
        da_destroy(&da);
    }

    printf("\nda_pop:\n");
    {
        DynArray da = da_create();
        da_push(&da, 100);
        da_push(&da, 200);
        int val;
        int rc = da_pop(&da, &val);
        TEST("pop → returns 0 (success)") EXPECT(rc == 0);
        TEST("popped value is 200") EXPECT(val == 200);
        TEST("size is now 1") EXPECT(da.size == 1);

        da_pop(&da, &val);
        rc = da_pop(&da, &val);
        TEST("pop empty → returns -1") EXPECT(rc == -1);
        da_destroy(&da);
    }

    printf("\nda_get/da_set:\n");
    {
        DynArray da = da_create();
        da_push(&da, 5);
        da_push(&da, 15);
        int val;
        da_set(&da, 1, 99);
        da_get(&da, 1, &val);
        TEST("set index 1 to 99, get → 99") EXPECT(val == 99);
        TEST("get out-of-bounds → -1") EXPECT(da_get(&da, 5, &val) == -1);
        TEST("set out-of-bounds → -1") EXPECT(da_set(&da, -1, 0) == -1);
        da_destroy(&da);
    }

    printf("\nda_destroy:\n");
    {
        DynArray da = da_create();
        da_push(&da, 1);
        da_destroy(&da);
        TEST("destroy → data is NULL") EXPECT(da.data == NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
