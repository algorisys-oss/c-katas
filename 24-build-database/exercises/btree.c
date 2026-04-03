/*
 * Kata: In-Memory B-Tree (Order 3)
 *
 * Build a B-Tree that maps int keys to int values.
 * Order 3 means: each node holds up to 2 keys and up to 3 children.
 * (Minimum degree t=2.)
 *
 * Functions to implement:
 *   - btree_create()            — allocate an empty tree
 *   - btree_destroy(tree)       — free all nodes
 *   - btree_insert(tree, k, v)  — insert key-value pair (with splitting)
 *   - btree_search(tree, k, &v) — find key, return 0 if found, -1 if not
 *   - btree_size(tree)          — count of key-value pairs in the tree
 *
 * 14 tests total.
 *
 * Hints:
 *   - When a node is full (2 keys) and you need to insert, split it first.
 *   - Splitting pushes the median key up to the parent.
 *   - The root can split too — this is the only way the tree grows taller.
 *   - Keep keys sorted within each node.
 */

#include <stdio.h>
#include <stdlib.h>
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

/* ── Constants ────────────────────────────────────────────────────── */

#define BTREE_ORDER  3          /* max children per node */
#define MAX_KEYS     (BTREE_ORDER - 1)  /* max keys per node = 2 */

/* ── Data structures ──────────────────────────────────────────────── */

typedef struct btree_node {
    int keys[MAX_KEYS];
    int values[MAX_KEYS];
    struct btree_node *children[BTREE_ORDER];
    int num_keys;
    int is_leaf;
} btree_node_t;

typedef struct {
    btree_node_t *root;
    int size;
} btree_t;

/* ── Helper: create a new node ────────────────────────────────────── */

static btree_node_t *btree_node_create(int is_leaf)
{
    btree_node_t *node = malloc(sizeof(btree_node_t));
    if (!node) {
        return NULL;
    }
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < BTREE_ORDER; i++) {
        node->children[i] = NULL;
    }
    return node;
}

/* ── TODO: Implement these functions ──────────────────────────────── */

/*
 * btree_create — allocate and return a new empty B-Tree.
 * The tree starts with a single empty leaf node as root.
 */
btree_t *btree_create(void)
{
    /* TODO: malloc a btree_t, create root as empty leaf, size=0 */
    return NULL;
}

/*
 * btree_destroy_node — recursively free a node and all its children.
 * (Helper for btree_destroy.)
 */
static void btree_destroy_node(btree_node_t *node)
{
    /* TODO: if node is NULL, return. If not leaf, recurse on children. Free node. */
    (void)node;
}

/*
 * btree_destroy — free the entire tree (all nodes + the tree struct).
 */
void btree_destroy(btree_t *tree)
{
    /* TODO: call btree_destroy_node on root, then free tree */
    (void)tree;
}

/*
 * btree_split_child — split a full child of 'parent' at index 'idx'.
 *
 * The child at parent->children[idx] is full (2 keys). Split it:
 *   1. Create a new node that gets the right half of the child's keys.
 *   2. Push the median key up into 'parent'.
 *   3. Update parent's children array.
 *
 * This is the heart of B-Tree insertion.
 */
static void btree_split_child(btree_node_t *parent, int idx)
{
    /* TODO:
     * - Let child = parent->children[idx]  (the full node)
     * - Create new_node (same leaf status as child)
     * - The median is child->keys[1] (the second key in a 2-key node)
     *   Actually for order 3 (MAX_KEYS=2), when we temporarily have 3 keys:
     *   median index is 1. Right half gets keys after median.
     *
     * For simplicity with order 3:
     * - new_node gets child->keys[MAX_KEYS-1] and child->values[MAX_KEYS-1]
     *   (Nope — think carefully about the split logic!)
     * - Shift parent's keys/children to make room at idx
     * - Insert median key/value into parent at idx
     * - parent->children[idx+1] = new_node
     * - Adjust num_keys for all three nodes
     */
    (void)parent; (void)idx;
}

