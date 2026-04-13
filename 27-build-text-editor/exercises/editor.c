/*
 * Exercise: A Complete Terminal Text Editor
 *
 * This is the capstone exercise. You will wire together every piece you've
 * built — gap buffer, screen buffer, terminal control, input parsing — into
 * a real, working text editor.
 *
 * All the building-block functions are already implemented below (you wrote
 * them in the previous exercises). Your job is to fill in:
 *
 *   1. process_key()  — dispatch keypresses to the right editor action
 *   2. main()         — the event loop that ties everything together
 *
 * When complete, compile and run:
 *   gcc -Wall -Wextra -pedantic -std=c99 -o editor editor.c
 *   ./editor                   (new file)
 *   ./editor myfile.txt        (open existing file)
 *
 * Controls:
 *   Ctrl+S  — Save
 *   Ctrl+Q  — Quit (press twice if unsaved changes)
 *   Arrows  — Move cursor
 *   Home    — Jump to start of line
 *   End     — Jump to end of line
 *   PgUp    — Scroll up one screen
 *   PgDn    — Scroll down one screen
 */

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

/* ════════════════════════════════════════════════════════════════════
 * Part 1: Gap Buffer (already implemented — from Exercise 1)
 * ════════════════════════════════════════════════════════════════════ */

typedef struct {
    char *buf;
    int buf_size;
    int gap_start;
    int gap_end;
} GapBuffer;

GapBuffer gap_create(int capacity)
{
    GapBuffer gb;
    gb.buf = malloc(capacity);
    if (!gb.buf) {
        fprintf(stderr, "gap_create: malloc failed\n");
        exit(1);
    }
    gb.buf_size = capacity;
    gb.gap_start = 0;
    gb.gap_end = capacity;
    return gb;
}

void gap_destroy(GapBuffer *gb)
{
    free(gb->buf);
    gb->buf = NULL;
}

static void gap_grow(GapBuffer *gb)
{
    int new_size = gb->buf_size * 2;
    if (new_size < 64) new_size = 64;
    char *new_buf = malloc(new_size);
    if (!new_buf) {
        fprintf(stderr, "gap_grow: malloc failed\n");
        exit(1);
    }
    memcpy(new_buf, gb->buf, gb->gap_start);
    int after_len = gb->buf_size - gb->gap_end;
    memcpy(new_buf + new_size - after_len, gb->buf + gb->gap_end, after_len);
    free(gb->buf);
    gb->buf = new_buf;
    gb->gap_end = new_size - after_len;
    gb->buf_size = new_size;
}

void gap_insert(GapBuffer *gb, char c)
{
    if (gb->gap_start == gb->gap_end) gap_grow(gb);
    gb->buf[gb->gap_start] = c;
    gb->gap_start++;
}

void gap_delete(GapBuffer *gb)
{
    if (gb->gap_start == 0) return;
    gb->gap_start--;
}

void gap_delete_forward(GapBuffer *gb)
{
    if (gb->gap_end == gb->buf_size) return;
    gb->gap_end++;
}

void gap_move_left(GapBuffer *gb)
{
    if (gb->gap_start == 0) return;
    gb->gap_end--;
    gb->buf[gb->gap_end] = gb->buf[gb->gap_start - 1];
    gb->gap_start--;
}

void gap_move_right(GapBuffer *gb)
{
    if (gb->gap_end == gb->buf_size) return;
    gb->buf[gb->gap_start] = gb->buf[gb->gap_end];
    gb->gap_start++;
    gb->gap_end++;
}

void gap_move_to(GapBuffer *gb, int pos)
{
    while (gb->gap_start > pos) gap_move_left(gb);
    while (gb->gap_start < pos) gap_move_right(gb);
}

