/*
 * Kata: Circular Buffer Queue
 *
 * Implement a fixed-capacity queue using a circular buffer.
 * The buffer wraps around using modular arithmetic:
 *
 *    next_index = (current_index + 1) % capacity
 *
 * We track a separate count to distinguish full from empty.
 *
 * 12 tests total.
 */

#include <stdio.h>
#include <stdlib.h>

/* ── Queue Data Structure ──────────────────────────────────────────
 *
 *    Array:  [ C | D |   |   | A | B ]
 *    Index:    0   1   2   3   4   5
 *                      ↑       ↑
 *                    back=2  front=4   count=4
 *
 * Enqueue writes at 'back' and advances it (wrapping).
 * Dequeue reads at 'front' and advances it (wrapping).
 */
typedef struct {
    int *data;      /* dynamically allocated circular buffer */
    int front;      /* index of the front element */
    int back;       /* index of the next empty slot */
    int count;      /* number of elements currently in the queue */
    int capacity;   /* maximum number of elements */
} Queue;

/* ── Exercise 1: queue_create ──────────────────────────────────────
 * Allocate and return a new queue with the given capacity.
 * Initialize front, back, and count to 0.
 */
Queue *queue_create(int capacity)
{
    /* TODO: malloc a Queue, malloc its data array, set fields */
    return NULL; /* placeholder */
}

/* ── Exercise 2: queue_destroy ─────────────────────────────────────
 * Free the queue's data array, then free the queue itself.
 */
void queue_destroy(Queue *q)
{
    /* TODO: free data, then free the struct */
}

/* ── Exercise 3: queue_enqueue ─────────────────────────────────────
 * Add a value to the back of the queue.
 * Return 1 on success, 0 if the queue is full.
 *
 * Hint: data[back] = value; back = (back + 1) % capacity; count++;
 */
int queue_enqueue(Queue *q, int value)
{
    /* TODO */
    return 0; /* placeholder */
}

/* ── Exercise 4: queue_dequeue ─────────────────────────────────────
 * Remove and return the front value.
 * Return -1 if the queue is empty (simple error signal).
 *
 * Hint: save data[front]; front = (front + 1) % capacity; count--;
 */
int queue_dequeue(Queue *q)
{
    /* TODO */
    return -1; /* placeholder */
}

/* ── Exercise 5: queue_peek ────────────────────────────────────────
 * Return the front value without removing it.
 * Return -1 if the queue is empty.
 */
int queue_peek(const Queue *q)
{
    /* TODO */
    return -1; /* placeholder */
}

/* ── Exercise 6: queue_is_empty ────────────────────────────────────
 * Return 1 if the queue has no elements, 0 otherwise.
 */
int queue_is_empty(const Queue *q)
{
    /* TODO */
    return 1; /* placeholder */
}

/* ── Exercise 7: queue_is_full ─────────────────────────────────────
 * Return 1 if the queue is at capacity, 0 otherwise.
 */
int queue_is_full(const Queue *q)
{
    /* TODO */
    return 0; /* placeholder */
}

/* ── Exercise 8: queue_size ────────────────────────────────────────
 * Return the number of elements currently in the queue.
 */
int queue_size(const Queue *q)
{
    /* TODO */
    return 0; /* placeholder */
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
    printf("=== Circular Buffer Queue ===\n\n");

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

        /* Dequeue one, then enqueue — tests wrapping */
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
