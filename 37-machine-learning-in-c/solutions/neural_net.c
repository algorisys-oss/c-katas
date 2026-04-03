/*
 * Solution: Simple Neural Network
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

/* ---- Implementation ---- */

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x)
{
    return x * (1.0 - x);
}

void nn_forward(const double inputs[], const double weights_ih[],
                const double weights_ho[], const double bias_h[],
                const double bias_o[], double hidden[], double output[],
                int input_size, int hidden_size, int output_size)
{
    /* Compute hidden layer */
    for (int j = 0; j < hidden_size; j++) {
        double sum = bias_h[j];
        for (int i = 0; i < input_size; i++) {
            sum += inputs[i] * weights_ih[i * hidden_size + j];
        }
        hidden[j] = sigmoid(sum);
    }
    /* Compute output layer */
    for (int k = 0; k < output_size; k++) {
        double sum = bias_o[k];
        for (int j = 0; j < hidden_size; j++) {
            sum += hidden[j] * weights_ho[j * output_size + k];
        }
        output[k] = sigmoid(sum);
    }
}

double nn_train_xor(void)
{
    double train_inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double train_targets[4] = {0, 1, 1, 0};
    double weights_ih[4] = {0.15, 0.25, 0.20, 0.30};
    double weights_ho[2] = {0.40, 0.50};
    double bias_h[2] = {0.35, 0.35};
    double bias_o[1] = {0.60};
    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];

    for (int epoch = 0; epoch < MAX_EPOCHS; epoch++) {
        for (int s = 0; s < 4; s++) {
            nn_forward(train_inputs[s], weights_ih, weights_ho,
                       bias_h, bias_o, hidden, output,
                       INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
            /* Output layer error */
            double output_delta = (train_targets[s] - output[0]) *
                                  sigmoid_derivative(output[0]);
            /* Hidden layer error */
            double hidden_delta[HIDDEN_SIZE];
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                hidden_delta[j] = output_delta * weights_ho[j] *
                                  sigmoid_derivative(hidden[j]);
            }
            /* Update output weights and bias */
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                weights_ho[j] += LEARNING_RATE * output_delta * hidden[j];
            }
            bias_o[0] += LEARNING_RATE * output_delta;
            /* Update hidden weights and biases */
            for (int i = 0; i < INPUT_SIZE; i++) {
                for (int j = 0; j < HIDDEN_SIZE; j++) {
                    weights_ih[i * HIDDEN_SIZE + j] +=
                        LEARNING_RATE * hidden_delta[j] * train_inputs[s][i];
                }
            }
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                bias_h[j] += LEARNING_RATE * hidden_delta[j];
            }
        }
    }

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

    TEST("sigmoid(0) = 0.5")
        EXPECT(APPROX(sigmoid(0.0), 0.5));

    TEST("sigmoid(10) near 1.0")
        EXPECT(sigmoid(10.0) > 0.999);

    TEST("sigmoid(-10) near 0.0")
        EXPECT(sigmoid(-10.0) < 0.001);

    TEST("sigmoid_derivative(0.5) = 0.25")
        EXPECT(APPROX(sigmoid_derivative(0.5), 0.25));

    TEST("sigmoid_derivative(0) = 0, sigmoid_derivative(1) = 0")
        EXPECT(APPROX(sigmoid_derivative(0.0), 0.0) &&
               APPROX(sigmoid_derivative(1.0), 0.0));

    TEST("nn_forward with known weights")
        double inputs[2] = {1.0, 0.5};
        double wih[4] = {0.1, 0.2, 0.3, 0.4};
        double who[2] = {0.5, 0.6};
        double bh[2] = {0.0, 0.0};
        double bo[1] = {0.0};
        double hidden[2], output[1];
        nn_forward(inputs, wih, who, bh, bo, hidden, output, 2, 2, 1);
        double exp_h0 = 1.0 / (1.0 + exp(-0.25));
        double exp_h1 = 1.0 / (1.0 + exp(-0.40));
        EXPECT(APPROX(hidden[0], exp_h0) && APPROX(hidden[1], exp_h1));

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

    TEST("nn_forward respects biases")
        double inputs[2] = {0.0, 0.0};
        double wih[4] = {0.1, 0.2, 0.3, 0.4};
        double who[2] = {0.5, 0.6};
        double bh[2] = {1.0, -1.0};
        double bo[1] = {0.5};
        double hidden[2], output[1];
        nn_forward(inputs, wih, who, bh, bo, hidden, output, 2, 2, 1);
        double exp_h0 = 1.0 / (1.0 + exp(-1.0));
        double exp_h1 = 1.0 / (1.0 + exp(1.0));
        EXPECT(APPROX(hidden[0], exp_h0) && APPROX(hidden[1], exp_h1));

    TEST("nn_train_xor converges (loss < 0.01)")
        double loss = nn_train_xor();
        printf("(loss=%.4f) ", loss);
        EXPECT(loss < 0.01);

    TEST("nn_train_xor predictions correct")
        double ti[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        double tt[4] = {0, 1, 1, 0};
        double wih[4] = {0.15, 0.25, 0.20, 0.30};
        double who[2] = {0.40, 0.50};
        double bh[2] = {0.35, 0.35};
        double bo[1] = {0.60};
        double hid[2], out[1];
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
        int correct = 1;
        for (int s = 0; s < 4; s++) {
            nn_forward(ti[s], wih, who, bh, bo, hid, out,
                       INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
            double rounded = (out[0] > 0.5) ? 1.0 : 0.0;
            if (rounded != tt[s]) correct = 0;
        }
        EXPECT(correct);

    printf("\n%d / %d tests passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