void gap_get_text(const GapBuffer *gb, char *out, int out_size)
{
    if (out_size <= 0) return;
    int before_len = gb->gap_start;
    int after_len = gb->buf_size - gb->gap_end;
    int max_chars = out_size - 1;
    int written = 0;
    int copy_before = before_len < max_chars ? before_len : max_chars;
    memcpy(out, gb->buf, copy_before);
    written += copy_before;
    int remaining = max_chars - written;
    int copy_after = after_len < remaining ? after_len : remaining;
    if (copy_after > 0) {
        memcpy(out + written, gb->buf + gb->gap_end, copy_after);
        written += copy_after;
    }
    out[written] = '\0';
}

int gap_length(const GapBuffer *gb)
{
    return gb->buf_size - (gb->gap_end - gb->gap_start);
}

/* ════════════════════════════════════════════════════════════════════
 * Part 2: Key Codes & Input (already implemented — from Exercise 4)
 * ════════════════════════════════════════════════════════════════════ */

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

#define CTRL_KEY(k) ((k) & 0x1f)

int read_key(void)
{
    char c;
    int nread;
    while ((nread = read(STDIN_FILENO, &c, 1)) == 0) {
        /* timeout — keep trying */
    }
    if (nread == -1 && errno != EAGAIN) return KEY_NONE;

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                    case '1': return KEY_HOME;
                    case '3': return KEY_DELETE;
                    case '4': return KEY_END;
                    case '5': return KEY_PAGE_UP;
                    case '6': return KEY_PAGE_DOWN;
                    }
                }
                return '\x1b';
            }
            switch (seq[1]) {
            case 'A': return KEY_ARROW_UP;
            case 'B': return KEY_ARROW_DOWN;
            case 'C': return KEY_ARROW_RIGHT;
            case 'D': return KEY_ARROW_LEFT;
            case 'H': return KEY_HOME;
            case 'F': return KEY_END;
            }
        }
        return '\x1b';
    }
    return (unsigned char)c;
}

/* ════════════════════════════════════════════════════════════════════
 * Part 3: Terminal Control (already implemented — from Exercise 3)
 * ════════════════════════════════════════════════════════════════════ */

static struct termios orig_termios;

void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int get_terminal_size(int *rows, int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
}

/* ════════════════════════════════════════════════════════════════════
 * Part 4: Editor State (already implemented)
 * ════════════════════════════════════════════════════════════════════ */

#define MAX_LINES 4096
#define LINE_INIT_CAP 128

typedef struct {
    GapBuffer *lines;       /* array of gap buffers, one per line */
    int num_lines;          /* number of lines */
    int lines_cap;          /* allocated capacity for lines array */
    int cursor_row;         /* cursor row (0-indexed) */
    int cursor_col;         /* cursor column (= gap_start of current line) */
    int row_offset;         /* first visible row for scrolling */
    int col_offset;         /* first visible col for horizontal scrolling */
    int screen_rows;        /* terminal rows available for text */
    int screen_cols;        /* terminal columns */
    char filename[256];     /* current file path */
    int dirty;              /* 1 if modified since last save */
    int quit_confirm;       /* 1 if user pressed Ctrl+Q once with dirty file */
    char status_msg[128];   /* message shown in status bar */
} EditorState;

void editor_init(EditorState *e)
{
    e->lines = malloc(sizeof(GapBuffer) * 256);
    if (!e->lines) {
        fprintf(stderr, "editor_init: malloc failed\n");
        exit(1);
    }
    e->lines_cap = 256;
    e->lines[0] = gap_create(LINE_INIT_CAP);
    e->num_lines = 1;
    e->cursor_row = 0;
    e->cursor_col = 0;
    e->row_offset = 0;
    e->col_offset = 0;
    e->screen_rows = 24;
    e->screen_cols = 80;
    e->filename[0] = '\0';
    e->dirty = 0;
    e->quit_confirm = 0;
    e->status_msg[0] = '\0';
}

void editor_destroy(EditorState *e)
{
    for (int i = 0; i < e->num_lines; i++) {
        gap_destroy(&e->lines[i]);
    }
    free(e->lines);
    e->lines = NULL;
}

