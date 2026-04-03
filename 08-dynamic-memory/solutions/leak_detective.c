/*
 * Solution: Leak Detective
 *
 * All memory bugs fixed. Run with valgrind to verify:
 *   valgrind --leak-check=full ./sol-leak_detective
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Bug 1 FIXED: free temp before returning */
char *make_greeting(const char *name)
{
    char *temp = malloc(64);
    if (temp == NULL) return NULL;
    snprintf(temp, 64, "Hello, %s!", name);

    char *result = malloc(strlen(temp) + 1);
    if (result == NULL) {
        free(temp);     /* FIX: also free temp on this error path */
        return NULL;
    }
    strcpy(result, temp);

    free(temp);         /* FIX: free the temporary buffer */
    return result;
}

/* Bug 2 FIXED: give data_b its own allocation */
void process_data(int n)
{
    int *data_a = malloc(n * sizeof(int));
    if (data_a == NULL) return;

    int *data_b = malloc(n * sizeof(int));  /* FIX: separate allocation */
    if (data_b == NULL) {
        free(data_a);
        return;
    }

    for (int i = 0; i < n; i++) {
        data_a[i] = i * 2;
        data_b[i] = i * 3;     /* now data_b is independent */
    }

    free(data_a);
    free(data_b);
}

/* Bug 3 FIXED: move free(copy) to after we're done using it */
int sum_from_string(const char *csv)
{
    char *copy = malloc(strlen(csv) + 1);
    if (copy == NULL) return 0;
    strcpy(copy, csv);

    int sum = 0;
    char *token = strtok(copy, ",");
    while (token != NULL) {
        sum += atoi(token);
        token = strtok(NULL, ",");
    }

    free(copy);     /* FIX: free AFTER we're done parsing */
    return sum;
}

/* Bug 4 FIXED: clean up all allocations on error paths */
typedef struct {
    int *values;
    char *label;
} Record;

Record *create_record(const char *label, int n, int simulate_fail)
{
    Record *rec = malloc(sizeof(Record));
    if (rec == NULL) return NULL;

    rec->values = malloc(n * sizeof(int));
    if (rec->values == NULL) {
        free(rec);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        rec->values[i] = i + 1;
    }

    if (simulate_fail) {
        free(rec->values);  /* FIX: free values before returning */
        free(rec);           /* FIX: free rec before returning */
        return NULL;
    }

    rec->label = malloc(strlen(label) + 1);
    if (rec->label == NULL) {
        free(rec->values);  /* FIX: free values before returning */
        free(rec);           /* FIX: free rec before returning */
        return NULL;
    }
    strcpy(rec->label, label);

    return rec;
}

void destroy_record(Record *rec)
{
    if (rec == NULL) return;
    free(rec->values);
    free(rec->label);
    free(rec);
}

/* Bug 5 FIXED: free word inside the loop */
int find_longest_word(const char *sentence)
{
    char *copy = malloc(strlen(sentence) + 1);
    if (copy == NULL) return 0;
    strcpy(copy, sentence);

    int longest = 0;
    char *token = strtok(copy, " ");
    while (token != NULL) {
        char *word = malloc(strlen(token) + 1);
        if (word == NULL) break;
        strcpy(word, token);

        int len = strlen(word);
        if (len > longest) {
            longest = len;
        }

        free(word);     /* FIX: free word each iteration */

        token = strtok(NULL, " ");
    }

    free(copy);
    return longest;
}

/* ── Test Harness ─────────────────────────────────────────────────── */

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
    printf("=== Leak Detective (Solution) ===\n\n");

    printf("Bug 1 — Memory Leak:\n");
    {
        char *greeting = make_greeting("Alice");
        TEST("greeting says 'Hello, Alice!'")
            EXPECT(greeting != NULL && strcmp(greeting, "Hello, Alice!") == 0);
        free(greeting);

        char *g2 = make_greeting("Bob");
        TEST("greeting says 'Hello, Bob!'")
            EXPECT(g2 != NULL && strcmp(g2, "Hello, Bob!") == 0);
        free(g2);
    }

    printf("\nBug 2 — Double Free:\n");
    {
        process_data(5);
        TEST("process_data(5) runs without crash") EXPECT(1);
        process_data(10);
        TEST("process_data(10) runs without crash") EXPECT(1);
    }

    printf("\nBug 3 — Use After Free:\n");
    {
        TEST("sum '1,2,3' → 6") EXPECT(sum_from_string("1,2,3") == 6);
        TEST("sum '10,20,30,40' → 100") EXPECT(sum_from_string("10,20,30,40") == 100);
    }

    printf("\nBug 4 — Leak in Error Path:\n");
    {
        Record *r = create_record("test", 3, 0);
        TEST("normal creation succeeds") EXPECT(r != NULL && r->values[2] == 3);
        destroy_record(r);

        Record *r2 = create_record("fail", 3, 1);
        TEST("simulated failure returns NULL") EXPECT(r2 == NULL);
    }

    printf("\nBug 5 — Leak in Loop:\n");
    {
        TEST("longest word in 'the quick brown fox' → 5")
            EXPECT(find_longest_word("the quick brown fox") == 5);
        TEST("longest word in 'hi' → 2")
            EXPECT(find_longest_word("hi") == 2);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    printf("\nRun: valgrind --leak-check=full ./sol-leak_detective\n");
    printf("Expected: 'All heap blocks were freed -- no leaks are possible'\n");
    return tests_passed == tests_total ? 0 : 1;
}
