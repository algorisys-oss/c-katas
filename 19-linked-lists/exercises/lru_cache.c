/*
 * Kata: LRU Cache
 *
 * Build a Least Recently Used (LRU) cache using a doubly linked list.
 *
 * How it works:
 *   - The cache holds up to `capacity` key-value pairs
 *   - When you access a key (get or put), it becomes the most recently used
 *   - When the cache is full and you insert a new key, the least recently
 *     used item (the tail of the list) is evicted
 *   - The most recently used item is always at the head of the list
 *
 * Functions:
 *   - lru_create(capacity): create a cache with the given capacity
 *   - lru_destroy(cache):   free all nodes and the cache struct
 *   - lru_get(cache, key):  return value for key, or -1 if not found
 *                            (moves accessed node to front)
 *   - lru_put(cache, key, value): insert or update key-value pair
 *                            (evicts LRU item if cache is full)
 *
 * 10 tests total.
 *
 * Note: This implementation uses a linear scan to find keys (no hash table).
 *       That's O(n) per lookup but keeps the focus on the linked list logic.
 *
 * Hints:
 *   - Each node stores a key and a value
 *   - Use a doubly linked list so you can remove any node in O(1)
 *   - "Move to front" = remove from current position + insert at head
 *   - "Evict LRU" = remove the tail node
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

/* ── Exercise 1: lru_create ──────────────────────────────────────────
 * Allocate and initialize an LRU cache with the given capacity.
 * head and tail start as NULL, size starts at 0.
 */
LRUCache *lru_create(int capacity)
{
    /* TODO: malloc an LRUCache, initialize fields, return it */
    (void)capacity;
    return NULL; /* placeholder */
}

/* ── Exercise 2: lru_destroy ─────────────────────────────────────────
 * Free every node in the cache, then free the cache struct itself.
 */
void lru_destroy(LRUCache *cache)
{
    /* TODO: walk the list from head, free each node, then free cache */
    (void)cache;
}

/* ── Helper: remove_node ─────────────────────────────────────────────
 * Remove a node from its current position in the doubly linked list.
 * Does NOT free the node — just unlinks it.
 * Update head/tail pointers if needed.
 */
static void remove_node(LRUCache *cache, DNode *node)
{
    /* TODO: rewire prev/next pointers, update head/tail if needed */
    (void)cache; (void)node;
}

/* ── Helper: insert_at_head ──────────────────────────────────────────
 * Insert a node at the head of the doubly linked list.
 * Update head (and tail if the list was empty).
 */
static void insert_at_head(LRUCache *cache, DNode *node)
{
    /* TODO: wire node as the new head */
    (void)cache; (void)node;
}

/* ── Helper: find_node ───────────────────────────────────────────────
 * Walk the list to find a node with the matching key.
 * Return the node pointer, or NULL if not found.
 */
static DNode *find_node(LRUCache *cache, int key)
{
    /* TODO: linear scan through the list */
    (void)cache; (void)key;
    return NULL; /* placeholder */
}

/* ── Exercise 3: lru_get ─────────────────────────────────────────────
 * Look up a key in the cache.
 *   - If found: move the node to the front (most recently used),
 *               return its value
 *   - If not found: return -1
 */
int lru_get(LRUCache *cache, int key)
{
    /* TODO: find node, move to front if found, return value or -1 */
    (void)cache; (void)key;
    return -1; /* placeholder */
}

/* ── Exercise 4: lru_put ─────────────────────────────────────────────
 * Insert or update a key-value pair.
 *   - If the key already exists: update its value, move to front
 *   - If the key is new:
 *       - If cache is full: evict the tail (LRU item), free it
 *       - Create a new node and insert at head
 *       - Update size
 */
void lru_put(LRUCache *cache, int key, int value)
{
    /* TODO: handle update, eviction, and insertion */
    (void)cache; (void)key; (void)value;
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
