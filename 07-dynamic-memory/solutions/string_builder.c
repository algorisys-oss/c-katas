/*
 * Solution: String Builder
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INITIAL_CAPACITY 8

typedef struct {
    char *data;
    int length;
    int capacity;
} StringBuilder;

StringBuilder sb_create(void)
{
    StringBuilder sb;
    sb.data = malloc(INITIAL_CAPACITY);
    if (sb.data == NULL) {
        sb.length = 0;
        sb.capacity = 0;
        return sb;
    }
    sb.data[0] = '\0';
    sb.length = 0;
    sb.capacity = INITIAL_CAPACITY;
    return sb;
}

int sb_ensure_capacity(StringBuilder *sb, int min_capacity)
{
    if (sb->capacity >= min_capacity) {
        return 0;
    }
    int new_cap = sb->capacity;
    while (new_cap < min_capacity) {
        new_cap *= 2;
    }
    char *temp = realloc(sb->data, new_cap);
    if (temp == NULL) {
        return -1;
    }
    sb->data = temp;
    sb->capacity = new_cap;
    return 0;
}

int sb_append(StringBuilder *sb, const char *str)
{
    int add_len = strlen(str);
    if (sb_ensure_capacity(sb, sb->length + add_len + 1) != 0) {
        return -1;
    }
    memcpy(sb->data + sb->length, str, add_len + 1);
    sb->length += add_len;
    return 0;
}

int sb_append_char(StringBuilder *sb, char c)
{
    if (sb_ensure_capacity(sb, sb->length + 2) != 0) {
        return -1;
    }
    sb->data[sb->length] = c;
    sb->length++;
    sb->data[sb->length] = '\0';
    return 0;
}

void sb_clear(StringBuilder *sb)
{
    sb->length = 0;
    if (sb->data != NULL) {
        sb->data[0] = '\0';
    }
}

void sb_destroy(StringBuilder *sb)
{
    free(sb->data);
    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;
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
    printf("=== String Builder (Solution) ===\n\n");

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
