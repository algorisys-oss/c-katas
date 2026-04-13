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
    /*
     * Each flag is a single bit in an integer. The expression &= ~(FLAG)
     * means "turn off this bit" — it ANDs with the bitwise inverse of the
     * flag, clearing that bit while leaving all others unchanged.
     *
     * ICANON — canonical mode: the terminal collects a whole line before
     *          sending it to your program (waits for Enter). Turning it
     *          off gives us each keypress immediately.
     * ECHO   — the terminal automatically echoes what you type back to
     *          the screen. We turn it off because the editor will draw
     *          the screen itself.
     */
    raw.c_lflag &= ~(ECHO | ICANON);
    /*
     * ISIG — when on, Ctrl+C sends SIGINT and Ctrl+Z sends SIGTSTP.
     *        We turn it off so we can handle those key combos ourselves.
     */
    raw.c_lflag &= ~(ISIG);
    /*
     * IXON  — software flow control. When on, Ctrl+S pauses output and
     *         Ctrl+Q resumes it (a leftover from the 1970s). We turn it
     *         off so we can use Ctrl+S for "save" and Ctrl+Q for "quit".
     * ICRNL — translates carriage return (CR, '\r', byte 13) into newline
     *         (NL, '\n', byte 10). We turn it off to get raw input — we
     *         want to know exactly which byte the terminal sent.
     */
    raw.c_iflag &= ~(IXON | ICRNL);
    /*
     * OPOST — output processing. When on, the terminal translates '\n'
     *         into '\r\n' (carriage return + newline) on output. We turn
     *         it off for exact control over what bytes reach the screen.
     *         This means we must write '\r\n' ourselves when we want a
     *         new line.
     */
    raw.c_oflag &= ~(OPOST);
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

> **How ANSI escape sequences work:** All sequences begin with the **ESC** byte
> (0x1B in hex, 27 in decimal, written as `\x1b` or `\033` in C). The `[`
> character after ESC is called the **CSI** (Control Sequence Introducer).
> After the CSI come optional numeric parameters separated by `;`, then a
> single letter that tells the terminal what to do.
>
> ```
>   \x1b  [  2  ;  5  H
>    │    │  │     │  │
>    │    │  │     │  └── 'H' = command: move cursor to position
>    │    │  │     └───── column 5
>    │    │  └─────────── row 2
>    │    └────────────── CSI (Control Sequence Introducer)
>    └─────────────────── ESC byte (0x1B)
> ```
>
> So `\x1b[2;5H` means "move the cursor to row 2, column 5." The letter at
> the end determines the action: `H` = cursor position, `J` = erase display,
> `m` = set color/style, and so on.

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

## 6. Terminal Control — Taking Over the Terminal

Now that you understand the concepts, it's time to code them. In this exercise
you will implement the functions that put the terminal into raw mode, query its
size, and generate ANSI escape sequences.

### The termios struct — a bit field of flags

The `termios` struct holds four groups of flags that control terminal behavior.
Each flag is a single bit in an integer. You turn a flag off with `&= ~(FLAG)`:

```
struct termios
├── c_iflag   (input flags)
│     IXON ──── software flow control (Ctrl+S / Ctrl+Q)
│     ICRNL ─── translate CR → NL
│
├── c_oflag   (output flags)
│     OPOST ─── translate NL → CR+NL on output
│
├── c_lflag   (local flags)
│     ECHO ──── echo typed chars to screen
│     ICANON ── buffer until Enter (canonical mode)
│     ISIG ──── Ctrl+C → SIGINT, Ctrl+Z → SIGTSTP
│
└── c_cc[]    (control characters)
      VMIN ──── min bytes for read to return
      VTIME ─── timeout in 1/10ths of a second
```

**To enter raw mode**, you turn OFF all the flags listed above and set
`VMIN=0, VTIME=1`. This makes `read()` return immediately with 0 bytes if
nothing was pressed, or after 100ms with whatever bytes arrived.

### Querying terminal size with ioctl

The `ioctl` system call with `TIOCGWINSZ` fills a `struct winsize`:

```c
struct winsize ws;
ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
// ws.ws_row = number of rows
// ws.ws_col = number of columns
```

This is how your editor knows how many lines of text it can display.

---

## 7. Keyboard Input — Decoding Raw Bytes

