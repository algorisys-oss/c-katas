/*
 * Solution: Producer-Consumer (Bounded Buffer)
 */

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

typedef struct {
    int *data;
    int capacity;
    int size;
    int head;
    int tail;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} bounded_buffer_t;

int buffer_init(bounded_buffer_t *buf, int capacity)
{
    buf->data = malloc(capacity * sizeof(int));
    if (buf->data == NULL) {
        return -1;
    }
    buf->capacity = capacity;
    buf->size = 0;
    buf->head = 0;
    buf->tail = 0;
    pthread_mutex_init(&buf->lock, NULL);
    pthread_cond_init(&buf->not_full, NULL);
    pthread_cond_init(&buf->not_empty, NULL);
    return 0;
}

void buffer_destroy(bounded_buffer_t *buf)
{
    free(buf->data);
    pthread_mutex_destroy(&buf->lock);
    pthread_cond_destroy(&buf->not_full);
    pthread_cond_destroy(&buf->not_empty);
}

void buffer_put(bounded_buffer_t *buf, int value)
{
    pthread_mutex_lock(&buf->lock);
    while (buf->size == buf->capacity) {
        pthread_cond_wait(&buf->not_full, &buf->lock);
    }
    buf->data[buf->tail] = value;
    buf->tail = (buf->tail + 1) % buf->capacity;
    buf->size++;
    pthread_cond_signal(&buf->not_empty);
    pthread_mutex_unlock(&buf->lock);
}

void buffer_get(bounded_buffer_t *buf, int *value)
{
    pthread_mutex_lock(&buf->lock);
    while (buf->size == 0) {
        pthread_cond_wait(&buf->not_empty, &buf->lock);
    }
    *value = buf->data[buf->head];
    buf->head = (buf->head + 1) % buf->capacity;
    buf->size--;
    pthread_cond_signal(&buf->not_full);
    pthread_mutex_unlock(&buf->lock);
}

int buffer_size(bounded_buffer_t *buf)
{
    pthread_mutex_lock(&buf->lock);
    int s = buf->size;
    pthread_mutex_unlock(&buf->lock);
    return s;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

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
    printf("=== Producer-Consumer (Solution) ===\n\n");

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

    printf("\nmulti-threaded producer-consumer:\n");
    {
        bounded_buffer_t buf;
        buffer_init(&buf, 16);

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
