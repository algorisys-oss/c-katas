# Module 22 — Building a Text Editor

Welcome to one of the most exciting modules in the entire course. You are going
to build the core pieces of a **real text editor** — the kind of program that
developers use every single day. By the end, you will understand how programs
like `vim`, `nano`, and `micro` actually work under the hood.

We will build this up piece by piece:

1. **Gap buffer** — the data structure that makes editing text fast
2. **Screen buffer** — a 2D grid of characters for rendering
3. **Terminal raw mode** — taking control of the terminal
4. **ANSI escape codes** — drawing to the screen
5. **Event loop** — the heartbeat of any interactive program

---

## 1. The Gap Buffer — The Heart of a Text Editor

### Why not just use a string?

Imagine you have a 10,000-character document stored in a `char` array. The
cursor is at position 50, and you type a single character. What happens?

With a plain array, you have to **shift 9,950 characters** one position to the
right to make room. Every. Single. Keystroke. That is O(n) per insertion, and
it makes editing large files painfully slow.

### The insight: keep a gap at the cursor

A **gap buffer** is a simple but brilliant trick. Instead of a plain array, you
keep a **gap** (a chunk of unused space) right at the cursor position. When you
type a character, it goes into the gap — no shifting needed. O(1) insertion.

When you move the cursor, you shift the gap to the new position. But you only
pay that cost when the cursor *moves*, not when you *type*. And most editing is
typing several characters in a row at the same spot.

### How it works

A gap buffer has four key values:

```
buf       = the underlying character array (the "buffer")
buf_size  = total capacity of the buffer
gap_start = index where the gap begins (= cursor position)
gap_end   = index just past the end of the gap
```

The **text** is everything in `buf` that is NOT in the gap:
- Text before cursor: `buf[0 .. gap_start-1]`
- Text after cursor:  `buf[gap_end .. buf_size-1]`

The **gap** is the unused space: `buf[gap_start .. gap_end-1]`

### Visualizing gap buffer operations

**Initial state** — empty buffer, capacity 10:

```
Index:  0   1   2   3   4   5   6   7   8   9
      +---+---+---+---+---+---+---+---+---+---+
buf:  | . | . | . | . | . | . | . | . | . | . |
      +---+---+---+---+---+---+---+---+---+---+
       ^                                       ^
       gap_start=0                    gap_end=10

Text: ""   (gap is the entire buffer)
```

**After inserting 'H', 'e', 'l', 'l', 'o':**

```
Index:  0   1   2   3   4   5   6   7   8   9
      +---+---+---+---+---+---+---+---+---+---+
buf:  | H | e | l | l | o | . | . | . | . | . |
      +---+---+---+---+---+---+---+---+---+---+
                           ^                   ^
                    gap_start=5       gap_end=10

Text: "Hello"   cursor is after 'o'
```

**After `move_left()` twice** — cursor moves to position 3:

Characters 'l' and 'o' are copied from before the gap to after the gap:

```
Index:  0   1   2   3   4   5   6   7   8   9
      +---+---+---+---+---+---+---+---+---+---+
buf:  | H | e | l | . | . | . | . | . | l | o |
      +---+---+---+---+---+---+---+---+---+---+
                   ^                       ^
            gap_start=3              gap_end=8

Text: "Hello"   cursor is between 'l' and 'l'
```

**After inserting 'X'** at the cursor:

```
Index:  0   1   2   3   4   5   6   7   8   9
      +---+---+---+---+---+---+---+---+---+---+
buf:  | H | e | l | X | . | . | . | . | l | o |
      +---+---+---+---+---+---+---+---+---+---+
                       ^                   ^
                gap_start=4          gap_end=8

Text: "HelXlo"   the 'X' was inserted at the cursor
```

**After `gap_delete()`** (backspace — deletes char before cursor):

```
Index:  0   1   2   3   4   5   6   7   8   9
      +---+---+---+---+---+---+---+---+---+---+
buf:  | H | e | l | X | . | . | . | . | l | o |
      +---+---+---+---+---+---+---+---+---+---+
                   ^                       ^
            gap_start=3              gap_end=8

Text: "Hello"   gap_start moved back, 'X' is now inside the gap
```

