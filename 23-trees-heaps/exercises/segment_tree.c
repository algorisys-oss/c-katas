/*
 * Kata: Segment Tree
 *
 * A segment tree answers range queries in O(log n) and supports point
 * updates in O(log n). It solves the problem that prefix sums can't:
 * efficient updates.
 *
 * The idea: build a binary tree where each node stores the answer for
 * a range. The root stores the answer for the whole array. Each node's
 * children split the range in half.
 *
 * For array [1, 3, 5, 7, 9, 11]:
 *
 *                    [36]                   range [0,5] (sum of all)
 *                  /      \
 *              [9]          [27]            [0,2] and [3,5]
 *             /   \        /    \
 *          [4]    [5]   [16]   [11]         [0,1],[2,2],[3,4],[5,5]
 *          / \          / \
 *        [1] [3]     [7] [9]               leaves = original values
 *
 * Storage: use an array (like heaps). Node i has children at 2*i+1 and 2*i+2.
 * An array of size 4*n is always large enough.
 *
 * Query for range [1,4] (sum of indices 1..4 = 3+5+7+9 = 24):
 *
 *                    [36]         Does [0,5] overlap [1,4]? Partial → recurse
 *                  /      \
 *              [9]          [27]  [0,2] partial → recurse.  [3,5] partial → recurse
 *             /   \        /    \
 *          [4]    [5]   [16]   [11]  [0,1] partial, [2,2] inside → return 5
 *          / \          / \          [3,4] inside → return 16, [5,5] outside → 0
 *        [1] [3]     [7] [9]
 *         ^    ^                     [0,0] outside → 0, [1,1] inside → return 3
 *
 *   Result: 0 + 3 + 5 + 16 + 0 = 24
 *
 * Update index 2 from 5 to 10 — walk from leaf to root, updating sums:
 *
 *   Leaf [5] → [10].  Parent [9] → [14].  Root [36] → [41].
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <limits.h>

#define MAX_N 1024

typedef struct {
    int tree[4 * MAX_N];  /* segment tree array (4x input size is safe) */
    int n;                 /* number of elements */
} SegTree;

/* ── Exercise 1: seg_build ───────────────────────────────────────────
 * Build a segment tree for range sum queries.
 *
 * Recursive approach:
 *   seg_build_helper(st, arr, node, start, end)
 *   - If start == end: this is a leaf, st->tree[node] = arr[start]
 *   - Otherwise: split into [start, mid] and [mid+1, end]
 *     - Build left child (node*2+1) and right child (node*2+2)
 *     - st->tree[node] = st->tree[node*2+1] + st->tree[node*2+2]
 */
static void seg_build_helper(SegTree *st, const int *arr,
                              int node, int start, int end)
{
    /* TODO:
     * if (start == end) { st->tree[node] = arr[start]; return; }
     * int mid = (start + end) / 2;
     * seg_build_helper(st, arr, node*2+1, start, mid);
     * seg_build_helper(st, arr, node*2+2, mid+1, end);
     * st->tree[node] = st->tree[node*2+1] + st->tree[node*2+2];
     */
    (void)st; (void)arr; (void)node; (void)start; (void)end;
}

void seg_build(SegTree *st, const int *arr, int n)
{
    st->n = n;
    /* TODO: call seg_build_helper(st, arr, 0, 0, n-1) */
    (void)arr;
}

/* ── Exercise 2: seg_query ───────────────────────────────────────────
 * Query the sum of elements in range [l, r] (inclusive).
 *
 * Recursive approach:
 *   seg_query_helper(st, node, start, end, l, r)
 *   - No overlap (start > r || end < l): return 0
 *   - Complete overlap (l <= start && end <= r): return st->tree[node]
 *   - Partial overlap: recurse on both children, return sum
 */
static int seg_query_helper(const SegTree *st, int node,
                             int start, int end, int l, int r)
{
    /* TODO:
     * if (start > r || end < l) return 0;
     * if (l <= start && end <= r) return st->tree[node];
     * int mid = (start + end) / 2;
     * int left_sum = seg_query_helper(st, node*2+1, start, mid, l, r);
     * int right_sum = seg_query_helper(st, node*2+2, mid+1, end, l, r);
     * return left_sum + right_sum;
     */
    (void)st; (void)node; (void)start; (void)end; (void)l; (void)r;
    return 0; /* placeholder */
}

int seg_query(const SegTree *st, int l, int r)
{
    /* TODO: return seg_query_helper(st, 0, 0, st->n - 1, l, r) */
    (void)l; (void)r; (void)st;
    return 0; /* placeholder */
}

/* ── Exercise 3: seg_update ──────────────────────────────────────────
 * Update the element at index idx to new_val.
 *
 * Walk down to the leaf for idx, update it, then propagate changes up.
 *
 * Recursive approach:
 *   seg_update_helper(st, node, start, end, idx, new_val)
 *   - If start == end: st->tree[node] = new_val; return
 *   - Otherwise: determine which child contains idx, recurse
 *   - Recalculate: st->tree[node] = st->tree[node*2+1] + st->tree[node*2+2]
 */
