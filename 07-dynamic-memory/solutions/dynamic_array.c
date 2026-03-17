/*
 * Solution: Dynamic Array
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DA_INITIAL_CAP 4

typedef struct {
    int *data;
    int size;
    int capacity;
} DynArray;

DynArray da_create(void)
{
    DynArray da;
    da.data = malloc(DA_INITIAL_CAP * sizeof(int));
    if (da.data == NULL) {
        da.size = 0;
        da.capacity = 0;
        return da;
    }
    da.size = 0;
    da.capacity = DA_INITIAL_CAP;
    return da;
}

int da_push(DynArray *da, int value)
{
    if (da->size == da->capacity) {
        int new_cap = da->capacity * 2;
        int *temp = realloc(da->data, new_cap * sizeof(int));
        if (temp == NULL) {
            return -1;
        }
        da->data = temp;
        da->capacity = new_cap;
    }
    da->data[da->size] = value;
    da->size++;
    return 0;
}

int da_pop(DynArray *da, int *out_value)
{
    if (da->size == 0) {
        return -1;
    }
    da->size--;
    *out_value = da->data[da->size];
    return 0;
}

int da_get(const DynArray *da, int index, int *out_value)
{
    if (index < 0 || index >= da->size) {
        return -1;
    }
    *out_value = da->data[index];
    return 0;
}

int da_set(DynArray *da, int index, int value)
{
    if (index < 0 || index >= da->size) {
        return -1;
    }
    da->data[index] = value;
    return 0;
}

void da_destroy(DynArray *da)
{
    free(da->data);
    da->data = NULL;
    da->size = 0;
    da->capacity = 0;
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
    printf("=== Dynamic Array (Solution) ===\n\n");

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
