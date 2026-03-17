#include <stdio.h>

double celsius_to_fahrenheit(double c);
double fahrenheit_to_celsius(double f);
double celsius_to_kelvin(double c);
double kelvin_to_celsius(double k);
double fahrenheit_to_kelvin(double f);
double kelvin_to_fahrenheit(double k);

int main(void)
{
    double test_temps[] = {-40.0, 0.0, 20.0, 37.0, 100.0};
    int n = sizeof(test_temps) / sizeof(test_temps[0]);

    printf("  Celsius  | Fahrenheit |  Kelvin\n");
    printf("-----------|------------|----------\n");

    for (int i = 0; i < n; i++) {
        double c = test_temps[i];
        double f = celsius_to_fahrenheit(c);
        double k = celsius_to_kelvin(c);
        printf(" %9.2f | %10.2f | %8.2f\n", c, f, k);
    }

    return 0;
}

double celsius_to_fahrenheit(double c)
{
    return (c * 9.0 / 5.0) + 32.0;
}

double fahrenheit_to_celsius(double f)
{
    return (f - 32.0) * 5.0 / 9.0;
}

double celsius_to_kelvin(double c)
{
    return c + 273.15;
}

double kelvin_to_celsius(double k)
{
    return k - 273.15;
}

double fahrenheit_to_kelvin(double f)
{
    return celsius_to_kelvin(fahrenheit_to_celsius(f));
}

double kelvin_to_fahrenheit(double k)
{
    return celsius_to_fahrenheit(kelvin_to_celsius(k));
}
