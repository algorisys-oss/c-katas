/*
 * Kata: Advanced Graph Algorithms — Dijkstra, Bellman-Ford, Kruskal, Cycles
 *
 * Now that you can traverse graphs with BFS/DFS, it's time for the heavy
 * hitters: weighted shortest paths, minimum spanning trees, and cycle
 * detection.
 *
 * Functions to implement:
 *   - dijkstra           (shortest path with non-negative weights)
 *   - bellman_ford       (shortest path with negative weights + cycle detect)
 *   - kruskal_mst        (minimum spanning tree)
 *   - detect_cycle_directed   (three-color DFS)
 *   - detect_cycle_undirected (Union-Find)
 *
 * 20 tests total.
 *
 * Hints:
 *   - Dijkstra: greedy — always process the closest unvisited vertex
 *   - Bellman-Ford: relax ALL edges V-1 times, then check for negative cycles
 *   - Kruskal: sort edges by weight, add if no cycle (use Union-Find)
 *   - Directed cycle: DFS with WHITE/GRAY/BLACK coloring
 *   - Undirected cycle: Union-Find — if both endpoints already connected, cycle!
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ── Weighted Graph (Adjacency Matrix) ───────────────────────────────
 *
 * For Dijkstra and other weighted algorithms, an adjacency matrix is
 * simpler to work with. adj[i][j] = weight of edge i→j, or 0 if no edge.
 *
 *       0   1   2   3   4
 *   0 [ 0   4   2   0   0 ]    0──4──1──1──4
 *   1 [ 4   0   0   8   1 ]    |           |
 *   2 [ 2   0   0   7   0 ]    2           6
 *   3 [ 0   8   7   0   5 ]    |           |
 *   4 [ 0   1   0   5   0 ]    2──7──3──5──/
 */
#define MAX_VERTICES 64
#define INF 999999

typedef struct {
    int adj[MAX_VERTICES][MAX_VERTICES];  /* 0 = no edge */
    int n;                                /* number of vertices */
} WeightedGraph;

/* ── Edge list (for Bellman-Ford and Kruskal) ────────────────────────
 *
 * Some algorithms work better with a flat list of edges rather than
 * a matrix. Each edge is just (source, destination, weight).
 */
typedef struct {
    int src, dest, weight;
} Edge;

#define MAX_EDGES 256

/* ══════════════════════════════════════════════════════════════════════
 *                    EXERCISE 1: DIJKSTRA'S ALGORITHM
 * ══════════════════════════════════════════════════════════════════════
 *
 * BFS finds the shortest path by edge count. But what if edges have
 * different weights? Consider a road map — you want the shortest
 * DISTANCE, not the fewest roads.
 *
 * BFS would say: 0→1 (1 edge, weight 4) is "shorter" than 0→2→3→1
 * (3 edges). But if 0→2→3→1 has total weight 3, that's actually better!
 *
 * Dijkstra's insight:
 *   1. Start with dist[source] = 0, all others = INF
 *   2. Pick the CLOSEST unvisited vertex (greedy choice)
 *   3. "Relax" all its neighbors: if going through this vertex is
 *      shorter than what we knew, update the distance
 *   4. Mark it visited — its distance is now final
 *   5. Repeat until all vertices are visited
 *
 * Think of it like a wavefront expanding outward, but moving faster
 * along lighter edges:
 *
 *   Step 0: dist = [0, INF, INF, INF, INF]    visit 0
 *   Step 1: relax 0's neighbors → [0, 4, 2, INF, INF]    visit 2 (closest)
 *   Step 2: relax 2's neighbors → [0, 4, 2, 9, INF]      visit 1 (closest)
 *   Step 3: relax 1's neighbors → [0, 4, 2, 9, 5]        visit 4 (closest)
 *   Step 4: relax 4's neighbors → [0, 4, 2, 9, 5]        visit 3
 *
 * LIMITATION: Dijkstra FAILS with negative weights. Why? Because the
 * greedy assumption ("once visited, distance is final") breaks when a
 * later, longer path could become shorter by using a negative edge.
 *
 * Parameters:
 *   g      — weighted graph (adjacency matrix, 0 = no edge)
 *   source — starting vertex
 *   dist   — output array: dist[v] = shortest distance from source to v
 */
