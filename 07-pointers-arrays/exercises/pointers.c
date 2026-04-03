/*
 * Kata: Pointer Basics
 *
 * Learn how pointers work by implementing fundamental operations:
 *   - Reading/writing through pointers
 *   - Swapping values via pointers
 *   - Pointer arithmetic
 *   - Passing pointers to functions
 *
 * 15 tests total.
 *
 * Hints:
 *   - & gives you the address of a variable
 *   - * follows the pointer to the value
 *   - Adding 1 to an int* moves it forward by sizeof(int) bytes
 */

#include <stdio.h>
#include <assert.h>

/* ── Exercise 1: swap ───────────────────────────────────────────────
 * Swap the values of two integers through pointers.
 *
 * After calling swap(&a, &b):
 *   a should have b's old value, b should have a's old value.
 */
void swap(int *a, int *b)
{
    /* TODO: Use a temporary variable and dereference pointers to swap */
}

/* ── Exercise 2: min_max ────────────────────────────────────────────
 * Find the minimum and maximum of an array and store them via pointers.
 *
 * Parameters:
 *   arr  — array of ints
 *   size — number of elements
 *   min  — pointer to store the minimum value
 *   max  — pointer to store the maximum value
 */
void min_max(const int *arr, int size, int *min, int *max)
{
    /* TODO: Initialize *min and *max to arr[0], then scan the array */
}

/* ── Exercise 3: sum_and_count ──────────────────────────────────────
 * Calculate the sum and count of positive numbers in an array.
 * Store results through pointers.
 *
 * Parameters:
 *   arr       — array of ints (may include negatives and zero)
 *   size      — number of elements
 *   out_sum   — pointer to store the sum of positive numbers
 *   out_count — pointer to store the count of positive numbers
 */
void sum_and_count_positive(const int *arr, int size, int *out_sum, int *out_count)
{
    /* TODO: Loop through array, accumulate sum and count for arr[i] > 0 */
}

/* ── Exercise 4: pointer_arithmetic_sum ─────────────────────────────
 * Sum the elements of an array using ONLY pointer arithmetic.
 * Do NOT use arr[i] subscript notation — use *(arr + i) or pointer increment.
 */
int pointer_sum(const int *arr, int size)
{
    /* TODO: Use pointer arithmetic to walk through the array and sum values */
    return 0; /* placeholder */
}

/* ── Exercise 5: reverse_array ──────────────────────────────────────
 * Reverse an array in-place using two pointers (front and back).
 */
void reverse_array(int *arr, int size)
{
    /* TODO: Use two pointers starting at front and back, swap and move inward */
}

/* ── Exercise 6: find_element ───────────────────────────────────────
 * Search for a value in an array.
 * Return a pointer to the first occurrence, or NULL if not found.
 */
int *find_element(int *arr, int size, int value)
{
    /* TODO: Loop through array, return &arr[i] when found, NULL otherwise */
    return NULL; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-40s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Pointer Basics ===\n\n");

    /* --- swap tests --- */
    printf("swap:\n");
    {
        int a = 10, b = 20;
        swap(&a, &b);
        TEST("swap(10, 20) → a=20, b=10") EXPECT(a == 20 && b == 10);
    }
    {
        int a = -5, b = 5;
        swap(&a, &b);
        TEST("swap(-5, 5) → a=5, b=-5") EXPECT(a == 5 && b == -5);
    }
    {
        int a = 42, b = 42;
        swap(&a, &b);
        TEST("swap same values") EXPECT(a == 42 && b == 42);
    }

    /* --- min_max tests --- */
    printf("\nmin_max:\n");
    {
        int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
        int mn, mx;
        min_max(arr, 8, &mn, &mx);
        TEST("min of {3,1,4,1,5,9,2,6} → 1") EXPECT(mn == 1);
        TEST("max of {3,1,4,1,5,9,2,6} → 9") EXPECT(mx == 9);
    }
    {
        int arr[] = {42};
        int mn, mx;
        min_max(arr, 1, &mn, &mx);
        TEST("single element: min=max=42") EXPECT(mn == 42 && mx == 42);
    }

    /* --- sum_and_count_positive tests --- */
    printf("\nsum_and_count_positive:\n");
    {
        int arr[] = {3, -2, 7, 0, -5, 10};
        int sum, count;
        sum_and_count_positive(arr, 6, &sum, &count);
        TEST("sum of positives {3,-2,7,0,-5,10} → 20") EXPECT(sum == 20);
        TEST("count of positives → 3") EXPECT(count == 3);
    }
    {
        int arr[] = {-1, -2, -3};
        int sum, count;
        sum_and_count_positive(arr, 3, &sum, &count);
        TEST("all negatives: sum=0, count=0") EXPECT(sum == 0 && count == 0);
    }

    /* --- pointer_sum tests --- */
    printf("\npointer_sum:\n");
    {
        int arr[] = {1, 2, 3, 4, 5};
        TEST("sum {1,2,3,4,5} → 15") EXPECT(pointer_sum(arr, 5) == 15);
    }
    {
        int arr[] = {-10, 20, -30, 40};
        TEST("sum {-10,20,-30,40} → 20") EXPECT(pointer_sum(arr, 4) == 20);
    }

    /* --- reverse_array tests --- */
    printf("\nreverse_array:\n");
    {
        int arr[] = {1, 2, 3, 4, 5};
        reverse_array(arr, 5);
        TEST("reverse {1,2,3,4,5} → {5,4,3,2,1}")
            EXPECT(arr[0]==5 && arr[1]==4 && arr[2]==3 && arr[3]==2 && arr[4]==1);
    }

    /* --- find_element tests --- */
    printf("\nfind_element:\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        TEST("find 30 in array → non-NULL") EXPECT(find_element(arr, 5, 30) == &arr[2]);
        TEST("find 99 in array → NULL") EXPECT(find_element(arr, 5, 99) == NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
