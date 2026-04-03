/*
 * Solution: Prefix Sums & Range Queries
 */

#include <stdio.h>
#include <stdlib.h>

/* ── build_prefix_sum ────────────────────────────────────────────────── */

void build_prefix_sum(const int *a, int n, int *prefix)
{
    prefix[0] = 0;
    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + a[i - 1];
    }
}

/* ── range_sum ───────────────────────────────────────────────────────── */

int range_sum(const int *prefix, int l, int r)
{
    return prefix[r + 1] - prefix[l];
}

/* ── count_range_evens ───────────────────────────────────────────────── */

int count_range_evens(const int *a, int n, int l, int r)
{
    /* Build prefix count of evens on the stack */
    int even_prefix[1024 + 1];
    even_prefix[0] = 0;
    for (int i = 1; i <= n; i++) {
        even_prefix[i] = even_prefix[i - 1] + (a[i - 1] % 2 == 0 ? 1 : 0);
    }
    return even_prefix[r + 1] - even_prefix[l];
}

/* ── equilibrium_index ───────────────────────────────────────────────── */

int equilibrium_index(const int *a, int n)
{
    int prefix[1024 + 1];
    prefix[0] = 0;
    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + a[i - 1];
    }

    for (int i = 0; i < n; i++) {
        int left_sum = prefix[i];
        int right_sum = prefix[n] - prefix[i + 1];
        if (left_sum == right_sum) {
            return i;
        }
    }
    return -1;
}

/* ── subarray_sum_equals_k ───────────────────────────────────────────── */

int subarray_sum_equals_k(const int *a, int n, int k)
{
    int prefix[1024 + 1];
    prefix[0] = 0;
    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + a[i - 1];
    }

    int count = 0;
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            if (prefix[r + 1] - prefix[l] == k) {
                count++;
            }
        }
    }
    return count;
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

int main(void)
{
    printf("=== Prefix Sums & Range Queries ===\n\n");

    /* --- build_prefix_sum --- */
    printf("build_prefix_sum:\n");
    {
        int a[] = {3, 1, 4, 1, 5};
        int prefix[6];
        build_prefix_sum(a, 5, prefix);
        TEST("prefix sum of [3,1,4,1,5]")
            EXPECT(prefix[0] == 0 && prefix[1] == 3 && prefix[2] == 4 &&
                   prefix[3] == 8 && prefix[4] == 9 && prefix[5] == 14);
    }
    {
        int a[] = {10};
        int prefix[2];
        build_prefix_sum(a, 1, prefix);
        TEST("prefix sum of single element [10]")
            EXPECT(prefix[0] == 0 && prefix[1] == 10);
    }

    /* --- range_sum --- */
    printf("\nrange_sum:\n");
    {
        int a[] = {3, 1, 4, 1, 5};
        int prefix[6];
        build_prefix_sum(a, 5, prefix);

        TEST("sum of a[1..3] = 1+4+1 = 6")
            EXPECT(range_sum(prefix, 1, 3) == 6);

        TEST("sum of a[0..4] = entire array = 14")
            EXPECT(range_sum(prefix, 0, 4) == 14);

        TEST("sum of a[2..2] = single element = 4")
            EXPECT(range_sum(prefix, 2, 2) == 4);

        TEST("sum of a[0..0] = first element = 3")
            EXPECT(range_sum(prefix, 0, 0) == 3);
    }

    /* --- count_range_evens --- */
    printf("\ncount_range_evens:\n");
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        TEST("evens in a[0..5] = {2,4,6} = 3")
            EXPECT(count_range_evens(a, 6, 0, 5) == 3);

        TEST("evens in a[1..3] = {2,4} = 2")
            EXPECT(count_range_evens(a, 6, 1, 3) == 2);

        TEST("evens in a[0..0] = {1} = 0")
            EXPECT(count_range_evens(a, 6, 0, 0) == 0);
    }

    /* --- equilibrium_index --- */
    printf("\nequilibrium_index:\n");
    {
        int a1[] = {1, 3, 5, 2, 2};
        TEST("equilibrium of [1,3,5,2,2] = index 2")
            EXPECT(equilibrium_index(a1, 5) == 2);
    }
    {
        int a2[] = {1, 2, 3};
        TEST("no equilibrium in [1,2,3]")
            EXPECT(equilibrium_index(a2, 3) == -1);
    }
    {
        int a3[] = {2, 0, 2};
        TEST("equilibrium of [2,0,2] = index 1")
            EXPECT(equilibrium_index(a3, 3) == 1);
    }

    /* --- subarray_sum_equals_k --- */
    printf("\nsubarray_sum_equals_k:\n");
    {
        int a[] = {1, 1, 1};
        TEST("subarrays summing to 2 in [1,1,1] = 2")
            EXPECT(subarray_sum_equals_k(a, 3, 2) == 2);
    }
    {
        int a[] = {1, 2, 3};
        TEST("subarrays summing to 3 in [1,2,3] = 2")
            EXPECT(subarray_sum_equals_k(a, 3, 3) == 2);
    }
    {
        int a[] = {3, 3, 3};
        TEST("subarrays summing to 9 in [3,3,3] = 1")
            EXPECT(subarray_sum_equals_k(a, 3, 9) == 1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
