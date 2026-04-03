/*
 * Kata: Parallel Sum
 *
 * Sum a large array in parallel using pthreads.
 *
 *   - sequential_sum(arr, n)              — single-threaded sum (baseline)
 *   - parallel_sum(arr, n, num_threads)   — split into chunks, thread per chunk
 *
 * 8 tests total.
 *
 * Hints:
 *   - Divide the array into num_threads chunks
 *   - Each thread sums its chunk into a thread-local variable (no mutex needed!)
 *   - After all threads join, add up their partial sums
 *   - Handle the case where n is not evenly divisible by num_threads
 *   - Pass a struct to each thread with: pointer to start, count of elements
 */

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* ── Thread argument structure ───────────────────────────────────
 * Each thread needs to know: where its chunk starts and how many elements.
 * It writes its partial sum into 'result'.
 */
typedef struct {
    const int *start;   /* pointer to first element of this chunk */
    int count;          /* number of elements in this chunk */
    long result;        /* partial sum computed by this thread */
} chunk_t;

/* ── Exercise 1: sequential_sum ──────────────────────────────────
 * Simple loop: sum all elements of arr[0..n-1].
 * Returns the total sum.
 */
long sequential_sum(const int *arr, int n)
{
    /* TODO: loop and sum */
    (void)arr;
    (void)n;
    return 0; /* placeholder */
}

/* ── Exercise 2: chunk_worker ────────────────────────────────────
 * Thread function: sum the elements in our chunk.
 *
 * arg points to a chunk_t. Sum chunk->start[0..chunk->count-1]
 * and store the result in chunk->result.
 */
static void *chunk_worker(void *arg)
{
    /* TODO: cast arg to chunk_t*, sum elements, store in result */
    (void)arg;
    return NULL;
}

/* ── Exercise 3: parallel_sum ────────────────────────────────────
 * Split the array into num_threads chunks, create a thread for each,
 * then combine their partial sums.
 *
 * Steps:
 *   1. Allocate arrays for pthread_t handles and chunk_t structs
 *   2. Divide n elements among num_threads (last thread gets remainder)
 *   3. Create threads with chunk_worker
 *   4. Join all threads
 *   5. Sum up all chunk results
 *   6. Free allocated memory
 *   7. Return total
 *
 * Edge case: if num_threads > n, use only n threads (1 element each).
 */
long parallel_sum(const int *arr, int n, int num_threads)
{
    /* TODO: implement parallel sum */
    (void)arr;
    (void)n;
    (void)num_threads;
    (void)chunk_worker; /* will be used when you implement this */
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

int main(void)
{
    printf("=== Parallel Sum ===\n\n");

    /* --- sequential_sum tests --- */
    printf("sequential_sum:\n");
    {
        int arr[] = {1, 2, 3, 4, 5};
        TEST("sum {1,2,3,4,5} = 15")
            EXPECT(sequential_sum(arr, 5) == 15);
    }
    {
        int arr[] = {-10, 20, -30, 40};
        TEST("sum {-10,20,-30,40} = 20")
            EXPECT(sequential_sum(arr, 4) == 20);
    }

    /* --- parallel_sum with 1 thread (same as sequential) --- */
    printf("\nparallel_sum (1 thread):\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        TEST("1 thread: sum {10..50} = 150")
            EXPECT(parallel_sum(arr, 5, 1) == 150);
    }

    /* --- parallel_sum with multiple threads --- */
    printf("\nparallel_sum (multiple threads):\n");
    {
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        TEST("2 threads: sum {1..10} = 55")
            EXPECT(parallel_sum(arr, 10, 2) == 55);
    }
    {
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        TEST("4 threads: sum {1..10} = 55")
            EXPECT(parallel_sum(arr, 10, 4) == 55);
    }
    {
        int arr[] = {1, 2, 3, 4, 5, 6, 7};
        TEST("3 threads: sum {1..7} = 28 (uneven)")
            EXPECT(parallel_sum(arr, 7, 3) == 28);
    }

    /* --- large array test --- */
    printf("\nparallel_sum (large array):\n");
    {
        int n = 100000;
        int *arr = malloc(n * sizeof(int));
        assert(arr != NULL);
        long expected = 0;
        for (int i = 0; i < n; i++) {
            arr[i] = i + 1;
            expected += (i + 1);
        }
        long par_result = parallel_sum(arr, n, 4);
        long seq_result = sequential_sum(arr, n);

        TEST("4 threads: 100k elements matches sequential")
            EXPECT(par_result == seq_result && par_result == expected);
        free(arr);
    }

    /* --- more threads than elements --- */
    printf("\nparallel_sum (edge cases):\n");
    {
        int arr[] = {5, 10};
        TEST("8 threads, 2 elements: sum = 15")
            EXPECT(parallel_sum(arr, 2, 8) == 15);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
