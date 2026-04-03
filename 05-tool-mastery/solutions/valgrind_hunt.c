/*
 * Kata: Valgrind Hunt
 *
 * This file contains 6 functions, each with a different memory error
 * that valgrind can detect. Your job: run valgrind, read its output,
 * find the bug, and fix it.
 *
 * How to work through this:
 *   1. Compile:  gcc -std=c99 -Wall -Wextra -pedantic -g -o valgrind_hunt valgrind_hunt.c
 *   2. Run:      valgrind --leak-check=full --track-origins=yes ./valgrind_hunt
 *   3. Read the valgrind output carefully for each function
 *   4. Fix each bug, recompile, run valgrind again
 *   5. Goal: zero errors, zero leaks
 *
 * Each function is self-contained. Comments explain what valgrind
 * error to expect.
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

/* -- Leak 1: malloc without free ------------------------------------------ */
/*
 * Valgrind will report:
 *   LEAK SUMMARY: definitely lost: 40 bytes in 1 blocks
 *
 * The function allocates memory for an array, uses it to compute
 * a result, but never frees the array. The fix: add free() before
 * returning.
 *
 * FIX: added free(arr) before return
 */
int leak1_sum_allocated(int n)
{
    int *arr = malloc((size_t)n * sizeof(int));
    if (!arr) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;  /* arr = {1, 2, 3, ..., n} */
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    free(arr);  /* FIX: free the allocated memory */
    return sum;
}

/* -- Leak 2: realloc loses original pointer ------------------------------- */
/*
 * Valgrind will report:
 *   LEAK SUMMARY: definitely lost: some bytes
 *
 * When realloc fails, it returns NULL but does NOT free the original
 * block. If you assign the result directly to your only pointer,
 * you lose the original memory forever.
 *
 * Even when realloc succeeds, the pattern here leaks because
 * we never free at the end.
 *
 * FIX: use a temporary pointer for realloc; caller frees the result
 */
char *leak2_build_string(void)
{
    char *str = malloc(8);
    if (!str) {
        return NULL;
    }
    strcpy(str, "Hello");

    /* FIX: use temp pointer so we don't lose str if realloc fails */
    char *tmp = realloc(str, 64);
    if (!tmp) {
        free(str);  /* FIX: free original on failure */
        return NULL;
    }
    str = tmp;
    strcat(str, ", World!");

    return str;  /* Caller must free — but we should document this */
}

/* -- Bug 3: Read past end of allocated block ------------------------------ */
/*
 * Valgrind will report:
 *   Invalid read of size 4
 *   Address 0x... is 0 bytes after a block of size 20 alloc'd
 *
 * We allocate space for 5 ints but read 6 elements.
 * The fix: don't read past the allocated size.
 *
 * FIX: changed loop bound from i < 6 to i < 5
 */
int bug3_overread(void)
{
    int *arr = malloc(5 * sizeof(int));
    if (!arr) {
        return -1;
    }

    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;  /* {10, 20, 30, 40, 50} */
    }

    int sum = 0;
    for (int i = 0; i < 5; i++) {  /* FIX: i < 5 (was i < 6) */
        sum += arr[i];
    }

    free(arr);
    return sum;
}

/* -- Bug 4: Use after free ------------------------------------------------ */
/*
 * Valgrind will report:
 *   Invalid read of size 1
 *   Address 0x... is 0 bytes inside a block of size 12 free'd
 *
 * We free the string, then try to compute its length.
 * The fix: use the data BEFORE freeing it.
 *
 * FIX: moved free(msg) after strlen(msg)
 */
int bug4_use_after_free(void)
{
    char *msg = malloc(32);
    if (!msg) {
        return -1;
    }
    strcpy(msg, "important!");

    int len = (int)strlen(msg);  /* FIX: use before free (was after free) */
    free(msg);

    return len;
}

