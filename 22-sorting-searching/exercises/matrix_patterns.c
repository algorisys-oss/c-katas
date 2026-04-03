/*
 * Kata: Matrix / 2D Array Interview Patterns
 *
 * Classic matrix manipulation problems that appear constantly in interviews.
 * The key insight for most matrix problems: find a clever traversal order
 * or an in-place transformation trick.
 *
 * Exercises:
 *   - spiral_order         (four-boundary shrinking rectangle)
 *   - rotate_90            (transpose + reverse rows)
 *   - set_matrix_zeroes    (use first row/col as markers)
 *   - search_sorted_matrix (staircase search from top-right)
 *   - transpose            (swap rows and columns)
 *
 * 17 tests total.
 *
 * ASCII art — spiral traversal with four boundaries:
 *
 *     top=0
 *     ┌─────────────────────┐
 *     │  1 → 2 → 3 → 4 → 5 │  ← move right along top row
 *     │                  ↓  │
 *     │  6   7   8   9  10  │  ← right boundary: move down
 *     │                  ↓  │
 *     │ 11  12  13  14  15  │
 *     │  ↑               ↓  │
 *     │ 16 ← 17 ← 18 ← 19  20  ← move left along bottom row
 *     └─────────────────────┘
 *     left=0              right=4
 *                         bottom=3
 *
 *   After each sweep, shrink the boundary inward:
 *   - Finished top row?    → top++
 *   - Finished right col?  → right--
 *   - Finished bottom row? → bottom--
 *   - Finished left col?   → left++
 *
 * ASCII art — rotate 90 degrees clockwise (the two-step trick):
 *
 *   Original:       Transpose:       Reverse rows:
 *   1 2 3           1 4 7            7 4 1
 *   4 5 6    →      2 5 8     →      8 5 2
 *   7 8 9           3 6 9            9 6 3
 *
 *   Step 1: swap matrix[i][j] with matrix[j][i] (mirror across diagonal)
 *   Step 2: reverse each row (mirror left-to-right)
 *   Combined effect: 90-degree clockwise rotation!
 *
 * ASCII art — staircase search in sorted matrix:
 *
 *    Start at top-right corner:
 *
 *     1   4   7  [11] ← start here
 *     2   5   8   12    target=5
 *     3   6   9   16    11 > 5 → go LEFT
 *    10  13  14   17
 *
 *     1   4  [7]  11
 *     2   5   8   12    7 > 5 → go LEFT
 *     3   6   9   16
 *    10  13  14   17
 *
 *     1  [4]  7   11
 *     2   5   8   12    4 < 5 → go DOWN
 *     3   6   9   16
 *    10  13  14   17
 *
 *     1   4   7   11
 *     2  [5]  8   12    5 == 5 → FOUND at (1,1)
 *     3   6   9   16
 *    10  13  14   17
 */

#include <stdio.h>
#include <string.h>

#define MAX_N 64

/* ── Exercise 1: Spiral Order ─────────────────────────────────────── *
 * Given an m x n matrix, return elements in spiral order.
 *
 * Use four boundaries: top, bottom, left, right.
 * Sweep right along top → down along right → left along bottom →
 * up along left. After each sweep, shrink that boundary inward.
 *
 * Store result in out[], return the number of elements written.
 *
 * Example:
 *   1 2 3
 *   4 5 6  →  [1, 2, 3, 6, 9, 8, 7, 4, 5]
 *   7 8 9
 */
int spiral_order(int matrix[MAX_N][MAX_N], int rows, int cols,
                 int *out)
{
    /* TODO: Use four boundaries (top, bottom, left, right).
     * Loop while top <= bottom && left <= right.
     * Each iteration: sweep right, sweep down, sweep left, sweep up.
     * After each sweep, shrink the boundary. */
    (void)matrix; (void)rows; (void)cols; (void)out;
    return 0;
}

/* ── Exercise 2: Rotate 90 Degrees Clockwise ──────────────────────── *
 * Rotate an n x n matrix 90 degrees clockwise IN-PLACE.
 *
 * Two-step trick:
 *   1. Transpose: swap matrix[i][j] with matrix[j][i]
 *   2. Reverse each row
 *
 * Why does this work geometrically?
 *   - Transpose mirrors across the main diagonal (top-left to bottom-right)
 *   - Reversing rows mirrors left-to-right
 *   - The composition of these two reflections is a 90-degree rotation!
 *
 * Example:
 *   1 2 3      7 4 1
 *   4 5 6  →   8 5 2
 *   7 8 9      9 6 3
 */
