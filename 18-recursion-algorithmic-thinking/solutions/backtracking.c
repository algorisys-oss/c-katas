#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SUBSETS 256
#define MAX_SUBSET_SIZE 10
#define MAX_PERMS 720
#define MAX_COMBOS 256
#define MAX_BOARD 9
#define GRID_ROWS 3
#define GRID_COLS 4

/* ── Exercise 1: generate_subsets ─────────────────────────────────── */

static void subsets_helper(int arr[], int n, int index,
                           int current[], int cur_size,
                           int results[][MAX_SUBSET_SIZE],
                           int result_sizes[], int *count)
{
    if (index == n) {
        /* We've decided include/exclude for every element — record this subset */
        for (int i = 0; i < cur_size; i++) {
            results[*count][i] = current[i];
        }
        result_sizes[*count] = cur_size;
        (*count)++;
        return;
    }
    /* Choice 1: SKIP arr[index] */
    subsets_helper(arr, n, index + 1, current, cur_size,
                   results, result_sizes, count);
    /* Choice 2: INCLUDE arr[index] */
    current[cur_size] = arr[index];
    subsets_helper(arr, n, index + 1, current, cur_size + 1,
                   results, result_sizes, count);
}

int generate_subsets(int arr[], int n,
                     int results[][MAX_SUBSET_SIZE],
                     int result_sizes[])
{
    int count = 0;
    int current[MAX_SUBSET_SIZE];
    subsets_helper(arr, n, 0, current, 0, results, result_sizes, &count);
    return count;
}

/* ── Exercise 2: generate_permutations ────────────────────────────── */

static void perms_helper(int arr[], int n, int start,
                         int results[][MAX_SUBSET_SIZE], int *count)
{
    if (start == n) {
        for (int i = 0; i < n; i++) {
            results[*count][i] = arr[i];
        }
        (*count)++;
        return;
    }
    for (int i = start; i < n; i++) {
        /* Swap arr[start] with arr[i] — try element i in position start */
        int tmp = arr[start]; arr[start] = arr[i]; arr[i] = tmp;
        perms_helper(arr, n, start + 1, results, count);
        /* Undo the swap — this is the BACKTRACK step */
        tmp = arr[start]; arr[start] = arr[i]; arr[i] = tmp;
    }
}

int generate_permutations(int arr[], int n,
                          int results[][MAX_SUBSET_SIZE])
{
    int count = 0;
    perms_helper(arr, n, 0, results, &count);
    return count;
}

/* ── Exercise 3: generate_combinations ────────────────────────────── */

static void combos_helper(int n, int k, int start,
                           int current[], int cur_size,
                           int results[][MAX_SUBSET_SIZE], int *count)
{
    if (cur_size == k) {
        for (int i = 0; i < k; i++) {
            results[*count][i] = current[i];
        }
        (*count)++;
        return;
    }
    /* Only consider elements from 'start' onward to avoid duplicates */
    for (int i = start; i <= n; i++) {
        current[cur_size] = i;
        combos_helper(n, k, i + 1, current, cur_size + 1, results, count);
    }
}

int generate_combinations(int n, int k,
                           int results[][MAX_SUBSET_SIZE])
{
    int count = 0;
    int current[MAX_SUBSET_SIZE];
    combos_helper(n, k, 1, current, 0, results, &count);
    return count;
}

/* ── Exercise 4: solve_n_queens ───────────────────────────────────── */

static int is_safe(int board[], int row, int col)
{
    for (int r = 0; r < row; r++) {
        /* Same column or same diagonal? */
        if (board[r] == col ||
            abs(board[r] - col) == abs(r - row)) {
            return 0;
        }
    }
    return 1;
}

static void queens_helper(int board[], int n, int row, int *count)
{
    if (row == n) {
        (*count)++;
        return;
    }
    for (int col = 0; col < n; col++) {
        if (is_safe(board, row, col)) {
            board[row] = col;       /* place queen */
            queens_helper(board, n, row + 1, count);
            board[row] = -1;        /* backtrack: remove queen */
        }
    }
}

int solve_n_queens(int n)
{
    int board[MAX_BOARD];
    memset(board, -1, sizeof(board));
    int count = 0;
    queens_helper(board, n, 0, &count);
    return count;
}

/* ── Exercise 5: solve_sudoku ─────────────────────────────────────── */

static int sudoku_valid(int board[9][9], int row, int col, int num)
{
    /* Check row */
    for (int j = 0; j < 9; j++) {
        if (board[row][j] == num) return 0;
    }
    /* Check column */
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == num) return 0;
    }
    /* Check 3x3 box */
    int box_r = (row / 3) * 3;
    int box_c = (col / 3) * 3;
    for (int i = box_r; i < box_r + 3; i++) {
        for (int j = box_c; j < box_c + 3; j++) {
            if (board[i][j] == num) return 0;
        }
    }
    return 1;
}

int solve_sudoku(int board[9][9])
{
    /* Find the next empty cell (value 0) */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (sudoku_valid(board, i, j, num)) {
                        board[i][j] = num;      /* try this digit */
                        if (solve_sudoku(board)) {
                            return 1;            /* solved! */
                        }
                        board[i][j] = 0;         /* backtrack */
                    }
                }
                return 0;  /* no digit works — trigger backtracking */
            }
        }
    }
    return 1;  /* no empty cells — board is solved */
}

