/*
 * Kata: Two-Pointer Technique
 *
 * The two-pointer technique uses two indices that move through an array
 * (or string) in a coordinated way. Instead of nested loops (O(n^2)),
 * two pointers often solve the problem in a single pass (O(n)).
 *
 * Three flavors:
 *   1. Opposite ends (converging): left starts at 0, right starts at end
 *   2. Same direction (fast/slow): both start at 0, fast runs ahead
 *   3. Separate arrays: one pointer per array
 *
 * Exercises:
 *   - reverse_array             (converging pointers)
 *   - remove_duplicates_sorted  (fast/slow pointers)
 *   - two_sum_sorted            (converging pointers)
 *   - container_with_most_water (converging pointers)
 *   - is_palindrome             (converging pointers)
 *
 * 16 tests total.
 *
 * ASCII art — the converging two-pointer pattern:
 *
 *   Array: [ 1 | 8 | 6 | 2 | 5 | 4 | 8 | 3 | 7 ]
 *            ^                                 ^
 *           left ──────────────────────────> right
 *            └── move inward each step ──────┘
 *
 *   - If we need a bigger result  → move left pointer right
 *   - If we need a smaller result → move right pointer left
 *   - Stop when left >= right
 *
 * ASCII art — the fast/slow pointer pattern:
 *
 *   Array: [ 1 | 1 | 2 | 2 | 3 ]
 *            ^   ^
 *          slow fast
 *
 *   slow = write position (where to put the next unique element)
 *   fast = read position (scanning ahead looking for new values)
 *
 *   Step 1: fast sees 1 (same as slow) → skip
 *           [ 1 | 1 | 2 | 2 | 3 ]
 *            ^       ^
 *          slow     fast
 *
 *   Step 2: fast sees 2 (different!) → write it at slow+1, advance slow
 *           [ 1 | 2 | 2 | 2 | 3 ]
 *                ^       ^
 *              slow     fast
 *
 *   Step 3: fast sees 2 (same) → skip
 *   Step 4: fast sees 3 (different!) → write it at slow+1, advance slow
 *           [ 1 | 2 | 3 | 2 | 3 ]
 *                      ^       ^
 *                    slow     fast
 *
 *   Result: first 3 elements [1, 2, 3] are the unique values. Return 3.
 *
 *
 * Hints:
 *   - Always think: what condition makes us move each pointer?
 *   - For sorted-array problems, sorted order gives us powerful guarantees
 *   - For container_with_most_water, the greedy insight: always move the
 *     shorter side inward (keeping the taller side can only help)
 */

#include <stdio.h>
#include <string.h>

/* ── Helper: swap two ints ──────────────────────────────────────── */
static void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ── Exercise 1: Reverse Array ─────────────────────────────────── *
 * Reverse arr[] in-place using two pointers: left starts at 0,
 * right starts at n-1. Swap them and move inward.
 *
 * Example: [1, 2, 3, 4, 5] → [5, 4, 3, 2, 1]
 */
void reverse_array(int *arr, int n)
{
    /* TODO: Use left/right pointers converging toward the center */
    (void)arr; (void)n; (void)swap;
}

/* ── Exercise 2: Remove Duplicates from Sorted Array ───────────── *
 * Given a sorted array, remove duplicates IN-PLACE and return the
 * new length. The first `result` elements of arr[] should contain
 * the unique values in order.
 *
 * Use slow/fast pointers:
 *   - slow = position of last unique element written
 *   - fast = current element being examined
 *
 * Example: [1, 1, 2, 2, 3] → [1, 2, 3, ...] return 3
 */
int remove_duplicates_sorted(int *arr, int n)
{
    /* TODO: Implement using slow/fast pointers */
    (void)arr; (void)n;
    return 0;
}

/* ── Exercise 3: Two Sum (Sorted Array) ────────────────────────── *
 * Given a SORTED array and a target sum, find two elements that
 * add up to target. Return their indices in idx1 and idx2.
 * Return 1 if found, 0 if not found.
 *
 * Use converging pointers:
 *   - left = 0, right = n-1
 *   - sum = arr[left] + arr[right]
 *   - sum too small? move left right (increase the smaller value)
 *   - sum too big?   move right left (decrease the larger value)
 *
 * Trace for arr = [1, 3, 5, 7, 11], target = 10:
 *
 *   Step 1: left=0, right=4 → 1 + 11 = 12 > 10 → move right left
 *   Step 2: left=0, right=3 → 1 + 7  = 8  < 10 → move left right
 *   Step 3: left=1, right=3 → 3 + 7  = 10 = 10 → found! (1, 3)
 */
int two_sum_sorted(const int *arr, int n, int target, int *idx1, int *idx2)
{
    /* TODO: Implement using converging pointers */
    (void)arr; (void)n; (void)target; (void)idx1; (void)idx2;
    return 0;
}

