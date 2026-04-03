/*
 * Solution: Simple Text Protocol Parser
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
 *   - Command must be non-empty uppercase letters only
 *   - Arguments can contain any printable characters except colon and newline
 *   - Maximum of max_args arguments
 *
 * Functions:
 *   - proto_parse     (parse a protocol line into command + args)
 *   - proto_build     (build a protocol line from command + args)
 *   - proto_validate  (check if a line is valid protocol format)
 *
 * 8 tests.
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
 * Returns the number of arguments parsed, or -1 on error.
 * The args array must have room for max_args entries, each of size MAX_ARG.
 */
int proto_parse(const char *line, char *cmd, char args[][MAX_ARG], int max_args)
{
    if (line == NULL || cmd == NULL || args == NULL) {
        return -1;
    }

    /* Find the end of the command (first colon or newline) */
    const char *p = line;
    const char *sep = p;
    while (*sep != ':' && *sep != '\n' && *sep != '\0') {
        sep++;
    }

    int cmd_len = (int)(sep - p);
    if (cmd_len <= 0 || cmd_len >= MAX_CMD) {
        return -1;
    }

    strncpy(cmd, p, cmd_len);
    cmd[cmd_len] = '\0';

    /* If no colon, there are no arguments */
    if (*sep != ':') {
        return 0;
    }

    /* Parse arguments separated by colons */
    int arg_count = 0;
    p = sep + 1;

    while (*p != '\n' && *p != '\0' && arg_count < max_args) {
        /* Find end of this argument */
        const char *arg_end = p;
        while (*arg_end != ':' && *arg_end != '\n' && *arg_end != '\0') {
            arg_end++;
        }

        int arg_len = (int)(arg_end - p);
        if (arg_len >= MAX_ARG) {
            arg_len = MAX_ARG - 1;
        }
        strncpy(args[arg_count], p, arg_len);
        args[arg_count][arg_len] = '\0';
        arg_count++;

        if (*arg_end == ':') {
            p = arg_end + 1;
        } else {
            break;
        }
    }

    return arg_count;
}

/* ── proto_build ─────────────────────────────────────────────────
 *
 * Build a protocol line from a command and arguments.
 *
 * Input:  cmd = "SET", args = {"name", "Alice"}, arg_count = 2
 * Output: out_buf = "SET:name:Alice\n"
 *
 * Returns the number of bytes written (excluding null terminator),
 * or -1 on error.
 */
int proto_build(const char *cmd, const char args[][MAX_ARG], int arg_count,
                char *out_buf, int size)
{
    if (cmd == NULL || out_buf == NULL || size <= 0) {
        return -1;
    }

    /* Start with the command */
    int pos = snprintf(out_buf, size, "%s", cmd);
    if (pos < 0 || pos >= size) {
        return -1;
    }

    /* Append each argument with a colon separator */
    for (int i = 0; i < arg_count; i++) {
        int remaining = size - pos;
        int written = snprintf(out_buf + pos, remaining, ":%s", args[i]);
        if (written < 0 || written >= remaining) {
            return -1;
        }
        pos += written;
    }

    /* Append newline */
    if (pos + 1 >= size) {
        return -1;
    }
    out_buf[pos] = '\n';
    pos++;
    out_buf[pos] = '\0';

    return pos;
}

/* ── proto_validate ──────────────────────────────────────────────
 *
 * Check if a protocol line is valid.
 *
 * Rules:
 *   - Must end with \n
 *   - Command (before first colon or \n) must be non-empty uppercase letters
 *   - Arguments can be any printable ASCII except colon and newline
 *
 * Returns 1 if valid, 0 if invalid.
 */
int proto_validate(const char *line)
{
    if (line == NULL || *line == '\0') {
        return 0;
    }

    const char *p = line;

    /* Validate command: must be non-empty uppercase A-Z */
    if (*p < 'A' || *p > 'Z') {
        return 0;
    }
    while (*p >= 'A' && *p <= 'Z') {
        p++;
    }

    /* After command, expect colon or newline */
    if (*p == '\n') {
        /* Command only, no args — valid if \n and nothing malformed follows */
        return 1;
    }
    if (*p != ':') {
        return 0;
    }

    /* Validate arguments: printable ASCII (32-126) except colon and newline,
     * separated by colons, terminated by newline */
    p++;  /* skip the colon after command */
    while (*p != '\0') {
        if (*p == '\n') {
            return 1;  /* valid termination */
        }
        if (*p == ':') {
            /* colon separates args — next char must not be \n or \0 (empty trailing arg ok) */
            p++;
            continue;
        }
        /* Must be printable ASCII (32-126), excluding colon (58) and newline (10) */
        if (*p < 32 || *p > 126) {
            return 0;
        }
        p++;
    }

    /* Reached end of string without \n — invalid */
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
