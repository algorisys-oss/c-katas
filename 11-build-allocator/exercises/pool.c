/*
 * pool.c — Pool Allocator
 * Module 11: Build Your Own Allocator
 *
 * A pool allocator divides a big block into fixed-size chunks and threads
 * a free list through them.  Alloc = pop.  Free = push.  Both O(1).
 *
 * The free list is EMBEDDED in the free blocks themselves:
 *
 *   free_list
 *      |
 *      v
 *   +--------+    +--------+    +--------+    +--------+
 *   | next ------>| next ------>| next ------>| NULL    |
 *   |  ...   |    |  ...   |    |  ...   |    |  ...   |
 *   +--------+    +--------+    +--------+    +--------+
 *   block 0       block 1       block 2       block 3
 *
 * When a block is allocated, its "next" pointer is overwritten by user
 * data — we don't need it anymore.  When freed, we write a "next"
 * pointer back into it and push it onto the free list head.
 *
 * Build:  make pool
 * Run:    ./pool
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Test harness                                                       */
/* ------------------------------------------------------------------ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ------------------------------------------------------------------ */
/*  Pool allocator                                                     */
/* ------------------------------------------------------------------ */

/*
 * Each free block's first bytes hold a pointer to the next free block.
 * This struct is "overlaid" onto free blocks — it doesn't use extra memory.
 */
typedef struct free_node {
    struct free_node *next;
} free_node_t;

typedef struct {
    unsigned char *buffer;     /* backing memory (one big malloc)       */
    size_t block_size;         /* size of each block in bytes           */
    size_t block_count;        /* total number of blocks                */
    free_node_t *free_list;    /* head of the embedded free list        */
} pool_t;

/*
 * pool_create — allocate backing memory and build the free list.
 *
 * block_size must be >= sizeof(void*) so we can embed the free list.
 * If block_size is too small, bump it up to sizeof(void*).
 *
 * The free list is built by walking the buffer from block 0 to block N-1,
 * setting each block's "next" to point to the following block.
 * The last block's "next" is NULL.
 */
static pool_t pool_create(size_t block_size, size_t block_count) {
    pool_t p;
    memset(&p, 0, sizeof(p));

    /* TODO:
     * 1. Ensure block_size >= sizeof(void*). If not, set it to sizeof(void*).
     * 2. malloc(block_size * block_count) into p.buffer.
     * 3. Set p.block_size and p.block_count.
     * 4. Build the free list:
     *    - For each block i (0 to block_count-1):
     *        - Compute block address: p.buffer + i * block_size
     *        - Cast it to free_node_t*
     *        - Set its ->next to the NEXT block (or NULL for the last)
     *    - Set p.free_list to the first block.
     * 5. Handle malloc failure. */

    (void)block_size;   /* remove when you implement */
    (void)block_count;

    return p;
}

/*
 * pool_alloc — pop a block from the free list.
 *
 * Returns NULL if the pool is exhausted (free_list is NULL).
 */
static void *pool_alloc(pool_t *p) {
    /* TODO:
     * 1. If p->free_list is NULL, return NULL.
     * 2. Save p->free_list as the block to return.
     * 3. Advance p->free_list to free_list->next.
     * 4. Return the block (cast to void*). */

    (void)p;
    return NULL;
}

/*
 * pool_free — push a block back onto the free list.
 *
 * The caller promises `ptr` was previously returned by pool_alloc
 * and belongs to this pool.
 */
static void pool_free(pool_t *p, void *ptr) {
    /* TODO:
     * 1. Cast ptr to free_node_t*.
     * 2. Set node->next = p->free_list.
     * 3. Set p->free_list = node.
     * (This is a linked-list push-to-front.) */

    (void)p;
    (void)ptr;
}

/*
 * pool_destroy — free the backing buffer and zero the struct.
 */
static void pool_destroy(pool_t *p) {
    /* TODO: free(p->buffer), then zero the struct. */
    (void)p;
}

/* ------------------------------------------------------------------ */
/*  Tests                                                              */
/* ------------------------------------------------------------------ */

static void test_create_destroy(void) {
    pool_t p = pool_create(32, 8);
    TEST("create sets fields correctly")
        EXPECT(p.buffer != NULL && p.block_size == 32 &&
               p.block_count == 8 && p.free_list != NULL);
    pool_destroy(&p);
    TEST("destroy zeros struct")
        EXPECT(p.buffer == NULL && p.free_list == NULL);
}

static void test_single_alloc(void) {
    pool_t p = pool_create(64, 4);
    void *blk = pool_alloc(&p);
    TEST("single alloc returns non-NULL")
        EXPECT(blk != NULL);
    pool_destroy(&p);
}

static void test_alloc_within_buffer(void) {
    pool_t p = pool_create(64, 4);
    void *blk = pool_alloc(&p);
    uintptr_t b = (uintptr_t)blk;
    uintptr_t lo = (uintptr_t)p.buffer;
    uintptr_t hi = lo + p.block_size * p.block_count;
    TEST("alloc is within buffer range")
        EXPECT(b >= lo && b < hi);
    pool_destroy(&p);
}