void rotate_90(int matrix[MAX_N][MAX_N], int n)
{
    /* TODO: Step 1 — transpose (swap [i][j] with [j][i] for i < j)
     *       Step 2 — reverse each row */
    (void)matrix; (void)n;
}

/* ── Exercise 3: Set Matrix Zeroes ────────────────────────────────── *
 * If an element is 0, set its entire row and column to 0.
 * Do it in O(1) extra space.
 *
 * The trick: use the first row and first column as markers.
 *   1. Check if the first row/col themselves contain any zeros (save flags)
 *   2. Scan the rest of the matrix: if matrix[i][j]==0, set
 *      matrix[i][0]=0 and matrix[0][j]=0 as markers
 *   3. Use the markers to zero out rows and columns
 *   4. Finally, zero out the first row/col if needed (using saved flags)
 *
 * Example:
 *   1 1 1      1 0 1
 *   1 0 1  →   0 0 0
 *   1 1 1      1 0 1
 */
void set_matrix_zeroes(int matrix[MAX_N][MAX_N], int rows, int cols)
{
    /* TODO: Implement the O(1) space algorithm using first row/col as markers */
    (void)matrix; (void)rows; (void)cols;
}

/* ── Exercise 4: Search Sorted Matrix ─────────────────────────────── *
 * Search for target in a matrix where each row and each column
 * is sorted in ascending order.
 *
 * Start at the top-right corner:
 *   - If current == target → found!
 *   - If current > target  → move left  (eliminate this column)
 *   - If current < target  → move down  (eliminate this row)
 *
 * Time: O(m + n) — at most m + n steps.
 *
 * Return 1 if found (and set *row, *col), 0 if not found.
 *
 * Example:
 *    1   4   7  11
 *    2   5   8  12    target=5 → found at (1,1)
 *    3   6   9  16
 *   10  13  14  17
 */
int search_sorted_matrix(int matrix[MAX_N][MAX_N], int rows, int cols,
                         int target, int *row, int *col)
{
    /* TODO: Start at top-right corner, staircase search */
    (void)matrix; (void)rows; (void)cols; (void)target;
    (void)row; (void)col;
    return 0;
}

/* ── Exercise 5: Transpose ────────────────────────────────────────── *
 * Transpose an m x n matrix: swap rows and columns.
 * The result is an n x m matrix stored in out[][].
 *
 * matrix[i][j] → out[j][i]
 *
 * Example:
 *   1 2 3      1 4
 *   4 5 6  →   2 5
 *              3 6
 */
void transpose(int matrix[MAX_N][MAX_N], int rows, int cols,
               int out[MAX_N][MAX_N])
{
    /* TODO: Copy matrix[i][j] to out[j][i] */
    (void)matrix; (void)rows; (void)cols; (void)out;
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

static int arr_eq(const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

static int mat_eq(int a[MAX_N][MAX_N], int b[MAX_N][MAX_N], int rows, int cols)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (a[i][j] != b[i][j]) return 0;
        }
    }
    return 1;
}

