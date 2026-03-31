/*
 * debug_list.c -- SOLUTIONS (all bugs fixed)
 *
 * Each fix is marked with a "FIX" comment explaining the change.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -o debug_list debug_list.c
 * Run:      ./debug_list
 * Memcheck: valgrind --leak-check=full ./debug_list
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
 * BUG #1 FIX: Added free() calls for deleted nodes
 *
 * The original code unlinked nodes from the list but never freed them.
 * Every malloc() must have a matching free(), or you leak memory.
 * ====================================================================== */
node_t *list_delete(node_t *head, int value) {
    if (head == NULL) {
        return NULL;
    }
    if (head->data == value) {
        node_t *new_head = head->next;
        free(head);              /* FIX: free the old head */
        return new_head;
    }
    node_t *prev = head;
    node_t *cur = head->next;
    while (cur != NULL) {
        if (cur->data == value) {
            prev->next = cur->next;
            free(cur);           /* FIX: free the deleted node */
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

/* ======================================================================
 * BUG #2 FIX: Save cur->next BEFORE overwriting it
 *
 * The original code set cur->next = new_node first, losing the pointer
 * to the rest of the list. Then new_node->next = cur->next just pointed
 * back to new_node itself (since cur->next was already new_node).
 *
 * The fix: set new_node->next FIRST, then link cur to new_node.
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
    new_node->next = cur->next;  /* FIX: save the rest of the list FIRST */
    cur->next = new_node;        /* FIX: then link cur to the new node */
    return head;
}

/* ======================================================================
 * BUG #3 FIX: Set new_node->next to NULL, not back to cur
 *
 * The original code set new_node->next = cur, creating a cycle
 * (cur -> new_node -> cur -> new_node -> ...).
 * The last node in a list should have next == NULL.
 * node_create already sets next to NULL, so we just remove the bad line.
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
    /* FIX: removed "new_node->next = cur" -- new_node->next is already NULL */
    return head;
}

/* ======================================================================
 * BUG #4 FIX: Check for NULL BEFORE accessing head->data
 *
 * The original code accessed head->data first, then checked if head
 * was NULL. If head is NULL, accessing ->data is undefined behavior
 * (usually a crash / segfault).
 * ====================================================================== */
int list_first(node_t *head) {
    if (head == NULL) {          /* FIX: check NULL first */
        return -1;
    }
    return head->data;
}

/* ======================================================================
 * BUG #5 FIX: Save data and next pointer BEFORE calling free()
 *
 * The original code called free(old_head) and then read old_head->next
 * and old_head->data. After free(), that memory is invalid -- reading
 * from it is undefined behavior (use-after-free).
 *
 * Fix: read everything you need first, then free.
 * ====================================================================== */
int list_pop_front(node_t **head_ref) {
    if (*head_ref == NULL) {
        return -1;
    }
    node_t *old_head = *head_ref;
    int val = old_head->data;        /* FIX: save data BEFORE free */
    *head_ref = old_head->next;      /* FIX: save next BEFORE free */
    free(old_head);                  /* FIX: free LAST */
    return val;
}

/* ======================================================================
 * MAIN -- Test Harness
 * ====================================================================== */
int main(void) {
    printf("\n=== Debug Linked List: Find & Fix the Bugs (SOLUTIONS) ===\n\n");

    /* --- Test Bug #1: Memory leak in delete --- */
    {
        int arr[] = {10, 20, 30, 40, 50};
        node_t *list = list_from_array(arr, 5);

        list = list_delete(list, 30);
        TEST("Bug #1: delete middle node (length)");
        EXPECT(list_length(list) == 4);

        node_t *cur = list;
        TEST("Bug #1: delete middle node (values)");
        EXPECT(cur->data == 10 && cur->next->data == 20 &&
               cur->next->next->data == 40 &&
               cur->next->next->next->data == 50);

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
        printf("  All bugs fixed! Memory check:\n");
        printf("  valgrind --leak-check=full ./debug_list\n\n");
    }

    return tests_failed > 0 ? 1 : 0;
}
