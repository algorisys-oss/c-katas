/*
 * Solution: Advanced Linked List Patterns
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Data Structure ──────────────────────────────────────────────── */

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* ── Helper: create a new node ───────────────────────────────────── */

Node *make_node(int data)
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

/* ── Helper: free a list (no cycles!) ────────────────────────────── */

void free_list(Node *head)
{
    while (head != NULL) {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

/* ── Exercise 1: detect_cycle ────────────────────────────────────── */

int detect_cycle(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return 1;
        }
    }

    return 0;
}

/* ── Exercise 2: find_cycle_start ────────────────────────────────── */

Node *find_cycle_start(Node *head)
{
    Node *slow = head;
    Node *fast = head;
    int has_cycle = 0;

    /* Phase 1: detect cycle and find meeting point */
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            has_cycle = 1;
            break;
        }
    }

    if (!has_cycle) {
        return NULL;
    }

    /* Phase 2: find cycle start — reset one pointer to head */
    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

/* ── Exercise 3: reverse_list ────────────────────────────────────── */

Node *reverse_list(Node *head)
{
    Node *prev = NULL;
    Node *curr = head;

    while (curr != NULL) {
        Node *next_node = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next_node;
    }

    return prev;
}

/* ── Exercise 4: reverse_k_group ─────────────────────────────────── */

Node *reverse_k_group(Node *head, int k)
{
    /* Check if there are at least k nodes */
    Node *check = head;
    for (int i = 0; i < k; i++) {
        if (check == NULL) {
            return head;  /* fewer than k nodes, leave as-is */
        }
        check = check->next;
    }

    /* Reverse k nodes */
    Node *prev = NULL;
    Node *curr = head;
    for (int i = 0; i < k; i++) {
        Node *next_node = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next_node;
    }

    /* head is now the tail of the reversed group.
     * Connect it to the result of reversing the rest. */
    head->next = reverse_k_group(curr, k);

    /* prev is the new head of this reversed group */
    return prev;
}

/* ── Exercise 5: merge_two_sorted ────────────────────────────────── */

