/*
 * Kata: Temperature Converter
 *
 * Write functions to convert between Celsius, Fahrenheit, and Kelvin:
 *
 *   double celsius_to_fahrenheit(double c)   — returns (c * 9.0/5.0) + 32.0
 *   double fahrenheit_to_celsius(double f)   — returns (f - 32.0) * 5.0/9.0
 *   double celsius_to_kelvin(double c)       — returns c + 273.15
 *   double kelvin_to_celsius(double k)       — returns k - 273.15
 *   double fahrenheit_to_kelvin(double f)    — convert F->C, then C->K (reuse!)
 *   double kelvin_to_fahrenheit(double k)    — convert K->C, then C->F (reuse!)
 *
 * Then write main() that prints a conversion table:
 *
 *   Celsius  | Fahrenheit | Kelvin
 *   ---------|------------|--------
 *     -40.00 |     -40.00 | 233.15
 *       0.00 |      32.00 | 273.15
 *      20.00 |      68.00 | 293.15
 *      37.00 |      98.60 | 310.15
 *     100.00 |     212.00 | 373.15
 *
 * Hints:
 *   - fahrenheit_to_kelvin should CALL celsius_to_kelvin(fahrenheit_to_celsius(f))
 *     This is function composition — reusing existing functions instead of
 *     rewriting the formula.
 *   - Use printf with width specifiers for aligned columns: %8.2f
 */

#include <stdio.h>

/* TODO: Write the declaration (prototype) for celsius_to_fahrenheit */
/* TODO: Write the declaration (prototype) for fahrenheit_to_celsius */
/* TODO: Write the declaration (prototype) for celsius_to_kelvin */
/* TODO: Write the declaration (prototype) for kelvin_to_celsius */
/* TODO: Write the declaration (prototype) for fahrenheit_to_kelvin */
/* TODO: Write the declaration (prototype) for kelvin_to_fahrenheit */

int main(void)
{
    double test_temps[] = {-40.0, 0.0, 20.0, 37.0, 100.0};
    int n = sizeof(test_temps) / sizeof(test_temps[0]);

    printf("  Celsius  | Fahrenheit |  Kelvin\n");
    printf("-----------|------------|----------\n");

    /* TODO: Loop through test_temps. For each value (which is in Celsius):
     *   - Convert to Fahrenheit using celsius_to_fahrenheit()
     *   - Convert to Kelvin using celsius_to_kelvin()
     *   - Print all three values in aligned columns using %9.2f
     */

    return 0;
}

/* TODO: Write the definition for celsius_to_fahrenheit */

/* TODO: Write the definition for fahrenheit_to_celsius */

/* TODO: Write the definition for celsius_to_kelvin */

/* TODO: Write the definition for kelvin_to_celsius */

/* TODO: Write the definition for fahrenheit_to_kelvin
 *   Hint: call celsius_to_kelvin(fahrenheit_to_celsius(f))
 */

/* TODO: Write the definition for kelvin_to_fahrenheit
 *   Hint: call celsius_to_fahrenheit(kelvin_to_celsius(k))
 */
