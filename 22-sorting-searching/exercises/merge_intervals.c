/*
 * Kata: Merge Intervals
 *
 * Interval problems show up everywhere: calendar scheduling, IP range
 * merging, genomic data processing. The core pattern is always the same:
 * sort by start time, then scan linearly to merge overlaps.
 *
 * ASCII art — merging overlapping intervals on a number line:
 *
 *   Before merging:
 *     [1,3]     [2,6]         [8,10]          [15,18]
 *     |---|
 *       |--------|
 *                             |---|
 *                                              |---|
 *     1  2  3  4  5  6  7  8  9  10 11 ... 15 16 17 18
 *
 *   After merging:
 *     [1,6]                   [8,10]          [15,18]
 *     |-----------|
 *                             |---|
 *                                              |---|
 *     1  2  3  4  5  6  7  8  9  10 11 ... 15 16 17 18
 *
 *   The overlap condition: curr.start <= prev.end
 *   If they overlap, merge: prev.end = max(prev.end, curr.end)
 *
 * Exercises:
 *   - sort_intervals     (sort by start time)
 *   - merge_intervals    (merge overlapping intervals)
 *   - insert_interval    (insert into non-overlapping list, merge)
 *
 * 11 tests total.
 *
 * Hints:
 *   - You already know quicksort from sorting.c — adapt it for intervals
 *   - For merging: maintain a result array, compare each interval with
 *     the last interval in the result
 *   - For insert: find where the new interval overlaps, merge that range
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int start;
    int end;
} Interval;

/* ── Exercise 1: Sort Intervals by Start Time ──────────────────── *
 * Sort an array of intervals by their start value (ascending).
 * If start values are equal, sort by end value.
 *
 * Use any O(n log n) sort. Quicksort with Lomuto partition works
 * well — you already know how from sorting.c.
 */
void sort_intervals(Interval *intervals, int n)
{
    /* TODO: Sort intervals by start (then by end if tied) */
    (void)intervals; (void)n;
}

/* ── Exercise 2: Merge Overlapping Intervals ───────────────────── *
 * Given a list of intervals, merge all overlapping intervals.
 * Return the merged intervals in result[] and set *result_len.
 *
 * Algorithm:
 *   1. Sort intervals by start time
 *   2. Initialize result with the first interval
 *   3. For each remaining interval:
 *      - If it overlaps with the last result interval, merge them
 *      - Otherwise, add it as a new interval
 *
 * The caller provides result[] with enough space (worst case = n).
 *
 * Example:
 *   [[1,3],[2,6],[8,10],[15,18]] → [[1,6],[8,10],[15,18]]
 */
void merge_intervals(Interval *intervals, int n,
                     Interval *result, int *result_len)
{
    /* TODO: Sort, then linear scan to merge overlaps */
    (void)intervals; (void)n; (void)result; (void)result_len;
    *result_len = 0;
}

/* ── Exercise 3: Insert Interval ───────────────────────────────── *
 * Given a SORTED, NON-OVERLAPPING list of intervals and a new
 * interval to insert, produce a new merged list.
 *
 * Algorithm:
 *   1. Add all intervals that end before new_interval starts
 *   2. Merge all intervals that overlap with new_interval
 *   3. Add all intervals that start after new_interval ends
 *
 * The caller provides result[] with enough space (worst case = n+1).
 *
 * Example:
 *   intervals: [[1,3],[6,9]], new: [2,5]
 *   result: [[1,5],[6,9]]
 */