When the terminal is in raw mode, each keypress arrives as one or more bytes
on `stdin`. Printable characters are a single byte (their ASCII code). But
special keys — arrows, Home, End, Delete, Page Up/Down — are sent as
**multi-byte escape sequences**.

### Escape sequence structure

All escape sequences start with the ESC byte (`0x1b`, decimal 27). Here is how
the terminal encodes special keys:

```
When you press Up Arrow, the terminal sends 3 bytes:

  Byte 1    Byte 2    Byte 3
  ┌──────┐  ┌──────┐  ┌──────┐
  │ 0x1b │  │ 0x5b │  │ 0x41 │
  │ (ESC)│  │  '['  │  │  'A'  │
  └──────┘  └──────┘  └──────┘

   ESC        CSI       'A' = Up

Down  = ESC [ B
Right = ESC [ C
Left  = ESC [ D
Home  = ESC [ H
End   = ESC [ F
```

Some keys use a 4-byte sequence with a tilde `~`:

```
Delete  = ESC [ 3 ~
Page Up = ESC [ 5 ~
Page Dn = ESC [ 6 ~
Home    = ESC [ 1 ~    (alternate encoding)
End     = ESC [ 4 ~    (alternate encoding)
```

### The parsing state machine

```
Read first byte:
  ├─ printable (32-126) → return as-is
  ├─ 0x1b (ESC) → read next byte:
  │   ├─ '[' (CSI) → read next byte:
  │   │   ├─ 'A' → ARROW_UP
  │   │   ├─ 'B' → ARROW_DOWN
  │   │   ├─ 'C' → ARROW_RIGHT
  │   │   ├─ 'D' → ARROW_LEFT
  │   │   ├─ 'H' → HOME
  │   │   ├─ 'F' → END
  │   │   └─ digit → read one more byte (must be '~'):
  │   │       ├─ "1~" → HOME
  │   │       ├─ "3~" → DELETE
  │   │       ├─ "4~" → END
  │   │       ├─ "5~" → PAGE_UP
  │   │       └─ "6~" → PAGE_DOWN
  │   └─ other → return ESC
  ├─ 127 → BACKSPACE
  └─ 1-26 → Ctrl+key combo
```

### Why separate parsing from reading?

We split the logic into two functions:

- `read_key_from_buf(buf, len)` — pure parsing, fully testable
- `read_key()` — calls `read(STDIN_FILENO, ...)` then delegates to the parser

This is a pattern you will see everywhere in systems programming: separate the
I/O from the logic so you can test the logic without actual file descriptors.

---

## 8. Editor State — Multi-Line Text Management

A real text editor manages multiple lines, each with its own gap buffer.
The editor state tracks which line the cursor is on, where on that line it is,
and which portion of the file is visible on screen.

### The array-of-gap-buffers model

```
EditorState
├── lines[0]: GapBuffer "Hello, wo|rld"     (cursor here, row=0, col=9)
├── lines[1]: GapBuffer "This is line two"
├── lines[2]: GapBuffer ""
└── lines[3]: GapBuffer "Last line"
    num_lines = 4
    cursor_row = 0, cursor_col = 9
```

Each line is an independent gap buffer. `cursor_col` is the same as that
line's `gap_start`. When you move up or down, the cursor switches to a
different gap buffer.

### Scrolling — the viewport window

Your terminal can only show a fixed number of rows. If the file has 500 lines
but your terminal has 24 rows, you need to scroll:

```
File (500 lines):          Screen (24 rows):
┌────────────────────┐
│ line 0             │
│ line 1             │
│ ...                │
│ line 45  ──────────┼──►  ┌──────────────────┐
│ line 46            │     │ line 45           │ ← row_offset = 45
│ line 47            │     │ line 46           │
│ ...                │     │ ...               │
│ line 68  █cursor   │     │ line 68  █cursor  │
│ ...                │     │ ...               │
│ line 69            │     │ line 69           │ ← last visible
│ ...                │     └──────────────────┘
│ line 499           │
└────────────────────┘

If cursor_row < row_offset → scroll up:   row_offset = cursor_row
If cursor_row >= row_offset + screen_rows → scroll down
```

### Line splitting (Enter key)

When you press Enter in the middle of a line, it splits into two:

```
Before (cursor at col 5):
  line[2]: "Hello, world"
                ^cursor

After Enter:
  line[2]: "Hello"        ← text before cursor stays
  line[3]: ", world"      ← text after cursor moves to new line
  (all lines below shift down by 1)
  cursor moves to row 3, col 0
```

