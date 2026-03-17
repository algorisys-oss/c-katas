/*
 * Kata: Mini Malloc (Free-List Allocator)
 *
 * Build a simplified version of malloc using a free-list:
 *   - my_malloc: find a free block big enough, split if needed
 *   - my_free: return a block to the free list, coalesce neighbors
 *
 * This allocator works on a pre-allocated buffer (no system calls).
 *
 * 11 tests total.
 *
 * Hints:
 *   - Each block has a header: size (size_t) + is_free (int)
 *   - The free list is implicit: walk through blocks sequentially
 *   - Use first-fit: return the first free block that's big enough
 *   - When freeing, merge adjacent free blocks (coalescing)
 *   - Minimum allocation: align to 8 bytes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HEAP_SIZE 1024
#define ALIGNMENT 8
#define HEADER_SIZE (align_up(sizeof(BlockHeader)))

/*
 * Block header — stored before each user data region.
 *
 *   [header][      user data       ][header][    user data   ] ...
 *    size    ← my_malloc returns     size
 *    free?     pointer here          free?
 */
typedef struct {
    size_t size;     /* size of user data (not including header) */
    int is_free;     /* 1 if free, 0 if allocated */
} BlockHeader;

/* The heap: a statically allocated buffer */
static char heap[HEAP_SIZE];
static int heap_initialized = 0;

static size_t align_up(size_t size)
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

/* ── Exercise 1: heap_init ────────────────────────────────────────────
 * Initialize the heap with one big free block.
 *
 * - Place a BlockHeader at the start of the heap buffer
 * - Set its size to HEAP_SIZE - HEADER_SIZE (remaining space)
 * - Mark it as free
 * - Set heap_initialized to 1
 */
void heap_init(void)
{
    /* TODO: place initial header, mark as free */
}

/* Helper: get the header pointer for a block at a given offset */
static BlockHeader *get_header(size_t offset)
{
    return (BlockHeader *)(heap + offset);
}

/* Helper: get the next block's offset, or HEAP_SIZE if none */
static size_t next_block_offset(size_t offset)
{
    BlockHeader *hdr = get_header(offset);
    return offset + HEADER_SIZE + hdr->size;
}

/* ── Exercise 2: my_malloc ────────────────────────────────────────────
 * Allocate 'size' bytes from the heap.
 *
 * - Initialize heap on first call
 * - Align size up to ALIGNMENT
 * - Walk through blocks (starting at offset 0):
 *   - If block is free and large enough:
 *     - If the block is much larger than needed (leftover >= HEADER_SIZE + ALIGNMENT),
 *       split it: shrink this block, create a new free block after it
 *     - Mark the block as allocated
 *     - Return pointer to the data area (right after the header)
 * - If no suitable block found, return NULL
 *
 * Returns: pointer to user data, or NULL
 */
void *my_malloc(size_t size)
{
    /* TODO: walk the block list, find first fit, split if needed */
    (void)size;
    return NULL; /* placeholder */
}

/* ── Exercise 3: my_free ──────────────────────────────────────────────
 * Free a previously allocated block.
 *
 * - Find the header (it's right before the user data pointer)
 * - Mark the block as free
 * - Coalesce: walk all blocks from the start and merge adjacent free blocks
 *
 * Coalescing: if block at offset A is free and the next block at offset B
 * is also free, merge them: A's size += HEADER_SIZE + B's size
 */
void my_free(void *ptr)
{
    /* TODO: mark as free, then coalesce adjacent free blocks */
    (void)ptr;
}

/* ── Helper for tests: count free/used blocks ─────────────────────── */

static int count_free_blocks(void)
{
    int count = 0;
    size_t offset = 0;
    while (offset < HEAP_SIZE) {
        BlockHeader *hdr = get_header(offset);
        if (hdr->is_free) count++;
        offset = next_block_offset(offset);
    }
    return count;
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
    printf("=== Mini Malloc ===\n\n");

    /* Reset heap for each test group */
    printf("basic allocation:\n");
    {
        heap_initialized = 0;
        void *p1 = my_malloc(32);
        TEST("first alloc returns non-NULL") EXPECT(p1 != NULL);

        /* Write to verify it works */
        memset(p1, 0xAA, 32);
        TEST("can write 32 bytes") EXPECT(((unsigned char *)p1)[31] == 0xAA);

        void *p2 = my_malloc(64);
        TEST("second alloc returns non-NULL") EXPECT(p2 != NULL);
        TEST("p2 is after p1") EXPECT((char *)p2 > (char *)p1);

        void *p3 = my_malloc(128);
        TEST("third alloc returns non-NULL") EXPECT(p3 != NULL);
    }

    printf("\nallocation failure:\n");
    {
        heap_initialized = 0;
        void *p = my_malloc(HEAP_SIZE);  /* too big */
        TEST("alloc entire heap returns NULL") EXPECT(p == NULL);

        /* Fill the heap */
        my_malloc(HEAP_SIZE - HEADER_SIZE - ALIGNMENT);
        void *p2 = my_malloc(1);
        TEST("alloc when full returns NULL") EXPECT(p2 == NULL);
    }

    printf("\nfree and reuse:\n");
    {
        heap_initialized = 0;
        void *p1 = my_malloc(100);
        void *p2 = my_malloc(100);
        my_free(p1);
        TEST("after free, block is reusable") EXPECT(count_free_blocks() >= 1);

        void *p3 = my_malloc(100);
        TEST("realloc into freed block") EXPECT(p3 == p1);
        (void)p2;
    }

    printf("\ncoalescing:\n");
    {
        heap_initialized = 0;
        void *p1 = my_malloc(100);
        void *p2 = my_malloc(100);
        void *p3 = my_malloc(100);
        my_free(p1);
        my_free(p2);
        my_free(p3);
        int free_count = count_free_blocks();
        TEST("three adjacent frees coalesce") EXPECT(free_count <= 2);
        /* After coalescing, should be able to allocate a large block */
        void *p4 = my_malloc(300);
        TEST("can allocate 300 bytes after coalescing") EXPECT(p4 != NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
