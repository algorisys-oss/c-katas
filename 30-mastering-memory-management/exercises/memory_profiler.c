/*
 * Kata: Memory Profiler
 *
 * Build a profiler that wraps malloc/free using macros, tracks every
 * allocation (file, line, size), and reports leaks at program exit.
 *
 * The approach:
 *   - Define macros: PROF_MALLOC(size) and PROF_FREE(ptr) that call
 *     profiled versions with __FILE__ and __LINE__
 *   - Keep a table of active allocations
 *   - At exit, report any allocations that were never freed
 *
 * 9 tests total.
 *
 * Hints:
 *   - Use a simple array of records (fixed max size is fine)
 *   - Each record stores: pointer, size, file, line, active flag
 *   - PROF_MALLOC calls prof_malloc(size, __FILE__, __LINE__)
 *   - PROF_FREE calls prof_free(ptr, __FILE__, __LINE__)
 *   - prof_report() walks the table and prints un-freed allocations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Maximum tracked allocations ──────────────────────────────────── */
#define MAX_ALLOCS 256

/* ── Allocation record ────────────────────────────────────────────── */
typedef struct {
    void *ptr;           /* the allocated pointer */
    size_t size;         /* allocation size */
    const char *file;    /* source file where allocated */
    int line;            /* source line where allocated */
    int active;          /* 1 if still allocated, 0 if freed */
} AllocRecord;

/* ── Global profiler state ────────────────────────────────────────── */
static AllocRecord alloc_table[MAX_ALLOCS];
static int alloc_count = 0;  /* total records ever created */

/* ── Exercise 1: prof_malloc ──────────────────────────────────────────
 * Allocate memory and record it in the table.
 *
 * - Call the real malloc(size)
 * - If successful, add a record to alloc_table with the pointer, size,
 *   file, line, and active=1
 * - Increment alloc_count
 * - Return the pointer (or NULL if malloc failed)
 */
void *prof_malloc(size_t size, const char *file, int line)
{
    /* TODO: call real malloc, record in table, return pointer */
    (void)size; (void)file; (void)line;
    return NULL; /* placeholder */
}

/* ── Exercise 2: prof_free ────────────────────────────────────────────
 * Free memory and mark the record as inactive.
 *
 * - Search alloc_table for a record matching ptr with active=1
 * - If found, mark it as active=0 and call the real free(ptr)
 * - If not found, print a warning: "WARNING: free of untracked pointer"
 */
void prof_free(void *ptr, const char *file, int line)
{
    /* TODO: find the record, mark inactive, call real free */
    (void)ptr; (void)file; (void)line;
}

/* ── Exercise 3: prof_report ──────────────────────────────────────────
 * Print a report of all leaked allocations (active=1 records).
 *
 * For each leaked record, print:
 *   "LEAK: %zu bytes at %p (allocated at %s:%d)\n"
 *
 * At the end, print:
 *   "Total: X allocations, Y frees, Z leaks (%zu bytes leaked)\n"
 *
 * Returns: number of leaks found
 */
int prof_report(void)
{
    /* TODO: walk alloc_table, count and print leaks */
    return 0; /* placeholder */
}

/* ── Exercise 4: prof_reset ───────────────────────────────────────────
 * Reset the profiler state (for testing).
 * Set alloc_count to 0. (Does NOT free any memory — just clears tracking.)
 */
void prof_reset(void)
{
    /* TODO: reset alloc_count to 0 */
}

/* ── Macros that capture file/line automatically ──────────────────── */
#define PROF_MALLOC(size) prof_malloc(size, __FILE__, __LINE__)
#define PROF_FREE(ptr) prof_free(ptr, __FILE__, __LINE__)

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Memory Profiler ===\n\n");

    printf("prof_malloc:\n");
    {
        prof_reset();
        void *p = PROF_MALLOC(100);
        TEST("returns non-NULL") EXPECT(p != NULL);
        TEST("alloc_count is 1") EXPECT(alloc_count == 1);
        TEST("record is active") EXPECT(alloc_table[0].active == 1);
        TEST("record size is 100") EXPECT(alloc_table[0].size == 100);
        PROF_FREE(p);
    }

    printf("\nprof_free:\n");
    {
        prof_reset();
        void *p = PROF_MALLOC(50);
        PROF_FREE(p);
        TEST("after free, record is inactive") EXPECT(alloc_table[0].active == 0);
        TEST("alloc_count stays at 1") EXPECT(alloc_count == 1);
    }

    printf("\nprof_report (no leaks):\n");
    {
        prof_reset();
        void *a = PROF_MALLOC(32);
        void *b = PROF_MALLOC(64);
        PROF_FREE(a);
        PROF_FREE(b);
        int leaks = prof_report();
        TEST("no leaks detected") EXPECT(leaks == 0);
    }

    printf("\nprof_report (with leaks):\n");
    {
        prof_reset();
        void *a = PROF_MALLOC(100);
        void *b = PROF_MALLOC(200);
        PROF_FREE(a);
        /* b is intentionally leaked */
        printf("  (Expected leak report below:)\n");
        int leaks = prof_report();
        TEST("1 leak detected") EXPECT(leaks == 1);

        /* Clean up the leak for valgrind */
        free(b);
    }

    printf("\nprof_reset:\n");
    {
        prof_reset();
        TEST("alloc_count is 0 after reset") EXPECT(alloc_count == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
