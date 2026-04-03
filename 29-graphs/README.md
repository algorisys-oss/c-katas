# Module 20 — Graphs

## What is a Graph?

Every data structure you've built so far had a rigid shape. Arrays are linear.
Trees branch downward. A **graph** is the most general structure of all: it's just
a collection of **things** and **connections** between them — with no rules about
who can connect to whom.

```
    A ---- B         Vertices (nodes): {A, B, C, D, E}
    |    / |         Edges (connections): {A-B, A-C, B-C, B-D, D-E}
    |   /  |
    C      D
            \
             E
```

### Formal Definition

A **graph** G = (V, E) consists of:

| Symbol | Name       | Meaning                                  |
|--------|------------|------------------------------------------|
| V      | Vertices   | The set of nodes (A, B, C, D, E above)   |
| E      | Edges      | The set of connections between vertices   |

That's it. Two sets. Everything else is built on top.

---

## Types of Graphs

### Undirected vs. Directed

In an **undirected** graph, edges go both ways. If A connects to B, then B
connects to A. Think of friendships — if you're my friend, I'm your friend.

```
  Undirected:            Directed:
  A ---- B               A ----> B
  |      |               |       |
  C ---- D               C <---- D
                          (arrows have direction)
```

In a **directed** graph (or **digraph**), edges have direction. A→B doesn't
mean B→A. Think of Twitter follows — I can follow you without you following me.

### Weighted vs. Unweighted

Sometimes edges carry a **weight** (cost, distance, time):

```
  Unweighted:            Weighted:
  A --- B                A --5-- B
  |     |                |       |
  C --- D                3       2
                         |       |
                         C --7-- D
```

Real-world examples of weighted graphs:
- **Road map**: vertices = cities, weights = distances in km
- **Network**: vertices = routers, weights = latency in ms
- **Flight routes**: vertices = airports, weights = ticket prices

### Common Terminology

| Term           | Meaning                                              |
|----------------|------------------------------------------------------|
| **Adjacent**   | Two vertices connected by an edge                    |
| **Degree**     | Number of edges touching a vertex                    |
| **Path**       | A sequence of vertices connected by edges            |
| **Cycle**      | A path that starts and ends at the same vertex       |
| **Connected**  | Every vertex can reach every other vertex             |
| **DAG**        | Directed Acyclic Graph — directed, no cycles         |

---

## Representing Graphs in Code

There are two classic ways to store a graph. Both have tradeoffs.

### Adjacency Matrix

A 2D array where `matrix[i][j] = 1` means there's an edge from vertex i to j.

For this undirected graph:

```
    0 --- 1
    |   / |
    |  /  |
    2     3
```

The adjacency matrix is:

```
        0   1   2   3
      +---+---+---+---+
  0   | 0 | 1 | 1 | 0 |     0 connects to 1 and 2
      +---+---+---+---+
  1   | 1 | 0 | 1 | 1 |     1 connects to 0, 2, and 3
      +---+---+---+---+
  2   | 1 | 1 | 0 | 0 |     2 connects to 0 and 1
      +---+---+---+---+
  3   | 0 | 1 | 0 | 0 |     3 connects to 1
      +---+---+---+---+
```

For undirected graphs, the matrix is **symmetric**: `matrix[i][j] == matrix[j][i]`.

### Adjacency List

For each vertex, store a list of its neighbors:

```
  Vertex 0: [1, 2]
  Vertex 1: [0, 2, 3]
  Vertex 2: [0, 1]
  Vertex 3: [1]
```

### Side-by-Side Comparison

```
  Graph:             Matrix:              Adjacency List:
                        0 1 2 3
  0 --- 1            0 [0 1 1 0]          0 -> [1, 2]
  |   / |            1 [1 0 1 1]          1 -> [0, 2, 3]
  |  /  |            2 [1 1 0 0]          2 -> [0, 1]
  2     3            3 [0 1 0 0]          3 -> [1]
```

### Tradeoffs

| Operation              | Adjacency Matrix | Adjacency List   |
|------------------------|-------------------|------------------|
| Space                  | O(V^2)            | O(V + E)         |
| Check if edge exists   | O(1)              | O(degree)        |
| List all neighbors     | O(V)              | O(degree)        |
| Add an edge            | O(1)              | O(1)             |
| Best for               | Dense graphs      | Sparse graphs    |

Most real-world graphs are **sparse** (far fewer edges than V^2), so adjacency
lists are the standard choice. That's what we'll implement.

### Implementation Strategy

We'll use a simple fixed-size approach — no linked lists needed:

```c
#define MAX_VERTICES 32
#define MAX_EDGES    64   /* max neighbors per vertex */

typedef struct {
    int adj[MAX_VERTICES][MAX_EDGES];     /* adj[v] = list of neighbors */
    int weight[MAX_VERTICES][MAX_EDGES];  /* weight[v][i] = weight of edge */
    int deg[MAX_VERTICES];                /* deg[v] = number of neighbors */
    int num_vertices;
} Graph;
```

