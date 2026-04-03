/*
 * Kata: Binary Tree Interview Patterns
 *
 * Classic binary tree problems beyond basic traversals.
 * These patterns appear constantly in technical interviews.
 *
 * Tree used in many tests:
 *
 *        1
 *       / \
 *      2   3
 *     / \    \
 *    4   5    6
 *
 * Exercises:
 *   1. tree_height(root)                     — height of the tree
 *   2. tree_diameter(root)                   — longest path between any two nodes
 *   3. lowest_common_ancestor(root, p, q)    — LCA of two nodes
 *   4. right_side_view(root, out, count)     — rightmost node at each level
 *   5. zigzag_level_order(root, ...)         — zigzag BFS traversal
 *   6. serialize_tree(root, buf)             — tree to string
 *   7. deserialize_tree(buf)                 — string to tree
 *
 * 20 tests total.
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

/* ── Exercise 1: tree_height ─────────────────────────────────────────
 *
 * Return the height of the tree.
 *   - NULL tree has height 0
 *   - A single node has height 1
 *   - Otherwise: 1 + max(height(left), height(right))
 *
 * Example:
 *        1          height = 3
 *       / \
 *      2   3        height of left subtree = 2
 *     / \           height of right subtree = 1
 *    4   5
 */
int tree_height(TreeNode *root)
{
    /* TODO: Base case — NULL returns 0 */

    /* TODO: Recursively compute height of left and right subtrees */

    /* TODO: Return 1 + max(left_height, right_height) */

    (void)root;
    return 0;  /* placeholder */
}

/* ── Exercise 2: tree_diameter ───────────────────────────────────────
 *
 * The diameter of a tree is the longest path between any two nodes,
 * measured in number of EDGES.
 *
 * Key insight: the longest path either:
 *   a) passes through the root (left_height + right_height), or
 *   b) is entirely within the left or right subtree
 *
 * Use a helper that computes height but also tracks the maximum
 * diameter seen so far via a pointer.
 *
 *        1
 *       / \
 *      2   3       diameter = 4 (path: 4 -> 2 -> 1 -> 3 -> 6)
 *     / \    \
 *    4   5    6
 *
 * Hint: define a helper function like:
 *   int diameter_helper(TreeNode *root, int *max_diameter)
 * that returns the height and updates *max_diameter.
 */
int tree_diameter(TreeNode *root)
{
    /* TODO: Initialize max_diameter to 0 */

    /* TODO: Call a helper that returns height and tracks max diameter */

    /* TODO: Return max_diameter */

    (void)root;
    return 0;  /* placeholder */
}

/* ── Exercise 3: lowest_common_ancestor ──────────────────────────────
 *
 * Given a binary tree (NOT a BST) and two node values p and q,
 * find their lowest common ancestor.
 *
 * The LCA is the deepest node that has both p and q as descendants
 * (a node can be a descendant of itself).
 *
 * Recursive algorithm:
 *   - If root is NULL, return NULL
 *   - If root->data is p or q, return root
 *   - Search left subtree and right subtree
 *   - If both return non-NULL, root is the LCA
 *   - If only one returns non-NULL, that's the LCA
 *
 * Why it works:
 *   Search left, search right:
 *   - Both found -> current is LCA
 *   - Only left  -> LCA is in left subtree
 *   - Only right -> LCA is in right subtree
 *
 *        1
 *       / \
 *      2   3        LCA(4, 5) = 2
 *     / \    \      LCA(4, 6) = 1
 *    4   5    6     LCA(2, 4) = 2
 */
TreeNode *lowest_common_ancestor(TreeNode *root, int p, int q)
{
    /* TODO: Base case — if root is NULL, return NULL */

    /* TODO: If root->data is p or q, return root */

    /* TODO: Search left and right subtrees recursively */

    /* TODO: If both left and right returned non-NULL, root is LCA */

    /* TODO: Otherwise return whichever is non-NULL */

    (void)root; (void)p; (void)q;
    return NULL;  /* placeholder */
}

