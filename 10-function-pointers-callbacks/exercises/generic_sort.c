/*
 * Kata: Generic Sort with Comparators
 *
 * Implement a generic sort that works with any data type by accepting
 * a comparator callback — just like the standard library's qsort.
 *
 *   - my_sort: sort any array using a comparator function
 *   - compare_ints_asc, compare_ints_desc: int comparators
 *   - compare_strings: string comparator
 *   - compare_students_by_gpa: struct comparator
 *
 * 12 tests total.
 *
 * Hints:
 *   - Use bubble sort for simplicity (focus is on the callback pattern)
 *   - To swap generic elements, use memcpy with a temporary buffer
 *   - Cast void* to char* for pointer arithmetic: (char*)base + i * size
 *   - The comparator signature matches qsort: int (*)(const void*, const void*)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*Comparator)(const void *, const void *);

/* ── Exercise 1: my_sort ──────────────────────────────────────────────
 * Sort an array using bubble sort with a comparator callback.
 *
 * Parameters:
 *   base  — pointer to the array
 *   count — number of elements
 *   size  — size of each element in bytes
 *   cmp   — comparator function
 *
 * The comparator returns:
 *   negative if a < b, 0 if a == b, positive if a > b
 *
 * Use bubble sort: for each pair of adjacent elements, if cmp says
 * they're in the wrong order, swap them. Repeat until no swaps needed.
 */
void my_sort(void *base, int count, int size, Comparator cmp)
{
    /* TODO: implement bubble sort using the comparator
     * - Use (char*)base + i * size to access elements
     * - Use a char buffer[size] for swapping (or malloc) */
    (void)base; (void)count; (void)size; (void)cmp;
}

/* ── Exercise 2: Comparators ──────────────────────────────────────────
 * Write comparator functions for different types.
 */

/* Compare ints ascending: a < b → negative, a > b → positive */
int compare_ints_asc(const void *a, const void *b)
{
    /* TODO: cast to int*, compare */
    (void)a; (void)b;
    return 0; /* placeholder */
}

/* Compare ints descending: reverse of ascending */
int compare_ints_desc(const void *a, const void *b)
{
    /* TODO: opposite of ascending */
    (void)a; (void)b;
    return 0; /* placeholder */
}

/* Compare C strings (char*) alphabetically using strcmp.
 * Note: the array contains char* pointers, so a and b are char** */
int compare_strings(const void *a, const void *b)
{
    /* TODO: cast to const char**, dereference, use strcmp */
    (void)a; (void)b;
    return 0; /* placeholder */
}

/* ── A student struct for testing ──────────────────────────────────── */

typedef struct {
    char name[32];
    float gpa;
} Student;

/* Compare students by GPA descending (highest first) */
int compare_students_by_gpa(const void *a, const void *b)
{
    /* TODO: cast to Student*, compare gpa values */
    (void)a; (void)b;
    return 0; /* placeholder */
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
    printf("=== Generic Sort ===\n\n");

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
