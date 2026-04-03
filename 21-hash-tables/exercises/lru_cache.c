/*
 * Kata: LRU Cache (Least Recently Used)
 *
 * ── What is a cache? ────────────────────────────────────────────────
 *
 * A cache is fast storage for frequently used data. Your browser has one:
 * instead of downloading the same image every time you visit a page, it
 * stores a copy locally. RAM is a cache for disk. L1 cache is a cache
 * for RAM. Caches are EVERYWHERE in computing.
 *
 * The problem: caches have limited space. When the cache is full and you
 * need to store something new, which old item do you throw away?
 *
 * ── LRU eviction policy ────────────────────────────────────────────
 *
 * LRU = Least Recently Used. Evict the item that hasn't been accessed
 * for the longest time. The intuition: if you haven't used something
 * recently, you're less likely to use it soon.
 *
 * ── Why TWO data structures? ───────────────────────────────────────
 *
 * We need:
 *   - O(1) lookup by key        → hash table
 *   - O(1) insert/remove/reorder → doubly linked list
 *
 * Neither data structure alone gives us both. Together, they do.
 *
 * ── How it works ────────────────────────────────────────────────────
 *
 *   Hash Table                     Doubly Linked List
 *   (key -> node pointer)          (most recent at head, LRU at tail)
 *
 *   ┌───────────────┐
 *   │ key=1 ──────────────→ ┌──────────────────────────────────┐
 *   ├───────────────┤       │                                  │
 *   │ key=2 ────────────┐   │  [DUMMY HEAD] ↔ [1:A] ↔ [3:C] ↔ [2:B] ↔ [DUMMY TAIL]
 *   ├───────────────┤   │   │    (most recent)         (least recent) │
 *   │ key=3 ──────────┐ │   └──────────────────────────────────┘
 *   ├───────────────┤ │ │
 *   │  (empty)      │ │ └──→ points to node [2:B]
 *   └───────────────┘ └────→ points to node [3:C]
 *
 *   Dummy head and tail simplify edge cases — you never have to
 *   check for NULL when inserting/removing at the ends.
 *
 * ── Operations ──────────────────────────────────────────────────────
 *
 *   GET(key):
 *     1. Look up key in hash table → get node pointer         O(1)
 *     2. If found: move node to front of list (most recent)   O(1)
 *     3. Return value (or -1 if not found)
 *
 *   PUT(key, value):
 *     1. If key exists: update value, move to front            O(1)
 *     2. If key is new:
 *        a. If at capacity: remove tail node (LRU), delete from hash table
 *        b. Create new node, insert at front of list
 *        c. Add to hash table
 *
 *   Before PUT(4, D) with capacity=3:
 *     [HEAD] ↔ [1:A] ↔ [3:C] ↔ [2:B] ↔ [TAIL]
 *                                  ↑ LRU — will be evicted
 *
 *   After PUT(4, D):
 *     [HEAD] ↔ [4:D] ↔ [1:A] ↔ [3:C] ↔ [TAIL]
 *               ↑ new              ↑ now LRU
 *     Node [2:B] was evicted and freed.
 *
 * 14 tests total.
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
    DLNode *head;       /* dummy head — never holds real data */
    DLNode *tail;       /* dummy tail — never holds real data */
    DLNode **table;     /* hash table: array of DLNode pointers */
    int table_size;
} LRUCache;

/* Simple hash function for integer keys */
static int hash_key(int key, int table_size)
{
    unsigned int k = (unsigned int)key;
    return (int)(k % (unsigned int)table_size);
}

/* ── Exercise 1: Doubly linked list helpers ───────────────────────── */

/* Create a new node with given key and value */
static DLNode *create_node(int key, int value)
{
    /* TODO: malloc a DLNode, set key, value, prev=NULL, next=NULL */
    (void)key; (void)value;
    return NULL; /* placeholder */
}

/* Remove a node from the list (does NOT free it).
 * Because we have dummy head/tail, we don't need to check for NULL. */
static void remove_node(DLNode *node)
{
    /* TODO: wire node->prev and node->next to each other */
    (void)node;
}

/* Insert a node right after the dummy head (making it the most recent) */
static void push_front(LRUCache *cache, DLNode *node)
{
    /* TODO: insert node between cache->head and cache->head->next */
    (void)cache; (void)node;
}

/* Move an existing node to the front (most recently used) */
static void move_to_front(LRUCache *cache, DLNode *node)
{
    /* TODO: remove from current position, then push_front */
    (void)cache; (void)node;
}

/* ── Exercise 2: lru_create ───────────────────────────────────────── */

