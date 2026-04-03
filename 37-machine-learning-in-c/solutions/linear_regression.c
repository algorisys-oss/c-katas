/*
 * Solution: Linear Regression with Gradient Descent
 *
 * Train a simple y = w*x + b model using gradient descent.
 * This bridges the gap between matrix math and neural networks:
 * you'll see how a model "learns" by adjusting its parameters
 * to minimize error, step by step.
 *
 * 12 tests total.
 *
 * Key ideas:
 *   - A "model" is just numbers (w and b) that we tweak
 *   - "Loss" measures how wrong the model is (Mean Squared Error)
 *   - "Gradient" tells us which direction to tweak w and b
 *   - "Training" = repeatedly compute gradient, then nudge w and b
 */

#include <stdio.h>
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

#define APPROX(a, b) (fabs((a) - (b)) < 0.001)
#define APPROX_LOOSE(a, b) (fabs((a) - (b)) < 0.5)

/* ---- Data and Model Structs ---- */

typedef struct {
    double x;
    double y;
} DataPoint;

typedef struct {
    double w;   /* weight (slope) */
    double b;   /* bias (y-intercept) */
} LinearModel;

/* ---- Constants ---- */

#define NUDGE 0.0001

/* ---- Implementations ---- */

double predict(const LinearModel *model, double x)
{
    return model->w * x + model->b;
}

double compute_loss(const LinearModel *model, const DataPoint data[], int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double error = predict(model, data[i].x) - data[i].y;
        sum += error * error;
    }
    return sum / n;
}

void compute_gradient_numerical(const LinearModel *model,
                                const DataPoint data[], int n,
                                double *dw, double *db)
{
    double loss_before = compute_loss(model, data, n);

    /* Nudge w */
    LinearModel copy = *model;
    copy.w += NUDGE;
    double loss_after_w = compute_loss(&copy, data, n);
    *dw = (loss_after_w - loss_before) / NUDGE;

    /* Nudge b */
    copy = *model;
    copy.b += NUDGE;
    double loss_after_b = compute_loss(&copy, data, n);
    *db = (loss_after_b - loss_before) / NUDGE;
}

void compute_gradient_analytical(const LinearModel *model,
                                 const DataPoint data[], int n,
                                 double *dw, double *db)
{
    double sum_dw = 0.0;
    double sum_db = 0.0;
    for (int i = 0; i < n; i++) {
        double error = predict(model, data[i].x) - data[i].y;
        sum_dw += error * data[i].x;
        sum_db += error;
    }
    *dw = (2.0 / n) * sum_dw;
    *db = (2.0 / n) * sum_db;
}

double train(LinearModel *model, const DataPoint data[], int n,
             double learning_rate, int epochs)
{
    for (int e = 0; e < epochs; e++) {
        double dw, db;
        compute_gradient_analytical(model, data, n, &dw, &db);
        model->w -= learning_rate * dw;
        model->b -= learning_rate * db;
    }
    return compute_loss(model, data, n);
}

/* ---- Tests ---- */

int main(void)
{
    printf("=== Linear Regression with Gradient Descent ===\n\n");

    /* Training data: roughly y = 2x + 0 with a bit of noise */
    DataPoint data[] = {
        {1.0, 2.1}, {2.0, 3.9}, {3.0, 6.2}, {4.0, 7.8}, {5.0, 10.1}
    };
    int n = 5;

    /* Test 1: predict with w=2, b=1, x=3 returns 7 */
    TEST("predict: w=2, b=1, x=3 -> 7")
        LinearModel m = {2.0, 1.0};
        EXPECT(APPROX(predict(&m, 3.0), 7.0));

    /* Test 2: predict with w=0, b=0, x=5 returns 0 */
    TEST("predict: w=0, b=0, x=5 -> 0")
        LinearModel m = {0.0, 0.0};
        EXPECT(APPROX(predict(&m, 5.0), 0.0));

    /* Test 3: compute_loss with perfect predictions returns 0 */
    TEST("loss is 0 with perfect model")
        DataPoint perfect[] = {{1.0, 3.0}, {2.0, 5.0}, {3.0, 7.0}};
        LinearModel m = {2.0, 1.0};
        EXPECT(APPROX(compute_loss(&m, perfect, 3), 0.0));

    /* Test 4: compute_loss with known errors returns correct MSE */
    TEST("loss with known errors")
        DataPoint simple[] = {{1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}};
        LinearModel m = {0.0, 0.0};
        EXPECT(APPROX(compute_loss(&m, simple, 3), 14.0 / 3.0));

    /* Test 5: numerical gradient for w is approximately correct */
    TEST("numerical gradient dw is reasonable")
        LinearModel m = {0.0, 0.0};
        double dw, db;
        compute_gradient_numerical(&m, data, n, &dw, &db);
        EXPECT(dw < -1.0);

    /* Test 6: numerical gradient for b is approximately correct */
    TEST("numerical gradient db is reasonable")
        LinearModel m = {0.0, 0.0};
        double dw, db;
        compute_gradient_numerical(&m, data, n, &dw, &db);
        EXPECT(db < -1.0);

    /* Test 7: analytical gradient for w matches numerical */
    TEST("analytical dw matches numerical dw")
        LinearModel m = {1.0, 0.5};
        double num_dw, num_db, ana_dw, ana_db;
        compute_gradient_numerical(&m, data, n, &num_dw, &num_db);
        compute_gradient_analytical(&m, data, n, &ana_dw, &ana_db);
        EXPECT(fabs(ana_dw - num_dw) < 0.01);

    /* Test 8: analytical gradient for b matches numerical */
    TEST("analytical db matches numerical db")
        LinearModel m = {1.0, 0.5};
        double num_dw, num_db, ana_dw, ana_db;
        compute_gradient_numerical(&m, data, n, &num_dw, &num_db);
        compute_gradient_analytical(&m, data, n, &ana_dw, &ana_db);
        EXPECT(fabs(ana_db - num_db) < 0.01);

    /* Test 9: training with 1000 epochs gets loss < 0.1 */
    TEST("training 1000 epochs -> loss < 0.1")
        LinearModel m = {0.0, 0.0};
        double loss = train(&m, data, n, 0.01, 1000);
        printf("(loss=%.4f) ", loss);
        EXPECT(loss < 0.1);

    /* Test 10: trained model w is close to 2 */
    TEST("trained w is close to 2")
        LinearModel m = {0.0, 0.0};
        train(&m, data, n, 0.01, 1000);
        printf("(w=%.3f) ", m.w);
        EXPECT(APPROX_LOOSE(m.w, 2.0));

    /* Test 11: trained model b is close to 0 */
    TEST("trained b is close to 0")
        LinearModel m = {0.0, 0.0};
        train(&m, data, n, 0.01, 1000);
        printf("(b=%.3f) ", m.b);
        EXPECT(APPROX_LOOSE(m.b, 0.0));

    /* Test 12: more epochs gets lower loss */
    TEST("more epochs -> lower loss")
        LinearModel m1 = {0.0, 0.0};
        LinearModel m2 = {0.0, 0.0};
        double loss_500 = train(&m1, data, n, 0.01, 500);
        double loss_2000 = train(&m2, data, n, 0.01, 2000);
        EXPECT(loss_2000 < loss_500);

    /* ---- Summary ---- */
    printf("\n%d / %d tests passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
