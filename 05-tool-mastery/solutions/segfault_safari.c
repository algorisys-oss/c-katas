/*
 * Kata: Segfault Safari
 *
 * This file contains 5 functions that cause crashes (segfaults or
 * other fatal errors). Your job: diagnose each crash using GDB
 * and/or AddressSanitizer, then fix it.
 *
 * How to work through this:
 *   1. Compile:  gcc -std=c99 -Wall -Wextra -pedantic -g -o segfault_safari segfault_safari.c
 *   2. Run:      ./segfault_safari   (it will crash on the first enabled test)
 *   3. Debug:    gdb ./segfault_safari
 *      (gdb) run
 *      (gdb) backtrace     <- see where it crashed
 *   4. Fix each crash one at a time
 *
 * Approach: each crash function is called from main(). They are all
 * enabled. If the program crashes on crash1, fix it, recompile,
 * then move on to crash2, etc. Or comment out the ones you haven't
 * fixed yet.
 *
 * For extra insight, also try:
 *   gcc -std=c99 -Wall -Wextra -pedantic -g -fsanitize=address -o segfault_safari segfault_safari.c
 *   ./segfault_safari
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -- Test Harness --------------------------------------------------------- */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* -- Crash 1: NULL pointer dereference ------------------------------------ */
/*
 * Diagnosis technique: GDB backtrace
 *
 *   (gdb) run
 *   Program received signal SIGSEGV, Segmentation fault.
 *   (gdb) backtrace
 *   (gdb) print ptr     <- will show 0x0 (NULL)
 *
 * The function receives a pointer that might be NULL but doesn't
 * check before using it.
 *
 * FIX: added NULL check — return 0 if ptr is NULL
 */
int crash1_null_deref(int *ptr, int len)
{
    if (ptr == NULL) {  /* FIX: check for NULL before dereferencing */
        return 0;
    }
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += ptr[i];
    }
    return sum;
}

/* -- Crash 2: Stack overflow from infinite recursion ---------------------- */
/*
 * Diagnosis technique: GDB backtrace (will show thousands of frames)
 *
 *   (gdb) run
 *   Program received signal SIGSEGV, Segmentation fault.
 *   (gdb) backtrace
 *   #0     crash2_recursive_sum ...
 *   #1     crash2_recursive_sum ...
 *   #2     crash2_recursive_sum ...
 *   ... (thousands of frames)
 *
 * The base case is wrong — it never triggers, so the function
 * calls itself forever until the stack overflows.
 *
 * FIX: changed base case from n == -1 to n <= 0
 */
int crash2_recursive_sum(int n)
{
    if (n <= 0) {  /* FIX: n <= 0 (was n == -1 — never reached for positive n) */
        return 0;
    }
    return n + crash2_recursive_sum(n - 1);
}

/* -- Crash 3: Writing to string literal ----------------------------------- */
/*
 * Diagnosis technique: GDB backtrace + ASan
 *
 * String literals like "hello" are stored in read-only memory.
 * Attempting to modify them causes a segfault.
 *
 *   (gdb) run
 *   Program received signal SIGSEGV
 *   (gdb) backtrace
 *   (gdb) print str    <- points to read-only memory
 *
 * FIX: use a char array (stack copy) instead of a char pointer
 * to a string literal.
 */
void crash3_modify_literal(char *result, size_t result_size)
{
    char str[] = "hello world";  /* FIX: array copy (was pointer to literal) */
    str[0] = 'H';               /* Now safe — writing to stack memory */

    /* Capitalize first letter and copy to result */
    if (result_size > strlen(str)) {
        result[0] = str[0];
        for (size_t i = 1; str[i] != '\0' && i < result_size - 1; i++) {
            result[i] = str[i];
        }
        result[strlen(str)] = '\0';
    }
}

/* -- Crash 4: Buffer overflow on stack ------------------------------------ */
/*
 * Diagnosis technique: ASan (compile with -fsanitize=address)
 *
 * This function copies a long string into a small stack buffer
 * without checking the length. This overwrites adjacent stack
 * memory, potentially corrupting the return address.
 *
 * With ASan:
 *   ==ERROR: AddressSanitizer: stack-buffer-overflow
 *
 * With GDB, the crash might appear at a random location because
 * the return address got corrupted.
 *
 * FIX: use strncpy with buffer size limit
 */