/* Create an LRU cache with the given capacity.
 * - Allocate the LRUCache struct
 * - Allocate dummy head and tail nodes, link them together
 * - Allocate the hash table (use table_size = capacity * 2 for fewer collisions)
 * - Initialize all table slots to NULL
 */
LRUCache *lru_create(int capacity)
{
    /* TODO: allocate and initialize all components */
    (void)capacity;
    return NULL; /* placeholder */
}

/* ── Exercise 3: lru_get ──────────────────────────────────────────── */

/* Look up a key in the cache.
 * - Hash the key to find the table slot
 * - Linear probe to find the matching node (or NULL)
 * - If found: move the node to front (it was just accessed), return value
 * - If not found: return -1
 */
int lru_get(LRUCache *cache, int key)
{
    /* TODO: hash lookup + move to front on hit */
    (void)cache; (void)key;
    return -1; /* placeholder */
}

/* ── Exercise 4: lru_put ──────────────────────────────────────────── */

/* Insert or update a key-value pair.
 * - If key exists: update value, move to front
 * - If key is new:
 *   - If at capacity: evict the LRU node (tail->prev)
 *     - Remove it from the list
 *     - Remove it from the hash table
 *     - Free it
 *     - Decrement size
 *   - Create a new node
 *   - Push it to front of list
 *   - Insert into hash table
 *   - Increment size
 */
void lru_put(LRUCache *cache, int key, int value)
{
    /* TODO: implement insert/update with eviction */
    (void)cache; (void)key; (void)value;
}

/* ── lru_destroy ──────────────────────────────────────────────────── */

/* Free all memory: walk the list from head to tail, free each node,
 * then free the table and the cache struct. */
void lru_destroy(LRUCache *cache)
{
    if (!cache) return;
    /* TODO: free all nodes by walking head->next until tail,
     * then free table, then free cache */
    (void)cache;
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

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

    /* --- Doubly linked list helpers --- */
    printf("Linked list helpers:\n");
    {
        LRUCache *c = lru_create(5);
        TEST("create cache with capacity 5")
            EXPECT(c != NULL);

        DLNode *n1 = create_node(1, 100);
        push_front(c, n1);
        TEST("push_front(1:100), head->next is node 1")
            EXPECT(c != NULL && c->head->next == n1);

        DLNode *n2 = create_node(2, 200);
        push_front(c, n2);
        TEST("push_front(2:200), head->next is node 2")
            EXPECT(c != NULL && c->head->next == n2 && n2->next == n1);

        move_to_front(c, n1);
        TEST("move_to_front(n1), head->next is node 1")
            EXPECT(c != NULL && c->head->next == n1 && n1->next == n2);

        /* Clean up manually for this test section */
        if (n1) free(n1);
        if (n2) free(n2);
        if (c) {
            free(c->head);
            free(c->tail);
            free(c->table);
            free(c);
        }
    }

    /* --- Basic cache operations --- */
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

    /* --- Eviction --- */
    printf("\nEviction:\n");
    {
        LRUCache *c = lru_create(2);

        lru_put(c, 1, 10);
        lru_put(c, 2, 20);

        /* Cache is full: [2, 1]. Adding 3 should evict 1 (LRU) */
        lru_put(c, 3, 30);
        TEST("put(3,30) evicts key 1")
            EXPECT(lru_get(c, 1) == -1);

        TEST("key 3 is in cache")
            EXPECT(lru_get(c, 3) == 30);

        TEST("key 2 is still in cache")
            EXPECT(lru_get(c, 2) == 20);

        /* Now cache has [2, 3]. Adding 4 evicts 3 (LRU, since 2 was just accessed) */
        lru_put(c, 4, 40);
        TEST("put(4,40) evicts key 3 (2 was accessed)")
            EXPECT(lru_get(c, 3) == -1);

        TEST("key 4 is in cache")
            EXPECT(lru_get(c, 4) == 40);

        lru_destroy(c);
    }

    /* --- Update existing key --- */
    printf("\nUpdate existing:\n");
    {
        LRUCache *c = lru_create(2);

        lru_put(c, 1, 10);
        lru_put(c, 2, 20);

        /* Update key 1 — this should move it to front */
        lru_put(c, 1, 100);
        TEST("update key 1 to 100")
            EXPECT(lru_get(c, 1) == 100);

        /* Adding key 3 should evict 2 (not 1, since 1 was updated) */
        lru_put(c, 3, 30);
        TEST("put(3,30) evicts key 2 (not 1)")
            EXPECT(lru_get(c, 2) == -1 && lru_get(c, 1) == 100);

        lru_destroy(c);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
