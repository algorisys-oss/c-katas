/*
 * Kata: Grid Problems — Islands, Rotten Oranges, Shortest Path
 *
 * Many classic problems are really graph problems on a 2D grid.
 * Every cell is a node, and its up/down/left/right neighbors are edges.
 *
 *   A 3x4 grid as a graph:
 *
 *     (0,0) --- (0,1) --- (0,2) --- (0,3)
 *       |         |         |         |
 *     (1,0) --- (1,1) --- (1,2) --- (1,3)
 *       |         |         |         |
 *     (2,0) --- (2,1) --- (2,2) --- (2,3)
 *
 *   Each cell has up to 4 neighbors (no diagonals):
 *
 *              up (r-1, c)
 *                 |
 *   left (r, c-1) -- (r, c) -- right (r, c+1)
 *                 |
 *             down (r+1, c)
 *
 * Functions to implement:
 *   - count_islands        (DFS flood fill to count connected components)
 *   - rotten_oranges       (multi-source BFS for minimum time)
 *   - shortest_path_grid   (BFS shortest path from corner to corner)
 *
 * 14 tests total.
 *
 * Hints:
 *   - The 4 directions can be stored in arrays:
 *     int dr[] = {-1, 1, 0, 0};   (up, down, same, same)
 *     int dc[] = {0, 0, -1, 1};   (same, same, left, right)
 *   - For BFS, implement a simple circular queue inline (see graph.c)
 *   - Always check bounds: 0 <= r < rows && 0 <= c < cols
 */

#include <stdio.h>
#include <string.h>

#define MAX_ROWS 20
#define MAX_COLS 20

/* Direction vectors for up, down, left, right */
static const int DR[] = {-1, 1, 0, 0};
static const int DC[] = {0, 0, -1, 1};

/* ── Simple Queue for BFS ────────────────────────────────────────
 *
 * We encode a (row, col) pair as a single int: row * MAX_COLS + col.
 * To decode: row = val / MAX_COLS, col = val % MAX_COLS.
 *
 * This avoids needing a struct — just an array of ints.
 */
#define MAX_QUEUE (MAX_ROWS * MAX_COLS)

/* ── Exercise 1: count_islands ───────────────────────────────────
 *
 * Given a grid of '1' (land) and '0' (water), count the number of
 * islands. An island is a group of '1's connected horizontally or
 * vertically (NOT diagonally).
 *
 * Approach: DFS flood fill
 *   1. Scan every cell
 *   2. When you find a '1', increment island count
 *   3. DFS from that cell to mark all connected '1's as '0' (visited)
 *   4. Continue scanning
 *
 *   DFS flood fill on this grid:
 *
 *     1 1 0 0 0       0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
 *     1 1 0 0 0  -->  0 0 0 0 0  -->  0 0 0 0 0  -->  0 0 0 0 0
 *     0 0 1 0 0       0 0 1 0 0       0 0 0 0 0       0 0 0 0 0
 *     0 0 0 1 1       0 0 0 1 1       0 0 0 1 1       0 0 0 0 0
 *     Island #1       Island #2       Island #3
 *     (flood fill     (flood fill     (flood fill
 *      top-left)       center)         bottom-right)
 *
 * Note: this function MODIFIES the grid (marks '1' as '0').
 *       If you need the original, copy it first.
 *
 * Time: O(rows * cols) — every cell visited at most once.
 */

/* Helper: DFS flood fill — mark all connected land as visited */
static void flood_fill(int grid[MAX_ROWS][MAX_COLS], int rows, int cols,
                        int r, int c)
{
    /* TODO: mark grid[r][c] as 0 (visited)
     *       for each of 4 directions:
     *         compute nr = r + DR[d], nc = c + DC[d]
     *         if in bounds AND grid[nr][nc] == 1:
     *           recurse on (nr, nc)
     */
}

int count_islands(int grid[MAX_ROWS][MAX_COLS], int rows, int cols)
{
    /* TODO: scan every cell, when grid[r][c] == 1:
     *         increment count
     *         call flood_fill to mark the whole island
     *       return count
     */
    return 0; /* placeholder */
}