static void editor_ensure_lines_cap(EditorState *e, int needed)
{
    if (needed <= e->lines_cap) return;
    int new_cap = e->lines_cap * 2;
    if (new_cap < needed) new_cap = needed;
    GapBuffer *new_lines = realloc(e->lines, sizeof(GapBuffer) * new_cap);
    if (!new_lines) {
        fprintf(stderr, "editor: realloc failed\n");
        exit(1);
    }
    e->lines = new_lines;
    e->lines_cap = new_cap;
}

int editor_line_length(const EditorState *e, int row)
{
    if (row < 0 || row >= e->num_lines) return 0;
    return gap_length(&e->lines[row]);
}

void editor_insert_char(EditorState *e, char c)
{
    GapBuffer *line = &e->lines[e->cursor_row];
    gap_move_to(line, e->cursor_col);
    gap_insert(line, c);
    e->cursor_col++;
    e->dirty = 1;
}

void editor_insert_newline(EditorState *e)
{
    GapBuffer *cur = &e->lines[e->cursor_row];
    gap_move_to(cur, e->cursor_col);

    int after_len = gap_length(cur) - e->cursor_col;
    char *after_text = NULL;
    if (after_len > 0) {
        after_text = malloc(after_len + 1);
        if (!after_text) exit(1);
        char *full = malloc(gap_length(cur) + 1);
        if (!full) exit(1);
        gap_get_text(cur, full, gap_length(cur) + 1);
        memcpy(after_text, full + e->cursor_col, after_len);
        after_text[after_len] = '\0';
        free(full);

        gap_move_to(cur, gap_length(cur));
        for (int i = 0; i < after_len; i++) {
            gap_delete(cur);
        }
    }

    editor_ensure_lines_cap(e, e->num_lines + 1);
    memmove(&e->lines[e->cursor_row + 2],
            &e->lines[e->cursor_row + 1],
            sizeof(GapBuffer) * (e->num_lines - e->cursor_row - 1));
    e->num_lines++;

    e->lines[e->cursor_row + 1] = gap_create(LINE_INIT_CAP);
    if (after_text) {
        for (int i = 0; i < after_len; i++) {
            gap_insert(&e->lines[e->cursor_row + 1], after_text[i]);
        }
        free(after_text);
    }

    e->cursor_row++;
    e->cursor_col = 0;
    e->dirty = 1;
}

void editor_delete_char(EditorState *e)
{
    if (e->cursor_col > 0) {
        GapBuffer *line = &e->lines[e->cursor_row];
        gap_move_to(line, e->cursor_col);
        gap_delete(line);
        e->cursor_col--;
        e->dirty = 1;
    } else if (e->cursor_row > 0) {
        int prev_len = gap_length(&e->lines[e->cursor_row - 1]);
        int cur_len = gap_length(&e->lines[e->cursor_row]);
        char *cur_text = malloc(cur_len + 1);
        if (!cur_text) exit(1);
        gap_get_text(&e->lines[e->cursor_row], cur_text, cur_len + 1);

        GapBuffer *prev = &e->lines[e->cursor_row - 1];
        gap_move_to(prev, gap_length(prev));
        for (int i = 0; i < cur_len; i++) {
            gap_insert(prev, cur_text[i]);
        }
        free(cur_text);

        gap_destroy(&e->lines[e->cursor_row]);
        memmove(&e->lines[e->cursor_row],
                &e->lines[e->cursor_row + 1],
                sizeof(GapBuffer) * (e->num_lines - e->cursor_row - 1));
        e->num_lines--;

        e->cursor_row--;
        e->cursor_col = prev_len;
        e->dirty = 1;
    }
}

