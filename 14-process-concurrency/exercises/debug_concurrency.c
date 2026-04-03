/*
 * Debug Kata: Concurrency Bugs
 *
 * This file contains 4 buggy concurrency functions using pthreads.
 * Each function compiles and runs, but has a concurrency bug.
 * Your job: find and fix each bug.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -pthread -o debug_concurrency debug_concurrency.c
 * Run:      ./debug_concurrency
 *
 * 4 bugs to fix.
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
 * Bug 1: Race Condition — unsynchronized shared counter
 * ================================================================
 *
 * Two threads each increment a shared counter 100,000 times.
 * The expected final value is 200,000, but without a mutex the
 * increments can interleave and the count ends up too low.
 *
 * To expose the bug, we run the test multiple times. A single run
 * might accidentally get the right answer, but over many runs
 * the race condition will show up.
 *
 * HINT: Protect the shared counter with a pthread_mutex_t.
 *       Lock before incrementing, unlock after.
 */

#define INCREMENTS_PER_THREAD 100000

static int shared_counter = 0;
/* TODO: add a mutex here */

void *increment_worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        /* BUG: no mutex protecting the shared counter */
        shared_counter++;
    }
    return NULL;
}

/* Returns the final counter value after two threads each do
 * INCREMENTS_PER_THREAD increments. */
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
 * Bug 2: Deadlock — opposite lock ordering
 * ================================================================
 *
 * Two threads each need to hold two mutexes (mutex_a and mutex_b).
 * Thread 1 locks A then B. Thread 2 locks B then A.
 * This causes a deadlock: each thread holds one lock and waits
 * for the other.
 *
 * We use pthread_mutex_trylock with a timeout approach to detect
 * the deadlock rather than hanging forever.
 *
 * HINT: Both threads should lock the mutexes in the SAME order.
 *       Convention: always lock A before B.
 */

static pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
static int resource_a = 0;
static int resource_b = 0;
static int deadlock_detected = 0;

void *worker_1(void *arg)
{
    (void)arg;
    /* Locks A, then B — this order is fine */
    pthread_mutex_lock(&mutex_a);
    usleep(1000);  /* small delay to increase chance of deadlock */
    pthread_mutex_lock(&mutex_b);

    resource_a += 1;
    resource_b += 1;

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    return NULL;
}

void *worker_2(void *arg)
{
    (void)arg;
    /* BUG: Locks B first, then A — opposite order from worker_1!
     * This creates a deadlock cycle: worker_1 holds A, waits for B;
     * worker_2 holds B, waits for A. */
    pthread_mutex_lock(&mutex_b);
    usleep(1000);  /* small delay to increase chance of deadlock */
    pthread_mutex_lock(&mutex_a);

    resource_a += 10;
    resource_b += 10;

    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    return NULL;
}

/* We detect the deadlock by using trylock in a helper that
 * gives up after a timeout instead of blocking forever. */
void *worker_2_trylock(void *arg)
{
    (void)arg;
    /* Same buggy order as worker_2 but using trylock so we
     * don't hang the whole test. */
    pthread_mutex_lock(&mutex_b);
    usleep(1000);
    int rc = pthread_mutex_trylock(&mutex_a);
    if (rc == EBUSY) {
        /* Could not acquire — deadlock would have occurred */
        deadlock_detected = 1;
        pthread_mutex_unlock(&mutex_b);
        return NULL;
    }
    resource_a += 10;
    resource_b += 10;
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    return NULL;
}

