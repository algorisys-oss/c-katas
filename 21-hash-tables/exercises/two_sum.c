/*
 * Kata: Two Sum — Three Ways
 *
 * The classic problem: given an array of integers and a target sum,
 * find two elements that add up to the target. Return their indices.
 *
 * We solve it THREE ways to show why data structures matter:
 *
 *   ┌──────────────────┬──────────┬──────────┬─────────────────────┐
 *   │ Approach          │ Time     │ Space    │ Requirement         │
 *   ├──────────────────┼──────────┼──────────┼─────────────────────┤
 *   │ Brute force       │ O(n^2)   │ O(1)     │ None                │
 *   │ Hash table        │ O(n)     │ O(n)     │ None                │
 *   │ Two pointers      │ O(n)     │ O(1)     │ Array must be sorted│
 *   └──────────────────┴──────────┴──────────┴─────────────────────┘
 *
 * Same problem, three approaches, vastly different performance.
 * With 1 million elements:
 *   - Brute force: ~500 billion comparisons
 *   - Hash table:  ~1 million lookups
 *   - Two pointers: ~1 million steps (but needs sorted input)
 *
 * 10 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Exercise 1: two_sum_brute ────────────────────────────────────────
 *
 * Find two indices i, j such that arr[i] + arr[j] == target.
 * Return them through *out_i and *out_j.
 * Return 1 if found, 0 if no such pair exists.
 *
 * Algorithm: check every possible pair (i, j) where i < j.
 *   for i in 0..n-1:
 *     for j in i+1..n-1:
 *       if arr[i] + arr[j] == target: found!
 *
 * This is O(n^2) — for each of n elements, we check up to n others.
 */
int two_sum_brute(const int *arr, int n, int target, int *out_i, int *out_j)
{
    /* TODO: nested loop over all pairs */
    (void)arr; (void)n; (void)target; (void)out_i; (void)out_j;
    return 0; /* placeholder */
}

/* ── Exercise 2: two_sum_hash ─────────────────────────────────────────
 *
 * The complement trick: for each element arr[i], the number we NEED
 * is (target - arr[i]). If we've already seen that number, we're done.
 *
 * Algorithm:
 *   - Create a hash table mapping value -> index
 *   - For each element arr[i]:
 *     - Compute complement = target - arr[i]
 *     - Look up complement in the hash table
 *     - If found: return (hash_table[complement], i)
 *     - If not found: insert arr[i] -> i into the hash table
 *
 * This is O(n) — one pass through the array, each lookup is O(1).
 *
 * For simplicity, we use a fixed-size hash table with linear probing.
 * The table maps integer values to their indices in the array.
 */

/* Simple hash table for int -> int mapping */
#define HT_SIZE 1024
#define HT_EMPTY -1

typedef struct {
    int keys[HT_SIZE];
    int values[HT_SIZE];
    int occupied[HT_SIZE]; /* 0 = empty, 1 = occupied */
} IntHashTable;

static void iht_init(IntHashTable *ht)
{
    memset(ht->occupied, 0, sizeof(ht->occupied));
}

/* Hash an integer key to a table index */
static int iht_hash(int key)
{
    /* Handle negative keys by making them positive */
    unsigned int k = (unsigned int)key;
    return (int)(k % HT_SIZE);
}

/* TODO: Implement iht_put — insert key-value pair with linear probing */
static void iht_put(IntHashTable *ht, int key, int value)
{
    /* TODO: find empty slot using linear probing, store key and value */
    (void)ht; (void)key; (void)value;
}

/* TODO: Implement iht_get — lookup key, store value in *out_value
 * Return 1 if found, 0 if not found. */
static int iht_get(const IntHashTable *ht, int key, int *out_value)
{
    /* TODO: linear probe to find matching key */
    (void)ht; (void)key; (void)out_value;
    return 0; /* placeholder */
}

int two_sum_hash(const int *arr, int n, int target, int *out_i, int *out_j)
{
    /* TODO: use IntHashTable to find the pair in O(n) time */
    (void)arr; (void)n; (void)target; (void)out_i; (void)out_j;
    return 0; /* placeholder */
}

