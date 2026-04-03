/*
 * arena.c — Arena (Bump) Allocator
 * Module 11: Build Your Own Allocator
 *
 * An arena allocates from a big pre-allocated block by bumping a pointer
 * forward. You can't free individual allocations — only reset the whole
 * arena at once. This makes it blazing fast and fragmentation-free.
 *
 *   +----------+----------+---------+--------------------+
 *   | alloc 1  | alloc 2  | alloc 3 |     free space     |
 *   +----------+----------+---------+--------------------+
 *   ^                                ^                    ^
 *   buffer                           offset             capacity
 *
 * Build:  make arena
 * Run:    ./arena
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
/*  Arena allocator                                                    */
/* ------------------------------------------------------------------ */

#define ARENA_DEFAULT_ALIGN 8

typedef struct {
    unsigned char *buffer;   /* backing memory from malloc           */
    size_t capacity;         /* total size of backing memory         */
    size_t offset;           /* current bump position (next alloc)   */
} arena_t;

/*
 * arena_create — allocate backing memory of `capacity` bytes.
 *
 * Returns an arena_t. On failure, buffer is NULL.
 */
static arena_t arena_create(size_t capacity) {
    arena_t a;
    /* TODO: Initialize all fields.
     *       malloc `capacity` bytes for buffer.
     *       Set offset to 0.
     *       Handle malloc failure (set buffer to NULL). */

    (void)capacity; /* remove this line when you implement */
    memset(&a, 0, sizeof(a));

    return a;
}

/*
 * arena_alloc — bump-allocate `size` bytes, aligned to ARENA_DEFAULT_ALIGN.
 *
 * Returns a pointer to the allocated region, or NULL if out of space.
 *
 * Alignment: round the current offset UP to the nearest multiple of
 * ARENA_DEFAULT_ALIGN before allocating.
 *
 *   aligned = (offset + (align-1)) & ~(align-1)
 */
static void *arena_alloc(arena_t *a, size_t size) {
    /* TODO:
     * 1. Compute aligned_offset from a->offset.
     * 2. Check if aligned_offset + size > a->capacity.  If so, return NULL.
     * 3. Set a->offset = aligned_offset + size.
     * 4. Return a->buffer + aligned_offset. */

    (void)a;    /* remove these when you implement */
    (void)size;
    return NULL;
}

/*
 * arena_reset — reset the arena so all memory can be reused.
 *
 * Does NOT free the backing buffer — just moves the pointer back.
 */
static void arena_reset(arena_t *a) {
    /* TODO: Set offset back to 0. */
    (void)a;
}

/*
 * arena_destroy — free the backing buffer and zero out the struct.
 */
static void arena_destroy(arena_t *a) {
    /* TODO: free(a->buffer), then zero the struct. */
    (void)a;
}

/* ------------------------------------------------------------------ */
/*  Tests                                                              */
/* ------------------------------------------------------------------ */

static void test_create_destroy(void) {
    arena_t a = arena_create(1024);
    TEST("create allocates buffer")
        EXPECT(a.buffer != NULL && a.capacity == 1024 && a.offset == 0);
    arena_destroy(&a);
    TEST("destroy zeros struct")
        EXPECT(a.buffer == NULL && a.capacity == 0 && a.offset == 0);
}

static void test_single_alloc(void) {
    arena_t a = arena_create(256);
    void *p = arena_alloc(&a, 32);
    TEST("single alloc returns non-NULL")
        EXPECT(p != NULL);
    TEST("offset advances after alloc")
        EXPECT(a.offset >= 32);
    arena_destroy(&a);
}

static void test_multiple_allocs(void) {
    arena_t a = arena_create(256);
    void *p1 = arena_alloc(&a, 16);
    void *p2 = arena_alloc(&a, 32);
    void *p3 = arena_alloc(&a, 8);
    TEST("multiple allocs return different pointers")
        EXPECT(p1 != p2 && p2 != p3 && p1 != p3);
    TEST("allocs are in order (p1 < p2 < p3)")
        EXPECT((uintptr_t)p1 < (uintptr_t)p2 &&
               (uintptr_t)p2 < (uintptr_t)p3);
    arena_destroy(&a);
}