void dijkstra(WeightedGraph *g, int source, int dist[])
{
    /* TODO: implement Dijkstra's algorithm
     *
     * Steps:
     *   1. Initialize dist[] — all INF except dist[source] = 0
     *   2. Create a visited[] array, all zeros
     *   3. Repeat n times:
     *      a. Find the unvisited vertex u with smallest dist[u]
     *      b. Mark u as visited
     *      c. For each neighbor v of u (where adj[u][v] > 0):
     *         if dist[u] + adj[u][v] < dist[v], update dist[v]
     */
}

/* ══════════════════════════════════════════════════════════════════════
 *                   EXERCISE 2: BELLMAN-FORD ALGORITHM
 * ══════════════════════════════════════════════════════════════════════
 *
 * Dijkstra can't handle negative edge weights. Bellman-Ford can.
 *
 * The idea is simpler (but slower):
 *   - Relax EVERY edge, V-1 times
 *   - Why V-1? The longest shortest path has at most V-1 edges.
 *     Each round of relaxation "propagates" shortest paths one
 *     edge further. After V-1 rounds, all paths are found.
 *
 * Negative cycle detection:
 *   After V-1 rounds, try relaxing once more. If ANY edge can still
 *   be relaxed, there's a negative cycle (distances decrease forever).
 *
 * Parameters:
 *   edges     — array of edges
 *   num_edges — number of edges
 *   num_verts — number of vertices
 *   source    — starting vertex
 *   dist      — output array: dist[v] = shortest distance from source
 *
 * Returns: 1 if successful, 0 if negative cycle detected
 */
