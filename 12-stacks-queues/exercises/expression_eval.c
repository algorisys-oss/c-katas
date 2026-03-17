/*
 * Kata: Expression Evaluator (Infix → Postfix → Evaluate)
 *
 * Two-step process:
 *   1. Convert infix expression to postfix (Shunting-Yard algorithm)
 *   2. Evaluate the postfix expression using a stack
 *
 * Supports: +, -, *, / with single-digit integer operands.
 * Spaces are allowed in the input.
 *
 * Example: "3 + 4 * 2" → postfix "3 4 2 * +" → result 11
 *
 * 8 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ── Helper: operator precedence ───────────────────────────────────
 * Return the precedence of an operator: * and / are 2, + and - are 1.
 * Return 0 for non-operators.
 */
static int precedence(char op)
{
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

/* ── Helper: is this character an operator? ────────────────────── */
static int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

/* ── Exercise 1: infix_to_postfix ──────────────────────────────────
 *
 * Convert an infix expression string to postfix notation.
 *
 * Algorithm (Shunting-Yard):
 *   - Walk through each character in input:
 *     - If it's a digit: append it to output
 *     - If it's an operator:
 *         While the operator stack is not empty AND the top of the
 *         stack has >= precedence: pop it to output (with a space)
 *         Then push current operator onto the stack
 *     - If it's '(': push onto stack
 *     - If it's ')': pop operators to output until '(' is found, discard '('
 *     - Skip spaces
 *   - After all tokens: pop remaining operators to output
 *
 * Separate tokens in the output with spaces.
 *
 * Parameters:
 *   input       — the infix expression string (e.g., "3 + 4 * 2")
 *   output      — buffer to write the postfix string into
 *   output_size — size of the output buffer
 */
void infix_to_postfix(const char *input, char *output, int output_size)
{
    /* TODO: Implement the Shunting-Yard algorithm */
    (void)input;
    (void)output_size;
    output[0] = '\0'; /* placeholder — empty string */
}

/* ── Exercise 2: eval_postfix ──────────────────────────────────────
 *
 * Evaluate a postfix expression and return the integer result.
 *
 * Algorithm:
 *   - Walk through each token (space-separated):
 *     - If it's a digit: push its value onto a value stack
 *     - If it's an operator: pop two values, apply the operator, push result
 *       (Note: for -, /: the first popped value is the RIGHT operand)
 *   - The final value on the stack is the result
 *
 * Parameters:
 *   expr — the postfix expression string (e.g., "3 4 2 * +")
 *
 * Returns: the integer result of the expression
 */
int eval_postfix(const char *expr)
{
    /* TODO: Evaluate the postfix expression using a stack */
    (void)expr;
    return 0; /* placeholder */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

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
    printf("=== Expression Evaluator ===\n\n");

    char buf[256];

    printf("infix_to_postfix:\n");
    {
        infix_to_postfix("3 + 4", buf, sizeof(buf));
        TEST("\"3 + 4\" → \"3 4 +\"")
            EXPECT(strcmp(buf, "3 4 +") == 0);

        infix_to_postfix("3 + 4 * 2", buf, sizeof(buf));
        TEST("\"3 + 4 * 2\" → \"3 4 2 * +\"")
            EXPECT(strcmp(buf, "3 4 2 * +") == 0);

        infix_to_postfix("(3 + 4) * 2", buf, sizeof(buf));
        TEST("\"(3 + 4) * 2\" → \"3 4 + 2 *\"")
            EXPECT(strcmp(buf, "3 4 + 2 *") == 0);

        infix_to_postfix("5 - 3 + 2", buf, sizeof(buf));
        TEST("\"5 - 3 + 2\" → \"5 3 - 2 +\"")
            EXPECT(strcmp(buf, "5 3 - 2 +") == 0);
    }

    printf("\neval_postfix:\n");
    {
        TEST("\"3 4 +\" → 7")
            EXPECT(eval_postfix("3 4 +") == 7);

        TEST("\"3 4 2 * +\" → 11")
            EXPECT(eval_postfix("3 4 2 * +") == 11);

        TEST("\"3 4 + 2 *\" → 14")
            EXPECT(eval_postfix("3 4 + 2 *") == 14);

        TEST("\"8 2 / 3 -\" → 1")
            EXPECT(eval_postfix("8 2 / 3 -") == 1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
