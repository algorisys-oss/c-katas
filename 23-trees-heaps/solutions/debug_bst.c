/*
 * Debug Kata: Binary Search Tree — SOLUTIONS
 *
 * All 5 bugs fixed. Each fix is marked with "FIX N:" comments.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic debug_bst.c -o debug_bst
 * Run:      ./debug_bst
 * Verify:   valgrind ./debug_bst
 */

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

/* ── Node Definition ─────────────────────────────────────────────── */

typedef struct BSTNode {
    int data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/* ── Create Node ─────────────────────────────────────────────────── */

static BSTNode *bst_create(int value)
{
    BSTNode *node = malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* ── Insert ──────────────────────────────────────────────────────── */

/*
 * FIX 1: Return the newly created node when tree is empty.
 *
 * Bug was:   bst_create(value);
 *            return root;           // root is still NULL!
 * Fix:       return bst_create(value);
 *
 * The caller needs the new node's address to use as the tree root.
 * The buggy code created the node but threw away the pointer and
 * returned the original NULL, so the tree was always empty.
 */
static BSTNode *bst_insert(BSTNode *root, int value)
{
    if (root == NULL) {
        return bst_create(value);
    }

    if (value < root->data) {
        root->left = bst_insert(root->left, value);
    } else if (value > root->data) {
        root->right = bst_insert(root->right, value);
    }

    return root;
}

/* ── Search ──────────────────────────────────────────────────────── */

/*
 * FIX 2: Use ">" not ">=" for the right subtree comparison.
 *
 * Bug was:   else if (value >= root->data)   // goes right even when equal!
 * Fix:       else if (value > root->data)
 *
 * When value == root->data, we've FOUND the node — we should return it.
 * With >=, the function incorrectly searches the right subtree for an
 * equal value, missing the current node entirely. The "return root"
 * at the bottom was unreachable because < and >= cover all cases.
 */
static BSTNode *bst_search(BSTNode *root, int value)
{
    if (root == NULL) {
        return NULL;
    }
    if (value < root->data) {
        return bst_search(root->left, value);
    } else if (value > root->data) {
        return bst_search(root->right, value);
    }
    return root;  /* found! */
}

/* ── In-Order Traversal ──────────────────────────────────────────── */

/*
 * FIX 3: Visit left subtree BEFORE storing the current node.
 *
 * Bug was:   result[*index] = root->data;  // store current FIRST
 *            (*index)++;
 *            bst_inorder(root->left, ...);  // then left
 *            bst_inorder(root->right, ...); // then right
 *
 * Fix:       bst_inorder(root->left, ...);  // left FIRST
 *            result[*index] = root->data;   // then current
 *            (*index)++;
 *            bst_inorder(root->right, ...); // then right
 *
 * In-order means: Left, Node, Right. The buggy version did
 * Node, Left, Right — which is pre-order traversal.
 */
static void bst_inorder(BSTNode *root, int *result, int *index)
{
    if (root == NULL) return;

    bst_inorder(root->left, result, index);

    result[*index] = root->data;
    (*index)++;

    bst_inorder(root->right, result, index);
}

/* ── Find Minimum ────────────────────────────────────────────────── */

static BSTNode *bst_find_min(BSTNode *root)
{
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

/* ── Delete ──────────────────────────────────────────────────────── */

/*
 * FIX 4: Delete successor from the RIGHT subtree, not the left.
 *
 * Bug was:   root->left = bst_delete(root->left, successor->data);
 * Fix:       root->right = bst_delete(root->right, successor->data);
 *
 * The in-order successor comes from the RIGHT subtree (it's the
 * smallest value there). After copying its value into the current
 * node, we must delete it from root->right. Deleting from root->left
 * either does nothing (value not found) or corrupts the tree.
 *
 * FIX 5: Free the node being removed before returning the replacement.
 *
 * Bug was:   return NULL;  or  return temp;  (without freeing root)
 * Fix:       free(root); return NULL;  or  free(root); return temp;
 *
 * Every node was allocated with malloc() in bst_create(). When we
 * remove a node from the tree, we must free() its memory. Without
 * this, every delete leaks sizeof(BSTNode) bytes.
 */
static BSTNode *bst_delete(BSTNode *root, int value)
{
    if (root == NULL) return NULL;

    if (value < root->data) {
        root->left = bst_delete(root->left, value);
    } else if (value > root->data) {
        root->right = bst_delete(root->right, value);
    } else {
        /* Found the node to delete */

        /* Case 1: No children (leaf) */
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        /* Case 2: One child */
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        /* Case 3: Two children */
        BSTNode *successor = bst_find_min(root->right);
        root->data = successor->data;
        root->right = bst_delete(root->right, successor->data);
    }
    return root;
}

/* ── Destroy ─────────────────────────────────────────────────────── */

static void bst_destroy(BSTNode *root)
{
    if (root == NULL) return;
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
}

/* ── Count nodes ─────────────────────────────────────────────────── */

static int bst_count(BSTNode *root)
{
    if (root == NULL) return 0;
    return 1 + bst_count(root->left) + bst_count(root->right);
}

/* ── Tests ───────────────────────────────────────────────────────── */

static void test_insert_empty(void)
{
    TEST("insert into empty tree returns root");
    BSTNode *root = NULL;
    root = bst_insert(root, 42);
    EXPECT(root != NULL && root->data == 42);
    bst_destroy(root);
}

static void test_search_finds(void)
{
    TEST("search finds value in right subtree");
    BSTNode *root = NULL;
    root = bst_insert(root, 20);
    root = bst_insert(root, 10);
    root = bst_insert(root, 30);

    BSTNode *found = bst_search(root, 30);
    EXPECT(found != NULL && found->data == 30);
    bst_destroy(root);
}

static void test_inorder_sorted(void)
{
    TEST("in-order traversal is sorted");
    BSTNode *root = NULL;
    root = bst_insert(root, 50);
    root = bst_insert(root, 30);
    root = bst_insert(root, 70);
    root = bst_insert(root, 20);
    root = bst_insert(root, 40);

    int result[5];
    int index = 0;
    bst_inorder(root, result, &index);

    int sorted = (index == 5 &&
                  result[0] == 20 && result[1] == 30 &&
                  result[2] == 40 && result[3] == 50 &&
                  result[4] == 70);
    EXPECT(sorted);
    bst_destroy(root);
}

static void test_delete_two_children(void)
{
    TEST("delete node with two children");
    BSTNode *root = NULL;
    root = bst_insert(root, 50);
    root = bst_insert(root, 30);
    root = bst_insert(root, 70);
    root = bst_insert(root, 20);
    root = bst_insert(root, 40);

    root = bst_delete(root, 30);

    int ok = 1;
    if (bst_search(root, 30) != NULL) ok = 0;
    if (bst_search(root, 20) == NULL) ok = 0;
    if (bst_search(root, 40) == NULL) ok = 0;
    if (bst_search(root, 50) == NULL) ok = 0;
    if (bst_search(root, 70) == NULL) ok = 0;
    if (bst_count(root) != 4) ok = 0;

    EXPECT(ok);
    bst_destroy(root);
}

static void test_delete_frees_memory(void)
{
    TEST("delete frees node memory (check valgrind)");
    BSTNode *root = NULL;
    root = bst_insert(root, 10);
    root = bst_insert(root, 5);
    root = bst_insert(root, 15);

    root = bst_delete(root, 5);
    root = bst_delete(root, 15);
    root = bst_delete(root, 10);

    EXPECT(root == NULL);
}

/* ── Main ────────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Binary Search Tree (SOLUTIONS) ===\n\n");

    test_insert_empty();
    test_search_finds();
    test_inorder_sorted();
    test_delete_two_children();
    test_delete_frees_memory();

    printf("\n  Results: %d passed, %d failed\n", tests_passed, tests_failed);
    if (tests_failed == 0) {
        printf("  All bugs fixed! Run with valgrind to confirm no leaks.\n");
    }
    printf("\n");

    return tests_failed > 0 ? 1 : 0;
}