/* ── Exercise 4: Container With Most Water ─────────────────────── *
 * Given an array of heights, find two lines that together with the
 * x-axis form a container that holds the most water.
 *
 * Water = min(heights[left], heights[right]) * (right - left)
 *
 * Visualization:
 *
 *   heights: [1, 8, 6, 2, 5, 4, 8, 3, 7]
 *
 *        8 |    #              #
 *        7 |    #  .  .  .  .  #  .  #
 *        6 |    #  #  .  .  .  #  .  #
 *        5 |    #  #  .  #  .  #  .  #
 *        4 |    #  #  .  #  #  #  .  #
 *        3 |    #  #  .  #  #  #  #  #
 *        2 |    #  #  #  #  #  #  #  #
 *        1 | #  #  #  #  #  #  #  #  #
 *          +----------------------------
 *            0  1  2  3  4  5  6  7  8
 *               ^                    ^
 *              left               right
 *
 *   Best container: between index 1 (height 8) and index 8 (height 7)
 *   Water = min(8, 7) * (8 - 1) = 7 * 7 = 49
 *
 * Greedy insight: always move the pointer with the SHORTER height.
 * Why? Moving the taller one can only make things worse (the width
 * shrinks and the min height cannot increase).
 */
int container_with_most_water(const int *heights, int n)
{
    /* TODO: Use converging pointers, track max area */
    (void)heights; (void)n;
    return 0;
}

/* ── Exercise 5: Is Palindrome ─────────────────────────────────── *
 * Check if a string is a palindrome using two pointers.
 * Compare characters from both ends moving inward.
 * Return 1 if palindrome, 0 if not.
 *
 * Example: "racecar" → left='r', right='r' ✓
 *                       left='a', right='a' ✓
 *                       left='c', right='c' ✓
 *                       left >= right → done, it's a palindrome!
 */
int is_palindrome(const char *s)
{
    /* TODO: Use left/right pointers converging toward center */
    (void)s;
    return 0;
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

/* Helper: check if first n elements of arr match expected */
static int arr_eq(const int *arr, const int *expected, int n)
{
    for (int i = 0; i < n; i++) {
        if (arr[i] != expected[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    printf("=== Two-Pointer Technique ===\n\n");

    /* --- reverse_array --- */
    printf("reverse_array:\n");
    {
        int arr[] = {1, 2, 3, 4, 5};
        int exp[] = {5, 4, 3, 2, 1};
        reverse_array(arr, 5);
        TEST("reverse [1,2,3,4,5]") EXPECT(arr_eq(arr, exp, 5));
    }
    {
        int arr[] = {1, 2, 3, 4};
        int exp[] = {4, 3, 2, 1};
        reverse_array(arr, 4);
        TEST("reverse even-length array") EXPECT(arr_eq(arr, exp, 4));
    }
    {
        int arr[] = {42};
        reverse_array(arr, 1);
        TEST("single element unchanged") EXPECT(arr[0] == 42);
    }

    /* --- remove_duplicates_sorted --- */
    printf("\nremove_duplicates_sorted:\n");
    {
        int arr[] = {1, 1, 2, 2, 3};
        int len = remove_duplicates_sorted(arr, 5);
        int exp[] = {1, 2, 3};
        TEST("[1,1,2,2,3] → 3 unique") EXPECT(len == 3 && arr_eq(arr, exp, 3));
    }
    {
        int arr[] = {1, 2, 3, 4, 5};
        int len = remove_duplicates_sorted(arr, 5);
        TEST("all unique → length unchanged") EXPECT(len == 5);
    }
    {
        int arr[] = {7, 7, 7, 7};
        int len = remove_duplicates_sorted(arr, 4);
        TEST("all same → length 1") EXPECT(len == 1 && arr[0] == 7);
    }
    {
        int arr[] = {1};
        int len = remove_duplicates_sorted(arr, 1);
        TEST("single element → length 1") EXPECT(len == 1 && arr[0] == 1);
    }

    /* --- two_sum_sorted --- */
    printf("\ntwo_sum_sorted:\n");
    {
        int arr[] = {1, 3, 5, 7, 11};
        int i1, i2;
        int found = two_sum_sorted(arr, 5, 10, &i1, &i2);
        TEST("target 10 in [1,3,5,7,11]") EXPECT(found && i1 == 1 && i2 == 3);
    }
    {
        int arr[] = {2, 7, 11, 15};
        int i1, i2;
        int found = two_sum_sorted(arr, 4, 9, &i1, &i2);
        TEST("target 9 in [2,7,11,15]") EXPECT(found && i1 == 0 && i2 == 1);
    }
    {
        int arr[] = {1, 2, 3, 4};
        int i1, i2;
        int found = two_sum_sorted(arr, 4, 100, &i1, &i2);
        TEST("target not achievable") EXPECT(!found);
    }

    /* --- container_with_most_water --- */
    printf("\ncontainer_with_most_water:\n");
    {
        int h[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
        TEST("classic example → 49") EXPECT(container_with_most_water(h, 9) == 49);
    }
    {
        int h[] = {1, 1};
        TEST("two bars of height 1 → 1") EXPECT(container_with_most_water(h, 2) == 1);
    }
    {
        int h[] = {4, 3, 2, 1, 4};
        TEST("[4,3,2,1,4] → 16") EXPECT(container_with_most_water(h, 5) == 16);
    }

    /* --- is_palindrome --- */
    printf("\nis_palindrome:\n");
    {
        TEST("\"racecar\" is palindrome") EXPECT(is_palindrome("racecar"));
    }
    {
        TEST("\"hello\" is not palindrome") EXPECT(!is_palindrome("hello"));
    }
    {
        TEST("\"a\" is palindrome") EXPECT(is_palindrome("a"));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
