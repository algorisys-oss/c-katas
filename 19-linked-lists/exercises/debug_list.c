/*
 * debug_list.c -- Find and Fix the Bugs!
 *
 * Each function below operates on a singly linked list and has exactly
 * ONE bug. Read the comment above each function to understand what it
 * SHOULD do, then find the bug and fix it.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -o debug_list debug_list.c
 * Run:      ./debug_list
 *
 * When all bugs are fixed, every test should print [PASS].
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ---- Test Harness ---- */
#define TEST(name) printf("  %-40s", name)
#define PASS() printf("[PASS]\n")
static int tests_passed = 0, tests_failed = 0;
#define EXPECT(cond) do { \
    if (cond) { PASS(); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); tests_failed++; } \
} while(0)

/* ---- Node definition ---- */
typedef struct node {
    int data;
    struct node *next;
} node_t;

/* Helper: create a new node (no bugs here) */
node_t *node_create(int data) {
    node_t *n = malloc(sizeof(node_t));
    if (n == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    n->data = data;
    n->next = NULL;
    return n;
}

/* Helper: free an entire list (no bugs here) */
void list_free(node_t *head) {
    while (head != NULL) {
        node_t *tmp = head;
        head = head->next;
        free(tmp);
    }
}

/* Helper: count nodes in a list (no bugs here) */
int list_length(node_t *head) {
    int count = 0;
    node_t *cur = head;
    /* Safety: stop after 1000 to avoid infinite loops from buggy code */
    while (cur != NULL && count < 1000) {
        count++;
        cur = cur->next;
    }
    return count;
}

/* Helper: build a list from an array (no bugs here) */
node_t *list_from_array(int *arr, int n) {
    if (n <= 0) {
        return NULL;
    }
    node_t *head = node_create(arr[0]);
    node_t *tail = head;
    for (int i = 1; i < n; i++) {
        tail->next = node_create(arr[i]);
        tail = tail->next;
    }
    return head;
}

/* ======================================================================
 * BUG #1: Memory leak in delete
 *
 * This function deletes the first node that contains `value` from the
 * list. It should free the deleted node and return the (possibly new)
 * head of the list.
 *
 * HINT: When you remove a node, you need to do TWO things:
 *       1. Redirect the previous node's `next` to skip over it
 *       2. Free the removed node's memory
 *       Is this function doing both?
 * ====================================================================== */
node_t *list_delete(node_t *head, int value) {
    if (head == NULL) {
        return NULL;
    }
    /* Special case: deleting the head */
    if (head->data == value) {
        node_t *new_head = head->next;
        /* BUG: forgot to free the old head! */
        return new_head;
    }
    /* Search for the node */
    node_t *prev = head;
    node_t *cur = head->next;
    while (cur != NULL) {
        if (cur->data == value) {
            prev->next = cur->next;
            /* BUG: forgot to free cur! */
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

/* ======================================================================
 * BUG #2: Lost nodes on insert
 *
 * This function inserts a new node with `value` AFTER the node at
 * position `pos` (0-indexed). For example, inserting after position 1
 * in [A, B, C] should give [A, B, NEW, C].
 *
 * HINT: When you insert a new node between two existing nodes, the
 *       ORDER in which you set the pointers matters. If you overwrite
 *       a pointer before saving it, you lose the rest of the list.
 *       Draw it out on paper!
 * ====================================================================== */
node_t *list_insert_after(node_t *head, int pos, int value) {
    node_t *cur = head;
    for (int i = 0; i < pos && cur != NULL; i++) {
        cur = cur->next;
    }
    if (cur == NULL) {
        return head;
    }
    node_t *new_node = node_create(value);
    cur->next = new_node;        /* <-- BUG: overwrites cur->next BEFORE saving it */
    new_node->next = cur->next;  /* now cur->next is already new_node -- cycle! */
    return head;
}

/* ======================================================================
 * BUG #3: Infinite loop when appending
 *
 * This function appends a new node with `value` at the END of the list.
 * But it accidentally creates a cycle, causing any traversal to loop
 * forever.
 *
 * HINT: After finding the last node, what should the new node's `next`
 *       pointer be? And are we setting the last node's `next` to the
 *       right thing?
 * ====================================================================== */
node_t *list_append(node_t *head, int value) {
    node_t *new_node = node_create(value);
    if (head == NULL) {
        return new_node;
    }
    node_t *cur = head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_node;
    new_node->next = cur;    /* <-- BUG: points back to cur, creating a cycle! */
    return head;
}

/* ======================================================================
 * BUG #4: NULL dereference on empty list
 *
 * This function returns the data value of the first node in the list.
 * It returns -1 if the list is empty. But it crashes on an empty list
 * instead of returning -1.
 *
 * HINT: What is `head` when the list is empty? What happens if you
 *       try to access a member of a NULL pointer?
 * ====================================================================== */
int list_first(node_t *head) {
    int val = head->data;    /* <-- BUG: accesses head before checking NULL */
    if (head == NULL) {
        return -1;
    }
    return val;
}

/* ======================================================================
 * BUG #5: Use-after-free
 *
 * This function removes and returns the data from the head of the list.
 * It updates the head pointer (via pointer-to-pointer) so the caller
 * sees the new head.
 *
 * HINT: After you free() a piece of memory, you must NEVER read from
 *       it again. The order of operations matters: save what you need
 *       BEFORE freeing.
 * ====================================================================== */
int list_pop_front(node_t **head_ref) {
    if (*head_ref == NULL) {
        return -1;
    }
    node_t *old_head = *head_ref;
    free(old_head);                  /* <-- BUG: freed too early! */
    *head_ref = old_head->next;      /* reading from freed memory */
    int val = old_head->data;        /* reading from freed memory */
    return val;
}

/* ======================================================================
 * MAIN -- Test Harness
 *
 * These tests will FAIL (or crash) until you fix each bug above.
 * ====================================================================== */
int main(void) {
    printf("\n=== Debug Linked List: Find & Fix the Bugs ===\n\n");

    /* --- Test Bug #1: Memory leak in delete --- */
    /* Note: The memory leak won't cause a test failure by itself,
     * but the function should still work correctly. Run with valgrind
     * to catch the leak:  valgrind ./debug_list                      */
    {
        int arr[] = {10, 20, 30, 40, 50};
        node_t *list = list_from_array(arr, 5);

        list = list_delete(list, 30);
        TEST("Bug #1: delete middle node (length)");
        EXPECT(list_length(list) == 4);

        /* Verify remaining values */
        node_t *cur = list;
        TEST("Bug #1: delete middle node (values)");
        EXPECT(cur->data == 10 && cur->next->data == 20 &&
               cur->next->next->data == 40 &&
               cur->next->next->next->data == 50);

        /* Delete head */
        list = list_delete(list, 10);
        TEST("Bug #1: delete head node");
        EXPECT(list != NULL && list->data == 20 && list_length(list) == 3);

        list_free(list);
    }

    /* --- Test Bug #2: Lost nodes on insert --- */
    {
        int arr[] = {10, 20, 30};
        node_t *list = list_from_array(arr, 3);

        list = list_insert_after(list, 1, 25);
        TEST("Bug #2: insert after pos 1 (length)");
        EXPECT(list_length(list) == 4);

        TEST("Bug #2: insert after pos 1 (order)");
        node_t *cur = list;
        EXPECT(cur->data == 10 && cur->next->data == 20 &&
               cur->next->next->data == 25 &&
               cur->next->next->next->data == 30);

        list_free(list);
    }

    /* --- Test Bug #3: Infinite loop when appending --- */
    {
        node_t *list = NULL;
        list = list_append(list, 10);
        list = list_append(list, 20);
        list = list_append(list, 30);

        TEST("Bug #3: append three nodes (length)");
        EXPECT(list_length(list) == 3);

        TEST("Bug #3: append three nodes (order)");
        EXPECT(list->data == 10 && list->next->data == 20 &&
               list->next->next->data == 30);

        TEST("Bug #3: last node's next is NULL");
        EXPECT(list->next->next->next == NULL);

        list_free(list);
    }

    /* --- Test Bug #4: NULL dereference --- */
    {
        TEST("Bug #4: first of empty list returns -1");
        EXPECT(list_first(NULL) == -1);

        node_t *one = node_create(42);
        TEST("Bug #4: first of non-empty list");
        EXPECT(list_first(one) == 42);
        free(one);
    }

    /* --- Test Bug #5: Use-after-free --- */
    {
        int arr[] = {100, 200, 300};
        node_t *list = list_from_array(arr, 3);

        int val = list_pop_front(&list);
        TEST("Bug #5: pop_front returns head data");
        EXPECT(val == 100);

        TEST("Bug #5: pop_front updates head");
        EXPECT(list != NULL && list->data == 200);

        TEST("Bug #5: list length after pop");
        EXPECT(list_length(list) == 2);

        list_free(list);
    }

    /* --- Summary --- */
    printf("\n  ----------------------------------------\n");
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);

    if (tests_failed == 0) {
        printf("  Run with valgrind to check for memory leaks:\n");
        printf("  valgrind --leak-check=full ./debug_list\n\n");
    }

    return tests_failed > 0 ? 1 : 0;
}
