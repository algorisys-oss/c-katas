/*
 * debug_pointers.c -- Find and Fix the Bugs!
 *
 * Each function below has exactly ONE bug related to pointers or arrays.
 * Read the comment above each function to understand what it SHOULD do,
 * then find the bug and fix it.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -o debug_pointers debug_pointers.c
 * Run:      ./debug_pointers
 *
 * When all bugs are fixed, every test should print [PASS].
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
 * BUG #1: Off-by-one in array access
 *
 * This function is supposed to sum all elements of an integer array.
 * But it reads past the end of the array, which can crash or give
 * wrong results.
 *
 * HINT: If an array has `n` elements, what are the valid indices?
 *       Remember: arrays in C are indexed from 0 to n-1.
 * ====================================================================== */
int sum_array(int *arr, int n) {
    int total = 0;
    for (int i = 0; i <= n; i++) {   /* <-- BUG IS HERE */
        total += arr[i];
    }
    return total;
}

/* ======================================================================
 * BUG #2: Dangling pointer (returning address of a local variable)
 *
 * This function is supposed to create a new integer set to `value`
 * and return a pointer to it. The caller will use and later free it.
 *
 * HINT: Local variables live on the stack. What happens to the stack
 *       frame when a function returns? Where should long-lived data go?
 * ====================================================================== */
int *make_int(int value) {
    int result = value;              /* <-- BUG: this is a local variable */
    return &result;                  /* returning address of local! */
}

/* ======================================================================
 * BUG #3: Wrong level of indirection
 *
 * This function is supposed to allocate an array of `n` integers and
 * store the pointer in the caller's variable (via `out`).
 * But the caller's pointer never actually gets updated.
 *
 * HINT: To modify an `int` in another function, you pass `int *`.
 *       So to modify an `int *` in another function, you need...?
 * ====================================================================== */
void alloc_array(int *out, int n) {  /* <-- BUG: wrong parameter type */
    out = malloc(n * sizeof(int));
    if (out == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        out[i] = i * 10;
    }
}

/* ======================================================================
 * BUG #4: Forgot to null-terminate a string
 *
 * This function copies the first `n` characters of `src` into a newly
 * allocated string and returns it. The caller will use it as a normal
 * C string (e.g., with printf, strcmp, strlen).
 *
 * HINT: What makes a char array a valid C string? What goes at the end?
 *       How much space do you need to allocate for n characters?
 * ====================================================================== */
char *copy_first_n(const char *src, int n) {
    char *dst = malloc(n);           /* <-- BUG: not enough space */
    if (dst == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    /* missing something here... */
    return dst;
}

/* ======================================================================
 * BUG #5: Pointer arithmetic mistake (sizeof not accounted for)
 *
 * This function takes an array of ints and tries to read the element
 * at a given index using raw pointer arithmetic with a char* cast.
 * It's doing the byte offset calculation wrong.
 *
 * HINT: An `int` is typically 4 bytes. If you cast an int pointer to
 *       a char pointer, adding 1 moves by 1 BYTE, not 1 int.
 *       How many bytes do you need to skip to reach element `index`?
 * ====================================================================== */
int read_element(int *arr, int index) {
    char *raw = (char *)arr;
    /* Move to the right position byte-by-byte */
    int *target = (int *)(raw + index);  /* <-- BUG: wrong offset */
    return *target;
}

/* ======================================================================
 * MAIN -- Test Harness
 *
 * These tests will FAIL until you fix each bug above.
 * ====================================================================== */
int main(void) {
    printf("\n=== Debug Pointers: Find & Fix the Bugs ===\n\n");

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
        free(p);  /* caller is responsible for freeing */
    }

    /* --- Test Bug #3: Wrong indirection level --- */
    {
        int *arr = NULL;
        alloc_array(arr, 5);
        TEST("Bug #3: alloc_array sets caller's pointer");
        EXPECT(arr != NULL);

        if (arr != NULL) {
            TEST("Bug #3: alloc_array fills values");
            EXPECT(arr[0] == 0 && arr[2] == 20 && arr[4] == 40);
            free(arr);
        } else {
            TEST("Bug #3: alloc_array fills values");
            EXPECT(0);  /* auto-fail since arr is NULL */
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
