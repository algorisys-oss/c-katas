/*
 * Solution: Pool Allocator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ALIGNMENT 8

typedef struct {
    char *buffer;
    size_t slot_size;
    size_t count;
    size_t used;
    void *free_head;
} Pool;

static size_t align_up(size_t size)
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

Pool pool_create(size_t count, size_t slot_size)
{
    Pool p;

    /* Ensure slot can hold at least a pointer (for the free list) */
    if (slot_size < sizeof(void *)) {
        slot_size = sizeof(void *);
    }
    slot_size = align_up(slot_size);

    p.buffer = malloc(count * slot_size);
    if (p.buffer == NULL) {
        p.slot_size = 0;
        p.count = 0;
        p.used = 0;
        p.free_head = NULL;
        return p;
    }

    p.slot_size = slot_size;
    p.count = count;
    p.used = 0;

    /* Build the free list: each slot points to the next */
    for (size_t i = 0; i < count - 1; i++) {
        void **slot = (void **)(p.buffer + i * slot_size);
        *slot = p.buffer + (i + 1) * slot_size;
    }
    /* Last slot points to NULL */
    void **last = (void **)(p.buffer + (count - 1) * slot_size);
    *last = NULL;

    p.free_head = p.buffer;
    return p;
}

void *pool_alloc(Pool *p)
{
    if (p->free_head == NULL) {
        return NULL;
    }
    void *slot = p->free_head;
    /* Advance free_head to the next free slot */
    p->free_head = *(void **)slot;
    p->used++;
    return slot;
}

void pool_free(Pool *p, void *ptr)
{
    /* Push ptr onto the front of the free list */
    *(void **)ptr = p->free_head;
    p->free_head = ptr;
    p->used--;
}

void pool_destroy(Pool *p)
{
    free(p->buffer);
    p->buffer = NULL;
    p->slot_size = 0;
    p->count = 0;
    p->used = 0;
    p->free_head = NULL;
}

/* ── A simple particle struct for testing ──────────────────────────── */

typedef struct {
    float x, y;
    float vx, vy;
    int alive;
} Particle;

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
    printf("=== Pool Allocator (Solution) ===\n\n");

    printf("pool_create:\n");
    {
        Pool p = pool_create(10, sizeof(Particle));
        TEST("buffer is not NULL") EXPECT(p.buffer != NULL);
        TEST("count is 10") EXPECT(p.count == 10);
        TEST("used is 0") EXPECT(p.used == 0);
        TEST("slot_size >= sizeof(Particle)") EXPECT(p.slot_size >= sizeof(Particle));
        pool_destroy(&p);
    }

    printf("\npool_alloc:\n");
    {
        Pool p = pool_create(3, sizeof(Particle));
        Particle *p1 = pool_alloc(&p);
        TEST("first alloc returns non-NULL") EXPECT(p1 != NULL);
        TEST("used is 1") EXPECT(p.used == 1);

        p1->x = 1.0f; p1->y = 2.0f; p1->alive = 1;
        TEST("can write to particle") EXPECT(p1->x == 1.0f && p1->alive == 1);

        Particle *p2 = pool_alloc(&p);
        Particle *p3 = pool_alloc(&p);
        TEST("used is 3 after 3 allocs") EXPECT(p.used == 3);

        Particle *p4 = pool_alloc(&p);
        TEST("alloc when full returns NULL") EXPECT(p4 == NULL);

        (void)p2; (void)p3;
        pool_destroy(&p);
    }

    printf("\npool_free:\n");
    {
        Pool p = pool_create(3, sizeof(Particle));
        Particle *p1 = pool_alloc(&p);
        Particle *p2 = pool_alloc(&p);
        Particle *p3 = pool_alloc(&p);

        pool_free(&p, p2);
        TEST("used is 2 after freeing one") EXPECT(p.used == 2);

        Particle *p4 = pool_alloc(&p);
        TEST("realloc returns freed slot") EXPECT(p4 == p2);
        TEST("used is 3 again") EXPECT(p.used == 3);

        (void)p1; (void)p3;
        pool_destroy(&p);
    }

    printf("\npool_destroy:\n");
    {
        Pool p = pool_create(5, sizeof(Particle));
        pool_alloc(&p);
        pool_destroy(&p);
        TEST("destroy sets buffer to NULL") EXPECT(p.buffer == NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
