/*
 * Solution: Expression Evaluator (Recursive Descent)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* ── Parser state ─────────────────────────────────────────────────── */
static const char *input_ptr;

/* ── skip_spaces ──────────────────────────────────────────────────── */
static void skip_spaces(void)
{
    while (*input_ptr && isspace((unsigned char)*input_ptr)) {
        input_ptr++;
    }
}

/* ── Forward declaration ──────────────────────────────────────────── */
static double parse_expr(void);

/* ── parse_factor ─────────────────────────────────────────────────
 * factor → NUMBER | '(' expr ')'
 */
static double parse_factor(void)
{
    skip_spaces();

    if (*input_ptr == '(') {
        input_ptr++;  /* consume '(' */
        double result = parse_expr();
        skip_spaces();
        if (*input_ptr == ')') {
            input_ptr++;  /* consume ')' */
        }
        return result;
    }

    /* Must be a number */
    char *end;
    double result = strtod(input_ptr, &end);
    input_ptr = end;
    return result;
}

/* ── parse_term ───────────────────────────────────────────────────
 * term → factor (('*' | '/') factor)*
 */
static double parse_term(void)
{
    double result = parse_factor();

    for (;;) {
        skip_spaces();
        if (*input_ptr == '*') {
            input_ptr++;
            result *= parse_factor();
        } else if (*input_ptr == '/') {
            input_ptr++;
            result /= parse_factor();
        } else {
            break;
        }
    }

    return result;
}

/* ── parse_expr ───────────────────────────────────────────────────
 * expr → term (('+' | '-') term)*
 */
static double parse_expr(void)
{
    double result = parse_term();

    for (;;) {
        skip_spaces();
        if (*input_ptr == '+') {
            input_ptr++;
            result += parse_term();
        } else if (*input_ptr == '-') {
            input_ptr++;
            result -= parse_term();
        } else {
            break;
        }
    }

    return result;
}

/* ── eval ─────────────────────────────────────────────────────────── */
double eval(const char *expression)
{
    input_ptr = expression;
    return parse_expr();
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
