/*
 * Solution: Merge Intervals
 *
 * Three exercises: sort intervals, merge overlapping, insert and merge.
 *
 * 11 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int start;
    int end;
} Interval;

/* ── Helper: swap two intervals ────────────────────────────────── */
static void interval_swap(Interval *a, Interval *b)
{
    Interval tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ── Helper: compare intervals (by start, then by end) ─────────── */
static int interval_less(const Interval *a, const Interval *b)
{
    if (a->start != b->start) {
        return a->start < b->start;
    }
    return a->end < b->end;
}

/* ── Sort Intervals (Quicksort, Lomuto partition) ──────────────── */
static int partition_intervals(Interval *arr, int lo, int hi)
{
    Interval pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (interval_less(&arr[j], &pivot)) {
            i++;
            interval_swap(&arr[i], &arr[j]);
        }
    }
    interval_swap(&arr[i + 1], &arr[hi]);
    return i + 1;
}

static void qsort_intervals(Interval *arr, int lo, int hi)
{
    if (lo >= hi) {
        return;
    }
    int p = partition_intervals(arr, lo, hi);
    qsort_intervals(arr, lo, p - 1);
    qsort_intervals(arr, p + 1, hi);
}

void sort_intervals(Interval *intervals, int n)
{
    if (n <= 1) {
        return;
    }
    qsort_intervals(intervals, 0, n - 1);
}

/* ── Merge Overlapping Intervals ───────────────────────────────── */
void merge_intervals(Interval *intervals, int n,
                     Interval *result, int *result_len)
{
    if (n == 0) {
        *result_len = 0;
        return;
    }

    /* Sort by start time first */
    sort_intervals(intervals, n);

    /* Start with the first interval */
    result[0] = intervals[0];
    int count = 1;

    for (int i = 1; i < n; i++) {
        /* Does this interval overlap with the last one in result? */
        if (intervals[i].start <= result[count - 1].end) {
            /* Merge: extend the end if needed */
            if (intervals[i].end > result[count - 1].end) {
                result[count - 1].end = intervals[i].end;
            }
        } else {
            /* No overlap: add as new interval */
            result[count] = intervals[i];
            count++;
        }
    }
    *result_len = count;
}

/* ── Insert Interval ───────────────────────────────────────────── */
void insert_interval(const Interval *intervals, int n,
                     Interval new_interval,
                     Interval *result, int *result_len)
{
    int count = 0;
    int i = 0;

    /* Phase 1: add all intervals that end before new_interval starts */
    while (i < n && intervals[i].end < new_interval.start) {
        result[count++] = intervals[i];
        i++;
    }

    /* Phase 2: merge all intervals that overlap with new_interval */
    while (i < n && intervals[i].start <= new_interval.end) {
        if (intervals[i].start < new_interval.start) {
            new_interval.start = intervals[i].start;
        }
        if (intervals[i].end > new_interval.end) {
            new_interval.end = intervals[i].end;
        }
        i++;
    }
    result[count++] = new_interval;

    /* Phase 3: add all remaining intervals */
    while (i < n) {
        result[count++] = intervals[i];
        i++;
    }
    *result_len = count;
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
