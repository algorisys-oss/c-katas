/*
 * Kata: Topological Sort (Kahn's Algorithm) & Cycle Detection
 *
 * Implement topological sorting on a directed graph using Kahn's
 * algorithm (BFS-based), and detect cycles in directed graphs.
 *
 * A topological sort orders vertices so that for every directed edge
 * u -> v, vertex u comes before v in the ordering.  This only works
 * on DAGs (Directed Acyclic Graphs).
 *
 * Functions to implement:
 *   - topo_sort     (Kahn's algorithm — BFS with in-degree tracking)
 *   - has_cycle     (detect if a directed graph has a cycle)
 *
 * 10 tests total.
 *
 * Hints:
 *   - Kahn's algorithm:
 *     1. Compute in-degree for every vertex
 *     2. Enqueue all vertices with in-degree 0
 *     3. While queue is not empty:
 *        a. Dequeue vertex u, add to result
 *        b. For each neighbor v of u, decrement in-degree[v]
 *        c. If in-degree[v] becomes 0, enqueue v
 *     4. If result has fewer than n vertices, a cycle exists
 *   - has_cycle can reuse topo_sort internally
 */

#include <stdio.h>
#include <string.h>

#define MAX_VERTICES 32

/* ── Exercise 1: topo_sort ───────────────────────────────────────
 * Perform topological sort on a directed graph using Kahn's algorithm.
 *
 * Parameters:
 *   adj    — adjacency matrix: adj[u][v] = 1 means edge u -> v
 *   n      — number of vertices (labeled 0 to n-1)
 *   result — output array: topologically sorted vertex order
 *
 * Returns 1 if successful (graph is a DAG).
 * Returns 0 if a cycle is detected (not all vertices can be sorted).
 *
 * Algorithm:
 *   1. Compute in_degree[v] = number of edges pointing TO v
 *   2. Put all vertices with in_degree 0 into a queue
 *   3. Process queue:
 *      - Dequeue u, add to result
 *      - For each edge u -> v: in_degree[v]--
 *      - If in_degree[v] == 0, enqueue v
 *   4. If count < n, there's a cycle
 */
int topo_sort(int adj[][MAX_VERTICES], int n, int result[])
{
    /* TODO: implement Kahn's algorithm */
    (void)adj; (void)n; (void)result;
    return 0;
}

/* ── Exercise 2: has_cycle ───────────────────────────────────────
 * Detect if a directed graph has a cycle.
 *
 * Parameters:
 *   adj — adjacency matrix: adj[u][v] = 1 means edge u -> v
 *   n   — number of vertices
 *
 * Returns 1 if the graph has a cycle, 0 if it is a DAG.
 *
 * Hint: run topo_sort. If it fails (returns 0), there's a cycle.
 */
int has_cycle(int adj[][MAX_VERTICES], int n)
{
    /* TODO: detect cycle using topo_sort */
    (void)adj; (void)n;
    return 0;
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* Helper: check that u appears before v in result[] of length n */
static int comes_before(int result[], int n, int u, int v)
{
    int pos_u = -1, pos_v = -1;
    for (int i = 0; i < n; i++) {
        if (result[i] == u) pos_u = i;
        if (result[i] == v) pos_v = i;
    }
    return (pos_u >= 0 && pos_v >= 0 && pos_u < pos_v);
}

int main(void)
{
    printf("=== Topological Sort (Kahn's Algorithm) ===\n\n");

    /* ── Test 1: linear chain  A -> B -> C  (0 -> 1 -> 2) ──── */
    printf("linear chain (0 -> 1 -> 2):\n");
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;  /* 0 -> 1 */
        adj[1][2] = 1;  /* 1 -> 2 */

        int result[MAX_VERTICES];
        int ok = topo_sort(adj, 3, result);

        TEST("linear chain — topo_sort succeeds")
        EXPECT(ok == 1);

        TEST("linear chain — order is 0, 1, 2")
        EXPECT(result[0] == 0 && result[1] == 1 && result[2] == 2);
    }

    /* ── Test 2: diamond  0->1, 0->2, 1->3, 2->3 ──────────── */
    printf("\ndiamond (0->1, 0->2, 1->3, 2->3):\n");
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;
        adj[0][2] = 1;
        adj[1][3] = 1;
        adj[2][3] = 1;

        int result[MAX_VERTICES];
        int ok = topo_sort(adj, 4, result);

        TEST("diamond — topo_sort succeeds")
        EXPECT(ok == 1);

        TEST("diamond — 0 comes before 1 and 2")
        EXPECT(comes_before(result, 4, 0, 1) &&
               comes_before(result, 4, 0, 2));

        TEST("diamond — 1 and 2 both come before 3")
        EXPECT(comes_before(result, 4, 1, 3) &&
               comes_before(result, 4, 2, 3));
    }

    /* ── Test 3: cycle  0->1, 1->2, 2->0 ──────────────────── */
    printf("\ncycle (0->1->2->0):\n");
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;
        adj[1][2] = 1;
        adj[2][0] = 1;  /* creates cycle */

        int result[MAX_VERTICES];

        TEST("cycle — topo_sort returns 0 (failure)")
        EXPECT(topo_sort(adj, 3, result) == 0);

        TEST("cycle — has_cycle returns 1")
        EXPECT(has_cycle(adj, 3) == 1);
    }

    /* ── Test 4: no cycle (DAG) ────────────────────────────── */
    printf("\nDAG (no cycle):\n");
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;
        adj[0][2] = 1;
        adj[1][3] = 1;

        TEST("DAG — has_cycle returns 0")
        EXPECT(has_cycle(adj, 4) == 0);
    }

    /* ── Test 5: disconnected graph  0->1, 2->3 (no connection) ── */
    printf("\ndisconnected DAG (0->1, 2->3):\n");
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;
        adj[2][3] = 1;

        int result[MAX_VERTICES];
        int ok = topo_sort(adj, 4, result);

        TEST("disconnected — topo_sort succeeds")
        EXPECT(ok == 1);

        TEST("disconnected — 0 before 1, 2 before 3")
        EXPECT(comes_before(result, 4, 0, 1) &&
               comes_before(result, 4, 2, 3));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
