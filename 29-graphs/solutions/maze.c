/*
 * Solution: Maze Solver using BFS
 *
 * Find the shortest path through a maze using breadth-first search.
 *
 * 8 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_ROWS 32
#define MAX_COLS 32

/* ── Direction offsets ───────────────────────────────────────────── */
static const int DR[] = {-1, 1, 0, 0};
static const int DC[] = {0, 0, -1, 1};

/* ── find_char ───────────────────────────────────────────────────── */
int find_char(char maze[][MAX_COLS], int rows, int cols, char ch,
              int *row, int *col)
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (maze[r][c] == ch) {
                *row = r;
                *col = c;
                return 1;
            }
        }
    }
    return 0;
}

/* ── solve_maze ──────────────────────────────────────────────────── */
int solve_maze(char maze[][MAX_COLS], int rows, int cols,
               int path[][2], int *path_len)
{
    *path_len = 0;

    int sr, sc, er, ec;
    if (!find_char(maze, rows, cols, 'S', &sr, &sc)) return 0;
    if (!find_char(maze, rows, cols, 'E', &er, &ec)) return 0;

    int visited[MAX_ROWS][MAX_COLS];
    int parent_r[MAX_ROWS][MAX_COLS];
    int parent_c[MAX_ROWS][MAX_COLS];
    memset(visited, 0, sizeof(visited));
    memset(parent_r, -1, sizeof(parent_r));
    memset(parent_c, -1, sizeof(parent_c));

    /* BFS queue: store (row, col) pairs */
    int q_r[MAX_ROWS * MAX_COLS];
    int q_c[MAX_ROWS * MAX_COLS];
    int q_front = 0, q_back = 0;

    visited[sr][sc] = 1;
    q_r[q_back] = sr;
    q_c[q_back] = sc;
    q_back++;

    int found = 0;

    while (q_front < q_back) {
        int r = q_r[q_front];
        int c = q_c[q_front];
        q_front++;

        if (r == er && c == ec) {
            found = 1;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (visited[nr][nc]) continue;
            if (maze[nr][nc] == '#') continue;

            visited[nr][nc] = 1;
            parent_r[nr][nc] = r;
            parent_c[nr][nc] = c;
            q_r[q_back] = nr;
            q_c[q_back] = nc;
            q_back++;
        }
    }

    if (!found) return 0;

    /* Reconstruct path from E back to S */
    int temp[MAX_ROWS * MAX_COLS][2];
    int tlen = 0;
    int cr = er, cc = ec;
    while (cr != -1 && cc != -1) {
        temp[tlen][0] = cr;
        temp[tlen][1] = cc;
        tlen++;
        if (cr == sr && cc == sc) break;
        int pr = parent_r[cr][cc];
        int pc = parent_c[cr][cc];
        cr = pr;
        cc = pc;
    }

    /* Reverse into path[] so it goes S -> E */
    *path_len = tlen;
    for (int i = 0; i < tlen; i++) {
        path[i][0] = temp[tlen - 1 - i][0];
        path[i][1] = temp[tlen - 1 - i][1];
    }

    return 1;
}

/* ── print_maze ──────────────────────────────────────────────────── */
void print_maze(char maze[][MAX_COLS], int rows, int cols)
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            putchar(maze[r][c]);
        }
        putchar('\n');
    }
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

    /* ── Test maze 1: simple 5x5 ──────────────────────────────── */
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

    /* ── Test maze 2: 8x8 with dead ends ──────────────────────── */
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
        ".####.#.",
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

    /* ── Test maze 3: unsolvable ──────────────────────────────── */
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
