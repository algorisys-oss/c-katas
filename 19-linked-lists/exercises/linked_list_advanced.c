/*
 * Kata: Advanced Linked List Patterns
 *
 * Classic linked list problems commonly asked in FAANG interviews.
 * These build on the basic linked list operations you already know.
 *
 * Cycle Detection — Floyd's Tortoise and Hare:
 *
 *   1 -> 2 -> 3 -> 4 -> 5
 *                  ^       |
 *                  |       v
 *                  7 <- 6
 *
 *   slow: 1, 2, 3, 4, 5, 6, 3, 4
 *   fast: 1, 3, 5, 6, 4, 6, 4, 6
 *   Meet at node 4!
 *
 * Reverse a List — Three-pointer technique:
 *
 *   Before: 1 -> 2 -> 3 -> 4 -> NULL
 *   After:  4 -> 3 -> 2 -> 1 -> NULL
 *
 * Reverse K-Group:
 *
 *   List: 1->2->3->4->5, k=2  =>  2->1->4->3->5
 *   List: 1->2->3->4->5, k=3  =>  3->2->1->4->5
 *
 * Palindrome Check — find middle, reverse second half, compare:
 *
 *   1->2->3->2->1  =>  true
 *   1->2->3->4->5  =>  false
 *
 * Exercises:
 *   1. detect_cycle(head)            — Floyd's cycle detection
 *   2. find_cycle_start(head)        — find where cycle begins
 *   3. reverse_list(head)            — reverse in-place
 *   4. reverse_k_group(head, k)      — reverse in groups of k
 *   5. merge_two_sorted(a, b)        — merge two sorted lists
 *   6. is_palindrome_list(head)      — check if list is a palindrome
 *
 * 20 tests total.
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

/* ── Exercise 1: detect_cycle ────────────────────────────────────────
 *
 * Determine whether a linked list contains a cycle.
 *
 * Use Floyd's Tortoise and Hare algorithm:
 *   - slow pointer moves 1 step at a time
 *   - fast pointer moves 2 steps at a time
 *   - If fast catches up to slow, there is a cycle
 *   - If fast reaches NULL, there is no cycle
 *
 * Why it works (pigeonhole principle):
 *   Once both pointers are inside the cycle, the distance between them
 *   decreases by 1 each step. They MUST eventually meet — fast laps slow.
 *
 * Return 1 if cycle exists, 0 otherwise.
 */
int detect_cycle(Node *head)
{
    /* TODO: Initialize slow and fast pointers to head */

    /* TODO: While fast and fast->next are not NULL:
     *   - Move slow one step
     *   - Move fast two steps
     *   - If slow == fast, return 1 (cycle found)
     */

    (void)head;
    return 0;  /* placeholder */
}

/* ── Exercise 2: find_cycle_start ────────────────────────────────────
 *
 * If the list has a cycle, return a pointer to the node where it starts.
 * If no cycle, return NULL.
 *
 * Algorithm:
 *   1. Detect cycle using Floyd's (slow/fast meet inside the cycle)
 *   2. Reset one pointer to head
 *   3. Move both pointers one step at a time
 *   4. They meet at the cycle start
 *
 * Why this works — the distance proof:
 *
 *   Head ──a──> Cycle start ──b──> Meeting point
 *                    ^                    |
 *                    └────── c ───────────┘
 *
 *   When slow and fast meet:
 *     slow traveled: a + b
 *     fast traveled: a + b + (b + c) = a + 2b + c
 *     fast = 2 * slow:  a + 2b + c = 2(a + b)
 *     Simplify:  c = a
 *
 *   So the distance from head to cycle start (a) equals
 *   the distance from meeting point to cycle start (c).
 *   Move one pointer from head and one from meeting point,
 *   both at speed 1 — they meet at the cycle start.
 */
Node *find_cycle_start(Node *head)
{
    /* TODO: Run Floyd's algorithm to find meeting point */

    /* TODO: If no cycle, return NULL */

    /* TODO: Reset one pointer to head, keep other at meeting point */

    /* TODO: Move both one step at a time until they meet */

    (void)head;
    return NULL;  /* placeholder */
}

/* ── Exercise 3: reverse_list ────────────────────────────────────────
 *
 * Reverse a singly linked list in-place. Return the new head.
 *
 * Use three pointers: prev, current, next_node.
 *
 *   Step by step for 1 -> 2 -> 3 -> NULL:
 *
 *   prev=NULL, curr=1:
 *     next_node = 2, curr->next = NULL, prev = 1, curr = 2
 *
 *   prev=1, curr=2:
 *     next_node = 3, curr->next = 1, prev = 2, curr = 3
 *
 *   prev=2, curr=3:
 *     next_node = NULL, curr->next = 2, prev = 3, curr = NULL
 *
 *   Return prev (3). List is now: 3 -> 2 -> 1 -> NULL
 */
Node *reverse_list(Node *head)
{
    /* TODO: Initialize prev = NULL, current = head */

    /* TODO: While current is not NULL:
     *   - Save next_node = current->next
     *   - Reverse the link: current->next = prev
     *   - Advance: prev = current, current = next_node
     */

    /* TODO: Return prev (the new head) */

    (void)head;
    return NULL;  /* placeholder */
}

