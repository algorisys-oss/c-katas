/*
 * Kata: String Builder
 *
 * Build a dynamic string that grows automatically as you append to it.
 * This teaches the core pattern behind every resizable data structure:
 *   - Allocate an initial buffer
 *   - Track length and capacity
 *   - Double capacity when full (amortized doubling)
 *   - Always null-terminate
 *
 * 15 tests total.
 *
 * Hints:
 *   - Use malloc for initial allocation, realloc to grow
 *   - Always check for NULL returns from malloc/realloc
 *   - Remember: strlen doesn't count the '\0', but your buffer needs room for it
 *   - When doubling, ensure new capacity is at least enough for the new data
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INITIAL_CAPACITY 8

typedef struct {
    char *data;      /* heap-allocated buffer */
    int length;      /* current string length (not counting '\0') */
    int capacity;    /* total buffer size */
} StringBuilder;

/* ── Exercise 1: sb_create ────────────────────────────────────────────
 * Create a new StringBuilder with an initial capacity of INITIAL_CAPACITY.
 *
 * - Allocate a buffer of INITIAL_CAPACITY bytes
 * - Set length to 0
 * - Null-terminate the buffer (data[0] = '\0')
 * - Return the initialized struct
 *
 * Returns: an initialized StringBuilder (by value)
 */
StringBuilder sb_create(void)
{
    StringBuilder sb;
    /* TODO: malloc a buffer, set length=0, capacity=INITIAL_CAPACITY,
     *       null-terminate, and return */
    sb.data = NULL;
    sb.length = 0;
    sb.capacity = 0;
    return sb;
}

/* ── Exercise 2: sb_ensure_capacity ───────────────────────────────────
 * Ensure the builder has room for at least `min_capacity` bytes total.
 *
 * If current capacity is already >= min_capacity, do nothing.
 * Otherwise, double the capacity until it's >= min_capacity, then realloc.
 *
 * Parameters:
 *   sb           — pointer to the StringBuilder
 *   min_capacity — minimum number of bytes the buffer must hold
 *
 * Returns: 0 on success, -1 if realloc fails
 */
int sb_ensure_capacity(StringBuilder *sb, int min_capacity)
{
    /* TODO: If capacity is already enough, return 0.
     *       Otherwise, double capacity until >= min_capacity,
     *       realloc the buffer, and return 0 (or -1 on failure). */
    return -1; /* placeholder */
}

/* ── Exercise 3: sb_append ────────────────────────────────────────────
 * Append a string to the builder.
 *
 * - Calculate the new length needed (current length + strlen of new string)
 * - Ensure capacity for new length + 1 (for '\0')
 * - Copy the new string into the buffer starting at data[length]
 * - Update length
 * - Null-terminate
 *
 * Returns: 0 on success, -1 on failure
 */
int sb_append(StringBuilder *sb, const char *str)
{
    /* TODO: Ensure capacity, copy str into buffer at offset, update length */
    return -1; /* placeholder */
}

/* ── Exercise 4: sb_append_char ───────────────────────────────────────
 * Append a single character to the builder.
 *
 * Returns: 0 on success, -1 on failure
 */
int sb_append_char(StringBuilder *sb, char c)
{
    /* TODO: Ensure capacity for one more char + '\0', append c, update length */
    return -1; /* placeholder */
}

/* ── Exercise 5: sb_clear ─────────────────────────────────────────────
 * Reset the builder to an empty string without freeing the buffer.
 * (Keep the allocated capacity for reuse.)
 */
void sb_clear(StringBuilder *sb)
{
    /* TODO: Reset length to 0, null-terminate at data[0] */
}

/* ── Exercise 6: sb_destroy ───────────────────────────────────────────
 * Free the builder's heap memory and reset all fields.
 */
void sb_destroy(StringBuilder *sb)
{
    /* TODO: free the buffer, set data=NULL, length=0, capacity=0 */
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== String Builder ===\n\n");

    printf("sb_create:\n");
    {
        StringBuilder sb = sb_create();
        TEST("initial length is 0") EXPECT(sb.length == 0);
        TEST("initial capacity is 8") EXPECT(sb.capacity == INITIAL_CAPACITY);
        TEST("data is not NULL") EXPECT(sb.data != NULL);
        TEST("data is empty string") EXPECT(sb.data[0] == '\0');
        sb_destroy(&sb);
    }

    printf("\nsb_append:\n");
    {
        StringBuilder sb = sb_create();
        sb_append(&sb, "hello");
        TEST("append 'hello' → length 5") EXPECT(sb.length == 5);
        TEST("data is 'hello'") EXPECT(strcmp(sb.data, "hello") == 0);

        sb_append(&sb, " world");
        TEST("append ' world' → length 11") EXPECT(sb.length == 11);
        TEST("data is 'hello world'") EXPECT(strcmp(sb.data, "hello world") == 0);
        TEST("capacity grew (>=12)") EXPECT(sb.capacity >= 12);
        sb_destroy(&sb);
    }

    printf("\nsb_append_char:\n");
    {
        StringBuilder sb = sb_create();
        sb_append(&sb, "abc");
        sb_append_char(&sb, 'd');
        TEST("append_char 'd' → 'abcd'") EXPECT(strcmp(sb.data, "abcd") == 0);
        TEST("length is 4") EXPECT(sb.length == 4);
        sb_destroy(&sb);
    }

    printf("\nsb_clear:\n");
    {
        StringBuilder sb = sb_create();
        sb_append(&sb, "hello world");
        int old_cap = sb.capacity;
        sb_clear(&sb);
        TEST("clear → length 0") EXPECT(sb.length == 0);
        TEST("clear → data is empty string") EXPECT(sb.data[0] == '\0');
        TEST("clear → capacity unchanged") EXPECT(sb.capacity == old_cap);
        sb_destroy(&sb);
    }

    printf("\nsb_destroy:\n");
    {
        StringBuilder sb = sb_create();
        sb_append(&sb, "test");
        sb_destroy(&sb);
        TEST("destroy → data is NULL") EXPECT(sb.data == NULL);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