Node *merge_two_sorted(Node *a, Node *b)
{
    Node dummy;
    dummy.next = NULL;
    Node *tail = &dummy;

    while (a != NULL && b != NULL) {
        if (a->data <= b->data) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    /* Append remaining nodes */
    tail->next = (a != NULL) ? a : b;

    return dummy.next;
}

/* ── Exercise 6: is_palindrome_list ──────────────────────────────── */

int is_palindrome_list(Node *head)
{
    if (head == NULL || head->next == NULL) {
        return 1;
    }

    /* Find middle with slow/fast */
    Node *slow = head;
    Node *fast = head;
    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    /* Reverse second half (starting after slow) */
    Node *second_half = reverse_list(slow->next);
    Node *second_head = second_half;  /* save for restoring later */

    /* Compare first half with reversed second half */
    Node *first = head;
    int result = 1;
    while (second_half != NULL) {
        if (first->data != second_half->data) {
            result = 0;
            break;
        }
        first = first->next;
        second_half = second_half->next;
    }

    /* Restore the list */
    slow->next = reverse_list(second_head);

    return result;
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

/* Helper: build a list from an array */
Node *build_list(int *arr, int n)
{
    if (n == 0) return NULL;
    Node *head = make_node(arr[0]);
    Node *curr = head;
    for (int i = 1; i < n; i++) {
        curr->next = make_node(arr[i]);
        curr = curr->next;
    }
    return head;
}

/* Helper: check if list matches an array */
int list_matches(Node *head, int *arr, int n)
{
    Node *curr = head;
    for (int i = 0; i < n; i++) {
        if (curr == NULL || curr->data != arr[i]) return 0;
        curr = curr->next;
    }
    return curr == NULL;
}

int main(void)
{
    printf("=== Advanced Linked List Patterns ===\n\n");

    /* ── detect_cycle tests ──────────────────────────────────── */
    printf("detect_cycle:\n");
    {
        Node *list = build_list((int[]){1, 2, 3}, 3);
        TEST("no cycle returns 0") EXPECT(detect_cycle(list) == 0);
        free_list(list);
    }
    {
        Node *n1 = make_node(1);
        Node *n2 = make_node(2);
        Node *n3 = make_node(3);
        n1->next = n2; n2->next = n3; n3->next = n1;
        TEST("cycle at head returns 1") EXPECT(detect_cycle(n1) == 1);
        n3->next = NULL;
        free_list(n1);
    }
    {
        Node *n1 = make_node(1);
        Node *n2 = make_node(2);
        Node *n3 = make_node(3);
        Node *n4 = make_node(4);
        Node *n5 = make_node(5);
        n1->next = n2; n2->next = n3; n3->next = n4;
        n4->next = n5; n5->next = n3;
        TEST("cycle in middle returns 1") EXPECT(detect_cycle(n1) == 1);
        n5->next = NULL;
        free_list(n1);
    }
    {
        Node *single = make_node(42);
        TEST("single node, no cycle") EXPECT(detect_cycle(single) == 0);
        free_list(single);
    }

    /* ── find_cycle_start tests ──────────────────────────────── */
    printf("\nfind_cycle_start:\n");
    {
        Node *list = build_list((int[]){1, 2, 3}, 3);
        TEST("no cycle returns NULL") EXPECT(find_cycle_start(list) == NULL);
        free_list(list);
    }
    {
        Node *n1 = make_node(1);
        Node *n2 = make_node(2);
        Node *n3 = make_node(3);
        Node *n4 = make_node(4);
        Node *n5 = make_node(5);
        n1->next = n2; n2->next = n3; n3->next = n4;
        n4->next = n5; n5->next = n3;
        TEST("cycle start at node 3") EXPECT(find_cycle_start(n1) == n3);
        n5->next = NULL;
        free_list(n1);
    }
    {
        Node *n1 = make_node(1);
        Node *n2 = make_node(2);
        Node *n3 = make_node(3);
        n1->next = n2; n2->next = n3; n3->next = n1;
        TEST("cycle start at head") EXPECT(find_cycle_start(n1) == n1);
        n3->next = NULL;
        free_list(n1);
    }

    /* ── reverse_list tests ──────────────────────────────────── */
    printf("\nreverse_list:\n");
    {
        Node *list = build_list((int[]){1, 2, 3, 4}, 4);
        list = reverse_list(list);
        TEST("reverse 1->2->3->4") EXPECT(list_matches(list, (int[]){4, 3, 2, 1}, 4));
        free_list(list);
    }
    {
        Node *single = make_node(42);
        single = reverse_list(single);
        TEST("reverse single node") EXPECT(single != NULL && single->data == 42 && single->next == NULL);
        free_list(single);
    }
    {
        TEST("reverse NULL list") EXPECT(reverse_list(NULL) == NULL);
    }

    /* ── reverse_k_group tests ───────────────────────────────── */
    printf("\nreverse_k_group:\n");
    {
        Node *list = build_list((int[]){1, 2, 3, 4, 5}, 5);
        list = reverse_k_group(list, 2);
        TEST("k=2: 1->2->3->4->5 => 2->1->4->3->5")
            EXPECT(list_matches(list, (int[]){2, 1, 4, 3, 5}, 5));
        free_list(list);
    }
    {
        Node *list = build_list((int[]){1, 2, 3, 4, 5}, 5);
        list = reverse_k_group(list, 3);
        TEST("k=3: 1->2->3->4->5 => 3->2->1->4->5")
            EXPECT(list_matches(list, (int[]){3, 2, 1, 4, 5}, 5));
        free_list(list);
    }
    {
        Node *list = build_list((int[]){1, 2, 3, 4}, 4);
        list = reverse_k_group(list, 4);
        TEST("k=4: exact fit") EXPECT(list_matches(list, (int[]){4, 3, 2, 1}, 4));
        free_list(list);
    }
    {
        Node *list = build_list((int[]){1, 2, 3}, 3);
        list = reverse_k_group(list, 5);
        TEST("k > length: no change") EXPECT(list_matches(list, (int[]){1, 2, 3}, 3));
        free_list(list);
    }

    /* ── merge_two_sorted tests ──────────────────────────────── */
    printf("\nmerge_two_sorted:\n");
    {
        Node *a = build_list((int[]){1, 3, 5}, 3);
        Node *b = build_list((int[]){2, 4, 6}, 3);
        Node *merged = merge_two_sorted(a, b);
        TEST("merge [1,3,5] and [2,4,6]")
            EXPECT(list_matches(merged, (int[]){1, 2, 3, 4, 5, 6}, 6));
        free_list(merged);
    }
    {
        Node *a = build_list((int[]){1, 2, 3}, 3);
        Node *merged = merge_two_sorted(a, NULL);
        TEST("merge with NULL list") EXPECT(list_matches(merged, (int[]){1, 2, 3}, 3));
        free_list(merged);
    }
    {
        Node *a = build_list((int[]){1, 1, 1}, 3);
        Node *b = build_list((int[]){1, 1}, 2);
        Node *merged = merge_two_sorted(a, b);
        TEST("merge with duplicates")
            EXPECT(list_matches(merged, (int[]){1, 1, 1, 1, 1}, 5));
        free_list(merged);
    }

    /* ── is_palindrome_list tests ────────────────────────────── */
    printf("\nis_palindrome_list:\n");
    {
        Node *list = build_list((int[]){1, 2, 3, 2, 1}, 5);
        TEST("1->2->3->2->1 is palindrome") EXPECT(is_palindrome_list(list) == 1);
        free_list(list);
    }
    {
        Node *list = build_list((int[]){1, 2, 3, 4, 5}, 5);
        TEST("1->2->3->4->5 is not palindrome") EXPECT(is_palindrome_list(list) == 0);
        free_list(list);
    }
    {
        Node *list = build_list((int[]){1, 2, 2, 1}, 4);
        TEST("1->2->2->1 even-length palindrome") EXPECT(is_palindrome_list(list) == 1);
        free_list(list);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
