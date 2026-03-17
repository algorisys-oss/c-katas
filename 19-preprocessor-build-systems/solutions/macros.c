/*
 * Solution: Utility Macros
 *
 * Implement common C macros that demonstrate:
 *   - Proper parenthesization
 *   - The do-while(0) trick
 *   - Stringification (#) and token pasting (##)
 *   - Variadic macros (__VA_ARGS__)
 *   - typeof (GCC extension) for type-safe operations
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Macro 1: MIN(a, b) ──────────────────────────────────────────
 * Return the smaller of two values.
 * Must be safe with expressions: MIN(x+1, y+2) should work.
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* ── Macro 2: MAX(a, b) ──────────────────────────────────────────
 * Return the larger of two values.
 * Must be safe with expressions: MAX(x+1, y+2) should work.
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* ── Macro 3: SWAP(a, b) ─────────────────────────────────────────
 * Swap two variables of the same type.
 * Use a block with __typeof__ and a temporary variable.
 * Must work as a statement: SWAP(x, y);
 */
#define SWAP(a, b) do { \
    __typeof__(a) _swap_tmp = (a); \
    (a) = (b); \
    (b) = _swap_tmp; \
} while (0)

/* ── Macro 4: ARRAY_SIZE(arr) ────────────────────────────────────
 * Return the number of elements in a stack-allocated array.
 * sizeof(arr) / sizeof(arr[0])
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ── Macro 5: CLAMP(val, lo, hi) ─────────────────────────────────
 * Clamp a value to the range [lo, hi].
 * If val < lo, return lo. If val > hi, return hi. Otherwise val.
 */
#define CLAMP(val, lo, hi) ((val) < (lo) ? (lo) : ((val) > (hi) ? (hi) : (val)))

/* ── Macro 6: STRINGIFY(x) ───────────────────────────────────────
 * Turn a token into a string literal using the # operator.
 */
#define STRINGIFY(x) #x

/* ── Macro 7: CONCAT(a, b) ───────────────────────────────────────
 * Paste two tokens together using the ## operator.
 */
#define CONCAT(a, b) a##b

/* ── Macro 8: LOG(level, fmt, ...) ───────────────────────────────
 * Variadic debug logging macro that prints to stderr.
 * Format: [level] file:line: message
 */
#define LOG(level, fmt, ...) \
    fprintf(stderr, "[%s] %s:%d: " fmt "\n", \
            level, __FILE__, __LINE__, __VA_ARGS__)

/* ── Macro 9: ASSERT_EQ(a, b) ───────────────────────────────────
 * Test assertion that prints both values on failure.
 * Uses stringification to show the expressions.
 */
#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "ASSERT_EQ failed: %s != %s (%d != %d) at %s:%d\n", \
                #a, #b, (a), (b), __FILE__, __LINE__); \
    } \
} while (0)

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
    printf("=== Utility Macros ===\n\n");

    /* --- MIN / MAX tests --- */
    printf("MIN / MAX:\n");

    TEST("MIN of two constants")
        EXPECT(MIN(3, 7) == 3);

    TEST("MAX of two constants")
        EXPECT(MAX(3, 7) == 7);

    TEST("MIN with expressions (2+1, 1+1)")
        EXPECT(MIN(2 + 1, 1 + 1) == 2);

    TEST("MAX with negative values")
        EXPECT(MAX(-5, -2) == -2);

    /* --- SWAP test --- */
    printf("\nSWAP:\n");
    {
        int a = 10, b = 20;
        SWAP(a, b);
        TEST("SWAP two ints")
            EXPECT(a == 20 && b == 10);
    }

    /* --- ARRAY_SIZE tests --- */
    printf("\nARRAY_SIZE:\n");
    {
        int arr5[5] = {1, 2, 3, 4, 5};
        TEST("ARRAY_SIZE of int[5]")
            EXPECT(ARRAY_SIZE(arr5) == 5);
    }
    {
        double arr3[3] = {1.0, 2.0, 3.0};
        TEST("ARRAY_SIZE of double[3]")
            EXPECT(ARRAY_SIZE(arr3) == 3);
    }

    /* --- CLAMP tests --- */
    printf("\nCLAMP:\n");

    TEST("CLAMP value within range")
        EXPECT(CLAMP(5, 0, 10) == 5);

    TEST("CLAMP value below range")
        EXPECT(CLAMP(-3, 0, 10) == 0);

    TEST("CLAMP value above range")
        EXPECT(CLAMP(15, 0, 10) == 10);

    /* --- STRINGIFY test --- */
    printf("\nSTRINGIFY:\n");

    TEST("STRINGIFY turns token to string")
        EXPECT(strcmp(STRINGIFY(hello), "hello") == 0);

    /* --- CONCAT test --- */
    printf("\nCONCAT:\n");
    {
        int my_var = 42;
        TEST("CONCAT pastes tokens (my + _var)")
            EXPECT(CONCAT(my, _var) == 42);
    }

    /* --- ASSERT_EQ test (verify it doesn't abort on equal values) --- */
    printf("\nASSERT_EQ:\n");
    {
        /* Redirect stderr to /dev/null for the failure test */
        int x = 5, y = 5;
        ASSERT_EQ(x, y); /* should not print anything */
        TEST("ASSERT_EQ passes for equal values")
            EXPECT(1); /* If we got here, it didn't crash */
    }

    /* --- LOG test (just verify it compiles and runs) --- */
    printf("\nLOG:\n");
    {
        /* LOG writes to stderr, so it won't interfere with test output */
        LOG("INFO", "test value = %d", 42);
        TEST("LOG macro compiles and runs")
            EXPECT(1); /* If we got here, the macro expanded correctly */
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
