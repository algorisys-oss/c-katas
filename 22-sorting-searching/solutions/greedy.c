/*
 * Solution: Greedy Algorithms
 *
 * Five classic greedy problems demonstrating the "take the best now"
 * strategy and when it provably gives optimal solutions.
 *
 * 19 tests total.
 */

#include <stdio.h>
#include <string.h>

typedef struct {
    int start;
    int end;
} Activity;

/* ── Helper: sort activities by end time (insertion sort) ──────── */
static void sort_by_end(Activity *acts, int n)
{
    for (int i = 1; i < n; i++) {
        Activity key = acts[i];
        int j = i - 1;
        while (j >= 0 && acts[j].end > key.end) {
            acts[j + 1] = acts[j];
            j--;
        }
        acts[j + 1] = key;
    }
}

/* ── Activity Selection ──────────────────────────────────────────── */
int activity_selection(Activity *acts, int n, int selected[], int *sel_count)
{
    sort_by_end(acts, n);

    *sel_count = 0;
    /* Always pick the first activity (earliest end time) */
    selected[*sel_count] = 0;
    (*sel_count)++;
    int last_end = acts[0].end;

    for (int i = 1; i < n; i++) {
        if (acts[i].start >= last_end) {
            selected[*sel_count] = i;
            (*sel_count)++;
            last_end = acts[i].end;
        }
    }

    return *sel_count;
}

/* ── Jump Game ──────────────────────────────────────────────────── */
int jump_game(const int *arr, int n)
{
    int farthest = 0;
    for (int i = 0; i < n; i++) {
        if (i > farthest) {
            return 0;  /* can't reach index i */
        }
        if (i + arr[i] > farthest) {
            farthest = i + arr[i];
        }
    }
    return 1;
}

/* ── Jump Game — Minimum Jumps ──────────────────────────────────── */
int jump_game_min(const int *arr, int n)
{
    if (n <= 1) {
        return 0;
    }
    int jumps = 0;
    int current_end = 0;
    int farthest = 0;

    for (int i = 0; i < n - 1; i++) {
        if (i + arr[i] > farthest) {
            farthest = i + arr[i];
        }
        if (i == current_end) {
            /* Must jump — reached the end of current BFS "level" */
            jumps++;
            current_end = farthest;
        }
    }
    return jumps;
}

/* ── Task Scheduler ─────────────────────────────────────────────── */
int task_scheduler(const char *tasks, int num_tasks, int n)
{
    /* Count frequency of each task (A-Z = 26 slots) */
    int freq[26];
    memset(freq, 0, sizeof(freq));
    for (int i = 0; i < num_tasks; i++) {
        freq[tasks[i] - 'A']++;
    }

    /* Find max frequency */
    int max_freq = 0;
    for (int i = 0; i < 26; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
        }
    }

    /* Count how many tasks share the max frequency */
    int num_max = 0;
    for (int i = 0; i < 26; i++) {
        if (freq[i] == max_freq) {
            num_max++;
        }
    }

    /* Formula: (max_freq - 1) frames of size (n + 1), plus the final
     * partial frame with num_max tasks */
    int result = (max_freq - 1) * (n + 1) + num_max;

    /* If we have many different tasks, we might not need idle slots
     * at all — in that case the answer is just total tasks */
    if (result < num_tasks) {
        result = num_tasks;
    }

    return result;
}

