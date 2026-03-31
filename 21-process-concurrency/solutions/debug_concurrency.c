/*
 * Debug Kata: Concurrency Bugs — SOLUTIONS
 *
 * All 4 bugs fixed. See comments marked "FIX:" for each change.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -pthread -o debug_concurrency debug_concurrency.c
 * Run:      ./debug_concurrency
 */

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

/* ── Test harness ─────────────────────────────────────────────── */

#define TEST(name) printf("  %-40s", name)
#define PASS() printf("[PASS]\n")
static int tests_passed = 0, tests_failed = 0;
#define EXPECT(cond) do { \
    if (cond) { PASS(); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); tests_failed++; } \
} while(0)

/* ================================================================
 * Bug 1 FIX: Added a mutex to protect the shared counter.
 * ================================================================ */

#define INCREMENTS_PER_THREAD 100000

static int shared_counter = 0;
/* FIX: added mutex */
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment_worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        /* FIX: lock before increment, unlock after */
        pthread_mutex_lock(&counter_mutex);
        shared_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int run_counter_test(void)
{
    shared_counter = 0;
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment_worker, NULL);
    pthread_create(&t2, NULL, increment_worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return shared_counter;
}

/* ================================================================
 * Bug 2 FIX: Both threads lock mutexes in the same order (A, then B).
 * ================================================================ */

static pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
static int resource_a = 0;
static int resource_b = 0;
static int deadlock_detected = 0;

void *worker_1(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex_a);
    usleep(1000);
    pthread_mutex_lock(&mutex_b);

    resource_a += 1;
    resource_b += 1;

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    return NULL;
}

void *worker_2_trylock(void *arg)
{
    (void)arg;
    /* FIX: lock in the same order as worker_1: A first, then B */
    pthread_mutex_lock(&mutex_a);
    usleep(1000);
    int rc = pthread_mutex_trylock(&mutex_b);
    if (rc == EBUSY) {
        deadlock_detected = 1;
        pthread_mutex_unlock(&mutex_a);
        return NULL;
    }

    resource_a += 10;
    resource_b += 10;

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    return NULL;
}

int run_deadlock_test(void)
{
    resource_a = 0;
    resource_b = 0;
    deadlock_detected = 0;

    pthread_mutex_init(&mutex_a, NULL);
    pthread_mutex_init(&mutex_b, NULL);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker_1, NULL);
    pthread_create(&t2, NULL, worker_2_trylock, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return deadlock_detected;
}

/* ================================================================
 * Bug 3 FIX: Unlock the mutex before returning on the error path.
 * ================================================================ */

static pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
static int shared_data = 0;

int process_data(int value)
{
    pthread_mutex_lock(&data_mutex);

    if (value < 0) {
        /* FIX: unlock before returning */
        pthread_mutex_unlock(&data_mutex);
        return -1;
    }

    shared_data += value;
    pthread_mutex_unlock(&data_mutex);
    return 0;
}

int run_unlock_test(void)
{
    pthread_mutex_init(&data_mutex, NULL);
    shared_data = 0;

    int rc1 = process_data(-5);
    if (rc1 != -1) {
        return -1;
    }

    int stuck = pthread_mutex_trylock(&data_mutex);
    if (stuck == EBUSY) {
        return 1;
    }
    pthread_mutex_unlock(&data_mutex);

    int rc2 = process_data(42);
    return (rc2 == 0 && shared_data == 42) ? 0 : -1;
}

/* ================================================================
 * Bug 4 FIX: Don't detach the thread — just create and join it.
 * ================================================================ */

static int detach_work_done = 0;

void *detach_worker(void *arg)
{
    (void)arg;
    detach_work_done = 1;
    return NULL;
}

int run_detach_test(void)
{
    detach_work_done = 0;

    pthread_t t;
    pthread_create(&t, NULL, detach_worker, NULL);

    /* FIX: removed pthread_detach(t) — just join directly */
    int rc = pthread_join(t, NULL);

    return rc;
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Concurrency Bugs (SOLUTIONS) ===\n\n");

    printf("Bug 1: Race condition\n");
    {
        int race_found = 0;
        int expected = 2 * INCREMENTS_PER_THREAD;
        for (int trial = 0; trial < 10; trial++) {
            int result = run_counter_test();
            if (result != expected) {
                race_found = 1;
                break;
            }
        }
        TEST("counter == 200000 (all trials)");
        EXPECT(!race_found);
    }

    printf("Bug 2: Deadlock\n");
    {
        int detected = run_deadlock_test();
        TEST("no deadlock detected");
        EXPECT(detected == 0);
    }

    printf("Bug 3: Missing unlock on error path\n");
    {
        int result = run_unlock_test();
        TEST("mutex unlocked after error path");
        EXPECT(result == 0);
    }

    printf("Bug 4: Joining a detached thread\n");
    {
        int rc = run_detach_test();
        TEST("pthread_join succeeds");
        EXPECT(rc == 0);
    }

    printf("\n  %d passed, %d failed\n\n", tests_passed, tests_failed);
    return tests_failed ? 1 : 0;
}
