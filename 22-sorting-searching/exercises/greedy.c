/*
 * Kata: Greedy Algorithms
 *
 * The greedy approach: at each step, make the choice that looks best
 * RIGHT NOW, without worrying about the future. Sometimes this gives
 * the optimal answer (and we can prove it). Sometimes it doesn't
 * (and we need dynamic programming instead).
 *
 * When greedy works, the problem has two properties:
 *   1. Greedy choice property — a locally optimal choice leads to
 *      a globally optimal solution
 *   2. Optimal substructure — the optimal solution contains optimal
 *      solutions to subproblems
 *
 * Functions to implement:
 *   - activity_selection  (pick max non-overlapping activities)
 *   - jump_game           (can you reach the end?)
 *   - jump_game_min       (minimum jumps to reach the end)
 *   - task_scheduler      (minimum time with cooldown)
 *   - gas_station         (find starting station for circular trip)
 *
 * 19 tests total.
 *
 *
 * ── Activity Selection ────────────────────────────────────────────
 *
 * Greedy: always pick the activity that ENDS earliest.
 * Why? It leaves the most room for future activities.
 *
 *   Timeline:
 *   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
 *   |  [====]                                            (1,4)  *
 *   |     [====]                                         (3,5)
 *   |  [=========]                                       (0,6)
 *   |              [==]                                  (5,7)  *
 *   |     [==========]                                   (3,9)
 *   |              [======]                              (5,9)
 *   |                 [======]                           (6,10)
 *   |                       [====]                       (8,11) *
 *   |                       [======]                     (8,12)
 *   |  [========================]                        (2,14)
 *   |                                   [======]         (12,16)*
 *
 *   Greedy picks: (1,4), (5,7), (8,11), (12,16) = 4 activities
 *
 *
 * ── Jump Game ─────────────────────────────────────────────────────
 *
 * Track the FARTHEST position reachable at each step.
 *
 *   arr = [2, 3, 1, 1, 4]
 *
 *   Index:    0    1    2    3    4
 *   Value:    2    3    1    1    4
 *   Reach:    2    4    3    4    8
 *             ^    ^              ^
 *             |    |              goal reached!
 *             |    farthest = max(2, 1+3) = 4
 *             farthest = 0+2 = 2
 *
 *   arr = [3, 2, 1, 0, 4]
 *
 *   Index:    0    1    2    3    4
 *   Value:    3    2    1    0    4
 *   Reach:    3    3    3    3    -
 *                            ^
 *                            stuck! farthest = 3, can't pass index 3
 *
 *
 * ── Task Scheduler ────────────────────────────────────────────────
 *
 * Always do the most frequent task first. Fill cooldown slots with
 * other tasks or idle time.
 *
 *   Tasks: [A,A,A,B,B,B], cooldown n=2
 *
 *   Slot:   1    2    3    4    5    6    7    8
 *   Task:   A    B   idle   A    B   idle   A    B
 *
 *   The most frequent task (A or B, count=3) determines the frame.
 *   Frames = (max_count - 1) = 2 frames of size (n+1) = 3
 *   Plus final partial frame with all max-frequency tasks.
 *   Total = (max_count - 1) * (n + 1) + count_of_max_freq_tasks
 *         = 2 * 3 + 2 = 8
 *
 *
 * ── Gas Station ───────────────────────────────────────────────────
 *
 * N stations in a circle. gas[i] = fuel gained, cost[i] = fuel to next.
 *
 *   Station:   0     1     2     3     4
 *   Gas:       1     2     3     4     5
 *   Cost:      3     4     5     1     2
 *   Balance:  -2    -2    -2    +3    +3
 *
 *   Key insight: if total gas >= total cost, a solution exists.
 *   If you run out at station j starting from i, then no station
 *   between i and j can work either. So skip to j+1.
 *
 *   Start at 3: tank=0 +4-1=3, +5-2=6, +1-3=4, +2-4=2, +3-5=0 ✓
 *
 *
 * Hints:
 *   - activity_selection: sort by end time first, then greedily pick
 *   - jump_game: iterate, tracking max reachable index
 *   - jump_game_min: BFS-like — track current "level" end and next farthest
 *   - task_scheduler: count frequencies, find max, count how many have max
 *   - gas_station: single pass tracking running tank and total balance
 */

