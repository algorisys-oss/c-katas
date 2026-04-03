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

/* ── Exercise 1: get_length ──────────────────────────────────────── */

int get_length(Node *head)
{
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

/* ── Exercise 2: find_intersection ───────────────────────────────── */

Node *find_intersection(Node *head_a, Node *head_b)
{
    int len_a = get_length(head_a);
    int len_b = get_length(head_b);

    /* Advance the longer list so both start the same distance from end */
    while (len_a > len_b) {
        head_a = head_a->next;
        len_a--;
    }
    while (len_b > len_a) {
        head_b = head_b->next;
        len_b--;
    }

    /* Walk together until pointers match */
    while (head_a != NULL && head_b != NULL) {
        if (head_a == head_b) {
            return head_a;
        }
        head_a = head_a->next;
        head_b = head_b->next;
    }

    return NULL;
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

void free_unique_prefix(Node *head, Node *shared_start)
{
    Node *curr = head;
    while (curr != NULL && curr != shared_start) {
        Node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}

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

    /* Test 1: Classic Y-shape */
    {
        Node *shared = node_create(6);
        shared->next = node_create(7);

        Node *a = node_create(1);
        a->next = node_create(2);
        a->next->next = node_create(3);
        a->next->next->next = shared;

        Node *b = node_create(4);
        b->next = node_create(5);
        b->next->next = shared;

        Node *result = find_intersection(a, b);
        TEST("Y-shape: finds intersection node") EXPECT(result == shared);
        TEST("Y-shape: intersection data is 6") EXPECT(result != NULL && result->data == 6);

        free_unique_prefix(a, shared);
        free_unique_prefix(b, shared);
        free_chain(shared);
    }

    /* Test 2: No intersection */
    {
        Node *a = node_create(1);
        a->next = node_create(2);

        Node *b = node_create(3);
        b->next = node_create(4);

        TEST("no intersection returns NULL") EXPECT(find_intersection(a, b) == NULL);

        free_chain(a);
        free_chain(b);
    }

    /* Test 3: Intersection at head of shorter list */
    {
        Node *shared = node_create(10);
        shared->next = node_create(20);

        Node *a = node_create(5);
        a->next = shared;

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
