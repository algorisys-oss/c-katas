/*
 * Kata: Producer-Consumer (Bounded Buffer)
 *
 * Build a thread-safe bounded buffer using mutex and condition variables.
 *
 *   - buffer_init(buf, capacity)  — allocate buffer with given capacity
 *   - buffer_destroy(buf)         — free buffer resources
 *   - buffer_put(buf, value)      — add item; blocks if full
 *   - buffer_get(buf, *value)     — remove item; blocks if empty
 *   - buffer_size(buf)            — current number of items
 *
 * 10 tests total.
 *
 * Hints:
 *   - Use pthread_mutex_t to protect all buffer state
 *   - Use pthread_cond_t not_full  (signaled when space opens up)
 *   - Use pthread_cond_t not_empty (signaled when item is added)
 *   - Always check conditions in a while loop (spurious wakeups!)
 *   - Use a circular buffer: head (read position), tail (write position)
 */

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* ── Bounded Buffer Structure ────────────────────────────────────── */

typedef struct {
    int *data;              /* circular buffer storage */
    int capacity;           /* max number of items */
    int size;               /* current number of items */
    int head;               /* read position (front of queue) */
    int tail;               /* write position (back of queue) */
    pthread_mutex_t lock;   /* protects all fields above */
    pthread_cond_t not_full;  /* signaled when buffer is no longer full */
    pthread_cond_t not_empty; /* signaled when buffer is no longer empty */
} bounded_buffer_t;

/* ── Exercise 1: buffer_init ─────────────────────────────────────
 * Allocate the data array (malloc) and initialize all fields.
 * Initialize the mutex and both condition variables.
 * Returns 0 on success, -1 on failure.
 */
int buffer_init(bounded_buffer_t *buf, int capacity)
{
    /* TODO: malloc buf->data, set capacity/size/head/tail,
     *       pthread_mutex_init, pthread_cond_init x2 */
    (void)buf;
    (void)capacity;
    return -1; /* placeholder */
}

/* ── Exercise 2: buffer_destroy ──────────────────────────────────
 * Free the data array and destroy the mutex and condition variables.
 */
void buffer_destroy(bounded_buffer_t *buf)
{
    /* TODO: free buf->data, pthread_mutex_destroy, pthread_cond_destroy x2 */
    (void)buf;
}

/* ── Exercise 3: buffer_put ──────────────────────────────────────
 * Add a value to the buffer. Blocks (waits) if the buffer is full.
 *
 * Steps:
 *   1. Lock the mutex
 *   2. While buffer is full, wait on not_full condition
 *   3. Write value at tail position, advance tail (circular!), increment size
 *   4. Signal not_empty (a consumer might be waiting)
 *   5. Unlock the mutex
 */
void buffer_put(bounded_buffer_t *buf, int value)
{
    /* TODO: implement blocking put */
    (void)buf;
    (void)value;
}

/* ── Exercise 4: buffer_get ──────────────────────────────────────
 * Remove a value from the buffer. Blocks (waits) if the buffer is empty.
 *
 * Steps:
 *   1. Lock the mutex
 *   2. While buffer is empty, wait on not_empty condition
 *   3. Read value from head position, advance head (circular!), decrement size
 *   4. Signal not_full (a producer might be waiting)
 *   5. Unlock the mutex
 */
void buffer_get(bounded_buffer_t *buf, int *value)
{
    /* TODO: implement blocking get */
    (void)buf;
    (void)value;
}

/* ── Exercise 5: buffer_size ─────────────────────────────────────
 * Return the current number of items in the buffer (thread-safe).
 */
int buffer_size(bounded_buffer_t *buf)
{
    /* TODO: lock, read size, unlock, return */
    (void)buf;
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* --- Multi-threaded test helpers --- */

#define MT_COUNT 1000

typedef struct {
    bounded_buffer_t *buf;
    int count;
} thread_arg_t;

static void *producer_fn(void *arg)
{
    thread_arg_t *ta = (thread_arg_t *)arg;
    for (int i = 0; i < ta->count; i++) {
        buffer_put(ta->buf, i);
    }
    return NULL;
}

static void *consumer_fn(void *arg)
{
    thread_arg_t *ta = (thread_arg_t *)arg;
    long sum = 0;
    int val;
    for (int i = 0; i < ta->count; i++) {
        buffer_get(ta->buf, &val);
        sum += val;
    }
    return (void *)sum;
}

int main(void)
{
    printf("=== Producer-Consumer (Bounded Buffer) ===\n\n");

    /* --- Single-threaded tests --- */
    printf("buffer_init / buffer_destroy:\n");
    {
        bounded_buffer_t buf;
        int rc = buffer_init(&buf, 5);
        TEST("init returns 0")         EXPECT(rc == 0);
        TEST("size after init is 0")   EXPECT(buffer_size(&buf) == 0);
        buffer_destroy(&buf);
    }

    printf("\nbuffer_put / buffer_get (single-threaded):\n");
    {
        bounded_buffer_t buf;
        buffer_init(&buf, 5);

        buffer_put(&buf, 42);
        TEST("size after 1 put is 1")  EXPECT(buffer_size(&buf) == 1);

        buffer_put(&buf, 99);
        TEST("size after 2 puts is 2") EXPECT(buffer_size(&buf) == 2);

        int val;
        buffer_get(&buf, &val);
        TEST("first get returns 42")   EXPECT(val == 42);

        buffer_get(&buf, &val);
        TEST("second get returns 99")  EXPECT(val == 99);

        TEST("size after all gets is 0") EXPECT(buffer_size(&buf) == 0);

        buffer_destroy(&buf);
    }

    printf("\nfill to capacity:\n");
    {
        bounded_buffer_t buf;
        buffer_init(&buf, 3);

        buffer_put(&buf, 10);
        buffer_put(&buf, 20);
        buffer_put(&buf, 30);
        TEST("size at capacity is 3")  EXPECT(buffer_size(&buf) == 3);

        int val;
        buffer_get(&buf, &val);
        TEST("get from full buf → 10") EXPECT(val == 10);
        TEST("size after get is 2")    EXPECT(buffer_size(&buf) == 2);

        buffer_destroy(&buf);
    }

    /* --- Multi-threaded test --- */
    printf("\nmulti-threaded producer-consumer:\n");
    {
        bounded_buffer_t buf;
        buffer_init(&buf, 16);  /* small buffer forces blocking */

        thread_arg_t parg = { &buf, MT_COUNT };
        thread_arg_t carg = { &buf, MT_COUNT };

        pthread_t producer, consumer;
        pthread_create(&producer, NULL, producer_fn, &parg);
        pthread_create(&consumer, NULL, consumer_fn, &carg);

        void *consumer_result;
        pthread_join(producer, NULL);
        pthread_join(consumer, &consumer_result);

        long expected_sum = (long)MT_COUNT * (MT_COUNT - 1) / 2;
        long actual_sum = (long)consumer_result;

        TEST("all items transferred correctly")
            EXPECT(actual_sum == expected_sum);

        TEST("buffer empty after all done")
            EXPECT(buffer_size(&buf) == 0);

        buffer_destroy(&buf);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
