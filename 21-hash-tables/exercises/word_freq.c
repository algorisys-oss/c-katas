/*
 * Kata: Word Frequency Counter
 *
 * Count how many times each word appears in a text string.
 * Uses a simple hash table internally to track counts.
 *
 *   - count_words(text, results, max_results) — tokenize and count
 *
 * A word_count_t struct holds a word and its count.
 * Returns the number of unique words found.
 *
 * 8 tests total.
 *
 * Hints:
 *   - Use strtok() or manual scanning to split on spaces/punctuation
 *   - Convert to lowercase for case-insensitive counting
 *   - Use a hash table internally (array of linked lists)
 *   - Only count alphabetic words (skip punctuation, numbers)
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAX_WORD_LEN 64

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} word_count_t;

/* ── Helper: to_lower_str ─────────────────────────────────────────
 * Convert a string to lowercase in-place.
 */
static void to_lower_str(char *s)
{
    for (int i = 0; s[i]; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

/* ── Internal hash table for counting ─────────────────────────────
 * You'll build a small hash table inside count_words to track
 * word frequencies. Define whatever structs/helpers you need here.
 */

#define FREQ_BUCKETS 64

typedef struct freq_node {
    char *word;
    int count;
    struct freq_node *next;
} freq_node_t;

/* ── Exercise: count_words ────────────────────────────────────────
 * Tokenize `text` into words (split on spaces and punctuation).
 * Count how many times each unique word appears (case-insensitive).
 * Store results in the `results` array (up to max_results entries).
 * Return the number of unique words found.
 *
 * Steps:
 *   1. Make a mutable copy of text (strtok modifies its input)
 *   2. Create a hash table (array of freq_node_t linked lists)
 *   3. For each word token:
 *      a. Convert to lowercase
 *      b. Skip if empty or non-alphabetic
 *      c. Look up in hash table — increment count or insert new
 *   4. Copy results from hash table into the results array
 *   5. Free the hash table and the text copy
 *   6. Return unique word count
 */
int count_words(const char *text, word_count_t *results, int max_results)
{
    /* TODO: make a mutable copy of text with strdup() */
    /* TODO: create a hash table (array of FREQ_BUCKETS pointers, all NULL) */
    /* TODO: tokenize with strtok(), delimiters: " \t\n.,;:!?\"'()-" */
    /* TODO: for each token, lowercase it, hash it, find or insert in table */
    /* TODO: collect results into the results array */
    /* TODO: free all hash table nodes and the text copy */
    (void)text;
    (void)results;
    (void)max_results;
    return 0; /* placeholder */
}

/* ── Helper for tests: find a word in results ─────────────────── */
static int find_count(const word_count_t *results, int n, const char *word)
{
    for (int i = 0; i < n; i++) {
        if (strcmp(results[i].word, word) == 0) {
            return results[i].count;
        }
    }
    return -1; /* not found */
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-40s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Word Frequency Counter ===\n\n");

    /* --- single word --- */
    printf("single word:\n");
    {
        word_count_t results[10];
        int n = count_words("hello", results, 10);
        TEST("one word → 1 unique") EXPECT(n == 1);
        TEST("hello count = 1") EXPECT(find_count(results, n, "hello") == 1);
    }

    /* --- repeated words --- */
    printf("\nrepeated words:\n");
    {
        word_count_t results[10];
        int n = count_words("the cat sat on the mat the cat", results, 10);
        TEST("unique word count = 5") EXPECT(n == 5);
        TEST("the count = 3") EXPECT(find_count(results, n, "the") == 3);
        TEST("cat count = 2") EXPECT(find_count(results, n, "cat") == 2);
    }

    /* --- case insensitive --- */
    printf("\ncase insensitive:\n");
    {
        word_count_t results[10];
        int n = count_words("Hello hello HELLO", results, 10);
        TEST("case insensitive → 1 unique") EXPECT(n == 1);
        TEST("hello count = 3") EXPECT(find_count(results, n, "hello") == 3);
    }

    /* --- punctuation handling --- */
    printf("\npunctuation:\n");
    {
        word_count_t results[10];
        int n = count_words("yes, yes! YES.", results, 10);
        TEST("strips punctuation → 1 unique") EXPECT(n == 1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
