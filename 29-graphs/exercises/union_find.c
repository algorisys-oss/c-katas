/*
 * Kata: Union-Find (Disjoint Set Union)
 *
 * Union-Find answers one question blazingly fast:
 * "Are these two elements in the same group?"
 *
 * Think of social networks. When person A befriends person B, their friend
 * groups merge. Union-Find tracks which group everyone belongs to.
 *
 * Two operations:
 *   - find(x):      Which group does x belong to? (returns the group's root)
 *   - union(x, y):  Merge the groups containing x and y
 *
 * Functions to implement:
 *   - uf_init             (initialize each element as its own group)
 *   - uf_find             (find root with path compression)
 *   - uf_union            (merge two groups by rank)
 *   - uf_connected        (check if two elements are in the same group)
 *   - uf_count_components (return the number of disjoint sets)
 *   - count_islands_uf    (count islands on a grid using Union-Find)
 *
 * 15 tests total.
 *
 *
 * ── How it works: Forest of Trees ──────────────────────────────────
 *
 * Each group is a tree. The root of the tree is the group's representative.
 * parent[i] points to i's parent. Roots point to themselves (self-loop).
 *
 *   Initially (5 elements, each is its own group):
 *
 *     0    1    2    3    4      parent: [0, 1, 2, 3, 4]
 *     (five separate trees)              count = 5
 *
 *   After union(0,1) and union(2,3):
 *
 *     0    2                     parent: [0, 0, 2, 2, 4]
 *    /    /                              count = 3
 *   1    3       4
 *
 *   After union(0,2):
 *
 *       0                        parent: [0, 0, 0, 2, 4]
 *      / \                               count = 2
 *     1   2
 *         |
 *         3        4
 *
 *
 * ── Path Compression ──────────────────────────────────────────────
 *
 * Without compression, find() walks up the tree to the root.
 * Long chains = slow find. Path compression fixes this by making
 * every node on the path point DIRECTLY to the root.
 *
 *   Before find(4):           After find(4):
 *
 *       0                         0
 *       |                       / | \
 *       1                      1  2  4
 *       |                         |
 *       2                         3
 *       |
 *       3
 *       |
 *       4
 *
 *   Chain 4→3→2→1→0            All point to root 0
 *
 *
 * ── Union by Rank ─────────────────────────────────────────────────
 *
 * When merging two trees, attach the SHORTER tree under the TALLER one.
 * This keeps trees flat (logarithmic height).
 *
 *   Bad (no rank):              Good (union by rank):
 *
 *       0                           0
 *       |                          / \
 *       1                         1   2
 *       |                             |
 *       2                             3
 *       |
 *       3
 *
 *   height = 4 (slow!)          height = 2 (fast!)
 *
 *
 * ── Islands Grid Example ──────────────────────────────────────────
 *
 *   Grid:         Union operations:
 *   1 1 0         union(0,1)    ← cells (0,0) and (0,1) are neighbors
 *   0 1 0         union(1,4)    ← cells (0,1) and (1,1) are neighbors
 *   0 0 1         cell (2,2)=1 has no land neighbors
 *
 *   Result: 2 islands (group {0,1,4} and group {8})
 *
 *
 * Hints:
 *   - uf_find: recursion makes path compression elegant:
 *       parent[x] = uf_find(uf, parent[x])
 *   - uf_union: find roots first, then compare ranks
 *   - For islands: map (row, col) to a flat index: row * cols + col
 */

#include <stdio.h>
#include <string.h>

#define MAX_NODES 256

typedef struct {
    int parent[MAX_NODES];  /* parent[i] = parent of i (root if parent[i] == i) */
    int rank[MAX_NODES];    /* rank[i] = upper bound on height of subtree at i */
    int size[MAX_NODES];    /* size[i] = number of elements in i's group (only valid for roots) */
    int count;              /* number of disjoint sets */
    int n;                  /* total number of elements */
} UnionFind;

/* ── Exercise 1: uf_init ─────────────────────────────────────────
 * Initialize n elements. Each element is its own parent (self-loop),
 * rank 0, size 1. count = n (every element is its own group).
 */
void uf_init(UnionFind *uf, int n)
{
    /* TODO: set uf->n, uf->count, and initialize parent/rank/size arrays */
    (void)uf; (void)n;
}

/* ── Exercise 2: uf_find ─────────────────────────────────────────
 * Find the root (representative) of x's group.
 *
 * Naive version: follow parent pointers until parent[x] == x.
 * Better version: add PATH COMPRESSION — make every node on the
 * path point directly to the root.
 *
 * Recursive approach:
 *   if parent[x] != x:
 *       parent[x] = uf_find(uf, parent[x])   ← compression!
 *   return parent[x]
 */
int uf_find(UnionFind *uf, int x)
{
    /* TODO: find root of x with path compression */
    (void)uf;
    return x; /* placeholder */
}

/* ── Exercise 3: uf_union ────────────────────────────────────────
 * Merge the groups containing x and y.
 *
 * Steps:
 *   1. Find roots: root_x = find(x), root_y = find(y)
 *   2. If same root, already in same group — do nothing
 *   3. Union by rank: attach shorter tree under taller tree
 *      - If rank[root_x] < rank[root_y]: parent[root_x] = root_y
 *      - If rank[root_x] > rank[root_y]: parent[root_y] = root_x
 *      - If equal: pick one, increment its rank
 *   4. Update size of the new root
 *   5. Decrement count (one fewer group)
 */
