/*
 * Solution: Keyboard Input — Reading & Decoding Keys
 *
 * When the terminal is in raw mode, every keypress arrives as one or more
 * bytes on stdin. Printable characters are a single byte (their ASCII value).
 * Special keys like arrows, Home, End, Delete, and Page Up/Down are sent as
 * multi-byte **escape sequences** starting with 0x1b (ESC).
 *
 * This module implements a key parser that decodes byte sequences into a
 * single integer key code. We separate the parsing logic (read_key_from_buf)
 * from the actual read() call so the parser is fully testable.
 *
 * 10 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

/* ── Key Codes ──────────────────────────────────────────────────── */

/*
 * Printable ASCII characters (32-126) keep their natural value.
 * Special keys get codes starting at 1000 so they never collide with ASCII.
 */
enum EditorKey {
    KEY_NONE       = 0,
    KEY_BACKSPACE  = 127,
    KEY_ARROW_UP   = 1000,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_DELETE
};

/* Ctrl+key strips bits 5-6, leaving only the lower 5 bits. */
#define CTRL_KEY(k) ((k) & 0x1f)

/* ── Implementation ─────────────────────────────────────────────── */

/*
 * read_key_from_buf: Parse the first key from a byte buffer.
 *
 * This is the testable core of the input system. Given a buffer of raw
 * bytes and its length, it decodes the first key and returns its code.
 *
 * Escape sequence parsing:
 *
 *   Read byte:
 *     ├─ printable (32-126) → return as-is
 *     ├─ 0x1b (ESC) → read next byte:
 *     │   ├─ '[' → read next byte:
 *     │   │   ├─ 'A' → ARROW_UP
 *     │   │   ├─ 'B' → ARROW_DOWN
 *     │   │   ├─ 'C' → ARROW_RIGHT
 *     │   │   ├─ 'D' → ARROW_LEFT
 *     │   │   ├─ 'H' → HOME
 *     │   │   ├─ 'F' → END
 *     │   │   └─ digit → read until '~':
 *     │   │       ├─ "1~" → HOME
 *     │   │       ├─ "3~" → DELETE
 *     │   │       ├─ "4~" → END
 *     │   │       ├─ "5~" → PAGE_UP
 *     │   │       └─ "6~" → PAGE_DOWN
 *     │   └─ other → return ESC (0x1b)
 *     ├─ 127 → BACKSPACE
 *     └─ 1-26 → Ctrl+key (CTRL_KEY)
 */
int read_key_from_buf(const char *buf, int len)
{
    if (len <= 0) {
        return KEY_NONE;
    }

    unsigned char c = buf[0];

    /* Escape sequence */
    if (c == 0x1b) {
        if (len < 3) {
            return 0x1b;  /* bare ESC key */
        }
        if (buf[1] == '[') {
            /* CSI sequence: ESC [ ... */
            if (buf[2] >= '0' && buf[2] <= '9') {
                /* Extended sequence: ESC [ digit ~ */
                if (len >= 4 && buf[3] == '~') {
                    switch (buf[2]) {
                    case '1': return KEY_HOME;
                    case '3': return KEY_DELETE;
                    case '4': return KEY_END;
                    case '5': return KEY_PAGE_UP;
                    case '6': return KEY_PAGE_DOWN;
                    }
                }
                return 0x1b;
            }
            /* Letter after CSI */
            switch (buf[2]) {
            case 'A': return KEY_ARROW_UP;
            case 'B': return KEY_ARROW_DOWN;
            case 'C': return KEY_ARROW_RIGHT;
            case 'D': return KEY_ARROW_LEFT;
            case 'H': return KEY_HOME;
            case 'F': return KEY_END;
            }
        }
        return 0x1b;
    }

    /* All other bytes pass through as-is (including 127 for backspace
     * and 1-26 for Ctrl+key combos). */
    return c;
}

/*
 * read_key: Read a key from stdin (the live version used in the editor).
 *
 * Calls read() to get bytes, then delegates to read_key_from_buf.
 * This function blocks until a key is available (or the 100ms VTIME
 * timeout fires if raw mode is configured).
 */
int read_key(void)
{
    char buf[4];
    int nread;

    /* Keep trying until we get at least one byte */
    while ((nread = read(STDIN_FILENO, buf, sizeof(buf))) == 0) {
        /* timeout — try again */
    }
    if (nread == -1) {
        return KEY_NONE;
    }
    return read_key_from_buf(buf, nread);
}

/* ── Tests ──────────────────────────────────────────────────────── */

int main(void)
{
    printf("Keyboard Input Tests\n");
    printf("=====================\n\n");

    /* --- Printable character --- */
    {
        int key = read_key_from_buf("a", 1);

        TEST("printable: 'a' returns 'a'")
            EXPECT(key == 'a');
    }

    /* --- Ctrl+Q --- */
    {
        char buf[1] = { CTRL_KEY('q') };
        int key = read_key_from_buf(buf, 1);

        TEST("ctrl: Ctrl+Q returns 0x11")
            EXPECT(key == CTRL_KEY('q'));
    }

    /* --- Ctrl+S --- */
    {
        char buf[1] = { CTRL_KEY('s') };
        int key = read_key_from_buf(buf, 1);

        TEST("ctrl: Ctrl+S returns 0x13")
            EXPECT(key == CTRL_KEY('s'));
    }

    /* --- Arrow Up --- */
    {
        int key = read_key_from_buf("\x1b[A", 3);

        TEST("escape: ESC[A returns ARROW_UP")
            EXPECT(key == KEY_ARROW_UP);
    }

    /* --- Arrow Down --- */
    {
        int key = read_key_from_buf("\x1b[B", 3);

        TEST("escape: ESC[B returns ARROW_DOWN")
            EXPECT(key == KEY_ARROW_DOWN);
    }

    /* --- Arrow Right --- */
    {
        int key = read_key_from_buf("\x1b[C", 3);

        TEST("escape: ESC[C returns ARROW_RIGHT")
            EXPECT(key == KEY_ARROW_RIGHT);
    }

    /* --- Arrow Left --- */
    {
        int key = read_key_from_buf("\x1b[D", 3);

        TEST("escape: ESC[D returns ARROW_LEFT")
            EXPECT(key == KEY_ARROW_LEFT);
    }

    /* --- Home key (ESC[H) --- */
    {
        int key = read_key_from_buf("\x1b[H", 3);

        TEST("escape: ESC[H returns HOME")
            EXPECT(key == KEY_HOME);
    }

    /* --- Delete key (ESC[3~) --- */
    {
        int key = read_key_from_buf("\x1b[3~", 4);

        TEST("escape: ESC[3~ returns DELETE")
            EXPECT(key == KEY_DELETE);
    }

    /* --- Page Down (ESC[6~) --- */
    {
        int key = read_key_from_buf("\x1b[6~", 4);

        TEST("escape: ESC[6~ returns PAGE_DOWN")
            EXPECT(key == KEY_PAGE_DOWN);
    }

    /* --- Summary --- */
    printf("\nResults: %d / %d passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
