/*
 * Kata: GDB Detective
 *
 * This file contains 5 buggy functions. Each bug requires a different
 * GDB technique to find and fix. The bugs are marked with BUG comments
 * but you should practice finding them WITH GDB, not just by reading.
 *
 * How to work through this:
 *   1. Compile:  gcc -std=c99 -Wall -Wextra -pedantic -g -o gdb_detective gdb_detective.c
 *   2. Run:      ./gdb_detective  (observe failures)
 *   3. Debug:    gdb ./gdb_detective
 *   4. Fix each bug, recompile, verify all tests pass
 *
 * Each function has a comment telling you which GDB technique to use.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ── Test Harness ─────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ── Bug 1: Off-by-one in array access ───────────────────────── */
/*
 * GDB technique: breakpoint + print
 *
 *   (gdb) break bug1_sum_array
 *   (gdb) run
 *   (gdb) next        (step through the loop)
 *   (gdb) print i
 *   (gdb) print arr[i]
 *
 * Watch what happens when i reaches the last iteration.
 * The loop goes ONE past the end of the array.
 */
int bug1_sum_array(int *arr, int len)
{
    int sum = 0;
    for (int i = 0; i <= len; i++) {  /* BUG: should be i < len */
        sum += arr[i];
    }
    return sum;
}

/* ── Bug 2: Wrong loop condition causes infinite loop ─────────── */
/*
 * GDB technique: step + watch
 *
 *   (gdb) break bug2_count_spaces
 *   (gdb) run
 *   (gdb) watch i
 *   (gdb) continue
 *
 * Watch how 'i' changes. Notice it never reaches the end condition.
 * The decrement should be an increment.
 *
 * WARNING: This function will hang if you run it without GDB.
 * In GDB you can Ctrl+C to interrupt, then inspect where you are.
 */
int bug2_count_spaces(const char *str)
{
    int count = 0;
    int i = 0;
    int len = (int)strlen(str);
    while (i < len) {
        if (str[i] == ' ') {
            count++;
        }
        i--;  /* BUG: should be i++ (causes infinite loop) */
    }
    return count;
}

/* ── Bug 3: Uninitialized variable ────────────────────────────── */
/*
 * GDB technique: print to see garbage value
 *
 *   (gdb) break bug3_find_max
 *   (gdb) run
 *   (gdb) print max     (BEFORE the loop — see garbage value)
 *   (gdb) next
 *   (gdb) print max     (see if it ever gets set properly)
 *
 * The variable 'max' is not initialized. It might contain any value,
 * so if all array elements are smaller than the garbage, the result
 * is wrong.
 */
int bug3_find_max(int *arr, int len)
{
    int max;  /* BUG: should be initialized to arr[0] */
    for (int i = 0; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

/* ── Bug 4: Returning pointer to local variable ──────────────── */
/*
 * GDB technique: backtrace after crash or wrong output
 *
 *   (gdb) run
 *   (gdb) backtrace       (if it crashes, see where)
 *
 * Or set a breakpoint and inspect the returned pointer:
 *   (gdb) break bug4_make_greeting
 *   (gdb) run
 *   (gdb) finish          (let the function return)
 *   (gdb) print result    (the pointer points to dead stack memory)
 *
 * The local array 'buf' lives on the stack. When the function returns,
 * that stack frame is gone. The pointer now points to garbage.
 */
const char *bug4_make_greeting(const char *name)
{
    char buf[128];  /* BUG: local array — dies when function returns */
    snprintf(buf, sizeof(buf), "Hello, %s!", name);
    return buf;  /* BUG: returning pointer to local variable */
}

/* ── Bug 5: Integer overflow ──────────────────────────────────── */
/*
 * GDB technique: watchpoint on the result variable
 *
 *   (gdb) break bug5_factorial
 *   (gdb) run
 *   (gdb) watch result
 *   (gdb) continue
 *
 * Watch 'result' grow. At some point it overflows a 32-bit int
 * and wraps to a negative number. The fix: use a smaller input
 * or use a wider type. For this exercise, use long long.
 *
 * Factorials grow FAST:
 *   12! =  479,001,600   (fits in int)
 *   13! = 6,227,020,800  (overflows int, which maxes at ~2.1 billion)
 */
int bug5_factorial(int n)
{
    int result = 1;  /* BUG: int overflows for n >= 13, use long long */
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

/* ── Main + Tests ─────────────────────────────────────────────── */

int main(void)
{
    printf("=== GDB Detective: Find and Fix the Bugs ===\n\n");

    /* --- Bug 1 tests --- */
    printf("Bug 1: Off-by-one array access\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        int sum = bug1_sum_array(arr, 5);
        TEST("sum of {10,20,30,40,50} should be 150");
        EXPECT(sum == 150);
    }

    /* --- Bug 2 tests --- */
    printf("\nBug 2: Infinite loop\n");
    {
        int spaces = bug2_count_spaces("hello world foo");
        TEST("'hello world foo' has 2 spaces");
        EXPECT(spaces == 2);

        int spaces2 = bug2_count_spaces("nospaces");
        TEST("'nospaces' has 0 spaces");
        EXPECT(spaces2 == 0);
    }

    /* --- Bug 3 tests --- */
    printf("\nBug 3: Uninitialized variable\n");
    {
        int arr1[] = {3, 7, 2, 9, 1};
        TEST("max of {3,7,2,9,1} should be 9");
        EXPECT(bug3_find_max(arr1, 5) == 9);

        int arr2[] = {-5, -3, -8, -1, -4};
        TEST("max of {-5,-3,-8,-1,-4} should be -1");
        EXPECT(bug3_find_max(arr2, 5) == -1);
    }

    /* --- Bug 4 tests --- */
    printf("\nBug 4: Pointer to local variable\n");
    {
        const char *greeting = bug4_make_greeting("Alice");
        TEST("greeting should be 'Hello, Alice!'");
        EXPECT(strcmp(greeting, "Hello, Alice!") == 0);
    }

    /* --- Bug 5 tests --- */
    printf("\nBug 5: Integer overflow\n");
    {
        /*
         * 15! = 1,307,674,368,000
         * This overflows a 32-bit int. The function should return
         * the correct value (requires fixing to use long long).
         *
         * For testing, we check 12! which fits in int (479001600)
         * and 15! which requires long long.
         */
        TEST("12! should be 479001600");
        EXPECT(bug5_factorial(12) == 479001600);

        /* This test will fail until you change return type to long long */
        long long big = (long long)bug5_factorial(15);
        TEST("15! should be 1307674368000");
        EXPECT(big == 1307674368000LL);
    }

    /* --- Summary --- */
    printf("\n========================================\n");
    printf("  Results: %d / %d tests passed\n", tests_passed, tests_total);
    printf("========================================\n");

    if (tests_passed == tests_total) {
        printf("  All bugs fixed! Great detective work.\n");
    } else {
        printf("  Some bugs remain. Use GDB to investigate!\n");
    }

    return (tests_passed == tests_total) ? 0 : 1;
}
