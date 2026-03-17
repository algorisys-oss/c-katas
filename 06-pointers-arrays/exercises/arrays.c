/*
 * Kata: Array Operations
 *
 * Implement common array operations using pointers and indices:
 *   1. array_reverse    — reverse in-place
 *   2. array_search     — linear search, return index
 *   3. array_rotate_left — rotate elements left by k positions
 *   4. array_merge      — merge two sorted arrays into one
 *   5. matrix_transpose — transpose a 2D matrix
 *   6. array_remove_dups — remove consecutive duplicates
 *
 * 18 tests total.
 */

#include <stdio.h>
#include <assert.h>

/* ── Exercise 1: array_reverse ──────────────────────────────────────
 * Reverse the array in-place.
 *
 * Example: {1, 2, 3, 4, 5} → {5, 4, 3, 2, 1}
 */
void array_reverse(int *arr, int size)
{
    /* TODO: Swap elements from both ends, moving inward */
}

/* ── Exercise 2: array_search ───────────────────────────────────────
 * Search for value in arr. Return its index, or -1 if not found.
 */
int array_search(const int *arr, int size, int value)
{
    /* TODO: Linear search through the array */
    return -1; /* placeholder */
}

/* ── Exercise 3: array_rotate_left ──────────────────────────────────
 * Rotate the array left by k positions, in-place.
 *
 * Example: {1, 2, 3, 4, 5} rotated left by 2 → {3, 4, 5, 1, 2}
 *
 * Hint: One clean approach:
 *   1. Reverse the first k elements
 *   2. Reverse the remaining elements
 *   3. Reverse the whole array
 *
 * Make sure to handle k >= size (use k % size).
 */
void array_rotate_left(int *arr, int size, int k)
{
    /* TODO: Implement the three-reverse rotation algorithm */
}

/* ── Exercise 4: array_merge ────────────────────────────────────────
 * Merge two sorted arrays into a single sorted array.
 *
 * Parameters:
 *   a, a_size — first sorted array and its size
 *   b, b_size — second sorted array and its size
 *   out       — output array (must have space for a_size + b_size elements)
 *
 * Hint: Use two index variables (one for each array).
 *       Compare elements, copy the smaller one into out.
 *       When one array is exhausted, copy the rest of the other.
 */
void array_merge(const int *a, int a_size, const int *b, int b_size, int *out)
{
    /* TODO: Merge a and b into out in sorted order */
}

/* ── Exercise 5: matrix_transpose ───────────────────────────────────
 * Transpose a rows×cols matrix into a cols×rows matrix.
 *
 * Input matrix is stored as a 1D array in row-major order:
 *   element at [r][c] is at index r * cols + c
 *
 * Output: element at [c][r] in the transposed matrix
 *   which is at index c * rows + r
 *
 * Example (2×3 → 3×2):
 *   Input:   {1, 2, 3,    Output:  {1, 4,
 *             4, 5, 6}              2, 5,
 *                                   3, 6}
 */
void matrix_transpose(const int *in, int rows, int cols, int *out)
{
    /* TODO: Copy in[r * cols + c] to out[c * rows + r] */
}

/* ── Exercise 6: array_remove_dups ──────────────────────────────────
 * Remove consecutive duplicates from a sorted array, in-place.
 * Return the new length.
 *
 * Example: {1, 1, 2, 3, 3, 3, 4} → {1, 2, 3, 4}, return 4
 *
 * Hint: Use a "write" pointer that only advances when a new value is seen.
 */
int array_remove_dups(int *arr, int size)
{
    /* TODO: Walk through array, keep only unique consecutive elements */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

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
    printf("=== Array Operations ===\n\n");

    /* --- array_reverse tests --- */
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

    /* --- array_search tests --- */
    printf("\narray_search:\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        TEST("find 30 → index 2")   EXPECT(array_search(arr, 5, 30) == 2);
        TEST("find 10 → index 0")   EXPECT(array_search(arr, 5, 10) == 0);
        TEST("find 99 → -1")        EXPECT(array_search(arr, 5, 99) == -1);
    }

    /* --- array_rotate_left tests --- */
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

    /* --- array_merge tests --- */
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

    /* --- matrix_transpose tests --- */
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

    /* --- array_remove_dups tests --- */
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
