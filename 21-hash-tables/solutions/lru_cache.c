/*
 * Solution: LRU Cache (Least Recently Used)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Data Structures ──────────────────────────────────────────────── */

typedef struct DLNode {
    int key;
    int value;
    struct DLNode *prev;
    struct DLNode *next;
} DLNode;

typedef struct {
    int capacity;
    int size;
    DLNode *head;       /* dummy head */
    DLNode *tail;       /* dummy tail */
    DLNode **table;     /* hash table: array of DLNode pointers */
    int table_size;
} LRUCache;

static int hash_key(int key, int table_size)
{
    unsigned int k = (unsigned int)key;
    return (int)(k % (unsigned int)table_size);
}

/* ── Doubly Linked List Helpers ──────────────────────────────────── */

static DLNode *create_node(int key, int value)
{
    DLNode *node = malloc(sizeof(DLNode));
    if (!node) return NULL;
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

static void remove_node(DLNode *node)
{
    /* Wire the neighbors to each other, bypassing this node */
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static void push_front(LRUCache *cache, DLNode *node)
{
    /* Insert node between dummy head and its current successor */
    node->next = cache->head->next;
    node->prev = cache->head;
    cache->head->next->prev = node;
    cache->head->next = node;
}

static void move_to_front(LRUCache *cache, DLNode *node)
{
    remove_node(node);
    push_front(cache, node);
}

/* ── Hash Table Helpers ──────────────────────────────────────────── */

/* Find a node in the hash table by key (linear probing) */
static DLNode *ht_find(LRUCache *cache, int key)
{
    int idx = hash_key(key, cache->table_size);
    for (int i = 0; i < cache->table_size; i++) {
        int probe = (idx + i) % cache->table_size;
        if (cache->table[probe] == NULL) {
            return NULL; /* empty slot — not found */
        }
        if (cache->table[probe] != (DLNode *)(long)-1 &&
            cache->table[probe]->key == key) {
            return cache->table[probe];
        }
    }
    return NULL;
}

/* Insert a node into the hash table */
static void ht_insert(LRUCache *cache, DLNode *node)
{
    int idx = hash_key(node->key, cache->table_size);
    for (int i = 0; i < cache->table_size; i++) {
        int probe = (idx + i) % cache->table_size;
        if (cache->table[probe] == NULL ||
            cache->table[probe] == (DLNode *)(long)-1) {
            cache->table[probe] = node;
            return;
        }
    }
}

/* Remove a node from the hash table (mark as deleted) */
static void ht_remove(LRUCache *cache, int key)
{
    int idx = hash_key(key, cache->table_size);
    for (int i = 0; i < cache->table_size; i++) {
        int probe = (idx + i) % cache->table_size;
        if (cache->table[probe] == NULL) {
            return; /* not found */
        }
        if (cache->table[probe] != (DLNode *)(long)-1 &&
            cache->table[probe]->key == key) {
            /* Use sentinel value to mark deleted (tombstone) */
            cache->table[probe] = (DLNode *)(long)-1;
            return;
        }
    }
}

/* ── LRU Cache Implementation ───────────────────────────────────── */

LRUCache *lru_create(int capacity)
{
    LRUCache *cache = malloc(sizeof(LRUCache));
    if (!cache) return NULL;

    cache->capacity = capacity;
    cache->size = 0;

    /* Create dummy head and tail, link them together */
    cache->head = create_node(0, 0);
    cache->tail = create_node(0, 0);
    if (!cache->head || !cache->tail) {
        free(cache->head);
        free(cache->tail);
        free(cache);
        return NULL;
    }
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;

    /* Hash table — use 2x capacity for fewer collisions */
    cache->table_size = capacity * 2;
    if (cache->table_size < 8) cache->table_size = 8;
    cache->table = calloc((size_t)cache->table_size, sizeof(DLNode *));
    if (!cache->table) {
        free(cache->head);
        free(cache->tail);
        free(cache);
        return NULL;
    }

    return cache;
}

int lru_get(LRUCache *cache, int key)
{
    DLNode *node = ht_find(cache, key);
    if (!node) return -1;

    /* Move to front — this key was just accessed */
    move_to_front(cache, node);
    return node->value;
}

void lru_put(LRUCache *cache, int key, int value)
{
    /* Check if key already exists */
    DLNode *existing = ht_find(cache, key);
    if (existing) {
        existing->value = value;
        move_to_front(cache, existing);
        return;
    }

    /* If at capacity, evict the LRU node (just before dummy tail) */
    if (cache->size >= cache->capacity) {
        DLNode *lru = cache->tail->prev;
        remove_node(lru);
        ht_remove(cache, lru->key);
        free(lru);
        cache->size--;
    }

    /* Insert new node */
    DLNode *node = create_node(key, value);
    if (!node) return;
    push_front(cache, node);
    ht_insert(cache, node);
    cache->size++;
}

void lru_destroy(LRUCache *cache)
{
    if (!cache) return;

    /* Walk from head to tail, freeing every node (including dummies) */
    DLNode *cur = cache->head;
    while (cur) {
        DLNode *next = cur->next;
        free(cur);
        cur = next;
    }

    free(cache->table);
    free(cache);
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== LRU Cache (Solution) ===\n\n");

    printf("Linked list helpers:\n");
    {
        LRUCache *c = lru_create(5);
        TEST("create cache with capacity 5")
            EXPECT(c != NULL);

        DLNode *n1 = create_node(1, 100);
        push_front(c, n1);
        TEST("push_front(1:100), head->next is node 1")
            EXPECT(c->head->next == n1);

        DLNode *n2 = create_node(2, 200);
        push_front(c, n2);
        TEST("push_front(2:200), head->next is node 2")
            EXPECT(c->head->next == n2 && n2->next == n1);

        move_to_front(c, n1);
        TEST("move_to_front(n1), head->next is node 1")
            EXPECT(c->head->next == n1 && n1->next == n2);

        /* Clean up manually for this test section */
        free(n1);
        free(n2);
        free(c->head);
        free(c->tail);
        free(c->table);
        free(c);
    }

    printf("\nCache operations:\n");
    {
        LRUCache *c = lru_create(2);

        lru_put(c, 1, 10);
        TEST("put(1,10), get(1) -> 10")
            EXPECT(lru_get(c, 1) == 10);

        TEST("get(2) -> -1 (not in cache)")
            EXPECT(lru_get(c, 2) == -1);

        lru_put(c, 2, 20);
        TEST("put(2,20), get(2) -> 20")
            EXPECT(lru_get(c, 2) == 20);

        TEST("get(1) still returns 10")
            EXPECT(lru_get(c, 1) == 10);

        lru_destroy(c);
    }

    printf("\nEviction:\n");
    {
        LRUCache *c = lru_create(2);

        lru_put(c, 1, 10);
        lru_put(c, 2, 20);

        lru_put(c, 3, 30);
        TEST("put(3,30) evicts key 1")
            EXPECT(lru_get(c, 1) == -1);

        TEST("key 3 is in cache")
            EXPECT(lru_get(c, 3) == 30);

        TEST("key 2 is still in cache")
            EXPECT(lru_get(c, 2) == 20);

        lru_put(c, 4, 40);
        TEST("put(4,40) evicts key 3 (2 was accessed)")
            EXPECT(lru_get(c, 3) == -1);

        TEST("key 4 is in cache")
            EXPECT(lru_get(c, 4) == 40);

        lru_destroy(c);
    }

    printf("\nUpdate existing:\n");
    {
        LRUCache *c = lru_create(2);

        lru_put(c, 1, 10);
        lru_put(c, 2, 20);

        lru_put(c, 1, 100);
        TEST("update key 1 to 100")
            EXPECT(lru_get(c, 1) == 100);

        lru_put(c, 3, 30);
        TEST("put(3,30) evicts key 2 (not 1)")
            EXPECT(lru_get(c, 2) == -1 && lru_get(c, 1) == 100);

        lru_destroy(c);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
