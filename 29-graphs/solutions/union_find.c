/*
 * Solution: Union-Find (Disjoint Set Union)
 *
 * Union-Find with path compression and union by rank.
 * Amortized O(alpha(n)) per operation — effectively constant.
 *
 * 15 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_NODES 256

typedef struct {
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int size[MAX_NODES];
    int count;
    int n;
} UnionFind;

/* ── uf_init ─────────────────────────────────────────────────── */
void uf_init(UnionFind *uf, int n)
{
    uf->n = n;
    uf->count = n;
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;   /* each element is its own root */
        uf->rank[i] = 0;
        uf->size[i] = 1;
    }
}

/* ── uf_find (with path compression) ─────────────────────────── */
int uf_find(UnionFind *uf, int x)
{
    if (uf->parent[x] != x) {
        /* Recursively find root, and compress: point x directly to root */
        uf->parent[x] = uf_find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

/* ── uf_union (by rank) ──────────────────────────────────────── */
void uf_union(UnionFind *uf, int x, int y)
{
    int rx = uf_find(uf, x);
    int ry = uf_find(uf, y);

    if (rx == ry) {
        return;  /* already in the same group */
    }

    /* Attach shorter tree under taller tree */
    if (uf->rank[rx] < uf->rank[ry]) {
        uf->parent[rx] = ry;
        uf->size[ry] += uf->size[rx];
    } else if (uf->rank[rx] > uf->rank[ry]) {
        uf->parent[ry] = rx;
        uf->size[rx] += uf->size[ry];
    } else {
        /* Equal rank: pick rx as root, increment its rank */
        uf->parent[ry] = rx;
        uf->size[rx] += uf->size[ry];
        uf->rank[rx]++;
    }

    uf->count--;
}

/* ── uf_connected ────────────────────────────────────────────── */
int uf_connected(UnionFind *uf, int x, int y)
{
    return uf_find(uf, x) == uf_find(uf, y);
}

/* ── uf_count_components ─────────────────────────────────────── */
int uf_count_components(UnionFind *uf)
{
    return uf->count;
}

/* ── count_islands_uf ────────────────────────────────────────── */
int count_islands_uf(int grid[][5], int rows, int cols)
{
    int total = rows * cols;
    UnionFind uf;
    uf_init(&uf, total);

    /* Direction arrays: right and down only (avoid double-counting) */
    int dr[] = {0, 1};
    int dc[] = {1, 0};

    int water_count = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 0) {
                water_count++;
                continue;
            }
            /* Union with right and down neighbors if they are land */
            for (int d = 0; d < 2; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols
                    && grid[nr][nc] == 1) {
                    uf_union(&uf, r * cols + c, nr * cols + nc);
                }
            }
        }
    }

    /* Subtract water cells: each water cell is its own "component"
     * in the UF but is not an island */
    return uf.count - water_count;
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
