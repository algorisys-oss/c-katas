/*
 * Kata: Memoization & Dynamic Programming
 *
 * In the fibonacci kata, you saw how naive recursion does redundant work:
 * fib(5) calls fib(3) twice, fib(2) three times, fib(1) five times...
 * The call tree EXPLODES because we keep recomputing the same answers.
 *
 * Memoization fixes this with one simple idea:
 *   "Before computing something, check if you already know the answer.
 *    If yes, return it immediately. If no, compute it and SAVE the result."
 *
 * That's it. You add a cache (usually an array) and two lines of logic:
 *   1. At the start: if cache[n] != UNSET, return cache[n]
 *   2. At the end:   cache[n] = result, then return result
 *
 * This turns O(2^n) into O(n) — each subproblem is solved ONCE.
 *
 * Two styles of dynamic programming (DP):
 *
 *   TOP-DOWN (memoization):
 *     - Start from the big problem, recurse into subproblems
 *     - Cache results as you go
 *     - Still uses recursion — natural if you already have the recursive solution
 *
 *   BOTTOM-UP (tabulation):
 *     - Start from the smallest subproblems, build up to the answer
 *     - Fill a table iteratively — no recursion, no stack overflow risk
 *     - Often uses less memory (you might only need the last few values)
 *
 * When does DP apply? Two conditions:
 *   1. OVERLAPPING SUBPROBLEMS — the same subproblem appears multiple times
 *   2. OPTIMAL SUBSTRUCTURE — the optimal solution contains optimal sub-solutions
 *
 * Exercises:
 *   1. fib_memo(n)                — Fibonacci with memoization
 *   2. climb_stairs(n)            — Stair climbing (secretly Fibonacci!)
 *   3. coin_change(coins, n, amt) — Minimum coins to make an amount
 *   4. lcs_length(a, b)           — Longest common subsequence length
 *
 * 17 tests total.
 */

#include <stdio.h>
#include <string.h>

#define MAX_N 100
#define UNSET -1

/* ── Exercise 1: fib_memo ───────────────────────────────────────────
 *
 * You already wrote naive fib — it was O(2^n) because of redundant calls:
 *
 *                    fib(5)
 *                   /      \
 *              fib(4)       fib(3)        <-- fib(3) computed TWICE
 *             /     \       /     \
 *         fib(3)  fib(2)  fib(2) fib(1)  <-- fib(2) computed THREE times
 *         /   \
 *     fib(2) fib(1)
 *
 * With memoization, the FIRST time you compute fib(3), you store the
 * result in cache[3]. The SECOND time fib(3) is called, you just return
 * cache[3] immediately — no recursion needed.
 *
 * Use a cache array of size MAX_N, initialized to UNSET (-1).
 * The caller initializes the cache; your function just uses it.
 *
 * Steps:
 *   1. If n <= 1, return n (base case — same as before)
 *   2. If cache[n] != UNSET, return cache[n] (already computed!)
 *   3. Otherwise, compute fib(n-1) + fib(n-2) recursively
 *   4. Store the result in cache[n]
 *   5. Return the result
 */
int fib_memo(int n, int cache[])
{
    /* TODO: Base case — return n if n <= 1 */

    /* TODO: Check cache — if cache[n] != UNSET, return it */

    /* TODO: Compute recursively, store in cache[n], return result */

    return 0;  /* placeholder */
}

/* ── Exercise 2: climb_stairs ───────────────────────────────────────
 *
 * You are climbing a staircase with n steps. Each time you can take
 * either 1 step or 2 steps. How many distinct ways can you reach the top?
 *
 * Think about it from the top: to reach step n, you either came from
 * step n-1 (took 1 step) or step n-2 (took 2 steps). So:
 *
 *   ways(n) = ways(n-1) + ways(n-2)
 *
 * Wait — that IS the Fibonacci recurrence! With base cases:
 *   ways(0) = 1  (one way to stay at the ground: do nothing)
 *   ways(1) = 1  (one way to reach step 1: take one step)
 *   ways(2) = 2  (1+1 or 2)
 *   ways(3) = 3  (1+1+1, 1+2, 2+1)
 *
 * Use memoization with a cache array (caller initializes it).
 *
 * Steps:
 *   1. If n <= 1, return 1 (base case)
 *   2. If cache[n] != UNSET, return cache[n]
 *   3. Compute climb_stairs(n-1) + climb_stairs(n-2)
 *   4. Store in cache[n] and return
 */
int climb_stairs(int n, int cache[])
{
    /* TODO: Base case — return 1 if n <= 1 */

    /* TODO: Check cache */

    /* TODO: Recurse, store, return */

    return 0;  /* placeholder */
}

/* ── Exercise 3: coin_change ────────────────────────────────────────
 *
 * Given coin denominations and an amount, find the MINIMUM number of
 * coins needed to make that amount. If impossible, return -1.
 *
 * Example: coins = {1, 5, 10, 25}, amount = 30
 *   Answer: 2 (one 25-cent + one 5-cent)
 *
 * Key insight: for each coin c, if we use it, we need 1 + min_coins(amount - c).
 * Try every coin and take the minimum:
 *
 *   min_coins(amount) = 1 + min( min_coins(amount - c) for each coin c )
 *
 * Base cases:
 *   amount == 0 → 0 coins needed
 *   amount < 0  → impossible (-1)
 *
 * This exercise uses BOTTOM-UP tabulation (no recursion):
 *   - Create a table dp[] of size (amount + 1)
 *   - dp[0] = 0 (zero coins for zero amount)
 *   - dp[i] = amount + 1 initially (a value larger than any valid answer)
 *   - For each amount i from 1 to amount:
 *       For each coin c:
 *         if c <= i and dp[i - c] + 1 < dp[i]:
 *           dp[i] = dp[i - c] + 1
 *   - If dp[amount] > amount, return -1 (impossible)
 *   - Otherwise return dp[amount]
 *
 * coins[] is the array of denominations, num_coins is its length.
 */
