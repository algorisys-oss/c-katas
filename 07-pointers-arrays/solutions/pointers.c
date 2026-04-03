/*
 * Solution: Pointer Basics
 */

#include <stdio.h>
#include <assert.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void min_max(const int *arr, int size, int *min, int *max)
{
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

void sum_and_count_positive(const int *arr, int size, int *out_sum, int *out_count)
{
    *out_sum = 0;
    *out_count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] > 0) {
            *out_sum += arr[i];
            (*out_count)++;
        }
    }
}

int pointer_sum(const int *arr, int size)
{
    int sum = 0;
    const int *end = arr + size;
    while (arr < end) {
        sum += *arr;
        arr++;
    }
    return sum;
}

void reverse_array(int *arr, int size)
{
    int *left = arr;
    int *right = arr + size - 1;
    while (left < right) {
        int temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

int *find_element(int *arr, int size, int value)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) return &arr[i];
    }
    return NULL;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

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
    printf("=== Pointer Basics (Solution) ===\n\n");

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

    printf("\npointer_sum:\n");
    {
        int arr[] = {1, 2, 3, 4, 5};
        TEST("sum {1,2,3,4,5} → 15") EXPECT(pointer_sum(arr, 5) == 15);
    }
    {
        int arr[] = {-10, 20, -30, 40};
        TEST("sum {-10,20,-30,40} → 20") EXPECT(pointer_sum(arr, 4) == 20);
    }

    printf("\nreverse_array:\n");
    {
        int arr[] = {1, 2, 3, 4, 5};
        reverse_array(arr, 5);
        TEST("reverse {1,2,3,4,5} → {5,4,3,2,1}")
            EXPECT(arr[0]==5 && arr[1]==4 && arr[2]==3 && arr[3]==2 && arr[4]==1);
    }

    printf("\nfind_element:\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        TEST("find 30 in array → non-NULL") EXPECT(find_element(arr, 5, 30) == &arr[2]);
        TEST("find 99 in array → NULL") EXPECT(find_element(arr, 5, 99) == NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