void uf_union(UnionFind *uf, int x, int y)
{
    /* TODO: merge groups containing x and y using union by rank */
    (void)uf; (void)x; (void)y;
}

/* ── Exercise 4: uf_connected ────────────────────────────────────
 * Return 1 if x and y are in the same group, 0 otherwise.
 * Just check if find(x) == find(y).
 */
int uf_connected(UnionFind *uf, int x, int y)
{
    /* TODO: check if x and y share the same root */
    (void)uf; (void)x; (void)y;
    return 0; /* placeholder */
}

/* ── Exercise 5: uf_count_components ─────────────────────────────
 * Return the number of disjoint sets.
 */
int uf_count_components(UnionFind *uf)
{
    /* TODO: return the current number of disjoint sets */
    (void)uf;
    return 0; /* placeholder */
}

/* ── Exercise 6: count_islands_uf ────────────────────────────────
 * Count the number of islands in a grid using Union-Find.
 *
 * Grid is a 2D array of 0s and 1s.
 *   - 1 = land, 0 = water
 *   - Connected land cells (up/down/left/right) form an island
 *
 * Strategy:
 *   1. Initialize Union-Find with rows * cols elements
 *   2. For each land cell, union it with adjacent land cells
 *      (only need to check RIGHT and DOWN to avoid double-counting)
 *   3. Count components, but only among land cells
 *
 * Hint: map (row, col) to flat index: row * cols + col
 * Hint: the final count needs to subtract water cells from uf->count,
 *       OR count land-cell roots manually.
 */
int count_islands_uf(int grid[][5], int rows, int cols)
{
    /* TODO: use Union-Find to count islands */
    (void)grid; (void)rows; (void)cols;
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
    printf("=== Union-Find (Disjoint Set Union) ===\n\n");

    /* --- uf_init tests --- */
    printf("uf_init:\n");
    {
        UnionFind uf;
        uf_init(&uf, 5);

        TEST("init: each element is its own parent")
            EXPECT(uf.parent[0] == 0 && uf.parent[1] == 1 &&
                   uf.parent[2] == 2 && uf.parent[3] == 3 &&
                   uf.parent[4] == 4);
    }

    /* --- uf_find tests --- */
    printf("\nuf_find:\n");
    {
        UnionFind uf;
        uf_init(&uf, 5);

        TEST("find on singleton returns itself")
            EXPECT(uf_find(&uf, 3) == 3);
    }
    {
        /* Manually build a chain: 4→3→2→1→0 to test path compression */
        UnionFind uf;
        uf_init(&uf, 5);
        uf.parent[4] = 3;
        uf.parent[3] = 2;
        uf.parent[2] = 1;
        uf.parent[1] = 0;

        TEST("find follows chain to root")
            EXPECT(uf_find(&uf, 4) == 0);

        /* After path compression, all nodes should point to root */
        TEST("path compression flattens chain")
            EXPECT(uf.parent[4] == 0 && uf.parent[3] == 0 &&
                   uf.parent[2] == 0 && uf.parent[1] == 0);
    }

    /* --- uf_union tests --- */
    printf("\nuf_union:\n");
    {
        UnionFind uf;
        uf_init(&uf, 6);

        uf_union(&uf, 0, 1);
        TEST("union(0,1): now connected")
            EXPECT(uf_find(&uf, 0) == uf_find(&uf, 1));

        uf_union(&uf, 2, 3);
        uf_union(&uf, 0, 2);
        TEST("union chain: 0,1,2,3 all connected")
            EXPECT(uf_find(&uf, 0) == uf_find(&uf, 3));

        TEST("union chain: 4 still separate")
            EXPECT(uf_find(&uf, 4) != uf_find(&uf, 0));

        /* Size tracking: group {0,1,2,3} should have size 4 */
        int root = uf_find(&uf, 0);
        TEST("group size is 4 after merging 4 elements")
            EXPECT(uf.size[root] == 4);
    }

    /* --- uf_connected tests --- */
    printf("\nuf_connected:\n");
    {
        UnionFind uf;
        uf_init(&uf, 5);
        uf_union(&uf, 0, 1);
        uf_union(&uf, 1, 2);

        TEST("connected(0,2) after union chain")
            EXPECT(uf_connected(&uf, 0, 2) == 1);

        TEST("not connected(0,4) without union")
            EXPECT(uf_connected(&uf, 0, 4) == 0);
    }

    /* --- uf_count_components tests --- */
    printf("\nuf_count_components:\n");
    {
        UnionFind uf;
        uf_init(&uf, 5);

        TEST("5 elements, no unions → 5 components")
            EXPECT(uf_count_components(&uf) == 5);

        uf_union(&uf, 0, 1);
        uf_union(&uf, 2, 3);
        TEST("after 2 unions → 3 components")
            EXPECT(uf_count_components(&uf) == 3);
    }

    /* --- count_islands_uf tests --- */
    printf("\ncount_islands_uf:\n");
    {
        int grid[4][5] = {
            {1, 1, 0, 0, 0},
            {1, 1, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 1, 1}
        };
        TEST("3 islands in 4x5 grid")
            EXPECT(count_islands_uf(grid, 4, 5) == 3);
    }
    {
        int grid[4][5] = {
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1}
        };
        TEST("all land → 1 island")
            EXPECT(count_islands_uf(grid, 4, 5) == 1);
    }
    {
        int grid[3][5] = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        };
        TEST("all water → 0 islands")
            EXPECT(count_islands_uf(grid, 3, 5) == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