/* ── Exercise 2: rotten_oranges ──────────────────────────────────
 *
 * Grid values: 0 = empty, 1 = fresh orange, 2 = rotten orange.
 * Each minute, every rotten orange rots its 4-directional neighbors.
 * Return the minimum minutes until no fresh orange remains, or -1
 * if it's impossible (some fresh orange is unreachable).
 *
 * This is MULTI-SOURCE BFS: start from ALL rotten oranges at once.
 *
 *   Multi-source BFS wavefront (two sources):
 *
 *   Minute 0:    Minute 1:    Minute 2:    Minute 3:
 *   2 1 1 1 2   2 2 1 1 2   2 2 2 2 2   2 2 2 2 2
 *   1 1 1 1 1   2 1 1 1 2   2 2 1 2 2   2 2 2 2 2
 *                                         Answer: 3 minutes
 *   (BFS expands from ALL rotten cells simultaneously each minute)
 *
 * Algorithm:
 *   1. Enqueue ALL rotten oranges (minute 0), count fresh oranges
 *   2. BFS: dequeue, rot fresh neighbors, enqueue them
 *   3. Track minutes (each BFS level = 1 minute)
 *   4. If fresh count reaches 0, return minutes. Else return -1.
 *
 * Time: O(rows * cols).
 */
int rotten_oranges(int grid[MAX_ROWS][MAX_COLS], int rows, int cols)
{
    /* TODO:
     * 1. Scan grid: enqueue all rotten (2) cells, count fresh (1) cells
     * 2. If fresh == 0, return 0 (nothing to rot)
     * 3. BFS level by level:
     *      - process all cells at current level
     *      - for each, check 4 neighbors
     *      - if neighbor is fresh (1), mark as rotten (2), enqueue, fresh--
     *      - after each level, minutes++
     * 4. If fresh == 0, return minutes. Else return -1.
     *
     * Queue encoding: queue[i] = r * MAX_COLS + c
     * Decode: r = queue[i] / MAX_COLS, c = queue[i] % MAX_COLS
     */
    return -1; /* placeholder */
}

/* ── Exercise 3: shortest_path_grid ──────────────────────────────
 *
 * Given a binary grid (0 = open, 1 = wall), find the shortest path
 * from top-left (0,0) to bottom-right (rows-1, cols-1).
 * Move only through 0-cells, up/down/left/right.
 * Return path length (number of steps), or -1 if no path exists.
 *
 * BFS on unweighted graph = shortest path (first arrival wins).
 *
 *   Example:
 *     0 0 1 0        S . # .        Path: (0,0)->(0,1)->(1,1)->
 *     0 0 0 0   ==>  . . . .              (1,2)->(1,3)->(2,3)->
 *     1 1 0 0        # # . .              (3,3)
 *     0 0 0 0        . . . E        Length: 6 steps
 *
 * Algorithm:
 *   1. If start or end is a wall, return -1
 *   2. BFS from (0,0), track distance with dist[][] = -1 (unvisited)
 *   3. dist[0][0] = 0
 *   4. For each cell dequeued, check 4 neighbors:
 *        if in bounds, not a wall, and dist == -1:
 *          dist[nr][nc] = dist[r][c] + 1
 *          if (nr, nc) is destination, return that distance
 *          enqueue
 *   5. If BFS ends without reaching destination, return -1
 *
 * Time: O(rows * cols).
 */
int shortest_path_grid(int grid[MAX_ROWS][MAX_COLS], int rows, int cols)
{
    /* TODO: BFS from (0,0) to (rows-1, cols-1)
     *
     * Use a dist[MAX_ROWS][MAX_COLS] array, initialized to -1.
     * Queue encoding: queue[i] = r * MAX_COLS + c
     *
     * Special cases:
     *   - single cell grid (rows==1, cols==1, grid[0][0]==0) -> return 0
     *   - start or end is wall -> return -1
     */
    return -1; /* placeholder */
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

/* Helper: copy a small array into a MAX_ROWS x MAX_COLS grid */
static void load_grid(int dst[MAX_ROWS][MAX_COLS],
                       const int *src, int rows, int cols)
{
    memset(dst, 0, sizeof(int) * MAX_ROWS * MAX_COLS);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            dst[r][c] = src[r * cols + c];
        }
    }
}