static void seg_update_helper(SegTree *st, int node,
                               int start, int end, int idx, int new_val)
{
    /* TODO:
     * if (start == end) { st->tree[node] = new_val; return; }
     * int mid = (start + end) / 2;
     * if (idx <= mid)
     *     seg_update_helper(st, node*2+1, start, mid, idx, new_val);
     * else
     *     seg_update_helper(st, node*2+2, mid+1, end, idx, new_val);
     * st->tree[node] = st->tree[node*2+1] + st->tree[node*2+2];
     */
    (void)st; (void)node; (void)start; (void)end; (void)idx; (void)new_val;
}

void seg_update(SegTree *st, int idx, int new_val)
{
    /* TODO: seg_update_helper(st, 0, 0, st->n - 1, idx, new_val) */
    (void)idx; (void)new_val; (void)st;
}

/* ── Exercise 4: seg_range_min ───────────────────────────────────────
 * Build and query a segment tree for range MINIMUM queries.
 *
 * Same structure, but instead of summing, take the minimum.
 * - Build: node = min(left_child, right_child)
 * - Query: return min of overlapping children (return INT_MAX for no overlap)
 * - The identity element for min is INT_MAX (like 0 is for sum)
 */

typedef struct {
    int tree[4 * MAX_N];
    int n;
} MinSegTree;

static void min_seg_build_helper(MinSegTree *st, const int *arr,
                                  int node, int start, int end)
{
    /* TODO:
     * if (start == end) { st->tree[node] = arr[start]; return; }
     * int mid = (start + end) / 2;
     * min_seg_build_helper(st, arr, node*2+1, start, mid);
     * min_seg_build_helper(st, arr, node*2+2, mid+1, end);
     * int l = st->tree[node*2+1], r = st->tree[node*2+2];
     * st->tree[node] = (l < r) ? l : r;
     */
    (void)st; (void)arr; (void)node; (void)start; (void)end;
}

void min_seg_build(MinSegTree *st, const int *arr, int n)
{
    st->n = n;
    /* TODO: min_seg_build_helper(st, arr, 0, 0, n-1) */
    (void)arr;
}

static int min_seg_query_helper(const MinSegTree *st, int node,
                                 int start, int end, int l, int r)
{
    /* TODO:
     * if (start > r || end < l) return INT_MAX;
     * if (l <= start && end <= r) return st->tree[node];
     * int mid = (start + end) / 2;
     * int lv = min_seg_query_helper(st, node*2+1, start, mid, l, r);
     * int rv = min_seg_query_helper(st, node*2+2, mid+1, end, l, r);
     * return (lv < rv) ? lv : rv;
     */
    (void)st; (void)node; (void)start; (void)end; (void)l; (void)r;
    return 0; /* placeholder */
}

int min_seg_query(const MinSegTree *st, int l, int r)
{
    /* TODO: return min_seg_query_helper(st, 0, 0, st->n - 1, l, r) */
    (void)l; (void)r; (void)st;
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
    printf("=== Segment Tree ===\n\n");

    /* --- seg_build --- */
    printf("seg_build:\n");
    {
        int arr[] = {1, 3, 5, 7, 9, 11};
        SegTree st;
        seg_build(&st, arr, 6);
        TEST("root stores total sum 36")
            EXPECT(st.tree[0] == 36);
        TEST("left child stores sum of [0,2] = 9")
            EXPECT(st.tree[1] == 9);
    }

    /* --- seg_query --- */
    printf("\nseg_query:\n");
    {
        int arr[] = {1, 3, 5, 7, 9, 11};
        SegTree st;
        seg_build(&st, arr, 6);

        TEST("query [0,5] = entire array = 36")
            EXPECT(seg_query(&st, 0, 5) == 36);
        TEST("query [1,4] = 3+5+7+9 = 24")
            EXPECT(seg_query(&st, 1, 4) == 24);
        TEST("query [2,2] = single element = 5")
            EXPECT(seg_query(&st, 2, 2) == 5);
        TEST("query [3,5] = 7+9+11 = 27")
            EXPECT(seg_query(&st, 3, 5) == 27);
        TEST("query [0,0] = first element = 1")
            EXPECT(seg_query(&st, 0, 0) == 1);
    }

    /* --- seg_update --- */
    printf("\nseg_update:\n");
    {
        int arr[] = {1, 3, 5, 7, 9, 11};
        SegTree st;
        seg_build(&st, arr, 6);

        seg_update(&st, 2, 10);  /* change index 2 from 5 to 10 */
        TEST("after update idx 2 to 10, query [0,5] = 41")
            EXPECT(seg_query(&st, 0, 5) == 41);
        TEST("after update, query [2,2] = 10")
            EXPECT(seg_query(&st, 2, 2) == 10);
        TEST("after update, query [1,4] = 3+10+7+9 = 29")
            EXPECT(seg_query(&st, 1, 4) == 29);

        seg_update(&st, 0, 100);  /* change index 0 from 1 to 100 */
        TEST("after 2nd update, query [0,5] = 140")
            EXPECT(seg_query(&st, 0, 5) == 140);
    }

    /* --- seg_range_min --- */
    printf("\nseg_range_min:\n");
    {
        int arr[] = {5, 2, 8, 1, 9, 3};
        MinSegTree mst;
        min_seg_build(&mst, arr, 6);

        TEST("min of [0,5] = 1")
            EXPECT(min_seg_query(&mst, 0, 5) == 1);
        TEST("min of [0,2] = 2")
            EXPECT(min_seg_query(&mst, 0, 2) == 2);
        TEST("min of [4,5] = 3")
            EXPECT(min_seg_query(&mst, 4, 5) == 3);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
