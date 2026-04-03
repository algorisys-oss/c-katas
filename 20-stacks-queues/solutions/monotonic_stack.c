/*
 * Solution: Monotonic Stack
 */

#include <stdio.h>
#include <stdlib.h>

void next_greater_element(const int *arr, int n, int *result)
{
    int *stack = malloc(sizeof(int) * n);
    if (!stack) return;
    int top = 0;

    /* Initialize all results to -1 (default: no greater element) */
    for (int i = 0; i < n; i++) {
        result[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        /* Pop indices whose values are smaller than current element.
         * The current element is the "next greater" for each of them. */
        while (top > 0 && arr[i] > arr[stack[top - 1]]) {
            int j = stack[--top];
            result[j] = arr[i];
        }
        stack[top++] = i;
    }

    free(stack);
}

void daily_temperatures(const int *temps, int n, int *result)
{
    int *stack = malloc(sizeof(int) * n);
    if (!stack) return;
    int top = 0;

    /* Initialize all results to 0 (default: no warmer day) */
    for (int i = 0; i < n; i++) {
        result[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        /* Same pattern as next_greater, but store distance instead of value */
        while (top > 0 && temps[i] > temps[stack[top - 1]]) {
            int j = stack[--top];
            result[j] = i - j;
        }
        stack[top++] = i;
    }

    free(stack);
}

int largest_rectangle_histogram(const int *heights, int n)
{
    int *stack = malloc(sizeof(int) * (n + 1));
    if (!stack) return 0;
    int top = 0;
    int max_area = 0;

    for (int i = 0; i <= n; i++) {
        /* Use height 0 as a sentinel to flush the stack at the end */
        int cur_height = (i == n) ? 0 : heights[i];

        while (top > 0 && cur_height < heights[stack[top - 1]]) {
            int h = heights[stack[--top]];
            /* Width extends from (stack_top + 1) to (i - 1) */
            int w = (top == 0) ? i : (i - stack[top - 1] - 1);
            int area = h * w;
            if (area > max_area) {
                max_area = area;
            }
        }
        stack[top++] = i;
    }

    free(stack);
    return max_area;
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

static int arrays_equal(const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void)
{
    printf("=== Monotonic Stack (Solution) ===\n\n");

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
