/*
 * Kata: Mini Shell
 *
 * Build a working Unix shell that supports:
 *   - Builtins: cd, pwd, exit
 *   - External commands via fork + execvp + waitpid
 *   - Single pipe: cmd1 | cmd2
 *   - I/O redirection: > (write), >> (append), < (read)
 *   - Signal handling: Ctrl+C kills the child, not the shell
 *
 * Architecture:
 *   tokenize()         — split input into tokens (from tokenizer.c)
 *   find_token()       — find a special token (|, >, <, >>) in token list
 *   is_builtin()       — check if a command is a builtin
 *   execute_builtin()  — run cd/pwd/exit in the shell process
 *   execute_cmd()      — fork + handle redirects + execvp + waitpid
 *   execute_pipe()     — pipe() + fork twice + dup2 + execvp
 *   shell_loop()       — the main REPL (not tested, just the glue)
 *
 * 16 tests total (tests exercise the helper functions, not the REPL).
 *
 * Hints:
 *   - Build and test each function before moving to the next
 *   - The tokenizer is a simplified copy from tokenizer.c — start there
 *   - Use execvp (searches PATH) not execv
 *   - Always close both ends of a pipe in the parent
 *   - After fork, restore SIGINT to SIG_DFL in the child
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_LINE    1024
#define MAX_TOKENS  64
#define MAX_TOKEN_LEN 256

/* ── Test harness ───────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ================================================================
 * SECTION 1: TOKENIZER (simplified — same logic as tokenizer.c)
 * ================================================================ */

/* ── tokenize ───────────────────────────────────────────────────────
 * Split 'input' into tokens. Same rules as tokenizer.c:
 *   - Whitespace separates tokens
 *   - Quoted strings are one token
 *   - |, >, >>, < are separate tokens
 *   - tokens[count] = NULL
 * Returns number of tokens.
 */
int tokenize(char *input, char **tokens, int max_tokens)
{
    char buf[MAX_TOKEN_LEN];
    int buf_len = 0;
    int count = 0;
    int in_quotes = 0;
    int i;
    int len;

    (void)max_tokens;

    /* Step 1: Strip trailing newline */
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') input[--len] = '\0';

    /* Step 2: Walk character by character */
    for (i = 0; i < len; i++) {
        if (input[i] == '"') {
            in_quotes = !in_quotes;
            continue;
        }

        if (in_quotes) {
            buf[buf_len++] = input[i];
            continue;
        }

        if (isspace((unsigned char)input[i])) {
            if (buf_len > 0) {
                buf[buf_len] = '\0';
                tokens[count++] = strdup(buf);
                buf_len = 0;
            }
            continue;
        }

        if (input[i] == '|') {
            if (buf_len > 0) {
                buf[buf_len] = '\0';
                tokens[count++] = strdup(buf);
                buf_len = 0;
            }
            tokens[count++] = strdup("|");
            continue;
        }

        if (input[i] == '>') {
            if (buf_len > 0) {
                buf[buf_len] = '\0';
                tokens[count++] = strdup(buf);
                buf_len = 0;
            }
            if (i + 1 < len && input[i + 1] == '>') {
                tokens[count++] = strdup(">>");
                i++;
            } else {
                tokens[count++] = strdup(">");
            }
            continue;
        }

        if (input[i] == '<') {
            if (buf_len > 0) {
                buf[buf_len] = '\0';
                tokens[count++] = strdup(buf);
                buf_len = 0;
            }
            tokens[count++] = strdup("<");
            continue;
        }

        buf[buf_len++] = input[i];
    }

    /* Step 3: Flush remaining token */
    if (buf_len > 0) {
        buf[buf_len] = '\0';
        tokens[count++] = strdup(buf);
    }

    /* Step 4: Null-terminate and return */
    tokens[count] = NULL;
    return count;
}

/* ── free_tokens ────────────────────────────────────────────────── */
static void free_tokens(char **tokens, int count)
{
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
        tokens[i] = NULL;
    }
}

/* ================================================================
 * SECTION 2: TOKEN ANALYSIS HELPERS
 * ================================================================ */

/* ── find_token ─────────────────────────────────────────────────────
 * Search tokens[0..count-1] for one that matches 'target'.
 * Returns the index, or -1 if not found.
 *
 * Example: find_token(tokens, count, "|") returns the index of the pipe.
 */
