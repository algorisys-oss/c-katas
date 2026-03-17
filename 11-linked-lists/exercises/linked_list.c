/*
 * Kata: Singly Linked List
 *
 * Build a singly linked list library from scratch:
 *   - list_create:     allocate and initialize an empty list
 *   - list_destroy:    free all nodes and the list itself
 *   - list_push_front: insert at head — O(1)
 *   - list_push_back:  insert at tail — O(n)
 *   - list_pop_front:  remove and return head value
 *   - list_find:       search for a value, return the node or NULL
 *   - list_delete:     delete first node with matching value
 *   - list_size:       return number of elements
 *   - list_reverse:    reverse the list in place
 *   - list_print:      print all elements
 *
 * 18 tests total.
 *
 * Hints:
 *   - Each node is malloc'd separately
 *   - list_destroy must walk the list and free every node
 *   - list_delete has a special case: deleting the head node
 *   - list_reverse uses three pointers: prev, curr, next_node
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Data Structures ─────────────────────────────────────────────── */

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int size;
} LinkedList;

/* ── Exercise 1: list_create ─────────────────────────────────────────
 * Allocate a new LinkedList on the heap.
 * Initialize head to NULL and size to 0.
 * Return a pointer to the list (or NULL if malloc fails).
 */
LinkedList *list_create(void)
{
    /* TODO: malloc a LinkedList, initialize fields, return it */
    return NULL; /* placeholder */
}

/* ── Exercise 2: list_destroy ────────────────────────────────────────
 * Free every node in the list, then free the list struct itself.
 * Must handle an empty list (head == NULL) gracefully.
 *
 * Hint: save curr->next BEFORE freeing curr.
 */
void list_destroy(LinkedList *list)
{
    /* TODO: walk the list, free each node, then free the list */
    (void)list;
}

/* ── Exercise 3: list_push_front ─────────────────────────────────────
 * Create a new node with the given data.
 * Insert it at the head of the list.
 * Update the size.
 */
void list_push_front(LinkedList *list, int data)
{
    /* TODO: malloc a node, wire it up, update head and size */
    (void)list; (void)data;
}

/* ── Exercise 4: list_push_back ──────────────────────────────────────
 * Create a new node with the given data.
 * Walk to the end of the list and append it.
 * Handle the empty list case (head == NULL).
 * Update the size.
 */
void list_push_back(LinkedList *list, int data)
{
    /* TODO: malloc a node, find the tail, append, update size */
    (void)list; (void)data;
}

/* ── Exercise 5: list_pop_front ──────────────────────────────────────
 * Remove the head node and return its data value.
 * If the list is empty, return -1.
 * Update the size.
 *
 * Hint: save the data and next pointer before freeing the node.
 */
int list_pop_front(LinkedList *list)
{
    /* TODO: save head data, advance head, free old head, return data */
    (void)list;
    return -1; /* placeholder */
}

/* ── Exercise 6: list_find ───────────────────────────────────────────
 * Search for the first node whose data matches the given value.
 * Return a pointer to the node, or NULL if not found.
 */
Node *list_find(LinkedList *list, int value)
{
    /* TODO: walk the list, return node if data matches */
    (void)list; (void)value;
    return NULL; /* placeholder */
}

/* ── Exercise 7: list_delete ─────────────────────────────────────────
 * Delete the first node whose data matches the given value.
 * Return 0 on success, -1 if the value was not found.
 *
 * Special case: if the matching node is the head, update head.
 * Otherwise, track the previous node and rewire prev->next.
 */
int list_delete(LinkedList *list, int value)
{
    /* TODO: find node, rewire pointers, free it, update size */
    (void)list; (void)value;
    return -1; /* placeholder */
}

/* ── Exercise 8: list_size ───────────────────────────────────────────
 * Return the number of elements in the list.
 */
int list_size(LinkedList *list)
{
    /* TODO: return list->size */
    (void)list;
    return 0; /* placeholder */
}

/* ── Exercise 9: list_reverse ────────────────────────────────────────
 * Reverse the list in place.
 * Use three pointers: prev, curr, next_node.
 * After reversal, head should point to what was the last node.
 */
void list_reverse(LinkedList *list)
{
    /* TODO: walk the list, reverse each next pointer, update head */
    (void)list;
}

/* ── Exercise 10: list_print ─────────────────────────────────────────
 * Print all elements in the format: "10 -> 20 -> 30 -> NULL\n"
 * If the list is empty, print "NULL\n"
 */
void list_print(LinkedList *list)
{
    /* TODO: walk the list, print each element */
    (void)list;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

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
    printf("=== Singly Linked List ===\n\n");

    /* ── list_create ─────────────────────────────────────────── */
    printf("list_create:\n");
    LinkedList *list = list_create();
    TEST("create returns non-NULL") EXPECT(list != NULL);
    TEST("new list size is 0") EXPECT(list_size(list) == 0);

    /* ── list_push_front ─────────────────────────────────────── */
    printf("\nlist_push_front:\n");
    list_push_front(list, 10);
    TEST("push_front 10, size is 1") EXPECT(list_size(list) == 1);
    list_push_front(list, 20);
    TEST("push_front 20, size is 2") EXPECT(list_size(list) == 2);
    TEST("head is 20 (most recent push_front)") EXPECT(list->head != NULL && list->head->data == 20);

    /* ── list_push_back ──────────────────────────────────────── */
    printf("\nlist_push_back:\n");
    list_push_back(list, 30);
    TEST("push_back 30, size is 3") EXPECT(list_size(list) == 3);
    {
        /* Walk to the tail to verify 30 is last */
        Node *tail = list->head;
        while (tail->next != NULL) tail = tail->next;
        TEST("tail is 30") EXPECT(tail->data == 30);
    }

    /* ── list_find ───────────────────────────────────────────── */
    printf("\nlist_find:\n");
    TEST("find 20 returns non-NULL") EXPECT(list_find(list, 20) != NULL);
    TEST("find 20 returns correct node") EXPECT(list_find(list, 20)->data == 20);
    TEST("find 99 returns NULL") EXPECT(list_find(list, 99) == NULL);

    /* ── list_pop_front ──────────────────────────────────────── */
    printf("\nlist_pop_front:\n");
    {
        int val = list_pop_front(list);
        TEST("pop_front returns 20") EXPECT(val == 20);
        TEST("after pop, size is 2") EXPECT(list_size(list) == 2);
        TEST("new head is 10") EXPECT(list->head != NULL && list->head->data == 10);
    }

    /* ── list_delete ─────────────────────────────────────────── */
    printf("\nlist_delete:\n");
    /* List is now: 10 -> 30 -> NULL */
    list_push_back(list, 40);  /* 10 -> 30 -> 40 */
    {
        int rc = list_delete(list, 30);
        TEST("delete 30 returns 0 (success)") EXPECT(rc == 0);
        TEST("after delete 30, size is 2") EXPECT(list_size(list) == 2);
        TEST("30 is no longer in list") EXPECT(list_find(list, 30) == NULL);
    }

    /* ── list_reverse ────────────────────────────────────────── */
    printf("\nlist_reverse:\n");
    /* List is now: 10 -> 40 -> NULL */
    list_reverse(list);
    TEST("after reverse, head is 40") EXPECT(list->head != NULL && list->head->data == 40);
    TEST("after reverse, tail is 10") EXPECT(list->head->next != NULL && list->head->next->data == 10);

    /* ── list_print ──────────────────────────────────────────── */
    printf("\nlist_print (visual check):\n  ");
    list_print(list);

    /* ── Cleanup ─────────────────────────────────────────────── */
    list_destroy(list);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
