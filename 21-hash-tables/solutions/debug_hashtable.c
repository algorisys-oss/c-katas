/*
 * Debug Kata: Hash Table — SOLUTIONS
 *
 * All 5 bugs fixed. Each fix is marked with "FIX N:" comments.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic debug_hashtable.c -o debug_hashtable
 * Run:      ./debug_hashtable
 * Verify:   valgrind ./debug_hashtable
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Test Macros ─────────────────────────────────────────────────── */

#define TEST(name) printf("  %-40s", name)
#define PASS() printf("[PASS]\n")
static int tests_passed = 0, tests_failed = 0;
#define EXPECT(cond) do { \
    if (cond) { PASS(); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); tests_failed++; } \
} while(0)

/* ── Data Structures ─────────────────────────────────────────────── */

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_THRESHOLD 0.75
#define TOMBSTONE ((char *)-1)

typedef struct {
    char *key;
    char *value;
} ht_entry_t;

typedef struct {
    ht_entry_t *entries;
    int capacity;
    int size;
} hash_table_t;

/* ── Hash Function ───────────────────────────────────────────────── */

/*
 * FIX 1: Process ALL characters, not just the first one.
 *
 * Bug was:   h = h * 33 + (unsigned char)key[0];
 * Fix:       Loop over every character with a for loop.
 */
static unsigned int hash(const char *key, int capacity)
{
    unsigned int h = 5381;
    for (int i = 0; key[i] != '\0'; i++) {
        h = h * 33 + (unsigned char)key[i];
    }
    return h % capacity;
}

/* ── Create / Destroy ────────────────────────────────────────────── */

static hash_table_t *ht_create(int capacity)
{
    hash_table_t *ht = malloc(sizeof(hash_table_t));
    if (!ht) return NULL;
    ht->entries = calloc(capacity, sizeof(ht_entry_t));
    if (!ht->entries) {
        free(ht);
        return NULL;
    }
    ht->capacity = capacity;
    ht->size = 0;
    return ht;
}

static void ht_destroy(hash_table_t *ht)
{
    if (!ht) return;
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->entries[i].key != NULL && ht->entries[i].key != TOMBSTONE) {
            free(ht->entries[i].key);
            free(ht->entries[i].value);
        }
    }
    free(ht->entries);
    free(ht);
}

/* ── Resize ──────────────────────────────────────────────────────── */

/*
 * FIX 2: Use new_capacity (not old_capacity) when rehashing.
 *
 * Bug was:   unsigned int idx = hash(old_entries[i].key, old_capacity);
 * Fix:       unsigned int idx = hash(old_entries[i].key, new_capacity);
 *
 * The hash function's second argument is the modulus. After resize,
 * we need indices in range [0, new_capacity), not [0, old_capacity).
 */
static int ht_resize(hash_table_t *ht, int new_capacity)
{
    ht_entry_t *old_entries = ht->entries;
    int old_capacity = ht->capacity;

    ht->entries = calloc(new_capacity, sizeof(ht_entry_t));
    if (!ht->entries) {
        ht->entries = old_entries;
        return -1;
    }
    ht->capacity = new_capacity;
    ht->size = 0;

    for (int i = 0; i < old_capacity; i++) {
        if (old_entries[i].key != NULL && old_entries[i].key != TOMBSTONE) {
            unsigned int idx = hash(old_entries[i].key, new_capacity);
            while (ht->entries[idx].key != NULL) {
                idx = (idx + 1) % new_capacity;
            }
            ht->entries[idx].key = old_entries[i].key;
            ht->entries[idx].value = old_entries[i].value;
            ht->size++;
        }
    }
    free(old_entries);
    return 0;
}

/* ── Set ─────────────────────────────────────────────────────────── */

/*
 * FIX 3: Free the old value before overwriting.
 *
 * Bug was:   ht->entries[idx].value = strdup(value);  // leaked old value
 * Fix:       free(ht->entries[idx].value);
 *            ht->entries[idx].value = strdup(value);
 *
 * strdup() allocates new memory. If we don't free the old string
 * first, that memory is lost forever — a classic memory leak.
 */
static void ht_set(hash_table_t *ht, const char *key, const char *value)
{
    if ((double)ht->size / ht->capacity >= LOAD_FACTOR_THRESHOLD) {
        ht_resize(ht, ht->capacity * 2);
    }

    unsigned int idx = hash(key, ht->capacity);
    while (ht->entries[idx].key != NULL) {
        if (ht->entries[idx].key != TOMBSTONE &&
            strcmp(ht->entries[idx].key, key) == 0) {
            free(ht->entries[idx].value);
            ht->entries[idx].value = strdup(value);
            return;
        }
        idx = (idx + 1) % ht->capacity;
    }

    ht->entries[idx].key = strdup(key);
    ht->entries[idx].value = strdup(value);
    ht->size++;
}

/* ── Get ─────────────────────────────────────────────────────────── */

/*
 * FIX 4: Use strcmp() instead of == for string comparison.
 *
 * Bug was:   ht->entries[idx].key == key
 * Fix:       strcmp(ht->entries[idx].key, key) == 0
 *
 * In C, == on char* compares pointer addresses, not string contents.
 * Two different char arrays with identical text have different addresses.
 * strcmp() compares the actual characters and returns 0 when equal.
 */