void editor_delete_forward(EditorState *e)
{
    int line_len = editor_line_length(e, e->cursor_row);
    if (e->cursor_col < line_len) {
        GapBuffer *line = &e->lines[e->cursor_row];
        gap_move_to(line, e->cursor_col);
        gap_delete_forward(line);
        e->dirty = 1;
    } else if (e->cursor_row < e->num_lines - 1) {
        int next_len = gap_length(&e->lines[e->cursor_row + 1]);
        char *next_text = malloc(next_len + 1);
        if (!next_text) exit(1);
        gap_get_text(&e->lines[e->cursor_row + 1], next_text, next_len + 1);

        GapBuffer *cur = &e->lines[e->cursor_row];
        gap_move_to(cur, gap_length(cur));
        for (int i = 0; i < next_len; i++) {
            gap_insert(cur, next_text[i]);
        }
        free(next_text);

        gap_destroy(&e->lines[e->cursor_row + 1]);
        memmove(&e->lines[e->cursor_row + 1],
                &e->lines[e->cursor_row + 2],
                sizeof(GapBuffer) * (e->num_lines - e->cursor_row - 2));
        e->num_lines--;
        e->dirty = 1;
    }
}

void editor_move_cursor(EditorState *e, int key)
{
    int line_len;

    switch (key) {
    case KEY_ARROW_LEFT:
        if (e->cursor_col > 0) {
            e->cursor_col--;
        } else if (e->cursor_row > 0) {
            e->cursor_row--;
            e->cursor_col = editor_line_length(e, e->cursor_row);
        }
        break;
    case KEY_ARROW_RIGHT:
        line_len = editor_line_length(e, e->cursor_row);
        if (e->cursor_col < line_len) {
            e->cursor_col++;
        } else if (e->cursor_row < e->num_lines - 1) {
            e->cursor_row++;
            e->cursor_col = 0;
        }
        break;
    case KEY_ARROW_UP:
        if (e->cursor_row > 0) {
            e->cursor_row--;
            line_len = editor_line_length(e, e->cursor_row);
            if (e->cursor_col > line_len) e->cursor_col = line_len;
        }
        break;
    case KEY_ARROW_DOWN:
        if (e->cursor_row < e->num_lines - 1) {
            e->cursor_row++;
            line_len = editor_line_length(e, e->cursor_row);
            if (e->cursor_col > line_len) e->cursor_col = line_len;
        }
        break;
    case KEY_HOME:
        e->cursor_col = 0;
        break;
    case KEY_END:
        e->cursor_col = editor_line_length(e, e->cursor_row);
        break;
    case KEY_PAGE_UP:
        for (int i = 0; i < e->screen_rows; i++)
            editor_move_cursor(e, KEY_ARROW_UP);
        break;
    case KEY_PAGE_DOWN:
        for (int i = 0; i < e->screen_rows; i++)
            editor_move_cursor(e, KEY_ARROW_DOWN);
        break;
    }
}

void editor_scroll(EditorState *e)
{
    if (e->cursor_row < e->row_offset)
        e->row_offset = e->cursor_row;
    if (e->cursor_row >= e->row_offset + e->screen_rows)
        e->row_offset = e->cursor_row - e->screen_rows + 1;
    if (e->cursor_col < e->col_offset)
        e->col_offset = e->cursor_col;
    if (e->cursor_col >= e->col_offset + e->screen_cols)
        e->col_offset = e->cursor_col - e->screen_cols + 1;
}

/* ════════════════════════════════════════════════════════════════════
 * Part 5: File I/O (already implemented)
 * ════════════════════════════════════════════════════════════════════ */

void editor_load_file(EditorState *e, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return;

    snprintf(e->filename, sizeof(e->filename), "%s", path);

    for (int i = 0; i < e->num_lines; i++) {
        gap_destroy(&e->lines[i]);
    }
    e->num_lines = 0;

    char line_buf[4096];
    while (fgets(line_buf, sizeof(line_buf), fp)) {
        int len = strlen(line_buf);
        while (len > 0 && (line_buf[len - 1] == '\n' || line_buf[len - 1] == '\r')) {
            line_buf[--len] = '\0';
        }
        editor_ensure_lines_cap(e, e->num_lines + 1);
        e->lines[e->num_lines] = gap_create(len + LINE_INIT_CAP);
        for (int i = 0; i < len; i++) {
            gap_insert(&e->lines[e->num_lines], line_buf[i]);
        }
        e->num_lines++;
    }
    fclose(fp);

    if (e->num_lines == 0) {
        e->lines[0] = gap_create(LINE_INIT_CAP);
        e->num_lines = 1;
    }

    e->dirty = 0;
    e->cursor_row = 0;
    e->cursor_col = 0;
}

