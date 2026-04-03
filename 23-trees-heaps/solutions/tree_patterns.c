/*
 * Solution: Binary Tree Interview Patterns
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Data Structure ──────────────────────────────────────────────── */

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/* ── Helper: create a new tree node ──────────────────────────────── */

TreeNode *make_node(int data)
{
    TreeNode *n = malloc(sizeof(TreeNode));
    if (!n) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/* ── Helper: free all nodes in a tree (post-order) ───────────────── */

void free_tree(TreeNode *root)
{
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* ── Exercise 1: tree_height ─────────────────────────────────────── */

int tree_height(TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }
    int left_h = tree_height(root->left);
    int right_h = tree_height(root->right);
    return 1 + (left_h > right_h ? left_h : right_h);
}

/* ── Exercise 2: tree_diameter ───────────────────────────────────── */

static int diameter_helper(TreeNode *root, int *max_diameter)
{
    if (root == NULL) {
        return 0;
    }
    int left_h = diameter_helper(root->left, max_diameter);
    int right_h = diameter_helper(root->right, max_diameter);

    /* The diameter through this node is left_h + right_h */
    int through_root = left_h + right_h;
    if (through_root > *max_diameter) {
        *max_diameter = through_root;
    }

    return 1 + (left_h > right_h ? left_h : right_h);
}

int tree_diameter(TreeNode *root)
{
    int max_diameter = 0;
    diameter_helper(root, &max_diameter);
    return max_diameter;
}

/* ── Exercise 3: lowest_common_ancestor ──────────────────────────── */

TreeNode *lowest_common_ancestor(TreeNode *root, int p, int q)
{
    if (root == NULL) {
        return NULL;
    }
    if (root->data == p || root->data == q) {
        return root;
    }

    TreeNode *left = lowest_common_ancestor(root->left, p, q);
    TreeNode *right = lowest_common_ancestor(root->right, p, q);

    /* If both subtrees found a target, this node is the LCA */
    if (left != NULL && right != NULL) {
        return root;
    }

    return (left != NULL) ? left : right;
}

/* ── Exercise 4: right_side_view ─────────────────────────────────── */

#define MAX_QUEUE 256

void right_side_view(TreeNode *root, int *out, int *count)
{
    *count = 0;
    if (root == NULL) return;

    TreeNode *queue[MAX_QUEUE];
    int front = 0, back = 0;
    queue[back++] = root;

    while (front < back) {
        int level_size = back - front;
        for (int i = 0; i < level_size; i++) {
            TreeNode *node = queue[front++];
            /* Last node in this level is the rightmost */
            if (i == level_size - 1) {
                out[(*count)++] = node->data;
            }
            if (node->left) queue[back++] = node->left;
            if (node->right) queue[back++] = node->right;
        }
    }
}

/* ── Exercise 5: zigzag_level_order ──────────────────────────────── */

#define MAX_NODES 100

void zigzag_level_order(TreeNode *root, int levels[][MAX_NODES],
                        int *sizes, int *num_levels)
{
    *num_levels = 0;
    if (root == NULL) return;

    TreeNode *queue[MAX_QUEUE];
    int front = 0, back = 0;
    queue[back++] = root;

    while (front < back) {
        int level_size = back - front;
        int level_idx = *num_levels;
        sizes[level_idx] = level_size;

        for (int i = 0; i < level_size; i++) {
            TreeNode *node = queue[front++];
            levels[level_idx][i] = node->data;
            if (node->left) queue[back++] = node->left;
            if (node->right) queue[back++] = node->right;
        }

        /* Reverse odd levels (right-to-left) */
        if (level_idx % 2 == 1) {
            for (int i = 0; i < level_size / 2; i++) {
                int tmp = levels[level_idx][i];
                levels[level_idx][i] = levels[level_idx][level_size - 1 - i];
                levels[level_idx][level_size - 1 - i] = tmp;
            }
        }

        (*num_levels)++;
    }
}

/* ── Exercise 6: serialize_tree ──────────────────────────────────── */

void serialize_tree(TreeNode *root, char *buf)
{
    if (root == NULL) {
        if (strlen(buf) > 0) {
            strcat(buf, ",");
        }
        strcat(buf, "null");
        return;
    }

    if (strlen(buf) > 0) {
        strcat(buf, ",");
    }
    char num[16];
    sprintf(num, "%d", root->data);
    strcat(buf, num);

    serialize_tree(root->left, buf);
    serialize_tree(root->right, buf);
}

/* ── Exercise 7: deserialize_tree ────────────────────────────────── */

static TreeNode *deserialize_helper(char **tokens, int *idx, int total)
{
    if (*idx >= total) {
        return NULL;
    }

    if (strcmp(tokens[*idx], "null") == 0) {
        (*idx)++;
        return NULL;
    }

    TreeNode *node = make_node(atoi(tokens[*idx]));
    (*idx)++;
    node->left = deserialize_helper(tokens, idx, total);
    node->right = deserialize_helper(tokens, idx, total);
    return node;
}

TreeNode *deserialize_tree(const char *buf)
{
    if (buf == NULL || strlen(buf) == 0) {
        return NULL;
    }

    /* Make a mutable copy for tokenizing */
    char *copy = malloc(strlen(buf) + 1);
    if (!copy) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    strcpy(copy, buf);

    /* Split into tokens */
    char *tokens[MAX_QUEUE];
    int total = 0;
    char *saveptr;
    char *token = strtok_r(copy, ",", &saveptr);
    while (token != NULL) {
        tokens[total++] = token;
        token = strtok_r(NULL, ",", &saveptr);
    }

    int idx = 0;
    TreeNode *root = deserialize_helper(tokens, &idx, total);
    free(copy);
    return root;
}

/* ── Test Harness ────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* Helper: build the standard test tree
 *        1
 *       / \
 *      2   3
 *     / \    \
 *    4   5    6
 */
TreeNode *build_test_tree(void)
{
    TreeNode *root = make_node(1);
    root->left = make_node(2);
    root->right = make_node(3);
    root->left->left = make_node(4);
    root->left->right = make_node(5);
    root->right->right = make_node(6);
    return root;
}

/* Helper: compare trees structurally */
int trees_equal(TreeNode *a, TreeNode *b)
{
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;
    return a->data == b->data &&
           trees_equal(a->left, b->left) &&
           trees_equal(a->right, b->right);
}

int main(void)
{
    printf("=== Binary Tree Interview Patterns ===\n\n");

    /* ── tree_height tests ───────────────────────────────────── */
    printf("tree_height:\n");
    {
        TreeNode *tree = build_test_tree();
        TEST("height of test tree is 3") EXPECT(tree_height(tree) == 3);
        free_tree(tree);
    }
    {
        TEST("height of NULL tree is 0") EXPECT(tree_height(NULL) == 0);
    }
    {
        TreeNode *single = make_node(1);
        TEST("height of single node is 1") EXPECT(tree_height(single) == 1);
        free_tree(single);
    }

    /* ── tree_diameter tests ─────────────────────────────────── */
    printf("\ntree_diameter:\n");
    {
        TreeNode *tree = build_test_tree();
        TEST("diameter of test tree is 4") EXPECT(tree_diameter(tree) == 4);
        free_tree(tree);
    }
    {
        TreeNode *single = make_node(1);
        TEST("diameter of single node is 0") EXPECT(tree_diameter(single) == 0);
        free_tree(single);
    }
    {
        TreeNode *root = make_node(1);
        root->left = make_node(2);
        root->left->left = make_node(3);
        TEST("diameter of skewed tree is 2") EXPECT(tree_diameter(root) == 2);
        free_tree(root);
    }

    /* ── lowest_common_ancestor tests ────────────────────────── */
    printf("\nlowest_common_ancestor:\n");
    {
        TreeNode *tree = build_test_tree();
        TreeNode *result = lowest_common_ancestor(tree, 4, 5);
        TEST("LCA(4, 5) = 2") EXPECT(result != NULL && result->data == 2);
        free_tree(tree);
    }
    {
        TreeNode *tree = build_test_tree();
        TreeNode *result = lowest_common_ancestor(tree, 4, 6);
        TEST("LCA(4, 6) = 1") EXPECT(result != NULL && result->data == 1);
        free_tree(tree);
    }
    {
        TreeNode *tree = build_test_tree();
        TreeNode *result = lowest_common_ancestor(tree, 2, 4);
        TEST("LCA(2, 4) = 2 (ancestor of itself)") EXPECT(result != NULL && result->data == 2);
        free_tree(tree);
    }
    {
        TreeNode *tree = build_test_tree();
        TreeNode *result = lowest_common_ancestor(tree, 5, 6);
        TEST("LCA(5, 6) = 1") EXPECT(result != NULL && result->data == 1);
        free_tree(tree);
    }

    /* ── right_side_view tests ───────────────────────────────── */
    printf("\nright_side_view:\n");
    {
        TreeNode *tree = build_test_tree();
        int out[10];
        int count = 0;
        right_side_view(tree, out, &count);
        TEST("right view: [1, 3, 6]")
            EXPECT(count == 3 && out[0] == 1 && out[1] == 3 && out[2] == 6);
        free_tree(tree);
    }
    {
        int out[10];
        int count = 0;
        right_side_view(NULL, out, &count);
        TEST("right view of NULL tree") EXPECT(count == 0);
    }
    {
        TreeNode *root = make_node(1);
        root->left = make_node(2);
        root->left->left = make_node(3);
        int out[10];
        int count = 0;
        right_side_view(root, out, &count);
        TEST("right view of left-skewed tree")
            EXPECT(count == 3 && out[0] == 1 && out[1] == 2 && out[2] == 3);
        free_tree(root);
    }

    /* ── zigzag_level_order tests ────────────────────────────── */
    printf("\nzigzag_level_order:\n");
    {
        TreeNode *tree = build_test_tree();
        int levels[10][MAX_NODES];
        int sizes[10];
        int num_levels = 0;
        zigzag_level_order(tree, levels, sizes, &num_levels);
        TEST("zigzag level 0: [1]")
            EXPECT(num_levels >= 1 && sizes[0] == 1 && levels[0][0] == 1);
        TEST("zigzag level 1: [3, 2]")
            EXPECT(num_levels >= 2 && sizes[1] == 2 &&
                   levels[1][0] == 3 && levels[1][1] == 2);
        TEST("zigzag level 2: [4, 5, 6]")
            EXPECT(num_levels >= 3 && sizes[2] == 3 &&
                   levels[2][0] == 4 && levels[2][1] == 5 && levels[2][2] == 6);
        free_tree(tree);
    }

    /* ── serialize_tree tests ────────────────────────────────── */
    printf("\nserialize_tree:\n");
    {
        TreeNode *root = make_node(1);
        root->left = make_node(2);
        root->right = make_node(3);
        char buf[256] = "";
        serialize_tree(root, buf);
        TEST("serialize simple tree")
            EXPECT(strcmp(buf, "1,2,null,null,3,null,null") == 0);
        free_tree(root);
    }
    {
        char buf[256] = "";
        serialize_tree(NULL, buf);
        TEST("serialize NULL tree") EXPECT(strcmp(buf, "null") == 0);
    }

    /* ── deserialize_tree tests ──────────────────────────────── */
    printf("\ndeserialize_tree:\n");
    {
        TreeNode *original = make_node(1);
        original->left = make_node(2);
        original->right = make_node(3);

        char buf[256] = "";
        serialize_tree(original, buf);
        TreeNode *rebuilt = deserialize_tree(buf);
        TEST("round-trip: serialize then deserialize")
            EXPECT(trees_equal(original, rebuilt));
        free_tree(original);
        free_tree(rebuilt);
    }
    {
        TreeNode *tree = build_test_tree();
        char buf[256] = "";
        serialize_tree(tree, buf);
        TreeNode *rebuilt = deserialize_tree(buf);
        TEST("round-trip: complex tree")
            EXPECT(trees_equal(tree, rebuilt));
        free_tree(tree);
        free_tree(rebuilt);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