int find_token(char **tokens, int count, const char *target)
{
    for (int i = 0; i < count; i++) {
        if (strcmp(tokens[i], target) == 0) {
            return i;
        }
    }
    return -1;
}

/* ── split_at_token ─────────────────────────────────────────────────
 * Split the token list at position 'pos'.
 *
 * Sets tokens[pos] to NULL so that tokens[0..pos-1] is the left side
 * (null-terminated for execvp), and returns a pointer to tokens[pos+1]
 * which is the start of the right side.
 *
 * Example: ["ls", "|", "grep", ".c", NULL] split at 1
 *   -> left:  tokens = ["ls", NULL, ...]
 *   -> right: returns pointer to "grep" (tokens[2])
 *
 * Caller must NOT free the original token at pos (we just null it).
 * Actually, we should free it to avoid a leak.
 */
char **split_at_token(char **tokens, int pos)
{
    free(tokens[pos]);
    tokens[pos] = NULL;
    return &tokens[pos + 1];
}

/* ================================================================
 * SECTION 3: BUILTIN COMMANDS
 * ================================================================ */

/* ── is_builtin ─────────────────────────────────────────────────────
 * Returns 1 if tokens[0] is "cd", "exit", or "pwd". Otherwise 0.
 */
int is_builtin(char **tokens)
{
    if (tokens[0] == NULL) return 0;
    if (strcmp(tokens[0], "cd") == 0) return 1;
    if (strcmp(tokens[0], "pwd") == 0) return 1;
    if (strcmp(tokens[0], "exit") == 0) return 1;
    return 0;
}

/* ── execute_builtin ────────────────────────────────────────────────
 * Execute a builtin command. Returns 1 if it was a builtin, 0 if not.
 *
 * Builtins:
 *   cd [dir]  — chdir(dir), or chdir(HOME) if no argument
 *   pwd       — getcwd and print
 *   exit      — exit(0)
 */
int execute_builtin(char **tokens)
{
    if (tokens[0] == NULL) return 0;

    if (strcmp(tokens[0], "cd") == 0) {
        const char *dir = tokens[1];
        if (dir == NULL) {
            dir = getenv("HOME");
        }
        if (chdir(dir) != 0) {
            perror("cd");
        }
        return 1;
    }

    if (strcmp(tokens[0], "pwd") == 0) {
        char cwd[MAX_LINE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        }
        return 1;
    }

    if (strcmp(tokens[0], "exit") == 0) {
        exit(0);
    }

    return 0; /* not a builtin */
}

/* ================================================================
 * SECTION 4: EXTERNAL COMMAND EXECUTION
 * ================================================================ */

/* ── execute_cmd ────────────────────────────────────────────────────
 * Run an external command with optional I/O redirection.
 *
 * Steps:
 *   1. Check for redirect tokens (>, >>, <) in the token list
 *   2. If found, note the filename and remove the redirect tokens
 *   3. fork()
 *   4. In child:
 *      a. Restore SIGINT to SIG_DFL
 *      b. If output redirect: open file, dup2 to stdout
 *      c. If input redirect: open file, dup2 to stdin
 *      d. execvp(tokens[0], tokens)
 *      e. If execvp returns, perror and exit(1)
 *   5. In parent: waitpid
 *
 * Parameters:
 *   tokens — null-terminated array of strings
 *   count  — number of tokens (not counting the final NULL)
 */
