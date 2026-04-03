/*
 * Kata: Fenwick Tree (Binary Indexed Tree)
 *
 * A Fenwick tree is a simpler, more cache-friendly alternative to a
 * segment tree for prefix-sum-style queries. It supports:
 *   - Point update: add a value to position i — O(log n)
 *   - Prefix query: sum of elements [1..i] — O(log n)
 *   - Range query: sum of [l..r] via query(r) - query(l-1)
 *
 * The trick: each index stores a partial sum whose range is determined
 * by its LOWEST SET BIT (the bit_tricks kata taught you n & (-n)).
 *
 * Index (binary)  Responsible for range    # elements (= lowest set bit)
 *  1  (0001)      [1, 1]                   1
 *  2  (0010)      [1, 2]                   2
 *  3  (0011)      [3, 3]                   1
 *  4  (0100)      [1, 4]                   4
 *  5  (0101)      [5, 5]                   1
 *  6  (0110)      [5, 6]                   2
 *  7  (0111)      [7, 7]                   1
 *  8  (1000)      [1, 8]                   8
 *
 * Why 1-indexed? The bit trick i & (-i) gives 0 when i = 0, which
 * would cause an infinite loop. Starting at 1 avoids this entirely.
 *
 * Query prefix sum of index 7 (sum of elements 1..7):
 *   Start at i = 7 (0111):  add tree[7]  → i -= 7 & (-7) = 1 → i = 6
 *   Now i = 6 (0110):       add tree[6]  → i -= 6 & (-6) = 2 → i = 4
 *   Now i = 4 (0100):       add tree[4]  → i -= 4 & (-4) = 4 → i = 0
 *   i = 0, done. Sum = tree[7] + tree[6] + tree[4]
 *
 * Update index 3 (add delta to position 3):
 *   Start at i = 3 (0011):  tree[3] += delta → i += 3 & (-3) = 1 → i = 4
 *   Now i = 4 (0100):       tree[4] += delta → i += 4 & (-4) = 4 → i = 8
 *   Now i = 8 (1000):       tree[8] += delta → i += 8 & (-8) = 8 → i = 16
 *   i > n, done.
 *
 * Comparison with Segment Tree:
 *   ┌──────────────────┬──────────────┬──────────────┐
 *   │                  │ Segment Tree │ Fenwick Tree │
 *   ├──────────────────┼──────────────┼──────────────┤
 *   │ Point update     │ O(log n)     │ O(log n)     │
 *   │ Range query      │ O(log n)     │ O(log n)     │
 *   │ Space            │ 4n           │ n + 1        │
 *   │ Code complexity  │ Higher       │ Very simple  │
 *   │ Supports min/max │ Yes          │ No           │
 *   │ Lazy propagation │ Yes          │ No           │
 *   └──────────────────┴──────────────┴──────────────┘
 *
 * 15 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_N 1024

typedef struct {
    int tree[MAX_N + 1];  /* 1-indexed: tree[1] through tree[n] */
    int n;
} FenwickTree;

/* ── Exercise 1: fenwick_update ──────────────────────────────────────
 * Add `delta` to position `i` (1-indexed).
 *
 * Walk up by adding the lowest set bit:
 *   while (i <= ft->n) { ft->tree[i] += delta; i += i & (-i); }
 */
void fenwick_update(FenwickTree *ft, int i, int delta)
{
    /* TODO: Add delta to tree[i], then walk up: i += i & (-i) */
    (void)ft; (void)i; (void)delta; /* suppress unused warnings */
}

/* ── Exercise 2: fenwick_query ───────────────────────────────────────
 * Return the prefix sum from index 1 to i (inclusive).
 *
 * Walk down by stripping the lowest set bit:
 *   sum = 0; while (i > 0) { sum += ft->tree[i]; i -= i & (-i); }
 */
int fenwick_query(const FenwickTree *ft, int i)
{
    /* TODO: Accumulate tree[i], walk down: i -= i & (-i) */
    (void)ft; (void)i; /* suppress unused warnings */
    return 0; /* placeholder */
}

/* ── Exercise 3: fenwick_range_query ─────────────────────────────────
 * Return the sum of elements from index l to r (inclusive, 1-indexed).
 *
 * Formula: query(r) - query(l - 1)
 */
int fenwick_range_query(const FenwickTree *ft, int l, int r)
{
    /* TODO: Return fenwick_query(ft, r) - fenwick_query(ft, l - 1) */
    (void)ft; (void)l; (void)r; /* suppress unused warnings */
    return 0; /* placeholder */
}

