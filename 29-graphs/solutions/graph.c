/*
 * Solution: Graphs — Adjacency List, BFS, DFS, Shortest Path
 *
 * A graph implemented with adjacency lists (fixed-size arrays),
 * BFS and DFS traversals, and BFS-based shortest path.
 *
 * 15 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_VERTICES 32
#define MAX_EDGES    64   /* max neighbors per vertex */

/* ── Graph Data Structure ─────────────────────────────────────────
 *
 *  adj[v][i]    = the i-th neighbor of vertex v
 *  weight[v][i] = weight of the edge from v to adj[v][i]
 *  deg[v]       = number of neighbors of vertex v
 */
typedef struct {
    int adj[MAX_VERTICES][MAX_EDGES];
    int weight[MAX_VERTICES][MAX_EDGES];
    int deg[MAX_VERTICES];
    int num_vertices;
} Graph;

/* ── graph_init ───────────────────────────────────────────────── */
void graph_init(Graph *g, int num_vertices)
{
    g->num_vertices = num_vertices;
    memset(g->deg, 0, sizeof(g->deg));
}

/* ── graph_add_edge ───────────────────────────────────────────── */
void graph_add_edge(Graph *g, int from, int to, int w)
{
    int d = g->deg[from];
    g->adj[from][d] = to;
    g->weight[from][d] = w;
    g->deg[from]++;
}

/* ── graph_bfs ────────────────────────────────────────────────── */
void graph_bfs(Graph *g, int start, int visited[], int order[], int *count)
{
    memset(visited, 0, sizeof(int) * (unsigned)g->num_vertices);
    *count = 0;

    int queue[MAX_VERTICES];
    int q_front = 0, q_back = 0;

    visited[start] = 1;
    queue[q_back] = start;
    q_back = (q_back + 1) % MAX_VERTICES;

    while (q_front != q_back) {
        int v = queue[q_front];
        q_front = (q_front + 1) % MAX_VERTICES;

        order[*count] = v;
        (*count)++;

        for (int i = 0; i < g->deg[v]; i++) {
            int u = g->adj[v][i];
            if (!visited[u]) {
                visited[u] = 1;
                queue[q_back] = u;
                q_back = (q_back + 1) % MAX_VERTICES;
            }
        }
    }
}

/* ── graph_dfs (recursive helper) ─────────────────────────────── */
static void dfs_visit(Graph *g, int v, int visited[], int order[], int *count)
{
    visited[v] = 1;
    order[*count] = v;
    (*count)++;

    for (int i = 0; i < g->deg[v]; i++) {
        int u = g->adj[v][i];
        if (!visited[u]) {
            dfs_visit(g, u, visited, order, count);
        }
    }
}

/* ── graph_dfs (entry point) ──────────────────────────────────── */
void graph_dfs(Graph *g, int start, int visited[], int order[], int *count)
{
    memset(visited, 0, sizeof(int) * (unsigned)g->num_vertices);
    *count = 0;
    dfs_visit(g, start, visited, order, count);
}

/* ── graph_shortest_path ──────────────────────────────────────── */
int graph_shortest_path(Graph *g, int start, int end)
{
    int dist[MAX_VERTICES];
    memset(dist, -1, sizeof(dist));
    dist[start] = 0;

    if (start == end) {
        return 0;
    }

    int queue[MAX_VERTICES];
    int q_front = 0, q_back = 0;

    queue[q_back] = start;
    q_back = (q_back + 1) % MAX_VERTICES;

    while (q_front != q_back) {
        int v = queue[q_front];
        q_front = (q_front + 1) % MAX_VERTICES;

        for (int i = 0; i < g->deg[v]; i++) {
            int u = g->adj[v][i];
            if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                if (u == end) {
                    return dist[u];
                }
                queue[q_back] = u;
                q_back = (q_back + 1) % MAX_VERTICES;
            }
        }
    }

    return -1;
}

