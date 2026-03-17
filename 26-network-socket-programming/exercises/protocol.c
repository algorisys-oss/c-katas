/*
 * Kata: Simple Text Protocol Parser
 *
 * Define and parse a simple text-based protocol: "CMD:arg1:arg2\n"
 *
 * This is how many real protocols work — Redis uses "GET key\r\n",
 * SMTP uses "MAIL FROM:<addr>\r\n", etc. Learning to parse a strict
 * text format is a key networking skill.
 *
 * Protocol format:
 *   COMMAND:arg1:arg2:...:argN\n
 *
 * Rules:
 *   - Command and args are separated by colons
 *   - Line ends with \n
 *   - Command must be non-empty uppercase letters only (A-Z)
 *   - Arguments can contain any printable characters except colon and newline
 *   - Maximum of max_args arguments
 *
 * Functions to implement:
 *   - proto_parse     (parse a protocol line into command + args)
 *   - proto_build     (build a protocol line from command + args)
 *   - proto_validate  (check if a line is valid protocol format)
 *
 * 8 tests.
 *
 * Hints:
 *   - Walk character by character to find colons and newlines
 *   - strncpy() to copy substrings of known length
 *   - snprintf() to build strings safely
 *   - A character c is uppercase if: c >= 'A' && c <= 'Z'
 *   - A character c is printable ASCII if: c >= 32 && c <= 126
 */

#include <stdio.h>
#include <string.h>

#define MAX_CMD      32
#define MAX_ARG      128
#define MAX_ARGS     8
#define MAX_LINE     512

/* ── proto_parse ─────────────────────────────────────────────────
 *
 * Parse a protocol line into a command and array of arguments.
 *
 * Input:  "SET:name:Alice\n"
 * Output: cmd = "SET", args[0] = "name", args[1] = "Alice", returns 2
 *
 * Input:  "PING\n"
 * Output: cmd = "PING", returns 0
 *
 * Returns the number of arguments parsed, or -1 on error.
 * The args array must have room for max_args entries, each of size MAX_ARG.
 */
int proto_parse(const char *line, char *cmd, char args[][MAX_ARG], int max_args)
{
    /* TODO: Implement this function
     *
     * Steps:
     *   1. Check for NULL inputs — return -1 if any are NULL
     *   2. Walk forward until you hit ':' or '\n' or '\0'
     *      — everything before that is the command
     *   3. Copy the command into cmd, null-terminate it
     *   4. If you hit '\n' or '\0' (no colon), return 0 (no args)
     *   5. Otherwise, skip the colon and parse arguments:
     *      - For each argument, walk forward to the next ':' or '\n' or '\0'
     *      - Copy into args[arg_count], increment arg_count
     *      - Stop when you hit '\n', '\0', or arg_count == max_args
     *   6. Return arg_count
     */
    (void)line; (void)cmd; (void)args; (void)max_args;
    return -1;
}

/* ── proto_build ─────────────────────────────────────────────────
 *
 * Build a protocol line from a command and arguments.
 *
 * Input:  cmd = "SET", args = {"key1", "value1"}, arg_count = 2
 * Output: out_buf = "SET:key1:value1\n"
 *
 * Input:  cmd = "QUIT", args = NULL, arg_count = 0
 * Output: out_buf = "QUIT\n"
 *
 * Returns the number of bytes written (excluding null terminator),
 * or -1 on error.
 */
int proto_build(const char *cmd, const char args[][MAX_ARG], int arg_count,
                char *out_buf, int size)
{
    /* TODO: Implement this function
     *
     * Steps:
     *   1. Check for NULL cmd and out_buf — return -1 if NULL
     *   2. Start by writing the command with snprintf()
     *   3. For each argument, append ":" + arg using snprintf()
     *      — track the current position in out_buf
     *      — check for buffer overflow at each step
     *   4. Append '\n' at the end
     *   5. Null-terminate the buffer
     *   6. Return the total number of bytes written
     */
    (void)cmd; (void)args; (void)arg_count; (void)out_buf; (void)size;
    return -1;
}