void insert_interval(const Interval *intervals, int n,
                     Interval new_interval,
                     Interval *result, int *result_len)
{
    /* TODO: Three-phase scan: before, overlap, after */
    (void)intervals; (void)n; (void)new_interval;
    (void)result; (void)result_len;
    *result_len = 0;
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

/* Helper: check if interval arrays match */
static int intervals_eq(const Interval *a, const Interval *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i].start != b[i].start || a[i].end != b[i].end) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    printf("=== Merge Intervals ===\n\n");

    /* --- sort_intervals --- */
    printf("sort_intervals:\n");
    {
        Interval arr[] = {{3, 5}, {1, 2}, {4, 7}, {1, 1}};
        Interval exp[] = {{1, 1}, {1, 2}, {3, 5}, {4, 7}};
        sort_intervals(arr, 4);
        TEST("sort four intervals") EXPECT(intervals_eq(arr, exp, 4));
    }
    {
        Interval arr[] = {{5, 10}, {1, 3}};
        Interval exp[] = {{1, 3}, {5, 10}};
        sort_intervals(arr, 2);
        TEST("sort two intervals") EXPECT(intervals_eq(arr, exp, 2));
    }

    /* --- merge_intervals --- */
    printf("\nmerge_intervals:\n");
    {
        Interval arr[] = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
        Interval exp[] = {{1, 6}, {8, 10}, {15, 18}};
        Interval result[4];
        int rlen;
        merge_intervals(arr, 4, result, &rlen);
        TEST("[[1,3],[2,6],[8,10],[15,18]]") EXPECT(rlen == 3 && intervals_eq(result, exp, 3));
    }
    {
        Interval arr[] = {{1, 4}, {4, 5}};
        Interval exp[] = {{1, 5}};
        Interval result[2];
        int rlen;
        merge_intervals(arr, 2, result, &rlen);
        TEST("[[1,4],[4,5]] touching → merge") EXPECT(rlen == 1 && intervals_eq(result, exp, 1));
    }
    {
        Interval arr[] = {{1, 2}, {3, 4}, {5, 6}};
        Interval exp[] = {{1, 2}, {3, 4}, {5, 6}};
        Interval result[3];
        int rlen;
        merge_intervals(arr, 3, result, &rlen);
        TEST("no overlaps → unchanged") EXPECT(rlen == 3 && intervals_eq(result, exp, 3));
    }
    {
        Interval arr[] = {{1, 10}, {2, 3}, {4, 5}, {6, 7}};
        Interval exp[] = {{1, 10}};
        Interval result[4];
        int rlen;
        merge_intervals(arr, 4, result, &rlen);
        TEST("all contained in first") EXPECT(rlen == 1 && intervals_eq(result, exp, 1));
    }
    {
        Interval arr[] = {{5, 8}};
        Interval exp[] = {{5, 8}};
        Interval result[1];
        int rlen;
        merge_intervals(arr, 1, result, &rlen);
        TEST("single interval") EXPECT(rlen == 1 && intervals_eq(result, exp, 1));
    }

    /* --- insert_interval --- */
    printf("\ninsert_interval:\n");
    {
        Interval arr[] = {{1, 3}, {6, 9}};
        Interval exp[] = {{1, 5}, {6, 9}};
        Interval result[3];
        int rlen;
        insert_interval(arr, 2, (Interval){2, 5}, result, &rlen);
        TEST("insert [2,5] into [[1,3],[6,9]]") EXPECT(rlen == 2 && intervals_eq(result, exp, 2));
    }
    {
        Interval arr[] = {{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}};
        Interval exp[] = {{1, 2}, {3, 10}, {12, 16}};
        Interval result[6];
        int rlen;
        insert_interval(arr, 5, (Interval){4, 8}, result, &rlen);
        TEST("insert [4,8] merges three") EXPECT(rlen == 3 && intervals_eq(result, exp, 3));
    }
    {
        Interval arr[] = {{3, 5}, {8, 10}};
        Interval exp[] = {{1, 2}, {3, 5}, {8, 10}};
        Interval result[3];
        int rlen;
        insert_interval(arr, 2, (Interval){1, 2}, result, &rlen);
        TEST("insert [1,2] before all") EXPECT(rlen == 3 && intervals_eq(result, exp, 3));
    }
    {
        Interval arr[] = {{1, 3}, {5, 7}};
        Interval exp[] = {{1, 3}, {5, 7}, {9, 11}};
        Interval result[3];
        int rlen;
        insert_interval(arr, 2, (Interval){9, 11}, result, &rlen);
        TEST("insert [9,11] after all") EXPECT(rlen == 3 && intervals_eq(result, exp, 3));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
