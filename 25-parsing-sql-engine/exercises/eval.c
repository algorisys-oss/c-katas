/*
 * Kata: Expression Evaluator (Recursive Descent)
 *
 * Build a calculator that evaluates arithmetic expressions like:
 *   "3 + 4 * 2"     → 11
 *   "(1 + 2) * 3"   → 9
 *   "10 / (5 - 3)"  → 5
 *
 * This teaches you recursive descent parsing — the most fundamental
 * parsing technique. Each grammar rule becomes a function.
 *
 * Grammar (encodes operator precedence):
 *   expr   → term (('+' | '-') term)*
 *   term   → factor (('*' | '/') factor)*
 *   factor → NUMBER | '(' expr ')'
 *
 * Why this works:
 *   - expr handles +/- (lowest precedence)
 *   - term handles * and / (higher precedence)
 *   - factor handles numbers and parentheses (highest precedence)
 *   - * and / are evaluated before + and - because term() is called
 *     from within expr(), so it runs first.
 *
 * 10 tests total.
 *
 * Hints:
 *   - Keep a global (or static) position variable to track where
 *     you are in the string
 *   - skip_spaces() before reading each token
 *   - parse_factor reads a number or recursively calls parse_expr
 *     for parenthesized sub-expressions
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* ── Parser state ─────────────────────────────────────────────────
 * We use a module-level pointer to track our position in the input.
 */
static const char *input_ptr;

/* ── skip_spaces ──────────────────────────────────────────────────
 * Advance input_ptr past any whitespace characters.
 */
static void skip_spaces(void)
{
    /* TODO: while *input_ptr is a space, advance input_ptr */
}

/* ── parse_factor ─────────────────────────────────────────────────
 * factor → NUMBER | '(' expr ')'
 *
 * If the current character is a digit, read the full number.
 * If it's '(', consume it, parse an expr, then consume ')'.
 */
static double parse_expr(void);  /* forward declaration */

static double parse_factor(void)
{
    /* TODO:
     * 1. skip_spaces()
     * 2. If *input_ptr == '(':
     *      - advance past '('
     *      - result = parse_expr()
     *      - skip_spaces()
     *      - advance past ')'   (you can assert it's ')')
     *      - return result
     * 3. Else (it's a number):
     *      - use strtod(input_ptr, &end) to read a double
     *      - set input_ptr = end
     *      - return the number
     */
    return 0;
}

/* ── parse_term ───────────────────────────────────────────────────
 * term → factor (('*' | '/') factor)*
 *
 * Parse a factor, then keep consuming * or / followed by another factor.
 */
static double parse_term(void)
{
    /* TODO:
     * 1. result = parse_factor()
     * 2. Loop:
     *      - skip_spaces()
     *      - if *input_ptr is '*': advance, result *= parse_factor()
     *      - if *input_ptr is '/': advance, result /= parse_factor()
     *      - else: break
     * 3. return result
     */
    return 0;
}

/* ── parse_expr ───────────────────────────────────────────────────
 * expr → term (('+' | '-') term)*
 *
 * Parse a term, then keep consuming + or - followed by another term.
 */
static double parse_expr(void)
{
    /* TODO:
     * 1. result = parse_term()
     * 2. Loop:
     *      - skip_spaces()
     *      - if *input_ptr is '+': advance, result += parse_term()
     *      - if *input_ptr is '-': advance, result -= parse_term()
     *      - else: break
     * 3. return result
     */
    return 0;
}

/* ── eval ─────────────────────────────────────────────────────────
 * Public entry point. Set input_ptr and call parse_expr.
 */
double eval(const char *expression)
{
    /* TODO:
     * 1. Set input_ptr = expression
     * 2. return parse_expr()
     */
    (void)expression;
    return 0;
}

/* ── Test harness ─────────────────────────────────────────────────── */

static int tests_total = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  %d. %-50s ", tests_total, name); \
} while (0);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); }

#define NEAR(a, b) (fabs((a) - (b)) < 0.0001)

int main(void)
{
    printf("=== Expression Evaluator Tests ===\n\n");

    /* Test 1: Single number */
    TEST("single number: 42")
        EXPECT(NEAR(eval("42"), 42.0));

    /* Test 2: Simple addition */
    TEST("addition: 3 + 4")
        EXPECT(NEAR(eval("3 + 4"), 7.0));

    /* Test 3: Simple subtraction */
    TEST("subtraction: 10 - 3")
        EXPECT(NEAR(eval("10 - 3"), 7.0));

    /* Test 4: Simple multiplication */
    TEST("multiplication: 6 * 7")
        EXPECT(NEAR(eval("6 * 7"), 42.0));

    /* Test 5: Simple division */
    TEST("division: 20 / 4")
        EXPECT(NEAR(eval("20 / 4"), 5.0));

    /* Test 6: Precedence — * before + */
    TEST("precedence: 3 + 4 * 2 = 11")
        EXPECT(NEAR(eval("3 + 4 * 2"), 11.0));

    /* Test 7: Precedence — multiple operators */
    TEST("precedence: 2 * 3 + 4 * 5 = 26")
        EXPECT(NEAR(eval("2 * 3 + 4 * 5"), 26.0));

    /* Test 8: Parentheses override precedence */
    TEST("parentheses: (3 + 4) * 2 = 14")
        EXPECT(NEAR(eval("(3 + 4) * 2"), 14.0));

    /* Test 9: Nested parentheses */
    TEST("nested parens: ((2 + 3) * (4 - 1)) = 15")
        EXPECT(NEAR(eval("((2 + 3) * (4 - 1))"), 15.0));

    /* Test 10: Complex expression */
    TEST("complex: 10 / (5 - 3) + 2 * 4 = 13")
        EXPECT(NEAR(eval("10 / (5 - 3) + 2 * 4"), 13.0));

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
