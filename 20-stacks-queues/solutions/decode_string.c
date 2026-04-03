/*
 * Solution: Decode String
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RESULT_LEN 4096
#define MAX_STACK_DEPTH 64

void decode_string(const char *input, char *output, int output_size)
{
    /* Stack stores the string-so-far and repeat count at each nesting level */
    char str_stack[MAX_STACK_DEPTH][MAX_RESULT_LEN];
    int count_stack[MAX_STACK_DEPTH];
    int stack_top = 0;

    char current[MAX_RESULT_LEN] = "";
    int num = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        if (isdigit((unsigned char)c)) {
            /* Accumulate multi-digit numbers */
            num = num * 10 + (c - '0');
        } else if (c == '[') {
            /* Save current state and start a fresh string */
            if (stack_top < MAX_STACK_DEPTH) {
                strncpy(str_stack[stack_top], current, MAX_RESULT_LEN - 1);
                str_stack[stack_top][MAX_RESULT_LEN - 1] = '\0';
                count_stack[stack_top] = num;
                stack_top++;
            }
            current[0] = '\0';
            num = 0;
        } else if (c == ']') {
            /* Pop the previous string and repeat count */
            if (stack_top > 0) {
                stack_top--;
                char *prev_str = str_stack[stack_top];
                int count = count_stack[stack_top];

                /* Build: prev_str + current * count */
                char temp[MAX_RESULT_LEN];
                strncpy(temp, prev_str, MAX_RESULT_LEN - 1);
                temp[MAX_RESULT_LEN - 1] = '\0';

                for (int r = 0; r < count; r++) {
                    int len = (int)strlen(temp);
                    int cur_len = (int)strlen(current);
                    if (len + cur_len < MAX_RESULT_LEN - 1) {
                        strcat(temp, current);
                    }
                }

                strncpy(current, temp, MAX_RESULT_LEN - 1);
                current[MAX_RESULT_LEN - 1] = '\0';
            }
        } else {
            /* Regular character — append to current string */
            int len = (int)strlen(current);
            if (len < MAX_RESULT_LEN - 2) {
                current[len] = c;
                current[len + 1] = '\0';
            }
        }
    }

    strncpy(output, current, (size_t)(output_size - 1));
    output[output_size - 1] = '\0';
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
    printf("=== Decode String (Solution) ===\n\n");

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
