/*
 * Exercise: Matrix Math Library
 *
 * Build the linear algebra foundation for machine learning.
 * Implement a Matrix struct and operations: create, destroy,
 * get/set, multiply, transpose, add, scale.
 *
 * 14 tests total.
 *
 * Hints:
 *   - Store matrix data in row-major order: index = row * cols + col
 *   - Matrix multiply: C[i][j] = sum of A[i][k] * B[k][j] for all k
 *   - Inner dimensions must match for multiply: (m×n) × (n×p) = (m×p)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---- Test Harness ---- */

static int tests_total = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

#define APPROX(a, b) (fabsf((a) - (b)) < 0.001f)

/* ---- Matrix Struct ---- */

typedef struct {
    float *data;    /* heap-allocated array of rows * cols floats */
    int rows;
    int cols;
} Matrix;

/* ---- TODO: Implement these functions ---- */

/*
 * mat_create: Allocate a matrix with the given dimensions.
 * Initialize all elements to 0.0.
 * Returns the Matrix struct (not a pointer).
 */
Matrix mat_create(int rows, int cols)
{
    /* TODO: allocate data with calloc, set rows/cols, return matrix */
    Matrix m = {NULL, 0, 0};
    return m;
}

/*
 * mat_destroy: Free the matrix's heap-allocated data.
 */
void mat_destroy(Matrix *m)
{
    /* TODO: free m->data, set pointer to NULL */
}

/*
 * mat_set: Set the element at (row, col) to val.
 * Row-major index: row * cols + col
 */
void mat_set(Matrix *m, int row, int col, float val)
{
    /* TODO */
}

/*
 * mat_get: Return the element at (row, col).
 */
float mat_get(const Matrix *m, int row, int col)
{
    /* TODO */
    return 0.0f;
}

/*
 * mat_multiply: Multiply a (m×n) by b (n×p), store in result (m×p).
 * Assumes result is already created with correct dimensions.
 *
 * result[i][j] = sum over k of a[i][k] * b[k][j]
 */
void mat_multiply(const Matrix *a, const Matrix *b, Matrix *result)
{
    /* TODO: triple nested loop — i over a->rows, j over b->cols, k over a->cols */
}

/*
 * mat_transpose: Transpose m (r×c) into result (c×r).
 * Assumes result is already created with correct dimensions.
 *
 * result[j][i] = m[i][j]
 */
void mat_transpose(const Matrix *m, Matrix *result)
{
    /* TODO: double nested loop, swap row/col indices */
}

/*
 * mat_add: Add matrices a and b element-wise, store in result.
 * All three matrices must have the same dimensions.
 */
void mat_add(const Matrix *a, const Matrix *b, Matrix *result)
{
    /* TODO: loop through all elements, result[i] = a[i] + b[i] */
}

/*
 * mat_scale: Multiply every element of m by scalar, store in result.
 * Both matrices must have the same dimensions.
 */
void mat_scale(const Matrix *m, float scalar, Matrix *result)
{
    /* TODO: loop through all elements, result[i] = m[i] * scalar */
}

/* ---- Tests ---- */

