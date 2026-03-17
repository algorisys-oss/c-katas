/*
 * Solution: Screen Buffer — A 2D Rendering Canvas
 *
 * A screen buffer is a width x height grid of characters. You write characters
 * into it, then render the whole thing to a string (rows joined by '\n').
 *
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

/* ── Implementation ──────────────────────────────────────────────── */

ScreenBuffer screen_create(int width, int height)
{
    ScreenBuffer sb;
    sb.width = width;
    sb.height = height;
    sb.cells = malloc(width * height);
    if (!sb.cells) {
        fprintf(stderr, "screen_create: malloc failed\n");
        exit(1);
    }
    memset(sb.cells, ' ', width * height);
    return sb;
}

void screen_destroy(ScreenBuffer *sb)
{
    free(sb->cells);
    sb->cells = NULL;
}

void screen_set(ScreenBuffer *sb, int row, int col, char c)
{
    if (row < 0 || row >= sb->height || col < 0 || col >= sb->width) {
        return;
    }
    sb->cells[row * sb->width + col] = c;
}

void screen_clear(ScreenBuffer *sb)
{
    memset(sb->cells, ' ', sb->width * sb->height);
}

void screen_render_to_string(const ScreenBuffer *sb, char *out, int out_size)
{
    if (out_size <= 0) {
        return;
    }

    int pos = 0;
    int max_chars = out_size - 1;

    for (int r = 0; r < sb->height && pos < max_chars; r++) {
        for (int c = 0; c < sb->width && pos < max_chars; c++) {
            out[pos++] = sb->cells[r * sb->width + c];
        }
        if (pos < max_chars) {
            out[pos++] = '\n';
        }
    }

    out[pos] = '\0';
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