/*
 * btree_insert_nonfull — insert key/value into a node that is NOT full.
 *
 * If node is a leaf, insert directly in sorted order.
 * If node is internal, find the right child and recurse.
 *   If that child is full, split it first, then decide which half to recurse into.
 */
static void btree_insert_nonfull(btree_node_t *node, int key, int value)
{
    /* TODO:
     * If leaf:
     *   - Find the right position for 'key' (shifting larger keys right)
     *   - Insert key and value
     *   - Increment num_keys
     *
     * If internal:
     *   - Find child index i such that key goes into children[i]
     *   - If children[i] is full (num_keys == MAX_KEYS), split it
     *     - After split, check if key goes to children[i] or children[i+1]
     *   - Recurse into the correct child
     */
    (void)node; (void)key; (void)value;
}

/*
 * btree_insert — insert a key-value pair into the tree.
 *
 * If the root is full, split it first (this grows the tree taller).
 * Then call btree_insert_nonfull on the (now non-full) root.
 */
void btree_insert(btree_t *tree, int key, int value)
{
    /* TODO:
     * - If root is full:
     *   - Create a new root node (internal, not leaf)
     *   - Old root becomes child[0] of new root
     *   - Split old root via btree_split_child(new_root, 0)
     *   - Update tree->root
     * - Call btree_insert_nonfull on tree->root
     * - Increment tree->size
     */
    (void)tree; (void)key; (void)value;
}

/*
 * btree_search — search for 'key' in the tree.
 * If found, store the value in *out_value and return 0.
 * If not found, return -1.
 */
int btree_search(btree_t *tree, int key, int *out_value)
{
    /* TODO:
     * Start at root. At each node:
     *   - Scan keys to find key or the right child to follow
     *   - If found, set *out_value and return 0
     *   - If leaf and not found, return -1
     *   - If internal and not found, recurse into correct child
     */
    (void)tree; (void)key; (void)out_value;
    return -1;
}

/*
 * btree_size — return the number of key-value pairs in the tree.
 */
int btree_size(btree_t *tree)
{
    /* TODO: just return tree->size */
    (void)tree;
    return 0;
}

/* ── Tests ────────────────────────────────────────────────────────── */

