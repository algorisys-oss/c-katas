/*
 * Kata: Linked List Intersection (The Y-Shape Problem)
 *
 * Two singly linked lists can MERGE into one — they share a common tail.
 * This creates a Y shape in memory:
 *
 *   List A:  1 -> 2 -> 3 \
 *                          -> 6 -> 7 -> NULL
 *   List B:       4 -> 5 /
 *
 * Node 6 is the INTERSECTION — it is literally the same node in memory,
 * not just a node with the same value. Both lists' pointers converge here.
 *
 * Your job: find that intersection node, or return NULL if the lists
 * don't intersect.
 *
 * Key insight: if list A has length 5 and list B has length 4, then A has
 * 1 extra node before the shared part. Advance A by 1, then walk both
 * together — they will meet at the intersection.
 *
 * Why pointer comparison, not value comparison?
 *   Two nodes can have the same value but be different allocations.
 *   Intersection means the SAME NODE in memory (same address).
 *
 * Exercises:
 *   1. get_length(head)              — count nodes in a list
 *   2. find_intersection(headA, headB) — find the shared node
 *
 * 8 tests total.
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Data Structure ──────────────────────────────────────────────── */

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* ── Helper: create a new node ───────────────────────────────────── */

Node *node_create(int data)
{
    Node *n = malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    n->data = data;
    n->next = NULL;
    return n;
}

/* ── Exercise 1: get_length ──────────────────────────────────────────
 *
 * Walk the list from head to NULL, counting nodes.
 * Return the count.
 *
 * Example:  1 -> 2 -> 3 -> NULL  => length 3
 *           NULL                  => length 0
 */
int get_length(Node *head)
{
    /* TODO: Walk the list, count nodes, return count */

    (void)head;
    return 0;  /* placeholder */
}

/* ── Exercise 2: find_intersection ───────────────────────────────────
 *
 * Find the first node that both lists share (same pointer, not same value).
 * Return a pointer to that node, or NULL if the lists don't intersect.
 *
 * Algorithm:
 *   1. Get the length of both lists
 *   2. Compute the difference: diff = |len_a - len_b|
 *   3. Advance the pointer of the LONGER list by diff steps
 *      (now both pointers are the same distance from the end)
 *   4. Walk both pointers forward together, one step at a time
 *   5. When they point to the SAME NODE (same address), that's the intersection
 *   6. If you reach NULL, the lists don't intersect
 *
 * Why this works:
 *   After advancing the longer list, both pointers have the same number
 *   of nodes remaining. If the lists share a tail, the pointers will
 *   converge at the first shared node.
 *
 *   List A:  1 -> 2 -> 3 -> 6 -> 7 -> NULL   (length 5)
 *   List B:       4 -> 5 -> 6 -> 7 -> NULL   (length 4)
 *   diff = 1, so advance A by 1:
 *
 *        pA = 2              pB = 4
 *        pA = 3              pB = 5
 *        pA = 6              pB = 6   <-- SAME NODE! Return it.
 *
 * Time: O(n + m)   Space: O(1)
 */
Node *find_intersection(Node *head_a, Node *head_b)
{
    /* TODO: Get lengths of both lists */

    /* TODO: Compute the difference */

    /* TODO: Advance the longer list's pointer by the difference */

    /* TODO: Walk both pointers together until they match or reach NULL */

    (void)head_a; (void)head_b;
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

/* Helper: free only the nodes that are NOT shared with another list.
 * We free nodes from head up to (but not including) shared_start. */
void free_unique_prefix(Node *head, Node *shared_start)
{
    Node *curr = head;
    while (curr != NULL && curr != shared_start) {
        Node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}

/* Helper: free a simple chain of nodes */
void free_chain(Node *head)
{
    while (head != NULL) {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main(void)
{
    printf("=== Linked List Intersection ===\n\n");

    /* ── get_length tests ─────────────────────────────────────── */
    printf("get_length:\n");
    {
        Node *a = node_create(1);
        a->next = node_create(2);
        a->next->next = node_create(3);

        TEST("length of 3-node list is 3") EXPECT(get_length(a) == 3);
        TEST("length of NULL list is 0") EXPECT(get_length(NULL) == 0);

        free_chain(a);
    }

    /* ── find_intersection tests ──────────────────────────────── */
    printf("\nfind_intersection:\n");

    /* Test 1: Classic Y-shape intersection
     *
     *   A: 1 -> 2 -> 3 \
     *                     -> 6 -> 7 -> NULL
     *   B:      4 -> 5 /
     */
    {
        /* Build the shared tail: 6 -> 7 -> NULL */
        Node *shared = node_create(6);
        shared->next = node_create(7);

        /* Build list A: 1 -> 2 -> 3 -> shared */
        Node *a = node_create(1);
        a->next = node_create(2);
        a->next->next = node_create(3);
        a->next->next->next = shared;

        /* Build list B: 4 -> 5 -> shared */
        Node *b = node_create(4);
        b->next = node_create(5);
        b->next->next = shared;

        Node *result = find_intersection(a, b);
        TEST("Y-shape: finds intersection node") EXPECT(result == shared);
        TEST("Y-shape: intersection data is 6") EXPECT(result != NULL && result->data == 6);

        /* Free only unique prefixes — shared tail freed once */
        free_unique_prefix(a, shared);
        free_unique_prefix(b, shared);
        free_chain(shared);
    }

    /* Test 2: No intersection — completely separate lists */
    {
        Node *a = node_create(1);
        a->next = node_create(2);

        Node *b = node_create(3);
        b->next = node_create(4);

        TEST("no intersection returns NULL") EXPECT(find_intersection(a, b) == NULL);

        free_chain(a);
        free_chain(b);
    }

    /* Test 3: Intersection at the very first node of the shorter list */
    {
        /* Shared: 10 -> 20 -> NULL */
        Node *shared = node_create(10);
        shared->next = node_create(20);

        /* A: 5 -> shared */
        Node *a = node_create(5);
        a->next = shared;

        /* B IS the shared list (starts at node 10) */
        Node *b = shared;

        TEST("intersection at head of B") EXPECT(find_intersection(a, b) == shared);

        free_unique_prefix(a, shared);
        free_chain(shared);
    }

    /* Test 4: Both lists are the same single node */
    {
        Node *shared = node_create(42);

        TEST("single shared node") EXPECT(find_intersection(shared, shared) == shared);

        free_chain(shared);
    }

    /* Test 5: One or both lists are NULL */
    {
        Node *a = node_create(1);
        TEST("NULL list returns NULL") EXPECT(find_intersection(a, NULL) == NULL);
        free_chain(a);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