static const char *ht_get(hash_table_t *ht, const char *key)
{
    unsigned int idx = hash(key, ht->capacity);
    while (ht->entries[idx].key != NULL) {
        if (ht->entries[idx].key != TOMBSTONE &&
            strcmp(ht->entries[idx].key, key) == 0) {
            return ht->entries[idx].value;
        }
        idx = (idx + 1) % ht->capacity;
    }
    return NULL;
}

/* ── Delete ──────────────────────────────────────────────────────── */

/* Not used by tests, but included as part of the complete implementation */
int ht_delete(hash_table_t *ht, const char *key)
{
    unsigned int idx = hash(key, ht->capacity);
    while (ht->entries[idx].key != NULL) {
        if (ht->entries[idx].key != TOMBSTONE &&
            strcmp(ht->entries[idx].key, key) == 0) {
            free(ht->entries[idx].key);
            free(ht->entries[idx].value);
            ht->entries[idx].key = TOMBSTONE;
            ht->entries[idx].value = NULL;
            ht->size--;
            return 1;
        }
        idx = (idx + 1) % ht->capacity;
    }
    return 0;
}

/* ── Probe ───────────────────────────────────────────────────────── */

/*
 * FIX 5: Track how many slots we've checked and stop at capacity.
 *
 * Bug was:   No limit on the loop — spins forever if table is full.
 * Fix:       Count iterations. If we've checked all `capacity` slots
 *            without finding the key or an empty slot, return -1.
 */
static int ht_probe(hash_table_t *ht, const char *key)
{
    unsigned int idx = hash(key, ht->capacity);
    int checked = 0;

    while (ht->entries[idx].key != NULL) {
        if (checked >= ht->capacity) {
            return -1;  /* table is full, key not found */
        }
        if (ht->entries[idx].key != TOMBSTONE &&
            strcmp(ht->entries[idx].key, key) == 0) {
            return (int)idx;
        }
        idx = (idx + 1) % ht->capacity;
        checked++;
    }
    return (int)idx;
}

/* ── Tests ───────────────────────────────────────────────────────── */

static void test_hash_distribution(void)
{
    TEST("hash distributes different keys");
    unsigned int h1 = hash("apple", 16);
    unsigned int h2 = hash("avocado", 16);
    unsigned int h3 = hash("ant", 16);
    unsigned int h4 = hash("banana", 16);
    int unique = 1;
    if (h2 != h1) unique++;
    if (h3 != h1 && h3 != h2) unique++;
    if (h4 != h1 && h4 != h2 && h4 != h3) unique++;
    EXPECT(unique >= 3);
}

static void test_resize_preserves(void)
{
    TEST("resize preserves all entries");
    hash_table_t *ht = ht_create(4);
    ht_set(ht, "one", "1");
    ht_set(ht, "two", "2");
    ht_set(ht, "three", "3");
    ht_set(ht, "four", "4");

    int ok = 1;
    const char *v;
    v = ht_get(ht, "one");   if (!v || strcmp(v, "1") != 0) ok = 0;
    v = ht_get(ht, "two");   if (!v || strcmp(v, "2") != 0) ok = 0;
    v = ht_get(ht, "three"); if (!v || strcmp(v, "3") != 0) ok = 0;
    v = ht_get(ht, "four");  if (!v || strcmp(v, "4") != 0) ok = 0;

    ht_destroy(ht);
    EXPECT(ok);
}

static void test_overwrite(void)
{
    TEST("overwrite updates value (check valgrind)");
    hash_table_t *ht = ht_create(8);
    ht_set(ht, "color", "red");
    ht_set(ht, "color", "blue");

    const char *v = ht_get(ht, "color");
    EXPECT(v != NULL && strcmp(v, "blue") == 0);
    ht_destroy(ht);
}

static void test_get_by_content(void)
{
    TEST("get finds keys by string content");
    hash_table_t *ht = ht_create(8);
    char key1[16];
    snprintf(key1, sizeof(key1), "fruit");
    ht_set(ht, key1, "apple");

    char key2[16];
    snprintf(key2, sizeof(key2), "fruit");

    const char *v = ht_get(ht, key2);
    EXPECT(v != NULL && strcmp(v, "apple") == 0);
    ht_destroy(ht);
}

static void test_probe_full_table(void)
{
    TEST("probe returns -1 when table is full");
    hash_table_t *ht = ht_create(4);

    for (int i = 0; i < 4; i++) {
        char buf[8];
        snprintf(buf, sizeof(buf), "k%d", i);
        ht->entries[i].key = strdup(buf);
        ht->entries[i].value = strdup("x");
        ht->size++;
    }

    int result = ht_probe(ht, "missing");
    EXPECT(result == -1);
    ht_destroy(ht);
}

/* ── Main ────────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Hash Table (SOLUTIONS) ===\n\n");

    test_hash_distribution();
    test_resize_preserves();
    test_overwrite();
    test_get_by_content();
    test_probe_full_table();

    printf("\n  Results: %d passed, %d failed\n", tests_passed, tests_failed);
    if (tests_failed == 0) {
        printf("  All bugs fixed! Run with valgrind to confirm no leaks.\n");
    }
    printf("\n");

    return tests_failed > 0 ? 1 : 0;
}
