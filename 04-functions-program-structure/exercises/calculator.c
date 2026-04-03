/*
 * Kata: Calculator
 *
 * Build a simple 4-operation calculator using functions.
 *
 * Write four functions:
 *   double add(double a, double b)       — returns a + b
 *   double subtract(double a, double b)  — returns a - b
 *   double multiply(double a, double b)  — returns a * b
 *   double divide(double a, double b)    — returns a / b (print error if b is 0)
 *
 * Then write main() that:
 *   1. Asks the user for two numbers and an operator (+, -, *, /)
 *   2. Calls the appropriate function
 *   3. Prints the result
 *
 * Example session:
 *   Enter first number: 10
 *   Enter operator (+, -, *, /): *
 *   Enter second number: 3
 *   10.00 * 3.00 = 30.00
 *
 * Hints:
 *   - Use scanf("%lf", &num) for doubles
 *   - Use scanf(" %c", &op) for the operator (note the space before %c)
 *   - For divide-by-zero, print an error and return 0.0
 */

#include <stdio.h>

/* TODO: Write the declaration (prototype) for add */
/* TODO: Write the declaration (prototype) for subtract */
/* TODO: Write the declaration (prototype) for multiply */
/* TODO: Write the declaration (prototype) for divide */

int main(void)
{
    double a, b, result = 0.0;
    char op;

    printf("Enter first number: ");
    scanf("%lf", &a);

    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &op);

    printf("Enter second number: ");
    scanf("%lf", &b);

    /* TODO: Use if/else or switch on 'op' to call the right function
     *       and store the return value in 'result'.
     *       If the operator is invalid, print "Unknown operator" and return 1.
     */

    printf("%.2f %c %.2f = %.2f\n", a, op, b, result);

    return 0;
}

/* TODO: Write the definition for add */

/* TODO: Write the definition for subtract */

/* TODO: Write the definition for multiply */

/* TODO: Write the definition for divide
 *   - If b is 0, print "Error: division by zero\n" and return 0.0
 */
