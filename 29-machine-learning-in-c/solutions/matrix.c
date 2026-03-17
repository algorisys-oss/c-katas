/*
 * Solution: Matrix Math Library
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
    float *data;
    int rows;
    int cols;
} Matrix;

/* ---- Implementation ---- */

Matrix mat_create(int rows, int cols)
{
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = calloc(rows * cols, sizeof(float));
    return m;
}

void mat_destroy(Matrix *m)
{
    free(m->data);
    m->data = NULL;
}

void mat_set(Matrix *m, int row, int col, float val)
{
    m->data[row * m->cols + col] = val;
}

float mat_get(const Matrix *m, int row, int col)
{
    return m->data[row * m->cols + col];
}

void mat_multiply(const Matrix *a, const Matrix *b, Matrix *result)
{
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            float sum = 0.0f;
            for (int k = 0; k < a->cols; k++) {
                sum += mat_get(a, i, k) * mat_get(b, k, j);
            }
            mat_set(result, i, j, sum);
        }
    }
}

void mat_transpose(const Matrix *m, Matrix *result)
{
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            mat_set(result, j, i, mat_get(m, i, j));
        }
    }
}

void mat_add(const Matrix *a, const Matrix *b, Matrix *result)
{
    int n = a->rows * a->cols;
    for (int i = 0; i < n; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
}

void mat_scale(const Matrix *m, float scalar, Matrix *result)
{
    int n = m->rows * m->cols;
    for (int i = 0; i < n; i++) {
        result->data[i] = m->data[i] * scalar;
    }
}

/* ---- Tests ---- */

int main(void)
{
    printf("=== Matrix Math Library ===\n\n");

    /* Test 1 */
    TEST("mat_create allocates correct size")
        Matrix m = mat_create(2, 3);
        int ok = (m.data != NULL && m.rows == 2 && m.cols == 3);
        mat_destroy(&m);
        EXPECT(ok);

    /* Test 2 */
    TEST("mat_create initializes to zero")
        Matrix m = mat_create(2, 2);
        int all_zero = 1;
        for (int i = 0; i < 4; i++) {
            if (m.data[i] != 0.0f) all_zero = 0;
        }
        mat_destroy(&m);
        EXPECT(all_zero);

    /* Test 3 */
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

    /* Test 4 */
    TEST("mat_set uses row-major order")
        Matrix m = mat_create(3, 3);
        mat_set(&m, 2, 1, 42.0f);
        int ok = APPROX(m.data[7], 42.0f);
        mat_destroy(&m);
        EXPECT(ok);

    /* Test 5 */
    TEST("mat_multiply 2x3 * 3x2")
        Matrix a = mat_create(2, 3);
        Matrix b = mat_create(3, 2);
        Matrix c = mat_create(2, 2);
        mat_set(&a, 0, 0, 1); mat_set(&a, 0, 1, 2); mat_set(&a, 0, 2, 3);
        mat_set(&a, 1, 0, 4); mat_set(&a, 1, 1, 5); mat_set(&a, 1, 2, 6);
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

    /* Test 6 */
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

    /* Test 7 */
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

    /* Test 8 */
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

    /* Test 9 */
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

    /* Test 10 */
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

    /* Test 11 */
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

    /* Test 12 */
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

    /* Test 13 */
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

    /* Test 14 */
    TEST("mat_destroy sets data to NULL")
        Matrix m = mat_create(2, 2);
        mat_destroy(&m);
        EXPECT(m.data == NULL);

    printf("\n%d / %d tests passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
