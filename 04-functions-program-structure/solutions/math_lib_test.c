#include <stdio.h>
#include <math.h>
#include "math_lib.h"

static int tests_run = 0;
static int tests_passed = 0;

static void check(int condition, const char *test_name)
{
    tests_run++;
    if (condition) {
        tests_passed++;
        printf("  PASS: %s\n", test_name);
    } else {
        printf("  FAIL: %s\n", test_name);
    }
}

int main(void)
{
    int arr1[] = {3, 7, 2, 9, 4};
    int arr2[] = {-5, -1, -8, -3};
    int arr3[] = {42};
    int arr4[] = {10, 20, 30, 40, 50};

    printf("--- max_of_array tests ---\n");
    check(max_of_array(arr1, 5) == 9,    "max of {3,7,2,9,4} is 9");
    check(max_of_array(arr2, 4) == -1,   "max of {-5,-1,-8,-3} is -1");
    check(max_of_array(arr3, 1) == 42,   "max of {42} is 42");
    check(max_of_array(arr4, 5) == 50,   "max of {10,20,30,40,50} is 50");

    printf("\n--- min_of_array tests ---\n");
    check(min_of_array(arr1, 5) == 2,    "min of {3,7,2,9,4} is 2");
    check(min_of_array(arr2, 4) == -8,   "min of {-5,-1,-8,-3} is -8");
    check(min_of_array(arr3, 1) == 42,   "min of {42} is 42");
    check(min_of_array(arr4, 5) == 10,   "min of {10,20,30,40,50} is 10");

    printf("\n--- average tests ---\n");
    check(fabs(average(arr1, 5) - 5.0) < 0.001,   "avg of {3,7,2,9,4} is 5.0");
    check(fabs(average(arr2, 4) - (-4.25)) < 0.001, "avg of {-5,-1,-8,-3} is -4.25");
    check(fabs(average(arr3, 1) - 42.0) < 0.001,  "avg of {42} is 42.0");
    check(fabs(average(arr4, 5) - 30.0) < 0.001,  "avg of {10,20,30,40,50} is 30.0");

    printf("\n--- factorial tests ---\n");
    check(factorial(0) == 1,     "0! = 1");
    check(factorial(1) == 1,     "1! = 1");
    check(factorial(5) == 120,   "5! = 120");
    check(factorial(10) == 3628800, "10! = 3628800");
    check(factorial(12) == 479001600L, "12! = 479001600");

    printf("\n=== Results: %d/%d tests passed ===\n", tests_passed, tests_run);

    return (tests_passed == tests_run) ? 0 : 1;
}
