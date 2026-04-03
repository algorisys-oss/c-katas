/*
 * Kata: Data Structure Design Problems
 *
 * "Design a data structure that supports X in O(1) time" — this is one
 * of the most common interview question patterns. The trick is always
 * the same: combine simpler data structures to get better complexity.
 *
 * Exercises:
 *   - Min Stack           (parallel stack tracking minimums)
 *   - Queue Using Stacks  (two-stack reversal trick)
 *   - Circular Deque      (modular arithmetic on a circular array)
 *   - Nested List Iterator (stack-based flattening)
 *
 * 20 tests total.
 *
 * ASCII art — Min Stack (parallel tracking):
 *
 *   Operation    Main stack       Min stack        getMin
 *   ─────────    ──────────       ─────────        ──────
 *   push(5)      [5]              [5]              5
 *   push(3)      [5, 3]           [5, 3]           3
 *   push(7)      [5, 3, 7]        [5, 3, 3]        3
 *   push(2)      [5, 3, 7, 2]     [5, 3, 3, 2]     2
 *   pop()        [5, 3, 7]        [5, 3, 3]        3
 *   pop()        [5, 3]           [5, 3]           3
 *   pop()        [5]              [5]              5
 *
 *   Key insight: min_stack[i] = min(main_stack[0..i])
 *   When we pop, we pop from both — the min automatically "un-updates."
 *
 * ASCII art — Queue from Two Stacks:
 *
 *   Enqueue 1, 2, 3:
 *     input:  [1, 2, 3]    output: []
 *
 *   Dequeue (output empty → pour input into output):
 *     input:  []           output: [3, 2, 1]
 *     Pop from output → returns 1 (FIFO!)
 *
 *   The pour reverses the order — LIFO + LIFO = FIFO!
 *
 *     input:          output:
 *     ┌───┐           ┌───┐
 *     │ 3 │ ──pour──→ │ 1 │ ← top (oldest element)
 *     │ 2 │           │ 2 │
 *     │ 1 │           │ 3 │
 *     └───┘           └───┘
 *
 *   Amortized O(1): each element is moved at most twice
 *   (once into input, once into output).
 *
 * ASCII art — Circular Deque:
 *
 *   capacity=5, data = [_, _, _, _, _]
 *
 *   insertRear(1): front=0, rear=1 → [1, _, _, _, _]
 *   insertRear(2): front=0, rear=2 → [1, 2, _, _, _]
 *   insertFront(0): front=4, rear=2 → [1, 2, _, _, 0]
 *
 *       indices:  0   1   2   3   4
 *       data:   [ 1 | 2 | _ | _ | 0 ]
 *                              ↑       ↑
 *                            rear    front
 *
 *   The array wraps around! front moves backward (mod capacity),
 *   rear moves forward (mod capacity).
 *
 *   front = (front - 1 + capacity) % capacity   (insertFront)
 *   rear  = (rear + 1) % capacity               (insertRear)
 */

#include <stdio.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════
 *  Exercise 1: Min Stack
 * ═══════════════════════════════════════════════════════════════════
 * A stack that supports push, pop, top, and getMin — all in O(1).
 *
 * The trick: maintain a parallel "min stack" alongside the main stack.
 * Every time you push, also push the current minimum onto min_stack.
 * Every time you pop, pop from both.
 */

#define STACK_CAP 1024

typedef struct {
    int data[STACK_CAP];
    int min_data[STACK_CAP];  /* Parallel stack tracking minimum at each level */
    int top;                  /* -1 = empty, points to top element */
} MinStack;

/* Initialize the min stack (set top to -1). */
void min_stack_init(MinStack *s)
{
    /* TODO: Initialize the stack */
    (void)s;
}

/* Push val onto the stack. Also push the new minimum onto min_data.
 * The new minimum is min(val, current minimum). */
void min_stack_push(MinStack *s, int val)
{
    /* TODO: Push val to data[]. Push min(val, current min) to min_data[]. */
    (void)s; (void)val;
}

/* Pop and return the top element. Also pop from min_data. */
int min_stack_pop(MinStack *s)
{
    /* TODO: Pop from both data[] and min_data[] */
    (void)s;
    return 0;
}

/* Return the top element without removing it. */
int min_stack_top(const MinStack *s)
{
    /* TODO */
    (void)s;
    return 0;
}

