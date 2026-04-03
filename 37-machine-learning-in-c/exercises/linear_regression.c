/*
 * Exercise: Linear Regression with Gradient Descent
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
 *
 * You'll implement two ways to compute gradients:
 *   1. Numerical: nudge a parameter, see how loss changes (no formulas!)
 *   2. Analytical: use the actual math formula (faster, same answer)
 *
 * Hints:
 *   - MSE = (1/n) * sum((predicted - actual)^2)
 *   - Numerical gradient: change w by a tiny amount, see how loss changes
 *     dw = (loss_after_nudge - loss_before) / nudge_amount
 *   - Analytical gradient for w: (2/n) * sum((predicted - actual) * x)
 *   - Analytical gradient for b: (2/n) * sum(predicted - actual)
 *   - Update rule: w = w - learning_rate * dw  (subtract because we
 *     want to go DOWNHILL on the loss landscape)
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

/*
 * A single (x, y) data point.
 * Think of it as one row in a spreadsheet: x is the input, y is the output.
 */
typedef struct {
    double x;
    double y;
} DataPoint;

/*
 * Our model: y = w * x + b
 *   w = weight (slope of the line)
 *   b = bias (y-intercept)
 *
 * Training means finding the best w and b to fit the data.
 */
typedef struct {
    double w;   /* weight (slope) */
    double b;   /* bias (y-intercept) */
} LinearModel;

/* ---- Constants ---- */

#define NUDGE 0.0001  /* tiny amount for numerical gradient */

/* ---- TODO: Implement these functions ---- */

/*
 * predict: Compute the model's prediction for input x.
 *   y_predicted = w * x + b
 *
 * This is the simplest possible "neural network" — one multiplication
 * and one addition. Every neural network is built from layers of these.
 */
double predict(const LinearModel *model, double x)
{
    /* TODO: return w * x + b */
    return 0.0;
}

/*
 * compute_loss: Compute Mean Squared Error over all data points.
 *
 *   MSE = (1/n) * sum over all points of (predicted - actual)^2
 *
 * Why squared? Because:
 *   - It makes all errors positive (no cancellation)
 *   - It penalizes big errors more than small ones
 *   - It's smooth (differentiable), which gradient descent needs
 *
 * Parameters:
 *   model - the current model (w and b)
 *   data  - array of DataPoint
 *   n     - number of data points
 *
 * Returns: the MSE (a single number; lower is better, 0 is perfect)
 */
double compute_loss(const LinearModel *model, const DataPoint data[], int n)
{
    /* TODO:
     * 1. Loop over all n data points
     * 2. For each point, compute: error = predict(model, data[i].x) - data[i].y
     * 3. Add error * error to a running sum
     * 4. Divide sum by n and return
     */
    return 0.0;
}

/*
 * compute_gradient_numerical: Compute gradients using the "nudge" method.
 *
 * This is the most intuitive way to understand derivatives:
 *   "If I wiggle w by a tiny amount, how much does the loss change?"
 *
 * Algorithm:
 *   1. Compute the current loss
 *   2. Nudge w by NUDGE (add 0.0001 to w)
 *   3. Compute the new loss
 *   4. dw = (new_loss - old_loss) / NUDGE
 *   5. Un-nudge w (put it back!)
 *   6. Repeat steps 2-5 for b to get db
 *
 * Parameters:
 *   model - the current model (w and b) — do NOT permanently modify it!
 *   data  - array of DataPoint
 *   n     - number of data points
 *   dw    - OUTPUT: pointer to store the gradient for w
 *   db    - OUTPUT: pointer to store the gradient for b
 *
 * IMPORTANT: You need to temporarily modify model->w and model->b
 * to compute the nudged loss, then restore the original values.
 * Cast away const or make a copy — the model should be unchanged after.
 */
void compute_gradient_numerical(const LinearModel *model,
                                const DataPoint data[], int n,
                                double *dw, double *db)
{
    /* TODO:
     * 1. Compute current loss
     * 2. Make a mutable copy of the model
     * 3. Nudge copy.w by NUDGE, compute loss, calculate dw
     * 4. Restore copy.w, nudge copy.b by NUDGE, compute loss, calculate db
     */
    *dw = 0.0;
    *db = 0.0;
}

