/*
 * Exercise: Simple Neural Network
 *
 * Build a single-hidden-layer neural network from scratch.
 * Implement the sigmoid activation, forward pass, and train
 * a network to learn XOR using backpropagation.
 *
 * 10 tests total.
 *
 * Architecture for XOR:
 *   Input (2) -> Hidden (2) -> Output (1)
 *
 * Hints:
 *   - sigmoid(x) = 1.0 / (1.0 + exp(-x))
 *   - sigmoid_derivative(output) = output * (1 - output)
 *     (where output is the already-sigmoid'd value)
 *   - Forward pass: for each layer, compute weighted sum + bias, then sigmoid
 *   - Backprop: compute output error, then propagate back to hidden layer
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

#define APPROX(a, b) (fabs((a) - (b)) < 0.05)

/* ---- Constants ---- */

#define INPUT_SIZE  2
#define HIDDEN_SIZE 2
#define OUTPUT_SIZE 1
#define LEARNING_RATE 2.0
#define MAX_EPOCHS 10000

/* ---- TODO: Implement these functions ---- */

/*
 * sigmoid: Compute the sigmoid activation function.
 *   sigmoid(x) = 1.0 / (1.0 + exp(-x))
 *
 * Maps any real number to (0, 1).
 * Used as the activation function for each neuron.
 */
double sigmoid(double x)
{
    /* TODO */
    return 0.0;
}

/*
 * sigmoid_derivative: Compute the derivative of sigmoid.
 *   sigmoid'(x) = x * (1.0 - x)
 *
 * NOTE: x here is the OUTPUT of sigmoid (already activated),
 * not the raw input. This is a convenient shortcut.
 */
double sigmoid_derivative(double x)
{
    /* TODO */
    return 0.0;
}

/*
 * nn_forward: Compute the forward pass of a single-hidden-layer network.
 *
 * Parameters:
 *   inputs[]     - input values (length: input_size)
 *   weights_ih[] - weights from input to hidden (length: input_size * hidden_size)
 *                  weight from input i to hidden j = weights_ih[i * hidden_size + j]
 *   weights_ho[] - weights from hidden to output (length: hidden_size * output_size)
 *                  weight from hidden j to output k = weights_ho[j * output_size + k]
 *   bias_h[]     - biases for hidden neurons (length: hidden_size)
 *   bias_o[]     - biases for output neurons (length: output_size)
 *   hidden[]     - OUTPUT: hidden layer activations (length: hidden_size)
 *   output[]     - OUTPUT: final output activations (length: output_size)
 *   input_size, hidden_size, output_size - layer sizes
 *
 * Algorithm:
 *   1. For each hidden neuron j:
 *      sum = bias_h[j]
 *      for each input i: sum += inputs[i] * weights_ih[i * hidden_size + j]
 *      hidden[j] = sigmoid(sum)
 *
 *   2. For each output neuron k:
 *      sum = bias_o[k]
 *      for each hidden j: sum += hidden[j] * weights_ho[j * output_size + k]
 *      output[k] = sigmoid(sum)
 */
void nn_forward(const double inputs[], const double weights_ih[],
                const double weights_ho[], const double bias_h[],
                const double bias_o[], double hidden[], double output[],
                int input_size, int hidden_size, int output_size)
{
    /* TODO: Compute hidden layer activations */

    /* TODO: Compute output layer activations */
}

/*
 * nn_train_xor: Train a 2-2-1 network to learn XOR.
 *
 * XOR truth table:
 *   (0,0) -> 0
 *   (0,1) -> 1
 *   (1,0) -> 1
 *   (1,1) -> 0
 *
 * Returns the final loss (MSE) after training.
 *
 * Algorithm:
 *   1. Initialize weights to the provided values (deterministic for testing)
 *   2. For each epoch:
 *      a. For each training sample:
 *         - Forward pass
 *         - Compute output error: output_delta = (target - output) * sigmoid'(output)
 *         - Compute hidden error: for each hidden neuron j,
 *           hidden_delta[j] = (output_delta * weight_ho[j]) * sigmoid'(hidden[j])
 *         - Update weights_ho: w += learning_rate * output_delta * hidden[j]
 *         - Update bias_o: b += learning_rate * output_delta
 *         - Update weights_ih: w += learning_rate * hidden_delta[j] * input[i]
 *         - Update bias_h: b += learning_rate * hidden_delta[j]
 *   3. Return MSE over all 4 samples
 *
 * Use these initial weights (for deterministic testing):
 *   weights_ih = {0.15, 0.25, 0.20, 0.30}
 *   weights_ho = {0.40, 0.50}
 *   bias_h = {0.35, 0.35}
 *   bias_o = {0.60}
 */