### Line merging (Backspace at start of line)

When you press Backspace at column 0, the current line merges with the one above:

```
Before (cursor at row 3, col 0):
  line[2]: "Hello"
  line[3]: ", world"
           ^cursor

After Backspace:
  line[2]: "Hello, world"
                 ^cursor (col = 5, the old length of line[2])
  (line[3] is removed, all lines below shift up by 1)
```

---

## 9. Rendering — Drawing the Screen

The rendering system converts the editor state into a visual frame. We build the
entire frame in a buffer, then write it to the terminal in a single `write()` call
to avoid flicker.

### Frame anatomy

```
┌─ gutter ─┬─── text area ──────────────────────┐
│   1 │Hello, world                              │ ← file line 0
│   2 │int main(void)                            │ ← file line 1
│   3 │{                                         │ ← file line 2
│   ~ │                                          │ ← no line here
│   ~ │                                          │ ← no line here
│ editor.c [modified]     Ln 1, Col 12 | 3 lines │ ← status bar (inverse)
│ Ctrl+S = save | Ctrl+Q = quit                  │ ← message bar
└────────────────────────────────────────────────┘
```

**Gutter**: Line numbers, right-aligned, dimmed. Empty rows show `~` (like vim).

**Status bar**: Uses ANSI inverse video (`\x1b[7m`) for a highlighted bar.
Shows filename, modified indicator, cursor position, and line count.

**Message bar**: Shows help text by default, or status messages (e.g., "Saved 42 bytes").

### The render pipeline

```
EditorState → build frame string → write() to terminal
     │                                       │
     │  For each visible row:                │
     │    1. Line number (dimmed)            │
     │    2. Text (from gap buffer)          │
     │    3. Clear rest of line (\x1b[K)     │
     │                                       │
     │  Status bar (inverse video)           │
     │  Message bar                          │
     │  Position cursor                      │
     │  Show cursor (\x1b[?25h)             │
     └───────────────────────────────────────┘
```

---

## 10. The Event Loop — Wiring It All Together

Now you bring every piece together. The main function follows a simple pattern:

```
main():
  1. enable_raw_mode()        ← take control of terminal
  2. editor_init()            ← create editor state
  3. get_terminal_size()      ← how big is the screen?
  4. editor_load_file()       ← load file from argv[1]
  5. LOOP:
     a. editor_render()       ← draw the screen
     b. read_key()            ← wait for keypress
     c. process_key()         ← dispatch to editor action
     d. if quit → break
  6. clean up                 ← clear screen, free memory
```

### The process_key dispatch table

`process_key()` is the brain of the editor — a switch statement that maps each
key to the right action:

```c
switch (key) {
    Ctrl+Q  → quit (with dirty-file safety check)
    Ctrl+S  → save file
    Arrows  → move cursor
    Home    → cursor to start of line
    End     → cursor to end of line
    PgUp    → scroll up one screen
    PgDn    → scroll down one screen
    Bksp    → delete char before cursor
    Delete  → delete char after cursor
    Enter   → split line at cursor
    Escape  → ignore
    32-126  → insert printable character
}
```

### Quit confirmation

When the user presses Ctrl+Q with unsaved changes, don't quit immediately.
Show a warning ("Unsaved changes! Press Ctrl+Q again to quit.") and set a
`quit_confirm` flag. Only actually quit on the second Ctrl+Q. Any other key
resets the flag.

---

## Exercises

### Exercise 1: `gap_buffer.c` (16 tests)

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

### Exercise 2: `screen_buffer.c` (10 tests)

Implement a simple screen buffer for rendering text to the terminal. This gives
you a 2D canvas to write characters onto before flushing to the screen.

Functions to implement:
- `screen_create(width, height)` — allocate a screen buffer
- `screen_destroy(sb)` — free the screen buffer
- `screen_set(sb, row, col, c)` — set a character at a position
- `screen_clear(sb)` — fill the entire buffer with spaces
- `screen_render_to_string(sb, buf, size)` — render to a string (rows joined by `\n`)

### Exercise 3: `terminal.c` (6 tests)

Implement terminal control functions: raw mode, terminal size querying, and
ANSI escape sequence generation.