int main(void)
{
    int grid[MAX_ROWS][MAX_COLS];

    printf("=== Grid Problems: Islands, Rotten Oranges, Shortest Path ===\n\n");

    /* ── count_islands tests ────────────────────────────────────── */
    printf("count_islands:\n");

    /* Test 1: multiple islands */
    {
        const int data[] = {
            1, 1, 0, 0, 0,
            1, 1, 0, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 0, 1, 1
        };
        load_grid(grid, data, 4, 5);
        TEST("3 islands in 4x5 grid")
            EXPECT(count_islands(grid, 4, 5) == 3);
    }

    /* Test 2: single island */
    {
        const int data[] = {
            1, 1,
            1, 1
        };
        load_grid(grid, data, 2, 2);
        TEST("single island (all land)")
            EXPECT(count_islands(grid, 2, 2) == 1);
    }

    /* Test 3: no islands */
    {
        const int data[] = {
            0, 0, 0,
            0, 0, 0
        };
        load_grid(grid, data, 2, 3);
        TEST("no islands (all water)")
            EXPECT(count_islands(grid, 2, 3) == 0);
    }

    /* Test 4: diagonal doesn't connect */
    {
        const int data[] = {
            1, 0,
            0, 1
        };
        load_grid(grid, data, 2, 2);
        TEST("diagonal cells are separate islands")
            EXPECT(count_islands(grid, 2, 2) == 2);
    }

    /* Test 5: complex shape */
    {
        const int data[] = {
            1, 0, 1, 0, 1,
            0, 0, 0, 0, 0,
            1, 0, 1, 0, 1
        };
        load_grid(grid, data, 3, 5);
        TEST("6 isolated single-cell islands")
            EXPECT(count_islands(grid, 3, 5) == 6);
    }

    /* ── rotten_oranges tests ───────────────────────────────────── */
    printf("\nrotten_oranges:\n");

    /* Test 6: simple grid, all rot in 2 minutes */
    {
        const int data[] = {
            2, 1, 1,
            1, 1, 0,
            0, 1, 1
        };
        load_grid(grid, data, 3, 3);
        TEST("all rot in 4 minutes (3x3 grid)")
            EXPECT(rotten_oranges(grid, 3, 3) == 4);
    }

    /* Test 7: impossible — isolated fresh orange */
    {
        const int data[] = {
            2, 1, 1,
            0, 1, 1,
            1, 0, 1
        };
        load_grid(grid, data, 3, 3);
        TEST("impossible: isolated fresh returns -1")
            EXPECT(rotten_oranges(grid, 3, 3) == -1);
    }

    /* Test 8: already all rotten */
    {
        const int data[] = {
            2, 2,
            2, 2
        };
        load_grid(grid, data, 2, 2);
        TEST("already all rotten: 0 minutes")
            EXPECT(rotten_oranges(grid, 2, 2) == 0);
    }

    /* Test 9: empty grid (no oranges at all) */
    {
        const int data[] = {
            0, 0,
            0, 0
        };
        load_grid(grid, data, 2, 2);
        TEST("empty grid (no oranges): 0 minutes")
            EXPECT(rotten_oranges(grid, 2, 2) == 0);
    }

    /* Test 10: multiple rotten sources */
    {
        const int data[] = {
            2, 1, 1, 1, 2,
            1, 1, 1, 1, 1
        };
        load_grid(grid, data, 2, 5);
        TEST("two rotten sources meet in middle: 3 min")
            EXPECT(rotten_oranges(grid, 2, 5) == 3);
    }

    /* ── shortest_path_grid tests ───────────────────────────────── */
    printf("\nshortest_path_grid:\n");

    /* Test 11: simple path exists */
    {
        const int data[] = {
            0, 0, 1, 0,
            0, 0, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0
        };
        load_grid(grid, data, 4, 4);
        TEST("shortest path in 4x4 grid is 6")
            EXPECT(shortest_path_grid(grid, 4, 4) == 6);
    }

    /* Test 12: no path (walled off) */
    {
        const int data[] = {
            0, 1, 0,
            0, 1, 0,
            0, 1, 0
        };
        load_grid(grid, data, 3, 3);
        TEST("no path through wall: returns -1")
            EXPECT(shortest_path_grid(grid, 3, 3) == -1);
    }

    /* Test 13: single cell grid */
    {
        const int data[] = {0};
        load_grid(grid, data, 1, 1);
        TEST("single cell grid: path length 0")
            EXPECT(shortest_path_grid(grid, 1, 1) == 0);
    }

    /* Test 14: maze-like grid */
    {
        const int data[] = {
            0, 0, 0, 0, 0,
            1, 1, 1, 1, 0,
            0, 0, 0, 0, 0,
            0, 1, 1, 1, 1,
            0, 0, 0, 0, 0
        };
        load_grid(grid, data, 5, 5);
        TEST("maze-like grid: path length 16")
            EXPECT(shortest_path_grid(grid, 5, 5) == 16);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
