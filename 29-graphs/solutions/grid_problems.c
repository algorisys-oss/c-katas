/*
 * Solution: Grid Problems — Islands, Rotten Oranges, Shortest Path
 *
 * Three classic problems that are really graph problems on a 2D grid.
 * Each cell is a node, and neighbors (up/down/left/right) are edges.
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_ROWS 20
#define MAX_COLS 20

/* Direction vectors for up, down, left, right */
static const int DR[] = {-1, 1, 0, 0};
static const int DC[] = {0, 0, -1, 1};

/* ── Simple Queue for BFS ──────────────────────────────────────── */
#define MAX_QUEUE (MAX_ROWS * MAX_COLS)

/* ── count_islands ───────────────────────────────────────────── */

/* DFS flood fill: mark all connected land as visited (set to 0) */
static void flood_fill(int grid[MAX_ROWS][MAX_COLS], int rows, int cols,
                        int r, int c)
{
    grid[r][c] = 0;
    for (int d = 0; d < 4; d++) {
        int nr = r + DR[d];
        int nc = c + DC[d];
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols
            && grid[nr][nc] == 1) {
            flood_fill(grid, rows, cols, nr, nc);
        }
    }
}

int count_islands(int grid[MAX_ROWS][MAX_COLS], int rows, int cols)
{
    int count = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 1) {
                count++;
                flood_fill(grid, rows, cols, r, c);
            }
        }
    }
    return count;
}

/* ── rotten_oranges ──────────────────────────────────────────── */
int rotten_oranges(int grid[MAX_ROWS][MAX_COLS], int rows, int cols)
{
    int queue[MAX_QUEUE];
    int q_front = 0, q_back = 0;
    int fresh = 0;

    /* Enqueue all rotten oranges, count fresh ones */
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 2) {
                queue[q_back] = r * MAX_COLS + c;
                q_back = (q_back + 1) % MAX_QUEUE;
            } else if (grid[r][c] == 1) {
                fresh++;
            }
        }
    }

    if (fresh == 0) {
        return 0;
    }

    int minutes = 0;

    /* BFS level by level — each level is one minute */
    while (q_front != q_back) {
        int level_size = (q_back - q_front + MAX_QUEUE) % MAX_QUEUE;

        for (int i = 0; i < level_size; i++) {
            int val = queue[q_front];
            q_front = (q_front + 1) % MAX_QUEUE;
            int r = val / MAX_COLS;
            int c = val % MAX_COLS;

            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols
                    && grid[nr][nc] == 1) {
                    grid[nr][nc] = 2;
                    fresh--;
                    queue[q_back] = nr * MAX_COLS + nc;
                    q_back = (q_back + 1) % MAX_QUEUE;
                }
            }
        }

        minutes++;

        if (fresh == 0) {
            return minutes;
        }
    }

    return -1;
}

/* ── shortest_path_grid ──────────────────────────────────────── */
int shortest_path_grid(int grid[MAX_ROWS][MAX_COLS], int rows, int cols)
{
    /* Single cell */
    if (rows == 1 && cols == 1 && grid[0][0] == 0) {
        return 0;
    }

    /* Start or end is a wall */
    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) {
        return -1;
    }

    int dist[MAX_ROWS][MAX_COLS];
    memset(dist, -1, sizeof(dist));
    dist[0][0] = 0;

    int queue[MAX_QUEUE];
    int q_front = 0, q_back = 0;

    queue[q_back] = 0; /* (0,0) encoded as 0 * MAX_COLS + 0 */
    q_back = (q_back + 1) % MAX_QUEUE;

    while (q_front != q_back) {
        int val = queue[q_front];
        q_front = (q_front + 1) % MAX_QUEUE;
        int r = val / MAX_COLS;
        int c = val % MAX_COLS;

        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols
                && grid[nr][nc] == 0 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                if (nr == rows - 1 && nc == cols - 1) {
                    return dist[nr][nc];
                }
                queue[q_back] = nr * MAX_COLS + nc;
                q_back = (q_back + 1) % MAX_QUEUE;
            }
        }
    }

    return -1;
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
