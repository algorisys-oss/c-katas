/*
 * stack_alloc.c — Stack (LIFO) Allocator
 * Module 11: Build Your Own Allocator
 *
 * Like an arena, but you CAN free — as long as you free in reverse order.
 * Each allocation stores a small header with its size so pop() knows how
 * far to rewind.
 *
 *   +------+----------+------+----------+-----------------------+
 *   | hdr  |  alloc 1 | hdr  |  alloc 2 |      free space       |
 *   | (sz) |  (data)  | (sz) |  (data)  |                       |
 *   +------+----------+------+----------+-----------------------+
 *   ^                                    ^                       ^
 *   buffer                              top                   capacity
 *
 * The header stores the total size (header + data) so pop() can subtract
 * it from `top` to reclaim the space.
 *
 * Build:  make stack_alloc
 * Run:    ./stack_alloc
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
/*  Stack allocator                                                    */
/* ------------------------------------------------------------------ */

/*
 * Header stored before each allocation.
 * Stores the total size (header + padding + user data) so pop can rewind.
 */
typedef struct {
    size_t total_size;   /* header + data (everything to subtract on pop) */
} stack_header_t;

#define STACK_ALIGN 8

typedef struct {
    unsigned char *buffer;   /* backing memory                          */
    size_t capacity;         /* total size of backing memory            */
    size_t top;              /* current offset (next free byte)         */
} stack_alloc_t;

/*
 * stack_alloc_create — allocate backing memory of `capacity` bytes.
 */
static stack_alloc_t stack_alloc_create(size_t capacity) {
    stack_alloc_t s;
    memset(&s, 0, sizeof(s));

    s.buffer = malloc(capacity);
    if (s.buffer) {
        s.capacity = capacity;
        s.top = 0;
    }

    return s;
}

/*
 * stack_alloc_push — allocate `size` bytes from the stack.
 *
 * Layout of one allocation:
 *   [padding for header alignment] [header (total_size)] [user data]
 *                                                        ^-- returned ptr
 *
 * Steps:
 *   1. Align current top for the header.
 *   2. Place header at aligned position.
 *   3. User data starts right after header.
 *   4. Advance top past the user data.
 *   5. Store (top_after - top_before) in header.total_size.
 *
 * Returns NULL if not enough space.
 */
static void *stack_alloc_push(stack_alloc_t *s, size_t size) {
    size_t old_top = s->top;

    /* Align top to STACK_ALIGN */
    size_t aligned = (s->top + (STACK_ALIGN - 1)) & ~(STACK_ALIGN - 1);

    /* Header goes at the aligned position */
    size_t header_pos = aligned;

    /* Data starts right after the header */
    size_t data_pos = header_pos + sizeof(stack_header_t);

    /* New top is after the user data */
    size_t new_top = data_pos + size;

    if (new_top > s->capacity) {
        return NULL;
    }

    /* Write the header: total_size = everything from old_top to new_top */
    ((stack_header_t *)(s->buffer + header_pos))->total_size = new_top - old_top;

    s->top = new_top;
    return s->buffer + data_pos;
}

/*
 * stack_alloc_pop — free the most recent allocation.
 *
 * Reads the header to find out how much to rewind.
 * `ptr` MUST be the most recently pushed pointer (LIFO).
 */
static void stack_alloc_pop(stack_alloc_t *s, void *ptr) {
    stack_header_t *header = (stack_header_t *)((unsigned char *)ptr - sizeof(stack_header_t));
    s->top -= header->total_size;
}

/*
 * stack_alloc_reset — reset the stack (free everything).
 */
static void stack_alloc_reset(stack_alloc_t *s) {
    s->top = 0;
}

/*
 * stack_alloc_destroy — free backing memory and zero the struct.
 */
static void stack_alloc_destroy(stack_alloc_t *s) {
    free(s->buffer);
    memset(s, 0, sizeof(*s));
}

/* ------------------------------------------------------------------ */
/*  Tests                                                              */
/* ------------------------------------------------------------------ */

static void test_create_destroy(void) {
    stack_alloc_t s = stack_alloc_create(1024);
    TEST("create allocates buffer")
        EXPECT(s.buffer != NULL && s.capacity == 1024 && s.top == 0);
    stack_alloc_destroy(&s);
    TEST("destroy zeros struct")
        EXPECT(s.buffer == NULL && s.capacity == 0 && s.top == 0);
}

static void test_single_push(void) {
    stack_alloc_t s = stack_alloc_create(256);
    void *p = stack_alloc_push(&s, 32);
    TEST("single push returns non-NULL")
        EXPECT(p != NULL);
    TEST("top advances after push")
        EXPECT(s.top > 0);
    stack_alloc_destroy(&s);
}