/* ── proto_validate ──────────────────────────────────────────────
 *
 * Check if a protocol line is valid.
 *
 * Rules:
 *   - Must end with \n
 *   - Command (before first colon or \n) must be non-empty uppercase A-Z only
 *   - Arguments can be any printable ASCII (32-126) except colon and newline
 *
 * Returns 1 if valid, 0 if invalid.
 */
int proto_validate(const char *line)
{
    /* TODO: Implement this function
     *
     * Steps:
     *   1. Check for NULL or empty string — return 0
     *   2. Check first char is uppercase (A-Z), if not return 0
     *   3. Walk forward while chars are uppercase A-Z (this is the command)
     *   4. After command, expect either ':' or '\n'
     *      — if '\n', it's a valid command-only line, return 1
     *      — if neither, return 0
     *   5. After the colon, validate arguments:
     *      — each char must be printable (32-126) or ':' or '\n'
     *      — ':' separates arguments
     *      — '\n' terminates the line — return 1
     *   6. If you reach '\0' without finding '\n', return 0
     */
    (void)line;
    return 0;
}

/* ── Test Harness ────────────────────────────────────────────── */

static int tests_total = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Protocol Parser ===\n\n");

    /* --- proto_parse tests --- */

    printf("-- proto_parse --\n");

    /* Test 1: Parse command with two args */
    {
        char cmd[MAX_CMD];
        char args[MAX_ARGS][MAX_ARG];
        int n = proto_parse("SET:name:Alice\n", cmd, args, MAX_ARGS);
        TEST("parse SET:name:Alice");
        EXPECT(n == 2 &&
               strcmp(cmd, "SET") == 0 &&
               strcmp(args[0], "name") == 0 &&
               strcmp(args[1], "Alice") == 0);
    }

    /* Test 2: Parse command with no args */
    {
        char cmd[MAX_CMD];
        char args[MAX_ARGS][MAX_ARG];
        int n = proto_parse("PING\n", cmd, args, MAX_ARGS);
        TEST("parse PING (no args)");
        EXPECT(n == 0 && strcmp(cmd, "PING") == 0);
    }

    /* Test 3: Parse command with one arg */
    {
        char cmd[MAX_CMD];
        char args[MAX_ARGS][MAX_ARG];
        int n = proto_parse("GET:username\n", cmd, args, MAX_ARGS);
        TEST("parse GET:username (one arg)");
        EXPECT(n == 1 &&
               strcmp(cmd, "GET") == 0 &&
               strcmp(args[0], "username") == 0);
    }

    printf("\n");

    /* --- proto_build tests --- */

    printf("-- proto_build --\n");

    /* Test 4: Build a SET command */
    {
        char buf[MAX_LINE];
        const char args[][MAX_ARG] = {"key1", "value1"};
        int n = proto_build("SET", args, 2, buf, MAX_LINE);
        TEST("build SET:key1:value1");
        EXPECT(n > 0 && strcmp(buf, "SET:key1:value1\n") == 0);
    }

    /* Test 5: Build a command with no args */
    {
        char buf[MAX_LINE];
        int n = proto_build("QUIT", NULL, 0, buf, MAX_LINE);
        TEST("build QUIT (no args)");
        EXPECT(n > 0 && strcmp(buf, "QUIT\n") == 0);
    }

    printf("\n");

    /* --- proto_validate tests --- */

    printf("-- proto_validate --\n");

    /* Test 6: Valid command with args */
    {
        TEST("validate SET:x:y (valid)");
        EXPECT(proto_validate("SET:x:y\n") == 1);
    }

    /* Test 7: Invalid — lowercase command */
    {
        TEST("reject lowercase command");
        EXPECT(proto_validate("set:x:y\n") == 0);
    }

    /* Test 8: Invalid — no newline terminator */
    {
        TEST("reject missing newline");
        EXPECT(proto_validate("SET:x:y") == 0);
    }

    printf("\n");

    /* --- Summary --- */
    printf("Results: %d / %d passed\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("ALL TESTS PASSED\n");
    }

    return tests_passed == tests_total ? 0 : 1;
}