Functions to implement:
- `terminal_init(t)` — save current terminal settings
- `terminal_enable_raw(t)` — switch to raw mode (disable echo, canonical, signals, etc.)
- `terminal_disable_raw(t)` — restore original settings
- `terminal_get_size(rows, cols)` — query terminal dimensions with ioctl
- `terminal_clear_screen(out, size)` — generate clear-screen escape sequence
- `terminal_set_cursor(out, size, row, col)` — generate cursor-positioning escape sequence

### Exercise 4: `input.c` (10 tests)

Implement keyboard input decoding. Parse raw byte sequences into key codes.

Functions to implement:
- `read_key_from_buf(buf, len)` — parse a byte buffer into a key code (testable core)
- `read_key()` — read from stdin and delegate to the parser (live version)

### Exercise 5: `editor.c` (interactive — no automated tests)

Wire everything into a complete working text editor. All building-block
functions are pre-filled. Your job is to implement:

- `process_key(e, key)` — the dispatch switch that maps keys to editor actions
- `main(argc, argv)` — the event loop (init → render → read → process → loop)

When complete, compile and run:

```
make editor
./editor              # new file
./editor myfile.txt   # open existing file
```

### Verification checklist for Exercise 5

Since the editor is interactive, you can't test it with assertions. Instead,
verify these behaviors manually:

- [ ] Editor launches and shows a cursor
- [ ] Typing inserts characters
- [ ] Arrow keys move the cursor
- [ ] Backspace deletes the character before the cursor
- [ ] Enter splits the line
- [ ] Ctrl+S saves the file
- [ ] Ctrl+Q quits (with confirmation if file is dirty)
- [ ] Opening an existing file displays its contents
- [ ] Line numbers appear in the gutter
- [ ] Status bar shows filename, cursor position, and line count
- [ ] Scrolling works when the file is longer than the screen

---

## Building & Testing

```bash
# Run all automated tests (exercises 1-4):
make test

# Build and run the editor solution:
make run

# Build your own editor exercise:
make editor
./editor myfile.txt

# Clean all binaries:
make clean
```

---

## Reading Real Code: kilo.c

Kilo is a complete text editor in ~1000 lines of C, written by Salvatore
Sanfilippo (creator of Redis). It uses the exact same techniques you've
learned: raw terminal mode, escape codes, and a simple buffer.

What to look for:
- **editorOpen()**: How it reads a file line by line
- **editorRefreshScreen()**: The render loop — clear, draw, position cursor
- **editorProcessKeypress()**: The event loop — read key, dispatch action
- **Raw mode setup**: Compare with your termios code

It's one of the best "learn by reading" C codebases. Every line is
understandable with what you've learned in this module.

Source: https://github.com/antirez/kilo

---

## What You Built

Congratulations — you have a working text editor written from scratch in C.

Here's the architecture of what you built:

```
┌─────────────────────────────────────────────────────────┐
│                      editor.c                           │
│                                                         │
│  main()                                                 │
│    │                                                    │
│    ├── enable_raw_mode()          ◄── Terminal Control   │
│    ├── editor_init()              ◄── Editor State       │
│    ├── get_terminal_size()        ◄── Terminal Control   │
│    ├── editor_load_file()         ◄── File I/O           │
│    │                                                    │
│    └── EVENT LOOP:                                      │
│        ├── editor_render()        ◄── Rendering          │
│        │     ├── editor_scroll()                        │
│        │     ├── gap_get_text()   ◄── Gap Buffer         │
│        │     └── write()          ◄── Terminal Output    │
│        │                                                │
│        ├── read_key()             ◄── Input Parsing      │
│        │                                                │
│        └── process_key()          ◄── Key Dispatch       │
│              ├── editor_insert_char()                    │
│              ├── editor_delete_char()                    │
│              ├── editor_insert_newline()                 │
│              ├── editor_move_cursor()                    │
│              └── editor_save_file()   ◄── File I/O       │
└─────────────────────────────────────────────────────────┘
```

### Ideas for extensions

If you want to keep going, try adding:
- **Search** (Ctrl+F): scan through lines for a substring, jump to matches
- **Syntax highlighting**: color keywords, strings, comments based on file type
- **Multiple buffers**: open and switch between multiple files
- **Undo/redo**: keep a stack of operations and reverse them
- **Copy/paste**: select text with Shift+arrows, yank, and paste

---

[← Previous: Module 21: Processes & Concurrency](../14-process-concurrency/README.md) | [Next: Module 23: Git Internals →](../28-git-internals/README.md)
