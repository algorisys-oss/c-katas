/*
 * Exercise: Keyboard Input — Reading & Decoding Keys
 *
 * When the terminal is in raw mode, every keypress arrives as one or more
 * bytes on stdin. Printable characters are a single byte (their ASCII value).
 * Special keys like arrows, Home, End, Delete, and Page Up/Down are sent as
 * multi-byte **escape sequences** starting with 0x1b (ESC).
 *
 * Your task: implement read_key_from_buf() which parses a byte buffer and
 * returns the key code. This is separated from actual I/O so it's testable.
 *
 * Escape sequence parsing tree:
 *
 *   Read byte:
 *     ├─ printable (32-126) → return as-is
 *     ├─ 0x1b (ESC) → need at least 3 bytes, read buf[1]:
 *     │   ├─ '[' → read buf[2]:
 *     │   │   ├─ 'A' → ARROW_UP
 *     │   │   ├─ 'B' → ARROW_DOWN
 *     │   │   ├─ 'C' → ARROW_RIGHT
 *     │   │   ├─ 'D' → ARROW_LEFT
 *     │   │   ├─ 'H' → HOME
 *     │   │   ├─ 'F' → END
 *     │   │   └─ digit (need 4 bytes, buf[3] must be '~'):
 *     │   │       ├─ "1~" → HOME
 *     │   │       ├─ "3~" → DELETE
 *     │   │       ├─ "4~" → END
 *     │   │       ├─ "5~" → PAGE_UP
 *     │   │       └─ "6~" → PAGE_DOWN
 *     │   └─ other → return ESC (0x1b)
 *     ├─ 127 → BACKSPACE
 *     └─ 1-26 → Ctrl+key (returned as-is)
 *
 * Implement all functions marked TODO.
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

/* Ctrl+key strips bits 5-6, leaving only the lower 5 bits.
 * Example: CTRL_KEY('q') = 'q' & 0x1f = 0x71 & 0x1f = 0x11 = 17 */
#define CTRL_KEY(k) ((k) & 0x1f)

/* ── Functions to Implement ─────────────────────────────────────── */

/*
 * read_key_from_buf: Parse the first key from a byte buffer.
 *
 *   Given a buffer `buf` of `len` bytes, decode the first key.
 *
 *   Steps:
 *     1. If len <= 0, return KEY_NONE
 *     2. Read first byte as unsigned char `c`
 *     3. If c == 0x1b (ESC):
 *        a. If len < 3, return bare ESC (0x1b)
 *        b. If buf[1] == '[':
 *           - If buf[2] is a digit ('0'-'9'):
 *             Check if len >= 4 and buf[3] == '~', then switch on buf[2]
 *           - Otherwise switch on buf[2] for letter codes (A/B/C/D/H/F)
 *        c. Otherwise return 0x1b
 *     4. For all other bytes, return c as-is
 *
 * TODO: implement this function
 */
int read_key_from_buf(const char *buf, int len)
{
    (void)buf;
    (void)len;
    /* TODO: implement the key parsing logic described above */
    return KEY_NONE;
}

/*
 * read_key: Read a key from stdin (the live version used in the editor).
 *
 *   1. Read up to 4 bytes from STDIN_FILENO into a local buffer
 *   2. If read returns 0, try again (timeout with no input)
 *   3. If read returns -1, return KEY_NONE (error)
 *   4. Call read_key_from_buf with the bytes you got
 *
 * TODO: implement this function
 */
int read_key(void)
{
    /* TODO: read bytes from stdin and delegate to read_key_from_buf */
    return KEY_NONE;
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
