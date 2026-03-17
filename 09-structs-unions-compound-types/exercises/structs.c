/*
 * Kata: Student Record System
 *
 * Build a CRUD system for student records using structs:
 *   - Create: initialize a student
 *   - Read: find a student by name
 *   - Update: modify a student's GPA
 *   - Delete: remove a student from the collection
 *
 * 14 tests total.
 *
 * Hints:
 *   - Use strcpy to copy strings into char arrays (can't use = for arrays)
 *   - Use strcmp to compare strings (returns 0 if equal)
 *   - For "delete", mark a record inactive rather than shifting the array
 *   - Pass structs by pointer for efficiency
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int age;
    float gpa;
    int active;      /* 1 if this record is in use, 0 if deleted */
} Student;

typedef struct {
    Student records[MAX_STUDENTS];
    int count;       /* number of active + deleted records */
} StudentDB;

/* ── Exercise 1: db_init ─────────────────────────────────────────────
 * Initialize the database to empty.
 */
void db_init(StudentDB *db)
{
    /* TODO: set count to 0 */
}

/* ── Exercise 2: db_add ──────────────────────────────────────────────
 * Add a student to the database.
 *
 * - Copy name, age, gpa into the next available slot
 * - Mark as active
 * - Increment count
 * - Return 0 on success, -1 if database is full
 */
int db_add(StudentDB *db, const char *name, int age, float gpa)
{
    /* TODO: check if full, then fill in next record */
    (void)db; (void)name; (void)age; (void)gpa;
    return -1; /* placeholder */
}

/* ── Exercise 3: db_find ─────────────────────────────────────────────
 * Find an active student by name.
 *
 * Returns: pointer to the Student, or NULL if not found
 */
Student *db_find(StudentDB *db, const char *name)
{
    /* TODO: loop through records, check active and name match */
    (void)db; (void)name;
    return NULL; /* placeholder */
}

/* ── Exercise 4: db_update_gpa ───────────────────────────────────────
 * Update a student's GPA by name.
 *
 * Returns: 0 on success, -1 if student not found
 */
int db_update_gpa(StudentDB *db, const char *name, float new_gpa)
{
    /* TODO: find the student, update gpa */
    (void)db; (void)name; (void)new_gpa;
    return -1; /* placeholder */
}

/* ── Exercise 5: db_delete ───────────────────────────────────────────
 * Delete a student by name (mark as inactive).
 *
 * Returns: 0 on success, -1 if student not found
 */
int db_delete(StudentDB *db, const char *name)
{
    /* TODO: find the student, set active=0 */
    (void)db; (void)name;
    return -1; /* placeholder */
}

/* ── Exercise 6: db_count_active ─────────────────────────────────────
 * Count the number of active (non-deleted) students.
 */
int db_count_active(const StudentDB *db)
{
    /* TODO: loop and count where active==1 */
    (void)db;
    return 0; /* placeholder */
}

/* ── Exercise 7: db_print ────────────────────────────────────────────
 * Print all active students in format:
 *   "  Name (age %d, GPA %.1f)\n"
 */
void db_print(const StudentDB *db)
{
    /* TODO: loop through records, print active ones */
    (void)db;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* float comparison with tolerance */
static int fequal(float a, float b) { return (a - b) < 0.01f && (b - a) < 0.01f; }

int main(void)
{
    printf("=== Student Record System ===\n\n");

    StudentDB db;
    db_init(&db);

    printf("db_add:\n");
    {
        int rc = db_add(&db, "Alice", 17, 3.8f);
        TEST("add Alice returns 0") EXPECT(rc == 0);
        db_add(&db, "Bob", 18, 3.5f);
        db_add(&db, "Charlie", 16, 3.9f);
        TEST("count is 3") EXPECT(db.count == 3);
    }

    printf("\ndb_find:\n");
    {
        Student *s = db_find(&db, "Bob");
        TEST("find Bob returns non-NULL") EXPECT(s != NULL);
        TEST("Bob's age is 18") EXPECT(s != NULL && s->age == 18);
        TEST("Bob's GPA is 3.5") EXPECT(s != NULL && fequal(s->gpa, 3.5f));

        Student *notfound = db_find(&db, "Dave");
        TEST("find Dave returns NULL") EXPECT(notfound == NULL);
    }

    printf("\ndb_update_gpa:\n");
    {
        int rc = db_update_gpa(&db, "Alice", 3.9f);
        TEST("update Alice returns 0") EXPECT(rc == 0);
        Student *s = db_find(&db, "Alice");
        TEST("Alice's new GPA is 3.9") EXPECT(s != NULL && fequal(s->gpa, 3.9f));

        rc = db_update_gpa(&db, "Nobody", 4.0f);
        TEST("update nonexistent returns -1") EXPECT(rc == -1);
    }

    printf("\ndb_delete:\n");
    {
        int rc = db_delete(&db, "Bob");
        TEST("delete Bob returns 0") EXPECT(rc == 0);
        TEST("find Bob after delete returns NULL") EXPECT(db_find(&db, "Bob") == NULL);
        TEST("active count is 2") EXPECT(db_count_active(&db) == 2);

        rc = db_delete(&db, "Nobody");
        TEST("delete nonexistent returns -1") EXPECT(rc == -1);
    }

    printf("\ndb_print:\n");
    {
        printf("  Active students:\n");
        db_print(&db);
        TEST("print runs without crash") EXPECT(1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
