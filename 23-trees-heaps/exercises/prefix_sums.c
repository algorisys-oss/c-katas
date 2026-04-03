/*
 * Kata: Prefix Sums & Range Queries
 *
 * Prefix sums let you answer "what is the sum from index l to r?" in O(1)
 * after an O(n) preprocessing step. The trick: precompute running totals.
 *
 *   Original array a[]:     [3,  1,  4,  1,  5]
 *   Prefix sum prefix[]:  [0,  3,  4,  8,  9, 14]
 *                          ^                    ^
 *                     prefix[0]=0         prefix[5]=sum of all
 *
 *   How prefix[] maps to a[]:
 *
 *     prefix[0] = 0                              (empty sum)
 *     prefix[1] = a[0]                         = 3
 *     prefix[2] = a[0] + a[1]                  = 4
 *     prefix[3] = a[0] + a[1] + a[2]           = 8
 *     prefix[4] = a[0] + a[1] + a[2] + a[3]   = 9
 *     prefix[5] = a[0] + a[1] + a[2] + a[3] + a[4] = 14
 *
 *   Range query: sum of a[l..r] = prefix[r+1] - prefix[l]
 *
 *     sum of a[1..3] = prefix[4] - prefix[1] = 9 - 3 = 6
 *
 *     On a number line:
 *
 *       index:   0    1    2    3    4
 *       value: [ 3 ][ 1 ][ 4 ][ 1 ][ 5 ]
 *                    |<-- sum = 6 -->|
 *                    l=1             r=3
 *
 *       prefix[4] = sum of a[0..3] = 9
 *     - prefix[1] = sum of a[0..0] = 3
 *       ─────────────────────────────
 *       result     = sum of a[1..3] = 6
 *
 * 15 tests total.
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Exercise 1: build_prefix_sum ────────────────────────────────────
 * Given an array a[] of length n, build a prefix sum array of length n+1.
 *
 * prefix[0] = 0
 * prefix[i] = a[0] + a[1] + ... + a[i-1]   for i = 1..n
 *
 * The caller provides the output array (already allocated, length n+1).
 */
void build_prefix_sum(const int *a, int n, int *prefix)
{
    /* TODO: Set prefix[0] = 0, then loop i = 1..n:
     *       prefix[i] = prefix[i-1] + a[i-1] */
}

/* ── Exercise 2: range_sum ───────────────────────────────────────────
 * Given a precomputed prefix sum array, return the sum of a[l..r]
 * (inclusive) in O(1).
 *
 * Formula: prefix[r+1] - prefix[l]
 */
int range_sum(const int *prefix, int l, int r)
{
    /* TODO: Return prefix[r+1] - prefix[l] */
    return 0; /* placeholder */
}

/* ── Exercise 3: count_range_evens ───────────────────────────────────
 * Given an array a[] of length n, count how many even numbers appear
 * between index l and r (inclusive).
 *
 * Strategy: build a "prefix count of evens" array, then query it like
 * a prefix sum. even_prefix[i] = number of even values in a[0..i-1].
 *
 * The caller provides the output count and has NOT built the prefix
 * array — you do everything here.
 */
int count_range_evens(const int *a, int n, int l, int r)
{
    /* TODO:
     * 1. Build a prefix count array (on the stack, max 1024 elements)
     *    even_prefix[0] = 0
     *    even_prefix[i] = even_prefix[i-1] + (a[i-1] % 2 == 0 ? 1 : 0)
     * 2. Return even_prefix[r+1] - even_prefix[l]
     */
    (void)a; (void)n; (void)l; (void)r; /* suppress unused warnings */
    return 0; /* placeholder */
}

/* ── Exercise 4: equilibrium_index ───────────────────────────────────
 * Find the first index where the sum of elements to the left equals
 * the sum of elements to the right.
 *
 *   [1, 3, 5, 2, 2]
 *        ^
 *   left sum of index 2 = a[0]+a[1] = 4
 *   right sum of index 2 = a[3]+a[4] = 4  → equilibrium!
 *
 * Use prefix sums:
 *   left_sum(i)  = prefix[i]
 *   right_sum(i) = prefix[n] - prefix[i+1]
 *
 * Return the first equilibrium index, or -1 if none exists.
 */
int equilibrium_index(const int *a, int n)
{
    /* TODO:
     * 1. Build prefix sum array
     * 2. For each i from 0 to n-1:
     *      if prefix[i] == prefix[n] - prefix[i+1], return i
     * 3. Return -1 if none found
     */
    (void)a; (void)n; /* suppress unused warnings */
    return -1; /* placeholder */
}

/* ── Exercise 5: subarray_sum_equals_k ───────────────────────────────
 * Count the number of contiguous subarrays whose sum equals k.
 *
 * A subarray a[l..r] has sum = prefix[r+1] - prefix[l].
 * So we need to count pairs (l, r) where prefix[r+1] - prefix[l] == k.
 *
 * Simple approach (O(n^2)): for each pair (l, r), check if
 * prefix[r+1] - prefix[l] == k.
 *
 * (A hash map approach gives O(n) but we haven't built hash maps yet,
 * so the O(n^2) approach is fine for learning.)
 */
int subarray_sum_equals_k(const int *a, int n, int k)
{
    /* TODO:
     * 1. Build prefix sum array
     * 2. For each l from 0 to n-1:
     *      for each r from l to n-1:
     *        if prefix[r+1] - prefix[l] == k, count++
     * 3. Return count
     */
    (void)a; (void)n; (void)k; /* suppress unused warnings */
    return 0; /* placeholder */
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
