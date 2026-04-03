/*
 * Kata: Backtracking
 *
 * Backtracking is the most important recursion pattern for problem-solving.
 * The key idea: build a solution incrementally, and if the current path
 * can't lead to a valid solution, UNDO the last choice and try a different
 * path. It's like exploring a maze: go forward until you hit a dead end,
 * then retrace your steps and try a different turn.
 *
 * The pattern:
 *
 *   backtrack(choices_made_so_far):
 *       if we have a complete solution:
 *           record/print it
 *           return
 *       for each possible next choice:
 *           if this choice is valid:
 *               make the choice
 *               backtrack(choices_made_so_far + this choice)
 *               UNDO the choice    <-- this is the "backtrack" step
 *
 * Why is the UNDO step critical? Because after the recursive call returns,
 * we need the state to be exactly as it was before — so the NEXT iteration
 * of the loop can try a different choice from the same starting point.
 *
 * Decision tree for subsets of [1, 2]:
 *
 *                          {}
 *                        /    \
 *                  skip 1      include 1
 *                  /    \        /    \
 *            skip 2  incl 2  skip 2  incl 2
 *              |       |       |       |
 *             {}      {2}     {1}    {1,2}
 *
 * Each leaf is a valid subset. We explore every path from root to leaf.
 *
 * Backtracking with PRUNING (N-Queens):
 *
 *                          []
 *                    /   |   |   \
 *                Q@0   Q@1  Q@2  Q@3       <- row 0: try each column
 *               /|\    /|\
 *              X Q@2  X  X  Q@3  ...       <- row 1: skip invalid cols (X)
 *                |           |
 *               ...         ...
 *
 * The X branches are PRUNED — we don't explore them at all because we
 * already know a queen placement there would be attacked. This is why
 * backtracking is much faster than brute force: we cut off entire
 * subtrees early.
 *
 * N-Queens board (N=4, one solution):
 *
 *     0   1   2   3
 *   +---+---+---+---+
 * 0 |   | Q |   |   |    board[0] = 1
 *   +---+---+---+---+
 * 1 |   |   |   | Q |    board[1] = 3
 *   +---+---+---+---+
 * 2 | Q |   |   |   |    board[2] = 0
 *   +---+---+---+---+
 * 3 |   |   | Q |   |    board[3] = 2
 *   +---+---+---+---+
 *
 * No two queens share a row, column, or diagonal.
 *
 * Exercises:
 *   1. generate_subsets(arr, n)      — all subsets of an array
 *   2. generate_permutations(arr, n) — all permutations via swapping
 *   3. generate_combinations(n, k)   — all k-element combinations from 1..n
 *   4. solve_n_queens(n)             — count N-Queens solutions
 *   5. solve_sudoku(board)           — fill a 9x9 Sudoku board
 *   6. word_search(grid, word)       — find a word in a 2D grid
 *
 * 17 tests total.
 */

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

/* ── Exercise 1: generate_subsets ─────────────────────────────────────
 *
 * Generate all subsets of arr[0..n-1].
 *
 * For each element, you have exactly two choices: include it or skip it.
 * That's 2 choices for each of n elements, giving 2^n total subsets.
 *
 * Strategy: write a helper that takes an index into the array.
 *   - If index == n, we've decided for every element — record this subset.
 *   - Otherwise, first recurse WITHOUT including arr[index] (skip),
 *     then recurse WITH including arr[index] (include).
 *
 * Store each subset in results[count], and its size in result_sizes[count].
 * Return the total number of subsets generated.
 *
 * Hint: use a 'current' array to track the subset being built. Pass its
 * size along. When you reach the base case, copy current into results.
 */

/* TODO: Write a helper function subsets_helper that:
 *   - Takes: arr, n, index, current[], cur_size, results, result_sizes, count
 *   - Base case: if index == n, copy current into results[*count], increment count
 *   - Recursive case: skip arr[index], then include arr[index] and recurse
 */

int generate_subsets(int arr[], int n,
                     int results[][MAX_SUBSET_SIZE],
                     int result_sizes[])
{
    /* TODO: Initialize count to 0, create current[] array, call helper */

    return 0;  /* placeholder */
}

/* ── Exercise 2: generate_permutations ────────────────────────────────
 *
 * Generate all permutations of arr[0..n-1].
 *
 * Strategy (swapping approach):
 *   - For position 'start', try every element from index start to n-1.
 *   - Swap arr[start] with arr[i] to put element i in position start.
 *   - Recurse to fill positions start+1, start+2, ...
 *   - UNDO the swap (backtrack) so the next iteration of i starts fresh.
 *
 * When start == n, all positions are filled — record the permutation.
 *
 * This generates n! permutations (3! = 6, 4! = 24, etc).
 *
 * Return the total number of permutations generated.
 */

/* TODO: Write a helper function perms_helper that:
 *   - Takes: arr, n, start, results, count
 *   - Base case: if start == n, copy arr into results[*count]
 *   - Recursive case: for i from start to n-1:
 *       swap arr[start] and arr[i]
 *       recurse with start + 1
 *       swap back (BACKTRACK)
 */

int generate_permutations(int arr[], int n,
                          int results[][MAX_SUBSET_SIZE])
{
    /* TODO: Initialize count to 0, call helper, return count */

    return 0;  /* placeholder */
}

