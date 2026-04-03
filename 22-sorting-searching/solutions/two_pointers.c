/*
 * Solution: Two-Pointer Technique
 *
 * Five exercises demonstrating converging and fast/slow pointer patterns.
 *
 * 16 tests total.
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

/* ── Reverse Array ─────────────────────────────────────────────── */
void reverse_array(int *arr, int n)
{
    int left = 0, right = n - 1;
    while (left < right) {
        swap(&arr[left], &arr[right]);
        left++;
        right--;
    }
}

/* ── Remove Duplicates from Sorted Array ───────────────────────── */
int remove_duplicates_sorted(int *arr, int n)
{
    if (n <= 1) {
        return n;
    }
    /* slow points to the last unique element written */
    int slow = 0;
    for (int fast = 1; fast < n; fast++) {
        if (arr[fast] != arr[slow]) {
            slow++;
            arr[slow] = arr[fast];
        }
    }
    return slow + 1;  /* Length = last index + 1 */
}

/* ── Two Sum (Sorted Array) ────────────────────────────────────── */
int two_sum_sorted(const int *arr, int n, int target, int *idx1, int *idx2)
{
    int left = 0, right = n - 1;
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            *idx1 = left;
            *idx2 = right;
            return 1;
        } else if (sum < target) {
            left++;   /* Need a bigger sum */
        } else {
            right--;  /* Need a smaller sum */
        }
    }
    return 0;
}

/* ── Container With Most Water ─────────────────────────────────── */
int container_with_most_water(const int *heights, int n)
{
    int left = 0, right = n - 1;
    int max_area = 0;
    while (left < right) {
        /* Water is limited by the shorter of the two lines */
        int h = heights[left] < heights[right] ? heights[left] : heights[right];
        int area = h * (right - left);
        if (area > max_area) {
            max_area = area;
        }
        /* Move the shorter side inward — keeping it can only hurt */
        if (heights[left] < heights[right]) {
            left++;
        } else {
            right--;
        }
    }
    return max_area;
}

/* ── Is Palindrome ─────────────────────────────────────────────── */
int is_palindrome(const char *s)
{
    int left = 0, right = (int)strlen(s) - 1;
    while (left < right) {
        if (s[left] != s[right]) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
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
