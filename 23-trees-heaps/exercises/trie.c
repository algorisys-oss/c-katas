/*
 * Kata: Trie (Prefix Tree)
 *
 * A trie is a tree where each edge represents a character, and paths from
 * root to nodes spell out strings.  It's the go-to data structure for
 * prefix-based operations (autocomplete, spell checking, IP routing).
 *
 *       (root)
 *      /   |   \
 *     a    b    c
 *     |    |
 *     p    a
 *     |   / \
 *     p  l   t
 *    / \  |
 *  (l) (end:"app")  l
 *   |
 *   e
 *   |
 *  (end:"apple")
 *
 * Why tries beat hash tables for string work:
 *   - Exact lookup is O(L) where L = word length (no hash collisions)
 *   - Prefix search is O(L) — just walk the tree
 *   - Autocomplete is O(L + results) — DFS from the prefix node
 *
 * 18 tests total.
 *
 * Hints:
 *   - Each node has 26 children (a-z), most will be NULL
 *   - Map a character to an index: c - 'a'
 *   - prefix_count tracks how many words pass through each node
 *   - Autocomplete needs DFS + string building (pass a buffer down)
 *   - trie_destroy must be post-order: free children before the node itself
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
    int is_end_of_word;     /* 1 if a complete word ends here */
    int prefix_count;       /* how many words pass through this node */
} TrieNode;

typedef struct {
    TrieNode *root;
} Trie;

/* ── Exercise 1: trie_create_node / trie_create ──────────────────
 * trie_create_node: allocate one TrieNode, set all children to NULL,
 *   is_end_of_word to 0, prefix_count to 0.
 * trie_create: allocate a Trie struct and create its root node.
 * Return NULL if any malloc fails.
 */
TrieNode *trie_create_node(void)
{
    /* TODO: malloc a TrieNode, zero-init all fields */
    return NULL; /* placeholder */
}

Trie *trie_create(void)
{
    /* TODO: malloc a Trie, set root = trie_create_node() */
    return NULL; /* placeholder */
}

/* ── Exercise 2: trie_insert ─────────────────────────────────────
 * Insert a lowercase word into the trie character by character.
 * For each character:
 *   1. Compute index = c - 'a'
 *   2. If children[index] is NULL, create a new node there
 *   3. Move to that child and increment its prefix_count
 * After the loop, mark the final node as is_end_of_word = 1.
 */
void trie_insert(Trie *t, const char *word)
{
    /* TODO: walk the trie, creating nodes as needed */
}

/* ── Exercise 3: trie_search ─────────────────────────────────────
 * Search for an exact word in the trie.
 * Walk the tree character by character. If at any point children[index]
 * is NULL, the word doesn't exist — return 0.
 * If you reach the end of the word, return is_end_of_word (it might be
 * just a prefix of a longer word, not a word itself).
 */
int trie_search(Trie *t, const char *word)
{
    /* TODO: walk the trie, return is_end_of_word at the end */
    return 0; /* placeholder */
}

/* ── Exercise 4: trie_starts_with ────────────────────────────────
 * Check if any word in the trie starts with the given prefix.
 * Like search, but you don't need is_end_of_word — if you can walk
 * the entire prefix without hitting NULL, some word starts with it.
 */
int trie_starts_with(Trie *t, const char *prefix)
{
    /* TODO: walk the trie for each char in prefix */
    return 0; /* placeholder */
}

/* ── Exercise 5: trie_count_prefix ───────────────────────────────
 * Count how many words in the trie start with the given prefix.
 * Walk to the end of the prefix (if you can't, return 0).
 * Return the prefix_count of the node you land on.
 */
int trie_count_prefix(Trie *t, const char *prefix)
{
    /* TODO: walk to prefix node, return its prefix_count */
    return 0; /* placeholder */
}

/* ── Exercise 6: trie_autocomplete ───────────────────────────────
 * Given a prefix, find all words that start with it.
 *
 * Strategy:
 *   1. Walk to the prefix node (if it doesn't exist, return 0 results).
 *   2. DFS from that node, building up the current word in a buffer.
 *   3. When you hit an is_end_of_word node, copy the buffer into results.
 *
 * Parameters:
 *   - results: array of char pointers to fill with strdup'd words
 *   - max_results: size of the results array
 *
 * Return: number of results found.
 *
 * The caller is responsible for freeing each result string.
 */

/* Helper: DFS to collect words from a given node */
static void collect_words(TrieNode *node, char *buffer, int depth,
                          char **results, int max_results, int *count)
{
    /* TODO:
     * - If node is NULL or *count >= max_results, return
     * - If node->is_end_of_word, null-terminate buffer and
     *   strdup it into results[*count], then increment *count
     * - For each child (0..25), if it exists:
     *     - Put the character ('a' + i) into buffer[depth]
     *     - Recurse with depth + 1
     */
}

int trie_autocomplete(Trie *t, const char *prefix, char **results,
                      int max_results)
{
    /* TODO:
     * 1. Walk to the prefix node
     * 2. Set up a buffer with the prefix already in it
     * 3. Call collect_words from the prefix node
     * 4. Return the count
     */
    return 0; /* placeholder */
}

/* ── Exercise 7: trie_destroy ────────────────────────────────────
 * Free all nodes in the trie recursively (post-order).
 * For each node: destroy all children first, then free the node itself.
 * Finally free the Trie struct.
 */
static void destroy_node(TrieNode *node)
{
    /* TODO: recursively free children, then free node */
}

void trie_destroy(Trie *t)
{
    /* TODO: destroy_node(t->root), then free(t) */
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

        /* Check that all expected words are present (order may vary) */
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

    /* --- Destroy (check with valgrind for leaks) --- */
    printf("\ndestroy:\n");
    trie_destroy(t);
    TEST("trie destroyed (verify with valgrind)")
        EXPECT(1);  /* if we get here without crash, basic check passes */

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
