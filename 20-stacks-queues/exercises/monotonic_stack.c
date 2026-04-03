/*
 * Kata: Monotonic Stack
 *
 * A monotonic stack maintains elements in sorted order (increasing or
 * decreasing). When you push a new element, you pop everything that
 * violates the order. This seemingly simple trick solves a whole class
 * of problems in O(n) time.
 *
 * Why O(n)? Even though there's a while loop inside a for loop, each
 * element is pushed onto the stack AT MOST ONCE and popped AT MOST ONCE.
 * That's 2n operations total — still O(n).
 *
 * ── How a decreasing monotonic stack works ──────────────────────────
 *
 *   Processing array [4, 5, 2, 10, 8] for "next greater element":
 *
 *   Step 1: Process 4
 *     Stack: [4]                  (push 4 — stack was empty)
 *
 *   Step 2: Process 5
 *     5 > 4, so pop 4 → next_greater[0] = 5
 *     Stack: [5]                  (push 5)
 *
 *   Step 3: Process 2
 *     2 < 5, so just push         (doesn't violate decreasing order)
 *     Stack: [5, 2]
 *
 *   Step 4: Process 10
 *     10 > 2, so pop 2 → next_greater[2] = 10
 *     10 > 5, so pop 5 → next_greater[1] = 10
 *     Stack: [10]                 (push 10)
 *
 *   Step 5: Process 8
 *     8 < 10, so just push
 *     Stack: [10, 8]
 *
 *   End: remaining elements have no next greater → -1
 *     next_greater[3] = -1, next_greater[4] = -1
 *
 *   Result: [5, 10, 10, -1, -1]
 *
 * ── Histogram visualization for largest_rectangle ───────────────────
 *
 *   Heights: [2, 1, 5, 6, 2, 3]
 *
 *         ┌───┐
 *     ┌───┤   │
 *     │   │   │       ┌───┐
 *     │   │   │  ┌───┐│   │
 *   ┌─┤   │   │  │   ││   │
 *   │ │   │   │  │   ││   │
 *   └─┴───┴───┴──┴───┴┴───┘
 *    2   1   5   6   2   3
 *
 *   Largest rectangle = 10 (height=5, width=2, covering bars 5 and 6)
 *             ┌───┐
 *         ┌───┤███│
 *         │███│███│
 *         │███│███│
 *         │   │   │
 *         │   │   │
 *
 * 13 tests total.
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Exercise 1: next_greater_element ─────────────────────────────────
 *
 * For each element in the array, find the next element to its RIGHT
 * that is strictly greater. If no such element exists, store -1.
 *
 * Algorithm:
 *   - Use a stack that stores INDICES (not values)
 *   - Walk left to right through the array
 *   - For each element arr[i]:
 *     - While the stack is not empty AND arr[i] > arr[stack_top]:
 *         Pop the index j — arr[i] is the next greater element for arr[j]
 *         Set result[j] = arr[i]
 *     - Push i onto the stack
 *   - After processing all elements, anything left on the stack gets -1
 *
 * Parameters:
 *   arr    — input array of integers
 *   n      — length of arr
 *   result — output array (same length as arr), caller-allocated
 */
void next_greater_element(const int *arr, int n, int *result)
{
    /* TODO: allocate a stack (array of int, size n), implement algorithm */
    (void)arr;
    (void)n;
    (void)result;
}

/* ── Exercise 2: daily_temperatures ───────────────────────────────────
 *
 * Given an array of daily temperatures, for each day find how many days
 * you have to wait until a warmer temperature. If no warmer day exists,
 * store 0.
 *
 * This is the same pattern as next_greater_element, but instead of
 * storing the value, you store the DISTANCE (i - j).
 *
 * Parameters:
 *   temps  — input array of temperatures
 *   n      — length of temps
 *   result — output array of wait-days, caller-allocated
 */
void daily_temperatures(const int *temps, int n, int *result)
{
    /* TODO: same monotonic stack pattern, but store (i - j) instead of value */
    (void)temps;
    (void)n;
    (void)result;
}

/* ── Exercise 3: largest_rectangle_histogram ──────────────────────────
 *
 * Given an array of bar heights representing a histogram, find the
 * area of the largest rectangle that can be formed within the histogram.
 *
 * Algorithm:
 *   - Use a stack of indices, maintaining INCREASING heights
 *   - For each bar i:
 *     - While stack is not empty AND heights[i] < heights[stack_top]:
 *         Pop index j — the bar at j can extend left to (new stack_top + 1)
 *         and right to (i - 1)
 *         Width = i - stack_top - 1 (or i if stack is empty)
 *         Area = heights[j] * width
 *         Track the maximum area
 *     - Push i
 *   - After all bars, pop remaining indices (they extend to the right end)
 *
 * Parameters:
 *   heights — array of bar heights
 *   n       — number of bars
 *
 * Returns: area of the largest rectangle
 */
