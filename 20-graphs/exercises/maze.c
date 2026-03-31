/*
 * Kata: Maze Solver using BFS
 *
 * Represent a maze as a 2D char array and find the shortest path
 * from start ('S') to end ('E') using breadth-first search.
 *
 * Characters:
 *   '#' = wall (impassable)
 *   '.' = open path
 *   'S' = start position
 *   'E' = end position
 *
 * Functions to implement:
 *   - find_char       (locate a character in the maze)
 *   - solve_maze      (BFS shortest path from S to E)
 *   - print_maze      (print the maze to stdout)
 *
 * 8 tests total.
 *
 * Hints:
 *   - BFS guarantees the shortest path in an unweighted grid
 *   - Use a visited[][] array to avoid revisiting cells
 *   - Store the parent of each cell to reconstruct the path
 *   - Four directions: up, down, left, right (no diagonals)
 */

#include <stdio.h>
#include <string.h>

#define MAX_ROWS 32
#define MAX_COLS 32

/* ── Direction offsets ───────────────────────────────────────────── */
static const int DR[] = {-1, 1, 0, 0};  /* row deltas: up, down, left, right */
static const int DC[] = {0, 0, -1, 1};  /* col deltas */

/* ── Exercise 1: find_char ───────────────────────────────────────
 * Scan the maze to find the first occurrence of character 'ch'.
 * Set *row and *col to its position.
 * Return 1 if found, 0 if not found.
 */
int find_char(char maze[][MAX_COLS], int rows, int cols, char ch,
              int *row, int *col)
{
    /* TODO: loop through maze to find ch, set *row and *col */
    (void)maze; (void)rows; (void)cols; (void)ch; (void)row; (void)col;
    return 0;
}

/* ── Exercise 2: solve_maze ──────────────────────────────────────
 * Find the shortest path from 'S' to 'E' using BFS.
 *
 * Parameters:
 *   maze      — the maze grid
 *   rows/cols — dimensions
 *   path      — output array of [row, col] pairs for the path
 *   path_len  — output: number of steps in the path
 *
 * Returns 1 if a path is found, 0 if no path exists.
 * The path should include both S and E positions.
 *
 * Hints:
 *   - First, find S and E positions using find_char
 *   - Use a queue of (row, col) pairs for BFS
 *   - Track parent[r][c] to reconstruct the path
 *   - After BFS reaches E, walk backwards from E to S using parents
 *   - Reverse the path so it goes S → E
 */
int solve_maze(char maze[][MAX_COLS], int rows, int cols,
               int path[][2], int *path_len)
{
    /* TODO: BFS from S to E, store path, return 1 if found */
    (void)maze; (void)rows; (void)cols; (void)path; (void)path_len;
    return 0;
}

/* ── Exercise 3: print_maze ──────────────────────────────────────
 * Print the maze row by row to stdout.
 * Each row is followed by a newline.
 */
void print_maze(char maze[][MAX_COLS], int rows, int cols)
{
    /* TODO: print each row of the maze */
    (void)maze; (void)rows; (void)cols;
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

int main(void)
{
    printf("=== Maze Solver (BFS) ===\n\n");

    /* ── Test maze 1: simple 5x5 ────────────────────────────────
     *
     *   S . # . .
     *   # . # . .
     *   . . . . #
     *   . # # . .
     *   . . . . E
     */
    printf("simple 5x5 maze:\n");
    char maze1[MAX_ROWS][MAX_COLS];
    int rows1 = 5, cols1 = 5;
    memset(maze1, 0, sizeof(maze1));
    const char *m1[] = {
        "S.#..",
        "#.#..",
        "....#",
        ".##..",
        "....E"
    };
    for (int r = 0; r < rows1; r++) {
        for (int c = 0; c < cols1; c++) {
            maze1[r][c] = m1[r][c];
        }
    }

    TEST("find_char locates 'S' at (0,0)")
        int sr, sc;
        int found_s = find_char(maze1, rows1, cols1, 'S', &sr, &sc);
    EXPECT(found_s == 1 && sr == 0 && sc == 0);

    TEST("find_char locates 'E' at (4,4)")
        int er, ec;
        int found_e = find_char(maze1, rows1, cols1, 'E', &er, &ec);
    EXPECT(found_e == 1 && er == 4 && ec == 4);

    TEST("solve simple maze — path found")
        int path1[MAX_ROWS * MAX_COLS][2];
        int plen1 = 0;
        int solved1 = solve_maze(maze1, rows1, cols1, path1, &plen1);
    EXPECT(solved1 == 1 && plen1 > 0);

    TEST("solve simple maze — path starts at S and ends at E")
        int path1b[MAX_ROWS * MAX_COLS][2];
        int plen1b = 0;
        solve_maze(maze1, rows1, cols1, path1b, &plen1b);
        int ok1b = (plen1b > 0 &&
                    path1b[0][0] == 0 && path1b[0][1] == 0 &&
                    path1b[plen1b - 1][0] == 4 && path1b[plen1b - 1][1] == 4);
    EXPECT(ok1b);

    TEST("solve simple maze — shortest path length is 9")
        int path1c[MAX_ROWS * MAX_COLS][2];
        int plen1c = 0;
        solve_maze(maze1, rows1, cols1, path1c, &plen1c);
    EXPECT(plen1c == 9);

    /* ── Test maze 2: 8x8 with dead ends ────────────────────────
     *
     *   S . # . . . . .
     *   . . # . # # # .
     *   # . # . . . . .
     *   # . . . # . # .
     *   # # # . # . # .
     *   . . . . # . . .
     *   . # # # # # . #
     *   . . . . . . . E
     */
    printf("\n8x8 maze with dead ends:\n");
    char maze2[MAX_ROWS][MAX_COLS];
    int rows2 = 8, cols2 = 8;
    memset(maze2, 0, sizeof(maze2));
    const char *m2[] = {
        "S.#.....",
        "..#.###.",
        "#.#.....",
        "#...#.#.",
        "###.#.#.",
        "....#...",
        ".####.#.",  /* note: changed to .#### to block left path */
        ".......E"
    };
    for (int r = 0; r < rows2; r++) {
        for (int c = 0; c < cols2; c++) {
            maze2[r][c] = m2[r][c];
        }
    }

    TEST("solve 8x8 maze — path found")
        int path2[MAX_ROWS * MAX_COLS][2];
        int plen2 = 0;
        int solved2 = solve_maze(maze2, rows2, cols2, path2, &plen2);
    EXPECT(solved2 == 1 && plen2 > 0);

    /* ── Test maze 3: unsolvable ────────────────────────────────
     *
     *   S . .
     *   # # #
     *   . . E
     */
    printf("\nunsolvable maze:\n");
    char maze3[MAX_ROWS][MAX_COLS];
    int rows3 = 3, cols3 = 3;
    memset(maze3, 0, sizeof(maze3));
    const char *m3[] = {
        "S..",
        "###",
        "..E"
    };
    for (int r = 0; r < rows3; r++) {
        for (int c = 0; c < cols3; c++) {
            maze3[r][c] = m3[r][c];
        }
    }

    TEST("unsolvable maze — returns 0")
        int path3[MAX_ROWS * MAX_COLS][2];
        int plen3 = 0;
        int solved3 = solve_maze(maze3, rows3, cols3, path3, &plen3);
    EXPECT(solved3 == 0);

    TEST("unsolvable maze — path_len is 0")
        int path3b[MAX_ROWS * MAX_COLS][2];
        int plen3b = 99;
        solve_maze(maze3, rows3, cols3, path3b, &plen3b);
    EXPECT(plen3b == 0);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
