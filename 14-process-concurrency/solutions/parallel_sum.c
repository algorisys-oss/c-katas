/*
 * Solution: Parallel Sum
 */

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

typedef struct {
    const int *start;
    int count;
    long result;
} chunk_t;

long sequential_sum(const int *arr, int n)
{
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

static void *chunk_worker(void *arg)
{
    chunk_t *chunk = (chunk_t *)arg;
    long sum = 0;
    for (int i = 0; i < chunk->count; i++) {
        sum += chunk->start[i];
    }
    chunk->result = sum;
    return NULL;
}

long parallel_sum(const int *arr, int n, int num_threads)
{
    /* Clamp threads to number of elements */
    if (num_threads > n) {
        num_threads = n;
    }

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    chunk_t *chunks = malloc(num_threads * sizeof(chunk_t));
    assert(threads != NULL && chunks != NULL);

    int base_size = n / num_threads;
    int remainder = n % num_threads;
    int offset = 0;

    for (int i = 0; i < num_threads; i++) {
        chunks[i].start = arr + offset;
        chunks[i].count = base_size + (i < remainder ? 1 : 0);
        chunks[i].result = 0;
        offset += chunks[i].count;
        pthread_create(&threads[i], NULL, chunk_worker, &chunks[i]);
    }

    long total = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total += chunks[i].result;
    }

    free(threads);
    free(chunks);
    return total;
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

int main(void)
{
    printf("=== Parallel Sum (Solution) ===\n\n");

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

    printf("\nparallel_sum (1 thread):\n");
    {
        int arr[] = {10, 20, 30, 40, 50};
        TEST("1 thread: sum {10..50} = 150")
            EXPECT(parallel_sum(arr, 5, 1) == 150);
    }

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

    printf("\nparallel_sum (edge cases):\n");
    {
        int arr[] = {5, 10};
        TEST("8 threads, 2 elements: sum = 15")
            EXPECT(parallel_sum(arr, 2, 8) == 15);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
