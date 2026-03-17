/*
 * Solution: Circular Buffer Queue
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int front;
    int back;
    int count;
    int capacity;
} Queue;

Queue *queue_create(int capacity)
{
    Queue *q = malloc(sizeof(Queue));
    if (!q) return NULL;
    q->data = malloc(sizeof(int) * capacity);
    if (!q->data) {
        free(q);
        return NULL;
    }
    q->front = 0;
    q->back = 0;
    q->count = 0;
    q->capacity = capacity;
    return q;
}

void queue_destroy(Queue *q)
{
    if (q) {
        free(q->data);
        free(q);
    }
}

int queue_enqueue(Queue *q, int value)
{
    if (q->count >= q->capacity) return 0;
    q->data[q->back] = value;
    q->back = (q->back + 1) % q->capacity;
    q->count++;
    return 1;
}

int queue_dequeue(Queue *q)
{
    if (q->count == 0) return -1;
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->count--;
    return value;
}

int queue_peek(const Queue *q)
{
    if (q->count == 0) return -1;
    return q->data[q->front];
}

int queue_is_empty(const Queue *q)
{
    return q->count == 0;
}

int queue_is_full(const Queue *q)
{
    return q->count == q->capacity;
}

int queue_size(const Queue *q)
{
    return q->count;
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
    printf("=== Circular Buffer Queue (Solution) ===\n\n");

    printf("Basic operations:\n");
    {
        Queue *q = queue_create(5);
        TEST("new queue is empty")
            EXPECT(queue_is_empty(q));

        TEST("new queue size is 0")
            EXPECT(queue_size(q) == 0);

        queue_enqueue(q, 10);
        queue_enqueue(q, 20);
        queue_enqueue(q, 30);
        TEST("enqueue 10,20,30 — size is 3")
            EXPECT(queue_size(q) == 3);

        TEST("peek returns 10 (front)")
            EXPECT(queue_peek(q) == 10);

        TEST("dequeue returns 10")
            EXPECT(queue_dequeue(q) == 10);

        TEST("dequeue returns 20")
            EXPECT(queue_dequeue(q) == 20);

        TEST("size is 1 after two dequeues")
            EXPECT(queue_size(q) == 1);

        queue_destroy(q);
    }

    printf("\nFull queue and wrapping:\n");
    {
        Queue *q = queue_create(3);

        queue_enqueue(q, 1);
        queue_enqueue(q, 2);
        queue_enqueue(q, 3);
        TEST("queue of capacity 3 with 3 elements is full")
            EXPECT(queue_is_full(q));

        TEST("enqueue on full queue returns 0")
            EXPECT(queue_enqueue(q, 4) == 0);

        int val = queue_dequeue(q);
        TEST("dequeue returns 1")
            EXPECT(val == 1);

        queue_enqueue(q, 4);
        TEST("after wrap: dequeue returns 2")
            EXPECT(queue_dequeue(q) == 2);

        TEST("after wrap: dequeue returns 3")
            EXPECT(queue_dequeue(q) == 3);

        TEST("after wrap: dequeue returns 4")
            EXPECT(queue_dequeue(q) == 4);

        TEST("queue is empty after draining")
            EXPECT(queue_is_empty(q));

        TEST("dequeue on empty returns -1")
            EXPECT(queue_dequeue(q) == -1);

        queue_destroy(q);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