/* ── Exercise 3: generate_combinations ────────────────────────────────
 *
 * Generate all combinations of k elements from {1, 2, ..., n}.
 *
 * A combination is like a subset of fixed size k. Order doesn't matter:
 * {1,3} and {3,1} are the same combination.
 *
 * To avoid duplicates, only consider elements AFTER the last one chosen.
 * If you chose 2, only try 3, 4, ... next (never go backwards).
 *
 * Strategy: helper takes a 'start' value (smallest number to consider).
 *   - If cur_size == k, record the combination.
 *   - For i from start to n, add i to current and recurse with start = i+1.
 *
 * Return the total number of combinations (should be C(n,k) = n!/(k!(n-k)!)).
 */

/* TODO: Write a helper function combos_helper that:
 *   - Takes: n, k, start, current[], cur_size, results, count
 *   - Base case: if cur_size == k, copy current into results
 *   - Recursive case: for i from start to n:
 *       add i to current, recurse with start = i+1
 */

int generate_combinations(int n, int k,
                           int results[][MAX_SUBSET_SIZE])
{
    /* TODO: Initialize count to 0, create current[], call helper */

    return 0;  /* placeholder */
}

/* ── Exercise 4: solve_n_queens ───────────────────────────────────────
 *
 * Place n queens on an n×n chessboard so no two queens attack each other.
 * Queens attack along rows, columns, and diagonals.
 *
 * Representation: board[row] = column of queen in that row.
 * Since we place exactly one queen per row, rows are automatically safe.
 *
 * Validity check for placing a queen at (row, col):
 *   For each previously placed queen at (r, board[r]) where r < row:
 *     - Same column? board[r] == col
 *     - Same diagonal? |board[r] - col| == |r - row|
 *   If either is true, the placement is INVALID.
 *
 * Strategy:
 *   - Try to place a queen in each column of the current row.
 *   - If valid, place it and recurse to the next row.
 *   - If row == n, we've placed all queens — count this solution.
 *   - Backtrack: reset board[row] = -1 after recursing.
 *
 * Return the total number of valid solutions.
 */

/* TODO: Write is_safe(board, row, col) that returns 1 if placing a queen
 *   at (row, col) doesn't conflict with queens in rows 0..row-1 */

/* TODO: Write queens_helper(board, n, row, count) that:
 *   - Base case: row == n → increment *count
 *   - For each col 0..n-1: if is_safe, place, recurse, backtrack */

int solve_n_queens(int n)
{
    /* TODO: Create board array, initialize to -1, call helper */

    return 0;  /* placeholder */
}

/* ── Exercise 5: solve_sudoku ─────────────────────────────────────────
 *
 * Fill in a 9×9 Sudoku board. Empty cells contain 0.
 *
 * Rules: each row, column, and 3×3 box must contain digits 1-9 exactly once.
 *
 * Strategy:
 *   1. Find the next empty cell (scanning row by row, left to right).
 *   2. If no empty cell, the board is solved — return 1.
 *   3. For digits 1-9:
 *      a. Check if digit is valid at that position (row, column, and 3×3 box).
 *      b. If valid, place the digit and recurse.
 *      c. If recursion succeeds, return 1 (solution found).
 *      d. Otherwise, reset cell to 0 (backtrack) and try next digit.
 *   4. If no digit works, return 0 (triggers backtracking in the caller).
 *
 * Validity check for placing num at (row, col):
 *   - No same num in the row
 *   - No same num in the column
 *   - No same num in the 3×3 box (box starts at row/3*3, col/3*3)
 *
 * Returns 1 if solved, 0 if unsolvable from current state.
 */

/* TODO: Write sudoku_valid(board, row, col, num) that checks row, column,
 *   and 3×3 box for conflicts */

int solve_sudoku(int board[9][9])
{
    /* TODO: Find next empty cell (value 0).
     *   If none, return 1 (solved).
     *   For digits 1-9: if valid, place, recurse, backtrack on failure.
     *   If no digit works, return 0.
     */

    return 0;  /* placeholder */
}

/* ── Exercise 6: word_search ──────────────────────────────────────────
 *
 * Given a 2D grid of characters and a word, determine if the word exists
 * in the grid. The word can be formed by traversing adjacent cells
 * (up, down, left, right). Each cell may be used only ONCE per word.
 *
 * Strategy:
 *   1. For each cell in the grid, try to start the word there.
 *   2. Helper function: at position (r, c), matching character at index idx:
 *      a. If idx == strlen(word), all characters matched — return 1.
 *      b. If out of bounds, already visited, or grid[r][c] != word[idx], return 0.
 *      c. Mark (r, c) as visited.
 *      d. Try all 4 directions with idx + 1.
 *      e. Unmark (r, c) (backtrack) regardless of result.
 *
 * Returns 1 if the word is found, 0 otherwise.
 */

/* TODO: Write ws_helper(grid, rows, cols, word, idx, r, c, visited) */

int word_search(char grid[][GRID_COLS], int rows, int cols,
                const char *word)
{
    /* TODO: For each cell, reset visited and try ws_helper starting there */

    return 0;  /* placeholder */
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