/* ── Exercise 4: reverse_k_group ─────────────────────────────────────
 *
 * Reverse the nodes of the list in groups of k.
 * If the number of remaining nodes is less than k, leave them as-is.
 *
 * Example: 1->2->3->4->5, k=2  =>  2->1->4->3->5
 * Example: 1->2->3->4->5, k=3  =>  3->2->1->4->5
 *
 * Algorithm:
 *   1. Check if there are at least k nodes remaining
 *   2. If yes, reverse the next k nodes
 *   3. Recursively process the rest
 *   4. Connect the reversed group to the result of the recursion
 *
 * Hint: after reversing a group, the original first node of that group
 * is now the LAST node — connect it to the result of the recursive call.
 */
Node *reverse_k_group(Node *head, int k)
{
    /* TODO: Check if there are at least k nodes from head */

    /* TODO: If fewer than k nodes, return head unchanged */

    /* TODO: Reverse exactly k nodes starting from head */

    /* TODO: The original head is now the tail of the reversed group.
     *       Connect it to the result of recursively processing the rest. */

    /* TODO: Return the new head of this reversed group */

    (void)head; (void)k;
    return NULL;  /* placeholder */
}

/* ── Exercise 5: merge_two_sorted ────────────────────────────────────
 *
 * Merge two sorted linked lists into one sorted list.
 * Do NOT allocate new nodes — rewire the existing ones.
 *
 * Use the dummy head technique:
 *   - Create a dummy node on the stack (not malloc'd)
 *   - Use a tail pointer that always points to the last node of the
 *     merged list
 *   - Compare heads of both lists, append the smaller one to tail
 *   - When one list is exhausted, append the remainder of the other
 *   - Return dummy.next (the real head)
 *
 * Time: O(n + m)   Space: O(1)
 */
Node *merge_two_sorted(Node *a, Node *b)
{
    /* TODO: Create a dummy node on the stack and a tail pointer */

    /* TODO: While both lists have nodes:
     *   - Compare a->data and b->data
     *   - Append the smaller to tail->next, advance that list
     *   - Advance tail
     */

    /* TODO: Append whichever list still has nodes */

    /* TODO: Return dummy.next */

    (void)a; (void)b;
    return NULL;  /* placeholder */
}

/* ── Exercise 6: is_palindrome_list ──────────────────────────────────
 *
 * Check if a linked list reads the same forwards and backwards.
 *
 * Algorithm:
 *   1. Find the middle using slow/fast pointers
 *   2. Reverse the second half
 *   3. Compare first half with reversed second half
 *   4. Restore the list (reverse the second half back) — good practice
 *   5. Return 1 if palindrome, 0 otherwise
 *
 * Example:
 *   1 -> 2 -> 3 -> 2 -> 1
 *   Middle: 3
 *   Reversed second half: 1 -> 2
 *   Compare with first half: 1 -> 2
 *   Match! Return 1.
 *
 * Hint: for odd-length lists, the middle element doesn't need comparing.
 *       slow ends up at the middle node. Reverse from slow->next.
 */
int is_palindrome_list(Node *head)
{
    /* TODO: Handle empty list and single node (both are palindromes) */

    /* TODO: Find middle with slow/fast pointers */

    /* TODO: Reverse the second half (from slow->next onward) */

    /* TODO: Compare first half and reversed second half node by node */

    /* TODO: Restore the list by reversing the second half back */

    /* TODO: Return result */

    (void)head;
    return 0;  /* placeholder */
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
    return curr == NULL;  /* no extra nodes */
}

int main(void)
{
    printf("=== Advanced Linked List Patterns ===\n\n");

    /* ── detect_cycle tests ──────────────────────────────────── */
    printf("detect_cycle:\n");
    {
        /* No cycle: 1 -> 2 -> 3 -> NULL */
        Node *list = build_list((int[]){1, 2, 3}, 3);
        TEST("no cycle returns 0") EXPECT(detect_cycle(list) == 0);
        free_list(list);
    }
    {
        /* Cycle at head: 1 -> 2 -> 3 -> back to 1 */
        Node *n1 = make_node(1);
        Node *n2 = make_node(2);
        Node *n3 = make_node(3);
        n1->next = n2; n2->next = n3; n3->next = n1;
        TEST("cycle at head returns 1") EXPECT(detect_cycle(n1) == 1);
        /* Break cycle before freeing */
        n3->next = NULL;
        free_list(n1);
    }
    {
        /* Cycle in middle: 1 -> 2 -> 3 -> 4 -> 5 -> back to 3 */
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
        /* Single node, no cycle */
        Node *single = make_node(42);
        TEST("single node, no cycle") EXPECT(detect_cycle(single) == 0);
        free_list(single);
    }

    /* ── find_cycle_start tests ──────────────────────────────── */
    printf("\nfind_cycle_start:\n");
    {
        /* No cycle */
        Node *list = build_list((int[]){1, 2, 3}, 3);
        TEST("no cycle returns NULL") EXPECT(find_cycle_start(list) == NULL);
        free_list(list);
    }
    {
        /* Cycle starts at node 3: 1 -> 2 -> 3 -> 4 -> 5 -> back to 3 */
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
        /* Cycle starts at head: 1 -> 2 -> 3 -> back to 1 */
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
