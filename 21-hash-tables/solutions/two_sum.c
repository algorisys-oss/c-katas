/*
 * Solution: Two Sum — Three Ways
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Brute Force: O(n^2) ─────────────────────────────────────────── */

int two_sum_brute(const int *arr, int n, int target, int *out_i, int *out_j)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] + arr[j] == target) {
                *out_i = i;
                *out_j = j;
                return 1;
            }
        }
    }
    return 0;
}

/* ── Hash Table: O(n) ─────────────────────────────────────────────── */

#define HT_SIZE 1024

typedef struct {
    int keys[HT_SIZE];
    int values[HT_SIZE];
    int occupied[HT_SIZE];
} IntHashTable;

static void iht_init(IntHashTable *ht)
{
    memset(ht->occupied, 0, sizeof(ht->occupied));
}

static int iht_hash(int key)
{
    unsigned int k = (unsigned int)key;
    return (int)(k % HT_SIZE);
}

static void iht_put(IntHashTable *ht, int key, int value)
{
    int idx = iht_hash(key);
    /* Linear probing to find an empty slot */
    for (int i = 0; i < HT_SIZE; i++) {
        int probe = (idx + i) % HT_SIZE;
        if (!ht->occupied[probe] || ht->keys[probe] == key) {
            ht->keys[probe] = key;
            ht->values[probe] = value;
            ht->occupied[probe] = 1;
            return;
        }
    }
}

static int iht_get(const IntHashTable *ht, int key, int *out_value)
{
    int idx = iht_hash(key);
    /* Linear probing to find the key */
    for (int i = 0; i < HT_SIZE; i++) {
        int probe = (idx + i) % HT_SIZE;
        if (!ht->occupied[probe]) {
            return 0; /* empty slot — key not in table */
        }
        if (ht->keys[probe] == key) {
            *out_value = ht->values[probe];
            return 1;
        }
    }
    return 0;
}

int two_sum_hash(const int *arr, int n, int target, int *out_i, int *out_j)
{
    IntHashTable ht;
    iht_init(&ht);

    for (int i = 0; i < n; i++) {
        int complement = target - arr[i];
        int prev_idx;

        /* Check if complement was seen in a previous iteration */
        if (iht_get(&ht, complement, &prev_idx)) {
            *out_i = prev_idx;
            *out_j = i;
            return 1;
        }

        /* Store current value -> index for future lookups */
        iht_put(&ht, arr[i], i);
    }

    return 0;
}

/* ── Two Pointers (sorted): O(n) time, O(1) space ────────────────── */

int two_sum_sorted(const int *arr, int n, int target, int *out_i, int *out_j)
{
    int left = 0;
    int right = n - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            *out_i = left;
            *out_j = right;
            return 1;
        } else if (sum < target) {
            left++;  /* Need a bigger sum — move left pointer right */
        } else {
            right--; /* Need a smaller sum — move right pointer left */
        }
    }

    return 0;
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
    printf("=== Two Sum — Three Ways (Solution) ===\n\n");

    int idx_i, idx_j;

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