static void test_alloc_all_blocks(void) {
    pool_t p = pool_create(32, 4);
    void *blocks[4];
    int ok = 1;
    for (int i = 0; i < 4; i++) {
        blocks[i] = pool_alloc(&p);
        if (!blocks[i]) ok = 0;
    }
    TEST("can alloc all blocks")
        EXPECT(ok);
    /* Verify all pointers are distinct */
    int distinct = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (blocks[i] == blocks[j]) distinct = 0;
        }
    }
    TEST("all returned pointers are distinct")
        EXPECT(distinct);
    pool_destroy(&p);
}

static void test_exhaustion(void) {
    pool_t p = pool_create(32, 2);
    pool_alloc(&p);
    pool_alloc(&p);
    void *nope = pool_alloc(&p);
    TEST("exhausted pool returns NULL")
        EXPECT(nope == NULL);
    pool_destroy(&p);
}

static void test_free_and_realloc(void) {
    pool_t p = pool_create(32, 2);
    void *a = pool_alloc(&p);
    void *b = pool_alloc(&p);
    if (a) pool_free(&p, a);
    void *c = pool_alloc(&p);
    TEST("free then alloc returns the freed block")
        EXPECT(a != NULL && c == a);
    (void)b;
    pool_destroy(&p);
}

static void test_alloc_free_cycle(void) {
    pool_t p = pool_create(64, 1);
    int ok = (p.buffer != NULL) ? 1 : 0;
    for (int i = 0; i < 100 && ok; i++) {
        void *blk = pool_alloc(&p);
        if (!blk) { ok = 0; break; }
        memset(blk, 0xCC, 64);
        pool_free(&p, blk);
    }
    TEST("alloc-free cycle 100 times on 1-block pool")
        EXPECT(ok);
    pool_destroy(&p);
}

static void test_free_different_order(void) {
    pool_t p = pool_create(32, 4);
    void *a = pool_alloc(&p);
    void *b = pool_alloc(&p);
    void *c = pool_alloc(&p);
    /* Free in different order: b, a, c */
    if (b) pool_free(&p, b);
    if (a) pool_free(&p, a);
    if (c) pool_free(&p, c);
    /* Should be able to alloc 3 again */
    void *x = pool_alloc(&p);
    void *y = pool_alloc(&p);
    void *z = pool_alloc(&p);
    TEST("free in different order, then realloc all")
        EXPECT(a != NULL && b != NULL && c != NULL &&
               x != NULL && y != NULL && z != NULL);
    pool_destroy(&p);
}

static void test_write_to_block(void) {
    pool_t p = pool_create(64, 4);
    char *blk = pool_alloc(&p);
    int ok = 0;
    if (blk) {
        memcpy(blk, "pool allocator!", 16);
        ok = (strcmp(blk, "pool allocator!") == 0);
    }
    TEST("can write and read from allocated block")
        EXPECT(ok);
    pool_destroy(&p);
}

static void test_min_block_size(void) {
    /* If user asks for block_size < sizeof(void*), we bump it up */
    pool_t p = pool_create(1, 4);
    TEST("block_size bumped to at least sizeof(void*)")
        EXPECT(p.block_size >= sizeof(void *));
    void *blk = pool_alloc(&p);
    TEST("alloc works even with tiny requested block_size")
        EXPECT(blk != NULL);
    pool_destroy(&p);
}

static void test_stress(void) {
    pool_t p = pool_create(48, 256);
    void *blocks[256];
    int alloc_count = 0;
    int ok = (p.buffer != NULL) ? 1 : 0;
    /* Alloc all */
    for (int i = 0; i < 256 && ok; i++) {
        blocks[i] = pool_alloc(&p);
        if (!blocks[i]) { ok = 0; break; }
        memset(blocks[i], (unsigned char)i, 48);
        alloc_count++;
    }
    /* Free all that were allocated */
    for (int i = 0; i < alloc_count; i++) {
        pool_free(&p, blocks[i]);
    }
    /* Alloc all again */
    for (int i = 0; i < 256 && ok; i++) {
        blocks[i] = pool_alloc(&p);
        if (!blocks[i]) { ok = 0; break; }
    }
    TEST("stress: alloc 256, free 256, alloc 256 again")
        EXPECT(ok);
    pool_destroy(&p);
}

static void test_free_list_integrity(void) {
    pool_t p = pool_create(32, 4);
    /* Alloc 2, free 2, verify free_list has 4 blocks again */
    void *a = pool_alloc(&p);
    void *b = pool_alloc(&p);
    if (b) pool_free(&p, b);
    if (a) pool_free(&p, a);
    /* Count free list nodes */
    int count = 0;
    free_node_t *node = p.free_list;
    while (node) {
        count++;
        node = node->next;
        if (count > 10) break; /* safety: detect loops */
    }
    TEST("free list has correct count after alloc+free")
        EXPECT(count == 4);
    pool_destroy(&p);
}

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */

int main(void) {
    printf("=== Pool Allocator Tests ===\n\n");

    test_create_destroy();
    test_single_alloc();
    test_alloc_within_buffer();
    test_alloc_all_blocks();
    test_exhaustion();
    test_free_and_realloc();
    test_alloc_free_cycle();
    test_free_different_order();
    test_write_to_block();
    test_min_block_size();
    test_stress();
    test_free_list_integrity();

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
    return (tests_passed == tests_total) ? 0 : 1;
}
