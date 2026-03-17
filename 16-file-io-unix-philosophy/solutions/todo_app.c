/*
 * Solution: Todo App with Fixed-Size Records
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TITLE_LEN 52
#define TODO_FILE "/tmp/c_katas_todo_test.bin"

typedef struct {
    int id;
    char title[TITLE_LEN];
    int done;
    int active;
} TodoRecord;

/* ── todo_init ───────────────────────────────────────────────────── */

int todo_init(const char *filename)
{
    FILE *f = fopen(filename, "w+b");
    if (f == NULL) {
        return -1;
    }
    fclose(f);
    return 0;
}

/* ── todo_add ────────────────────────────────────────────────────── */

int todo_add(const char *filename, const char *title)
{
    FILE *f = fopen(filename, "r+b");
    if (f == NULL) {
        return -1;
    }

    /* Seek to end to determine current record count */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    int id = (int)(size / (long)sizeof(TodoRecord));

    /* Build the record */
    TodoRecord rec;
    memset(&rec, 0, sizeof(rec));
    rec.id = id;
    strncpy(rec.title, title, TITLE_LEN - 1);
    rec.title[TITLE_LEN - 1] = '\0';
    rec.done = 0;
    rec.active = 1;

    /* Write at end */
    size_t written = fwrite(&rec, sizeof(TodoRecord), 1, f);
    fclose(f);

    return (written == 1) ? id : -1;
}

/* ── todo_get ────────────────────────────────────────────────────── */

int todo_get(const char *filename, int id, TodoRecord *out)
{
    if (id < 0) {
        return -1;
    }

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return -1;
    }

    /* Check file size to validate id */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    int record_count = (int)(size / (long)sizeof(TodoRecord));

    if (id >= record_count) {
        fclose(f);
        return -1;
    }

    /* Seek to the record */
    fseek(f, (long)id * (long)sizeof(TodoRecord), SEEK_SET);
    size_t read_count = fread(out, sizeof(TodoRecord), 1, f);
    fclose(f);

    return (read_count == 1) ? 0 : -1;
}

/* ── todo_complete ───────────────────────────────────────────────── */

int todo_complete(const char *filename, int id)
{
    TodoRecord rec;
    if (todo_get(filename, id, &rec) != 0) {
        return -1;
    }

    rec.done = 1;

    FILE *f = fopen(filename, "r+b");
    if (f == NULL) {
        return -1;
    }

    fseek(f, (long)id * (long)sizeof(TodoRecord), SEEK_SET);
    size_t written = fwrite(&rec, sizeof(TodoRecord), 1, f);
    fclose(f);

    return (written == 1) ? 0 : -1;
}

/* ── todo_delete ─────────────────────────────────────────────────── */

int todo_delete(const char *filename, int id)
{
    TodoRecord rec;
    if (todo_get(filename, id, &rec) != 0) {
        return -1;
    }

    rec.active = 0;

    FILE *f = fopen(filename, "r+b");
    if (f == NULL) {
        return -1;
    }

    fseek(f, (long)id * (long)sizeof(TodoRecord), SEEK_SET);
    size_t written = fwrite(&rec, sizeof(TodoRecord), 1, f);
    fclose(f);

    return (written == 1) ? 0 : -1;
}

/* ── todo_list ───────────────────────────────────────────────────── */

int todo_list(const char *filename, TodoRecord records[], int max, int *count)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return -1;
    }

    *count = 0;
    TodoRecord rec;

    while (fread(&rec, sizeof(TodoRecord), 1, f) == 1) {
        if (rec.active && *count < max) {
            records[*count] = rec;
            (*count)++;
        }
    }

    fclose(f);
    return 0;
}

/* ── Test harness ────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    TodoRecord rec;
    TodoRecord list[10];
    int count;
    int ret;

    printf("=== Todo App (Fixed-Size Records) ===\n\n");

    /* Clean up any leftover test file */
    remove(TODO_FILE);

    /* --- Init tests --- */
    printf("init:\n");

    TEST("init creates file")
        EXPECT(todo_init(TODO_FILE) == 0);

    /* --- Add tests --- */
    printf("\nadd:\n");

    TEST("add first todo returns id 0")
        EXPECT(todo_add(TODO_FILE, "Buy groceries") == 0);

    TEST("add second todo returns id 1")
        EXPECT(todo_add(TODO_FILE, "Write C code") == 1);

    TEST("add third todo returns id 2")
        EXPECT(todo_add(TODO_FILE, "Read SICP") == 2);

    /* --- Get tests --- */
    printf("\nget:\n");

    ret = todo_get(TODO_FILE, 0, &rec);
    TEST("get record 0")
        EXPECT(ret == 0 && rec.id == 0 &&
               strcmp(rec.title, "Buy groceries") == 0 &&
               rec.done == 0 && rec.active == 1);

    ret = todo_get(TODO_FILE, 2, &rec);
    TEST("get record 2")
        EXPECT(ret == 0 && rec.id == 2 &&
               strcmp(rec.title, "Read SICP") == 0);

    TEST("get invalid id returns -1")
        EXPECT(todo_get(TODO_FILE, 99, &rec) == -1);

    /* --- Complete tests --- */
    printf("\ncomplete:\n");

    todo_complete(TODO_FILE, 1);
    todo_get(TODO_FILE, 1, &rec);
    TEST("complete record 1")
        EXPECT(rec.done == 1 && rec.active == 1);

    /* --- Delete tests --- */
    printf("\ndelete:\n");

    todo_delete(TODO_FILE, 0);
    todo_get(TODO_FILE, 0, &rec);
    TEST("delete record 0")
        EXPECT(rec.active == 0);

    /* --- List tests --- */
    printf("\nlist:\n");

    todo_list(TODO_FILE, list, 10, &count);
    TEST("list shows only active records")
        EXPECT(count == 2);

    todo_list(TODO_FILE, list, 10, &count);
    TEST("list records have correct data")
        EXPECT(count == 2 &&
               strcmp(list[0].title, "Write C code") == 0 &&
               list[0].done == 1 &&
               strcmp(list[1].title, "Read SICP") == 0 &&
               list[1].done == 0);

    todo_list(TODO_FILE, list, 1, &count);
    TEST("list with max=1 limits output")
        EXPECT(count == 1);

    /* Clean up test file */
    remove(TODO_FILE);

    /* --- Summary --- */
    printf("\n%d / %d tests passed\n", tests_passed, tests_total);

    return (tests_passed == tests_total) ? 0 : 1;
}
