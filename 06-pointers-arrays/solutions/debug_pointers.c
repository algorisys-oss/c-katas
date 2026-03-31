/*
 * debug_pointers.c -- SOLUTIONS (all bugs fixed)
 *
 * Each fix is marked with a "FIX" comment explaining the change.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -o debug_pointers debug_pointers.c
 * Run:      ./debug_pointers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ---- Test Harness ---- */
#define TEST(name) printf("  %-40s", name)
#define PASS() printf("[PASS]\n")
static int tests_passed = 0, tests_failed = 0;
#define EXPECT(cond) do { \
    if (cond) { PASS(); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); tests_failed++; } \
} while(0)

/* ======================================================================
 * BUG #1 FIX: Changed  i <= n  to  i < n
 *
 * An array of n elements has valid indices 0 through n-1.
 * Using <= n reads one element past the end (undefined behavior).
 * ====================================================================== */
int sum_array(int *arr, int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {    /* FIX: < instead of <= */
        total += arr[i];
    }
    return total;
}

/* ======================================================================
 * BUG #2 FIX: Use malloc instead of a local variable
 *
 * Local variables are destroyed when the function returns (their stack
 * frame is reclaimed). To return data that outlives the function, we
 * must allocate it on the heap with malloc.
 * ====================================================================== */
int *make_int(int value) {
    int *result = malloc(sizeof(int));  /* FIX: allocate on the heap */
    if (result == NULL) {
        return NULL;
    }
    *result = value;
    return result;
}

/* ======================================================================
 * BUG #3 FIX: Take int **out instead of int *out
 *
 * To modify the caller's pointer, we need a pointer TO a pointer.
 * This is the same principle as passing int* to modify an int:
 * to modify an int*, we pass int**.
 * ====================================================================== */
void alloc_array(int **out, int n) {   /* FIX: int ** instead of int * */
    *out = malloc(n * sizeof(int));    /* FIX: dereference to set caller's pointer */
    if (*out == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        (*out)[i] = i * 10;           /* FIX: dereference before indexing */
    }
}

/* ======================================================================
 * BUG #4 FIX: Allocate n+1 bytes and add '\0' at the end
 *
 * A C string must end with a null terminator ('\0'). We need to:
 * 1. Allocate n+1 bytes (n characters + 1 for '\0')
 * 2. Write '\0' after copying the characters
 * ====================================================================== */
char *copy_first_n(const char *src, int n) {
    char *dst = malloc(n + 1);       /* FIX: allocate space for '\0' */
    if (dst == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    dst[n] = '\0';                   /* FIX: null-terminate the string */
    return dst;
}

/* ======================================================================
 * BUG #5 FIX: Multiply index by sizeof(int) for byte offset
 *
 * When we cast to char*, pointer arithmetic works in units of 1 byte.
 * But each int occupies sizeof(int) bytes (typically 4). So to reach
 * element `index`, we must skip index * sizeof(int) bytes.
 * ====================================================================== */
int read_element(int *arr, int index) {
    char *raw = (char *)arr;
    /* FIX: multiply by sizeof(int) to get correct byte offset */
    int *target = (int *)(raw + index * sizeof(int));
    return *target;
}

/* ======================================================================
 * MAIN -- Test Harness
 * ====================================================================== */
int main(void) {
    printf("\n=== Debug Pointers: Find & Fix the Bugs (SOLUTIONS) ===\n\n");

    /* --- Test Bug #1: Off-by-one --- */
    {
        int nums[] = {10, 20, 30, 40, 50};
        int result = sum_array(nums, 5);
        TEST("Bug #1: sum_array({10,20,30,40,50})");
        EXPECT(result == 150);
    }

    /* --- Test Bug #2: Dangling pointer --- */
    {
        int *p = make_int(42);
        TEST("Bug #2: make_int(42)");
        EXPECT(p != NULL && *p == 42);
        free(p);
    }

    /* --- Test Bug #3: Wrong indirection level --- */
    {
        int *arr = NULL;
        alloc_array(&arr, 5);         /* FIX: pass address of pointer */
        TEST("Bug #3: alloc_array sets caller's pointer");
        EXPECT(arr != NULL);

        if (arr != NULL) {
            TEST("Bug #3: alloc_array fills values");
            EXPECT(arr[0] == 0 && arr[2] == 20 && arr[4] == 40);
            free(arr);
        } else {
            TEST("Bug #3: alloc_array fills values");
            EXPECT(0);
        }
    }

    /* --- Test Bug #4: Missing null terminator --- */
    {
        const char *src = "Hello, World!";
        char *sub = copy_first_n(src, 5);
        TEST("Bug #4: copy_first_n(\"Hello, World!\", 5)");
        EXPECT(sub != NULL && strcmp(sub, "Hello") == 0);

        TEST("Bug #4: strlen is correct");
        EXPECT(sub != NULL && strlen(sub) == 5);
        free(sub);
    }

    /* --- Test Bug #5: Pointer arithmetic --- */
    {
        int data[] = {100, 200, 300, 400, 500};
        TEST("Bug #5: read_element(data, 0)");
        EXPECT(read_element(data, 0) == 100);

        TEST("Bug #5: read_element(data, 3)");
        EXPECT(read_element(data, 3) == 400);

        TEST("Bug #5: read_element(data, 4)");
        EXPECT(read_element(data, 4) == 500);
    }

    /* --- Summary --- */
    printf("\n  ----------------------------------------\n");
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
