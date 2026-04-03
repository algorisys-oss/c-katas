#include <stdio.h>

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);

int main(void)
{
    double a = 0.0, b = 0.0, result = 0.0;
    char op = '+';

    printf("Enter first number: ");
    if (scanf("%lf", &a) != 1) { fprintf(stderr, "Invalid number\n"); return 1; }

    printf("Enter operator (+, -, *, /): ");
    if (scanf(" %c", &op) != 1) { fprintf(stderr, "Invalid operator\n"); return 1; }

    printf("Enter second number: ");
    if (scanf("%lf", &b) != 1) { fprintf(stderr, "Invalid number\n"); return 1; }

    switch (op) {
        case '+':
            result = add(a, b);
            break;
        case '-':
            result = subtract(a, b);
            break;
        case '*':
            result = multiply(a, b);
            break;
        case '/':
            result = divide(a, b);
            break;
        default:
            printf("Unknown operator: %c\n", op);
            return 1;
    }

    printf("%.2f %c %.2f = %.2f\n", a, op, b, result);

    return 0;
}

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    if (b == 0.0) {
        printf("Error: division by zero\n");
        return 0.0;
    }
    return a / b;
}