/*
 * compute_gradient_analytical: Compute gradients using the math formulas.
 *
 * For MSE loss with y = w*x + b:
 *   dw = (2/n) * sum over all points of ((predicted - actual) * x)
 *   db = (2/n) * sum over all points of (predicted - actual)
 *
 * Why does this work?
 *   - The derivative of (predicted - actual)^2 with respect to w
 *     is 2 * (predicted - actual) * x  (chain rule)
 *   - The derivative with respect to b is 2 * (predicted - actual) * 1
 *
 * This gives the SAME answer as the numerical method, but faster
 * (one pass through the data instead of three).
 *
 * Parameters:
 *   model - the current model
 *   data  - array of DataPoint
 *   n     - number of data points
 *   dw    - OUTPUT: pointer to store the gradient for w
 *   db    - OUTPUT: pointer to store the gradient for b
 */
void compute_gradient_analytical(const LinearModel *model,
                                 const DataPoint data[], int n,
                                 double *dw, double *db)
{
    /* TODO:
     * 1. Initialize sum_dw = 0, sum_db = 0
     * 2. For each data point:
     *    predicted = predict(model, data[i].x)
     *    error = predicted - data[i].y
     *    sum_dw += error * data[i].x
     *    sum_db += error
     * 3. *dw = (2.0 / n) * sum_dw
     *    *db = (2.0 / n) * sum_db
     */
    *dw = 0.0;
    *db = 0.0;
}

/*
 * train: Run gradient descent for a number of epochs.
 *
 * Each epoch:
 *   1. Compute gradients (use the analytical method — it's faster)
 *   2. Update w: model->w -= learning_rate * dw
 *   3. Update b: model->b -= learning_rate * db
 *
 * The minus sign is crucial: gradients point UPHILL (direction of
 * steepest increase), but we want to go DOWNHILL (decrease loss).
 *
 * Parameters:
 *   model         - the model to train (modified in place)
 *   data          - training data
 *   n             - number of data points
 *   learning_rate - step size (too big = overshoot, too small = slow)
 *   epochs        - number of training iterations
 *
 * Returns: the final loss after all epochs
 */
double train(LinearModel *model, const DataPoint data[], int n,
             double learning_rate, int epochs)
{
    /* TODO:
     * 1. Loop for 'epochs' iterations
     * 2. Each iteration: compute gradients, update w and b
     * 3. After the loop, return compute_loss(model, data, n)
     */
    return 0.0;
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
        LinearModel m = {2.0, 1.0};  /* y = 2x + 1 fits perfectly */
        EXPECT(APPROX(compute_loss(&m, perfect, 3), 0.0));

    /* Test 4: compute_loss with known errors returns correct MSE */
    TEST("loss with known errors")
        /* model: y = 0*x + 0 = 0 for all x */
        /* data: (1,1), (2,2), (3,3) */
        /* errors: 1, 2, 3 -> squared: 1, 4, 9 -> MSE = 14/3 = 4.6667 */
        DataPoint simple[] = {{1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}};
        LinearModel m = {0.0, 0.0};
        EXPECT(APPROX(compute_loss(&m, simple, 3), 14.0 / 3.0));

    /* Test 5: numerical gradient for w is approximately correct */
    TEST("numerical gradient dw is reasonable")
        LinearModel m = {0.0, 0.0};
        double dw, db;
        compute_gradient_numerical(&m, data, n, &dw, &db);
        /* With w=0, b=0: predictions are all 0, errors are negative,
         * so gradient for w should be negative (we need to increase w) */
        EXPECT(dw < -1.0);

    /* Test 6: numerical gradient for b is approximately correct */
    TEST("numerical gradient db is reasonable")
        LinearModel m = {0.0, 0.0};
        double dw, db;
        compute_gradient_numerical(&m, data, n, &dw, &db);
        /* Similarly, db should be negative */
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