void editor_save_file(EditorState *e)
{
    if (e->filename[0] == '\0') {
        snprintf(e->status_msg, sizeof(e->status_msg),
                 "No filename — use: ./editor <filename>");
        return;
    }

    FILE *fp = fopen(e->filename, "w");
    if (!fp) {
        snprintf(e->status_msg, sizeof(e->status_msg),
                 "Error: could not save %s", e->filename);
        return;
    }

    int total_bytes = 0;
    char line_buf[4096];
    for (int i = 0; i < e->num_lines; i++) {
        gap_get_text(&e->lines[i], line_buf, sizeof(line_buf));
        int len = strlen(line_buf);
        fwrite(line_buf, 1, len, fp);
        fwrite("\n", 1, 1, fp);
        total_bytes += len + 1;
    }
    fclose(fp);

    e->dirty = 0;
    snprintf(e->status_msg, sizeof(e->status_msg),
             "Saved %d bytes to %s", total_bytes, e->filename);
}

/* ════════════════════════════════════════════════════════════════════
 * Part 6: Rendering (already implemented)
 * ════════════════════════════════════════════════════════════════════ */

#define RENDER_BUF_SIZE (64 * 1024)

void editor_render(EditorState *e)
{
    editor_scroll(e);

    static char buf[RENDER_BUF_SIZE];
    int pos = 0;
    char line_text[4096];

    /* Hide cursor and move to top-left */
    pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "\x1b[?25l\x1b[H");

    int gutter_width = 4;
    {
        int n = e->num_lines;
        int digits = 1;
        while (n >= 10) { n /= 10; digits++; }
        gutter_width = digits + 2;
        if (gutter_width < 4) gutter_width = 4;
    }

    int text_width = e->screen_cols - gutter_width - 1;
    if (text_width < 1) text_width = 1;

    for (int y = 0; y < e->screen_rows; y++) {
        int file_row = y + e->row_offset;

        if (file_row < e->num_lines) {
            pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos,
                            "\x1b[2m%*d \x1b[0m",
                            gutter_width - 1, file_row + 1);

            gap_get_text(&e->lines[file_row], line_text, sizeof(line_text));
            int len = strlen(line_text);
            int start = e->col_offset;
            if (start < len) {
                int show = len - start;
                if (show > text_width) show = text_width;
                for (int i = 0; i < show; i++) {
                    if (pos < RENDER_BUF_SIZE - 1)
                        buf[pos++] = line_text[start + i];
                }
            }
        } else {
            pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos,
                            "\x1b[2m%*s~ \x1b[0m", gutter_width - 2, "");
        }

        pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "\x1b[K\r\n");
    }

    /* Status bar (inverse video) */
    pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "\x1b[7m");
    {
        char left[128], right[64];
        snprintf(left, sizeof(left), " %s%s",
                 e->filename[0] ? e->filename : "[new file]",
                 e->dirty ? " [modified]" : "");
        snprintf(right, sizeof(right), " Ln %d, Col %d | %d lines ",
                 e->cursor_row + 1, e->cursor_col + 1, e->num_lines);

        int left_len = strlen(left);
        int right_len = strlen(right);
        int pad = e->screen_cols - left_len - right_len;
        if (pad < 0) pad = 0;

        pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "%s", left);
        for (int i = 0; i < pad; i++) {
            if (pos < RENDER_BUF_SIZE - 1) buf[pos++] = ' ';
        }
        pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "%s", right);
    }
    pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "\x1b[0m\r\n");

    /* Message bar */
    pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, "\x1b[K");
    if (e->status_msg[0]) {
        pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos, " %s", e->status_msg);
    } else {
        pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos,
                        " Ctrl+S = save | Ctrl+Q = quit");
    }

    int cursor_screen_row = e->cursor_row - e->row_offset + 1;
    int cursor_screen_col = e->cursor_col - e->col_offset + gutter_width + 1;
    pos += snprintf(buf + pos, RENDER_BUF_SIZE - pos,
                    "\x1b[%d;%dH\x1b[?25h",
                    cursor_screen_row, cursor_screen_col);

    write(STDOUT_FILENO, buf, pos);
}

