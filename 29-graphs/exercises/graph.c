/*
 * Kata: Graphs — Adjacency List, BFS, DFS, Shortest Path
 *
 * Implement a graph using an adjacency list with fixed-size arrays,
 * then write BFS and DFS traversals plus shortest-path queries.
 *
 * Functions to implement:
 *   - graph_init         (initialize an empty graph)
 *   - graph_add_edge     (add a directed weighted edge)
 *   - graph_bfs          (breadth-first traversal)
 *   - graph_dfs          (depth-first traversal)
 *   - graph_shortest_path (BFS shortest path in unweighted graph)
 *   - graph_has_path     (check if a path exists between two vertices)
 *
 * 15 tests total.
 *
 * Hints:
 *   - For BFS, use a circular queue with an array of MAX_VERTICES
 *   - For DFS, use recursion (the call stack is your stack)
 *   - graph_shortest_path treats all edges as weight 1 (unweighted BFS)
 *   - graph_has_path can reuse BFS or DFS internally
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
 *
 *  Example — vertex 0 has neighbors 1 (weight 5) and 2 (weight 3):
 *
 *    adj[0] = [1, 2, ...]     weight[0] = [5, 3, ...]     deg[0] = 2
 */
typedef struct {
    int adj[MAX_VERTICES][MAX_EDGES];
    int weight[MAX_VERTICES][MAX_EDGES];
    int deg[MAX_VERTICES];
    int num_vertices;
} Graph;

/* ── Exercise 1: graph_init ───────────────────────────────────────
 * Initialize a graph with the given number of vertices and no edges.
 * Set all degrees to 0.
 */
void graph_init(Graph *g, int num_vertices)
{
    /* TODO: set num_vertices, zero out all degrees */
}

/* ── Exercise 2: graph_add_edge ───────────────────────────────────
 * Add a DIRECTED edge from vertex 'from' to vertex 'to' with the
 * given weight. Append 'to' to from's adjacency list.
 *
 * Hint: adj[from][deg[from]] = to;
 *       weight[from][deg[from]] = w;
 *       deg[from]++;
 */
void graph_add_edge(Graph *g, int from, int to, int w)
{
    /* TODO: add 'to' as a neighbor of 'from' with weight w */
}

/* ── Exercise 3: graph_bfs ────────────────────────────────────────
 * Perform BFS starting from 'start'. Fill the 'order' array with
 * the vertices in the order they are visited. Set *count to the
 * number of vertices visited.
 *
 * The 'visited' array is provided (size MAX_VERTICES) — you must
 * initialize it to all zeros, then mark vertices as you visit them.
 *
 * Use a circular queue:
 *   int queue[MAX_VERTICES], q_front = 0, q_back = 0;
 *   enqueue: queue[q_back] = v; q_back = (q_back + 1) % MAX_VERTICES;
 *   dequeue: v = queue[q_front]; q_front = (q_front + 1) % MAX_VERTICES;
 *   empty:   q_front == q_back
 */
void graph_bfs(Graph *g, int start, int visited[], int order[], int *count)
{
    /* TODO: implement BFS */
}

/* ── Exercise 4: graph_dfs (recursive helper) ─────────────────────
 * Visit vertex v: mark it visited, add to order, then recurse on
 * each unvisited neighbor.
 */
static void dfs_visit(Graph *g, int v, int visited[], int order[], int *count)
{
    /* TODO: mark v visited, add to order, recurse on unvisited neighbors */
}

/* ── Exercise 4: graph_dfs (entry point) ──────────────────────────
 * Initialize visited[] to all zeros, then call dfs_visit.
 */
void graph_dfs(Graph *g, int start, int visited[], int order[], int *count)
{
    /* TODO: clear visited, set *count = 0, call dfs_visit */
}

/* ── Exercise 5: graph_shortest_path ──────────────────────────────
 * Find the shortest path (fewest edges) from 'start' to 'end'
 * using BFS. Return the distance, or -1 if no path exists.
 *
 * Treat every edge as weight 1 (unweighted).
 *
 * Hint: use a dist[] array initialized to -1.
 *       dist[start] = 0;
 *       for each neighbor u of v: if dist[u] == -1, dist[u] = dist[v] + 1;
 */
int graph_shortest_path(Graph *g, int start, int end)
{
    /* TODO: BFS with distance tracking */
    return -1; /* placeholder */
}

/* ── Exercise 6: graph_has_path ───────────────────────────────────
 * Return 1 if there is a path from 'start' to 'end', 0 otherwise.
 *
 * Hint: you can reuse BFS or DFS. After traversal, check if 'end'
 * was visited.
 */
int graph_has_path(Graph *g, int start, int end)
{
    /* TODO: use BFS or DFS to check reachability */
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