int main(void)
{
    printf("=== Matrix Math Library ===\n\n");

    /* Test 1: Create a matrix */
    TEST("mat_create allocates correct size")
        Matrix m = mat_create(2, 3);
        int ok = (m.data != NULL && m.rows == 2 && m.cols == 3);
        mat_destroy(&m);
        EXPECT(ok);

    /* Test 2: Elements initialized to zero */
    TEST("mat_create initializes to zero")
        Matrix m = mat_create(2, 2);
        int all_zero = 1;
        for (int i = 0; i < 4; i++) {
            if (m.data[i] != 0.0f) all_zero = 0;
        }
        mat_destroy(&m);
        EXPECT(all_zero);

    /* Test 3: Set and get */
    TEST("mat_set and mat_get")
        Matrix m = mat_create(2, 3);
        mat_set(&m, 0, 0, 1.0f);
        mat_set(&m, 0, 2, 3.0f);
        mat_set(&m, 1, 1, 5.0f);
        int ok = APPROX(mat_get(&m, 0, 0), 1.0f) &&
                 APPROX(mat_get(&m, 0, 2), 3.0f) &&
                 APPROX(mat_get(&m, 1, 1), 5.0f);
        mat_destroy(&m);
        EXPECT(ok);

    /* Test 4: Row-major indexing */
    TEST("mat_set uses row-major order")
        Matrix m = mat_create(3, 3);
        mat_set(&m, 2, 1, 42.0f);
        /* row 2, col 1 => index = 2*3 + 1 = 7 */
        int ok = APPROX(m.data[7], 42.0f);
        mat_destroy(&m);
        EXPECT(ok);

    /* Test 5: Matrix multiply 2x3 * 3x2 = 2x2 */
    TEST("mat_multiply 2x3 * 3x2")
        Matrix a = mat_create(2, 3);
        Matrix b = mat_create(3, 2);
        Matrix c = mat_create(2, 2);
        /* A = [[1,2,3],[4,5,6]] */
        mat_set(&a, 0, 0, 1); mat_set(&a, 0, 1, 2); mat_set(&a, 0, 2, 3);
        mat_set(&a, 1, 0, 4); mat_set(&a, 1, 1, 5); mat_set(&a, 1, 2, 6);
        /* B = [[7,8],[9,10],[11,12]] */
        mat_set(&b, 0, 0, 7);  mat_set(&b, 0, 1, 8);
        mat_set(&b, 1, 0, 9);  mat_set(&b, 1, 1, 10);
        mat_set(&b, 2, 0, 11); mat_set(&b, 2, 1, 12);
        mat_multiply(&a, &b, &c);
        int ok = APPROX(mat_get(&c, 0, 0), 58.0f) &&
                 APPROX(mat_get(&c, 0, 1), 64.0f) &&
                 APPROX(mat_get(&c, 1, 0), 139.0f) &&
                 APPROX(mat_get(&c, 1, 1), 154.0f);
        mat_destroy(&a); mat_destroy(&b); mat_destroy(&c);
        EXPECT(ok);

    /* Test 6: Multiply by identity */
    TEST("mat_multiply by identity matrix")
        Matrix a = mat_create(2, 2);
        Matrix id = mat_create(2, 2);
        Matrix c = mat_create(2, 2);
        mat_set(&a, 0, 0, 3); mat_set(&a, 0, 1, 7);
        mat_set(&a, 1, 0, 2); mat_set(&a, 1, 1, 5);
        mat_set(&id, 0, 0, 1); mat_set(&id, 1, 1, 1);
        mat_multiply(&a, &id, &c);
        int ok = APPROX(mat_get(&c, 0, 0), 3.0f) &&
                 APPROX(mat_get(&c, 0, 1), 7.0f) &&
                 APPROX(mat_get(&c, 1, 0), 2.0f) &&
                 APPROX(mat_get(&c, 1, 1), 5.0f);
        mat_destroy(&a); mat_destroy(&id); mat_destroy(&c);
        EXPECT(ok);

    /* Test 7: Multiply 1x3 * 3x1 = 1x1 (dot product) */
    TEST("mat_multiply as dot product 1x3 * 3x1")
        Matrix a = mat_create(1, 3);
        Matrix b = mat_create(3, 1);
        Matrix c = mat_create(1, 1);
        mat_set(&a, 0, 0, 1); mat_set(&a, 0, 1, 2); mat_set(&a, 0, 2, 3);
        mat_set(&b, 0, 0, 4); mat_set(&b, 1, 0, 5); mat_set(&b, 2, 0, 6);
        mat_multiply(&a, &b, &c);
        int ok = APPROX(mat_get(&c, 0, 0), 32.0f);
        mat_destroy(&a); mat_destroy(&b); mat_destroy(&c);
        EXPECT(ok);

    /* Test 8: Transpose 2x3 -> 3x2 */
    TEST("mat_transpose 2x3 -> 3x2")
        Matrix m = mat_create(2, 3);
        Matrix t = mat_create(3, 2);
        mat_set(&m, 0, 0, 1); mat_set(&m, 0, 1, 2); mat_set(&m, 0, 2, 3);
        mat_set(&m, 1, 0, 4); mat_set(&m, 1, 1, 5); mat_set(&m, 1, 2, 6);
        mat_transpose(&m, &t);
        int ok = APPROX(mat_get(&t, 0, 0), 1.0f) &&
                 APPROX(mat_get(&t, 1, 0), 2.0f) &&
                 APPROX(mat_get(&t, 2, 0), 3.0f) &&
                 APPROX(mat_get(&t, 0, 1), 4.0f) &&
                 APPROX(mat_get(&t, 1, 1), 5.0f) &&
                 APPROX(mat_get(&t, 2, 1), 6.0f);
        mat_destroy(&m); mat_destroy(&t);
        EXPECT(ok);

    /* Test 9: Transpose 1x4 -> 4x1 */
    TEST("mat_transpose 1x4 -> 4x1")
        Matrix m = mat_create(1, 4);
        Matrix t = mat_create(4, 1);
        mat_set(&m, 0, 0, 10); mat_set(&m, 0, 1, 20);
        mat_set(&m, 0, 2, 30); mat_set(&m, 0, 3, 40);
        mat_transpose(&m, &t);
        int ok = APPROX(mat_get(&t, 0, 0), 10.0f) &&
                 APPROX(mat_get(&t, 1, 0), 20.0f) &&
                 APPROX(mat_get(&t, 2, 0), 30.0f) &&
                 APPROX(mat_get(&t, 3, 0), 40.0f);
        mat_destroy(&m); mat_destroy(&t);
        EXPECT(ok);

    /* Test 10: Add two matrices */
    TEST("mat_add element-wise")
        Matrix a = mat_create(2, 2);
        Matrix b = mat_create(2, 2);
        Matrix c = mat_create(2, 2);
        mat_set(&a, 0, 0, 1); mat_set(&a, 0, 1, 2);
        mat_set(&a, 1, 0, 3); mat_set(&a, 1, 1, 4);
        mat_set(&b, 0, 0, 10); mat_set(&b, 0, 1, 20);
        mat_set(&b, 1, 0, 30); mat_set(&b, 1, 1, 40);
        mat_add(&a, &b, &c);
        int ok = APPROX(mat_get(&c, 0, 0), 11.0f) &&
                 APPROX(mat_get(&c, 0, 1), 22.0f) &&
                 APPROX(mat_get(&c, 1, 0), 33.0f) &&
                 APPROX(mat_get(&c, 1, 1), 44.0f);
        mat_destroy(&a); mat_destroy(&b); mat_destroy(&c);
        EXPECT(ok);

    /* Test 11: Add with negative values */
    TEST("mat_add with negatives")
        Matrix a = mat_create(1, 3);
        Matrix b = mat_create(1, 3);
        Matrix c = mat_create(1, 3);
        mat_set(&a, 0, 0, 5);  mat_set(&a, 0, 1, -3); mat_set(&a, 0, 2, 0);
        mat_set(&b, 0, 0, -2); mat_set(&b, 0, 1, 3);  mat_set(&b, 0, 2, 7);
        mat_add(&a, &b, &c);
        int ok = APPROX(mat_get(&c, 0, 0), 3.0f) &&
                 APPROX(mat_get(&c, 0, 1), 0.0f) &&
                 APPROX(mat_get(&c, 0, 2), 7.0f);
        mat_destroy(&a); mat_destroy(&b); mat_destroy(&c);
        EXPECT(ok);

    /* Test 12: Scale a matrix */
    TEST("mat_scale by 3.0")
        Matrix m = mat_create(2, 2);
        Matrix r = mat_create(2, 2);
        mat_set(&m, 0, 0, 1); mat_set(&m, 0, 1, 2);
        mat_set(&m, 1, 0, 3); mat_set(&m, 1, 1, 4);
        mat_scale(&m, 3.0f, &r);
        int ok = APPROX(mat_get(&r, 0, 0), 3.0f) &&
                 APPROX(mat_get(&r, 0, 1), 6.0f) &&
                 APPROX(mat_get(&r, 1, 0), 9.0f) &&
                 APPROX(mat_get(&r, 1, 1), 12.0f);
        mat_destroy(&m); mat_destroy(&r);
        EXPECT(ok);

    /* Test 13: Scale by zero */
    TEST("mat_scale by 0.0")
        Matrix m = mat_create(2, 2);
        Matrix r = mat_create(2, 2);
        mat_set(&m, 0, 0, 5); mat_set(&m, 0, 1, 10);
        mat_set(&m, 1, 0, 15); mat_set(&m, 1, 1, 20);
        mat_scale(&m, 0.0f, &r);
        int ok = APPROX(mat_get(&r, 0, 0), 0.0f) &&
                 APPROX(mat_get(&r, 0, 1), 0.0f) &&
                 APPROX(mat_get(&r, 1, 0), 0.0f) &&
                 APPROX(mat_get(&r, 1, 1), 0.0f);
        mat_destroy(&m); mat_destroy(&r);
        EXPECT(ok);

    /* Test 14: Destroy sets data to NULL */
    TEST("mat_destroy sets data to NULL")
        Matrix m = mat_create(2, 2);
        mat_destroy(&m);
        EXPECT(m.data == NULL);

    /* ---- Summary ---- */
    printf("\n%d / %d tests passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
