/*
 * Solution: Advanced Graph Algorithms — Dijkstra, Bellman-Ford, Kruskal, Cycles
 *
 * Weighted shortest paths, minimum spanning trees, and cycle detection.
 *
 * 20 tests total.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VERTICES 64
#define INF 999999

typedef struct {
    int adj[MAX_VERTICES][MAX_VERTICES];  /* 0 = no edge */
    int n;                                /* number of vertices */
} WeightedGraph;

typedef struct {
    int src, dest, weight;
} Edge;

#define MAX_EDGES 256

/* ── dijkstra ────────────────────────────────────────────────────── */
void dijkstra(WeightedGraph *g, int source, int dist[])
{
    int visited[MAX_VERTICES];
    memset(visited, 0, sizeof(visited));

    /* all distances start at infinity except the source */
    for (int i = 0; i < g->n; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    for (int iter = 0; iter < g->n; iter++) {
        /* find the closest unvisited vertex */
        int u = -1;
        for (int v = 0; v < g->n; v++) {
            if (!visited[v] && (u == -1 || dist[v] < dist[u])) {
                u = v;
            }
        }

        if (u == -1 || dist[u] == INF) {
            break; /* remaining vertices are unreachable */
        }

        visited[u] = 1;

        /* relax all neighbors of u */
        for (int v = 0; v < g->n; v++) {
            if (g->adj[u][v] > 0 && dist[u] + g->adj[u][v] < dist[v]) {
                dist[v] = dist[u] + g->adj[u][v];
            }
        }
    }
}

/* ── bellman_ford ────────────────────────────────────────────────── */
int bellman_ford(Edge edges[], int num_edges, int num_verts,
                 int source, int dist[])
{
    for (int i = 0; i < num_verts; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    /* relax all edges V-1 times */
    for (int iter = 0; iter < num_verts - 1; iter++) {
        for (int e = 0; e < num_edges; e++) {
            int u = edges[e].src;
            int v = edges[e].dest;
            int w = edges[e].weight;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    /* check for negative cycles — one more relaxation pass */
    for (int e = 0; e < num_edges; e++) {
        int u = edges[e].src;
        int v = edges[e].dest;
        int w = edges[e].weight;
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            return 0; /* negative cycle detected */
        }
    }

    return 1;
}

/* ── Union-Find helpers for Kruskal ──────────────────────────────── */

static int find(int parent[], int v)
{
    /* path compression: point each node directly at its root */
    while (parent[v] != v) {
        parent[v] = parent[parent[v]]; /* path halving */
        v = parent[v];
    }
    return v;
}

static void union_sets(int parent[], int rank[], int u, int v)
{
    int ru = find(parent, u);
    int rv = find(parent, v);
    if (ru == rv) return;

    /* union by rank: attach shorter tree under taller */
    if (rank[ru] < rank[rv]) {
        parent[ru] = rv;
    } else if (rank[ru] > rank[rv]) {
        parent[rv] = ru;
    } else {
        parent[rv] = ru;
        rank[ru]++;
    }
}

static int edge_cmp(const void *a, const void *b)
{
    return ((const Edge *)a)->weight - ((const Edge *)b)->weight;
}

/* ── kruskal_mst ─────────────────────────────────────────────────── */
int kruskal_mst(Edge edges[], int num_edges, int num_verts,
                Edge mst[], int *mst_size)
{
    qsort(edges, (size_t)num_edges, sizeof(Edge), edge_cmp);

    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
    for (int i = 0; i < num_verts; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    *mst_size = 0;
    int total_weight = 0;

    for (int e = 0; e < num_edges && *mst_size < num_verts - 1; e++) {
        int ru = find(parent, edges[e].src);
        int rv = find(parent, edges[e].dest);

        if (ru != rv) {
            mst[*mst_size] = edges[e];
            (*mst_size)++;
            total_weight += edges[e].weight;
            union_sets(parent, rank, edges[e].src, edges[e].dest);
        }
    }

    return total_weight;
}

/* ── detect_cycle_directed (three-color DFS) ─────────────────────── */
#define WHITE 0
#define GRAY  1
#define BLACK 2

static int dfs_cycle(int adj[MAX_VERTICES][MAX_VERTICES], int n,
                     int v, int color[])
{
    color[v] = GRAY;

    for (int u = 0; u < n; u++) {
        if (adj[v][u]) {
            if (color[u] == GRAY) {
                return 1; /* back edge to vertex on current path = cycle */
            }
            if (color[u] == WHITE && dfs_cycle(adj, n, u, color)) {
                return 1;
            }
        }
    }

    color[v] = BLACK;
    return 0;
}

int detect_cycle_directed(int adj[MAX_VERTICES][MAX_VERTICES], int n)
{
    int color[MAX_VERTICES];
    memset(color, 0, sizeof(color)); /* all WHITE */

    /* must try every vertex — graph may be disconnected */
    for (int v = 0; v < n; v++) {
        if (color[v] == WHITE) {
            if (dfs_cycle(adj, n, v, color)) {
                return 1;
            }
        }
    }
    return 0;
}

/* ── detect_cycle_undirected (Union-Find) ────────────────────────── */
int detect_cycle_undirected(Edge edges[], int num_edges, int num_verts)
{
    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
    for (int i = 0; i < num_verts; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    for (int e = 0; e < num_edges; e++) {
        int ru = find(parent, edges[e].src);
        int rv = find(parent, edges[e].dest);

        if (ru == rv) {
            return 1; /* already connected — adding this edge creates a cycle */
        }
        union_sets(parent, rank, edges[e].src, edges[e].dest);
    }

    return 0;
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

    /* ── Dijkstra tests ───────────────────────────────────────────── */
    printf("Dijkstra's algorithm:\n");
    {
        WeightedGraph g;
        memset(&g, 0, sizeof(g));
        g.n = 5;
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

    /* ── Bellman-Ford tests ───────────────────────────────────────── */
    printf("\nBellman-Ford algorithm:\n");
    {
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

    /* ── Kruskal's MST tests ─────────────────────────────────────── */
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
        Edge edges[] = {
            {0, 1, 1}, {1, 2, 2}, {0, 2, 3}
        };
        Edge mst[MAX_EDGES];
        int mst_size = 0;
        int total = kruskal_mst(edges, 3, 3, mst, &mst_size);

        TEST("triangle MST weight is 3")
            EXPECT(total == 3 && mst_size == 2);
    }

    /* ── Directed cycle detection tests ───────────────────────────── */
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
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));

        TEST("no cycle in single vertex")
            EXPECT(detect_cycle_directed(adj, 1) == 0);
    }
    {
        int adj[MAX_VERTICES][MAX_VERTICES];
        memset(adj, 0, sizeof(adj));
        adj[0][1] = 1;
        adj[2][3] = 1; adj[3][2] = 1;

        TEST("detects cycle in disconnected component")
            EXPECT(detect_cycle_directed(adj, 4) == 1);
    }

    /* ── Undirected cycle detection tests ─────────────────────────── */
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
        Edge edges[] = { {0, 1, 1}, {1, 2, 1}, {1, 3, 1} };

        TEST("no cycle in tree")
            EXPECT(detect_cycle_undirected(edges, 3, 4) == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