For vertex `v`, its neighbors are `adj[v][0]` through `adj[v][deg[v]-1]`.

---

## Breadth-First Search (BFS)

BFS explores a graph **level by level**, like ripples spreading from a stone
dropped in water. It visits all vertices at distance 1, then distance 2, then
distance 3, and so on.

### The Algorithm

BFS uses a **queue** (first-in, first-out):

```
BFS(graph, start):
    create a queue Q
    mark start as visited
    enqueue start into Q

    while Q is not empty:
        v = dequeue from Q
        process v (add to output order)
        for each neighbor u of v:
            if u is not visited:
                mark u as visited
                enqueue u into Q
```

### Step-by-Step Trace

Starting BFS from vertex 0 on this graph:

```
    0 --- 1 --- 4
    |     |
    2     3
```

```
  Step   Queue          Action             Visited         Order
  ────   ─────          ──────             ───────         ─────
  1      [0]            Start              {0}             []
  2      [1, 2]         Dequeue 0          {0,1,2}         [0]
                         → enqueue 1, 2
  3      [2, 3, 4]      Dequeue 1          {0,1,2,3,4}     [0,1]
                         → enqueue 3, 4
  4      [3, 4]         Dequeue 2          {0,1,2,3,4}     [0,1,2]
                         → no new neighbors
  5      [4]            Dequeue 3          {0,1,2,3,4}     [0,1,2,3]
                         → no new neighbors
  6      []             Dequeue 4          {0,1,2,3,4}     [0,1,2,3,4]
                         → no new neighbors
  Done!  Queue empty.
```

### BFS Gives Shortest Paths (Unweighted)

Because BFS explores level by level, the **first time** it reaches a vertex is
always via the **shortest path** (fewest edges). This makes BFS perfect for
finding shortest paths in unweighted graphs.

To find the shortest path, track the distance to each vertex:

```
BFS shortest path:
    dist[start] = 0
    for each newly visited neighbor u of v:
        dist[u] = dist[v] + 1
```

On our example graph, distances from vertex 0:

```
    0 --- 1 --- 4         dist[0] = 0
    |     |                dist[1] = 1
    2     3                dist[2] = 1
                           dist[3] = 2
                           dist[4] = 2
```

### BFS Queue Implementation

For BFS we need a simple queue. We'll use a circular buffer inline:

```c
int queue[MAX_VERTICES];
int q_front = 0, q_back = 0;

/* enqueue */
queue[q_back++] = vertex;
if (q_back >= MAX_VERTICES) q_back = 0;

/* dequeue */
int v = queue[q_front++];
if (q_front >= MAX_VERTICES) q_front = 0;

/* empty? */
q_front == q_back
```

---

## Depth-First Search (DFS)

DFS explores a graph by going **as deep as possible** before backtracking. It's
like exploring a maze: at each junction, pick one path and follow it until you
hit a dead end, then backtrack and try the next path.

### The Algorithm

DFS naturally uses **recursion** (which uses the call stack):

```
DFS(graph, v, visited):
    mark v as visited
    process v (add to output order)
    for each neighbor u of v:
        if u is not visited:
            DFS(graph, u, visited)
```

### Step-by-Step Trace

Starting DFS from vertex 0 on the same graph:

```
    0 --- 1 --- 4
    |     |
    2     3
```

```
  Step   Stack (call)     Action             Visited         Order
  ────   ────────────     ──────             ───────         ─────
  1      [0]              Visit 0            {0}             [0]
  2      [0, 1]           Visit 1 (nbr of 0) {0,1}          [0,1]
  3      [0, 1, 3]        Visit 3 (nbr of 1) {0,1,3}        [0,1,3]
  4      [0, 1]           Backtrack (3 done)  {0,1,3}        [0,1,3]
  5      [0, 1, 4]        Visit 4 (nbr of 1) {0,1,3,4}      [0,1,3,4]
  6      [0, 1]           Backtrack (4 done)  {0,1,3,4}      [0,1,3,4]
  7      [0]              Backtrack (1 done)  {0,1,3,4}      [0,1,3,4]
  8      [0, 2]           Visit 2 (nbr of 0) {0,1,2,3,4}    [0,1,3,4,2]
  9      [0]              Backtrack (2 done)  {0,1,2,3,4}    [0,1,3,4,2]
  10     []               Backtrack (0 done)  {0,1,2,3,4}    [0,1,3,4,2]
  Done!
```

Notice the DFS order differs from BFS: DFS goes deep (0→1→3→4) before
exploring 2. BFS would have visited 2 before 3 and 4.

