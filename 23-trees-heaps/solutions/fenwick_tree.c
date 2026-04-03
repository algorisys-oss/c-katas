/*
 * Solution: Fenwick Tree (Binary Indexed Tree)
 */

#include <stdio.h>
#include <string.h>

#define MAX_N 1024

typedef struct {
    int tree[MAX_N + 1];  /* 1-indexed */
    int n;
} FenwickTree;

/* ── fenwick_update ──────────────────────────────────────────────────── */

void fenwick_update(FenwickTree *ft, int i, int delta)
{
    while (i <= ft->n) {
        ft->tree[i] += delta;
        i += i & (-i);  /* add lowest set bit to move to parent */
    }
}

/* ── fenwick_query ───────────────────────────────────────────────────── */

int fenwick_query(const FenwickTree *ft, int i)
{
    int sum = 0;
    while (i > 0) {
        sum += ft->tree[i];
        i -= i & (-i);  /* strip lowest set bit */
    }
    return sum;
}

/* ── fenwick_range_query ─────────────────────────────────────────────── */

int fenwick_range_query(const FenwickTree *ft, int l, int r)
{
    return fenwick_query(ft, r) - fenwick_query(ft, l - 1);
}

/* ── fenwick_build ───────────────────────────────────────────────────── */

void fenwick_build(FenwickTree *ft, const int *a, int n)
{
    ft->n = n;
    memset(ft->tree, 0, sizeof(ft->tree));
    for (int i = 0; i < n; i++) {
        fenwick_update(ft, i + 1, a[i]);
    }
}

/* ── fenwick_point_query ─────────────────────────────────────────────── */

int fenwick_point_query(const FenwickTree *ft, int i)
{
    return fenwick_range_query(ft, i, i);
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

        fenwick_update(&ft, 3, 5);
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
