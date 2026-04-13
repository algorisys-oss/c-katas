/*
 * Exercise: Terminal Control — Raw Mode & Screen Management
 *
 * This module teaches you how to take control of the terminal: disabling
 * line buffering and echo (raw mode), querying the terminal size, and
 * generating ANSI escape sequences for cursor positioning and screen clearing.
 *
 * Key concepts:
 *   - termios API: tcgetattr/tcsetattr to change terminal behavior
 *   - Bitwise flag manipulation: &= ~(FLAG) to turn off a flag
 *   - ioctl(TIOCGWINSZ): query the terminal's row/column count
 *   - ANSI escape sequences: \x1b[...  control codes for the terminal
 *
 * Implement all functions marked TODO.
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

/* ── Functions to Implement ─────────────────────────────────────── */

/*
 * terminal_init: Save the current terminal settings into t->orig.
 *   - Use tcgetattr(STDIN_FILENO, ...) to read current settings
 *   - Set is_raw to 0
 *
 * TODO: implement this function
 */
void terminal_init(Terminal *t)
{
    (void)t;
    /* TODO: save current terminal settings, set is_raw = 0 */
}

/*
 * terminal_enable_raw: Switch the terminal into raw mode.
 *
 *   Start from t->orig, then disable these flags:
 *     Local flags (c_lflag):
 *       ECHO   — don't echo typed characters
 *       ICANON — send keypresses immediately (don't wait for Enter)
 *       ISIG   — don't let Ctrl-C/Ctrl-Z generate signals
 *     Input flags (c_iflag):
 *       IXON   — disable Ctrl-S/Ctrl-Q flow control
 *       ICRNL  — don't translate carriage return to newline
 *     Output flags (c_oflag):
 *       OPOST  — don't translate \n to \r\n on output
 *
 *   Also set:
 *     c_cc[VMIN]  = 0   (read returns immediately with 0 bytes if no input)
 *     c_cc[VTIME] = 1   (100ms timeout for read)
 *
 *   Apply with tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)
 *   Set is_raw to 1
 *
 * TODO: implement this function
 */
void terminal_enable_raw(Terminal *t)
{
    (void)t;
    /* TODO: apply raw mode flags, set is_raw = 1 */
}

/*
 * terminal_disable_raw: Restore the original terminal settings.
 *   - Use tcsetattr to restore t->orig
 *   - Set is_raw to 0
 *
 * TODO: implement this function
 */
void terminal_disable_raw(Terminal *t)
{
    (void)t;
    /* TODO: restore original settings, set is_raw = 0 */
}

/*
 * terminal_get_size: Query the terminal dimensions.
 *   - Use ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)
 *   - If it fails or ws_col is 0, return -1
 *   - Otherwise set *rows = ws.ws_row, *cols = ws.ws_col, return 0
 *
 * TODO: implement this function
 */
int terminal_get_size(int *rows, int *cols)
{
    (void)rows;
    (void)cols;
    /* TODO: use ioctl to get terminal size */
    return -1;
}

/*
 * terminal_clear_screen: Write the ANSI escape sequence that clears the
 * entire screen and moves the cursor to the top-left corner.
 *
 *   The sequence is: "\x1b[2J\x1b[H"
 *     \x1b[2J — clear entire screen
 *     \x1b[H  — move cursor to row 1, col 1 (home)
 *
 *   Write the result into `out` using snprintf (not directly to stdout).
 *   Return the number of characters written.
 *
 * TODO: implement this function
 */
int terminal_clear_screen(char *out, int out_size)
{
    (void)out;
    (void)out_size;
    /* TODO: snprintf the escape sequence into out */
    return 0;
}

/*
 * terminal_set_cursor: Write the ANSI escape sequence that moves the
 * cursor to the given position.
 *
 *   ANSI uses 1-indexed row and column:  "\x1b[{row};{col}H"
 *   Example: row=3, col=7 → "\x1b[3;7H"
 *
 *   Write the result into `out` using snprintf.
 *   Return the number of characters written.
 *
 * TODO: implement this function
 */
int terminal_set_cursor(char *out, int out_size, int row, int col)
{
    (void)out;
    (void)out_size;
    (void)row;
    (void)col;
    /* TODO: snprintf the cursor positioning escape sequence into out */
    return 0;
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
