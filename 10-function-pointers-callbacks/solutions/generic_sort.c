/*
 * Solution: Generic Sort with Comparators
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*Comparator)(const void *, const void *);

void my_sort(void *base, int count, int size, Comparator cmp)
{
    char *arr = (char *)base;
    char *tmp = malloc(size);
    if (tmp == NULL) return;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            char *a = arr + j * size;
            char *b = arr + (j + 1) * size;
            if (cmp(a, b) > 0) {
                memcpy(tmp, a, size);
                memcpy(a, b, size);
                memcpy(b, tmp, size);
            }
        }
    }
    free(tmp);
}

int compare_ints_asc(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    /* Don't use ia - ib: it overflows when ia and ib differ by > INT_MAX */
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int compare_ints_desc(const void *a, const void *b)
{
    return compare_ints_asc(b, a);
}

int compare_strings(const void *a, const void *b)
{
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
}

typedef struct {
    char name[32];
    float gpa;
} Student;

int compare_students_by_gpa(const void *a, const void *b)
{
    const Student *sa = (const Student *)a;
    const Student *sb = (const Student *)b;
    if (sb->gpa > sa->gpa) return 1;
    if (sb->gpa < sa->gpa) return -1;
    return 0;
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

int main(void)
{
    printf("=== Generic Sort (Solution) ===\n\n");

    printf("sort ints ascending:\n");
    {
        int arr[] = {5, 2, 8, 1, 9, 3};
        my_sort(arr, 6, sizeof(int), compare_ints_asc);
        TEST("first element is 1") EXPECT(arr[0] == 1);
        TEST("last element is 9") EXPECT(arr[5] == 9);
        TEST("fully sorted") EXPECT(arr[0]==1 && arr[1]==2 && arr[2]==3 &&
                                     arr[3]==5 && arr[4]==8 && arr[5]==9);
    }

    printf("\nsort ints descending:\n");
    {
        int arr[] = {5, 2, 8, 1, 9, 3};
        my_sort(arr, 6, sizeof(int), compare_ints_desc);
        TEST("first element is 9") EXPECT(arr[0] == 9);
        TEST("last element is 1") EXPECT(arr[5] == 1);
    }

    printf("\nsort strings:\n");
    {
        const char *words[] = {"banana", "apple", "cherry", "date"};
        my_sort(words, 4, sizeof(const char *), compare_strings);
        TEST("first is 'apple'") EXPECT(strcmp(words[0], "apple") == 0);
        TEST("last is 'date'") EXPECT(strcmp(words[3], "date") == 0);
        TEST("fully sorted") EXPECT(strcmp(words[0],"apple")==0 &&
                                     strcmp(words[1],"banana")==0 &&
                                     strcmp(words[2],"cherry")==0 &&
                                     strcmp(words[3],"date")==0);
    }

    printf("\nsort students by GPA:\n");
    {
        Student students[] = {
            {"Alice", 3.5f}, {"Bob", 3.9f}, {"Charlie", 3.2f}, {"Diana", 3.7f}
        };
        my_sort(students, 4, sizeof(Student), compare_students_by_gpa);
        TEST("highest GPA first (Bob)") EXPECT(strcmp(students[0].name, "Bob") == 0);
        TEST("lowest GPA last (Charlie)") EXPECT(strcmp(students[3].name, "Charlie") == 0);
    }

    printf("\nedge cases:\n");
    {
        int single[] = {42};
        my_sort(single, 1, sizeof(int), compare_ints_asc);
        TEST("single element unchanged") EXPECT(single[0] == 42);

        int empty[] = {0};
        my_sort(empty, 0, sizeof(int), compare_ints_asc);
        TEST("empty array doesn't crash") EXPECT(1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
