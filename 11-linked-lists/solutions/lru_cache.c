/*
 * Solution: LRU Cache
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Data Structures ─────────────────────────────────────────────── */

typedef struct DNode {
    int key;
    int value;
    struct DNode *prev;
    struct DNode *next;
} DNode;

typedef struct {
    DNode *head;    /* most recently used */
    DNode *tail;    /* least recently used */
    int size;
    int capacity;
} LRUCache;

/* ── lru_create ──────────────────────────────────────────────────── */

LRUCache *lru_create(int capacity)
{
    LRUCache *cache = malloc(sizeof(LRUCache));
    if (cache == NULL) return NULL;
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
    cache->capacity = capacity;
    return cache;
}

/* ── lru_destroy ─────────────────────────────────────────────────── */

void lru_destroy(LRUCache *cache)
{
    if (cache == NULL) return;
    DNode *curr = cache->head;
    while (curr != NULL) {
        DNode *next = curr->next;
        free(curr);
        curr = next;
    }
    free(cache);
}

/* ── Helper: remove_node ─────────────────────────────────────────── */

static void remove_node(LRUCache *cache, DNode *node)
{
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        cache->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        cache->tail = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
}

/* ── Helper: insert_at_head ──────────────────────────────────────── */

static void insert_at_head(LRUCache *cache, DNode *node)
{
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head != NULL) {
        cache->head->prev = node;
    }
    cache->head = node;

    if (cache->tail == NULL) {
        cache->tail = node;
    }
}

/* ── Helper: find_node ───────────────────────────────────────────── */

static DNode *find_node(LRUCache *cache, int key)
{
    DNode *curr = cache->head;
    while (curr != NULL) {
        if (curr->key == key) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/* ── lru_get ─────────────────────────────────────────────────────── */

int lru_get(LRUCache *cache, int key)
{
    DNode *node = find_node(cache, key);
    if (node == NULL) {
        return -1;
    }
    /* Move to front (most recently used) */
    remove_node(cache, node);
    insert_at_head(cache, node);
    return node->value;
}

/* ── lru_put ─────────────────────────────────────────────────────── */

void lru_put(LRUCache *cache, int key, int value)
{
    DNode *node = find_node(cache, key);

    if (node != NULL) {
        /* Key exists: update value and move to front */
        node->value = value;
        remove_node(cache, node);
        insert_at_head(cache, node);
        return;
    }

    /* Key is new — check if we need to evict */
    if (cache->size >= cache->capacity) {
        /* Evict the tail (least recently used) */
        DNode *lru = cache->tail;
        remove_node(cache, lru);
        free(lru);
        cache->size--;
    }

    /* Create new node and insert at head */
    DNode *new_node = malloc(sizeof(DNode));
    if (new_node == NULL) return;
    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    insert_at_head(cache, new_node);
    cache->size++;
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
    printf("=== LRU Cache ===\n\n");

    /* ── Create ──────────────────────────────────────────────── */
    printf("lru_create:\n");
    LRUCache *cache = lru_create(3);
    TEST("create returns non-NULL") EXPECT(cache != NULL);
    TEST("initial size is 0") EXPECT(cache->size == 0);

    /* ── Put & Get basics ────────────────────────────────────── */
    printf("\nlru_put and lru_get:\n");
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);
    lru_put(cache, 3, 300);
    TEST("get key 1 returns 100") EXPECT(lru_get(cache, 1) == 100);
    TEST("get key 2 returns 200") EXPECT(lru_get(cache, 2) == 200);
    TEST("get missing key returns -1") EXPECT(lru_get(cache, 99) == -1);

    /* ── Eviction ────────────────────────────────────────────── */
    printf("\neviction:\n");
    /* Cache: 2(MRU) -> 1 -> 3(LRU), because we just accessed 1 then 2 */
    lru_put(cache, 4, 400);  /* should evict key 3 (LRU) */
    TEST("after eviction, key 3 is gone") EXPECT(lru_get(cache, 3) == -1);
    TEST("key 4 is present") EXPECT(lru_get(cache, 4) == 400);
    TEST("size stays at capacity") EXPECT(cache->size == 3);

    /* ── Update existing key ─────────────────────────────────── */
    printf("\nupdate existing key:\n");
    lru_put(cache, 1, 111);
    TEST("update key 1 to 111") EXPECT(lru_get(cache, 1) == 111);

    /* ── Access promotes item (prevents eviction) ────────────── */
    printf("\naccess-based ordering:\n");
    lru_destroy(cache);
    cache = lru_create(2);
    lru_put(cache, 10, 1000);
    lru_put(cache, 20, 2000);
    lru_get(cache, 10);         /* promote key 10 — key 20 is now LRU */
    lru_put(cache, 30, 3000);   /* should evict key 20, not 10 */
    TEST("get promotes: key 10 survives eviction") EXPECT(lru_get(cache, 10) == 1000);

    /* ── Cleanup ─────────────────────────────────────────────── */
    lru_destroy(cache);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
