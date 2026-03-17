/*
 * Solution: Hash Table with Separate Chaining
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
    ht_entry_t **buckets;
    int capacity;
    int size;
} hash_table_t;

/* ── Hash Function ────────────────────────────────────────────────── */

static unsigned int hash(const char *key, int capacity)
{
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += (unsigned char)key[i];
    }
    return sum % (unsigned int)capacity;
}

/* ── ht_create ────────────────────────────────────────────────────── */

hash_table_t *ht_create(int capacity)
{
    hash_table_t *ht = malloc(sizeof(hash_table_t));
    if (!ht) return NULL;
    ht->buckets = calloc((size_t)capacity, sizeof(ht_entry_t *));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }
    ht->capacity = capacity;
    ht->size = 0;
    return ht;
}

/* ── ht_destroy ───────────────────────────────────────────────────── */

void ht_destroy(hash_table_t *ht)
{
    for (int i = 0; i < ht->capacity; i++) {
        ht_entry_t *node = ht->buckets[i];
        while (node) {
            ht_entry_t *next = node->next;
            free(node->key);
            free(node);
            node = next;
        }
    }
    free(ht->buckets);
    free(ht);
}

/* ── ht_set ───────────────────────────────────────────────────────── */

void ht_set(hash_table_t *ht, const char *key, int value)
{
    unsigned int index = hash(key, ht->capacity);

    /* Check if key already exists — update if so */
    ht_entry_t *node = ht->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    /* Key is new — prepend a new node */
    ht_entry_t *new_node = malloc(sizeof(ht_entry_t));
    if (!new_node) return;
    new_node->key = strdup(key);
    if (!new_node->key) {
        free(new_node);
        return;
    }
    new_node->value = value;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->size++;
}

/* ── ht_get ───────────────────────────────────────────────────────── */

int ht_get(const hash_table_t *ht, const char *key, int *out_value)
{
    unsigned int index = hash(key, ht->capacity);
    ht_entry_t *node = ht->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            *out_value = node->value;
            return 0;
        }
        node = node->next;
    }
    return -1;
}

/* ── ht_delete ────────────────────────────────────────────────────── */

int ht_delete(hash_table_t *ht, const char *key)
{
    unsigned int index = hash(key, ht->capacity);
    ht_entry_t *node = ht->buckets[index];
    ht_entry_t *prev = NULL;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                ht->buckets[index] = node->next;
            }
            free(node->key);
            free(node);
            ht->size--;
            return 0;
        }
        prev = node;
        node = node->next;
    }
    return -1;
}

/* ── ht_size ──────────────────────────────────────────────────────── */

int ht_size(const hash_table_t *ht)
{
    return ht->size;
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
    printf("=== Hash Table (Solution) ===\n\n");

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
