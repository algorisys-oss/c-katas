/*
 * Kata: Pool Allocator
 *
 * Build a pool allocator for a particle system. All allocations are
 * the same size. Free slots form a linked list using the slot memory itself.
 *
 *   - pool_create: allocate N slots of a given size
 *   - pool_alloc: pop a slot from the free list (O(1))
 *   - pool_free: push a slot back onto the free list (O(1))
 *   - pool_destroy: free the underlying memory
 *
 * 13 tests total.
 *
 * Hints:
 *   - The free list is stored INSIDE the free slots (no extra memory needed)
 *   - Each free slot's first bytes store a pointer to the next free slot
 *   - slot_size must be >= sizeof(void*) so we can store the next pointer
 *   - When a slot is allocated, its memory is given to the user
 *   - When freed, we write the free-list pointer back into the slot
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char *buffer;       /* the big block of memory */
    size_t slot_size;   /* size of each slot (aligned) */
    size_t count;       /* total number of slots */
    size_t used;        /* number of currently allocated slots */
    void *free_head;    /* pointer to first free slot (linked list) */
} Pool;

/* ── Exercise 1: pool_create ──────────────────────────────────────────
 * Create a pool with 'count' slots, each of size 'slot_size'.
 *
 * - Ensure slot_size is at least sizeof(void*) (we need room for the next ptr)
 * - Align slot_size up to 8 bytes
 * - malloc a buffer of count * aligned_slot_size bytes
 * - Initialize the free list: link all slots together
 *   (each slot's first bytes point to the next slot, last slot points to NULL)
 * - Set used to 0
 *
 * Returns: initialized Pool (by value)
 */
Pool pool_create(size_t count, size_t slot_size)
{
    Pool p;
    /* TODO: calculate aligned size, malloc buffer, build free list */
    p.buffer = NULL;
    p.slot_size = 0;
    p.count = 0;
    p.used = 0;
    p.free_head = NULL;
    (void)count;
    (void)slot_size;
    return p;
}

/* ── Exercise 2: pool_alloc ───────────────────────────────────────────
 * Allocate one slot from the pool.
 *
 * - If free_head is NULL, the pool is full → return NULL
 * - Otherwise, save free_head, advance free_head to the next free slot
 *   (read the next pointer stored in the slot), increment used, return the slot
 *
 * Returns: pointer to the allocated slot, or NULL if full
 */
void *pool_alloc(Pool *p)
{
    /* TODO: pop from free list, increment used */
    (void)p;
    return NULL; /* placeholder */
}

/* ── Exercise 3: pool_free ────────────────────────────────────────────
 * Return a slot to the pool.
 *
 * - Write free_head into the slot's first bytes (make it point to old head)
 * - Set free_head to this slot
 * - Decrement used
 *
 * Note: no bounds checking needed for this kata, but in production you'd
 * verify the pointer is within the pool's buffer range.
 */
void pool_free(Pool *p, void *ptr)
{
    /* TODO: push onto free list, decrement used */
    (void)p;
    (void)ptr;
}

/* ── Exercise 4: pool_destroy ─────────────────────────────────────────
 * Free the pool's buffer and reset all fields.
 */
void pool_destroy(Pool *p)
{
    /* TODO: free buffer, reset all fields */
    (void)p;
}

/* ── A simple particle struct for testing ──────────────────────────── */

typedef struct {
    float x, y;       /* position */
    float vx, vy;     /* velocity */
    int alive;        /* 1 if active */
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
    printf("=== Pool Allocator ===\n\n");

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

        /* Use the allocated particle */
        p1->x = 1.0f; p1->y = 2.0f; p1->alive = 1;
        TEST("can write to particle") EXPECT(p1->x == 1.0f && p1->alive == 1);

        Particle *p2 = pool_alloc(&p);
        Particle *p3 = pool_alloc(&p);
        TEST("used is 3 after 3 allocs") EXPECT(p.used == 3);

        /* Pool is full */
        Particle *p4 = pool_alloc(&p);
        TEST("alloc when full returns NULL") EXPECT(p4 == NULL);

        /* Silence unused warnings */
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

        /* Reallocate into the freed slot */
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