### BFS vs. DFS Summary

| Property        | BFS                    | DFS                     |
|-----------------|------------------------|--------------------------|
| Data structure  | Queue                  | Stack (or recursion)     |
| Explores        | Level by level         | Deep, then backtrack     |
| Shortest path?  | Yes (unweighted)       | No                       |
| Uses            | Shortest path, level   | Cycle detection, topo    |
|                 | order, connectivity    | sort, maze solving       |

---

## Dijkstra's Algorithm

BFS finds shortest paths in **unweighted** graphs. For **weighted** graphs
(where edges have costs), we need **Dijkstra's algorithm**.

### The Idea

Dijkstra's works like this: imagine you're at a city and you want to find the
shortest driving distance to every other city. You:

1. Start at the source (distance 0)
2. Always expand the **closest unvisited** city next
3. When you visit a city, check if going *through* it gives a shorter path to
   its neighbors — this is called **relaxation**

### The Algorithm

```
Dijkstra(graph, source):
    dist[v] = INFINITY for all v
    dist[source] = 0
    visited[v] = false for all v

    repeat V times:
        u = unvisited vertex with smallest dist[u]
        mark u as visited
        for each neighbor v of u:
            new_dist = dist[u] + weight(u, v)
            if new_dist < dist[v]:      ← relaxation
                dist[v] = new_dist
```

### Step-by-Step Trace

```
    0 --5-- 1 --2-- 4
    |       |
    3       1
    |       |
    2 --7-- 3
```

Finding shortest paths from vertex 0:

```
  Step   Visit   dist[0]  dist[1]  dist[2]  dist[3]  dist[4]
  ────   ─────   ───────  ───────  ───────  ───────  ───────
  init           0        INF      INF      INF      INF
  1      0       0        5        3        INF      INF
                          (0+5)    (0+3)
  2      2       0        5        3        10       INF
                                            (3+7)
  3      1       0        5        3        6        7
                                            (5+1)    (5+2)
  4      3       0        5        3        6        7
                          (no improvement)
  5      4       0        5        3        6        7
  Done!
```

**Key insight**: Dijkstra's always picks the unvisited vertex with the smallest
known distance. This greedy choice is correct because edge weights are
non-negative — you can never find a shortcut through a more-distant vertex.

> **Note**: Dijkstra's does NOT work with negative edge weights. For that you
> need Bellman-Ford (a topic for later).

### Complexity

With a simple array to find the minimum: **O(V^2)**
With a priority queue (min-heap): **O((V + E) log V)**

> **What is a priority queue / min-heap?** A priority queue is like a hospital
> waiting room --- the most urgent patient gets seen first, regardless of who
> arrived first. A **min-heap** is a tree-based data structure that efficiently
> keeps track of the smallest element (you learned about heaps in Module 15).
> Dijkstra uses a min-heap to always process the closest unvisited vertex next:
> instead of scanning all vertices to find the smallest distance (O(V)), a
> min-heap gives you the minimum in O(log V).

For sparse graphs, the heap version is much faster. We'll implement the simple
O(V^2) version here.

---

## Graphs Model Real Problems

Graphs are everywhere. Once you learn to see problems as graphs, you can apply
graph algorithms to solve them:

| Problem                | Vertices          | Edges                    | Algorithm    |
|------------------------|-------------------|--------------------------|--------------|
| GPS navigation         | Intersections     | Roads (weighted)         | Dijkstra     |
| Social network         | People            | Friendships              | BFS          |
| Web crawler            | Web pages         | Links (directed)         | BFS/DFS      |
| Course prerequisites   | Courses           | Dependencies (directed)  | Topological sort |
| Network routing        | Routers           | Connections (weighted)   | Dijkstra     |
| Maze solving           | Cells             | Passages                 | DFS/BFS      |

> **What is topological sort?** It is an ordering of vertices in a directed
> acyclic graph (DAG) such that for every edge A→B, A comes before B in the
> ordering. Think of it like a course schedule: if "Calculus 1" is a
> prerequisite for "Calculus 2", topological sort guarantees Calculus 1 appears
> first. You can compute it with a modified DFS.

---

## Exercises

| File            | Concepts                                | Tests |
|-----------------|------------------------------------------|-------|
| `graph.c`       | Adjacency list, BFS, DFS, shortest path  | 15    |
| `maze.c`        | BFS maze solver, path reconstruction     | 8     |
| `topo_sort.c`   | Topological sort (Kahn's), cycle detect  | 10    |

Build and test:
```bash
make exercises    # build student code (won't pass until you fill in TODOs)
make test         # build and run solutions to verify
```

---

[← Previous: Module 19 — The Preprocessor & Build Systems](../19-preprocessor-build-systems/README.md) | [Next: Module 21: Processes & Concurrency →](../21-process-concurrency/README.md)