double nn_train_xor(void)
{
    /* Training data: XOR */
    double train_inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double train_targets[4] = {0, 1, 1, 0};

    /* Initial weights (deterministic) */
    double weights_ih[4] = {0.15, 0.25, 0.20, 0.30};
    double weights_ho[2] = {0.40, 0.50};
    double bias_h[2] = {0.35, 0.35};
    double bias_o[1] = {0.60};

    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];

    /* TODO: Training loop
     *
     * For MAX_EPOCHS iterations:
     *   For each of the 4 training samples:
     *     1. Forward pass (use nn_forward)
     *     2. Compute output error delta:
     *        output_delta = (target - output[0]) * sigmoid_derivative(output[0])
     *     3. Compute hidden error deltas:
     *        hidden_delta[j] = output_delta * weights_ho[j] * sigmoid_derivative(hidden[j])
     *     4. Update weights_ho[j] += LEARNING_RATE * output_delta * hidden[j]
     *     5. Update bias_o[0] += LEARNING_RATE * output_delta
     *     6. Update weights_ih[i * HIDDEN_SIZE + j] += LEARNING_RATE * hidden_delta[j] * input[i]
     *     7. Update bias_h[j] += LEARNING_RATE * hidden_delta[j]
     */

    /* Compute final MSE */
    double mse = 0.0;
    for (int s = 0; s < 4; s++) {
        nn_forward(train_inputs[s], weights_ih, weights_ho,
                   bias_h, bias_o, hidden, output,
                   INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
        double err = train_targets[s] - output[0];
        mse += err * err;
    }
    mse /= 4.0;
    return mse;
}

/* ---- Tests ---- */

