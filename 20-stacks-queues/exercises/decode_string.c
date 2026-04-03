/*
 * Kata: Decode String
 *
 * Given an encoded string like "3[a2[b]]", decode it to "abbabbabb".
 *
 * Rules:
 *   - A number followed by [...] means repeat the contents that many times
 *   - Can be nested: 3[a2[b]] = 3[abb] = abbabbabb
 *   - Numbers can be multi-digit: 12[a] = "aaaaaaaaaaaa"
 *   - Characters outside brackets are kept as-is
 *
 * Why a stack? Nested brackets are like nested parentheses — every time
 * you see '[', you "enter a new level" and need to remember the previous
 * level. That's exactly what a stack does.
 *
 * Connection to expression evaluation: in the previous exercise, you used
 * a stack to handle nested parentheses in math expressions. This is the
 * same idea — brackets create nesting, and stacks handle nesting.
 *
 * ── Stack trace for "3[a2[b]]" ──────────────────────────────────────
 *
 *   We maintain a stack of (string, repeat_count) pairs.
 *   "current" is the string we're building at the current nesting level.
 *
 *   Char   Action                         Stack              current
 *   ────   ──────────────────────────     ──────────────     ───────
 *   '3'    count = 3                      []                 ""
 *   '['    push("", 3), reset current     [("", 3)]         ""
 *   'a'    append 'a' to current          [("", 3)]         "a"
 *   '2'    count = 2                      [("", 3)]         "a"
 *   '['    push("a", 2), reset current    [("", 3),("a",2)] ""
 *   'b'    append 'b' to current          [("", 3),("a",2)] "b"
 *   ']'    pop ("a", 2)                   [("", 3)]         "a"+"bb"="abb"
 *          current = popped_str + current * count
 *   ']'    pop ("", 3)                    []                 ""+"abbabbabb"
 *          current = popped_str + current * count
 *
 *   Result: "abbabbabb"
 *
 * 6 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RESULT_LEN 4096
#define MAX_STACK_DEPTH 64

/* ── Exercise 1: decode_string ────────────────────────────────────────
 *
 * Decode the encoded string and write the result into 'output'.
 *
 * Algorithm (stack-based):
 *   - Maintain a stack of (string, count) pairs
 *   - Maintain a "current string" being built at the current level
 *   - For each character in the input:
 *     - If it's a digit: accumulate into a number (handles multi-digit)
 *     - If it's '[':
 *         Push (current_string, accumulated_number) onto the stack
 *         Reset current_string to empty
 *         Reset accumulated_number to 0
 *     - If it's ']':
 *         Pop (prev_string, count) from the stack
 *         current_string = prev_string + current_string repeated count times
 *     - If it's a letter: append to current_string
 *   - After processing all characters, current_string is the answer
 *
 * Parameters:
 *   input  — the encoded string (e.g., "3[a2[b]]")
 *   output — buffer to write the decoded result into
 *   output_size — size of the output buffer
 */
void decode_string(const char *input, char *output, int output_size)
{
    /* TODO: implement the stack-based decode algorithm
     *
     * Hint: you'll need a stack where each frame stores:
     *   - a string (what was being built before we entered this '[')
     *   - a repeat count (the number before the '[')
     *
     * You can use fixed-size arrays for simplicity:
     *   char str_stack[MAX_STACK_DEPTH][MAX_RESULT_LEN];
     *   int  count_stack[MAX_STACK_DEPTH];
     *   int  stack_top = 0;
     *
     * And a "current" buffer for the string being built:
     *   char current[MAX_RESULT_LEN] = "";
     */
    (void)input;
    (void)output_size;
    output[0] = '\0'; /* placeholder — empty string */
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
    printf("=== Decode String ===\n\n");

    char buf[MAX_RESULT_LEN];

    printf("decode_string:\n");
    {
        decode_string("3[a]", buf, sizeof(buf));
        TEST("\"3[a]\" -> \"aaa\"")
            EXPECT(strcmp(buf, "aaa") == 0);

        decode_string("2[ab]", buf, sizeof(buf));
        TEST("\"2[ab]\" -> \"abab\"")
            EXPECT(strcmp(buf, "abab") == 0);

        decode_string("3[a2[b]]", buf, sizeof(buf));
        TEST("\"3[a2[b]]\" -> \"abbabbabb\"")
            EXPECT(strcmp(buf, "abbabbabb") == 0);

        decode_string("2[a]3[b]", buf, sizeof(buf));
        TEST("\"2[a]3[b]\" -> \"aabbb\"")
            EXPECT(strcmp(buf, "aabbb") == 0);

        decode_string("abc", buf, sizeof(buf));
        TEST("\"abc\" -> \"abc\" (no encoding)")
            EXPECT(strcmp(buf, "abc") == 0);

        decode_string("2[a2[b3[c]]]", buf, sizeof(buf));
        TEST("\"2[a2[b3[c]]]\" -> deeply nested")
            EXPECT(strcmp(buf, "abcccbcccabcccbccc") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
