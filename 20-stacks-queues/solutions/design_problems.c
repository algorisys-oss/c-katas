/*
 * Solution: Data Structure Design Problems
 *
 * Four exercises: Min Stack, Queue Using Stacks, Circular Deque,
 * Nested List Iterator.
 *
 * 20 tests total.
 */

#include <stdio.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════
 *  Min Stack
 * ═══════════════════════════════════════════════════════════════════ */

#define STACK_CAP 1024

typedef struct {
    int data[STACK_CAP];
    int min_data[STACK_CAP];
    int top;
} MinStack;

void min_stack_init(MinStack *s)
{
    s->top = -1;
}

void min_stack_push(MinStack *s, int val)
{
    if (s->top >= STACK_CAP - 1) return;
    s->top++;
    s->data[s->top] = val;
    if (s->top == 0) {
        s->min_data[s->top] = val;
    } else {
        /* New minimum is the smaller of val and previous minimum */
        int prev_min = s->min_data[s->top - 1];
        s->min_data[s->top] = (val < prev_min) ? val : prev_min;
    }
}

int min_stack_pop(MinStack *s)
{
    if (s->top < 0) return 0;
    int val = s->data[s->top];
    s->top--;
    return val;
}

int min_stack_top(const MinStack *s)
{
    if (s->top < 0) return 0;
    return s->data[s->top];
}

int min_stack_get_min(const MinStack *s)
{
    if (s->top < 0) return 0;
    return s->min_data[s->top];
}

/* ═══════════════════════════════════════════════════════════════════
 *  Queue Using Two Stacks
 * ═══════════════════════════════════════════════════════════════════ */

typedef struct {
    int input[STACK_CAP];
    int input_top;
    int output[STACK_CAP];
    int output_top;
} StackQueue;

void sq_init(StackQueue *q)
{
    q->input_top = -1;
    q->output_top = -1;
}

void sq_enqueue(StackQueue *q, int val)
{
    q->input_top++;
    q->input[q->input_top] = val;
}

int sq_dequeue(StackQueue *q)
{
    /* If output is empty, pour all input elements into output */
    if (q->output_top < 0) {
        while (q->input_top >= 0) {
            q->output_top++;
            q->output[q->output_top] = q->input[q->input_top];
            q->input_top--;
        }
    }
    if (q->output_top < 0) return 0;  /* Queue empty */
    int val = q->output[q->output_top];
    q->output_top--;
    return val;
}

int sq_is_empty(const StackQueue *q)
{
    return q->input_top < 0 && q->output_top < 0;
}

/* ═══════════════════════════════════════════════════════════════════
 *  Circular Deque
 * ═══════════════════════════════════════════════════════════════════ */

typedef struct {
    int data[STACK_CAP];
    int front;
    int rear;
    int size;
    int capacity;
} CircularDeque;

void deque_init(CircularDeque *dq, int capacity)
{
    dq->front = 0;
    dq->rear = 0;
    dq->size = 0;
    dq->capacity = capacity;
}

int deque_is_full(const CircularDeque *dq)
{
    return dq->size == dq->capacity;
}

int deque_is_empty(const CircularDeque *dq)
{
    return dq->size == 0;
}

int deque_insert_front(CircularDeque *dq, int val)
{
    if (deque_is_full(dq)) return 0;
    /* Move front backward with wrap-around */
    dq->front = (dq->front - 1 + dq->capacity) % dq->capacity;
    dq->data[dq->front] = val;
    dq->size++;
    return 1;
}

int deque_insert_rear(CircularDeque *dq, int val)
{
    if (deque_is_full(dq)) return 0;
    dq->data[dq->rear] = val;
    dq->rear = (dq->rear + 1) % dq->capacity;
    dq->size++;
    return 1;
}

int deque_delete_front(CircularDeque *dq)
{
    if (deque_is_empty(dq)) return 0;
    dq->front = (dq->front + 1) % dq->capacity;
    dq->size--;
    return 1;
}

int deque_delete_rear(CircularDeque *dq)
{
    if (deque_is_empty(dq)) return 0;
    dq->rear = (dq->rear - 1 + dq->capacity) % dq->capacity;
    dq->size--;
    return 1;
}

int deque_get_front(const CircularDeque *dq)
{
    if (deque_is_empty(dq)) return -1;
    return dq->data[dq->front];
}

int deque_get_rear(const CircularDeque *dq)
{
    if (deque_is_empty(dq)) return -1;
    /* rear points to the next empty slot, so the last element is at rear-1 */
    int idx = (dq->rear - 1 + dq->capacity) % dq->capacity;
    return dq->data[idx];
}

/* ═══════════════════════════════════════════════════════════════════
 *  Nested List Iterator
 * ═══════════════════════════════════════════════════════════════════ */

typedef struct NestedItem {
    int is_list;
    int value;
    struct NestedItem *items;
    int count;
} NestedItem;

typedef struct {
    NestedItem *stack_items[256];
    int stack_index[256];
    int stack_count[256];
    int stack_top;
} NestedIterator;

void ni_init(NestedIterator *it, NestedItem *items, int count)
{
    it->stack_top = 0;
    it->stack_items[0] = items;
    it->stack_index[0] = 0;
    it->stack_count[0] = count;
}

int ni_has_next(NestedIterator *it)
{
    while (it->stack_top >= 0) {
        int level = it->stack_top;
        int idx = it->stack_index[level];
        int cnt = it->stack_count[level];

        if (idx >= cnt) {
            /* Exhausted this list — pop and continue */
            it->stack_top--;
            continue;
        }

        NestedItem *current = &it->stack_items[level][idx];
        if (!current->is_list) {
            /* Found an integer — ready to return */
            return 1;
        }

        /* Current item is a nested list — push it and advance past it */
        it->stack_index[level]++;  /* Skip the list item at this level */
        it->stack_top++;
        it->stack_items[it->stack_top] = current->items;
        it->stack_index[it->stack_top] = 0;
        it->stack_count[it->stack_top] = current->count;
    }
    return 0;
}

int ni_next(NestedIterator *it)
{
    int level = it->stack_top;
    int idx = it->stack_index[level];
    int val = it->stack_items[level][idx].value;
    it->stack_index[level]++;
    return val;
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
        NestedItem inner2[] = {{0, 3, NULL, 0}};
        NestedItem inner1[] = {
            {0, 2, NULL, 0},
            {1, 0, inner2, 1}
        };
        NestedItem top[] = {
            {0, 1, NULL, 0},
            {1, 0, inner1, 2},
            {0, 4, NULL, 0}
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
