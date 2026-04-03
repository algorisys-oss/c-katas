/*
 * Kata: Arena Allocator (Bump Allocator)
 *
 * Build the simplest possible custom allocator:
 *   - arena_create: allocate one big block upfront
 *   - arena_alloc: bump a pointer forward (O(1) allocation)
 *   - arena_reset: reset the pointer to reuse all memory
 *   - arena_destroy: free the underlying block
 *
 * 13 tests total.
 *
 * Hints:
 *   - The arena is just: a buffer, a capacity, and an offset
 *   - arena_alloc returns buffer + offset, then offset += size
 *   - Align allocations to 8 bytes: round size up to next multiple of 8
 *   - Return NULL if there's not enough space left
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ALIGNMENT 8

typedef struct {
    char *buffer;     /* the big block of memory */
    size_t capacity;  /* total size of the block */
    size_t offset;    /* next free byte (bump pointer) */
} Arena;

/* Helper: round up to nearest multiple of ALIGNMENT */
static size_t align_up(size_t size)
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

/* ── Exercise 1: arena_create ─────────────────────────────────────────
 * Allocate a new arena with the given capacity.
 *
 * - malloc a buffer of 'capacity' bytes
 * - Set offset to 0
 * - Return the initialized Arena (by value)
 * - If malloc fails, return an arena with buffer=NULL, capacity=0
 */
Arena arena_create(size_t capacity)
{
    Arena a;
    /* TODO: malloc a buffer, initialize fields */
    a.buffer = NULL;
    a.capacity = 0;
    a.offset = 0;
    return a;
}

/* ── Exercise 2: arena_alloc ──────────────────────────────────────────
 * Allocate 'size' bytes from the arena.
 *
 * - Align the size up to ALIGNMENT bytes
 * - If there's enough room (offset + aligned_size <= capacity), return
 *   a pointer to buffer + offset and bump offset forward
 * - If not enough room, return NULL
 *
 * Returns: pointer to the allocated memory, or NULL
 */
void *arena_alloc(Arena *a, size_t size)
{
    /* TODO: align size, check bounds, bump offset, return pointer */
    (void)a;
    (void)size;
    (void)align_up; /* suppress unused warning until you use it */
    return NULL; /* placeholder */
}

/* ── Exercise 3: arena_reset ──────────────────────────────────────────
 * Reset the arena so all memory can be reused.
 * (Don't free the buffer — just reset the offset to 0.)
 */
void arena_reset(Arena *a)
{
    /* TODO: set offset back to 0 */
    (void)a;
}

/* ── Exercise 4: arena_destroy ────────────────────────────────────────
 * Free the arena's buffer and reset all fields.
 */
void arena_destroy(Arena *a)
{
    /* TODO: free buffer, set buffer=NULL, capacity=0, offset=0 */
    (void)a;
}

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
    printf("=== Arena Allocator ===\n\n");

    printf("arena_create:\n");
    {
        Arena a = arena_create(256);
        TEST("buffer is not NULL") EXPECT(a.buffer != NULL);
        TEST("capacity is 256") EXPECT(a.capacity == 256);
        TEST("offset is 0") EXPECT(a.offset == 0);
        arena_destroy(&a);
    }

    printf("\narena_alloc:\n");
    {
        Arena a = arena_create(64);
        void *p1 = arena_alloc(&a, 10);
        TEST("first alloc returns non-NULL") EXPECT(p1 != NULL);
        TEST("offset moved forward (aligned to 16)") EXPECT(a.offset == 16);

        void *p2 = arena_alloc(&a, 20);
        TEST("second alloc returns non-NULL") EXPECT(p2 != NULL);
        TEST("p2 is after p1") EXPECT((char *)p2 > (char *)p1);

        /* Write to allocated memory to verify it works */
        memset(p1, 0xAA, 10);
        memset(p2, 0xBB, 20);
        TEST("can write to p1") EXPECT(((unsigned char *)p1)[0] == 0xAA);
        TEST("can write to p2") EXPECT(((unsigned char *)p2)[0] == 0xBB);

        /* Try to allocate more than remaining space */
        void *p3 = arena_alloc(&a, 100);
        TEST("alloc too large returns NULL") EXPECT(p3 == NULL);
        arena_destroy(&a);
    }

    printf("\narena_reset:\n");
    {
        Arena a = arena_create(64);
        arena_alloc(&a, 32);
        arena_reset(&a);
        TEST("reset sets offset to 0") EXPECT(a.offset == 0);

        /* Can allocate again after reset */
        void *p = arena_alloc(&a, 32);
        TEST("can allocate after reset") EXPECT(p != NULL);
        arena_destroy(&a);
    }

    printf("\narena_destroy:\n");
    {
        Arena a = arena_create(64);
        arena_alloc(&a, 16);
        arena_destroy(&a);
        TEST("destroy sets buffer to NULL") EXPECT(a.buffer == NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