Wait — the 'X' is still physically in `buf[3]`! But it does not matter. The gap
covers indices 3-7, so that 'X' is ignored. This is why gap buffers are fast:
delete is just `gap_start--`.

### Gap buffer operation summary

| Operation      | What happens                           | Cost  |
|----------------|----------------------------------------|-------|
| `insert(c)`    | `buf[gap_start++] = c`                 | O(1)  |
| `delete()`     | `gap_start--`                          | O(1)  |
| `move_left()`  | Copy `buf[gap_start-1]` to `buf[gap_end-1]`, adjust pointers | O(1) |
| `move_right()` | Copy `buf[gap_end]` to `buf[gap_start]`, adjust pointers | O(1) |
| `get_text()`   | Copy text before gap + text after gap  | O(n)  |

---

## 2. The Screen Buffer

A screen buffer is a 2D array of characters that represents what should be
drawn on screen. Instead of printing directly to the terminal (which causes
flicker), you:

1. Write everything to the screen buffer
2. Convert the buffer to a single string
3. Send the entire string to the terminal at once

This is called **double buffering** — the same technique used in video games.

```
Screen buffer (4 rows x 10 cols):

     Col: 0  1  2  3  4  5  6  7  8  9
Row 0:  [ H ][ e ][ l ][ l ][ o ][   ][   ][   ][   ][   ]
Row 1:  [ W ][ o ][ r ][ l ][ d ][   ][   ][   ][   ][   ]
Row 2:  [   ][   ][   ][   ][   ][   ][   ][   ][   ][   ]
Row 3:  [ ~ ][   ][   ][   ][   ][   ][   ][   ][   ][   ]

Rendered string:
"Hello     \nWorld     \n          \n~         \n"
```

---

## 3. Terminal Raw Mode (Concept)

Normally, the terminal works in **cooked mode**: it waits for you to press Enter
before sending anything to your program, and it echoes characters back to the
screen automatically.

A text editor needs **raw mode**:
- Every keypress is sent immediately (no waiting for Enter)
- No automatic echoing (the editor decides what to display)
- Special keys (Ctrl-C, Ctrl-Z) are not intercepted by the terminal

In C, you control this with the `termios` API:

```c
#include <termios.h>
#include <unistd.h>

struct termios orig;

void enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig);       /* save original settings */
    struct termios raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON);     /* disable echo + canonical mode */
    raw.c_lflag &= ~(ISIG);              /* disable Ctrl-C / Ctrl-Z signals */
    raw.c_iflag &= ~(IXON | ICRNL);      /* disable Ctrl-S/Q, fix Ctrl-M */
    raw.c_oflag &= ~(OPOST);             /* disable output processing */
    raw.c_cc[VMIN] = 0;                  /* read returns immediately */
    raw.c_cc[VTIME] = 1;                 /* 100ms timeout */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);  /* restore original */
}
```

**Always restore the original settings** when your program exits, or the
terminal will be left in a broken state. Use `atexit(disable_raw_mode)`.

---

## 4. ANSI Escape Codes

ANSI escape codes are special character sequences that control the terminal.
They all start with `\x1b[` (ESC + `[`).

### Cursor Movement

| Code              | Effect                            |
|-------------------|-----------------------------------|
| `\x1b[H`         | Move cursor to top-left (1,1)     |
| `\x1b[{r};{c}H`  | Move cursor to row r, column c    |
| `\x1b[A`         | Move cursor up 1 line             |
| `\x1b[B`         | Move cursor down 1 line           |
| `\x1b[C`         | Move cursor right 1 column        |
| `\x1b[D`         | Move cursor left 1 column         |
| `\x1b[{n}A`      | Move cursor up n lines            |
| `\x1b[{n}B`      | Move cursor down n lines          |

### Screen Clearing

| Code              | Effect                            |
|-------------------|-----------------------------------|
| `\x1b[2J`        | Clear entire screen               |
| `\x1b[J`         | Clear from cursor to end of screen|
| `\x1b[1J`        | Clear from start of screen to cursor |
| `\x1b[K`         | Clear from cursor to end of line  |
| `\x1b[2K`        | Clear entire line                 |

