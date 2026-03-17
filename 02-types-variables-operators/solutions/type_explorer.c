/*
 * type_explorer.c — Solution
 *
 * Compile: gcc -Wall -Wextra -pedantic -std=c99 -o type_explorer type_explorer.c
 * Run:     ./type_explorer
 */

#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    /* PART 1: Type sizes */

    printf("=== Type Sizes ===\n");
    printf("%-14s %zu bytes\n", "char:", sizeof(char));
    printf("%-14s %zu bytes\n", "short:", sizeof(short));
    printf("%-14s %zu bytes\n", "int:", sizeof(int));
    printf("%-14s %zu bytes\n", "long:", sizeof(long));
    printf("%-14s %zu bytes\n", "long long:", sizeof(long long));
    printf("%-14s %zu bytes\n", "float:", sizeof(float));
    printf("%-14s %zu bytes\n", "double:", sizeof(double));
    printf("%-14s %zu bytes\n", "unsigned char:", sizeof(unsigned char));
    printf("%-14s %zu bytes\n", "unsigned short:", sizeof(unsigned short));
    printf("%-14s %zu bytes\n", "unsigned int:", sizeof(unsigned int));
    printf("%-14s %zu bytes\n", "unsigned long:", sizeof(unsigned long));
    printf("\n");

    /* PART 2: Integer ranges */

    printf("=== Integer Ranges ===\n");
    printf("char:       %d to %d\n", CHAR_MIN, CHAR_MAX);
    printf("uchar:      0 to %u\n", (unsigned)UCHAR_MAX);
    printf("short:      %d to %d\n", SHRT_MIN, SHRT_MAX);
    printf("ushort:     0 to %u\n", (unsigned)USHRT_MAX);
    printf("int:        %d to %d\n", INT_MIN, INT_MAX);
    printf("uint:       0 to %u\n", UINT_MAX);
    printf("long:       %ld to %ld\n", LONG_MIN, LONG_MAX);
    printf("ulong:      0 to %lu\n", ULONG_MAX);
    printf("long long:  %lld to %lld\n", LLONG_MIN, LLONG_MAX);
    printf("ulong long: 0 to %llu\n", ULLONG_MAX);
    printf("\n");

    /* PART 3: Float/double ranges */

    printf("=== Float Ranges ===\n");
    printf("float  min:       %e\n", FLT_MIN);
    printf("float  max:       %e\n", FLT_MAX);
    printf("float  precision: %d digits\n", FLT_DIG);
    printf("double min:       %e\n", DBL_MIN);
    printf("double max:       %e\n", DBL_MAX);
    printf("double precision: %d digits\n", DBL_DIG);
    printf("\n");

    /* PART 4: Overflow demonstration */

    printf("=== Overflow Demonstration ===\n");

    /* Unsigned overflow: wraps around */
    unsigned char max_uc = 255;
    printf("unsigned char max_uc = %u\n", max_uc);
    max_uc = (unsigned char)(max_uc + 1);
    printf("After adding 1:        %u  (wrapped to 0!)\n", max_uc);

    unsigned char min_uc = 0;
    printf("unsigned char min_uc = %u\n", min_uc);
    min_uc = (unsigned char)(min_uc - 1);
    printf("After subtracting 1:   %u  (wrapped to 255!)\n", min_uc);

    printf("\n");

    /* Integer promotion demonstration */
    char a = 1, b = 2;
    printf("sizeof(a):     %zu  (char = 1 byte)\n", sizeof(a));
    printf("sizeof(a + b): %zu  (int = %zu bytes!)\n",
           sizeof(a + b), sizeof(int));
    printf("char + char is promoted to int before addition!\n");

    return 0;
}