#include <stdio.h>
#include <string.h>

/* ── Helper: simple sort for activity end times ────────────────── */
typedef struct {
    int start;
    int end;
} Activity;

/* ── Exercise 1: Activity Selection ──────────────────────────────
 * Given n activities with start/end times, select the maximum number
 * of non-overlapping activities.
 *
 * Strategy:
 *   1. Sort activities by end time (ascending)
 *   2. Pick the first activity
 *   3. For each remaining activity: if its start >= last picked end,
 *      pick it
 *
 * Return the count of selected activities.
 * Store the selected activities in 'selected' array (indices into
 * the sorted array). Set *sel_count to the number selected.
 */
int activity_selection(Activity *acts, int n, int selected[], int *sel_count)
{
    /* TODO: sort by end time, then greedily pick non-overlapping activities */
    (void)acts; (void)n; (void)selected; (void)sel_count;
    return 0; /* placeholder — return count of selected */
}

/* ── Exercise 2: Jump Game ───────────────────────────────────────
 * Given arr[i] = max jump length from position i,
 * return 1 if you can reach the last index, 0 otherwise.
 *
 * Greedy: track the farthest reachable position.
 *   farthest = 0
 *   for i = 0 to n-1:
 *       if i > farthest: return 0  (stuck!)
 *       farthest = max(farthest, i + arr[i])
 *   return 1
 */
int jump_game(const int *arr, int n)
{
    /* TODO: track farthest reachable position */
    (void)arr; (void)n;
    return 0; /* placeholder */
}

/* ── Exercise 3: Jump Game — Minimum Jumps ───────────────────────
 * Return the minimum number of jumps to reach the last index.
 * Assume the answer always exists (you can always reach the end).
 *
 * BFS-like greedy:
 *   - jumps = 0, current_end = 0, farthest = 0
 *   - for i = 0 to n-2:
 *       farthest = max(farthest, i + arr[i])
 *       if i == current_end:  (reached end of current "level")
 *           jumps++
 *           current_end = farthest
 *   - return jumps
 */
int jump_game_min(const int *arr, int n)
{
    /* TODO: BFS-like greedy for minimum jumps */
    (void)arr; (void)n;
    return 0; /* placeholder */
}

/* ── Exercise 4: Task Scheduler ──────────────────────────────────
 * Given an array of task characters (A-Z) and a cooldown period n,
 * return the minimum number of time slots to complete all tasks.
 *
 * Between two same tasks, there must be at least n slots.
 * You can insert idle slots or do different tasks in between.
 *
 * Greedy formula:
 *   1. Count frequency of each task
 *   2. Find max_freq (the highest frequency)
 *   3. Count num_max (how many tasks have that max frequency)
 *   4. result = (max_freq - 1) * (n + 1) + num_max
 *   5. But if we have enough different tasks, no idle needed:
 *      return max(result, total_tasks)
 */
int task_scheduler(const char *tasks, int num_tasks, int n)
{
    /* TODO: compute minimum time slots using frequency analysis */
    (void)tasks; (void)num_tasks; (void)n;
    return 0; /* placeholder */
}

/* ── Exercise 5: Gas Station ─────────────────────────────────────
 * N gas stations in a circle.
 *   gas[i]  = fuel gained at station i
 *   cost[i] = fuel needed to travel from station i to i+1
 *
 * Find the starting station index to complete the circuit.
 * Return -1 if impossible.
 *
 * Greedy insight:
 *   - If total gas < total cost → impossible (-1)
 *   - Track running tank. If tank goes negative at station i,
 *     then no station from start..i can work. Reset start to i+1.
 */
int gas_station(const int *gas, const int *cost, int n)
{
    /* TODO: single-pass greedy to find starting station */
    (void)gas; (void)cost; (void)n;
    return -1; /* placeholder */
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