/* ── Exercise 6: word_search ──────────────────────────────────────── */

static int ws_helper(char grid[][GRID_COLS], int rows, int cols,
                     const char *word, int idx,
                     int r, int c, int visited[][GRID_COLS])
{
    if (word[idx] == '\0') {
        return 1;  /* all characters matched */
    }
    if (r < 0 || r >= rows || c < 0 || c >= cols) return 0;
    if (visited[r][c]) return 0;
    if (grid[r][c] != word[idx]) return 0;

    visited[r][c] = 1;  /* mark as used */

    /* Try all four directions */
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        if (ws_helper(grid, rows, cols, word, idx + 1,
                      r + dr[d], c + dc[d], visited)) {
            visited[r][c] = 0;  /* clean up before returning */
            return 1;
        }
    }

    visited[r][c] = 0;  /* backtrack: unmark */
    return 0;
}

int word_search(char grid[][GRID_COLS], int rows, int cols,
                const char *word)
{
    int visited[GRID_ROWS][GRID_COLS];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            memset(visited, 0, sizeof(visited));
            if (ws_helper(grid, rows, cols, word, 0, i, j, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

/* ── Test Harness ────────────────────────────────────────────────── */

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
    printf("=== Backtracking ===\n\n");

    /* ── generate_subsets tests ──────────────────────────────── */
    printf("generate_subsets:\n");
    {
        int arr[] = {1, 2, 3};
        int results[MAX_SUBSETS][MAX_SUBSET_SIZE];
        int sizes[MAX_SUBSETS];
        int count = generate_subsets(arr, 3, results, sizes);

        TEST("3-element array has 8 subsets") EXPECT(count == 8);

        /* Count how many are empty (size 0) and how many are full (size 3) */
        int empty = 0, full = 0;
        for (int i = 0; i < count; i++) {
            if (sizes[i] == 0) empty++;
            if (sizes[i] == 3) full++;
        }
        TEST("exactly 1 empty subset") EXPECT(empty == 1);
        TEST("exactly 1 full subset") EXPECT(full == 1);
    }

    /* ── generate_permutations tests ─────────────────────────── */
    printf("\ngenerate_permutations:\n");
    {
        int arr[] = {1, 2, 3};
        int results[MAX_PERMS][MAX_SUBSET_SIZE];
        int count = generate_permutations(arr, 3, results);

        TEST("3-element array has 6 permutations") EXPECT(count == 6);

        /* First permutation should start with 1 (identity) */
        TEST("first perm is [1,2,3]")
            EXPECT(results[0][0] == 1 && results[0][1] == 2 && results[0][2] == 3);

        /* Single element */
        int one[] = {42};
        count = generate_permutations(one, 1, results);
        TEST("1-element array has 1 permutation") EXPECT(count == 1);
    }

    /* ── generate_combinations tests ──────────────────────────── */
    printf("\ngenerate_combinations:\n");
    {
        int results[MAX_COMBOS][MAX_SUBSET_SIZE];

        int count = generate_combinations(4, 2, results);
        TEST("C(4,2) = 6") EXPECT(count == 6);

        count = generate_combinations(5, 3, results);
        TEST("C(5,3) = 10") EXPECT(count == 10);

        count = generate_combinations(3, 3, results);
        TEST("C(3,3) = 1") EXPECT(count == 1);
    }

    /* ── solve_n_queens tests ─────────────────────────────────── */
    printf("\nsolve_n_queens:\n");
    {
        TEST("N=1 has 1 solution") EXPECT(solve_n_queens(1) == 1);
        TEST("N=4 has 2 solutions") EXPECT(solve_n_queens(4) == 2);
        TEST("N=8 has 92 solutions") EXPECT(solve_n_queens(8) == 92);
    }

    /* ── solve_sudoku tests ───────────────────────────────────── */
    printf("\nsolve_sudoku:\n");
    {
        /* A well-known puzzle with unique solution */
        int board[9][9] = {
            {5,3,0, 0,7,0, 0,0,0},
            {6,0,0, 1,9,5, 0,0,0},
            {0,9,8, 0,0,0, 0,6,0},

            {8,0,0, 0,6,0, 0,0,3},
            {4,0,0, 8,0,3, 0,0,1},
            {7,0,0, 0,2,0, 0,0,6},

            {0,6,0, 0,0,0, 2,8,0},
            {0,0,0, 4,1,9, 0,0,5},
            {0,0,0, 0,8,0, 0,7,9}
        };
        int solved = solve_sudoku(board);
        TEST("sudoku is solvable") EXPECT(solved == 1);

        /* Verify a few known cells from the unique solution */
        TEST("board[0][2] = 4")
            EXPECT(board[0][2] == 4 && board[0][3] == 6 && board[0][5] == 8);
    }

    /* ── word_search tests ────────────────────────────────────── */
    printf("\nword_search:\n");
    {
        char grid[GRID_ROWS][GRID_COLS] = {
            {'A','B','C','E'},
            {'S','F','C','S'},
            {'A','D','E','E'}
        };

        TEST("'ABCCED' found")
            EXPECT(word_search(grid, 3, 4, "ABCCED") == 1);
        TEST("'SEE' found")
            EXPECT(word_search(grid, 3, 4, "SEE") == 1);
        TEST("'ABCB' not found (can't reuse)")
            EXPECT(word_search(grid, 3, 4, "ABCB") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
