/*
 * float_mysteries.c — Solution
 *
 * Compile: gcc -Wall -Wextra -pedantic -std=c99 -o float_mysteries float_mysteries.c -lm
 * Run:     ./float_mysteries
 */

#include <stdio.h>
#include <math.h>
#include <float.h>

int main(void)
{
    /* EXPERIMENT 1: The classic 0.1 + 0.2 problem */

    printf("=== Experiment 1: 0.1 + 0.2 ===\n");
    double a = 0.1;
    double b = 0.2;
    double c = a + b;

    printf("a     = %.20f\n", a);
    printf("b     = %.20f\n", b);
    printf("a + b = %.20f\n", c);
    printf("0.3   = %.20f\n", 0.3);
    printf("c == 0.3?  %s\n", (c == 0.3) ? "YES" : "NO");
    printf("\n");

    /* EXPERIMENT 2: Safe float comparison with epsilon */

    printf("=== Experiment 2: Epsilon comparison ===\n");
    double epsilon = 1e-9;

    if (fabs(c - 0.3) < epsilon) {
        printf("With epsilon 1e-9: c IS approximately equal to 0.3\n");
    } else {
        printf("With epsilon 1e-9: c is NOT approximately equal to 0.3\n");
    }

    printf("Actual difference: %e\n", fabs(c - 0.3));
    printf("DBL_EPSILON:       %e\n", DBL_EPSILON);

    if (fabs(c - 0.3) < DBL_EPSILON) {
        printf("With DBL_EPSILON:  c IS approximately equal to 0.3\n");
    } else {
        printf("With DBL_EPSILON:  c is NOT approximately equal to 0.3\n");
    }
    printf("\n");

    /* EXPERIMENT 3: Precision loss with large numbers */

    printf("=== Experiment 3: Precision loss ===\n");
    float big = 1000000.0f;
    float small_val = 0.001f;
    float result_f = big + small_val;

    printf("float:  %.10f + %.10f = %.10f\n", big, small_val, result_f);
    printf("float:  result - big = %.10f\n", result_f - big);
    printf("float:  Expected:       %.10f\n", small_val);
    printf("\n");

    double big_d = 1000000.0;
    double small_d = 0.001;
    double result_d = big_d + small_d;

    printf("double: %.10f + %.10f = %.10f\n", big_d, small_d, result_d);
    printf("double: result - big = %.10f\n", result_d - big_d);
    printf("double: Expected:       %.10f\n", small_d);
    printf("(double has more precision, so it does better)\n");
    printf("\n");

    /* EXPERIMENT 4: Accumulation error */

    printf("=== Experiment 4: Accumulation error ===\n");
    double sum = 0.0;
    int i;
    for (i = 0; i < 10; i++) {
        sum += 0.1;
    }
    printf("0.1 added 10 times = %.20f\n", sum);
    printf("Expected 1.0       = %.20f\n", 1.0);
    printf("sum == 1.0?  %s\n", (sum == 1.0) ? "YES" : "NO");
    printf("Difference:          %e\n", sum - 1.0);
    printf("\n");

    /* EXPERIMENT 5: The money trap */

    printf("=== Experiment 5: Money trap ===\n");

    printf("Method 1 (WRONG): floats for dollars\n");
    float balance = 100.00f;
    balance = balance - 99.99f;
    printf("  100.00 - 99.99 = %.10f  (should be 0.01)\n", balance);
    printf("\n");

    printf("Method 2 (CORRECT): integers for cents\n");
    int balance_cents = 10000;  /* $100.00 */
    balance_cents = balance_cents - 9999;  /* - $99.99 */
    printf("  10000 - 9999 = %d cents = $%d.%02d  (exact!)\n",
           balance_cents, balance_cents / 100, balance_cents % 100);
    printf("\n");

    /* EXPERIMENT 6: Special float values */

    printf("=== Experiment 6: Special values ===\n");
    printf(" 1.0 /  0.0 = %f\n", 1.0 / 0.0);
    printf("-1.0 /  0.0 = %f\n", -1.0 / 0.0);
    printf(" 0.0 /  0.0 = %f\n", 0.0 / 0.0);

    double nan_val = 0.0 / 0.0;
    printf("nan == nan?  %s\n", (nan_val == nan_val) ? "YES" : "NO");
    printf("(NaN is NOT equal to itself -- this is how you detect it)\n");

    return 0;
}