int coin_change(int coins[], int num_coins, int amount)
{
    /* TODO: Handle edge case — amount is 0 → return 0 */

    /* TODO: Create dp array of size (amount + 1) */

    /* TODO: Initialize dp[0] = 0, all others to amount + 1 */

    /* TODO: For each amount i from 1 to amount:
     *         For each coin c in coins[]:
     *           if c <= i and dp[i - c] + 1 < dp[i]:
     *             dp[i] = dp[i - c] + 1
     */

    /* TODO: If dp[amount] > amount, return -1; else return dp[amount] */

    return -1;  /* placeholder */
}

/* ── Exercise 4: lcs_length ─────────────────────────────────────────
 *
 * Given two strings a and b, find the length of their Longest Common
 * Subsequence (LCS). A subsequence is formed by deleting some (or no)
 * characters without changing the order of remaining characters.
 *
 * Example: a = "ABCDE", b = "ACE"
 *   LCS is "ACE" → length 3
 *
 * This is classic 2D dynamic programming. Build a table dp[i][j] where:
 *   dp[i][j] = LCS length of a[0..i-1] and b[0..j-1]
 *
 * Rules:
 *   - If a[i-1] == b[j-1]: dp[i][j] = dp[i-1][j-1] + 1
 *     (the matching character extends the LCS)
 *   - Else: dp[i][j] = max(dp[i-1][j], dp[i][j-1])
 *     (skip one character from a, or one from b — take the better option)
 *   - Base case: dp[0][j] = dp[i][0] = 0
 *     (empty string has LCS length 0 with anything)
 *
 * Use bottom-up tabulation. Strings are limited to 100 chars.
 *
 * Steps:
 *   1. Get lengths len_a = strlen(a), len_b = strlen(b)
 *   2. Create dp[MAX_N + 1][MAX_N + 1] and zero it
 *   3. Fill the table row by row using the rules above
 *   4. Return dp[len_a][len_b]
 */
int lcs_length(const char *a, const char *b)
{
    /* TODO: Get string lengths */

    /* TODO: Create and zero the 2D dp table */

    /* TODO: Fill the table:
     *   for i from 1 to len_a:
     *     for j from 1 to len_b:
     *       if a[i-1] == b[j-1]: dp[i][j] = dp[i-1][j-1] + 1
     *       else: dp[i][j] = max of dp[i-1][j] and dp[i][j-1]
     */

    /* TODO: Return dp[len_a][len_b] */

    return 0;  /* placeholder */
}

/* ── Test Harness ────────────────────────────────────────────────── */

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
    printf("=== Memoization & Dynamic Programming ===\n\n");

    /* ── fib_memo tests ──────────────────────────────────────── */
    printf("fib_memo:\n");
    {
        int cache[MAX_N];
        memset(cache, UNSET, sizeof(cache));
        /* Note: memset sets bytes to 0xFF, which for int gives -1 on
         * two's complement systems. UNSET is -1, so this works. */

        TEST("fib_memo(0) = 0") EXPECT(fib_memo(0, cache) == 0);
        TEST("fib_memo(1) = 1") EXPECT(fib_memo(1, cache) == 1);
        TEST("fib_memo(10) = 55") EXPECT(fib_memo(10, cache) == 55);
        TEST("fib_memo(20) = 6765") EXPECT(fib_memo(20, cache) == 6765);
    }

    /* ── climb_stairs tests ──────────────────────────────────── */
    printf("\nclimb_stairs:\n");
    {
        int cache[MAX_N];
        memset(cache, UNSET, sizeof(cache));

        TEST("climb_stairs(0) = 1") EXPECT(climb_stairs(0, cache) == 1);
        TEST("climb_stairs(1) = 1") EXPECT(climb_stairs(1, cache) == 1);
        TEST("climb_stairs(3) = 3") EXPECT(climb_stairs(3, cache) == 3);
        TEST("climb_stairs(5) = 8") EXPECT(climb_stairs(5, cache) == 8);
    }

    /* ── coin_change tests ───────────────────────────────────── */
    printf("\ncoin_change:\n");
    {
        int coins[] = {1, 5, 10, 25};
        int n = 4;

        TEST("coin_change(0) = 0") EXPECT(coin_change(coins, n, 0) == 0);
        TEST("coin_change(1) = 1") EXPECT(coin_change(coins, n, 1) == 1);
        TEST("coin_change(6) = 2 (5+1)") EXPECT(coin_change(coins, n, 6) == 2);
        TEST("coin_change(30) = 2 (25+5)") EXPECT(coin_change(coins, n, 30) == 2);
        TEST("coin_change(43) = 6 (25+10+5+1+1+1)") EXPECT(coin_change(coins, n, 43) == 6);
    }

    /* ── lcs_length tests ────────────────────────────────────── */
    printf("\nlcs_length:\n");
    {
        TEST("lcs('ABCDE','ACE') = 3") EXPECT(lcs_length("ABCDE", "ACE") == 3);
        TEST("lcs('ABC','ABC') = 3") EXPECT(lcs_length("ABC", "ABC") == 3);
        TEST("lcs('ABC','DEF') = 0") EXPECT(lcs_length("ABC", "DEF") == 0);
        TEST("lcs('','anything') = 0") EXPECT(lcs_length("", "anything") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
