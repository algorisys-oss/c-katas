/*
 * Exercise: Gap Buffer — The Core Data Structure of a Text Editor
 *
 * A gap buffer stores text in an array with a "gap" of unused space at the
 * cursor position. Insertions and deletions at the cursor are O(1) because
 * you just write into the gap or shrink it — no shifting required.
 *
 * Layout:
 *   [text before cursor] [----gap----] [text after cursor]
 *    0..gap_start-1       gap_start..   gap_end..buf_size-1
 *                         gap_end-1
 *
 * Implement all functions marked TODO.
 * 16 tests total.
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

/* ── Gap Buffer Structure ────────────────────────────────────────── */

typedef struct {
    char *buf;       /* the underlying character array */
    int buf_size;    /* total capacity of buf */
    int gap_start;   /* index where the gap begins (= cursor position) */
    int gap_end;     /* index one past the end of the gap */
} GapBuffer;

/* ── Functions to Implement ──────────────────────────────────────── */

/*
 * gap_create: Allocate a gap buffer with the given capacity.
 *   - malloc the buf array of size `capacity`
 *   - the gap spans the entire buffer initially (gap_start=0, gap_end=capacity)
 *   - return the GapBuffer (by value)
 *
 * TODO: implement this function
 */
GapBuffer gap_create(int capacity)
{
    GapBuffer gb;
    (void)capacity;
    /* TODO: allocate gb.buf with malloc, set buf_size, gap_start, gap_end */
    return gb;
}

/*
 * gap_destroy: Free the buffer's memory.
 *
 * TODO: implement this function
 */
void gap_destroy(GapBuffer *gb)
{
    (void)gb;
    /* TODO: free gb->buf and set it to NULL */
}

/*
 * gap_insert: Insert character `c` at the cursor position.
 *   - Place c at buf[gap_start], then advance gap_start
 *   - If the gap is empty (gap_start == gap_end), do nothing
 *
 * TODO: implement this function
 */
void gap_insert(GapBuffer *gb, char c)
{
    (void)gb;
    (void)c;
    /* TODO */
}

/*
 * gap_delete: Delete the character before the cursor (backspace).
 *   - Move gap_start back by one (the character is now "inside" the gap)
 *   - If gap_start is already 0, do nothing
 *
 * TODO: implement this function
 */
void gap_delete(GapBuffer *gb)
{
    (void)gb;
    /* TODO */
}

/*
 * gap_move_left: Move the cursor one position to the left.
 *   - Copy buf[gap_start - 1] to buf[gap_end - 1]
 *   - Decrement both gap_start and gap_end
 *   - If gap_start is 0, do nothing
 *
 * TODO: implement this function
 */
void gap_move_left(GapBuffer *gb)
{
    (void)gb;
    /* TODO */
}

/*
 * gap_move_right: Move the cursor one position to the right.
 *   - Copy buf[gap_end] to buf[gap_start]
 *   - Increment both gap_start and gap_end
 *   - If gap_end is buf_size, do nothing
 *
 * TODO: implement this function
 */
void gap_move_right(GapBuffer *gb)
{
    (void)gb;
    /* TODO */
}

/*
 * gap_get_text: Extract the text content (skipping the gap) into `out`.
 *   - Copy buf[0..gap_start-1] then buf[gap_end..buf_size-1] into out
 *   - Null-terminate the result
 *   - Do not write more than `out_size` bytes (including the '\0')
 *
 * TODO: implement this function
 */
void gap_get_text(const GapBuffer *gb, char *out, int out_size)
{
    (void)gb;
    (void)out;
    (void)out_size;
    /* TODO */
}

/*
 * gap_length: Return the number of text characters (total size minus gap size).
 *
 * TODO: implement this function
 */
int gap_length(const GapBuffer *gb)
{
    (void)gb;
    /* TODO */
    return 0;
}

/* ── Tests ───────────────────────────────────────────────────────── */

