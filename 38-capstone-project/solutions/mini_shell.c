/*
 * Solution: Mini Shell — Parsing Layer
 */

#include <stdio.h>
#include <string.h>

int shell_parse_line(char *line, char *args[], int max_args)
{
    int argc = 0;
    char *p = line;
    while (*p != '\0' && argc < max_args - 1) {
        /* skip whitespace */
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0') break;
        /* start of token */
        args[argc++] = p;
        /* advance to next whitespace or end */
        while (*p != ' ' && *p != '\t' && *p != '\0') p++;
        if (*p != '\0') {
            *p = '\0';
            p++;
        }
    }
    args[argc] = NULL;
    return argc;
}

int shell_find_pipe(char *args[], int argc)
{
    for (int i = 0; i < argc; i++) {
        if (strcmp(args[i], "|") == 0) return i;
    }
    return -1;
}

int shell_is_builtin(const char *cmd)
{
    return strcmp(cmd, "cd") == 0
        || strcmp(cmd, "exit") == 0
        || strcmp(cmd, "help") == 0;
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
    printf("=== Mini Shell — Parsing Layer (Solution) ===\n\n");

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
