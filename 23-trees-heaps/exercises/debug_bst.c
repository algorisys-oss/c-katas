/*
 * Debug Kata: Binary Search Tree
 *
 * This file contains a BST implementation with 5 bugs.
 * Each buggy function is marked with a BUG comment and a HINT.
 * Your job: find and fix each bug so all tests pass.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic debug_bst.c -o debug_bst
 * Run:      ./debug_bst
 * Verify:   valgrind ./debug_bst
 *
 * 5 bugs, 5 tests. Good luck!
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

/* ── Create Node (correct) ───────────────────────────────────────── */

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
 * BUG 1: Insert doesn't return the new root
 *
 * This function should insert a value into the BST and return the
 * root of the (possibly modified) tree. When the tree is empty
 * (root == NULL), the new node IS the new root — we must return it.
 *
 * HINT: Look at what happens when root is NULL. The new node is
 *       created, but is it returned to the caller?
 */
static BSTNode *bst_insert(BSTNode *root, int value)
{
    if (root == NULL) {
        bst_create(value);
        return root;  /* root is still NULL here! */
    }

    if (value < root->data) {
        root->left = bst_insert(root->left, value);
    } else if (value > root->data) {
        root->right = bst_insert(root->right, value);
    }
    /* value == root->data: no duplicates, do nothing */

    return root;
}

/* ── Search ──────────────────────────────────────────────────────── */

/*
 * BUG 2: Search uses wrong comparison
 *
 * In a BST, values LESS than the node go left, values GREATER go
 * right. This function has the comparison slightly wrong, causing
 * it to miss values that should be found on the right side.
 *
 * HINT: Think carefully about what ">=" means vs ">".
 *       If value equals root->data, where should we go?
 *       We should STOP and return the node — not keep searching!
 */
static BSTNode *bst_search(BSTNode *root, int value)
{
    if (root == NULL) {
        return NULL;
    }
    if (value < root->data) {
        return bst_search(root->left, value);
    } else if (value >= root->data) {
        return bst_search(root->right, value);
    }
    return root;  /* found! (but this line is unreachable with the bug) */
}

/* ── In-Order Traversal ──────────────────────────────────────────── */

/*
 * BUG 3: Visits nodes in wrong order
 *
 * In-order traversal should visit: left subtree, THEN current node,
 * THEN right subtree. This produces sorted output for a BST.
 *
 * But this code visits the current node BEFORE the left subtree,
 * which is actually pre-order traversal — the output won't be sorted.
 *
 * HINT: The three operations (visit left, store current, visit right)
 *       are in the wrong order. What should come first?
 */
static void bst_inorder(BSTNode *root, int *result, int *index)
{
    if (root == NULL) return;

    /* BUG: stores current node BEFORE visiting left subtree */
    result[*index] = root->data;
    (*index)++;

    bst_inorder(root->left, result, index);
    bst_inorder(root->right, result, index);
}

/* ── Find Minimum (helper for delete) ────────────────────────────── */

static BSTNode *bst_find_min(BSTNode *root)
{
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

/* ── Delete ──────────────────────────────────────────────────────── */

/*
 * BUG 4: Delete doesn't handle two-children case correctly
 *
 * When deleting a node with TWO children:
 * 1. Find the in-order successor (smallest in right subtree)
 * 2. Copy successor's value into the current node
 * 3. Delete the successor from the right subtree
 *
 * This code finds the successor and copies its value, but then
 * it doesn't properly delete the successor from the right subtree.
 *
 * HINT: After copying the successor's value, you need to recursively
 *       delete the successor from root->right. Look at what subtree
 *       the recursive call operates on.
 */

/*
 * BUG 5: Memory leak in delete
 *
 * When deleting a node with zero or one child, the node is removed
 * from the tree — but its memory is never freed.
 *
 * HINT: Before returning the replacement node, you need to free()
 *       the node you're removing. It was allocated with malloc()
 *       in bst_create(), so it must be freed when no longer needed.
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
            /* BUG 5: should free(root) here! */
            return NULL;
        }
        /* Case 2: One child */
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            /* BUG 5: should free(root) here! */
            return temp;
        }
        if (root->right == NULL) {
            BSTNode *temp = root->left;
            /* BUG 5: should free(root) here! */
            return temp;
        }
        /* Case 3: Two children */
        BSTNode *successor = bst_find_min(root->right);
        root->data = successor->data;
        /* BUG 4: deletes from left subtree instead of right */
        root->left = bst_delete(root->left, successor->data);
    }
    return root;
}

/* ── Destroy (correct — frees entire tree) ───────────────────────── */

static void bst_destroy(BSTNode *root)
{
    if (root == NULL) return;
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
}

/* ── Count nodes (correct helper for tests) ──────────────────────── */

static int bst_count(BSTNode *root)
{
    if (root == NULL) return 0;
    return 1 + bst_count(root->left) + bst_count(root->right);
}

/* ── Tests ───────────────────────────────────────────────────────── */

/*
 * Test 1: Insert into empty tree returns new root
 *
 * If insert is correct, inserting into NULL should return a valid
 * node with the inserted value — not NULL.
 */
static void test_insert_empty(void)
{
    TEST("insert into empty tree returns root");
    BSTNode *root = NULL;
    root = bst_insert(root, 42);
    EXPECT(root != NULL && root->data == 42);
    bst_destroy(root);
}

/*
 * Test 2: Search finds existing values
 *
 * Build a tree:       20
 *                   /    \
 *                 10      30
 *
 * Search for 30 should find it (it's in the right subtree).
 */
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

/*
 * Test 3: In-order traversal produces sorted output
 *
 * Insert: 50, 30, 70, 20, 40
 *
 * In-order should produce: [20, 30, 40, 50, 70]
 */
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

/*
 * Test 4: Delete node with two children
 *
 * Tree:       50                 50
 *           /    \     =>      /    \
 *         30      70         40      70
 *        /  \               /
 *      20    40           20
 *
 * Delete 30 (two children). The in-order successor (40) should
 * replace it, and the tree should still be valid.
 */
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

    /* 30 should be gone, others remain */
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

/*
 * Test 5: Delete frees memory (check with valgrind)
 *
 * Insert and delete a leaf node. If delete doesn't free(), valgrind
 * will report "definitely lost" bytes.
 *
 * We can also check that repeated insert/delete doesn't crash.
 */
static void test_delete_frees_memory(void)
{
    TEST("delete frees node memory (check valgrind)");
    BSTNode *root = NULL;
    root = bst_insert(root, 10);
    root = bst_insert(root, 5);
    root = bst_insert(root, 15);

    /* Delete leaf nodes */
    root = bst_delete(root, 5);
    root = bst_delete(root, 15);
    /* Delete root (now a leaf) */
    root = bst_delete(root, 10);

    /* Tree should be empty */
    EXPECT(root == NULL);
    /* If delete didn't free, valgrind will catch it */
}

/* ── Main ────────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Binary Search Tree (5 bugs to find) ===\n\n");

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
