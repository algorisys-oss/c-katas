/*
 * Solution: Mini Malloc (Free-List Allocator)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HEAP_SIZE 1024
#define ALIGNMENT 8
#define HEADER_SIZE (align_up(sizeof(BlockHeader)))

typedef struct {
    size_t size;
    int is_free;
} BlockHeader;

static char heap[HEAP_SIZE];
static int heap_initialized = 0;

static size_t align_up(size_t size)
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

static BlockHeader *get_header(size_t offset)
{
    return (BlockHeader *)(heap + offset);
}

static size_t next_block_offset(size_t offset)
{
    BlockHeader *hdr = get_header(offset);
    return offset + HEADER_SIZE + hdr->size;
}

void heap_init(void)
{
    BlockHeader *hdr = get_header(0);
    hdr->size = HEAP_SIZE - HEADER_SIZE;
    hdr->is_free = 1;
    heap_initialized = 1;
}

static void coalesce(void)
{
    size_t offset = 0;
    while (offset < HEAP_SIZE) {
        BlockHeader *hdr = get_header(offset);
        size_t next = next_block_offset(offset);
        if (hdr->is_free && next < HEAP_SIZE) {
            BlockHeader *next_hdr = get_header(next);
            if (next_hdr->is_free) {
                /* Merge: absorb next block into this one */
                hdr->size += HEADER_SIZE + next_hdr->size;
                continue; /* Check again — might merge more */
            }
        }
        offset = next_block_offset(offset);
    }
}

void *my_malloc(size_t size)
{
    if (!heap_initialized) {
        heap_init();
    }

    size = align_up(size);

    size_t offset = 0;
    while (offset < HEAP_SIZE) {
        BlockHeader *hdr = get_header(offset);
        if (hdr->is_free && hdr->size >= size) {
            /* Split if there's enough leftover for a new block */
            if (hdr->size >= size + HEADER_SIZE + ALIGNMENT) {
                size_t remaining = hdr->size - size - HEADER_SIZE;
                hdr->size = size;

                BlockHeader *new_block = get_header(offset + HEADER_SIZE + size);
                new_block->size = remaining;
                new_block->is_free = 1;
            }
            hdr->is_free = 0;
            return heap + offset + HEADER_SIZE;
        }
        offset = next_block_offset(offset);
    }
    return NULL;
}

void my_free(void *ptr)
{
    if (ptr == NULL) return;

    /* Header is right before the user data */
    BlockHeader *hdr = (BlockHeader *)((char *)ptr - HEADER_SIZE);
    hdr->is_free = 1;

    coalesce();
}

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
    printf("=== Mini Malloc (Solution) ===\n\n");

    printf("basic allocation:\n");
    {
        heap_initialized = 0;
        void *p1 = my_malloc(32);
        TEST("first alloc returns non-NULL") EXPECT(p1 != NULL);

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
        void *p = my_malloc(HEAP_SIZE);
        TEST("alloc entire heap returns NULL") EXPECT(p == NULL);

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
        void *p4 = my_malloc(300);
        TEST("can allocate 300 bytes after coalescing") EXPECT(p4 != NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
