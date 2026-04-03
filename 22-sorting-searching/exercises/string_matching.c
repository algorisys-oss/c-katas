/*
 * Kata: String Pattern Matching Algorithms
 *
 * String matching is one of the most fundamental problems in CS:
 * "given a text and a pattern, find where the pattern occurs in the text."
 *
 * We build up from brute force to two classic algorithms:
 *   - KMP (Knuth-Morris-Pratt): never go backwards in the text
 *   - Rabin-Karp: use rolling hashes to skip comparisons
 *
 * Exercises:
 *   - brute_force_search  (baseline O(n*m) algorithm)
 *   - build_kmp_table     (the prefix function — the hard part)
 *   - kmp_search          (linear search using the prefix table)
 *   - rabin_karp_search   (rolling hash string matching)
 *
 * 14 tests total.
 *
 * ASCII art — why brute force is O(n*m) in the worst case:
 *
 *   Text:    "AAAAAAAAB"
 *   Pattern: "AAAB"
 *
 *   Position 0: AAAA vs AAAB → match,match,match,FAIL at index 3
 *   Position 1:  AAAA vs AAAB → match,match,match,FAIL
 *   Position 2:   AAAA vs AAAB → match,match,match,FAIL
 *   ...
 *   Position 5:      AAAB vs AAAB → MATCH!
 *
 *   Each attempt does m comparisons before failing. n attempts total.
 *   That's O(n * m).
 *
 * ASCII art — KMP prefix table construction:
 *
 *   Pattern: "ABABAC"
 *   Index:    0 1 2 3 4 5
 *
 *   The prefix table (also called failure function) for each position j
 *   stores the length of the longest proper prefix of pattern[0..j]
 *   that is ALSO a suffix of pattern[0..j].
 *
 *   j=0: "A"      → no proper prefix that's also a suffix → table[0] = 0
 *   j=1: "AB"     → prefixes: "A". Suffixes: "B". No match → table[1] = 0
 *   j=2: "ABA"    → prefix "A" == suffix "A" → table[2] = 1
 *   j=3: "ABAB"   → prefix "AB" == suffix "AB" → table[3] = 2
 *   j=4: "ABABA"  → prefix "ABA" == suffix "ABA" → table[4] = 3
 *   j=5: "ABABAC" → no prefix matches suffix ending in C → table[5] = 0
 *
 *   Result: [0, 0, 1, 2, 3, 0]
 *
 *   When a mismatch occurs at index 5 ('C'), the table says:
 *   "The prefix 'ABA' (length 3) matches the suffix just before
 *   the mismatch, so jump to index 3 instead of starting over."
 *
 * ASCII art — Rabin-Karp rolling hash:
 *
 *   Text: "ABCDEF"   Pattern: "BCD"   base=256, prime=101
 *
 *   hash("ABC") = (65*256^2 + 66*256 + 67) % 101
 *
 *   Sliding to "BCD":
 *   hash("BCD") = (hash("ABC") - 65*256^2) * 256 + 68) % 101
 *                  ──────────────────────  ─────  ───
 *                  remove 'A' contribution  shift   add 'D'
 *
 *   If hash("BCD") == hash(pattern), verify character by character
 *   (because hash collisions can cause false positives).
 */

#include <stdio.h>
#include <string.h>

/* ── Exercise 1: Brute Force String Search ────────────────────────── *
 * For each position i in text, check if pattern matches starting at i.
 * Return the index of the first match, or -1 if not found.
 *
 * Time:  O(n * m) worst case
 * Space: O(1)
 *
 * This is the baseline — simple but slow for pathological inputs.
 */
int brute_force_search(const char *text, const char *pattern)
{
    /* TODO: For each starting position in text, compare each character
     * of pattern. Return starting index on match, -1 if not found. */
    (void)text; (void)pattern;
    return -1;
}

/* ── Exercise 2: Build KMP Prefix Table ───────────────────────────── *
 * Build the failure/prefix function for the KMP algorithm.
 *
 * table[j] = length of the longest proper prefix of pattern[0..j]
 *            that is also a suffix of pattern[0..j].
 *
 * Algorithm:
 *   - table[0] = 0 (a single character has no proper prefix)
 *   - Use variable `len` to track the length of the current matching prefix
 *   - For each position j from 1 to m-1:
 *       - If pattern[j] == pattern[len], extend the match: table[j] = ++len
 *       - Else if len > 0, fall back: len = table[len - 1] and retry
 *       - Else, no match: table[j] = 0
 *
 * Example:
 *   Pattern: "ABABAC"
 *   Table:   [0, 0, 1, 2, 3, 0]
 */
void build_kmp_table(const char *pattern, int *table)
{
    /* TODO: Build the prefix function table */
    (void)pattern; (void)table;
}