int main(void)
{
    printf("Gap Buffer Tests\n");
    printf("================\n\n");

    char text[64];

    /* --- Basic creation --- */
    {
        GapBuffer gb = gap_create(16);

        TEST("create: gap spans entire buffer")
            EXPECT(gb.gap_start == 0 && gb.gap_end == 16 && gb.buf != NULL);

        TEST("create: length is 0 on empty buffer")
            EXPECT(gap_length(&gb) == 0);

        gap_destroy(&gb);
    }

    /* --- Insert single character --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'A');
        gap_get_text(&gb, text, sizeof(text));

        TEST("insert: single character")
            EXPECT(strcmp(text, "A") == 0 && gap_length(&gb) == 1);

        gap_destroy(&gb);
    }

    /* --- Insert multiple characters --- */
    {
        GapBuffer gb = gap_create(16);
        const char *s = "Hello";
        for (int i = 0; s[i]; i++) {
            gap_insert(&gb, s[i]);
        }
        gap_get_text(&gb, text, sizeof(text));

        TEST("insert: multiple characters spell 'Hello'")
            EXPECT(strcmp(text, "Hello") == 0 && gap_length(&gb) == 5);

        gap_destroy(&gb);
    }

    /* --- Insert fills buffer to capacity --- */
    {
        GapBuffer gb = gap_create(4);
        gap_insert(&gb, 'A');
        gap_insert(&gb, 'B');
        gap_insert(&gb, 'C');
        gap_insert(&gb, 'D');
        gap_get_text(&gb, text, sizeof(text));
        gap_insert(&gb, 'X');  /* gap full — ignored */

        TEST("insert: fills buffer to capacity")
            EXPECT(strcmp(text, "ABCD") == 0 && gap_length(&gb) == 4);

        gap_destroy(&gb);
    }

    /* --- Delete removes last inserted char --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'A');
        gap_insert(&gb, 'B');
        gap_delete(&gb);
        gap_get_text(&gb, text, sizeof(text));

        TEST("delete: removes last inserted char")
            EXPECT(strcmp(text, "A") == 0 && gap_length(&gb) == 1);

        gap_destroy(&gb);
    }

    /* --- Delete on empty buffer --- */
    {
        GapBuffer gb = gap_create(16);
        gap_delete(&gb);

        TEST("delete: on empty buffer does nothing")
            EXPECT(gap_length(&gb) == 0);

        gap_destroy(&gb);
    }

    /* --- Delete all characters --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'X');
        gap_insert(&gb, 'Y');
        gap_delete(&gb);
        gap_delete(&gb);
        gap_get_text(&gb, text, sizeof(text));

        TEST("delete: all characters results in empty text")
            EXPECT(strcmp(text, "") == 0 && gap_length(&gb) == 0);

        gap_destroy(&gb);
    }

    /* --- Move left then insert --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'A');
        gap_insert(&gb, 'B');
        gap_move_left(&gb);
        gap_insert(&gb, 'X');
        gap_get_text(&gb, text, sizeof(text));

        TEST("move_left: moves cursor back one position")
            EXPECT(strcmp(text, "AXB") == 0);

        gap_destroy(&gb);
    }

    /* --- Move left at start --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'A');
        gap_move_left(&gb);
        gap_move_left(&gb);  /* already at start */
        gap_insert(&gb, 'Z');
        gap_get_text(&gb, text, sizeof(text));

        TEST("move_left: at start of buffer does nothing")
            EXPECT(strcmp(text, "ZA") == 0);

        gap_destroy(&gb);
    }

    /* --- Move right then insert --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'A');
        gap_insert(&gb, 'B');
        gap_insert(&gb, 'C');
        gap_move_left(&gb);
        gap_move_left(&gb);
        gap_move_right(&gb);
        gap_insert(&gb, 'X');
        gap_get_text(&gb, text, sizeof(text));

        TEST("move_right: moves cursor forward one position")
            EXPECT(strcmp(text, "ABXC") == 0);

        gap_destroy(&gb);
    }

    /* --- Move right at end --- */
    {
        GapBuffer gb = gap_create(16);
        gap_insert(&gb, 'A');
        gap_move_right(&gb);  /* already at end */
        gap_insert(&gb, 'Z');
        gap_get_text(&gb, text, sizeof(text));

        TEST("move_right: at end of text does nothing")
            EXPECT(strcmp(text, "AZ") == 0);

        gap_destroy(&gb);
    }

    /* --- Insert in middle after move_left --- */
    {
        GapBuffer gb = gap_create(16);
        const char *s = "ABCD";
        for (int i = 0; s[i]; i++) {
            gap_insert(&gb, s[i]);
        }
        gap_move_left(&gb);
        gap_move_left(&gb);
        gap_insert(&gb, '1');
        gap_insert(&gb, '2');
        gap_get_text(&gb, text, sizeof(text));

        TEST("insert after move_left: text in middle")
            EXPECT(strcmp(text, "AB12CD") == 0);

        gap_destroy(&gb);
    }

    /* --- Delete after move_left --- */
    {
        GapBuffer gb = gap_create(16);
        const char *s = "ABCD";
        for (int i = 0; s[i]; i++) {
            gap_insert(&gb, s[i]);
        }
        gap_move_left(&gb);
        gap_delete(&gb);
        gap_get_text(&gb, text, sizeof(text));

        TEST("delete after move_left: removes correct char")
            EXPECT(strcmp(text, "ABD") == 0);

        gap_destroy(&gb);
    }

    /* --- get_text respects size limit --- */
    {
        GapBuffer gb = gap_create(16);
        const char *s = "HelloWorld";
        for (int i = 0; s[i]; i++) {
            gap_insert(&gb, s[i]);
        }
        char small[6];
        gap_get_text(&gb, small, sizeof(small));

        TEST("get_text: respects output buffer size limit")
            EXPECT(small[5] == '\0' && strncmp(small, "Hello", 5) == 0);

        gap_destroy(&gb);
    }

    /* --- get_text on empty buffer --- */
    {
        GapBuffer gb = gap_create(16);
        gap_get_text(&gb, text, sizeof(text));

        TEST("get_text: empty buffer gives empty string")
            EXPECT(strcmp(text, "") == 0);

        gap_destroy(&gb);
    }

    /* --- Summary --- */
    printf("\nResults: %d / %d passed\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
