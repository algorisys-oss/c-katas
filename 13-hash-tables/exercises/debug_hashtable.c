/*
 * Debug Kata: Hash Table
 *
 * This file contains a hash table implementation with 5 bugs.
 * Each buggy function is marked with a BUG comment and a HINT.
 * Your job: find and fix each bug so all tests pass.
 *
 * The hash table uses open addressing with linear probing.
 * String keys, string values (both dynamically allocated).
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic debug_hashtable.c -o debug_hashtable
 * Run:      ./debug_hashtable
 *
 * 5 bugs, 5 tests. Good luck!
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
    fflush(stdout); \
} while(0)

/* ── Data Structures ─────────────────────────────────────────────── */

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_THRESHOLD 0.75
#define TOMBSTONE ((char *)-1)  /* marks deleted slots */

typedef struct {
    char *key;      /* NULL = empty, TOMBSTONE = deleted */
    char *value;
} ht_entry_t;

typedef struct {
    ht_entry_t *entries;
    int capacity;
    int size;       /* number of live entries (not counting tombstones) */
} hash_table_t;

/* ── Hash Function ───────────────────────────────────────────────── */

/*
 * BUG 1: Wrong hash computation
 *
 * This function should compute a hash from ALL characters of the key.
 * Right now it only uses the first character, which causes massive
 * collisions — "apple", "avocado", "ant" all hash to the same slot.
 *
 * HINT: The loop isn't looping. Look at how many characters are used.
 */
static unsigned int hash(const char *key, int capacity)
{
    unsigned int h = 5381;
    /* djb2 hash — should process every character */
    h = h * 33 + (unsigned char)key[0];
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
 * BUG 2: Resize loses entries
 *
 * When the table grows, every entry must be re-hashed into the NEW
 * (larger) table. But this code computes the new index incorrectly,
 * so some entries end up in the wrong slot or overwrite each other.
 *
 * HINT: Which capacity is being used to compute the new index —
 *       the old one or the new one?
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
            /* Rehash into new table */
            unsigned int idx = hash(old_entries[i].key, old_capacity);
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
 * BUG 3: Memory leak on overwrite
 *
 * When a key already exists, we want to UPDATE its value.
 * This code allocates a new value string but forgets something
 * important about the old value.
 *
 * HINT: What happens to the old value string when you overwrite it?
 *       Think about strdup() and who owns that memory.
 */
static void ht_set(hash_table_t *ht, const char *key, const char *value)
{
    /* Resize if load factor exceeded */
    if ((double)ht->size / ht->capacity >= LOAD_FACTOR_THRESHOLD) {
        ht_resize(ht, ht->capacity * 2);
    }

    unsigned int idx = hash(key, ht->capacity);
    while (ht->entries[idx].key != NULL) {
        if (ht->entries[idx].key != TOMBSTONE &&
            strcmp(ht->entries[idx].key, key) == 0) {
            /* Key exists — update value */
            ht->entries[idx].value = strdup(value);
            return;
        }
        idx = (idx + 1) % ht->capacity;
    }

    /* Empty slot found — insert new entry */
    ht->entries[idx].key = strdup(key);
    ht->entries[idx].value = strdup(value);
    ht->size++;
}

/* ── Get ─────────────────────────────────────────────────────────── */

/*
 * BUG 4: strcmp vs pointer comparison
 *
 * This function should find a key in the table and return its value.
 * It looks correct at first glance, but the key comparison is wrong.
 *
 * HINT: In C, using == on two strings compares their ADDRESSES,
 *       not their contents. "hello" == "hello" is NOT guaranteed to
 *       be true. What function compares string contents?
 */
static const char *ht_get(hash_table_t *ht, const char *key)
{
    unsigned int idx = hash(key, ht->capacity);
    while (ht->entries[idx].key != NULL) {
        if (ht->entries[idx].key != TOMBSTONE &&
            ht->entries[idx].key == key) {
            return ht->entries[idx].value;
        }
        idx = (idx + 1) % ht->capacity;
    }
    return NULL;  /* not found */
}

/* ── Delete ──────────────────────────────────────────────────────── */

static int ht_delete(hash_table_t *ht, const char *key)
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
            return 1;  /* deleted */
        }
        idx = (idx + 1) % ht->capacity;
    }
    return 0;  /* not found */
}

/* ── Probe (find slot for key) ───────────────────────────────────── */

