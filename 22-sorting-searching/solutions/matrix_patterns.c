/*
 * Solution: Matrix / 2D Array Interview Patterns
 *
 * Five exercises demonstrating classic matrix manipulation techniques.
 *
 * 17 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_N 64

/* ── Spiral Order ─────────────────────────────────────────────────── */
int spiral_order(int matrix[MAX_N][MAX_N], int rows, int cols,
                 int *out)
{
    int top = 0, bottom = rows - 1;
    int left = 0, right = cols - 1;
    int idx = 0;

    while (top <= bottom && left <= right) {
        /* Sweep right along top row */
        for (int j = left; j <= right; j++) {
            out[idx++] = matrix[top][j];
        }
        top++;

        /* Sweep down along right column */
        for (int i = top; i <= bottom; i++) {
            out[idx++] = matrix[i][right];
        }
        right--;

        /* Sweep left along bottom row (if rows remain) */
        if (top <= bottom) {
            for (int j = right; j >= left; j--) {
                out[idx++] = matrix[bottom][j];
            }
            bottom--;
        }

        /* Sweep up along left column (if columns remain) */
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                out[idx++] = matrix[i][left];
            }
            left++;
        }
    }

    return idx;
}

/* ── Rotate 90 Degrees Clockwise ──────────────────────────────────── */
void rotate_90(int matrix[MAX_N][MAX_N], int n)
{
    /* Step 1: Transpose — mirror across main diagonal */
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int tmp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = tmp;
        }
    }

    /* Step 2: Reverse each row — mirror left-to-right */
    for (int i = 0; i < n; i++) {
        int left = 0, right = n - 1;
        while (left < right) {
            int tmp = matrix[i][left];
            matrix[i][left] = matrix[i][right];
            matrix[i][right] = tmp;
            left++;
            right--;
        }
    }
}

/* ── Set Matrix Zeroes ────────────────────────────────────────────── */
void set_matrix_zeroes(int matrix[MAX_N][MAX_N], int rows, int cols)
{
    /* Save whether first row and first column contain any zeros */
    int first_row_zero = 0, first_col_zero = 0;

    for (int j = 0; j < cols; j++) {
        if (matrix[0][j] == 0) {
            first_row_zero = 1;
            break;
        }
    }
    for (int i = 0; i < rows; i++) {
        if (matrix[i][0] == 0) {
            first_col_zero = 1;
            break;
        }
    }

    /* Use first row/col as markers for the rest of the matrix */
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            if (matrix[i][j] == 0) {
                matrix[i][0] = 0;  /* Mark this row */
                matrix[0][j] = 0;  /* Mark this column */
            }
        }
    }

    /* Zero out marked rows */
    for (int i = 1; i < rows; i++) {
        if (matrix[i][0] == 0) {
            for (int j = 1; j < cols; j++) {
                matrix[i][j] = 0;
            }
        }
    }

    /* Zero out marked columns */
    for (int j = 1; j < cols; j++) {
        if (matrix[0][j] == 0) {
            for (int i = 1; i < rows; i++) {
                matrix[i][j] = 0;
            }
        }
    }

    /* Finally, zero out first row/col if they originally had zeros */
    if (first_row_zero) {
        for (int j = 0; j < cols; j++) {
            matrix[0][j] = 0;
        }
    }
    if (first_col_zero) {
        for (int i = 0; i < rows; i++) {
            matrix[i][0] = 0;
        }
    }
}

/* ── Search Sorted Matrix ─────────────────────────────────────────── */
int search_sorted_matrix(int matrix[MAX_N][MAX_N], int rows, int cols,
                         int target, int *row, int *col)
{
    /* Start at top-right corner */
    int r = 0, c = cols - 1;

    while (r < rows && c >= 0) {
        if (matrix[r][c] == target) {
            *row = r;
            *col = c;
            return 1;
        } else if (matrix[r][c] > target) {
            c--;  /* Too big — eliminate this column */
        } else {
            r++;  /* Too small — eliminate this row */
        }
    }

    return 0;
}

/* ── Transpose ────────────────────────────────────────────────────── */
void transpose(int matrix[MAX_N][MAX_N], int rows, int cols,
               int out[MAX_N][MAX_N])
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            out[j][i] = matrix[i][j];
        }
    }
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