int largest_rectangle_histogram(const int *heights, int n)
{
    /* TODO: implement using a monotonic increasing stack */
    (void)heights;
    (void)n;
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

/* Helper: compare two int arrays for equality */
static int arrays_equal(const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void)
{
    printf("=== Monotonic Stack ===\n\n");

    /* --- next_greater_element --- */
    printf("next_greater_element:\n");
    {
        int arr1[] = {4, 5, 2, 10, 8};
        int expected1[] = {5, 10, 10, -1, -1};
        int result1[5];
        next_greater_element(arr1, 5, result1);
        TEST("[4,5,2,10,8] -> [5,10,10,-1,-1]")
            EXPECT(arrays_equal(result1, expected1, 5));

        int arr2[] = {1, 2, 3, 4, 5};
        int expected2[] = {2, 3, 4, 5, -1};
        int result2[5];
        next_greater_element(arr2, 5, result2);
        TEST("[1,2,3,4,5] -> [2,3,4,5,-1]")
            EXPECT(arrays_equal(result2, expected2, 5));

        int arr3[] = {5, 4, 3, 2, 1};
        int expected3[] = {-1, -1, -1, -1, -1};
        int result3[5];
        next_greater_element(arr3, 5, result3);
        TEST("[5,4,3,2,1] -> [-1,-1,-1,-1,-1]")
            EXPECT(arrays_equal(result3, expected3, 5));

        int arr4[] = {2, 7, 4, 3, 5};
        int expected4[] = {7, -1, 5, 5, -1};
        int result4[5];
        next_greater_element(arr4, 5, result4);
        TEST("[2,7,4,3,5] -> [7,-1,5,5,-1]")
            EXPECT(arrays_equal(result4, expected4, 5));

        int arr5[] = {42};
        int expected5[] = {-1};
        int result5[1];
        next_greater_element(arr5, 1, result5);
        TEST("single element [42] -> [-1]")
            EXPECT(arrays_equal(result5, expected5, 1));
    }

    /* --- daily_temperatures --- */
    printf("\ndaily_temperatures:\n");
    {
        int temps1[] = {73, 74, 75, 71, 69, 72, 76, 73};
        int expected1[] = {1, 1, 4, 2, 1, 1, 0, 0};
        int result1[8];
        daily_temperatures(temps1, 8, result1);
        TEST("[73,74,75,71,69,72,76,73] -> [1,1,4,2,1,1,0,0]")
            EXPECT(arrays_equal(result1, expected1, 8));

        int temps2[] = {30, 40, 50, 60};
        int expected2[] = {1, 1, 1, 0};
        int result2[4];
        daily_temperatures(temps2, 4, result2);
        TEST("[30,40,50,60] -> [1,1,1,0]")
            EXPECT(arrays_equal(result2, expected2, 4));

        int temps3[] = {60, 50, 40, 30};
        int expected3[] = {0, 0, 0, 0};
        int result3[4];
        daily_temperatures(temps3, 4, result3);
        TEST("[60,50,40,30] -> [0,0,0,0]")
            EXPECT(arrays_equal(result3, expected3, 4));

        int temps4[] = {50};
        int expected4[] = {0};
        int result4[1];
        daily_temperatures(temps4, 1, result4);
        TEST("single day [50] -> [0]")
            EXPECT(arrays_equal(result4, expected4, 1));
    }

    /* --- largest_rectangle_histogram --- */
    printf("\nlargest_rectangle_histogram:\n");
    {
        int h1[] = {2, 1, 5, 6, 2, 3};
        TEST("[2,1,5,6,2,3] -> 10")
            EXPECT(largest_rectangle_histogram(h1, 6) == 10);

        int h2[] = {2, 4};
        TEST("[2,4] -> 4")
            EXPECT(largest_rectangle_histogram(h2, 2) == 4);

        int h3[] = {3, 3, 3, 3};
        TEST("[3,3,3,3] -> 12")
            EXPECT(largest_rectangle_histogram(h3, 4) == 12);

        int h4[] = {6, 2, 5, 4, 5, 1, 6};
        TEST("[6,2,5,4,5,1,6] -> 12")
            EXPECT(largest_rectangle_histogram(h4, 7) == 12);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