int bellman_ford(Edge edges[], int num_edges, int num_verts,
                 int source, int dist[])
{
    /* TODO: implement Bellman-Ford algorithm
     *
     * Steps:
     *   1. Initialize dist[] — all INF except dist[source] = 0
     *   2. Repeat (num_verts - 1) times:
     *      For each edge (src, dest, weight):
     *        if dist[src] != INF && dist[src] + weight < dist[dest]:
     *          dist[dest] = dist[src] + weight
     *   3. Check for negative cycles — one more pass:
     *      For each edge: if dist[src] + weight < dist[dest], return 0
     *   4. Return 1 (no negative cycle)
     */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════════
 *                   EXERCISE 3: KRUSKAL'S MST
 * ══════════════════════════════════════════════════════════════════════
 *
 * A Minimum Spanning Tree (MST) connects ALL vertices with the
 * minimum total edge weight, using exactly V-1 edges and no cycles.
 *
 * Kruskal's greedy approach:
 *   1. Sort all edges by weight (lightest first)
 *   2. For each edge: if it connects two different components, take it
 *   3. Stop when you have V-1 edges
 *
 * "Connects two different components" = Union-Find!
 *   - find(u) != find(v) → different components → safe to add
 *   - find(u) == find(v) → same component → would create cycle, skip
 *
 * Example:
 *   Sorted edges: (0,2,2) (1,4,1) (0,1,4) (3,4,5) (2,3,7) (1,3,8)
 *   Take (1,4,1): components {0}{1,4}{2}{3}
 *   Take (0,2,2): components {0,2}{1,4}{3}
 *   Take (0,1,4): components {0,1,2,4}{3}
 *   Take (3,4,5): components {0,1,2,3,4} — done! V-1=4 edges
 *   Total weight: 1+2+4+5 = 12
 *
 * Parameters:
 *   edges     — array of edges (will be sorted in place)
 *   num_edges — number of edges
 *   num_verts — number of vertices
 *   mst       — output: edges selected for the MST
 *   mst_size  — output: number of edges in MST (should be num_verts-1)
 *
 * Returns: total weight of MST
 *
 * Hint: you'll need a Union-Find. Implement find() and union_sets()
 * as helper functions using a parent[] array.
 */

/* Union-Find helpers for Kruskal's — implement these first */

/* find: return root of v's set. Use path compression:
 * while parent[v] != v, set parent[v] = parent[parent[v]], then v = parent[v] */
static int find(int parent[], int v)
{
    /* TODO: implement find with path compression */
    return v; /* placeholder */
}

/* union_sets: merge the sets containing u and v.
 * Use union by rank to keep trees shallow. */
static void union_sets(int parent[], int rank[], int u, int v)
{
    /* TODO: find roots of u and v, merge by rank */
}

/* Comparison function for qsort — sort edges by weight ascending */
static int edge_cmp(const void *a, const void *b)
{
    /* TODO: compare two Edge structs by weight */
    return 0; /* placeholder */
}

int kruskal_mst(Edge edges[], int num_edges, int num_verts,
                Edge mst[], int *mst_size)
{
    /* TODO: implement Kruskal's algorithm
     *
     * Steps:
     *   1. Sort edges by weight using qsort and edge_cmp
     *   2. Initialize Union-Find: parent[i] = i, rank[i] = 0
     *   3. For each edge (lightest first):
     *      if find(src) != find(dest):
     *        add edge to mst[], union the sets
     *   4. Return total weight
     */
    *mst_size = 0;
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════════
 *               EXERCISE 4: CYCLE DETECTION (DIRECTED)
 * ══════════════════════════════════════════════════════════════════════
 *
 * In a directed graph, a cycle means you can follow directed edges and
 * return to where you started: A→B→C→A.
 *
 * The classic approach uses DFS with three colors:
 *
 *   WHITE (0) = not yet visited
 *   GRAY  (1) = currently being explored (on the recursion stack)
 *   BLACK (2) = completely finished (all descendants explored)
 *
 * During DFS, if you encounter a GRAY vertex, you've found a back edge
 * — a cycle! A GRAY vertex means "I'm currently exploring this path,
 * and I've looped back to it."
 *
 *   A ──→ B ──→ C       Visit A (gray), visit B (gray), visit C (gray)
 *   ↑           │       C→A: A is GRAY = back edge = CYCLE!
 *   └───────────┘
 *
 * Parameters:
 *   adj   — adjacency matrix (adj[i][j] = 1 if edge i→j exists)
 *   n     — number of vertices
 *
 * Returns: 1 if cycle exists, 0 otherwise
 */
#define WHITE 0
#define GRAY  1
#define BLACK 2

/* DFS helper for cycle detection — returns 1 if cycle found */
static int dfs_cycle(int adj[MAX_VERTICES][MAX_VERTICES], int n,
                     int v, int color[])
{
    /* TODO: implement three-color DFS
     *
     * 1. Color v GRAY
     * 2. For each neighbor u of v (where adj[v][u] == 1):
     *    - If color[u] == GRAY → cycle found, return 1
     *    - If color[u] == WHITE → recurse; if it returns 1, return 1
     * 3. Color v BLACK (done exploring)
     * 4. Return 0 (no cycle through this path)
     */
    return 0; /* placeholder */
}

int detect_cycle_directed(int adj[MAX_VERTICES][MAX_VERTICES], int n)
{
    /* TODO: initialize all colors to WHITE, then run DFS from each
     * unvisited vertex. Return 1 if any DFS finds a cycle. */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════════
 *              EXERCISE 5: CYCLE DETECTION (UNDIRECTED)
 * ══════════════════════════════════════════════════════════════════════
 *
 * For undirected graphs, cycle detection is simpler with Union-Find:
 *
 * For each edge (u, v):
 *   - If u and v are already in the same set → adding this edge
 *     creates a cycle!
 *   - Otherwise, union them
 *
 * Why does this work? Union-Find tracks connected components. If two
 * vertices are already connected (same component) and there's ANOTHER
 * edge between them, that edge creates an alternate path = a cycle.
 *
 * Why not use three-color DFS for undirected graphs? Because in an
 * undirected graph, the edge u-v means both u→v and v→u. When DFS
 * visits v from u and then sees u as v's neighbor, that's NOT a back
 * edge — it's just the edge we came from. You'd need to track parents
 * to distinguish real back edges from the parent edge.
 *
 * Parameters:
 *   edges     — edge list (undirected: each edge listed once)
 *   num_edges — number of edges
 *   num_verts — number of vertices
 *
 * Returns: 1 if cycle exists, 0 otherwise
 */
int detect_cycle_undirected(Edge edges[], int num_edges, int num_verts)
{
    /* TODO: implement using Union-Find
     *
     * 1. Initialize parent[i] = i for all vertices
     * 2. For each edge:
     *    - root_src = find(parent, edge.src)
     *    - root_dest = find(parent, edge.dest)
     *    - If root_src == root_dest → return 1 (cycle!)
     *    - Otherwise, union_sets(parent, rank, edge.src, edge.dest)
     * 3. Return 0 (no cycle)
     *
     * Reuse the find() and union_sets() helpers from Kruskal.
     */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════════ */

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
    printf("=== Advanced Graph Algorithms ===\n\n");

    /* ── Dijkstra tests ─────────────────────────────────────────────
     *
     *    0 ──4── 1 ──1── 4
     *    │       │       │
     *    2       8       6
     *    │       │       │
     *    2 ──7── 3 ──5──/
     *
     * Shortest from 0: [0, 4, 2, 9, 5]
     */
    printf("Dijkstra's algorithm:\n");
    {
        WeightedGraph g;
        memset(&g, 0, sizeof(g));
        g.n = 5;
        /* undirected edges — set both directions */
        g.adj[0][1] = 4; g.adj[1][0] = 4;
        g.adj[0][2] = 2; g.adj[2][0] = 2;
        g.adj[1][3] = 8; g.adj[3][1] = 8;
        g.adj[1][4] = 1; g.adj[4][1] = 1;
        g.adj[2][3] = 7; g.adj[3][2] = 7;
        g.adj[3][4] = 5; g.adj[4][3] = 5;

        int dist[MAX_VERTICES];
        dijkstra(&g, 0, dist);

        TEST("dist to self is 0")
            EXPECT(dist[0] == 0);

        TEST("dist 0→1 is 4 (direct)")
            EXPECT(dist[1] == 4);

        TEST("dist 0→2 is 2 (direct)")
            EXPECT(dist[2] == 2);

        TEST("dist 0→4 is 5 (0→1→4)")
            EXPECT(dist[4] == 5);

        TEST("dist 0→3 is 9 (0→2→3)")
            EXPECT(dist[3] == 9);
    }

    /* ── Bellman-Ford tests ─────────────────────────────────────────
     *
     * Same graph as Dijkstra first, then test negative weights.
     */
    printf("\nBellman-Ford algorithm:\n");
    {
        /* Same graph: 0──4──1──1──4, 0──2──2──7──3──5──4 */
        Edge edges[] = {
            {0, 1, 4}, {1, 0, 4},
            {0, 2, 2}, {2, 0, 2},
            {1, 3, 8}, {3, 1, 8},
            {1, 4, 1}, {4, 1, 1},
            {2, 3, 7}, {3, 2, 7},
            {3, 4, 5}, {4, 3, 5}
        };
        int dist[MAX_VERTICES];
        int ok = bellman_ford(edges, 12, 5, 0, dist);

        TEST("no negative cycle in normal graph")
            EXPECT(ok == 1);

        TEST("matches Dijkstra: dist 0→4 is 5")
            EXPECT(dist[4] == 5);
    }
    {
        /* Graph with negative weight (but no negative cycle):
         * 0 ──5──→ 1 ──(-3)──→ 2
         * Shortest 0→2: 5 + (-3) = 2 */
        Edge edges[] = {
            {0, 1, 5},
            {1, 2, -3}
        };
        int dist[MAX_VERTICES];
        int ok = bellman_ford(edges, 2, 3, 0, dist);

        TEST("handles negative weight: 0→2 = 2")
            EXPECT(ok == 1 && dist[2] == 2);
    }
    {
        /* Negative cycle: 0→1 (1), 1→2 (-3), 2→0 (1)
         * Total cycle weight: 1 + (-3) + 1 = -1 → infinite improvement */
        Edge edges[] = {
            {0, 1, 1},
            {1, 2, -3},
            {2, 0, 1}
        };
        int dist[MAX_VERTICES];
        int ok = bellman_ford(edges, 3, 3, 0, dist);

        TEST("detects negative cycle")
            EXPECT(ok == 0);
    }

    /* ── Kruskal's MST tests ────────────────────────────────────────
     *
     * Graph:            MST (weight = 12):
     *  0──4──1──1──4      0──4──1──1──4
     *  │           │      │           │
     *  2           │      2           │
     *  │           │      │           │
     *  2     3──5──/      2     3──5──/
     *
     *  Edges sorted: (1,4,1)(0,2,2)(0,1,4)(3,4,5)(2,3,7)(1,3,8)
     *  MST: (1,4,1)+(0,2,2)+(0,1,4)+(3,4,5) = 12
     */
    printf("\nKruskal's MST:\n");
    {
        Edge edges[] = {
            {0, 1, 4}, {0, 2, 2}, {1, 3, 8},
            {1, 4, 1}, {2, 3, 7}, {3, 4, 5}
        };
        Edge mst[MAX_EDGES];
        int mst_size = 0;
        int total = kruskal_mst(edges, 6, 5, mst, &mst_size);

        TEST("MST has V-1 = 4 edges")
            EXPECT(mst_size == 4);

        TEST("MST total weight is 12")
            EXPECT(total == 12);

        TEST("MST lightest edge is weight 1")
            EXPECT(mst[0].weight == 1);
    }
    {
        /* Triangle: 0──1──1, 1──2──2, 0──2──3
         * MST: edges (0,1,1) and (1,2,2), total = 3 */
        Edge edges[] = {
            {0, 1, 1}, {1, 2, 2}, {0, 2, 3}
        };
        Edge mst[MAX_EDGES];
        int mst_size = 0;
        int total = kruskal_mst(edges, 3, 3, mst, &mst_size);

        TEST("triangle MST weight is 3")
            EXPECT(total == 3 && mst_size == 2);
    }

    /* ── Directed cycle detection tests ─────────────────────────────
     *
     *  Cycle: 0→1→2→0        No cycle: 0→1→2
     */
    printf("\ncycle detection (directed):\n");
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1; adj[1][2] = 1; adj[2][0] = 1;

        TEST("detects cycle: 0→1→2→0")
            EXPECT(detect_cycle_directed(adj, 3) == 1);
    }
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1; adj[1][2] = 1;

        TEST("no cycle in DAG: 0→1→2")
            EXPECT(detect_cycle_directed(adj, 3) == 0);
    }
    {
        /* Single vertex, no edges */
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));

        TEST("no cycle in single vertex")
            EXPECT(detect_cycle_directed(adj, 1) == 0);
    }
    {
        /* Disconnected: 0→1, 2→3→2 (cycle in second component) */
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;
        adj[2][3] = 1; adj[3][2] = 1;

        TEST("detects cycle in disconnected component")
            EXPECT(detect_cycle_directed(adj, 4) == 1);
    }

    /* ── Undirected cycle detection tests ───────────────────────────
     *
     *  Cycle: 0-1-2-0 (triangle)
     *  No cycle: 0-1-2 (path)
     */
    printf("\ncycle detection (undirected):\n");
    {
        Edge edges[] = { {0, 1, 1}, {1, 2, 1}, {2, 0, 1} };

        TEST("detects cycle in triangle")
            EXPECT(detect_cycle_undirected(edges, 3, 3) == 1);
    }
    {
        Edge edges[] = { {0, 1, 1}, {1, 2, 1} };

        TEST("no cycle in path 0-1-2")
            EXPECT(detect_cycle_undirected(edges, 2, 3) == 0);
    }
    {
        /* 4 vertices, tree: 0-1, 1-2, 1-3 → no cycle */
        Edge edges[] = { {0, 1, 1}, {1, 2, 1}, {1, 3, 1} };

        TEST("no cycle in tree")
            EXPECT(detect_cycle_undirected(edges, 3, 4) == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
