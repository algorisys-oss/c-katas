/*
 * float_mysteries.c — Explore floating point quirks
 *
 * Floating point numbers are approximations. In this kata you will
 * discover why that matters through hands-on experiments.
 *
 * Compile: gcc -Wall -Wextra -pedantic -std=c99 -o float_mysteries float_mysteries.c
 * Run:     ./float_mysteries
 */

#include <stdio.h>
#include <math.h>
#include <float.h>

int main(void)
{
    /* ================================================================
     * EXPERIMENT 1: The classic 0.1 + 0.2 problem
     *
     * Add 0.1 and 0.2 and see if the result equals 0.3.
     * Print the result with 20 decimal places to see what really
     * happens.
     * ================================================================ */

    printf("=== Experiment 1: 0.1 + 0.2 ===\n");

    /* TODO:
     * 1. Declare a double 'a' = 0.1, a double 'b' = 0.2
     * 2. Declare a double 'c' = a + b
     * 3. Print c with 20 decimal places:  printf("%.20f\n", c);
     * 4. Print 0.3 with 20 decimal places for comparison
     * 5. Print whether c == 0.3 using:
     *    printf("c == 0.3?  %s\n", (c == 0.3) ? "YES" : "NO");
     */


    printf("\n");

    /* ================================================================
     * EXPERIMENT 2: Safe float comparison with epsilon
     *
     * Since == does not work reliably, we compare using a small
     * tolerance (epsilon): two floats are "equal" if the absolute
     * difference between them is less than epsilon.
     *
     * fabs(a - b) < epsilon  means "close enough"
     * ================================================================ */

    printf("=== Experiment 2: Epsilon comparison ===\n");

    /* TODO:
     * 1. Using a, b, c from Experiment 1 (or redeclare them):
     *    double a = 0.1, b = 0.2, c = a + b;
     * 2. Define a small epsilon:
     *    double epsilon = 1e-9;
     * 3. Compare using epsilon:
     *    if (fabs(c - 0.3) < epsilon)
     *        printf("c is approximately equal to 0.3\n");
     *    else
     *        printf("c is NOT approximately equal to 0.3\n");
     * 4. Also try with DBL_EPSILON from <float.h> and see what happens.
     */


    printf("\n");

    /* ================================================================
     * EXPERIMENT 3: Precision loss with large numbers
     *
     * A float has about 7 digits of precision. What happens when
     * you add a small number to a very large number?
     * ================================================================ */

    printf("=== Experiment 3: Precision loss ===\n");

    /* TODO:
     * 1. Declare: float big = 1000000.0f;
     * 2. Declare: float small_val = 0.001f;
     * 3. Add them:  float result = big + small_val;
     * 4. Print with 10 decimal places:
     *    printf("%.10f + %.10f = %.10f\n", big, small_val, result);
     * 5. Now try: did result - big == small_val?
     *    printf("result - big = %.10f\n", result - big);
     *    printf("Expected:       %.10f\n", small_val);
     *
     * 6. Repeat the same experiment with double instead of float.
     *    Does double do better?
     */


    printf("\n");

    /* ================================================================
     * EXPERIMENT 4: Accumulation error
     *
     * Adding 0.1 ten times should give 1.0, right?
     * ================================================================ */

    printf("=== Experiment 4: Accumulation error ===\n");

    /* TODO:
     * 1. Declare: double sum = 0.0;
     * 2. In a loop, add 0.1 to sum ten times:
     *    int i;
     *    for (i = 0; i < 10; i++) {
     *        sum += 0.1;
     *    }
     * 3. Print sum with 20 decimal places.
     * 4. Print whether sum == 1.0
     * 5. Print the difference: sum - 1.0
     */


    printf("\n");

    /* ================================================================
     * EXPERIMENT 5: The money trap
     *
     * Why you should NEVER use floats for money.
     * ================================================================ */

    printf("=== Experiment 5: Money trap ===\n");

    /* TODO:
     * Method 1 (WRONG): Use float for dollars
     * 1. Declare: float balance = 100.00f;
     * 2. Subtract 99.99f from it.
     * 3. Print with 10 decimal places. Is it exactly 0.01?
     *
     * Method 2 (CORRECT): Use int for cents
     * 1. Declare: int balance_cents = 10000;  (100.00 in cents)
     * 2. Subtract 9999 (99.99 in cents).
     * 3. Print: printf("$%d.%02d\n", balance_cents / 100, balance_cents % 100);
     *    This is always exact!
     */


    printf("\n");

    /* ================================================================
     * EXPERIMENT 6: Special float values
     *
     * IEEE 754 defines special values: infinity, negative infinity,
     * and NaN (Not a Number).
     * ================================================================ */

    printf("=== Experiment 6: Special values ===\n");

    /* TODO:
     * 1. Compute and print: 1.0 / 0.0   (what do you get?)
     * 2. Compute and print: -1.0 / 0.0
     * 3. Compute and print: 0.0 / 0.0   (NaN!)
     * 4. Test NaN:
     *    double nan_val = 0.0 / 0.0;
     *    printf("nan == nan?  %s\n", (nan_val == nan_val) ? "YES" : "NO");
     *    (NaN is NOT equal to itself! This is how you detect it.)
     *
     * Use %f to print these values.
     */


    return 0;
}
