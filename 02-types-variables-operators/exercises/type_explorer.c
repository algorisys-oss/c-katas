/*
 * type_explorer.c — Discover type sizes, ranges, and overflow behavior
 *
 * In this kata you will:
 *   1. Print the sizeof every fundamental C type
 *   2. Print the min/max range of each integer type using <limits.h>
 *   3. Demonstrate what happens when integers overflow
 *
 * Compile: gcc -Wall -Wextra -pedantic -std=c99 -o type_explorer type_explorer.c
 * Run:     ./type_explorer
 */

#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    /* ================================================================
     * PART 1: Type sizes
     *
     * Use sizeof to print how many bytes each type occupies.
     * The format specifier for sizeof results is %zu.
     *
     * Example:
     *   printf("char:      %zu bytes\n", sizeof(char));
     * ================================================================ */

    printf("=== Type Sizes ===\n");

    /* TODO: Print sizeof for each of these types:
     *   char, short, int, long, long long,
     *   float, double,
     *   unsigned char, unsigned short, unsigned int, unsigned long
     *
     * Use the format:  printf("%-14s %zu bytes\n", "type:", sizeof(type));
     */


    printf("\n");

    /* ================================================================
     * PART 2: Integer ranges
     *
     * <limits.h> defines constants for the min and max of each type.
     * Print them to see what range each type can hold.
     *
     * Useful constants:
     *   CHAR_MIN, CHAR_MAX, UCHAR_MAX
     *   SHRT_MIN, SHRT_MAX, USHRT_MAX
     *   INT_MIN,  INT_MAX,  UINT_MAX
     *   LONG_MIN, LONG_MAX, ULONG_MAX
     *   LLONG_MIN, LLONG_MAX, ULLONG_MAX
     *
     * For signed types use %d or %ld or %lld.
     * For unsigned types use %u or %lu or %llu.
     * ================================================================ */

    printf("=== Integer Ranges ===\n");

    /* TODO: Print the min and max for each integer type.
     *
     * Format suggestion:
     *   printf("char:       %d to %d\n", CHAR_MIN, CHAR_MAX);
     *   printf("uchar:      0 to %u\n", (unsigned)UCHAR_MAX);
     *
     * Do the same for short, int, long, and long long (signed and unsigned).
     */


    printf("\n");

    /* ================================================================
     * PART 3: Float/double ranges
     *
     * <float.h> defines FLT_MIN, FLT_MAX, DBL_MIN, DBL_MAX, etc.
     * FLT_MIN is the smallest *positive* normalized float (not the
     * most negative -- that would be -FLT_MAX).
     *
     * Also print FLT_DIG and DBL_DIG: the number of decimal digits
     * of precision.
     * ================================================================ */

    printf("=== Float Ranges ===\n");

    /* TODO: Print for float and double:
     *   - smallest positive value (FLT_MIN / DBL_MIN)
     *   - largest value (FLT_MAX / DBL_MAX)
     *   - digits of precision (FLT_DIG / DBL_DIG)
     *
     * Use %e for very large/small numbers (scientific notation).
     * Example:
     *   printf("float  min:       %e\n", FLT_MIN);
     */


    printf("\n");

    /* ================================================================
     * PART 4: Overflow demonstration
     *
     * What happens when you exceed the maximum value of a type?
     * Try it and observe!
     *
     * Note: unsigned overflow is well-defined in C (wraps around).
     * Signed overflow is UNDEFINED BEHAVIOR (the compiler can do
     * anything), but in practice it usually wraps on most systems.
     * ================================================================ */

    printf("=== Overflow Demonstration ===\n");

    /* TODO: Unsigned overflow
     *
     * 1. Declare an unsigned char with value 255 (UCHAR_MAX).
     * 2. Print its value.
     * 3. Add 1 to it.
     * 4. Print its value again. What happened?
     *
     * 5. Declare an unsigned char with value 0.
     * 6. Subtract 1 from it.
     * 7. Print its value. What happened?
     */


    printf("\n");

    /* TODO: Observe what sizeof reports for an expression
     *
     * Declare two char variables, add them together, and use sizeof
     * on the result. What type does the compiler use for char + char?
     *
     * char a = 1, b = 2;
     * printf("sizeof(a):     %zu\n", sizeof(a));
     * printf("sizeof(a + b): %zu\n", sizeof(a + b));
     *
     * This demonstrates INTEGER PROMOTION: char + char becomes int!
     */


    return 0;
}