/*
 * BUG 5: Infinite loop in probe
 *
 * This function does linear probing to find a key's slot.
 * If the key exists, return its index. If not, return the first
 * empty slot where it could be inserted.
 *
 * But if the table is full (no empty slots), this loops forever.
 *
 * HINT: You need to limit how many slots you check. If you've
 *       checked every slot and didn't find the key or an empty slot,
 *       return -1 to signal "table full."
 */
static int ht_probe(hash_table_t *ht, const char *key)
{
    unsigned int idx = hash(key, ht->capacity);

    while (ht->entries[idx].key != NULL) {
        if (ht->entries[idx].key != TOMBSTONE &&
            strcmp(ht->entries[idx].key, key) == 0) {
            return (int)idx;  /* found the key */
        }
        idx = (idx + 1) % ht->capacity;
        /* No check for wrapping all the way around! */
    }
    return (int)idx;  /* empty slot */
}

/* ── Tests ───────────────────────────────────────────────────────── */

/*
 * Test 1: Hash distributes keys across slots
 *
 * If the hash function works correctly, keys with different characters
 * should (usually) land in different buckets. "apple" and "avocado"
 * share a first letter but differ in the rest — they should NOT
 * always collide.
 */
static void test_hash_distribution(void)
{
    TEST("hash distributes different keys");
    /* With a good hash, these should NOT all map to the same slot */
    unsigned int h1 = hash("apple", 16);
    unsigned int h2 = hash("avocado", 16);
    unsigned int h3 = hash("ant", 16);
    unsigned int h4 = hash("banana", 16);
    /* At least 3 of 4 keys should land in different slots */
    int unique = 1;
    if (h2 != h1) unique++;
    if (h3 != h1 && h3 != h2) unique++;
    if (h4 != h1 && h4 != h2 && h4 != h3) unique++;
    EXPECT(unique >= 3);
}

/*
 * Test 2: Resize preserves all entries
 *
 * Insert entries, force a resize, then verify every entry is still
 * retrievable. If resize uses the wrong capacity, entries vanish.
 */
static void test_resize_preserves(void)
{
    TEST("resize preserves all entries");
    hash_table_t *ht = ht_create(4);
    ht_set(ht, "one", "1");
    ht_set(ht, "two", "2");
    ht_set(ht, "three", "3");
    /* This insert should trigger resize (load > 0.75) */
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

/*
 * Test 3: Overwrite updates value without leaking
 *
 * Insert a key, then set it again with a different value.
 * The old value should be freed (no leak) and the new value stored.
 *
 * NOTE: This test checks correctness. Run with valgrind to catch
 * the memory leak:  valgrind ./debug_hashtable
 */
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

/*
 * Test 4: Get finds keys by content, not by pointer
 *
 * Create a key string on the stack, insert it, then look it up
 * using a DIFFERENT string with the same content. If ht_get uses
 * pointer comparison (==), it will fail because the addresses differ.
 */
static void test_get_by_content(void)
{
    TEST("get finds keys by string content");
    hash_table_t *ht = ht_create(8);
    char key1[16];
    snprintf(key1, sizeof(key1), "fruit");
    ht_set(ht, key1, "apple");

    /* Look up with a different pointer but same string */
    char key2[16];
    snprintf(key2, sizeof(key2), "fruit");

    /* key1 and key2 are at different addresses */
    const char *v = ht_get(ht, key2);
    EXPECT(v != NULL && strcmp(v, "apple") == 0);
    ht_destroy(ht);
}

/*
 * Test 5: Probe handles a full table without hanging
 *
 * Fill every slot, then probe for a key that doesn't exist.
 * If probe doesn't limit its search, it loops forever.
 */
static void test_probe_full_table(void)
{
    TEST("probe returns -1 when table is full");
    /* Create a tiny table with capacity 4 */
    hash_table_t *ht = ht_create(4);

    /* Manually fill every slot (bypass ht_set to avoid resize) */
    for (int i = 0; i < 4; i++) {
        char buf[8];
        snprintf(buf, sizeof(buf), "k%d", i);
        ht->entries[i].key = strdup(buf);
        ht->entries[i].value = strdup("x");
        ht->size++;
    }

    /* Probe for a key that doesn't exist — should NOT hang */
    int result = ht_probe(ht, "missing");
    EXPECT(result == -1);
    ht_destroy(ht);
}

/* ── Main ────────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Hash Table (5 bugs to find) ===\n");
    printf("  NOTE: Bug 5 causes an infinite loop. If the program hangs\n");
    printf("  after test 4, that IS the bug — fix ht_probe() first!\n\n");
    fflush(stdout);

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
