/*
 * Solution: Arena Allocator (Bump Allocator)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ALIGNMENT 8

typedef struct {
    char *buffer;
    size_t capacity;
    size_t offset;
} Arena;

static size_t align_up(size_t size)
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

Arena arena_create(size_t capacity)
{
    Arena a;
    a.buffer = malloc(capacity);
    if (a.buffer == NULL) {
        a.capacity = 0;
        a.offset = 0;
        return a;
    }
    a.capacity = capacity;
    a.offset = 0;
    return a;
}

void *arena_alloc(Arena *a, size_t size)
{
    size_t aligned = align_up(size);
    if (a->offset + aligned > a->capacity) {
        return NULL;
    }
    void *ptr = a->buffer + a->offset;
    a->offset += aligned;
    return ptr;
}

void arena_reset(Arena *a)
{
    a->offset = 0;
}

void arena_destroy(Arena *a)
{
    free(a->buffer);
    a->buffer = NULL;
    a->capacity = 0;
    a->offset = 0;
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
    printf("=== Arena Allocator (Solution) ===\n\n");

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

        memset(p1, 0xAA, 10);
        memset(p2, 0xBB, 20);
        TEST("can write to p1") EXPECT(((unsigned char *)p1)[0] == 0xAA);
        TEST("can write to p2") EXPECT(((unsigned char *)p2)[0] == 0xBB);

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
