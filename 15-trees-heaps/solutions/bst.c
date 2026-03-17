/*
 * Solution: Binary Search Tree
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

/* ── bst_create_node ──────────────────────────────────────────────── */

BSTNode *bst_create_node(int value)
{
    BSTNode *node = malloc(sizeof(BSTNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* ── bst_insert ───────────────────────────────────────────────────── */

BSTNode *bst_insert(BSTNode *root, int value)
{
    if (root == NULL) {
        return bst_create_node(value);
    }
    if (value < root->data) {
        root->left = bst_insert(root->left, value);
    } else if (value > root->data) {
        root->right = bst_insert(root->right, value);
    }
    /* value == root->data: duplicate, do nothing */
    return root;
}

/* ── bst_search ───────────────────────────────────────────────────── */

BSTNode *bst_search(BSTNode *root, int value)
{
    if (root == NULL) {
        return NULL;
    }
    if (value == root->data) {
        return root;
    } else if (value < root->data) {
        return bst_search(root->left, value);
    } else {
        return bst_search(root->right, value);
    }
}

/* ── bst_delete ───────────────────────────────────────────────────── */

BSTNode *bst_delete(BSTNode *root, int value)
{
    if (root == NULL) {
        return NULL;
    }

    if (value < root->data) {
        root->left = bst_delete(root->left, value);
    } else if (value > root->data) {
        root->right = bst_delete(root->right, value);
    } else {
        /* Found the node to delete */

        /* Case 1: Leaf node */
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        /* Case 2: One child */
        if (root->left == NULL) {
            BSTNode *right_child = root->right;
            free(root);
            return right_child;
        }
        if (root->right == NULL) {
            BSTNode *left_child = root->left;
            free(root);
            return left_child;
        }

        /* Case 3: Two children — find in-order successor */
        BSTNode *successor = root->right;
        while (successor->left != NULL) {
            successor = successor->left;
        }
        root->data = successor->data;
        root->right = bst_delete(root->right, successor->data);
    }

    return root;
}

/* ── bst_inorder ──────────────────────────────────────────────────── */

void bst_inorder(BSTNode *root, int *out, int *index)
{
    if (root == NULL) {
        return;
    }
    bst_inorder(root->left, out, index);
    out[*index] = root->data;
    (*index)++;
    bst_inorder(root->right, out, index);
}

/* ── bst_height ───────────────────────────────────────────────────── */

int bst_height(BSTNode *root)
{
    if (root == NULL) {
        return -1;
    }
    int left_h = bst_height(root->left);
    int right_h = bst_height(root->right);
    return (left_h > right_h ? left_h : right_h) + 1;
}

/* ── bst_size ─────────────────────────────────────────────────────── */

int bst_size(BSTNode *root)
{
    if (root == NULL) {
        return 0;
    }
    return 1 + bst_size(root->left) + bst_size(root->right);
}

/* ── bst_destroy ──────────────────────────────────────────────────── */

void bst_destroy(BSTNode *root)
{
    if (root == NULL) {
        return;
    }
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
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
