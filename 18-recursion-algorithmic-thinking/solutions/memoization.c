#include <stdio.h>
#include <string.h>

#define MAX_N 100
#define UNSET -1

/* ── Exercise 1: fib_memo ──────────────────────────────────────────── */

int fib_memo(int n, int cache[])
{
    if (n <= 1) {
        return n;
    }
    if (cache[n] != UNSET) {
        return cache[n];
    }
    cache[n] = fib_memo(n - 1, cache) + fib_memo(n - 2, cache);
    return cache[n];
}

/* ── Exercise 2: climb_stairs ──────────────────────────────────────── */

int climb_stairs(int n, int cache[])
{
    if (n <= 1) {
        return 1;
    }
    if (cache[n] != UNSET) {
        return cache[n];
    }
    cache[n] = climb_stairs(n - 1, cache) + climb_stairs(n - 2, cache);
    return cache[n];
}

/* ── Exercise 3: coin_change ───────────────────────────────────────── */

int coin_change(int coins[], int num_coins, int amount)
{
    if (amount == 0) {
        return 0;
    }

    int dp[amount + 1];
    dp[0] = 0;
    for (int i = 1; i <= amount; i++) {
        dp[i] = amount + 1;  /* sentinel: larger than any valid answer */
    }

    for (int i = 1; i <= amount; i++) {
        for (int c = 0; c < num_coins; c++) {
            if (coins[c] <= i && dp[i - coins[c]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[c]] + 1;
            }
        }
    }

    return dp[amount] > amount ? -1 : dp[amount];
}

/* ── Exercise 4: lcs_length ────────────────────────────────────────── */

int lcs_length(const char *a, const char *b)
{
    int len_a = (int)strlen(a);
    int len_b = (int)strlen(b);

    /* dp[i][j] = LCS length of a[0..i-1] and b[0..j-1] */
    int dp[MAX_N + 1][MAX_N + 1];
    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= len_a; i++) {
        for (int j = 1; j <= len_b; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = dp[i - 1][j] > dp[i][j - 1]
                          ? dp[i - 1][j]
                          : dp[i][j - 1];
            }
        }
    }

    return dp[len_a][len_b];
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
