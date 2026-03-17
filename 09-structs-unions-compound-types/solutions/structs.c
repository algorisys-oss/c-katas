/*
 * Solution: Student Record System
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int age;
    float gpa;
    int active;
} Student;

typedef struct {
    Student records[MAX_STUDENTS];
    int count;
} StudentDB;

void db_init(StudentDB *db)
{
    db->count = 0;
}

int db_add(StudentDB *db, const char *name, int age, float gpa)
{
    if (db->count >= MAX_STUDENTS) return -1;

    Student *s = &db->records[db->count];
    strncpy(s->name, name, NAME_LEN - 1);
    s->name[NAME_LEN - 1] = '\0';
    s->age = age;
    s->gpa = gpa;
    s->active = 1;
    db->count++;
    return 0;
}

Student *db_find(StudentDB *db, const char *name)
{
    for (int i = 0; i < db->count; i++) {
        if (db->records[i].active && strcmp(db->records[i].name, name) == 0) {
            return &db->records[i];
        }
    }
    return NULL;
}

int db_update_gpa(StudentDB *db, const char *name, float new_gpa)
{
    Student *s = db_find(db, name);
    if (s == NULL) return -1;
    s->gpa = new_gpa;
    return 0;
}

int db_delete(StudentDB *db, const char *name)
{
    Student *s = db_find(db, name);
    if (s == NULL) return -1;
    s->active = 0;
    return 0;
}

int db_count_active(const StudentDB *db)
{
    int count = 0;
    for (int i = 0; i < db->count; i++) {
        if (db->records[i].active) count++;
    }
    return count;
}

void db_print(const StudentDB *db)
{
    for (int i = 0; i < db->count; i++) {
        if (db->records[i].active) {
            const Student *s = &db->records[i];
            printf("  %s (age %d, GPA %.1f)\n", s->name, s->age, s->gpa);
        }
    }
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

static int fequal(float a, float b) { return (a - b) < 0.01f && (b - a) < 0.01f; }

int main(void)
{
    printf("=== Student Record System (Solution) ===\n\n");

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
