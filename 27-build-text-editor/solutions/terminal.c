/*
 * Solution: Terminal Control — Raw Mode & Screen Management
 *
 * This module teaches you how to take control of the terminal: disabling
 * line buffering and echo (raw mode), querying the terminal size, and
 * generating ANSI escape sequences for cursor positioning and screen clearing.
 *
 * 6 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

/* ── Test Harness ────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ── Terminal Structure ─────────────────────────────────────────── */

typedef struct {
    struct termios orig;  /* saved original terminal settings */
    int is_raw;           /* 1 if currently in raw mode */
} Terminal;

/* ── Implementation ─────────────────────────────────────────────── */

/*
 * terminal_init: Save the current terminal settings and mark as not raw.
 */
void terminal_init(Terminal *t)
{
    tcgetattr(STDIN_FILENO, &t->orig);
    t->is_raw = 0;
}

/*
 * terminal_enable_raw: Switch the terminal into raw mode.
 *
 * Raw mode disables:
 *   ECHO   — don't echo typed characters
 *   ICANON — don't wait for Enter (send each keypress immediately)
 *   ISIG   — don't let Ctrl-C / Ctrl-Z generate signals
 *   IXON   — don't intercept Ctrl-S / Ctrl-Q for flow control
 *   ICRNL  — don't translate CR to NL
 *   OPOST  — don't translate NL to CR+NL on output
 *
 * VMIN=0, VTIME=1 means read() returns after 100ms even with no input.
 */
void terminal_enable_raw(Terminal *t)
{
    struct termios raw = t->orig;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    t->is_raw = 1;
}

/*
 * terminal_disable_raw: Restore the original terminal settings.
 */
void terminal_disable_raw(Terminal *t)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &t->orig);
    t->is_raw = 0;
}

/*
 * terminal_get_size: Query the terminal dimensions using ioctl.
 * Returns 0 on success, -1 on failure.
 */
int terminal_get_size(int *rows, int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
}

/*
 * terminal_clear_screen: Write the ANSI escape sequence that clears the
 * entire screen and moves the cursor to the top-left corner.
 *
 * Output goes into `out` (not directly to stdout) so we can test it.
 */
int terminal_clear_screen(char *out, int out_size)
{
    return snprintf(out, out_size, "\x1b[2J\x1b[H");
}

/*
 * terminal_set_cursor: Write the ANSI escape sequence that moves the
 * cursor to the given row and column (1-indexed, as ANSI expects).
 *
 * Output goes into `out` so we can test it.
 */
int terminal_set_cursor(char *out, int out_size, int row, int col)
{
    return snprintf(out, out_size, "\x1b[%d;%dH", row, col);
}

/* ── Tests ──────────────────────────────────────────────────────── */

int main(void)
{
    printf("Terminal Control Tests\n");
    printf("======================\n\n");

    char buf[64];

    /* --- terminal_init --- */
    {
        Terminal t;
        terminal_init(&t);

        TEST("init: is_raw starts at 0")
            EXPECT(t.is_raw == 0);
    }

    /* --- enable/disable raw toggle --- */
    {
        Terminal t;
        terminal_init(&t);
        terminal_enable_raw(&t);

        TEST("enable_raw: sets is_raw to 1")
            EXPECT(t.is_raw == 1);

        terminal_disable_raw(&t);

        TEST("disable_raw: sets is_raw back to 0")
            EXPECT(t.is_raw == 0);
    }

    /* --- terminal_get_size --- */
    {
        int rows = 0, cols = 0;
        int rc = terminal_get_size(&rows, &cols);

        /* ioctl may fail if stdout is not a terminal (e.g. piped output).
         * We test that it either succeeds with valid values, or fails cleanly. */
        TEST("get_size: succeeds with valid dimensions or fails cleanly")
            EXPECT((rc == 0 && rows > 0 && cols > 0) || rc == -1);
    }

    /* --- terminal_clear_screen --- */
    {
        terminal_clear_screen(buf, sizeof(buf));

        TEST("clear_screen: produces correct escape sequence")
            EXPECT(strcmp(buf, "\x1b[2J\x1b[H") == 0);
    }

    /* --- terminal_set_cursor --- */
    {
        terminal_set_cursor(buf, sizeof(buf), 5, 12);

        TEST("set_cursor: row 5, col 12 produces '\\x1b[5;12H'")
            EXPECT(strcmp(buf, "\x1b[5;12H") == 0);
    }

    /* --- Summary --- */
    printf("\nResults: %d / %d passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
