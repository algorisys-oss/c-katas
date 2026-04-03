/*
 * Solution: Simple Key-Value Store (Sorted Array)
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ── Test harness ─────────────────────────────────────────────────── */

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  test %d: %-50s ", tests_run, name); \
} while (0)

#define EXPECT(expr) do { \
    if (expr) { \
        tests_passed++; \
        printf("[PASS]\n"); \
    } else { \
        printf("[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #expr); \
    } \
} while (0)

/* ── Data structures ──────────────────────────────────────────────── */

typedef struct {
    char *key;
    char *value;
} kv_entry_t;

typedef struct {
    kv_entry_t *entries;
    int count;
    int capacity;
} kv_store_t;

/* ── Implementation ───────────────────────────────────────────────── */

kv_store_t *kv_create(int capacity)
{
    kv_store_t *store = malloc(sizeof(kv_store_t));
    if (!store) {
        return NULL;
    }
    store->entries = malloc((size_t)capacity * sizeof(kv_entry_t));
    if (!store->entries) {
        free(store);
        return NULL;
    }
    store->count = 0;
    store->capacity = capacity;
    return store;
}

void kv_destroy(kv_store_t *store)
{
    if (!store) {
        return;
    }
    for (int i = 0; i < store->count; i++) {
        free(store->entries[i].key);
        free(store->entries[i].value);
    }
    free(store->entries);
    free(store);
}

static int kv_find(kv_store_t *store, const char *key)
{
    int lo = 0;
    int hi = store->count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = strcmp(key, store->entries[mid].key);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return -1;
}

/*
 * Find the insertion point for a key (index where it should go).
 * Returns the index such that all keys before it are < key.
 */
static int kv_insertion_point(kv_store_t *store, const char *key)
{
    int lo = 0;
    int hi = store->count;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (strcmp(store->entries[mid].key, key) < 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int kv_set(kv_store_t *store, const char *key, const char *value)
{
    int idx = kv_find(store, key);
    if (idx >= 0) {
        /* Key exists — update value */
        free(store->entries[idx].value);
        store->entries[idx].value = strdup(value);
        return 0;
    }

    /* Key doesn't exist — insert */
    if (store->count == store->capacity) {
        return -1;
    }

    int pos = kv_insertion_point(store, key);

    /* Shift entries right */
    for (int i = store->count; i > pos; i--) {
        store->entries[i] = store->entries[i - 1];
    }

    store->entries[pos].key = strdup(key);
    store->entries[pos].value = strdup(value);
    store->count++;
    return 0;
}

int kv_get(kv_store_t *store, const char *key, char *out_buf, int buf_size)
{
    int idx = kv_find(store, key);
    if (idx < 0) {
        return -1;
    }
    snprintf(out_buf, (size_t)buf_size, "%s", store->entries[idx].value);
    return 0;
}

int kv_delete(kv_store_t *store, const char *key)
{
    int idx = kv_find(store, key);
    if (idx < 0) {
        return -1;
    }

    free(store->entries[idx].key);
    free(store->entries[idx].value);

    /* Shift entries left */
    for (int i = idx; i < store->count - 1; i++) {
        store->entries[i] = store->entries[i + 1];
    }
    store->count--;
    return 0;
}

int kv_count(kv_store_t *store)
{
    return store->count;
}

/* ── Tests ────────────────────────────────────────────────────────── */

int main(void)
{
    printf("=== Key-Value Store Tests ===\n\n");

    /* Test 1: create an empty store */
    {
        kv_store_t *store = kv_create(10);
        TEST("create empty store");
        EXPECT(store != NULL && kv_count(store) == 0);
        kv_destroy(store);
    }

    /* Test 2: set and get a single key */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "name", "Alice");
        char buf[64] = {0};
        int found = kv_get(store, "name", buf, sizeof(buf));
        TEST("set and get single key");
        EXPECT(found == 0 && strcmp(buf, "Alice") == 0);
        kv_destroy(store);
    }

    /* Test 3: count after inserts */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "a", "1");
        kv_set(store, "b", "2");
        kv_set(store, "c", "3");
        TEST("count is 3 after three inserts");
        EXPECT(kv_count(store) == 3);
        kv_destroy(store);
    }

    /* Test 4: get miss */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "name", "Alice");
        char buf[64] = {0};
        int found = kv_get(store, "age", buf, sizeof(buf));
        TEST("get miss returns -1");
        EXPECT(found == -1);
        kv_destroy(store);
    }

    /* Test 5: get on empty store */
    {
        kv_store_t *store = kv_create(10);
        char buf[64] = {0};
        int found = kv_get(store, "anything", buf, sizeof(buf));
        TEST("get on empty store returns -1");
        EXPECT(found == -1);
        kv_destroy(store);
    }

    /* Test 6: update existing key */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "name", "Alice");
        kv_set(store, "name", "Bob");
        char buf[64] = {0};
        kv_get(store, "name", buf, sizeof(buf));
        TEST("update existing key");
        EXPECT(strcmp(buf, "Bob") == 0 && kv_count(store) == 1);
        kv_destroy(store);
    }

    /* Test 7: delete a key */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "name", "Alice");
        kv_set(store, "age", "17");
        int result = kv_delete(store, "name");
        TEST("delete existing key");
        EXPECT(result == 0 && kv_count(store) == 1);
        kv_destroy(store);
    }

    /* Test 8: deleted key is not found */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "name", "Alice");
        kv_delete(store, "name");
        char buf[64] = {0};
        int found = kv_get(store, "name", buf, sizeof(buf));
        TEST("deleted key is not found");
        EXPECT(found == -1);
        kv_destroy(store);
    }

    /* Test 9: delete miss */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "name", "Alice");
        int result = kv_delete(store, "ghost");
        TEST("delete non-existent key returns -1");
        EXPECT(result == -1 && kv_count(store) == 1);
        kv_destroy(store);
    }

    /* Test 10: many keys remain searchable */
    {
        kv_store_t *store = kv_create(100);
        const char *keys[] = {"delta", "alpha", "charlie", "bravo", "echo"};
        const char *vals[] = {"4", "1", "3", "2", "5"};
        for (int i = 0; i < 5; i++) {
            kv_set(store, keys[i], vals[i]);
        }
        int all_found = 1;
        for (int i = 0; i < 5; i++) {
            char buf[64] = {0};
            if (kv_get(store, keys[i], buf, sizeof(buf)) != 0 ||
                strcmp(buf, vals[i]) != 0) {
                all_found = 0;
            }
        }
        TEST("five out-of-order keys all searchable");
        EXPECT(all_found && kv_count(store) == 5);
        kv_destroy(store);
    }

    /* Test 11: store at capacity rejects new inserts */
    {
        kv_store_t *store = kv_create(2);
        kv_set(store, "a", "1");
        kv_set(store, "b", "2");
        int result = kv_set(store, "c", "3");
        TEST("full store rejects new insert");
        EXPECT(result == -1 && kv_count(store) == 2);
        kv_destroy(store);
    }

    /* Test 12: get truncates long values to buf_size */
    {
        kv_store_t *store = kv_create(10);
        kv_set(store, "msg", "Hello, World!");
        char buf[6] = {0};
        kv_get(store, "msg", buf, sizeof(buf));
        TEST("get truncates to buffer size");
        EXPECT(strcmp(buf, "Hello") == 0);
        kv_destroy(store);
    }

    /* ── Summary ─────────────────────────────────────────────────── */
    printf("\n%d/%d tests passed.\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