/* ── Exercise 3: two_sum_sorted ───────────────────────────────────────
 *
 * If the array is SORTED, we can use two pointers — no extra memory.
 *
 * Algorithm:
 *   - left = 0, right = n - 1
 *   - While left < right:
 *     - sum = arr[left] + arr[right]
 *     - If sum == target: found!
 *     - If sum < target: move left pointer right (need a bigger number)
 *     - If sum > target: move right pointer left (need a smaller number)
 *
 * Why does this work? If sum is too small, arr[left] is the smallest
 * unused number, so we need the next bigger one. If sum is too big,
 * arr[right] is the largest unused number, so we need a smaller one.
 *
 * This is O(n) time and O(1) space — but requires sorted input.
 */
int two_sum_sorted(const int *arr, int n, int target, int *out_i, int *out_j)
{
    /* TODO: two-pointer approach on sorted array */
    (void)arr; (void)n; (void)target; (void)out_i; (void)out_j;
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
    printf("=== Two Sum — Three Ways ===\n\n");

    int idx_i, idx_j;

    /* --- Brute force --- */
    printf("two_sum_brute:\n");
    {
        int arr1[] = {2, 7, 11, 15};
        TEST("[2,7,11,15] target=9 -> (0,1)")
            EXPECT(two_sum_brute(arr1, 4, 9, &idx_i, &idx_j) == 1 &&
                   idx_i == 0 && idx_j == 1);

        int arr2[] = {3, 2, 4};
        TEST("[3,2,4] target=6 -> (1,2)")
            EXPECT(two_sum_brute(arr2, 3, 6, &idx_i, &idx_j) == 1 &&
                   idx_i == 1 && idx_j == 2);

        int arr3[] = {1, 2, 3};
        TEST("[1,2,3] target=10 -> not found")
            EXPECT(two_sum_brute(arr3, 3, 10, &idx_i, &idx_j) == 0);
    }

    /* --- Hash table --- */
    printf("\ntwo_sum_hash:\n");
    {
        int arr1[] = {2, 7, 11, 15};
        TEST("[2,7,11,15] target=9 -> (0,1)")
            EXPECT(two_sum_hash(arr1, 4, 9, &idx_i, &idx_j) == 1 &&
                   idx_i == 0 && idx_j == 1);

        int arr2[] = {3, 2, 4};
        TEST("[3,2,4] target=6 -> (1,2)")
            EXPECT(two_sum_hash(arr2, 3, 6, &idx_i, &idx_j) == 1 &&
                   idx_i == 1 && idx_j == 2);

        int arr3[] = {1, 5, -3, 8};
        TEST("[1,5,-3,8] target=2 -> (1,2)")
            EXPECT(two_sum_hash(arr3, 4, 2, &idx_i, &idx_j) == 1 &&
                   idx_i == 1 && idx_j == 2);

        int arr4[] = {1, 2, 3};
        TEST("[1,2,3] target=10 -> not found")
            EXPECT(two_sum_hash(arr4, 3, 10, &idx_i, &idx_j) == 0);
    }

    /* --- Two pointers (sorted) --- */
    printf("\ntwo_sum_sorted:\n");
    {
        int arr1[] = {2, 7, 11, 15};
        TEST("[2,7,11,15] target=9 -> (0,1)")
            EXPECT(two_sum_sorted(arr1, 4, 9, &idx_i, &idx_j) == 1 &&
                   idx_i == 0 && idx_j == 1);

        int arr2[] = {1, 3, 5, 7, 9};
        TEST("[1,3,5,7,9] target=12 -> (1,4)")
            EXPECT(two_sum_sorted(arr2, 5, 12, &idx_i, &idx_j) == 1 &&
                   idx_i == 1 && idx_j == 4);

        int arr3[] = {1, 2, 3, 4, 5};
        TEST("[1,2,3,4,5] target=100 -> not found")
            EXPECT(two_sum_sorted(arr3, 5, 100, &idx_i, &idx_j) == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
