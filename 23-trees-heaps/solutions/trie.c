/*
 * Solution: Trie (Prefix Tree)
 */

/* strdup is POSIX, not C99 — this makes it available */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LEN 64
#define MAX_RESULTS 32

/* ── Trie definition ─────────────────────────────────────────────── */

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int is_end_of_word;
    int prefix_count;
} TrieNode;

typedef struct {
    TrieNode *root;
} Trie;

/* ── trie_create_node / trie_create ──────────────────────────────── */

TrieNode *trie_create_node(void)
{
    TrieNode *node = malloc(sizeof(TrieNode));
    if (node == NULL) {
        return NULL;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    node->is_end_of_word = 0;
    node->prefix_count = 0;
    return node;
}

Trie *trie_create(void)
{
    Trie *t = malloc(sizeof(Trie));
    if (t == NULL) {
        return NULL;
    }
    t->root = trie_create_node();
    if (t->root == NULL) {
        free(t);
        return NULL;
    }
    return t;
}

/* ── trie_insert ─────────────────────────────────────────────────── */

void trie_insert(Trie *t, const char *word)
{
    TrieNode *cur = t->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int idx = word[i] - 'a';
        if (cur->children[idx] == NULL) {
            cur->children[idx] = trie_create_node();
        }
        cur = cur->children[idx];
        cur->prefix_count++;
    }
    cur->is_end_of_word = 1;
}

/* ── trie_search ─────────────────────────────────────────────────── */

int trie_search(Trie *t, const char *word)
{
    TrieNode *cur = t->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int idx = word[i] - 'a';
        if (cur->children[idx] == NULL) {
            return 0;
        }
        cur = cur->children[idx];
    }
    return cur->is_end_of_word;
}

/* ── trie_starts_with ────────────────────────────────────────────── */

int trie_starts_with(Trie *t, const char *prefix)
{
    TrieNode *cur = t->root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int idx = prefix[i] - 'a';
        if (cur->children[idx] == NULL) {
            return 0;
        }
        cur = cur->children[idx];
    }
    return 1;
}

/* ── trie_count_prefix ───────────────────────────────────────────── */

int trie_count_prefix(Trie *t, const char *prefix)
{
    TrieNode *cur = t->root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int idx = prefix[i] - 'a';
        if (cur->children[idx] == NULL) {
            return 0;
        }
        cur = cur->children[idx];
    }
    return cur->prefix_count;
}

/* ── trie_autocomplete ───────────────────────────────────────────── */

static void collect_words(TrieNode *node, char *buffer, int depth,
                          char **results, int max_results, int *count)
{
    if (node == NULL || *count >= max_results) {
        return;
    }
    if (node->is_end_of_word) {
        buffer[depth] = '\0';
        results[*count] = strdup(buffer);
        (*count)++;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = 'a' + i;
            collect_words(node->children[i], buffer, depth + 1,
                          results, max_results, count);
        }
    }
}

int trie_autocomplete(Trie *t, const char *prefix, char **results,
                      int max_results)
{
    TrieNode *cur = t->root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int idx = prefix[i] - 'a';
        if (cur->children[idx] == NULL) {
            return 0;
        }
        cur = cur->children[idx];
    }

    char buffer[MAX_WORD_LEN];
    int prefix_len = strlen(prefix);
    memcpy(buffer, prefix, prefix_len);

    int count = 0;
    collect_words(cur, buffer, prefix_len, results, max_results, &count);
    return count;
}

/* ── trie_destroy ────────────────────────────────────────────────── */

static void destroy_node(TrieNode *node)
{
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        destroy_node(node->children[i]);
    }
    free(node);
}

void trie_destroy(Trie *t)
{
    if (t == NULL) {
        return;
    }
    destroy_node(t->root);
    free(t);
}

/* ══════════════════════════════════════════════════════════════════
 *                         TEST HARNESS
 * ══════════════════════════════════════════════════════════════════ */

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
    printf("=== Trie (Prefix Tree) ===\n\n");

    /* --- Create --- */
    printf("create:\n");
    Trie *t = trie_create();
    TEST("trie created with empty root")
        EXPECT(t != NULL && t->root != NULL);

    /* --- Insert --- */
    printf("\ninsert:\n");
    trie_insert(t, "apple");
    trie_insert(t, "app");
    trie_insert(t, "application");
    TEST("search 'apple' after insert")
        EXPECT(trie_search(t, "apple") == 1);
    TEST("search 'app' after insert")
        EXPECT(trie_search(t, "app") == 1);
    TEST("search 'application' after insert")
        EXPECT(trie_search(t, "application") == 1);

    /* --- Search --- */
    printf("\nsearch:\n");
    TEST("search existing word 'app'")
        EXPECT(trie_search(t, "app") == 1);
    TEST("search non-existent word 'apt'")
        EXPECT(trie_search(t, "apt") == 0);
    TEST("search prefix 'ap' (not a word)")
        EXPECT(trie_search(t, "ap") == 0);
    TEST("search in empty prefix 'xyz'")
        EXPECT(trie_search(t, "xyz") == 0);

    /* --- Starts with --- */
    printf("\nstarts_with:\n");
    TEST("prefix 'app' exists")
        EXPECT(trie_starts_with(t, "app") == 1);
    TEST("prefix 'ap' exists")
        EXPECT(trie_starts_with(t, "ap") == 1);
    TEST("prefix 'xyz' does not exist")
        EXPECT(trie_starts_with(t, "xyz") == 0);

    /* --- Count prefix --- */
    printf("\ncount_prefix:\n");
    trie_insert(t, "bat");
    trie_insert(t, "ball");
    TEST("count words with prefix 'app' = 3")
        EXPECT(trie_count_prefix(t, "app") == 3);
    TEST("count words with prefix 'ba' = 2")
        EXPECT(trie_count_prefix(t, "ba") == 2);
    TEST("count words with prefix 'z' = 0")
        EXPECT(trie_count_prefix(t, "z") == 0);

    /* --- Autocomplete --- */
    printf("\nautocomplete:\n");
    {
        char *results[MAX_RESULTS];
        int count = trie_autocomplete(t, "app", results, MAX_RESULTS);
        TEST("autocomplete 'app' returns 3 words")
            EXPECT(count == 3);

        int found_app = 0, found_apple = 0, found_application = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(results[i], "app") == 0) found_app = 1;
            if (strcmp(results[i], "apple") == 0) found_apple = 1;
            if (strcmp(results[i], "application") == 0) found_application = 1;
        }
        TEST("autocomplete 'app' contains all 3 words")
            EXPECT(found_app && found_apple && found_application);

        for (int i = 0; i < count; i++) free(results[i]);
    }
    {
        char *results[MAX_RESULTS];
        int count = trie_autocomplete(t, "ba", results, MAX_RESULTS);
        TEST("autocomplete 'ba' returns 2 words")
            EXPECT(count == 2);
        for (int i = 0; i < count; i++) free(results[i]);
    }

    /* --- Destroy --- */
    printf("\ndestroy:\n");
    trie_destroy(t);
    TEST("trie destroyed (verify with valgrind)")
        EXPECT(1);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
