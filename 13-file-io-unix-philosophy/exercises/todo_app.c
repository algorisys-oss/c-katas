/*
 * Kata: Todo App with Fixed-Size Records
 *
 * Build a todo app backed by a binary file with fixed-size records.
 * Uses fseek for random access — a tiny database!
 *
 * The TodoRecord struct is exactly 64 bytes:
 *   - int id       (4 bytes)
 *   - char title[52] (52 bytes)
 *   - int done     (4 bytes)
 *   - int active   (4 bytes)
 *   = 64 bytes total
 *
 * Functions to implement:
 *   - todo_init:     create/truncate the file
 *   - todo_add:      append a new record
 *   - todo_get:      read record by id (using fseek)
 *   - todo_complete: mark a record as done (using fseek)
 *   - todo_delete:   mark a record as inactive (using fseek)
 *   - todo_list:     read all active records
 *
 * 12 tests total.
 *
 * Hints:
 *   - Record N starts at offset N * sizeof(TodoRecord)
 *   - id is assigned sequentially starting from 0
 *   - Use fseek(f, id * sizeof(TodoRecord), SEEK_SET) to jump to a record
 *   - "Deleting" just sets active = 0 (soft delete)
 *   - todo_add should seek to end to find current count, then append
 *   - Always open with "r+b" for read/write, "w+b" for init
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

/* ── Exercise 1: todo_init ───────────────────────────────────────────
 * Create or truncate the todo file.
 * Opens the file with "w+b" to create an empty file.
 * Returns 0 on success, -1 on failure.
 */
int todo_init(const char *filename)
{
    /* TODO: open file with "w+b", check for NULL, close it */
    return -1; /* placeholder */
}

/* ── Exercise 2: todo_add ────────────────────────────────────────────
 * Add a new todo record to the file.
 *
 * - Open file with "r+b"
 * - Seek to end to determine how many records exist (ftell / sizeof)
 * - That count becomes the new record's id
 * - Fill in the record: id, title (strncpy), done=0, active=1
 * - Write the record at the end
 * - Close file
 *
 * Returns the id of the new record, or -1 on failure.
 */
int todo_add(const char *filename, const char *title)
{
    /* TODO: open, seek to end, compute id, write record, close */
    return -1; /* placeholder */
}

/* ── Exercise 3: todo_get ────────────────────────────────────────────
 * Read a record by its id using random access.
 *
 * - Open file with "rb"
 * - Seek to id * sizeof(TodoRecord)
 * - Read one record into *out
 * - Close file
 *
 * Returns 0 on success, -1 on failure (bad id, read error, etc).
 */
int todo_get(const char *filename, int id, TodoRecord *out)
{
    /* TODO: open, seek to record position, read, close */
    return -1; /* placeholder */
}

/* ── Exercise 4: todo_complete ───────────────────────────────────────
 * Mark a record as done.
 *
 * - Read the record using todo_get (to verify it exists)
 * - Set done = 1
 * - Open file with "r+b"
 * - Seek to the record's position
 * - Write the updated record
 * - Close file
 *
 * Returns 0 on success, -1 on failure.
 */
int todo_complete(const char *filename, int id)
{
    /* TODO: get record, set done=1, seek, write back */
    return -1; /* placeholder */
}

/* ── Exercise 5: todo_delete ─────────────────────────────────────────
 * Soft-delete a record by setting active = 0.
 *
 * Same pattern as todo_complete but sets active = 0.
 *
 * Returns 0 on success, -1 on failure.
 */
int todo_delete(const char *filename, int id)
{
    /* TODO: get record, set active=0, seek, write back */
    return -1; /* placeholder */
}

/* ── Exercise 6: todo_list ───────────────────────────────────────────
 * Read all active records from the file.
 *
 * - Open file with "rb"
 * - Read records one at a time
 * - Only include records where active == 1
 * - Store them in the records[] array (up to max entries)
 * - Set *count to the number of active records found
 * - Close file
 *
 * Returns 0 on success, -1 on failure.
 */
int todo_list(const char *filename, TodoRecord records[], int max, int *count)
{
    /* TODO: open, read all records, filter active ones, close */
    return -1; /* placeholder */
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