/* ── graph_has_path ───────────────────────────────────────────── */
int graph_has_path(Graph *g, int start, int end)
{
    int visited[MAX_VERTICES];
    int order[MAX_VERTICES];
    int count = 0;
    graph_bfs(g, start, visited, order, &count);
    return visited[end];
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
    printf("=== Graphs: Adjacency List, BFS, DFS ===\n\n");

    /* ── Build a test graph ──────────────────────────────────────
     *
     *    0 --- 1 --- 4
     *    |     |
     *    2     3
     *
     * Represented as directed edges in both directions (undirected):
     */
    Graph g;
    graph_init(&g, 5);
    /* 0 <-> 1 */
    graph_add_edge(&g, 0, 1, 1);
    graph_add_edge(&g, 1, 0, 1);
    /* 0 <-> 2 */
    graph_add_edge(&g, 0, 2, 1);
    graph_add_edge(&g, 2, 0, 1);
    /* 1 <-> 3 */
    graph_add_edge(&g, 1, 3, 1);
    graph_add_edge(&g, 3, 1, 1);
    /* 1 <-> 4 */
    graph_add_edge(&g, 1, 4, 1);
    graph_add_edge(&g, 4, 1, 1);

    /* --- Add edge tests --- */
    printf("add edges:\n");

    TEST("vertex 0 has degree 2")
        EXPECT(g.deg[0] == 2);

    TEST("vertex 1 has degree 3")
        EXPECT(g.deg[1] == 3);

    TEST("vertex 0 neighbors are 1 and 2")
        EXPECT(g.adj[0][0] == 1 && g.adj[0][1] == 2);

    /* --- BFS tests --- */
    printf("\nBFS traversal:\n");
    {
        int visited[MAX_VERTICES];
        int order[MAX_VERTICES];
        int count = 0;
        graph_bfs(&g, 0, visited, order, &count);

        TEST("BFS from 0 visits all 5 vertices")
            EXPECT(count == 5);

        TEST("BFS first vertex is start (0)")
            EXPECT(order[0] == 0);

        /* BFS visits level by level: 0, then {1,2}, then {3,4} */
        TEST("BFS visits 1 and 2 before 3 and 4")
            EXPECT((order[1] == 1 || order[1] == 2) &&
                   (order[2] == 1 || order[2] == 2) &&
                   order[1] != order[2]);
    }

    /* --- DFS tests --- */
    printf("\nDFS traversal:\n");
    {
        int visited[MAX_VERTICES];
        int order[MAX_VERTICES];
        int count = 0;
        graph_dfs(&g, 0, visited, order, &count);

        TEST("DFS from 0 visits all 5 vertices")
            EXPECT(count == 5);

        TEST("DFS first vertex is start (0)")
            EXPECT(order[0] == 0);

        /* DFS goes deep: 0 -> 1 -> ... before backtracking to 2 */
        TEST("DFS second vertex is 1 (goes deep first)")
            EXPECT(order[1] == 1);
    }

    /* --- Shortest path tests --- */
    printf("\nshortest path (BFS):\n");

    TEST("shortest path 0 -> 0 is 0")
        EXPECT(graph_shortest_path(&g, 0, 0) == 0);

    TEST("shortest path 0 -> 1 is 1")
        EXPECT(graph_shortest_path(&g, 0, 1) == 1);

    TEST("shortest path 0 -> 4 is 2")
        EXPECT(graph_shortest_path(&g, 0, 4) == 2);

    /* --- Has path tests --- */
    printf("\nhas path:\n");

    TEST("path exists 0 -> 4")
        EXPECT(graph_has_path(&g, 0, 4) == 1);

    /* --- Disconnected graph test --- */
    printf("\ndisconnected graph:\n");
    {
        Graph g2;
        graph_init(&g2, 4);
        /* 0 <-> 1, but 2 and 3 are isolated */
        graph_add_edge(&g2, 0, 1, 1);
        graph_add_edge(&g2, 1, 0, 1);

        TEST("no path in disconnected graph (0 -> 2)")
            EXPECT(graph_has_path(&g2, 0, 2) == 0);

        TEST("shortest path returns -1 when no path (0 -> 3)")
            EXPECT(graph_shortest_path(&g2, 0, 3) == -1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
