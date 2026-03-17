/*
 * math_lib.c — Implementation of the math library
 *
 * This file DEFINES the functions that are DECLARED in math_lib.h.
 * Include the header so the compiler can verify that your definitions
 * match the declarations.
 */

#include "math_lib.h"

/*
 * max_of_array — Find the largest element in an array.
 *
 * Strategy:
 *   1. Assume the first element is the max
 *   2. Walk through the rest of the array
 *   3. If you find something bigger, update the max
 *   4. Return the max
 */
int max_of_array(int arr[], int n)
{
    /* TODO: Implement this function */
    (void)arr;
    (void)n;
    return 0;
}

/*
 * min_of_array — Find the smallest element in an array.
 *
 * Strategy: same as max_of_array but look for smaller values.
 */
int min_of_array(int arr[], int n)
{
    /* TODO: Implement this function */
    (void)arr;
    (void)n;
    return 0;
}

/*
 * average — Compute the arithmetic mean.
 *
 * Strategy:
 *   1. Sum all the elements (use a long or double to avoid overflow)
 *   2. Divide by n
 *   3. Return as a double
 */
double average(int arr[], int n)
{
    /* TODO: Implement this function */
    (void)arr;
    (void)n;
    return 0.0;
}

/*
 * factorial — Compute n!
 *
 * Remember:
 *   0! = 1
 *   1! = 1
 *   5! = 5 * 4 * 3 * 2 * 1 = 120
 *
 * Strategy: use a loop that multiplies from 1 to n.
 */
long factorial(int n)
{
    /* TODO: Implement this function */
    (void)n;
    return 0;
}
