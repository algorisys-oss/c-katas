/*
 * Solution: Memory Profiler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOCS 256

typedef struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    int active;
} AllocRecord;

static AllocRecord alloc_table[MAX_ALLOCS];
static int alloc_count = 0;

void *prof_malloc(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);
    if (ptr == NULL) return NULL;

    if (alloc_count < MAX_ALLOCS) {
        alloc_table[alloc_count].ptr = ptr;
        alloc_table[alloc_count].size = size;
        alloc_table[alloc_count].file = file;
        alloc_table[alloc_count].line = line;
        alloc_table[alloc_count].active = 1;
        alloc_count++;
    }
    return ptr;
}

void prof_free(void *ptr, const char *file, int line)
{
    if (ptr == NULL) return;

    for (int i = 0; i < alloc_count; i++) {
        if (alloc_table[i].ptr == ptr && alloc_table[i].active) {
            alloc_table[i].active = 0;
            free(ptr);
            return;
        }
    }
    printf("WARNING: free of untracked pointer %p at %s:%d\n", ptr, file, line);
}

int prof_report(void)
{
    int leaks = 0;
    int total_allocs = 0;
    int total_frees = 0;
    size_t leaked_bytes = 0;

    printf("  --- Memory Profiler Report ---\n");

    for (int i = 0; i < alloc_count; i++) {
        total_allocs++;
        if (alloc_table[i].active) {
            leaks++;
            leaked_bytes += alloc_table[i].size;
            printf("  LEAK: %zu bytes at %p (allocated at %s:%d)\n",
                   alloc_table[i].size, alloc_table[i].ptr,
                   alloc_table[i].file, alloc_table[i].line);
        } else {
            total_frees++;
        }
    }

    printf("  Total: %d allocations, %d frees, %d leaks (%zu bytes leaked)\n",
           total_allocs, total_frees, leaks, leaked_bytes);
    return leaks;
}

void prof_reset(void)
{
    alloc_count = 0;
}

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
    printf("=== Memory Profiler (Solution) ===\n\n");

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
        printf("  (Expected leak report below:)\n");
        int leaks = prof_report();
        TEST("1 leak detected") EXPECT(leaks == 1);

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