/* ── Exercise 3: KMP Search ───────────────────────────────────────── *
 * Search for pattern in text using the KMP algorithm.
 *
 * The key idea: when a mismatch occurs at pattern[j], don't restart
 * from the beginning of the pattern. Instead, use table[j-1] to jump
 * to the position where matching can continue.
 *
 * We NEVER go backwards in the text — each character is visited at most
 * twice (once by the text pointer, once indirectly through the table).
 * That's why it's O(n + m).
 *
 * Return the index of the first match, or -1 if not found.
 *
 * Time:  O(n + m)
 * Space: O(m) for the prefix table
 */
int kmp_search(const char *text, const char *pattern)
{
    /* TODO: Build the prefix table, then scan text.
     * Use two indices: i for text, j for pattern.
     * On match: advance both.
     * On mismatch: if j > 0, j = table[j-1]; else i++. */
    (void)text; (void)pattern;
    return -1;
}

/* ── Exercise 4: Rabin-Karp Search ────────────────────────────────── *
 * Rolling hash string matching.
 *
 * 1. Compute hash of the pattern
 * 2. Compute hash of the first window of text (same length as pattern)
 * 3. Slide the window one character at a time:
 *    - Remove the contribution of the leftmost character
 *    - Add the contribution of the new rightmost character
 *    - If hashes match, verify character by character (avoid false positives)
 *
 * Use polynomial rolling hash:
 *   hash = (c0 * base^(m-1) + c1 * base^(m-2) + ... + c_{m-1}) % prime
 *
 * Parameters: base = 256, prime = 101
 *
 * Return the index of the first match, or -1 if not found.
 *
 * Time:  O(n + m) average, O(n * m) worst case (many hash collisions)
 * Space: O(1)
 */
int rabin_karp_search(const char *text, const char *pattern)
{
    /* TODO: Implement rolling hash search.
     * 1. Compute h = base^(m-1) % prime (for removing leading character)
     * 2. Compute initial hashes for pattern and first window
     * 3. Slide window, update hash, check matches */
    (void)text; (void)pattern;
    return -1;
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

static int arr_eq(const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void)
{
    printf("=== String Pattern Matching ===\n\n");

    /* --- brute_force_search --- */
    printf("brute_force_search:\n");
    {
        TEST("find \"BCD\" in \"ABCDEF\"")
            EXPECT(brute_force_search("ABCDEF", "BCD") == 1);
    }
    {
        TEST("pattern not found")
            EXPECT(brute_force_search("ABCDEF", "XYZ") == -1);
    }
    {
        TEST("pattern at start")
            EXPECT(brute_force_search("HELLO", "HEL") == 0);
    }

    /* --- build_kmp_table --- */
    printf("\nbuild_kmp_table:\n");
    {
        int table[6];
        int exp[] = {0, 0, 1, 2, 3, 0};
        build_kmp_table("ABABAC", table);
        TEST("table for \"ABABAC\"")
            EXPECT(arr_eq(table, exp, 6));
    }
    {
        int table[7];
        int exp[] = {0, 0, 0, 0, 1, 2, 3};
        build_kmp_table("ABCDABC", table);
        TEST("table for \"ABCDABC\"")
            EXPECT(arr_eq(table, exp, 7));
    }
    {
        int table[5];
        int exp[] = {0, 1, 2, 3, 4};
        build_kmp_table("AAAAA", table);
        TEST("table for \"AAAAA\"")
            EXPECT(arr_eq(table, exp, 5));
    }

    /* --- kmp_search --- */
    printf("\nkmp_search:\n");
    {
        TEST("find \"ABABAC\" in text")
            EXPECT(kmp_search("ABABABACABC", "ABABAC") == 2);
    }
    {
        TEST("pattern not found")
            EXPECT(kmp_search("ABCDEFGH", "XYZ") == -1);
    }
    {
        TEST("multiple occurrences (find first)")
            EXPECT(kmp_search("AAAAAA", "AA") == 0);
    }
    {
        TEST("pattern longer than text")
            EXPECT(kmp_search("AB", "ABCD") == -1);
    }

    /* --- rabin_karp_search --- */
    printf("\nrabin_karp_search:\n");
    {
        TEST("find \"BCD\" in \"ABCDEF\"")
            EXPECT(rabin_karp_search("ABCDEF", "BCD") == 1);
    }
    {
        TEST("pattern not found")
            EXPECT(rabin_karp_search("ABCDEF", "XYZ") == -1);
    }
    {
        TEST("pattern at end")
            EXPECT(rabin_karp_search("ABCDEF", "DEF") == 3);
    }
    {
        TEST("entire text is pattern")
            EXPECT(rabin_karp_search("HELLO", "HELLO") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
