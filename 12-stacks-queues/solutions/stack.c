/*
 * Solution: Array-Based Stack + Balanced Parentheses Checker
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    int top;
    int capacity;
} Stack;

Stack *stack_create(int capacity)
{
    Stack *s = malloc(sizeof(Stack));
    if (!s) return NULL;
    s->data = malloc(sizeof(char) * capacity);
    if (!s->data) {
        free(s);
        return NULL;
    }
    s->top = 0;
    s->capacity = capacity;
    return s;
}

void stack_destroy(Stack *s)
{
    if (s) {
        free(s->data);
        free(s);
    }
}

int stack_push(Stack *s, char c)
{
    if (s->top >= s->capacity) return 0;
    s->data[s->top] = c;
    s->top++;
    return 1;
}

char stack_pop(Stack *s)
{
    if (s->top == 0) return '\0';
    s->top--;
    return s->data[s->top];
}

char stack_peek(const Stack *s)
{
    if (s->top == 0) return '\0';
    return s->data[s->top - 1];
}

int stack_is_empty(const Stack *s)
{
    return s->top == 0;
}

static char matching_open(char close)
{
    if (close == ')') return '(';
    if (close == ']') return '[';
    if (close == '}') return '{';
    return '\0';
}

int is_balanced(const char *expr)
{
    int len = (int)strlen(expr);
    Stack *s = stack_create(len + 1);
    if (!s) return 0;

    for (int i = 0; i < len; i++) {
        char c = expr[i];
        if (c == '(' || c == '[' || c == '{') {
            stack_push(s, c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (stack_is_empty(s)) {
                stack_destroy(s);
                return 0;
            }
            char top = stack_pop(s);
            if (top != matching_open(c)) {
                stack_destroy(s);
                return 0;
            }
        }
    }

    int balanced = stack_is_empty(s);
    stack_destroy(s);
    return balanced;
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
    printf("=== Stack & Balanced Parentheses (Solution) ===\n\n");

    printf("Stack operations:\n");
    {
        Stack *s = stack_create(10);
        TEST("new stack is empty")
            EXPECT(stack_is_empty(s));

        stack_push(s, 'A');
        TEST("push A, peek returns A")
            EXPECT(stack_peek(s) == 'A');

        stack_push(s, 'B');
        stack_push(s, 'C');
        TEST("push B,C — peek returns C")
            EXPECT(stack_peek(s) == 'C');

        TEST("pop returns C")
            EXPECT(stack_pop(s) == 'C');

        TEST("pop returns B")
            EXPECT(stack_pop(s) == 'B');

        TEST("pop returns A")
            EXPECT(stack_pop(s) == 'A');

        TEST("stack is empty after popping all")
            EXPECT(stack_is_empty(s));

        TEST("pop on empty returns '\\0'")
            EXPECT(stack_pop(s) == '\0');

        stack_destroy(s);
    }

    printf("\nis_balanced:\n");
    {
        TEST("empty string is balanced")
            EXPECT(is_balanced("") == 1);

        TEST("\"()\" is balanced")
            EXPECT(is_balanced("()") == 1);

        TEST("\"{[()]}\" is balanced")
            EXPECT(is_balanced("{[()]}") == 1);

        TEST("\"((()))\" is balanced")
            EXPECT(is_balanced("((()))") == 1);

        TEST("\"(]\" is NOT balanced")
            EXPECT(is_balanced("(]") == 0);

        TEST("\"({[}])\" is NOT balanced")
            EXPECT(is_balanced("({[}])") == 0);

        TEST("\"(\" is NOT balanced (unclosed)")
            EXPECT(is_balanced("(") == 0);

        TEST("\")\" is NOT balanced (no opener)")
            EXPECT(is_balanced(")") == 0);

        TEST("\"a + (b * [c - d])\" is balanced")
            EXPECT(is_balanced("a + (b * [c - d])") == 1);

        TEST("\"((()\" is NOT balanced")
            EXPECT(is_balanced("((()") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
