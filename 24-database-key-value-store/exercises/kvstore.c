/*
 * Kata: Simple Key-Value Store (Sorted Array)
 *
 * Build an in-memory key-value store backed by a sorted array.
 * String keys, string values. Sorted by key for binary search lookups.
 *
 * Functions to implement:
 *   - kv_create(capacity)               — allocate a store
 *   - kv_destroy(store)                 — free everything
 *   - kv_set(store, key, value)         — insert or update
 *   - kv_get(store, key, buf, bufsize)  — lookup, return 0/-1
 *   - kv_delete(store, key)             — remove a key
 *   - kv_count(store)                   — number of entries
 *
 * 12 tests total.
 *
 * Hints:
 *   - Use strdup() to copy keys and values (remember to free them).
 *   - Keep the entries sorted by key so you can use binary search.
 *   - On insert, shift entries right to make room. On delete, shift left.
 *   - For kv_set with an existing key, update the value (don't add duplicate).
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

/* ── TODO: Implement these functions ──────────────────────────────── */

/*
 * kv_create — allocate a new key-value store with the given capacity.
 * Returns NULL on allocation failure.
 */
kv_store_t *kv_create(int capacity)
{
    /* TODO:
     * - malloc a kv_store_t
     * - malloc entries array (capacity * sizeof(kv_entry_t))
     * - set count=0, capacity=capacity
     * - return the store
     */
    (void)capacity;
    return NULL;
}

/*
 * kv_destroy — free all entries (keys + values) and the store itself.
 */
void kv_destroy(kv_store_t *store)
{
    /* TODO:
     * - Loop through entries, free each key and value
     * - Free the entries array
     * - Free the store
     */
    (void)store;
}

/*
 * kv_find — binary search for a key in the sorted entries.
 * Returns the index if found, or -1 if not found.
 * (Helper function — not part of the public API.)
 */
static int kv_find(kv_store_t *store, const char *key)
{
    /* TODO:
     * Binary search through store->entries (sorted by key).
     * Use strcmp() to compare keys.
     * Return index if found, -1 if not.
     */
    (void)store; (void)key;
    return -1;
}

/*
 * kv_set — insert a new key-value pair, or update the value if key exists.
 * Returns 0 on success, -1 if store is full and key is new.
 * Keys and values are copied (the store owns its own memory).
 */
int kv_set(kv_store_t *store, const char *key, const char *value)
{
    /* TODO:
     * 1. Search for existing key (kv_find)
     * 2. If found: free old value, strdup new value, return 0
     * 3. If not found:
     *    - If count == capacity, return -1
     *    - Find insertion point (maintain sorted order)
     *    - Shift entries right to make room
     *    - Insert new entry (strdup both key and value)
     *    - Increment count
     *    - Return 0
     */
    (void)store; (void)key; (void)value;
    return -1;
}

/*
 * kv_get — look up a key. If found, copy the value into out_buf
 *          (up to buf_size-1 chars + null terminator) and return 0.
 *          If not found, return -1.
 */
int kv_get(kv_store_t *store, const char *key, char *out_buf, int buf_size)
{
    /* TODO:
     * 1. Find the key (kv_find)
     * 2. If not found, return -1
     * 3. Copy value into out_buf using snprintf (safe, respects buf_size)
     * 4. Return 0
     */
    (void)store; (void)key; (void)out_buf; (void)buf_size;
    return -1;
}

/*
 * kv_delete — remove a key from the store.
 * Returns 0 if deleted, -1 if key not found.
 */
int kv_delete(kv_store_t *store, const char *key)
{
    /* TODO:
     * 1. Find the key (kv_find)
     * 2. If not found, return -1
     * 3. Free the key and value at that index
     * 4. Shift entries left to fill the gap
     * 5. Decrement count
     * 6. Return 0
     */
    (void)store; (void)key;
    return -1;
}

/*
 * kv_count — return the number of entries in the store.
 */
int kv_count(kv_store_t *store)
{
    /* TODO: return store->count */
    (void)store;
    return 0;
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