static void test_reset(void) {
    arena_t a = arena_create(256);
    arena_alloc(&a, 64);
    arena_alloc(&a, 64);
    arena_reset(&a);
    TEST("reset sets offset to 0")
        EXPECT(a.offset == 0);
    arena_destroy(&a);
}

static void test_alloc_after_reset(void) {
    arena_t a = arena_create(256);
    void *p1 = arena_alloc(&a, 32);
    arena_reset(&a);
    void *p2 = arena_alloc(&a, 32);
    TEST("alloc after reset reuses memory")
        EXPECT(p1 == p2);
    arena_destroy(&a);
}

static void test_out_of_space(void) {
    arena_t a = arena_create(64);
    void *p1 = arena_alloc(&a, 48);
    void *p2 = arena_alloc(&a, 48);
    TEST("out of space returns NULL")
        EXPECT(p1 != NULL && p2 == NULL);
    arena_destroy(&a);
}

static void test_exact_fit(void) {
    arena_t a = arena_create(64);
    void *p = arena_alloc(&a, 64);
    TEST("exact capacity alloc succeeds")
        EXPECT(p != NULL);
    void *p2 = arena_alloc(&a, 1);
    TEST("no room after exact fit")
        EXPECT(p2 == NULL);
    arena_destroy(&a);
}

static void test_alignment(void) {
    arena_t a = arena_create(256);
    arena_alloc(&a, 3);  /* misalign on purpose */
    void *p = arena_alloc(&a, 16);
    TEST("second alloc is 8-byte aligned")
        EXPECT(((uintptr_t)p % ARENA_DEFAULT_ALIGN) == 0);
    arena_destroy(&a);
}

static void test_zero_size(void) {
    arena_t a = arena_create(256);
    void *p = arena_alloc(&a, 0);
    TEST("zero-size alloc returns non-NULL (valid pointer)")
        EXPECT(p != NULL);
    arena_destroy(&a);
}

static void test_write_to_allocated(void) {
    arena_t a = arena_create(256);
    char *s = arena_alloc(&a, 16);
    int ok = 0;
    if (s) {
        memcpy(s, "hello, arena!", 14);
        ok = (strcmp(s, "hello, arena!") == 0);
    }
    TEST("can write and read allocated memory")
        EXPECT(ok);
    arena_destroy(&a);
}

static void test_stress(void) {
    arena_t a = arena_create(4096);
    int ok = (a.buffer != NULL) ? 1 : 0;
    for (int i = 0; i < 100 && ok; i++) {
        void *p = arena_alloc(&a, 32);
        if (!p) { ok = 0; break; }
        memset(p, 0xAB, 32);
    }
    TEST("stress: 100 x 32-byte allocs from 4096")
        EXPECT(ok && a.offset <= 4096);
    arena_destroy(&a);
}

static void test_reset_and_reuse_full(void) {
    arena_t a = arena_create(128);
    /* Fill it up */
    for (int i = 0; i < 4; i++) {
        arena_alloc(&a, 32);
    }
    void *fail = arena_alloc(&a, 32);
    arena_reset(&a);
    void *ok = arena_alloc(&a, 32);
    TEST("reset allows full reuse after exhaustion")
        EXPECT(fail == NULL && ok != NULL);
    arena_destroy(&a);
}

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */

int main(void) {
    printf("=== Arena Allocator Tests ===\n\n");

    test_create_destroy();
    test_single_alloc();
    test_multiple_allocs();
    test_reset();
    test_alloc_after_reset();
    test_out_of_space();
    test_exact_fit();
    test_alignment();
    test_zero_size();
    test_write_to_allocated();
    test_stress();
    test_reset_and_reuse_full();

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
    return (tests_passed == tests_total) ? 0 : 1;
}