int run_deadlock_test(void)
{
    /* Reset state */
    resource_a = 0;
    resource_b = 0;
    deadlock_detected = 0;

    /* Re-init mutexes to clean state */
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
 * Bug 3: Missing mutex unlock on error path
 * ================================================================
 *
 * process_data() locks a mutex, validates its input, then does
 * some work. If validation fails, it returns -1 — but forgets
 * to unlock the mutex first! The next call will deadlock.
 *
 * HINT: Always unlock the mutex before returning, even on error.
 *       Or restructure so the early return happens before the lock.
 */

static pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
static int shared_data = 0;

int process_data(int value)
{
    pthread_mutex_lock(&data_mutex);

    /* Validate input */
    if (value < 0) {
        /* BUG: returning without unlocking the mutex!
         * The next call to process_data() will deadlock. */
        return -1;
    }

    shared_data += value;
    pthread_mutex_unlock(&data_mutex);
    return 0;
}

/* We test by calling with a bad value, then a good value.
 * If the bug is present, the second call deadlocks.
 * We use trylock to detect this instead of hanging. */
int run_unlock_test(void)
{
    /* Re-init to clean state */
    pthread_mutex_init(&data_mutex, NULL);
    shared_data = 0;

    /* First call: bad value, should return -1 */
    int rc1 = process_data(-5);
    if (rc1 != -1) {
        return -1;  /* unexpected */
    }

    /* Second call: if mutex was left locked, this will deadlock.
     * We use trylock to check if the mutex is stuck. */
    int stuck = pthread_mutex_trylock(&data_mutex);
    if (stuck == EBUSY) {
        /* Mutex is still locked — bug confirmed! */
        return 1;  /* 1 = bug detected */
    }
    /* If we got the lock, the bug is fixed. Unlock and proceed. */
    pthread_mutex_unlock(&data_mutex);

    /* Now do a normal call to make sure it works */
    int rc2 = process_data(42);
    return (rc2 == 0 && shared_data == 42) ? 0 : -1;
}

/* ================================================================
 * Bug 4: Joining a detached thread
 * ================================================================
 *
 * A thread is created and then detached with pthread_detach().
 * Detached threads clean up after themselves when they finish —
 * you cannot join them. But the code tries to pthread_join() the
 * detached thread, which is undefined behavior (usually returns
 * an error code like EINVAL).
 *
 * HINT: Either don't detach (so you can join), or don't join
 *       (and let the detached thread clean up on its own).
 */

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

    /* BUG: detaching, then trying to join.
     * pthread_join on a detached thread is undefined behavior.
     * It typically returns EINVAL. */
    pthread_detach(t);
    int rc = pthread_join(t, NULL);

    /* rc will be non-zero (error) because we can't join a detached thread */
    return rc;  /* non-zero = bug detected */
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Concurrency Bugs ===\n\n");

    /* --- Bug 1: Race condition ---
     * NOTE: A single run might accidentally produce the correct result.
     * We run it multiple times. If ANY run gives the wrong answer,
     * the race condition is confirmed. */
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
        /* When FIXED, every trial should produce the correct result.
         * When BROKEN, at least one trial will likely be wrong. */
        TEST("counter == 200000 (all trials)");
        EXPECT(!race_found);
    }

    /* --- Bug 2: Deadlock --- */
    printf("Bug 2: Deadlock\n");
    {
        int detected = run_deadlock_test();
        /* When FIXED, no deadlock is detected (detected == 0).
         * When BROKEN, trylock fails because worker_1 holds mutex_a. */
        TEST("no deadlock detected");
        EXPECT(detected == 0);
    }

    /* --- Bug 3: Missing unlock on error --- */
    printf("Bug 3: Missing unlock on error path\n");
    {
        int result = run_unlock_test();
        /* When FIXED, returns 0 (mutex properly unlocked).
         * When BROKEN, returns 1 (mutex left locked). */
        TEST("mutex unlocked after error path");
        EXPECT(result == 0);
    }

    /* --- Bug 4: Joining a detached thread --- */
    printf("Bug 4: Joining a detached thread\n");
    {
        int rc = run_detach_test();
        /* When FIXED, pthread_join succeeds (returns 0).
         * When BROKEN, pthread_join returns an error (non-zero). */
        TEST("pthread_join succeeds");
        EXPECT(rc == 0);
    }

    /* ── Summary ─────────────────────────────────────────────── */
    printf("\n  %d passed, %d failed\n\n", tests_passed, tests_failed);
    return tests_failed ? 1 : 0;
}
