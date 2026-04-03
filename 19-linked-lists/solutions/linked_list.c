/*
 * Solution: Singly Linked List
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

/* ── list_create ─────────────────────────────────────────────────── */

LinkedList *list_create(void)
{
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

/* ── list_destroy ────────────────────────────────────────────────── */

void list_destroy(LinkedList *list)
{
    if (list == NULL) return;
    Node *curr = list->head;
    while (curr != NULL) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

/* ── list_push_front ─────────────────────────────────────────────── */

void list_push_front(LinkedList *list, int data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return;
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

/* ── list_push_back ──────────────────────────────────────────────── */

void list_push_back(LinkedList *list, int data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return;
    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *curr = list->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
    list->size++;
}

/* ── list_pop_front ──────────────────────────────────────────────── */

int list_pop_front(LinkedList *list)
{
    if (list->head == NULL) {
        return -1;
    }
    Node *old_head = list->head;
    int data = old_head->data;
    list->head = old_head->next;
    free(old_head);
    list->size--;
    return data;
}

/* ── list_find ───────────────────────────────────────────────────── */

Node *list_find(LinkedList *list, int value)
{
    Node *curr = list->head;
    while (curr != NULL) {
        if (curr->data == value) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/* ── list_delete ─────────────────────────────────────────────────── */

int list_delete(LinkedList *list, int value)
{
    if (list->head == NULL) {
        return -1;
    }

    /* Special case: deleting the head */
    if (list->head->data == value) {
        Node *old_head = list->head;
        list->head = old_head->next;
        free(old_head);
        list->size--;
        return 0;
    }

    /* General case: find the node, track previous */
    Node *prev = list->head;
    Node *curr = list->head->next;
    while (curr != NULL) {
        if (curr->data == value) {
            prev->next = curr->next;
            free(curr);
            list->size--;
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}

/* ── list_size ───────────────────────────────────────────────────── */

int list_size(LinkedList *list)
{
    return list->size;
}

/* ── list_reverse ────────────────────────────────────────────────── */

void list_reverse(LinkedList *list)
{
    Node *prev = NULL;
    Node *curr = list->head;
    while (curr != NULL) {
        Node *next_node = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next_node;
    }
    list->head = prev;
}

/* ── list_print ──────────────────────────────────────────────────── */

void list_print(LinkedList *list)
{
    Node *curr = list->head;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
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
