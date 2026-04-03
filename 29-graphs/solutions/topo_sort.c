/*
 * Solution: Topological Sort (Kahn's Algorithm) & Cycle Detection
 *
 * Topological sort using BFS with in-degree tracking, plus cycle
 * detection for directed graphs.
 *
 * 10 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_VERTICES 32

/* ── topo_sort ───────────────────────────────────────────────────── */
int topo_sort(int adj[][MAX_VERTICES], int n, int result[])
{
    int in_degree[MAX_VERTICES];
    memset(in_degree, 0, sizeof(in_degree));

    /* Compute in-degrees */
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (adj[u][v]) {
                in_degree[v]++;
            }
        }
    }

    /* Enqueue all vertices with in-degree 0 */
    int queue[MAX_VERTICES];
    int q_front = 0, q_back = 0;
    for (int v = 0; v < n; v++) {
        if (in_degree[v] == 0) {
            queue[q_back++] = v;
        }
    }

    int count = 0;
    while (q_front < q_back) {
        int u = queue[q_front++];
        result[count++] = u;

        for (int v = 0; v < n; v++) {
            if (adj[u][v]) {
                in_degree[v]--;
                if (in_degree[v] == 0) {
                    queue[q_back++] = v;
                }
            }
        }
    }

    return (count == n) ? 1 : 0;
}

/* ── has_cycle ───────────────────────────────────────────────────── */
int has_cycle(int adj[][MAX_VERTICES], int n)
{
    int result[MAX_VERTICES];
    return topo_sort(adj, n, result) == 0 ? 1 : 0;
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
        adj[0][1] = 1;
        adj[1][2] = 1;

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
        adj[2][0] = 1;

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
