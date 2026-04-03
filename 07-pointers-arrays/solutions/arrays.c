/*
 * Solution: Array Operations
 */

#include <stdio.h>
#include <assert.h>

static void reverse_range(int *arr, int left, int right)
{
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

void array_reverse(int *arr, int size)
{
    reverse_range(arr, 0, size - 1);
}

int array_search(const int *arr, int size, int value)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) return i;
    }
    return -1;
}

void array_rotate_left(int *arr, int size, int k)
{
    if (size <= 1) return;
    k = k % size;
    if (k == 0) return;

    /* Three-reverse algorithm:
     *   reverse [0, k-1]
     *   reverse [k, size-1]
     *   reverse [0, size-1]
     */
    reverse_range(arr, 0, k - 1);
    reverse_range(arr, k, size - 1);
    reverse_range(arr, 0, size - 1);
}

void array_merge(const int *a, int a_size, const int *b, int b_size, int *out)
{
    int i = 0, j = 0, k = 0;
    while (i < a_size && j < b_size) {
        if (a[i] <= b[j]) {
            out[k++] = a[i++];
        } else {
            out[k++] = b[j++];
        }
    }
    while (i < a_size) out[k++] = a[i++];
    while (j < b_size) out[k++] = b[j++];
}

void matrix_transpose(const int *in, int rows, int cols, int *out)
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            out[c * rows + r] = in[r * cols + c];
        }
    }
}

int array_remove_dups(int *arr, int size)
{
    if (size <= 1) return size;
    int write = 1;
    for (int i = 1; i < size; i++) {
        if (arr[i] != arr[write - 1]) {
            arr[write++] = arr[i];
        }
    }
    return write;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

static int arrays_equal(const int *a, const int *b, int size)
{
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void)
{
    printf("=== Array Operations (Solution) ===\n\n");

    printf("array_reverse:\n");
    {
        int a[] = {1, 2, 3, 4, 5};
        int expected[] = {5, 4, 3, 2, 1};
        array_reverse(a, 5);
        TEST("reverse {1,2,3,4,5}")
            EXPECT(arrays_equal(a, expected, 5));
    }
    {
        int a[] = {1, 2};
        array_reverse(a, 2);
        TEST("reverse {1,2}")
            EXPECT(a[0] == 2 && a[1] == 1);
    }
    {
        int a[] = {42};
        array_reverse(a, 1);
        TEST("reverse single element")
            EXPECT(a[0] == 42);
    }

    printf("\narray_search:\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        TEST("find 30 → index 2")   EXPECT(array_search(arr, 5, 30) == 2);
        TEST("find 10 → index 0")   EXPECT(array_search(arr, 5, 10) == 0);
        TEST("find 99 → -1")        EXPECT(array_search(arr, 5, 99) == -1);
    }

    printf("\narray_rotate_left:\n");
    {
        int a[] = {1, 2, 3, 4, 5};
        int expected[] = {3, 4, 5, 1, 2};
        array_rotate_left(a, 5, 2);
        TEST("rotate {1,2,3,4,5} left by 2")
            EXPECT(arrays_equal(a, expected, 5));
    }
    {
        int a[] = {1, 2, 3, 4, 5};
        int expected[] = {1, 2, 3, 4, 5};
        array_rotate_left(a, 5, 5);
        TEST("rotate left by size → unchanged")
            EXPECT(arrays_equal(a, expected, 5));
    }
    {
        int a[] = {1, 2, 3, 4, 5};
        int expected[] = {3, 4, 5, 1, 2};
        array_rotate_left(a, 5, 7);
        TEST("rotate left by 7 (k > size)")
            EXPECT(arrays_equal(a, expected, 5));
    }

    printf("\narray_merge:\n");
    {
        int a[] = {1, 3, 5};
        int b[] = {2, 4, 6};
        int out[6];
        int expected[] = {1, 2, 3, 4, 5, 6};
        array_merge(a, 3, b, 3, out);
        TEST("merge {1,3,5} + {2,4,6}")
            EXPECT(arrays_equal(out, expected, 6));
    }
    {
        int a[] = {1, 2, 3};
        int b[] = {4, 5};
        int out[5];
        int expected[] = {1, 2, 3, 4, 5};
        array_merge(a, 3, b, 2, out);
        TEST("merge non-overlapping {1,2,3} + {4,5}")
            EXPECT(arrays_equal(out, expected, 5));
    }

    printf("\nmatrix_transpose:\n");
    {
        int in[] = {1, 2, 3, 4, 5, 6};
        int out[6];
        int expected[] = {1, 4, 2, 5, 3, 6};
        matrix_transpose(in, 2, 3, out);
        TEST("transpose 2x3 matrix")
            EXPECT(arrays_equal(out, expected, 6));
    }
    {
        int in[] = {1, 2, 3, 4};
        int out[4];
        int expected[] = {1, 3, 2, 4};
        matrix_transpose(in, 2, 2, out);
        TEST("transpose 2x2 matrix")
            EXPECT(arrays_equal(out, expected, 4));
    }

    printf("\narray_remove_dups:\n");
    {
        int arr[] = {1, 1, 2, 3, 3, 3, 4};
        int len = array_remove_dups(arr, 7);
        TEST("remove dups {1,1,2,3,3,3,4} → len 4")
            EXPECT(len == 4 && arr[0]==1 && arr[1]==2 && arr[2]==3 && arr[3]==4);
    }
    {
        int arr[] = {5, 5, 5, 5};
        int len = array_remove_dups(arr, 4);
        TEST("all duplicates → len 1")
            EXPECT(len == 1 && arr[0] == 5);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