/* -- Bug 5: Double free --------------------------------------------------- */
/*
 * Valgrind will report:
 *   Invalid free() / delete / delete[] / realloc()
 *   Address 0x... is 0 bytes inside a block of size 24 free'd
 *
 * We free the same pointer twice. This is undefined behavior and
 * can cause crashes or security vulnerabilities.
 * The fix: only free once. Set pointer to NULL after free.
 *
 * FIX: removed second free, set pointer to NULL
 */
int bug5_double_free(void)
{
    int *data = malloc(6 * sizeof(int));
    if (!data) {
        return -1;
    }

    for (int i = 0; i < 6; i++) {
        data[i] = i * i;  /* {0, 1, 4, 9, 16, 25} */
    }

    int sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += data[i];
    }

    free(data);
    data = NULL;  /* FIX: removed second free, set to NULL */
    return sum;
}

/* -- Bug 6: Use of uninitialized value from malloc ------------------------ */
/*
 * Valgrind will report:
 *   Conditional jump or move depends on uninitialised value(s)
 *   (with --track-origins=yes, it will point to the malloc)
 *
 * malloc does NOT zero-initialize memory. The bytes could be
 * anything. We read from the buffer without writing first.
 * The fix: use calloc, or initialize the memory before reading.
 *
 * FIX: changed malloc to calloc
 */
int bug6_uninitialized_read(void)
{
    int *counts = calloc(5, sizeof(int));  /* FIX: calloc (was malloc) */
    if (!counts) {
        return -1;
    }

    /* Increment some counts (now properly zero-initialized) */
    counts[0] += 10;
    counts[2] += 20;
    counts[4] += 30;

    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += counts[i];
    }

    free(counts);
    return total;
}

/* -- Main + Tests --------------------------------------------------------- */

int main(void)
{
    printf("=== Valgrind Hunt: Find the Memory Errors ===\n\n");

    /* --- Leak 1 tests --- */
    printf("Leak 1: malloc without free\n");
    {
        int sum = leak1_sum_allocated(10);
        TEST("sum of 1..10 should be 55");
        EXPECT(sum == 55);
    }

    /* --- Leak 2 tests --- */
    printf("\nLeak 2: realloc loses original pointer\n");
    {
        char *result = leak2_build_string();
        TEST("should build 'Hello, World!'");
        EXPECT(result != NULL && strcmp(result, "Hello, World!") == 0);
        free(result);  /* FIX: caller frees the result */
    }

    /* --- Bug 3 tests --- */
    printf("\nBug 3: Read past end of allocated block\n");
    {
        int sum = bug3_overread();
        TEST("sum of {10,20,30,40,50} should be 150");
        EXPECT(sum == 150);
    }

    /* --- Bug 4 tests --- */
    printf("\nBug 4: Use after free\n");
    {
        int len = bug4_use_after_free();
        TEST("length of 'important!' should be 10");
        EXPECT(len == 10);
    }

    /* --- Bug 5 tests --- */
    printf("\nBug 5: Double free\n");
    {
        int sum = bug5_double_free();
        TEST("sum of {0,1,4,9,16,25} should be 55");
        EXPECT(sum == 55);
    }

    /* --- Bug 6 tests --- */
    printf("\nBug 6: Uninitialized value from malloc\n");
    {
        int total = bug6_uninitialized_read();
        TEST("total of increments should be 60");
        EXPECT(total == 60);
    }

    /* --- Summary --- */
    printf("\n========================================\n");
    printf("  Results: %d / %d tests passed\n", tests_passed, tests_total);
    printf("========================================\n");

    if (tests_passed == tests_total) {
        printf("  All memory errors fixed!\n");
        printf("  Now run: valgrind --leak-check=full ./valgrind_hunt\n");
        printf("  Goal: 0 errors, 0 leaks.\n");
    } else {
        printf("  Some tests failing. Fix bugs and check with valgrind!\n");
    }

    return (tests_passed == tests_total) ? 0 : 1;
}