int crash4_buffer_overflow(const char *input)
{
    char buf[16];  /* small buffer — only 16 bytes */
    strncpy(buf, input, sizeof(buf) - 1);  /* FIX: bounded copy */
    buf[sizeof(buf) - 1] = '\0';           /* FIX: ensure null termination */

    int len = (int)strlen(buf);
    return len;
}

/* -- Crash 5: Out-of-bounds array access ---------------------------------- */
/*
 * Diagnosis technique: GDB with watchpoint, or ASan
 *
 *   (gdb) break crash5_matrix_sum
 *   (gdb) run
 *   (gdb) print row
 *   (gdb) print col
 *
 * The nested loop uses wrong bounds: it uses COLS for row limit
 * and ROWS for column limit (swapped). If ROWS != COLS, this
 * reads/writes out of bounds.
 *
 * FIX: corrected loop bounds — r < ROWS, c < COLS
 */
#define ROWS 3
#define COLS 5

int crash5_matrix_sum(void)
{
    int matrix[ROWS][COLS];

    /* Initialize: each cell = row * 10 + col */
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            matrix[r][c] = r * 10 + c;
        }
    }

    /* Sum all elements — bounds now correct */
    int sum = 0;
    for (int r = 0; r < ROWS; r++) {  /* FIX: r < ROWS (was r < COLS) */
        for (int c = 0; c < COLS; c++) {  /* FIX: c < COLS (was c < ROWS) */
            sum += matrix[r][c];
        }
    }
    return sum;
}

/* -- Main + Tests --------------------------------------------------------- */

int main(void)
{
    printf("=== Segfault Safari: Diagnose the Crashes ===\n\n");

    /* --- Crash 1 tests --- */
    printf("Crash 1: NULL pointer dereference\n");
    {
        int arr[] = {10, 20, 30};
        TEST("sum of {10,20,30} should be 60");
        EXPECT(crash1_null_deref(arr, 3) == 60);

        TEST("NULL pointer should return 0 (not crash)");
        EXPECT(crash1_null_deref(NULL, 0) == 0);
    }

    /* --- Crash 2 tests --- */
    printf("\nCrash 2: Stack overflow from infinite recursion\n");
    {
        TEST("sum 1..5 should be 15");
        EXPECT(crash2_recursive_sum(5) == 15);

        TEST("sum 1..0 should be 0");
        EXPECT(crash2_recursive_sum(0) == 0);

        TEST("sum 1..100 should be 5050");
        EXPECT(crash2_recursive_sum(100) == 5050);
    }

    /* --- Crash 3 tests --- */
    printf("\nCrash 3: Writing to string literal\n");
    {
        char result[64] = {0};
        crash3_modify_literal(result, sizeof(result));
        TEST("should capitalize to 'Hello world'");
        EXPECT(strcmp(result, "Hello world") == 0);
    }

    /* --- Crash 4 tests --- */
    printf("\nCrash 4: Buffer overflow on stack\n");
    {
        TEST("short string should work fine");
        EXPECT(crash4_buffer_overflow("hi") == 2);

        /* This input is way too long for the 16-byte buffer */
        TEST("long string should not crash (fix the overflow)");
        EXPECT(crash4_buffer_overflow("this is a very long string that overflows") > 0);
    }

    /* --- Crash 5 tests --- */
    printf("\nCrash 5: Out-of-bounds array access\n");
    {
        /*
         * 3x5 matrix where cell[r][c] = r*10 + c:
         *   row 0: 0  1  2  3  4   = 10
         *   row 1: 10 11 12 13 14  = 60
         *   row 2: 20 21 22 23 24  = 110
         *   Total = 180
         */
        TEST("3x5 matrix sum should be 180");
        EXPECT(crash5_matrix_sum() == 180);
    }

    /* --- Summary --- */
    printf("\n========================================\n");
    printf("  Results: %d / %d tests passed\n", tests_passed, tests_total);
    printf("========================================\n");

    if (tests_passed == tests_total) {
        printf("  All crashes fixed! You survived the safari.\n");
    } else {
        printf("  Some crashes remain. Use GDB + ASan to investigate!\n");
    }

    return (tests_passed == tests_total) ? 0 : 1;
}