/* ── Exercise 4: right_side_view ─────────────────────────────────────
 *
 * Return what you'd see looking at the tree from the right side:
 * the rightmost node at each level.
 *
 * Use BFS (level-order traversal). For each level, the LAST node
 * is visible from the right side.
 *
 *        1
 *       / \
 *      2   3       Right view: [1, 3, 6]
 *       \    \
 *        5    6
 *
 * Parameters:
 *   root  — root of the tree
 *   out   — array to fill with right-side values
 *   count — pointer to int, set to the number of values written
 *
 * Hint: use an array as a queue. Process one level at a time by
 * tracking the level size before dequeuing.
 */
void right_side_view(TreeNode *root, int *out, int *count)
{
    /* TODO: Handle NULL root */

    /* TODO: Create a queue (array of TreeNode pointers) */

    /* TODO: BFS level by level:
     *   - Record the size of the current level
     *   - Dequeue all nodes in this level
     *   - The last node dequeued is the rightmost — add to out
     *   - Enqueue children of each node
     */

    (void)root; (void)out;
    *count = 0;  /* placeholder */
}

/* ── Exercise 5: zigzag_level_order ──────────────────────────────────
 *
 * Level-order traversal but alternating direction:
 *   Level 0: left to right
 *   Level 1: right to left
 *   Level 2: left to right
 *   ...
 *
 *        1
 *       / \
 *      2   3       Zigzag: [1] [3, 2] [4, 5, 6]
 *     / \    \
 *    4   5    6
 *
 * Parameters:
 *   root   — root of the tree
 *   levels — 2D array: levels[i] holds the values at level i
 *   sizes  — array: sizes[i] = number of values at level i
 *   num_levels — pointer to int, set to total number of levels
 *
 * Hint: do a normal BFS but collect each level into a temporary array.
 * If the level index is odd, reverse that array before storing.
 */
#define MAX_NODES 100

void zigzag_level_order(TreeNode *root, int levels[][MAX_NODES],
                        int *sizes, int *num_levels)
{
    /* TODO: Handle NULL root */

    /* TODO: BFS with level tracking */

    /* TODO: For odd levels, reverse the order */

    (void)root; (void)levels; (void)sizes;
    *num_levels = 0;  /* placeholder */
}

/* ── Exercise 6: serialize_tree ──────────────────────────────────────
 *
 * Convert a binary tree to a string using preorder traversal.
 * Use "null" to represent NULL children.
 * Separate values with commas.
 *
 *        1
 *       / \
 *      2   3  =>  "1,2,null,null,3,null,null"
 *
 * Parameters:
 *   root — root of the tree
 *   buf  — character buffer to write the result into
 *
 * Hint: use sprintf to write into buf + strlen(buf).
 *       Append commas between elements.
 */
void serialize_tree(TreeNode *root, char *buf)
{
    /* TODO: If root is NULL, append "null" to buf */

    /* TODO: Append root->data to buf */

    /* TODO: Recursively serialize left and right subtrees */

    (void)root; (void)buf;
}

/* ── Exercise 7: deserialize_tree ────────────────────────────────────
 *
 * Reconstruct a tree from a serialized string.
 * The string is in the same preorder format produced by serialize_tree.
 *
 * Algorithm:
 *   - Split the string by commas (or use strtok)
 *   - Use a recursive helper with an index pointer
 *   - If current token is "null", return NULL and advance index
 *   - Otherwise, create a node, then recursively build left and right
 *
 * Parameters:
 *   buf — the serialized string
 *
 * Returns the root of the reconstructed tree.
 *
 * Hint: use a static/passed index or strtok_r to track position.
 *       Make a copy of buf before tokenizing (strtok modifies the string).
 */
TreeNode *deserialize_tree(const char *buf)
{
    /* TODO: Copy buf into a mutable buffer */

    /* TODO: Tokenize by comma and recursively build the tree */

    (void)buf;
    return NULL;  /* placeholder */
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
        /* Skewed tree: 1 -> 2 -> 3 */
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
        /* Left-skewed tree: right side sees every node */
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
