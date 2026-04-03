/*
 * Solution: Word Frequency Counter
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

static void to_lower_str(char *s)
{
    for (int i = 0; s[i]; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

/* ── Internal hash table for counting ─────────────────────────────── */

#define FREQ_BUCKETS 64

typedef struct freq_node {
    char *word;
    int count;
    struct freq_node *next;
} freq_node_t;

static unsigned int freq_hash(const char *key)
{
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += (unsigned char)key[i];
    }
    return sum % FREQ_BUCKETS;
}

int count_words(const char *text, word_count_t *results, int max_results)
{
    /* Make a mutable copy for strtok */
    char *copy = strdup(text);
    if (!copy) return 0;

    /* Internal hash table */
    freq_node_t *buckets[FREQ_BUCKETS];
    memset(buckets, 0, sizeof(buckets));
    int unique = 0;

    /* Tokenize */
    const char *delims = " \t\n.,;:!?\"'()-";
    char *token = strtok(copy, delims);
    while (token) {
        to_lower_str(token);

        /* Skip empty tokens */
        if (token[0] == '\0') {
            token = strtok(NULL, delims);
            continue;
        }

        /* Look up in hash table */
        unsigned int idx = freq_hash(token);
        freq_node_t *node = buckets[idx];
        int found = 0;
        while (node) {
            if (strcmp(node->word, token) == 0) {
                node->count++;
                found = 1;
                break;
            }
            node = node->next;
        }

        if (!found) {
            freq_node_t *new_node = malloc(sizeof(freq_node_t));
            if (!new_node) break;
            new_node->word = strdup(token);
            if (!new_node->word) {
                free(new_node);
                break;
            }
            new_node->count = 1;
            new_node->next = buckets[idx];
            buckets[idx] = new_node;
            unique++;
        }

        token = strtok(NULL, delims);
    }

    /* Collect results */
    int result_count = 0;
    for (int i = 0; i < FREQ_BUCKETS && result_count < max_results; i++) {
        freq_node_t *node = buckets[i];
        while (node && result_count < max_results) {
            strncpy(results[result_count].word, node->word, MAX_WORD_LEN - 1);
            results[result_count].word[MAX_WORD_LEN - 1] = '\0';
            results[result_count].count = node->count;
            result_count++;
            node = node->next;
        }
    }

    /* Free internal hash table */
    for (int i = 0; i < FREQ_BUCKETS; i++) {
        freq_node_t *node = buckets[i];
        while (node) {
            freq_node_t *next = node->next;
            free(node->word);
            free(node);
            node = next;
        }
    }

    free(copy);
    return unique;
}

/* ── Helper for tests: find a word in results ─────────────────────── */

static int find_count(const word_count_t *results, int n, const char *word)
{
    for (int i = 0; i < n; i++) {
        if (strcmp(results[i].word, word) == 0) {
            return results[i].count;
        }
    }
    return -1;
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
    printf("=== Word Frequency Counter (Solution) ===\n\n");

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