int main(void)
{
    printf("=== B-Tree Tests ===\n\n");

    /* Test 1: create an empty tree */
    {
        btree_t *tree = btree_create();
        TEST("create empty tree");
        EXPECT(tree != NULL && btree_size(tree) == 0);
        btree_destroy(tree);
    }

    /* Test 2: insert one key and search for it */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 42, 100);
        int val = 0;
        int found = btree_search(tree, 42, &val);
        TEST("insert and search single key");
        EXPECT(found == 0 && val == 100);
        btree_destroy(tree);
    }

    /* Test 3: size after one insert */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 42, 100);
        TEST("size is 1 after one insert");
        EXPECT(btree_size(tree) == 1);
        btree_destroy(tree);
    }

    /* Test 4: search miss on empty tree */
    {
        btree_t *tree = btree_create();
        int val = 0;
        int found = btree_search(tree, 99, &val);
        TEST("search miss on empty tree");
        EXPECT(found == -1);
        btree_destroy(tree);
    }

    /* Test 5: search miss on non-empty tree */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 10, 1);
        btree_insert(tree, 20, 2);
        int val = 0;
        int found = btree_search(tree, 15, &val);
        TEST("search miss on non-empty tree");
        EXPECT(found == -1);
        btree_destroy(tree);
    }

    /* Test 6: insert two keys (no split needed) */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 10, 1);
        btree_insert(tree, 20, 2);
        int v1 = 0, v2 = 0;
        btree_search(tree, 10, &v1);
        btree_search(tree, 20, &v2);
        TEST("insert two keys, no split");
        EXPECT(v1 == 1 && v2 == 2 && btree_size(tree) == 2);
        btree_destroy(tree);
    }

    /* Test 7: insert three keys (triggers first root split) */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 10, 1);
        btree_insert(tree, 20, 2);
        btree_insert(tree, 5, 3);
        int v1 = 0, v2 = 0, v3 = 0;
        btree_search(tree, 10, &v1);
        btree_search(tree, 20, &v2);
        btree_search(tree, 5, &v3);
        TEST("three inserts trigger root split");
        EXPECT(v1 == 1 && v2 == 2 && v3 == 3 && btree_size(tree) == 3);
        btree_destroy(tree);
    }

    /* Test 8: insert six keys (multiple splits) */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 10, 1);
        btree_insert(tree, 20, 2);
        btree_insert(tree, 5, 3);
        btree_insert(tree, 15, 4);
        btree_insert(tree, 25, 5);
        btree_insert(tree, 30, 6);
        TEST("six inserts, multiple splits, size correct");
        EXPECT(btree_size(tree) == 6);
        btree_destroy(tree);
    }

    /* Test 9: all six keys are searchable after splits */
    {
        btree_t *tree = btree_create();
        int keys[] = {10, 20, 5, 15, 25, 30};
        int vals[] = {1, 2, 3, 4, 5, 6};
        for (int i = 0; i < 6; i++) {
            btree_insert(tree, keys[i], vals[i]);
        }
        int all_found = 1;
        for (int i = 0; i < 6; i++) {
            int v = 0;
            if (btree_search(tree, keys[i], &v) != 0 || v != vals[i]) {
                all_found = 0;
            }
        }
        TEST("all six keys searchable after splits");
        EXPECT(all_found);
        btree_destroy(tree);
    }

    /* Test 10: insert 20 sequential keys */
    {
        btree_t *tree = btree_create();
        for (int i = 1; i <= 20; i++) {
            btree_insert(tree, i, i * 10);
        }
        TEST("20 sequential inserts, size correct");
        EXPECT(btree_size(tree) == 20);
        btree_destroy(tree);
    }

    /* Test 11: all 20 sequential keys are searchable */
    {
        btree_t *tree = btree_create();
        for (int i = 1; i <= 20; i++) {
            btree_insert(tree, i, i * 10);
        }
        int all_found = 1;
        for (int i = 1; i <= 20; i++) {
            int v = 0;
            if (btree_search(tree, i, &v) != 0 || v != i * 10) {
                all_found = 0;
            }
        }
        TEST("all 20 sequential keys searchable");
        EXPECT(all_found);
        btree_destroy(tree);
    }

    /* Test 12: insert keys in reverse order */
    {
        btree_t *tree = btree_create();
        for (int i = 15; i >= 1; i--) {
            btree_insert(tree, i, i * 100);
        }
        int all_found = 1;
        for (int i = 1; i <= 15; i++) {
            int v = 0;
            if (btree_search(tree, i, &v) != 0 || v != i * 100) {
                all_found = 0;
            }
        }
        TEST("15 reverse-order inserts all searchable");
        EXPECT(all_found);
        btree_destroy(tree);
    }

    /* Test 13: search for key smaller than all */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 10, 1);
        btree_insert(tree, 20, 2);
        btree_insert(tree, 30, 3);
        int val = 0;
        TEST("search miss: key smaller than all");
        EXPECT(btree_search(tree, 1, &val) == -1);
        btree_destroy(tree);
    }

    /* Test 14: search for key larger than all */
    {
        btree_t *tree = btree_create();
        btree_insert(tree, 10, 1);
        btree_insert(tree, 20, 2);
        btree_insert(tree, 30, 3);
        int val = 0;
        TEST("search miss: key larger than all");
        EXPECT(btree_search(tree, 99, &val) == -1);
        btree_destroy(tree);
    }

    /* ── Summary ─────────────────────────────────────────────────── */
    printf("\n%d/%d tests passed.\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