int main(void)
{
    printf("=== Matrix / 2D Array Patterns ===\n\n");

    /* --- spiral_order --- */
    printf("spiral_order:\n");
    {
        int m[MAX_N][MAX_N] = {{1,2,3},{4,5,6},{7,8,9}};
        int out[MAX_N * MAX_N];
        int exp[] = {1,2,3,6,9,8,7,4,5};
        int n = spiral_order(m, 3, 3, out);
        TEST("3x3 matrix spiral") EXPECT(n == 9 && arr_eq(out, exp, 9));
    }
    {
        int m[MAX_N][MAX_N] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
        int out[MAX_N * MAX_N];
        int exp[] = {1,2,3,4,8,12,11,10,9,5,6,7};
        int n = spiral_order(m, 3, 4, out);
        TEST("3x4 matrix spiral") EXPECT(n == 12 && arr_eq(out, exp, 12));
    }
    {
        int m[MAX_N][MAX_N] = {{1,2},{3,4},{5,6}};
        int out[MAX_N * MAX_N];
        int exp[] = {1,2,4,6,5,3};
        int n = spiral_order(m, 3, 2, out);
        TEST("3x2 matrix spiral") EXPECT(n == 6 && arr_eq(out, exp, 6));
    }
    {
        int m[MAX_N][MAX_N] = {{42}};
        int out[MAX_N * MAX_N];
        int n = spiral_order(m, 1, 1, out);
        TEST("1x1 matrix spiral") EXPECT(n == 1 && out[0] == 42);
    }

    /* --- rotate_90 --- */
    printf("\nrotate_90:\n");
    {
        int m[MAX_N][MAX_N] = {{1,2,3},{4,5,6},{7,8,9}};
        int exp[MAX_N][MAX_N] = {{7,4,1},{8,5,2},{9,6,3}};
        rotate_90(m, 3);
        TEST("3x3 rotation") EXPECT(mat_eq(m, exp, 3, 3));
    }
    {
        int m[MAX_N][MAX_N] = {{1,2},{3,4}};
        int exp[MAX_N][MAX_N] = {{3,1},{4,2}};
        rotate_90(m, 2);
        TEST("2x2 rotation") EXPECT(mat_eq(m, exp, 2, 2));
    }
    {
        int m[MAX_N][MAX_N] = {{5}};
        rotate_90(m, 1);
        TEST("1x1 rotation (no change)") EXPECT(m[0][0] == 5);
    }

    /* --- set_matrix_zeroes --- */
    printf("\nset_matrix_zeroes:\n");
    {
        int m[MAX_N][MAX_N] = {{1,1,1},{1,0,1},{1,1,1}};
        int exp[MAX_N][MAX_N] = {{1,0,1},{0,0,0},{1,0,1}};
        set_matrix_zeroes(m, 3, 3);
        TEST("single zero in center") EXPECT(mat_eq(m, exp, 3, 3));
    }
    {
        int m[MAX_N][MAX_N] = {{0,1,2,0},{3,4,5,2},{1,3,1,5}};
        int exp[MAX_N][MAX_N] = {{0,0,0,0},{0,4,5,0},{0,3,1,0}};
        set_matrix_zeroes(m, 3, 4);
        TEST("zeros in first row") EXPECT(mat_eq(m, exp, 3, 4));
    }
    {
        int m[MAX_N][MAX_N] = {{1,2,3},{4,5,6}};
        int exp[MAX_N][MAX_N] = {{1,2,3},{4,5,6}};
        set_matrix_zeroes(m, 2, 3);
        TEST("no zeros — unchanged") EXPECT(mat_eq(m, exp, 2, 3));
    }
    {
        int m[MAX_N][MAX_N] = {{0,0},{0,0}};
        int exp[MAX_N][MAX_N] = {{0,0},{0,0}};
        set_matrix_zeroes(m, 2, 2);
        TEST("all zeros — stays all zeros") EXPECT(mat_eq(m, exp, 2, 2));
    }

    /* --- search_sorted_matrix --- */
    printf("\nsearch_sorted_matrix:\n");
    {
        int m[MAX_N][MAX_N] = {
            {1,4,7,11},{2,5,8,12},{3,6,9,16},{10,13,14,17}
        };
        int r, c;
        int found = search_sorted_matrix(m, 4, 4, 5, &r, &c);
        TEST("find 5 in 4x4 sorted matrix") EXPECT(found && r == 1 && c == 1);
    }
    {
        int m[MAX_N][MAX_N] = {
            {1,4,7,11},{2,5,8,12},{3,6,9,16},{10,13,14,17}
        };
        int r, c;
        int found = search_sorted_matrix(m, 4, 4, 15, &r, &c);
        TEST("15 not in matrix") EXPECT(!found);
    }
    {
        int m[MAX_N][MAX_N] = {
            {1,4,7,11},{2,5,8,12},{3,6,9,16},{10,13,14,17}
        };
        int r, c;
        int found = search_sorted_matrix(m, 4, 4, 1, &r, &c);
        TEST("find 1 (top-left corner)") EXPECT(found && r == 0 && c == 0);
    }
    {
        int m[MAX_N][MAX_N] = {
            {1,4,7,11},{2,5,8,12},{3,6,9,16},{10,13,14,17}
        };
        int r, c;
        int found = search_sorted_matrix(m, 4, 4, 17, &r, &c);
        TEST("find 17 (bottom-right)") EXPECT(found && r == 3 && c == 3);
    }

    /* --- transpose --- */
    printf("\ntranspose:\n");
    {
        int m[MAX_N][MAX_N] = {{1,2,3},{4,5,6}};
        int out[MAX_N][MAX_N];
        int exp[MAX_N][MAX_N] = {{1,4},{2,5},{3,6}};
        transpose(m, 2, 3, out);
        TEST("2x3 → 3x2 transpose") EXPECT(mat_eq(out, exp, 3, 2));
    }
    {
        int m[MAX_N][MAX_N] = {{1,2},{3,4},{5,6}};
        int out[MAX_N][MAX_N];
        int exp[MAX_N][MAX_N] = {{1,3,5},{2,4,6}};
        transpose(m, 3, 2, out);
        TEST("3x2 → 2x3 transpose") EXPECT(mat_eq(out, exp, 2, 3));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