/* ════════════════════════════════════════════════════════════════════
 * Part 7: Key Processing — YOUR TURN!
 * ════════════════════════════════════════════════════════════════════
 *
 * Implement process_key() — the brain of the editor.
 *
 * This function receives a key code (from read_key) and dispatches it
 * to the appropriate editor action. Return 0 to continue, 1 to quit.
 *
 * Key mapping:
 *   CTRL_KEY('q')     → Quit. But if the file is dirty and quit_confirm
 *                        is 0, show a warning in status_msg and set
 *                        quit_confirm to 1 instead. Only quit on the
 *                        second Ctrl+Q.
 *   CTRL_KEY('s')     → Save (editor_save_file)
 *   Arrow keys        → Move cursor (editor_move_cursor)
 *   Home / End        → Move cursor
 *   Page Up / Down    → Move cursor
 *   KEY_BACKSPACE     → Delete char before cursor (editor_delete_char)
 *   KEY_DELETE         → Delete char after cursor (editor_delete_forward)
 *   '\r' (Enter)      → Insert newline (editor_insert_newline)
 *   '\x1b' (Escape)   → Ignore
 *   32-126 (printable) → Insert character (editor_insert_char)
 *
 * IMPORTANT: Clear status_msg at the start. Reset quit_confirm to 0
 * on any key that isn't Ctrl+Q.
 */
int process_key(EditorState *e, int key)
{
    /* TODO: Clear the status message */

    /* TODO: Handle each key using a switch statement */

    /* Hint: the switch should cover these cases:
     *   case CTRL_KEY('q'):  — quit logic with dirty-file safety
     *   case CTRL_KEY('s'):  — save
     *   case KEY_ARROW_UP/DOWN/LEFT/RIGHT:  — cursor movement
     *   case KEY_HOME: case KEY_END:  — cursor movement
     *   case KEY_PAGE_UP: case KEY_PAGE_DOWN:  — cursor movement
     *   case KEY_BACKSPACE:  — delete backward
     *   case KEY_DELETE:     — delete forward
     *   case '\r':           — newline
     *   case '\x1b':         — ignore
     *   default:             — insert printable chars (32-126)
     */

    (void)e;
    (void)key;
    return 0;
}

/* ════════════════════════════════════════════════════════════════════
 * Part 8: Main — YOUR TURN!
 * ════════════════════════════════════════════════════════════════════
 *
 * Wire everything together:
 *
 *   1. Call enable_raw_mode() to take control of the terminal
 *   2. Create an EditorState and call editor_init()
 *   3. Call get_terminal_size() and set screen_rows (minus 2 for
 *      status bar + message bar) and screen_cols
 *   4. If argv[1] exists, load that file with editor_load_file()
 *   5. Enter the event loop:
 *      a. Call editor_render() to draw the screen
 *      b. Call read_key() to get the next keypress
 *      c. If key is KEY_NONE, continue (timeout, no input)
 *      d. Call process_key() — if it returns 1, break (quit)
 *   6. After the loop: clear the screen and call editor_destroy()
 */
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    /* TODO: Step 1 — enable raw mode */

    /* TODO: Step 2 — initialize editor state */

    /* TODO: Step 3 — get terminal size */

    /* TODO: Step 4 — load file if argv[1] is provided */

    /* TODO: Step 5 — event loop */

    /* TODO: Step 6 — clean up (clear screen, destroy editor) */

    return 0;
}
