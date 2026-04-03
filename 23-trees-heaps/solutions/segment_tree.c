/*
 * Solution: Segment Tree
 */

#include <stdio.h>
#include <limits.h>

#define MAX_N 1024

typedef struct {
    int tree[4 * MAX_N];
    int n;
} SegTree;

/* ── seg_build ───────────────────────────────────────────────────────── */

static void seg_build_helper(SegTree *st, const int *arr,
                              int node, int start, int end)
{
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }
    int mid = (start + end) / 2;
    seg_build_helper(st, arr, node * 2 + 1, start, mid);
    seg_build_helper(st, arr, node * 2 + 2, mid + 1, end);
    st->tree[node] = st->tree[node * 2 + 1] + st->tree[node * 2 + 2];
}

void seg_build(SegTree *st, const int *arr, int n)
{
    st->n = n;
    seg_build_helper(st, arr, 0, 0, n - 1);
}

/* ── seg_query ───────────────────────────────────────────────────────── */

static int seg_query_helper(const SegTree *st, int node,
                             int start, int end, int l, int r)
{
    /* No overlap */
    if (start > r || end < l) {
        return 0;
    }
    /* Complete overlap */
    if (l <= start && end <= r) {
        return st->tree[node];
    }
    /* Partial overlap — recurse on both children */
    int mid = (start + end) / 2;
    int left_sum = seg_query_helper(st, node * 2 + 1, start, mid, l, r);
    int right_sum = seg_query_helper(st, node * 2 + 2, mid + 1, end, l, r);
    return left_sum + right_sum;
}

int seg_query(const SegTree *st, int l, int r)
{
    return seg_query_helper(st, 0, 0, st->n - 1, l, r);
}

/* ── seg_update ──────────────────────────────────────────────────────── */

static void seg_update_helper(SegTree *st, int node,
                               int start, int end, int idx, int new_val)
{
    if (start == end) {
        st->tree[node] = new_val;
        return;
    }
    int mid = (start + end) / 2;
    if (idx <= mid) {
        seg_update_helper(st, node * 2 + 1, start, mid, idx, new_val);
    } else {
        seg_update_helper(st, node * 2 + 2, mid + 1, end, idx, new_val);
    }
    st->tree[node] = st->tree[node * 2 + 1] + st->tree[node * 2 + 2];
}

void seg_update(SegTree *st, int idx, int new_val)
{
    seg_update_helper(st, 0, 0, st->n - 1, idx, new_val);
}

/* ── seg_range_min ───────────────────────────────────────────────────── */

typedef struct {
    int tree[4 * MAX_N];
    int n;
} MinSegTree;

static void min_seg_build_helper(MinSegTree *st, const int *arr,
                                  int node, int start, int end)
{
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }
    int mid = (start + end) / 2;
    min_seg_build_helper(st, arr, node * 2 + 1, start, mid);
    min_seg_build_helper(st, arr, node * 2 + 2, mid + 1, end);
    int l = st->tree[node * 2 + 1];
    int r = st->tree[node * 2 + 2];
    st->tree[node] = (l < r) ? l : r;
}

void min_seg_build(MinSegTree *st, const int *arr, int n)
{
    st->n = n;
    min_seg_build_helper(st, arr, 0, 0, n - 1);
}

static int min_seg_query_helper(const MinSegTree *st, int node,
                                 int start, int end, int l, int r)
{
    if (start > r || end < l) {
        return INT_MAX;
    }
    if (l <= start && end <= r) {
        return st->tree[node];
    }
    int mid = (start + end) / 2;
    int lv = min_seg_query_helper(st, node * 2 + 1, start, mid, l, r);
    int rv = min_seg_query_helper(st, node * 2 + 2, mid + 1, end, l, r);
    return (lv < rv) ? lv : rv;
}

int min_seg_query(const MinSegTree *st, int l, int r)
{
    return min_seg_query_helper(st, 0, 0, st->n - 1, l, r);
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

        seg_update(&st, 2, 10);
        TEST("after update idx 2 to 10, query [0,5] = 41")
            EXPECT(seg_query(&st, 0, 5) == 41);
        TEST("after update, query [2,2] = 10")
            EXPECT(seg_query(&st, 2, 2) == 10);
        TEST("after update, query [1,4] = 3+10+7+9 = 29")
            EXPECT(seg_query(&st, 1, 4) == 29);

        seg_update(&st, 0, 100);
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