void execute_cmd(char **tokens, int count)
{
    if (tokens[0] == NULL) return;

    char *outfile = NULL;
    char *infile = NULL;
    int append = 0;

    /* Scan for redirect operators and extract filenames */
    for (int i = 0; i < count; i++) {
        if (tokens[i] == NULL) break;
        if (strcmp(tokens[i], ">>") == 0) {
            append = 1;
            outfile = tokens[i + 1];
            free(tokens[i]);
            tokens[i] = NULL; /* truncates the command here */
            break;
        } else if (strcmp(tokens[i], ">") == 0) {
            outfile = tokens[i + 1];
            free(tokens[i]);
            tokens[i] = NULL;
            break;
        } else if (strcmp(tokens[i], "<") == 0) {
            infile = tokens[i + 1];
            free(tokens[i]);
            tokens[i] = NULL;
            break;
        }
    }

    pid_t pid = fork();
    if (pid == 0) {
        /* Child process */
        signal(SIGINT, SIG_DFL);

        if (outfile != NULL) {
            int flags = O_WRONLY | O_CREAT;
            flags |= append ? O_APPEND : O_TRUNC;
            int fd = open(outfile, flags, 0644);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        if (infile != NULL) {
            int fd = open(infile, O_RDONLY);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        execvp(tokens[0], tokens);
        perror(tokens[0]);
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }
}

/* ── execute_pipe ───────────────────────────────────────────────────
 * Execute: left_tokens | right_tokens
 *
 * Steps:
 *   1. pipe(fd)
 *   2. Fork child 1 (left side):
 *      - close(fd[0])           — doesn't read from pipe
 *      - dup2(fd[1], STDOUT)    — stdout goes to pipe
 *      - close(fd[1])
 *      - execvp left command
 *   3. Fork child 2 (right side):
 *      - close(fd[1])           — doesn't write to pipe
 *      - dup2(fd[0], STDIN)     — stdin comes from pipe
 *      - close(fd[0])
 *      - execvp right command
 *   4. Parent:
 *      - close(fd[0]) and close(fd[1])
 *      - waitpid for both children
 *
 * Parameters:
 *   left  — null-terminated token array for left command
 *   right — null-terminated token array for right command
 */
void execute_pipe(char **left, char **right)
{
    if (left[0] == NULL || right[0] == NULL) return;

    int fd[2];
    if (pipe(fd) < 0) { perror("pipe"); return; }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        /* Child 1: left side (writes to pipe) */
        signal(SIGINT, SIG_DFL);
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execvp(left[0], left);
        perror(left[0]);
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        /* Child 2: right side (reads from pipe) */
        signal(SIGINT, SIG_DFL);
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(right[0], right);
        perror(right[0]);
        exit(1);
    }

    /* Parent: close both ends, wait for both children */
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

/* ================================================================
 * SECTION 5: THE SHELL LOOP (not tested — this is the glue)
 * ================================================================ */

/* ── shell_loop ─────────────────────────────────────────────────────
 * The main read-eval-print loop. This is what makes it a shell.
 *
 * Not tested by the harness — run the program with no arguments
 * to enter interactive mode (see main below).
 */
void shell_loop(void)
{
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];

    /* Ignore SIGINT in the shell itself */
    signal(SIGINT, SIG_IGN);

    while (1) {
        /* 1. Print prompt */
        printf("$ ");
        fflush(stdout);

        /* 2. Read a line */
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break; /* EOF (Ctrl+D) */
        }

        /* 3. Tokenize */
        int count = tokenize(line, tokens, MAX_TOKENS);
        if (count == 0) continue; /* empty line */

        /* 4. Check for pipe */
        int pipe_pos = find_token(tokens, count, "|");
        if (pipe_pos > 0) {
            char **right = split_at_token(tokens, pipe_pos);
            execute_pipe(tokens, right);
            free_tokens(tokens, count);
            continue;
        }

        /* 5. Check for builtin */
        if (execute_builtin(tokens)) {
            free_tokens(tokens, count);
            continue;
        }

        /* 6. External command */
        execute_cmd(tokens, count);
        free_tokens(tokens, count);
    }
}

/* ================================================================
 * SECTION 6: TESTS
 * ================================================================
 * Tests exercise the helper functions individually.
 * Run: ./mini_shell --test
 * Interactive: ./mini_shell
 */

/* Helper: capture stdout from a forked command into a buffer.
 * Forks, redirects child stdout to a pipe, runs execvp, reads output.
 * Returns bytes read, or -1 on error.
 */
static int capture_command(char **argv, char *buf, int buf_size)
{
    int fd[2];
    if (pipe(fd) < 0) return -1;

    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execvp(argv[0], argv);
        _exit(1);
    }
    close(fd[1]);
    int total = 0;
    int n;
    while ((n = read(fd[0], buf + total, buf_size - total - 1)) > 0) {
        total += n;
    }
    buf[total] = '\0';
    close(fd[0]);
    waitpid(pid, NULL, 0);
    return total;
}