/* ── Exercise 4: fenwick_build ───────────────────────────────────────
 * Build a Fenwick tree from an array a[] of length n.
 *
 * Simple approach: initialize tree to all zeros, then call
 * fenwick_update for each element.
 *
 * Note: a[] is 0-indexed but the Fenwick tree is 1-indexed.
 * a[0] goes to position 1, a[1] to position 2, etc.
 */
void fenwick_build(FenwickTree *ft, const int *a, int n)
{
    /* TODO:
     * ft->n = n;
     * memset(ft->tree, 0, sizeof(ft->tree));
     * for (int i = 0; i < n; i++)
     *     fenwick_update(ft, i + 1, a[i]);
     */
    ft->n = n;
    memset(ft->tree, 0, sizeof(ft->tree));
    (void)a; /* suppress unused warning */
}

/* ── Exercise 5: fenwick_point_query ─────────────────────────────────
 * Get the value of a single element at position i (1-indexed).
 *
 * This is just a range query of size 1: range_query(i, i).
 */
int fenwick_point_query(const FenwickTree *ft, int i)
{
    /* TODO: Return fenwick_range_query(ft, i, i) */
    (void)ft; (void)i; /* suppress unused warnings */
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
    printf("=== Fenwick Tree (Binary Indexed Tree) ===\n\n");

    /* --- fenwick_build & fenwick_query --- */
    printf("fenwick_build & fenwick_query:\n");
    {
        int a[] = {1, 3, 5, 7, 9, 11};
        FenwickTree ft;
        fenwick_build(&ft, a, 6);

        TEST("prefix sum [1..1] = 1")
            EXPECT(fenwick_query(&ft, 1) == 1);
        TEST("prefix sum [1..3] = 1+3+5 = 9")
            EXPECT(fenwick_query(&ft, 3) == 9);
        TEST("prefix sum [1..6] = 36")
            EXPECT(fenwick_query(&ft, 6) == 36);
    }

    /* --- fenwick_update --- */
    printf("\nfenwick_update:\n");
    {
        int a[] = {1, 3, 5, 7, 9, 11};
        FenwickTree ft;
        fenwick_build(&ft, a, 6);

        fenwick_update(&ft, 3, 5);  /* add 5 to position 3 (was 5, now 10) */
        TEST("after adding 5 to pos 3, prefix [1..3] = 14")
            EXPECT(fenwick_query(&ft, 3) == 14);
        TEST("after adding 5 to pos 3, prefix [1..6] = 41")
            EXPECT(fenwick_query(&ft, 6) == 41);
        TEST("prefix [1..2] unchanged = 4")
            EXPECT(fenwick_query(&ft, 2) == 4);
    }

    /* --- fenwick_range_query --- */
    printf("\nfenwick_range_query:\n");
    {
        int a[] = {1, 3, 5, 7, 9, 11};
        FenwickTree ft;
        fenwick_build(&ft, a, 6);

        TEST("range [2,4] = 3+5+7 = 15")
            EXPECT(fenwick_range_query(&ft, 2, 4) == 15);
        TEST("range [1,6] = entire array = 36")
            EXPECT(fenwick_range_query(&ft, 1, 6) == 36);
        TEST("range [3,3] = single element = 5")
            EXPECT(fenwick_range_query(&ft, 3, 3) == 5);
        TEST("range [5,6] = 9+11 = 20")
            EXPECT(fenwick_range_query(&ft, 5, 6) == 20);
    }

    /* --- fenwick_build (separate test) --- */
    printf("\nfenwick_build:\n");
    {
        int a[] = {10, 20, 30};
        FenwickTree ft;
        fenwick_build(&ft, a, 3);
        TEST("build [10,20,30]: prefix [1..3] = 60")
            EXPECT(fenwick_query(&ft, 3) == 60);
        TEST("build [10,20,30]: prefix [1..1] = 10")
            EXPECT(fenwick_query(&ft, 1) == 10);
    }

    /* --- fenwick_point_query --- */
    printf("\nfenwick_point_query:\n");
    {
        int a[] = {1, 3, 5, 7, 9, 11};
        FenwickTree ft;
        fenwick_build(&ft, a, 6);

        TEST("point query pos 1 = 1")
            EXPECT(fenwick_point_query(&ft, 1) == 1);
        TEST("point query pos 4 = 7")
            EXPECT(fenwick_point_query(&ft, 4) == 7);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
