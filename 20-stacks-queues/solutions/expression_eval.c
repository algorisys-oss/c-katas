/*
 * Solution: Expression Evaluator (Infix → Postfix → Evaluate)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int precedence(char op)
{
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

static int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

void infix_to_postfix(const char *input, char *output, int output_size)
{
    char op_stack[256];
    int op_top = 0;
    int out_pos = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        if (c == ' ') {
            continue;
        }

        if (isdigit((unsigned char)c)) {
            if (out_pos > 0 && out_pos < output_size - 1) {
                output[out_pos++] = ' ';
            }
            output[out_pos++] = c;
        } else if (c == '(') {
            op_stack[op_top++] = c;
        } else if (c == ')') {
            while (op_top > 0 && op_stack[op_top - 1] != '(') {
                if (out_pos > 0 && out_pos < output_size - 1) {
                    output[out_pos++] = ' ';
                }
                output[out_pos++] = op_stack[--op_top];
            }
            if (op_top > 0) {
                op_top--; /* discard '(' */
            }
        } else if (is_operator(c)) {
            while (op_top > 0 && op_stack[op_top - 1] != '(' &&
                   precedence(op_stack[op_top - 1]) >= precedence(c)) {
                if (out_pos > 0 && out_pos < output_size - 1) {
                    output[out_pos++] = ' ';
                }
                output[out_pos++] = op_stack[--op_top];
            }
            op_stack[op_top++] = c;
        }
    }

    /* Pop remaining operators */
    while (op_top > 0) {
        if (out_pos > 0 && out_pos < output_size - 1) {
            output[out_pos++] = ' ';
        }
        output[out_pos++] = op_stack[--op_top];
    }

    output[out_pos] = '\0';
}

int eval_postfix(const char *expr)
{
    int val_stack[256];
    int val_top = 0;

    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];

        if (c == ' ') {
            continue;
        }

        if (isdigit((unsigned char)c)) {
            val_stack[val_top++] = c - '0';
        } else if (is_operator(c)) {
            int b = val_stack[--val_top]; /* right operand (popped first) */
            int a = val_stack[--val_top]; /* left operand */
            int result = 0;
            switch (c) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = (b != 0) ? a / b : 0; break;
            }
            val_stack[val_top++] = result;
        }
    }

    return val_stack[0];
}

/* ── Test Harness ─────────────────────────────────────────────────── */

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
    printf("=== Expression Evaluator (Solution) ===\n\n");

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