### Text Formatting & Color

| Code              | Effect                            |
|-------------------|-----------------------------------|
| `\x1b[0m`        | Reset all formatting              |
| `\x1b[1m`        | Bold                              |
| `\x1b[4m`        | Underline                         |
| `\x1b[7m`        | Inverse (swap fg/bg)              |
| `\x1b[30m`-`\x1b[37m` | Set foreground color (black-white) |
| `\x1b[40m`-`\x1b[47m` | Set background color              |
| `\x1b[38;5;{n}m` | 256-color foreground              |
| `\x1b[48;5;{n}m` | 256-color background              |

### Color Table (Standard 8)

| Code | Color   |  Code | Color        |
|------|---------|-------|--------------|
| 30   | Black   |  40   | Black bg     |
| 31   | Red     |  41   | Red bg       |
| 32   | Green   |  42   | Green bg     |
| 33   | Yellow  |  43   | Yellow bg    |
| 34   | Blue    |  44   | Blue bg      |
| 35   | Magenta |  45   | Magenta bg   |
| 36   | Cyan    |  46   | Cyan bg      |
| 37   | White   |  47   | White bg     |

### Example: drawing a status bar

```c
/* Move to row 24, col 1; set inverse video; print status; reset */
printf("\x1b[24;1H\x1b[7m editor.c | Ln 15, Col 8 \x1b[0m");
```

---

## 5. The Event Loop

Every interactive program follows the same pattern:

```
┌──────────────────────────────────────┐
│           Event Loop                 │
│                                      │
│   ┌──────────┐                       │
│   │ Read     │ ◄── keyboard input    │
│   │ input    │                       │
│   └────┬─────┘                       │
│        │                             │
│        ▼                             │
│   ┌──────────┐                       │
│   │ Process  │ ◄── update gap buffer │
│   │ input    │     move cursor, etc. │
│   └────┬─────┘                       │
│        │                             │
│        ▼                             │
│   ┌──────────┐                       │
│   │ Render   │ ◄── write screen buf  │
│   │ screen   │     to terminal       │
│   └────┬─────┘                       │
│        │                             │
│        └────────── loop back ───┐    │
│                                 │    │
└─────────────────────────────────┘    │
                                       │
     (until user presses Ctrl-Q) ──────┘
```

In pseudocode:

```c
while (1) {
    int key = read_key();          /* step 1: read input */
    if (key == CTRL_Q) break;

    process_key(key);              /* step 2: update state */

    render_screen();               /* step 3: draw */
}
```

---

## Exercises

### Exercise 1: `gap_buffer.c`

Implement a gap buffer data structure — the core of your text editor. This is
where you learn how text editors handle insertions and deletions efficiently.

Functions to implement:
- `gap_create(capacity)` — allocate a gap buffer
- `gap_destroy(gb)` — free the gap buffer
- `gap_insert(gb, c)` — insert a character at the cursor
- `gap_delete(gb)` — delete the character before the cursor (backspace)
- `gap_move_left(gb)` — move cursor one position left
- `gap_move_right(gb)` — move cursor one position right
- `gap_get_text(gb, buf, size)` — extract the text (skipping the gap)
- `gap_length(gb)` — return the number of characters (excluding the gap)

### Exercise 2: `screen_buffer.c`

Implement a simple screen buffer for rendering text to the terminal. This gives
you a 2D canvas to write characters onto before flushing to the screen.

Functions to implement:
- `screen_create(width, height)` — allocate a screen buffer
- `screen_destroy(sb)` — free the screen buffer
- `screen_set(sb, row, col, c)` — set a character at a position
- `screen_clear(sb)` — fill the entire buffer with spaces
- `screen_render_to_string(sb, buf, size)` — render to a string (rows joined by `\n`)

---

## What comes next

In the next module, we will wire these pieces together into a working terminal
text editor: raw mode, key reading, the event loop, file loading and saving.
But first — master the gap buffer. It is the foundation everything else builds on.
