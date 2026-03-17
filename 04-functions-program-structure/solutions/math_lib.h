/*
 * math_lib.h — Header file for a small math library
 *
 * This file DECLARES the functions. The DEFINITIONS (actual code)
 * live in math_lib.c.
 *
 * Any .c file that wants to use these functions does:
 *     #include "math_lib.h"
 */

#ifndef MATH_LIB_H
#define MATH_LIB_H

/*
 * max_of_array — Find the largest element in an array.
 *   arr: pointer to the first element
 *   n:   number of elements (must be > 0)
 *   Returns: the largest value in arr
 */
int max_of_array(int arr[], int n);

/*
 * min_of_array — Find the smallest element in an array.
 *   arr: pointer to the first element
 *   n:   number of elements (must be > 0)
 *   Returns: the smallest value in arr
 */
int min_of_array(int arr[], int n);

/*
 * average — Compute the arithmetic mean of an array of integers.
 *   arr: pointer to the first element
 *   n:   number of elements (must be > 0)
 *   Returns: the average as a double
 */
double average(int arr[], int n);

/*
 * factorial — Compute n! (n factorial).
 *   n: a non-negative integer
 *   Returns: n! as a long (0! = 1, 1! = 1, 5! = 120, etc.)
 */
long factorial(int n);

#endif /* MATH_LIB_H */
