/*
 * Exercise: Screen Buffer — A 2D Rendering Canvas
 *
 * A screen buffer is a width x height grid of characters. You write characters
 * into it, then render the whole thing to a string (rows joined by '\n').
 * This avoids flicker by building the full frame before sending it to the
 * terminal.
 *
 * Implement all functions marked TODO.
 * 10 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* ── Screen Buffer Structure ─────────────────────────────────────── */

typedef struct {
    char *cells;     /* row-major array: cells[row * width + col] */
    int width;
    int height;
} ScreenBuffer;

/* ── Functions to Implement ──────────────────────────────────────── */

/*
 * screen_create: Allocate a screen buffer of the given dimensions.
 *   - malloc the cells array (width * height bytes)
 *   - fill all cells with spaces ' '
 *   - return the ScreenBuffer (by value)
 *
 * TODO: implement this function
 */
ScreenBuffer screen_create(int width, int height)
{
    ScreenBuffer sb = {0};
    (void)width;
    (void)height;
    /* TODO: allocate sb.cells, set width/height, fill with spaces */
    return sb;
}

/*
 * screen_destroy: Free the buffer's memory.
 *
 * TODO: implement this function
 */
void screen_destroy(ScreenBuffer *sb)
{
    (void)sb;
    /* TODO: free sb->cells and set it to NULL */
}

/*
 * screen_set: Set the character at (row, col) to `c`.
 *   - If row or col is out of bounds, do nothing.
 *
 * TODO: implement this function
 */
void screen_set(ScreenBuffer *sb, int row, int col, char c)
{
    (void)sb;
    (void)row;
    (void)col;
    (void)c;
    /* TODO */
}

/*
 * screen_clear: Fill the entire buffer with spaces ' '.
 *
 * TODO: implement this function
 */
void screen_clear(ScreenBuffer *sb)
{
    (void)sb;
    /* TODO */
}

/*
 * screen_render_to_string: Render the buffer into `out` as a string.
 *   - Each row is followed by a '\n'
 *   - Null-terminate the result
 *   - Do not write more than `out_size` bytes (including '\0')
 *
 *   Example for a 3-wide, 2-tall buffer with "Hi" on row 0:
 *     "Hi \n   \n"
 *
 * TODO: implement this function
 */
void screen_render_to_string(const ScreenBuffer *sb, char *out, int out_size)
{
    (void)sb;
    (void)out;
    (void)out_size;
    /* TODO */
}

/* ── Tests ───────────────────────────────────────────────────────── */

int main(void)
{
    printf("Screen Buffer Tests\n");
    printf("====================\n\n");

    char rendered[256];

    /* --- Creation --- */
    {
        ScreenBuffer sb = screen_create(4, 3);
        int all_spaces = 1;
        for (int i = 0; i < 4 * 3; i++) {
            if (sb.cells[i] != ' ') { all_spaces = 0; break; }
        }

        TEST("create: buffer is allocated and filled with spaces")
            EXPECT(sb.cells != NULL && sb.width == 4 && sb.height == 3 && all_spaces);

        screen_destroy(&sb);
    }

    /* --- screen_set --- */
    {
        ScreenBuffer sb = screen_create(5, 3);
        screen_set(&sb, 0, 0, 'X');
        screen_set(&sb, 1, 2, 'Y');

        TEST("set: places character at correct position")
            EXPECT(sb.cells[0] == 'X' && sb.cells[1 * 5 + 2] == 'Y');

        screen_destroy(&sb);
    }

    {
        ScreenBuffer sb = screen_create(3, 3);
        screen_set(&sb, -1, 0, 'Z');
        screen_set(&sb, 0, 3, 'Z');
        screen_set(&sb, 3, 0, 'Z');
        int all_spaces = 1;
        for (int i = 0; i < 9; i++) {
            if (sb.cells[i] != ' ') { all_spaces = 0; break; }
        }

        TEST("set: out-of-bounds is ignored")
            EXPECT(all_spaces);

        screen_destroy(&sb);
    }

    /* --- screen_clear --- */
    {
        ScreenBuffer sb = screen_create(4, 2);
        screen_set(&sb, 0, 0, 'A');
        screen_set(&sb, 1, 3, 'B');
        screen_clear(&sb);
        int all_spaces = 1;
        for (int i = 0; i < 8; i++) {
            if (sb.cells[i] != ' ') { all_spaces = 0; break; }
        }

        TEST("clear: resets all cells to spaces")
            EXPECT(all_spaces);

        screen_destroy(&sb);
    }

    /* --- screen_render_to_string --- */
    {
        ScreenBuffer sb = screen_create(3, 2);
        screen_render_to_string(&sb, rendered, sizeof(rendered));

        TEST("render: empty buffer gives rows of spaces with newlines")
            EXPECT(strcmp(rendered, "   \n   \n") == 0);

        screen_destroy(&sb);
    }

    {
        ScreenBuffer sb = screen_create(4, 2);
        screen_set(&sb, 0, 0, 'H');
        screen_set(&sb, 0, 1, 'i');
        screen_set(&sb, 1, 0, '!');
        screen_render_to_string(&sb, rendered, sizeof(rendered));

        TEST("render: characters appear in correct positions")
            EXPECT(strcmp(rendered, "Hi  \n!   \n") == 0);

        screen_destroy(&sb);
    }

    {
        ScreenBuffer sb = screen_create(5, 1);
        screen_set(&sb, 0, 0, 'A');
        screen_set(&sb, 0, 4, 'Z');
        screen_render_to_string(&sb, rendered, sizeof(rendered));

        TEST("render: single row buffer")
            EXPECT(strcmp(rendered, "A   Z\n") == 0);

        screen_destroy(&sb);
    }

    {
        ScreenBuffer sb = screen_create(1, 3);
        screen_set(&sb, 0, 0, 'X');
        screen_set(&sb, 2, 0, 'Y');
        screen_render_to_string(&sb, rendered, sizeof(rendered));

        TEST("render: single column buffer")
            EXPECT(strcmp(rendered, "X\n \nY\n") == 0);

        screen_destroy(&sb);
    }

    {
        ScreenBuffer sb = screen_create(5, 2);
        screen_set(&sb, 0, 0, 'A');
        screen_set(&sb, 0, 1, 'B');
        char small[5];
        screen_render_to_string(&sb, small, sizeof(small));

        TEST("render: respects output buffer size limit")
            EXPECT(small[4] == '\0');

        screen_destroy(&sb);
    }

    {
        ScreenBuffer sb = screen_create(3, 2);
        screen_set(&sb, 0, 0, 'Z');
        screen_clear(&sb);
        screen_render_to_string(&sb, rendered, sizeof(rendered));

        TEST("render: after set and clear, buffer is blank")
            EXPECT(strcmp(rendered, "   \n   \n") == 0);

        screen_destroy(&sb);
    }

    /* --- Summary --- */
    printf("\nResults: %d / %d passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
