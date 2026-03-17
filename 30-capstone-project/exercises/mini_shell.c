/*
 * Kata: Mini Shell — Parsing Layer
 *
 * A Unix shell reads a line, parses it into arguments, and executes
 * the command. This exercise focuses on the parsing: the part we can
 * test without creating real processes.
 *
 * You'll implement:
 *   1. shell_parse_line  — split a command line into an argv-style array
 *   2. shell_find_pipe   — find the "|" token in the args array
 *   3. shell_is_builtin  — check if a command is a shell built-in
 *
 * 12 tests total.
 *
 * After completing this, see the README for how to add fork/exec
 * and build a full working shell.
 */

#include <stdio.h>
#include <string.h>

/* ── Exercise 1: shell_parse_line ─────────────────────────────────
 * Split `line` into tokens separated by whitespace (spaces and tabs).
 * Store pointers to each token in `args[]`, up to `max_args - 1` tokens.
 * Set args[argc] = NULL (just like execvp expects).
 * Return the number of tokens (argc).
 *
 * You MUST modify `line` in-place (replace spaces with '\0') and
 * point args[i] into the modified line. Do NOT allocate memory.
 *
 * Example:
 *   char line[] = "ls -l /tmp";
 *   char *args[64];
 *   int argc = shell_parse_line(line, args, 64);
 *   // argc == 3
 *   // args[0] == "ls", args[1] == "-l", args[2] == "/tmp", args[3] == NULL
 *
 * Hints:
 *   - Skip leading whitespace before each token
 *   - Walk forward until next whitespace or '\0'
 *   - Replace the whitespace with '\0' to terminate the token
 *   - Repeat until end of line or max_args - 1 reached
 */
int shell_parse_line(char *line, char *args[], int max_args)
{
    /* TODO: Split line into tokens, store in args[], return count */
    (void)line;
    (void)args;
    (void)max_args;
    return 0; /* placeholder */
}

/* ── Exercise 2: shell_find_pipe ──────────────────────────────────
 * Search args[0..argc-1] for the "|" token.
 * Return its index if found, or -1 if there is no pipe.
 *
 * Example:
 *   args = {"ls", "-l", "|", "grep", ".c", NULL}
 *   argc = 5
 *   shell_find_pipe(args, argc) → 2
 */
int shell_find_pipe(char *args[], int argc)
{
    /* TODO: Find "|" in args, return index or -1 */
    (void)args;
    (void)argc;
    return 0; /* placeholder */
}

/* ── Exercise 3: shell_is_builtin ─────────────────────────────────
 * Return 1 if `cmd` is a shell built-in command, 0 otherwise.
 * Built-in commands: "cd", "exit", "help"
 *
 * Use strcmp to check each one.
 */
int shell_is_builtin(const char *cmd)
{
    /* TODO: Return 1 if cmd is "cd", "exit", or "help" */
    (void)cmd;
    return 0; /* placeholder */
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Mini Shell — Parsing Layer ===\n\n");

    /* --- shell_parse_line --- */
    printf("shell_parse_line:\n");
    {
        char line[] = "ls";
        char *args[64];
        int argc = shell_parse_line(line, args, 64);
        TEST("single command")
            EXPECT(argc == 1 && strcmp(args[0], "ls") == 0 && args[1] == NULL);
    }
    {
        char line[] = "ls -l /tmp";
        char *args[64];
        int argc = shell_parse_line(line, args, 64);
        TEST("command with arguments")
            EXPECT(argc == 3 && strcmp(args[0], "ls") == 0
                   && strcmp(args[1], "-l") == 0
                   && strcmp(args[2], "/tmp") == 0 && args[3] == NULL);
    }
    {
        char line[] = "  echo   hello   world  ";
        char *args[64];
        int argc = shell_parse_line(line, args, 64);
        TEST("extra whitespace")
            EXPECT(argc == 3 && strcmp(args[0], "echo") == 0
                   && strcmp(args[1], "hello") == 0
                   && strcmp(args[2], "world") == 0);
    }
    {
        char line[] = "";
        char *args[64];
        int argc = shell_parse_line(line, args, 64);
        TEST("empty line")
            EXPECT(argc == 0 && args[0] == NULL);
    }
    {
        char line[] = "   \t  ";
        char *args[64];
        int argc = shell_parse_line(line, args, 64);
        TEST("whitespace only")
            EXPECT(argc == 0 && args[0] == NULL);
    }
    {
        char line[] = "ls | grep .c";
        char *args[64];
        int argc = shell_parse_line(line, args, 64);
        TEST("command with pipe")
            EXPECT(argc == 4 && strcmp(args[0], "ls") == 0
                   && strcmp(args[1], "|") == 0
                   && strcmp(args[2], "grep") == 0
                   && strcmp(args[3], ".c") == 0);
    }
    {
        char line[] = "a b c";
        char *args[3]; /* max_args = 3, so at most 2 tokens */
        int argc = shell_parse_line(line, args, 3);
        TEST("max_args limits tokens")
            EXPECT(argc == 2 && strcmp(args[0], "a") == 0
                   && strcmp(args[1], "b") == 0 && args[2] == NULL);
    }

    /* --- shell_find_pipe --- */
    printf("\nshell_find_pipe:\n");
    {
        char *args[] = {"ls", "-l", "|", "grep", ".c", NULL};
        TEST("pipe in middle")
            EXPECT(shell_find_pipe(args, 5) == 2);
    }
    {
        char *args[] = {"ls", "-l", NULL};
        TEST("no pipe")
            EXPECT(shell_find_pipe(args, 2) == -1);
    }

    /* --- shell_is_builtin --- */
    printf("\nshell_is_builtin:\n");
    {
        TEST("cd is builtin")
            EXPECT(shell_is_builtin("cd") == 1);
    }
    {
        TEST("exit is builtin")
            EXPECT(shell_is_builtin("exit") == 1);
    }
    {
        TEST("ls is not builtin")
            EXPECT(shell_is_builtin("ls") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