/* ── Gas Station ─────────────────────────────────────────────────── */
int gas_station(const int *gas, const int *cost, int n)
{
    int total_tank = 0;
    int current_tank = 0;
    int start = 0;

    for (int i = 0; i < n; i++) {
        int balance = gas[i] - cost[i];
        total_tank += balance;
        current_tank += balance;

        if (current_tank < 0) {
            /* Can't reach station i+1 from current start.
             * No station between start and i can work either.
             * Try starting from i+1. */
            start = i + 1;
            current_tank = 0;
        }
    }

    /* If total gas < total cost, no solution exists */
    return total_tank >= 0 ? start : -1;
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
    printf("=== Greedy Algorithms ===\n\n");

    /* --- activity_selection tests --- */
    printf("activity_selection:\n");
    {
        Activity acts[] = {
            {1,4}, {3,5}, {0,6}, {5,7}, {3,9},
            {5,9}, {6,10}, {8,11}, {8,12}, {2,14}, {12,16}
        };
        int selected[11];
        int sel_count = 0;
        int count = activity_selection(acts, 11, selected, &sel_count);
        TEST("classic example → 4 activities")
            EXPECT(count == 4);
    }
    {
        Activity acts[] = {{1,2}, {3,4}, {5,6}};
        int selected[3];
        int sel_count = 0;
        int count = activity_selection(acts, 3, selected, &sel_count);
        TEST("non-overlapping → all selected")
            EXPECT(count == 3);
    }
    {
        Activity acts[] = {{1,10}, {2,3}, {4,5}, {6,7}};
        int selected[4];
        int sel_count = 0;
        int count = activity_selection(acts, 4, selected, &sel_count);
        TEST("one long vs three short → 3")
            EXPECT(count == 3);
    }
    {
        Activity acts[] = {{0,5}};
        int selected[1];
        int sel_count = 0;
        int count = activity_selection(acts, 1, selected, &sel_count);
        TEST("single activity → 1")
            EXPECT(count == 1);
    }

    /* --- jump_game tests --- */
    printf("\njump_game:\n");
    {
        int arr[] = {2, 3, 1, 1, 4};
        TEST("[2,3,1,1,4] → reachable")
            EXPECT(jump_game(arr, 5) == 1);
    }
    {
        int arr[] = {3, 2, 1, 0, 4};
        TEST("[3,2,1,0,4] → stuck at index 3")
            EXPECT(jump_game(arr, 5) == 0);
    }
    {
        int arr[] = {0};
        TEST("[0] → already at end")
            EXPECT(jump_game(arr, 1) == 1);
    }
    {
        int arr[] = {1, 0, 1};
        TEST("[1,0,1] → stuck at index 1")
            EXPECT(jump_game(arr, 3) == 0);
    }

    /* --- jump_game_min tests --- */
    printf("\njump_game_min:\n");
    {
        int arr[] = {2, 3, 1, 1, 4};
        TEST("[2,3,1,1,4] → 2 jumps")
            EXPECT(jump_game_min(arr, 5) == 2);
    }
    {
        int arr[] = {2, 3, 0, 1, 4};
        TEST("[2,3,0,1,4] → 2 jumps")
            EXPECT(jump_game_min(arr, 5) == 2);
    }
    {
        int arr[] = {1, 1, 1, 1};
        TEST("[1,1,1,1] → 3 jumps")
            EXPECT(jump_game_min(arr, 4) == 3);
    }

    /* --- task_scheduler tests --- */
    printf("\ntask_scheduler:\n");
    {
        char tasks[] = {'A','A','A','B','B','B'};
        TEST("AAABBB, n=2 → 8")
            EXPECT(task_scheduler(tasks, 6, 2) == 8);
    }
    {
        char tasks[] = {'A','A','A','B','B','B'};
        TEST("AAABBB, n=0 → 6")
            EXPECT(task_scheduler(tasks, 6, 0) == 6);
    }
    {
        char tasks[] = {'A','A','A','A','A','A','B','C','D','E','F','G'};
        TEST("AAAAAABCDEFG, n=2 → 16")
            EXPECT(task_scheduler(tasks, 12, 2) == 16);
    }
    {
        char tasks[] = {'A','B','C','D','E','F'};
        TEST("all different, n=2 → 6")
            EXPECT(task_scheduler(tasks, 6, 2) == 6);
    }

    /* --- gas_station tests --- */
    printf("\ngas_station:\n");
    {
        int gas[]  = {1, 2, 3, 4, 5};
        int cost[] = {3, 4, 5, 1, 2};
        TEST("classic example → start at 3")
            EXPECT(gas_station(gas, cost, 5) == 3);
    }
    {
        int gas[]  = {2, 3, 4};
        int cost[] = {3, 4, 3};
        TEST("impossible → -1")
            EXPECT(gas_station(gas, cost, 3) == -1);
    }
    {
        int gas[]  = {5, 1, 2, 3, 4};
        int cost[] = {4, 4, 1, 5, 1};
        TEST("start at 4")
            EXPECT(gas_station(gas, cost, 5) == 4);
    }
    {
        int gas[]  = {3};
        int cost[] = {2};
        TEST("single station → 0")
            EXPECT(gas_station(gas, cost, 1) == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