/* Return the minimum element in O(1). */
int min_stack_get_min(const MinStack *s)
{
    /* TODO: Return min_data[top] — the current minimum */
    (void)s;
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════
 *  Exercise 2: Queue Using Two Stacks
 * ═══════════════════════════════════════════════════════════════════
 * Implement a FIFO queue using only two LIFO stacks.
 *
 * One stack for input (enqueue goes here).
 * One stack for output (dequeue comes from here).
 * When output is empty and we need to dequeue, pour all elements
 * from input to output — this reverses them into FIFO order.
 */

typedef struct {
    int input[STACK_CAP];
    int input_top;
    int output[STACK_CAP];
    int output_top;
} StackQueue;

/* Initialize both stacks to empty. */
void sq_init(StackQueue *q)
{
    /* TODO */
    (void)q;
}

/* Enqueue: push onto the input stack. */
void sq_enqueue(StackQueue *q, int val)
{
    /* TODO: Push val onto input stack */
    (void)q; (void)val;
}

/* Dequeue: pop from output stack. If output is empty, pour input
 * into output first. Return the dequeued value. */
int sq_dequeue(StackQueue *q)
{
    /* TODO: If output empty, pour input→output. Then pop output. */
    (void)q;
    return 0;
}

/* Return 1 if the queue is empty (both stacks empty). */
int sq_is_empty(const StackQueue *q)
{
    /* TODO */
    (void)q;
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════
 *  Exercise 3: Circular Deque (Double-Ended Queue)
 * ═══════════════════════════════════════════════════════════════════
 * Add and remove from both front and back using a circular array.
 *
 * The key: modular arithmetic handles the wrap-around.
 *   front = (front - 1 + capacity) % capacity   (move front backward)
 *   rear  = (rear + 1) % capacity               (move rear forward)
 */

typedef struct {
    int data[STACK_CAP];
    int front;
    int rear;
    int size;
    int capacity;
} CircularDeque;

/* Initialize with the given capacity. */
void deque_init(CircularDeque *dq, int capacity)
{
    /* TODO: Set front=0, rear=0, size=0, capacity */
    (void)dq; (void)capacity;
}

/* Insert at front. Return 1 on success, 0 if full. */
int deque_insert_front(CircularDeque *dq, int val)
{
    /* TODO: Move front backward (with wrap), store val, increment size */
    (void)dq; (void)val;
    return 0;
}

/* Insert at rear. Return 1 on success, 0 if full. */
int deque_insert_rear(CircularDeque *dq, int val)
{
    /* TODO: Store val at rear, move rear forward (with wrap), increment size */
    (void)dq; (void)val;
    return 0;
}

/* Delete from front. Return 1 on success, 0 if empty. */
int deque_delete_front(CircularDeque *dq)
{
    /* TODO: Move front forward (with wrap), decrement size */
    (void)dq;
    return 0;
}

/* Delete from rear. Return 1 on success, 0 if empty. */
int deque_delete_rear(CircularDeque *dq)
{
    /* TODO: Move rear backward (with wrap), decrement size */
    (void)dq;
    return 0;
}

/* Get front element. Return -1 if empty. */
int deque_get_front(const CircularDeque *dq)
{
    /* TODO */
    (void)dq;
    return -1;
}

/* Get rear element. Return -1 if empty. */
int deque_get_rear(const CircularDeque *dq)
{
    /* TODO */
    (void)dq;
    return -1;
}

/* Return 1 if full. */
int deque_is_full(const CircularDeque *dq)
{
    /* TODO */
    (void)dq;
    return 0;
}

/* Return 1 if empty. */
int deque_is_empty(const CircularDeque *dq)
{
    /* TODO */
    (void)dq;
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════
 *  Exercise 4: Flatten Nested List Iterator
 * ═══════════════════════════════════════════════════════════════════
 * Given a nested list like [1, [2, [3]], 4], iterate elements
 * one at a time: 1, 2, 3, 4.
 *
 * Use a stack to track where we are in each nesting level.
 * Each stack frame stores: which list we're in and which index
 * we're currently at in that list.
 */

typedef struct NestedItem {
    int is_list;              /* 0 = integer, 1 = nested list */
    int value;                /* Valid if is_list == 0 */
    struct NestedItem *items; /* Valid if is_list == 1 */
    int count;                /* Number of items if is_list == 1 */
} NestedItem;

typedef struct {
    NestedItem *stack_items[256];  /* Pointer to current list at each level */
    int stack_index[256];          /* Current index within that list */
    int stack_count[256];          /* Number of items in that list */
    int stack_top;                 /* -1 = empty */
} NestedIterator;

/* Initialize the iterator with the top-level list. */
void ni_init(NestedIterator *it, NestedItem *items, int count)
{
    /* TODO: Push the top-level list onto the stack */
    (void)it; (void)items; (void)count;
}

/* Return 1 if there are more integers to return.
 * This should advance past any nested lists by pushing them onto
 * the stack until we find an integer or exhaust all items. */
int ni_has_next(NestedIterator *it)
{
    /* TODO: While stack not empty:
     *   - If current index >= count at top level, pop (exhausted this list)
     *   - Else if current item is an integer, return 1
     *   - Else current item is a list: push it, advance index */
    (void)it;
    return 0;
}

/* Return the next integer and advance. Call ni_has_next first. */
int ni_next(NestedIterator *it)
{
    /* TODO: The current item (after ni_has_next) must be an integer.
     * Return its value and advance the index. */
    (void)it;
    return 0;
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

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
    printf("=== Data Structure Design Problems ===\n\n");

    /* --- Min Stack --- */
    printf("Min Stack:\n");
    {
        MinStack s;
        min_stack_init(&s);
        min_stack_push(&s, 5);
        TEST("push 5, getMin=5")
            EXPECT(min_stack_get_min(&s) == 5);
        min_stack_push(&s, 3);
        TEST("push 3, getMin=3")
            EXPECT(min_stack_get_min(&s) == 3);
        min_stack_push(&s, 7);
        TEST("push 7, getMin still 3")
            EXPECT(min_stack_get_min(&s) == 3);
        min_stack_pop(&s);
        TEST("pop 7, getMin still 3")
            EXPECT(min_stack_get_min(&s) == 3);
        min_stack_pop(&s);
        TEST("pop 3, getMin back to 5")
            EXPECT(min_stack_get_min(&s) == 5);
    }

    /* --- Queue Using Two Stacks --- */
    printf("\nQueue Using Two Stacks:\n");
    {
        StackQueue q;
        sq_init(&q);
        TEST("new queue is empty")
            EXPECT(sq_is_empty(&q));
        sq_enqueue(&q, 1);
        sq_enqueue(&q, 2);
        sq_enqueue(&q, 3);
        TEST("dequeue returns 1 (FIFO)")
            EXPECT(sq_dequeue(&q) == 1);
        TEST("dequeue returns 2")
            EXPECT(sq_dequeue(&q) == 2);
        sq_enqueue(&q, 4);
        TEST("dequeue returns 3 (not 4)")
            EXPECT(sq_dequeue(&q) == 3);
        TEST("dequeue returns 4")
            EXPECT(sq_dequeue(&q) == 4);
    }

    /* --- Circular Deque --- */
    printf("\nCircular Deque:\n");
    {
        CircularDeque dq;
        deque_init(&dq, 5);
        TEST("new deque is empty")
            EXPECT(deque_is_empty(&dq));
        deque_insert_rear(&dq, 1);
        deque_insert_rear(&dq, 2);
        deque_insert_front(&dq, 0);
        TEST("front is 0 after insertFront(0)")
            EXPECT(deque_get_front(&dq) == 0);
        TEST("rear is 2 after insertRear(1,2)")
            EXPECT(deque_get_rear(&dq) == 2);
        deque_insert_rear(&dq, 3);
        deque_insert_rear(&dq, 4);
        TEST("deque is full (5 elements, cap 5)")
            EXPECT(deque_is_full(&dq));
        TEST("insertRear fails when full")
            EXPECT(deque_insert_rear(&dq, 99) == 0);
        deque_delete_front(&dq);
        TEST("after deleteFront, front is 1")
            EXPECT(deque_get_front(&dq) == 1);
    }

    /* --- Nested List Iterator --- */
    printf("\nNested List Iterator:\n");
    {
        /* Build: [1, [2, [3]], 4] */
        NestedItem inner2[] = {{0, 3, NULL, 0}};            /* [3] */
        NestedItem inner1[] = {
            {0, 2, NULL, 0},                                /* 2 */
            {1, 0, inner2, 1}                               /* [3] */
        };
        NestedItem top[] = {
            {0, 1, NULL, 0},                                /* 1 */
            {1, 0, inner1, 2},                              /* [2, [3]] */
            {0, 4, NULL, 0}                                 /* 4 */
        };
        NestedIterator it;
        ni_init(&it, top, 3);
        TEST("first element is 1")
            EXPECT(ni_has_next(&it) && ni_next(&it) == 1);
        TEST("second element is 2")
            EXPECT(ni_has_next(&it) && ni_next(&it) == 2);
        TEST("third element is 3 (deeply nested)")
            EXPECT(ni_has_next(&it) && ni_next(&it) == 3);
        TEST("fourth element is 4")
            EXPECT(ni_has_next(&it) && ni_next(&it) == 4);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