static void test_push_and_pop(void) {
    stack_alloc_t s = stack_alloc_create(512);
    void *p = stack_alloc_push(&s, 64);
    size_t top_after_push = s.top;
    if (p) stack_alloc_pop(&s, p);
    TEST("pop rewinds top")
        EXPECT(p != NULL && s.top < top_after_push);
    TEST("pop rewinds to 0 for first alloc")
        EXPECT(p != NULL && s.top == 0);
    stack_alloc_destroy(&s);
}

static void test_multiple_push_pop(void) {
    stack_alloc_t s = stack_alloc_create(512);
    void *a = stack_alloc_push(&s, 16);
    size_t top_a = s.top;
    void *b = stack_alloc_push(&s, 32);
    void *c = stack_alloc_push(&s, 48);
    (void)a;

    /* Pop in reverse order */
    if (c) stack_alloc_pop(&s, c);
    if (b) stack_alloc_pop(&s, b);
    TEST("after popping b and c, top returns to after a")
        EXPECT(a != NULL && b != NULL && c != NULL && s.top == top_a);
    stack_alloc_destroy(&s);
}

static void test_write_read(void) {
    stack_alloc_t s = stack_alloc_create(256);
    char *msg = stack_alloc_push(&s, 32);
    int ok = 0;
    if (msg) {
        memcpy(msg, "stack alloc!", 13);
        ok = (strcmp(msg, "stack alloc!") == 0);
        stack_alloc_pop(&s, msg);
    }
    TEST("can write and read from pushed memory")
        EXPECT(ok);
    stack_alloc_destroy(&s);
}

static void test_overflow(void) {
    stack_alloc_t s = stack_alloc_create(64);
    void *p1 = stack_alloc_push(&s, 32);
    void *p2 = stack_alloc_push(&s, 64); /* won't fit */
    TEST("push beyond capacity returns NULL")
        EXPECT(p1 != NULL && p2 == NULL);
    stack_alloc_destroy(&s);
}

static void test_reset(void) {
    stack_alloc_t s = stack_alloc_create(256);
    stack_alloc_push(&s, 32);
    stack_alloc_push(&s, 64);
    stack_alloc_reset(&s);
    TEST("reset sets top to 0")
        EXPECT(s.top == 0);
    stack_alloc_destroy(&s);
}

static void test_push_after_reset(void) {
    stack_alloc_t s = stack_alloc_create(256);
    void *p1 = stack_alloc_push(&s, 32);
    stack_alloc_reset(&s);
    void *p2 = stack_alloc_push(&s, 32);
    TEST("push after reset reuses memory")
        EXPECT(p1 == p2);
    stack_alloc_destroy(&s);
}

static void test_push_pop_cycle(void) {
    stack_alloc_t s = stack_alloc_create(256);
    int ok = (s.buffer != NULL) ? 1 : 0;
    for (int i = 0; i < 50 && ok; i++) {
        void *p = stack_alloc_push(&s, 16);
        if (!p) { ok = 0; break; }
        memset(p, 0xAB, 16);
        stack_alloc_pop(&s, p);
    }
    TEST("push-pop cycle 50 times, top stays at 0")
        EXPECT(ok && s.top == 0);
    stack_alloc_destroy(&s);
}

static void test_different_sizes(void) {
    stack_alloc_t s = stack_alloc_create(1024);
    size_t sizes[] = {1, 7, 13, 64, 3, 100, 8, 255};
    void *ptrs[8];
    int count = 0;
    int ok = 1;
    for (int i = 0; i < 8; i++) {
        ptrs[i] = stack_alloc_push(&s, sizes[i]);
        if (!ptrs[i]) { ok = 0; break; }
        count++;
    }
    /* Pop all in reverse */
    for (int i = count - 1; i >= 0; i--) {
        stack_alloc_pop(&s, ptrs[i]);
    }
    TEST("various sizes push/pop, top returns to 0")
        EXPECT(ok && s.top == 0);
    stack_alloc_destroy(&s);
}

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */

int main(void) {
    printf("=== Stack Allocator Tests ===\n\n");

    test_create_destroy();
    test_single_push();
    test_push_and_pop();
    test_multiple_push_pop();
    test_write_read();
    test_overflow();
    test_reset();
    test_push_after_reset();
    test_push_pop_cycle();
    test_different_sizes();

    printf("\n  Results: %d/%d passed\n", tests_passed, tests_total);
    return (tests_passed == tests_total) ? 0 : 1;
}