int main(void)
{
    printf("=== Neural Network ===\n\n");

    /* Test 1: sigmoid(0) = 0.5 */
    TEST("sigmoid(0) = 0.5")
        EXPECT(APPROX(sigmoid(0.0), 0.5));

    /* Test 2: sigmoid(large positive) near 1.0 */
    TEST("sigmoid(10) near 1.0")
        EXPECT(sigmoid(10.0) > 0.999);

    /* Test 3: sigmoid(large negative) near 0.0 */
    TEST("sigmoid(-10) near 0.0")
        EXPECT(sigmoid(-10.0) < 0.001);

    /* Test 4: sigmoid_derivative at 0.5 */
    TEST("sigmoid_derivative(0.5) = 0.25")
        EXPECT(APPROX(sigmoid_derivative(0.5), 0.25));

    /* Test 5: sigmoid_derivative at 0 and 1 */
    TEST("sigmoid_derivative(0) = 0, sigmoid_derivative(1) = 0")
        EXPECT(APPROX(sigmoid_derivative(0.0), 0.0) &&
               APPROX(sigmoid_derivative(1.0), 0.0));

    /* Test 6: Forward pass with known weights */
    TEST("nn_forward with known weights")
        double inputs[2] = {1.0, 0.5};
        double wih[4] = {0.1, 0.2, 0.3, 0.4};
        double who[2] = {0.5, 0.6};
        double bh[2] = {0.0, 0.0};
        double bo[1] = {0.0};
        double hidden[2], output[1];
        nn_forward(inputs, wih, who, bh, bo, hidden, output, 2, 2, 1);
        /* hidden[0] = sigmoid(1.0*0.1 + 0.5*0.3) = sigmoid(0.25) */
        /* hidden[1] = sigmoid(1.0*0.2 + 0.5*0.4) = sigmoid(0.40) */
        double exp_h0 = 1.0 / (1.0 + exp(-0.25));
        double exp_h1 = 1.0 / (1.0 + exp(-0.40));
        EXPECT(APPROX(hidden[0], exp_h0) && APPROX(hidden[1], exp_h1));

    /* Test 7: Forward pass output computation */
    TEST("nn_forward output layer correct")
        double inputs[2] = {1.0, 0.5};
        double wih[4] = {0.1, 0.2, 0.3, 0.4};
        double who[2] = {0.5, 0.6};
        double bh[2] = {0.0, 0.0};
        double bo[1] = {0.0};
        double hidden[2], output[1];
        nn_forward(inputs, wih, who, bh, bo, hidden, output, 2, 2, 1);
        double h0 = 1.0 / (1.0 + exp(-0.25));
        double h1 = 1.0 / (1.0 + exp(-0.40));
        double exp_out = 1.0 / (1.0 + exp(-(h0 * 0.5 + h1 * 0.6)));
        EXPECT(APPROX(output[0], exp_out));

    /* Test 8: Forward pass with biases */
    TEST("nn_forward respects biases")
        double inputs[2] = {0.0, 0.0};
        double wih[4] = {0.1, 0.2, 0.3, 0.4};
        double who[2] = {0.5, 0.6};
        double bh[2] = {1.0, -1.0};
        double bo[1] = {0.5};
        double hidden[2], output[1];
        nn_forward(inputs, wih, who, bh, bo, hidden, output, 2, 2, 1);
        /* With zero inputs, hidden[j] = sigmoid(bias_h[j]) */
        double exp_h0 = 1.0 / (1.0 + exp(-1.0));
        double exp_h1 = 1.0 / (1.0 + exp(1.0));
        EXPECT(APPROX(hidden[0], exp_h0) && APPROX(hidden[1], exp_h1));

    /* Test 9: XOR training converges (loss < 0.01) */
    TEST("nn_train_xor converges (loss < 0.01)")
        double loss = nn_train_xor();
        printf("(loss=%.4f) ", loss);
        EXPECT(loss < 0.01);

    /* Test 10: XOR predictions are correct after training */
    TEST("nn_train_xor predictions correct")
        double ti[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        double tt[4] = {0, 1, 1, 0};
        double wih[4] = {0.15, 0.25, 0.20, 0.30};
        double who[2] = {0.40, 0.50};
        double bh[2] = {0.35, 0.35};
        double bo[1] = {0.60};
        double hid[2], out[1];
        /* Train */
        for (int epoch = 0; epoch < MAX_EPOCHS; epoch++) {
            for (int s = 0; s < 4; s++) {
                nn_forward(ti[s], wih, who, bh, bo, hid, out,
                           INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
                double od = (tt[s] - out[0]) * sigmoid_derivative(out[0]);
                double hd[2];
                for (int j = 0; j < HIDDEN_SIZE; j++)
                    hd[j] = od * who[j] * sigmoid_derivative(hid[j]);
                for (int j = 0; j < HIDDEN_SIZE; j++)
                    who[j] += LEARNING_RATE * od * hid[j];
                bo[0] += LEARNING_RATE * od;
                for (int i = 0; i < INPUT_SIZE; i++)
                    for (int j = 0; j < HIDDEN_SIZE; j++)
                        wih[i * HIDDEN_SIZE + j] += LEARNING_RATE * hd[j] * ti[s][i];
                for (int j = 0; j < HIDDEN_SIZE; j++)
                    bh[j] += LEARNING_RATE * hd[j];
            }
        }
        /* Check predictions */
        int correct = 1;
        for (int s = 0; s < 4; s++) {
            nn_forward(ti[s], wih, who, bh, bo, hid, out,
                       INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
            double rounded = (out[0] > 0.5) ? 1.0 : 0.0;
            if (rounded != tt[s]) correct = 0;
        }
        EXPECT(correct);

    /* ---- Summary ---- */
    printf("\n%d / %d tests passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
