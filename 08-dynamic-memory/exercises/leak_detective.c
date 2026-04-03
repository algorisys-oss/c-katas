/*
 * Kata: Leak Detective
 *
 * This file contains INTENTIONALLY BUGGY code with memory problems:
 *   - Memory leaks (malloc without free)
 *   - Double frees
 *   - Use after free
 *
 * Your job:
 *   1. Read each function carefully and find the bugs
 *   2. Fix all the bugs so every function is memory-safe
 *   3. Compile with: gcc -g -Wall -Wextra -std=c99 -o leak_detective leak_detective.c
 *   4. Verify with:  valgrind --leak-check=full ./leak_detective
 *
 * Target: valgrind reports "All heap blocks were freed -- no leaks are possible"
 *
 * 10 tests total (tests verify correct behavior, valgrind verifies no leaks).
 *
 * Hints:
 *   - Read the comments for each bug to understand what's wrong
 *   - After fixing, the output should still be correct
 *   - Compile with -g so valgrind shows line numbers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ── Bug 1: Memory Leak ──────────────────────────────────────────────
 * This function creates a copy of a string but the caller never
 * gets a chance to free the intermediate buffer.
 *
 * FIX the leak without changing what the function returns.
 */
char *make_greeting(const char *name)
{
    /* BUG: temp is allocated but never freed */
    char *temp = malloc(64);
    if (temp == NULL) return NULL;
    snprintf(temp, 64, "Hello, %s!", name);

    char *result = malloc(strlen(temp) + 1);
    if (result == NULL) return NULL;    /* TODO: is this the only problem here? */
    strcpy(result, temp);

    /* TODO: fix the leak */
    return result;
}

/* ── Bug 2: Double Free ──────────────────────────────────────────────
 * This function processes two arrays but frees one of them twice.
 *
 * FIX the double free.
 */
void process_data(int n)
{
    int *data_a = malloc(n * sizeof(int));
    int *data_b = data_a;  /* BUG: data_b points to the SAME memory as data_a */

    if (data_a == NULL) return;

    for (int i = 0; i < n; i++) {
        data_a[i] = i * 2;
    }

    /* TODO: data_b should be its own allocation, not an alias */

    free(data_a);
    free(data_b);   /* BUG: this frees the same memory again! */
}

/* ── Bug 3: Use After Free ───────────────────────────────────────────
 * This function builds a result string but reads from freed memory.
 *
 * FIX the use-after-free.
 */
int sum_from_string(const char *csv)
{
    /* Parse comma-separated integers and return their sum */
    char *copy = malloc(strlen(csv) + 1);
    if (copy == NULL) return 0;
    strcpy(copy, csv);

    free(copy);  /* BUG: freed too early! */

    int sum = 0;
    char *token = strtok(copy, ",");  /* BUG: reading freed memory! */
    while (token != NULL) {
        sum += atoi(token);
        token = strtok(NULL, ",");
    }

    /* TODO: move the free to the right place */
    return sum;
}

/* ── Bug 4: Leak in Error Path ───────────────────────────────────────
 * This function allocates two resources but leaks one if the second
 * allocation fails. (We simulate failure for testing.)
 *
 * FIX the leak in the error path.
 */
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
        /* Simulating label allocation failure */
        /* BUG: we return NULL but rec and rec->values are leaked! */
        return NULL;  /* TODO: free everything before returning */
    }

    rec->label = malloc(strlen(label) + 1);
    if (rec->label == NULL) {
        /* BUG: same issue — partial cleanup */
        return NULL;  /* TODO: free everything before returning */
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

/* ── Bug 5: Leak in Loop ─────────────────────────────────────────────
 * This function allocates memory in a loop but only frees the last one.
 *
 * FIX so all allocations are freed.
 */
int find_longest_word(const char *sentence)
{
    char *copy = malloc(strlen(sentence) + 1);
    if (copy == NULL) return 0;
    strcpy(copy, sentence);

    int longest = 0;
    char *token = strtok(copy, " ");
    while (token != NULL) {
        /* BUG: each iteration allocates a duplicate but never frees it */
        char *word = malloc(strlen(token) + 1);
        if (word == NULL) break;
        strcpy(word, token);

        int len = strlen(word);
        if (len > longest) {
            longest = len;
        }

        /* TODO: free word here! */

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
    printf("=== Leak Detective ===\n\n");

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
    printf("\nNow run: valgrind --leak-check=full ./leak_detective\n");
    printf("Target: 'All heap blocks were freed -- no leaks are possible'\n");
    return tests_passed == tests_total ? 0 : 1;
}
