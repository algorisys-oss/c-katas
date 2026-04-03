/*
 * Kata: Hash Table with Separate Chaining
 *
 * Build a hash table from scratch using an array of linked lists.
 * String keys, integer values.
 *
 *   - ht_create(capacity)  — allocate a hash table with given bucket count
 *   - ht_destroy(ht)       — free all memory
 *   - ht_set(ht, key, val) — insert or update a key-value pair
 *   - ht_get(ht, key, out) — lookup a key, store value in *out
 *   - ht_delete(ht, key)   — remove a key-value pair
 *   - ht_size(ht)          — return number of entries
 *
 * 14 tests total.
 *
 * Hints:
 *   - Each bucket is a linked list of (key, value) nodes
 *   - Use strdup() to copy keys (don't store the caller's pointer)
 *   - Hash function: sum of character ASCII values, modulo capacity
 *   - Remember to free both the key string and the node on delete/destroy
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ── Data Structures ──────────────────────────────────────────────── */

typedef struct ht_entry {
    char *key;
    int value;
    struct ht_entry *next;
} ht_entry_t;

typedef struct {
    ht_entry_t **buckets;   /* array of linked list heads */
    int capacity;           /* number of buckets */
    int size;               /* number of entries */
} hash_table_t;

/* ── Hash Function ────────────────────────────────────────────────── */

/* Simple hash: sum of character values modulo capacity. */
static unsigned int hash(const char *key, int capacity)
{
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += (unsigned char)key[i];
    }
    return sum % (unsigned int)capacity;
}

/* ── Exercise 1: ht_create ────────────────────────────────────────
 * Allocate and return a new hash table with the given capacity.
 * All buckets should be initialized to NULL.
 */
hash_table_t *ht_create(int capacity)
{
    /* TODO: malloc the hash_table_t struct */
    /* TODO: malloc the buckets array (array of ht_entry_t pointers) */
    /* TODO: initialize all buckets to NULL */
    /* TODO: set capacity and size */
    return NULL; /* placeholder */
}

/* ── Exercise 2: ht_destroy ───────────────────────────────────────
 * Free all memory: every node's key, every node, the buckets array,
 * and the hash table struct itself.
 */
void ht_destroy(hash_table_t *ht)
{
    /* TODO: for each bucket, walk the linked list and free each node */
    /*       (free node->key, then free node) */
    /* TODO: free the buckets array */
    /* TODO: free the hash table struct */
}

/* ── Exercise 3: ht_set ───────────────────────────────────────────
 * Insert a key-value pair. If the key already exists, update its value.
 * Use strdup() to make a copy of the key string.
 */
void ht_set(hash_table_t *ht, const char *key, int value)
{
    /* TODO: compute the bucket index using hash() */
    /* TODO: walk the chain — if key exists, update value and return */
    /* TODO: if key is new, allocate a new node, strdup the key */
    /* TODO: prepend the new node to the bucket's linked list */
    /* TODO: increment ht->size */
}

/* ── Exercise 4: ht_get ───────────────────────────────────────────
 * Look up a key. If found, store the value in *out_value and return 0.
 * If not found, return -1 (don't modify *out_value).
 */
int ht_get(const hash_table_t *ht, const char *key, int *out_value)
{
    /* TODO: compute the bucket index */
    /* TODO: walk the chain, comparing keys with strcmp() */
    /* TODO: if found, set *out_value and return 0 */
    return -1; /* placeholder: not found */
}

/* ── Exercise 5: ht_delete ────────────────────────────────────────
 * Remove the entry with the given key.
 * Return 0 if deleted, -1 if key not found.
 * Remember to free the key string and the node.
 */
int ht_delete(hash_table_t *ht, const char *key)
{
    /* TODO: compute the bucket index */
    /* TODO: walk the chain with a "prev" pointer to unlink the node */
    /* TODO: free the key, free the node, decrement size, return 0 */
    return -1; /* placeholder: not found */
}

/* ── Exercise 6: ht_size ──────────────────────────────────────────
 * Return the number of entries in the hash table.
 */
int ht_size(const hash_table_t *ht)
{
    /* TODO: return the size field */
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-40s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Hash Table ===\n\n");

    /* --- ht_create / ht_size --- */
    printf("ht_create / ht_size:\n");
    hash_table_t *ht = ht_create(8);
    TEST("new table size is 0") EXPECT(ht_size(ht) == 0);

    /* --- ht_set / ht_get --- */
    printf("\nht_set / ht_get:\n");
    {
        int val;
        ht_set(ht, "alice", 100);
        TEST("set alice=100, get alice") EXPECT(ht_get(ht, "alice", &val) == 0 && val == 100);
    }
    {
        int val;
        ht_set(ht, "bob", 200);
        TEST("set bob=200, get bob") EXPECT(ht_get(ht, "bob", &val) == 0 && val == 200);
    }
    {
        int val;
        TEST("get nonexistent key → -1") EXPECT(ht_get(ht, "charlie", &val) == -1);
    }
    TEST("size after 2 inserts") EXPECT(ht_size(ht) == 2);

    /* --- update existing key --- */
    printf("\nupdate existing key:\n");
    {
        int val;
        ht_set(ht, "alice", 999);
        TEST("update alice=999, get alice") EXPECT(ht_get(ht, "alice", &val) == 0 && val == 999);
        TEST("size unchanged after update") EXPECT(ht_size(ht) == 2);
    }

    /* --- ht_delete --- */
    printf("\nht_delete:\n");
    {
        int val;
        TEST("delete bob → 0") EXPECT(ht_delete(ht, "bob") == 0);
        TEST("get bob after delete → -1") EXPECT(ht_get(ht, "bob", &val) == -1);
        TEST("size after delete") EXPECT(ht_size(ht) == 1);
        TEST("delete nonexistent → -1") EXPECT(ht_delete(ht, "nobody") == -1);
    }

    /* --- collision handling --- */
    printf("\ncollision handling:\n");
    {
        /* "act" and "cat" have same char sum (97+99+116 = 312) */
        int val;
        ht_set(ht, "cat", 10);
        ht_set(ht, "act", 20);
        TEST("colliding keys: get cat") EXPECT(ht_get(ht, "cat", &val) == 0 && val == 10);
        TEST("colliding keys: get act") EXPECT(ht_get(ht, "act", &val) == 0 && val == 20);
        TEST("delete from collision chain") EXPECT(ht_delete(ht, "cat") == 0 && ht_get(ht, "act", &val) == 0 && val == 20);
    }

    /* --- cleanup --- */
    ht_destroy(ht);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
