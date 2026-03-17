/*
 * Kata: Array-Based Stack + Balanced Parentheses Checker
 *
 * Implement an array-based stack, then use it to check whether
 * an expression has balanced parentheses, brackets, and braces.
 *
 * 12 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Stack Data Structure ──────────────────────────────────────────
 *
 * The stack stores characters (for bracket matching).
 * It uses a dynamically allocated array with a fixed capacity.
 *
 *    Array:  [ '(' | '{' | '[' |     |     ]
 *    Index:    0     1     2     3     4
 *                                ↑
 *                               top = 3 (next empty slot)
 */
typedef struct {
    char *data;     /* dynamically allocated array */
    int top;        /* index of next empty slot (0 = empty stack) */
    int capacity;   /* maximum number of elements */
} Stack;

/* ── Exercise 1: stack_create ──────────────────────────────────────
 * Allocate and return a new stack with the given capacity.
 * Initialize top to 0.
 */
Stack *stack_create(int capacity)
{
    /* TODO: malloc a Stack, malloc its data array, set fields */
    return NULL; /* placeholder */
}

/* ── Exercise 2: stack_destroy ─────────────────────────────────────
 * Free the stack's data array, then free the stack itself.
 */
void stack_destroy(Stack *s)
{
    /* TODO: free data, then free the struct */
}

/* ── Exercise 3: stack_push ────────────────────────────────────────
 * Push a character onto the stack.
 * Return 1 on success, 0 if the stack is full.
 */
int stack_push(Stack *s, char c)
{
    /* TODO: check if full, then store c at top and increment top */
    return 0; /* placeholder */
}

/* ── Exercise 4: stack_pop ─────────────────────────────────────────
 * Remove and return the top character.
 * Return '\0' if the stack is empty.
 */
char stack_pop(Stack *s)
{
    /* TODO: check if empty, then decrement top and return element */
    return '\0'; /* placeholder */
}

/* ── Exercise 5: stack_peek ────────────────────────────────────────
 * Return the top character without removing it.
 * Return '\0' if the stack is empty.
 */
char stack_peek(const Stack *s)
{
    /* TODO: return data[top - 1] if not empty */
    return '\0'; /* placeholder */
}

/* ── Exercise 6: stack_is_empty ────────────────────────────────────
 * Return 1 if the stack has no elements, 0 otherwise.
 */
int stack_is_empty(const Stack *s)
{
    /* TODO */
    return 1; /* placeholder */
}

/* ── Exercise 7: is_balanced ───────────────────────────────────────
 * Check if the parentheses/brackets/braces in expr are balanced.
 *
 * Algorithm:
 *   - Walk through each character in expr
 *   - If it's an opening bracket ( '(', '[', '{' ), push it
 *   - If it's a closing bracket ( ')', ']', '}' ):
 *       - If the stack is empty → not balanced
 *       - Pop the top and check if it matches the closing bracket
 *       - If it doesn't match → not balanced
 *   - After processing all characters, the stack must be empty
 *
 * Return 1 if balanced, 0 otherwise.
 */
int is_balanced(const char *expr)
{
    /* TODO: create a stack, walk through expr, check matching brackets */
    (void)expr; /* suppress unused warning — remove when implementing */
    return 0;   /* placeholder */
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
    printf("=== Stack & Balanced Parentheses ===\n\n");

    /* --- Stack basic operations --- */
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

    /* --- Balanced parentheses --- */
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