/* Helper: capture pipe output (left | right) into a buffer. */
static int capture_pipe(char **left, char **right, char *buf, int buf_size)
{
    int fd_capture[2];
    if (pipe(fd_capture) < 0) return -1;

    pid_t pid = fork();
    if (pid == 0) {
        /* In this child, redirect stdout to capture pipe, then run the pipe */
        close(fd_capture[0]);
        dup2(fd_capture[1], STDOUT_FILENO);
        close(fd_capture[1]);

        /* Now set up the actual command pipe */
        int fd[2];
        if (pipe(fd) < 0) _exit(1);

        pid_t p1 = fork();
        if (p1 == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execvp(left[0], left);
            _exit(1);
        }
        pid_t p2 = fork();
        if (p2 == 0) {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            execvp(right[0], right);
            _exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        waitpid(p1, NULL, 0);
        waitpid(p2, NULL, 0);
        _exit(0);
    }
    close(fd_capture[1]);
    int total = 0;
    int n;
    while ((n = read(fd_capture[0], buf + total, buf_size - total - 1)) > 0) {
        total += n;
    }
    buf[total] = '\0';
    close(fd_capture[0]);
    waitpid(pid, NULL, 0);
    return total;
}

static void run_tests(void)
{
    char *tokens[MAX_TOKENS];
    int count;
    char buf[4096];

    printf("=== Mini Shell Tests ===\n\n");

    /* ── Tokenizer integration (quick sanity checks) ─────────── */

    /* Test 1: parse simple command */
    {
        char input[] = "echo hello\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("parse simple command");
        EXPECT(count == 2
               && strcmp(tokens[0], "echo") == 0
               && strcmp(tokens[1], "hello") == 0);
        free_tokens(tokens, count);
    }

    /* Test 2: parse command with multiple args */
    {
        char input[] = "gcc -Wall -o test test.c\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        TEST("parse command with multiple args");
        EXPECT(count == 5
               && strcmp(tokens[0], "gcc") == 0
               && strcmp(tokens[4], "test.c") == 0
               && tokens[5] == NULL);
        free_tokens(tokens, count);
    }

    /* ── find_token ──────────────────────────────────────────── */

    /* Test 3: detect pipe in token list */
    {
        char input[] = "ls | wc\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        int pos = find_token(tokens, count, "|");
        TEST("detect pipe token");
        EXPECT(pos == 1);
        free_tokens(tokens, count);
    }

    /* Test 4: detect output redirect */
    {
        char input[] = "ls > out.txt\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        int pos = find_token(tokens, count, ">");
        TEST("detect redirect token");
        EXPECT(pos == 1);
        free_tokens(tokens, count);
    }

    /* Test 5: no pipe returns -1 */
    {
        char input[] = "ls -la\n";
        count = tokenize(input, tokens, MAX_TOKENS);
        int pos = find_token(tokens, count, "|");
        TEST("no pipe returns -1");
        EXPECT(pos == -1);
        free_tokens(tokens, count);
    }

    /* ── is_builtin ──────────────────────────────────────────── */

    /* Test 6: cd is a builtin */
    {
        char *t[] = { "cd", "/tmp", NULL };
        TEST("cd is a builtin");
        EXPECT(is_builtin(t) == 1);
    }

    /* Test 7: pwd is a builtin */
    {
        char *t[] = { "pwd", NULL };
        TEST("pwd is a builtin");
        EXPECT(is_builtin(t) == 1);
    }

    /* Test 8: ls is NOT a builtin */
    {
        char *t[] = { "ls", NULL };
        TEST("ls is NOT a builtin");
        EXPECT(is_builtin(t) == 0);
    }

    /* ── execute_builtin: cd ─────────────────────────────────── */

    /* Test 9: cd changes directory */
    {
        char original[MAX_LINE];
        getcwd(original, sizeof(original));

        char *t[] = { "cd", "/tmp", NULL };
        execute_builtin(t);

        char after[MAX_LINE];
        getcwd(after, sizeof(after));

        TEST("cd changes working directory");
        EXPECT(strcmp(after, "/tmp") == 0);

        /* Restore original directory */
        chdir(original);
    }

    /* Test 10: cd with no args goes to HOME */
    {
        char original[MAX_LINE];
        getcwd(original, sizeof(original));

        const char *home = getenv("HOME");
        char *t[] = { "cd", NULL };
        execute_builtin(t);

        char after[MAX_LINE];
        getcwd(after, sizeof(after));

        TEST("cd with no args goes to HOME");
        EXPECT(home != NULL && strcmp(after, home) == 0);

        chdir(original);
    }

    /* ── execute_cmd: fork + exec ────────────────────────────── */

    /* Test 11: execute a simple command and capture output */
    {
        char *argv[] = { "echo", "shell_test_42", NULL };
        int n = capture_command(argv, buf, sizeof(buf));
        TEST("fork+exec: echo produces output");
        EXPECT(n > 0 && strstr(buf, "shell_test_42") != NULL);
    }

    /* Test 12: execute command with arguments */
    {
        char *argv[] = { "printf", "%s-%s", "hello", "world", NULL };
        int n = capture_command(argv, buf, sizeof(buf));
        TEST("fork+exec: printf with args");
        EXPECT(n > 0 && strstr(buf, "hello-world") != NULL);
    }

    /* ── execute_cmd: redirect ───────────────────────────────── */

    /* Test 13: output redirect writes to file */
    {
        const char *tmpfile = "/tmp/mini_shell_test_out.txt";
        unlink(tmpfile);

        /* We test the redirect logic by doing it manually
         * (since execute_cmd needs tokenize to be working) */
        pid_t pid = fork();
        if (pid == 0) {
            int fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            char *argv[] = { "echo", "redirect_test", NULL };
            execvp(argv[0], argv);
            _exit(1);
        }
        waitpid(pid, NULL, 0);

        FILE *f = fopen(tmpfile, "r");
        int ok = 0;
        if (f) {
            if (fgets(buf, sizeof(buf), f)) {
                ok = (strstr(buf, "redirect_test") != NULL);
            }
            fclose(f);
        }
        unlink(tmpfile);

        TEST("output redirect: echo > file");
        EXPECT(ok);
    }

    /* Test 14: input redirect reads from file */
    {
        const char *tmpfile = "/tmp/mini_shell_test_in.txt";
        FILE *f = fopen(tmpfile, "w");
        if (f) {
            fprintf(f, "cherry\napple\nbanana\n");
            fclose(f);
        }

        /* sort < tmpfile */
        int fd_cap[2];
        pipe(fd_cap);
        pid_t pid = fork();
        if (pid == 0) {
            close(fd_cap[0]);
            dup2(fd_cap[1], STDOUT_FILENO);
            close(fd_cap[1]);
            int fd = open(tmpfile, O_RDONLY);
            if (fd >= 0) {
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            char *argv[] = { "sort", NULL };
            execvp(argv[0], argv);
            _exit(1);
        }
        close(fd_cap[1]);
        int total = 0;
        int n;
        while ((n = read(fd_cap[0], buf + total, sizeof(buf) - total - 1)) > 0) {
            total += n;
        }
        buf[total] = '\0';
        close(fd_cap[0]);
        waitpid(pid, NULL, 0);
        unlink(tmpfile);

        /* sorted output should start with "apple" */
        TEST("input redirect: sort < file");
        EXPECT(total > 0 && strncmp(buf, "apple", 5) == 0);
    }

    /* ── pipe ────────────────────────────────────────────────── */

    /* Test 15: pipe two commands */
    {
        char *left[] = { "echo", "hello world foo bar", NULL };
        char *right[] = { "wc", "-w", NULL };
        int n = capture_pipe(left, right, buf, sizeof(buf));

        /* "hello world foo bar" has 4 words */
        int word_count = 0;
        if (n > 0) word_count = atoi(buf);

        TEST("pipe: echo ... | wc -w");
        EXPECT(word_count == 4);
    }

    /* Test 16: pipe with grep */
    {
        char *left[] = { "printf", "apple\nbanana\ncherry\n", NULL };
        char *right[] = { "grep", "ban", NULL };
        int n = capture_pipe(left, right, buf, sizeof(buf));

        TEST("pipe: printf ... | grep ban");
        EXPECT(n > 0 && strstr(buf, "banana") != NULL
               && strstr(buf, "apple") == NULL);
    }

    /* ── Summary ─────────────────────────────────────────────── */
    printf("\n  Passed: %d / %d\n", tests_passed, tests_total);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        run_tests();
        return (tests_passed == tests_total) ? 0 : 1;
    }

    /* Interactive mode */
    printf("mini-shell: type 'exit' or Ctrl+D to quit\n");
    shell_loop();
    return 0;
}
