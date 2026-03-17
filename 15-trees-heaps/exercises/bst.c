/*
 * Kata: Binary Search Tree
 *
 * Implement a BST with these operations:
 *   - Create a node, insert, search, delete
 *   - In-order traversal (produces sorted output)
 *   - Height and size calculations
 *   - Proper memory cleanup (post-order free)
 *
 * 16 tests total.
 *
 * Hints:
 *   - Insert/search/delete are naturally recursive
 *   - Delete with two children: use the in-order successor
 *     (smallest value in the right subtree)
 *   - bst_destroy should free children before parent (post-order)
 *   - bst_inorder fills an array — pass an index pointer to track position
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ── Node definition ──────────────────────────────────────────────── */

typedef struct BSTNode {
    int data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/* ── Exercise 1: bst_create_node ──────────────────────────────────
 * Allocate a new BSTNode with the given value.
 * Initialize left and right to NULL.
 * Return the pointer to the new node (or NULL if malloc fails).
 */
BSTNode *bst_create_node(int value)
{
    /* TODO: malloc a BSTNode, set data, left, right */
    return NULL; /* placeholder */
}

/* ── Exercise 2: bst_insert ───────────────────────────────────────
 * Insert a value into the BST rooted at `root`.
 * Return the (possibly new) root of the subtree.
 *
 * If root is NULL, create and return a new node.
 * If value < root->data, insert into left subtree.
 * If value > root->data, insert into right subtree.
 * If value == root->data, do nothing (no duplicates).
 */
BSTNode *bst_insert(BSTNode *root, int value)
{
    /* TODO: Recursive insert */
    return NULL; /* placeholder */
}

/* ── Exercise 3: bst_search ───────────────────────────────────────
 * Search for a value in the BST.
 * Return a pointer to the node if found, or NULL if not found.
 */
BSTNode *bst_search(BSTNode *root, int value)
{
    /* TODO: Recursive search — go left if less, right if greater */
    return NULL; /* placeholder */
}

/* ── Exercise 4: bst_delete ───────────────────────────────────────
 * Delete a value from the BST rooted at `root`.
 * Return the (possibly new) root of the subtree.
 *
 * Three cases:
 *   1. Leaf node: free it, return NULL.
 *   2. One child: free the node, return the child.
 *   3. Two children: find the in-order successor (smallest in right
 *      subtree), copy its value to this node, then delete the
 *      successor from the right subtree.
 */
BSTNode *bst_delete(BSTNode *root, int value)
{
    /* TODO: Recursive delete with all three cases */
    return NULL; /* placeholder */
}

/* ── Exercise 5: bst_inorder ──────────────────────────────────────
 * Fill `out` array with values from an in-order traversal.
 * `index` is a pointer to the current position in the array.
 *
 * Usage:
 *   int arr[100];
 *   int idx = 0;
 *   bst_inorder(root, arr, &idx);
 *   // arr[0..idx-1] now contains sorted values
 */
void bst_inorder(BSTNode *root, int *out, int *index)
{
    /* TODO: Recurse left, store root->data at out[*index]++, recurse right */
}

/* ── Exercise 6: bst_height ───────────────────────────────────────
 * Return the height of the tree.
 * An empty tree (NULL) has height -1.
 * A single node has height 0.
 */
int bst_height(BSTNode *root)
{
    /* TODO: max(height(left), height(right)) + 1, base case: -1 for NULL */
    return 0; /* placeholder */
}

/* ── Exercise 7: bst_size ─────────────────────────────────────────
 * Return the number of nodes in the tree.
 */
int bst_size(BSTNode *root)
{
    /* TODO: 1 + size(left) + size(right), base case: 0 for NULL */
    return 0; /* placeholder */
}

/* ── Exercise 8: bst_destroy ──────────────────────────────────────
 * Free all nodes in the tree using post-order traversal.
 * (Free children before freeing the parent.)
 */
void bst_destroy(BSTNode *root)
{
    /* TODO: Post-order traversal: destroy left, destroy right, free root */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Binary Search Tree ===\n\n");

    /* --- Insert & Search tests --- */
    printf("insert & search:\n");
    BSTNode *tree = NULL;
    tree = bst_insert(tree, 50);
    tree = bst_insert(tree, 30);
    tree = bst_insert(tree, 70);
    tree = bst_insert(tree, 20);
    tree = bst_insert(tree, 40);
    tree = bst_insert(tree, 60);
    tree = bst_insert(tree, 80);

    /*
     *        [50]
     *       /    \
     *    [30]    [70]
     *    /  \    /  \
     * [20][40][60][80]
     */

    TEST("search for root (50)")
        EXPECT(bst_search(tree, 50) != NULL && bst_search(tree, 50)->data == 50);

    TEST("search for leaf (20)")
        EXPECT(bst_search(tree, 20) != NULL && bst_search(tree, 20)->data == 20);

    TEST("search for internal node (70)")
        EXPECT(bst_search(tree, 70) != NULL && bst_search(tree, 70)->data == 70);

    TEST("search for non-existent (99)")
        EXPECT(bst_search(tree, 99) == NULL);

    TEST("search for non-existent (5)")
        EXPECT(bst_search(tree, 5) == NULL);

    /* --- Size & Height tests --- */
    printf("\nsize & height:\n");

    TEST("size of 7-node tree")
        EXPECT(bst_size(tree) == 7);

    TEST("height of balanced 7-node tree")
        EXPECT(bst_height(tree) == 2);

    /* --- In-order traversal test --- */
    printf("\ninorder traversal:\n");
    {
        int sorted[10];
        int idx = 0;
        bst_inorder(tree, sorted, &idx);
        TEST("inorder produces sorted output")
            EXPECT(idx == 7 &&
                   sorted[0] == 20 && sorted[1] == 30 &&
                   sorted[2] == 40 && sorted[3] == 50 &&
                   sorted[4] == 60 && sorted[5] == 70 &&
                   sorted[6] == 80);
    }

    /* --- Duplicate insert test --- */
    printf("\nduplicate insert:\n");
    tree = bst_insert(tree, 50);
    TEST("duplicate insert does not change size")
        EXPECT(bst_size(tree) == 7);

    /* --- Delete tests --- */
    printf("\ndelete:\n");

    /* Delete a leaf */
    tree = bst_delete(tree, 20);
    TEST("delete leaf (20): search returns NULL")
        EXPECT(bst_search(tree, 20) == NULL);
    TEST("delete leaf: size decreases to 6")
        EXPECT(bst_size(tree) == 6);

    /* Delete node with one child */
    tree = bst_delete(tree, 30);
    TEST("delete one-child node (30): search returns NULL")
        EXPECT(bst_search(tree, 30) == NULL);
    TEST("delete one-child: child (40) still exists")
        EXPECT(bst_search(tree, 40) != NULL);
    TEST("delete one-child: size decreases to 5")
        EXPECT(bst_size(tree) == 5);

    /* Delete node with two children */
    tree = bst_delete(tree, 70);
    TEST("delete two-children node (70): search returns NULL")
        EXPECT(bst_search(tree, 70) == NULL);
    TEST("delete two-children: both children (60, 80) still reachable")
        EXPECT(bst_search(tree, 60) != NULL && bst_search(tree, 80) != NULL);

    /* --- Cleanup --- */
    bst_destroy(tree);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
