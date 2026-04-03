/*
 * Kata: String Functions from Scratch
 *
 * Implement your own versions of the standard C string functions.
 * Do NOT use <string.h> — the whole point is to build them yourself!
 *
 * You'll implement:
 *   1. my_strlen    — count characters until '\0'
 *   2. my_strcpy    — copy a string (including '\0')
 *   3. my_strcmp     — compare two strings lexicographically
 *   4. my_strcat    — append one string to another
 *   5. my_strchr    — find first occurrence of a character
 *   6. my_strrev    — reverse a string in-place
 *
 * 20 tests total.
 *
 * Remember: a C string is just an array of chars ending with '\0'.
 * Every function needs to handle the null terminator correctly.
 */

#include <stdio.h>
#include <assert.h>

/* ── Exercise 1: my_strlen ──────────────────────────────────────────
 * Return the length of the string (not counting '\0').
 *
 * Example: my_strlen("Hello") → 5
 *
 * Hint: Walk forward from s[0] until you hit '\0', counting steps.
 */
int my_strlen(const char *s)
{
    /* TODO: Count characters until '\0' */
    return 0; /* placeholder */
}

/* ── Exercise 2: my_strcpy ──────────────────────────────────────────
 * Copy the string src into dest (including the '\0').
 * Return dest.
 *
 * Assume dest has enough space.
 */
char *my_strcpy(char *dest, const char *src)
{
    /* TODO: Copy each character from src to dest, including '\0' */
    return dest; /* placeholder */
}

/* ── Exercise 3: my_strcmp ──────────────────────────────────────────
 * Compare two strings lexicographically (character by character).
 *
 * Return:
 *   negative if a < b
 *   0        if a == b
 *   positive if a > b
 *
 * Hint: Walk both strings in parallel. If characters differ,
 *       return the difference. If both hit '\0', they're equal.
 */
int my_strcmp(const char *a, const char *b)
{
    /* TODO: Compare character by character */
    return 0; /* placeholder */
}

/* ── Exercise 4: my_strcat ──────────────────────────────────────────
 * Append src to the end of dest. Return dest.
 *
 * Hint: First find the end of dest (the '\0'), then copy src there.
 * Assume dest has enough space.
 */
char *my_strcat(char *dest, const char *src)
{
    /* TODO: Find end of dest, then copy src starting there */
    return dest; /* placeholder */
}

/* ── Exercise 5: my_strchr ──────────────────────────────────────────
 * Find the first occurrence of character c in string s.
 * Return a pointer to it, or NULL if not found.
 *
 * Note: If c is '\0', return a pointer to the terminating '\0'.
 */
char *my_strchr(const char *s, int c)
{
    /* TODO: Walk through s looking for c */
    return NULL; /* placeholder */
}

/* ── Exercise 6: my_strrev ──────────────────────────────────────────
 * Reverse the string in-place.
 *
 * Example: "Hello" → "olleH"
 *
 * Hint: Use two pointers — one at the start, one at the end.
 *       Swap and move inward.
 */
void my_strrev(char *s)
{
    /* TODO: Reverse the string in-place using two pointers */
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
    printf("=== String Functions from Scratch ===\n\n");

    /* --- my_strlen tests --- */
    printf("my_strlen:\n");
    TEST("strlen(\"Hello\") → 5")              EXPECT(my_strlen("Hello") == 5);
    TEST("strlen(\"\") → 0")                   EXPECT(my_strlen("") == 0);
    TEST("strlen(\"A\") → 1")                  EXPECT(my_strlen("A") == 1);
    TEST("strlen(\"Hello, World!\") → 13")     EXPECT(my_strlen("Hello, World!") == 13);

    /* --- my_strcpy tests --- */
    printf("\nmy_strcpy:\n");
    {
        char buf[32];
        my_strcpy(buf, "Hello");
        TEST("strcpy \"Hello\"")               EXPECT(buf[0]=='H' && buf[4]=='o' && buf[5]=='\0');
    }
    {
        char buf[32];
        my_strcpy(buf, "");
        TEST("strcpy empty string")            EXPECT(buf[0] == '\0');
    }
    {
        char buf[32];
        char *ret = my_strcpy(buf, "Test");
        TEST("strcpy returns dest")            EXPECT(ret == buf);
    }

    /* --- my_strcmp tests --- */
    printf("\nmy_strcmp:\n");
    TEST("strcmp(\"abc\", \"abc\") → 0")       EXPECT(my_strcmp("abc", "abc") == 0);
    TEST("strcmp(\"abc\", \"abd\") → negative") EXPECT(my_strcmp("abc", "abd") < 0);
    TEST("strcmp(\"abd\", \"abc\") → positive") EXPECT(my_strcmp("abd", "abc") > 0);
    TEST("strcmp(\"ab\", \"abc\") → negative")  EXPECT(my_strcmp("ab", "abc") < 0);
    TEST("strcmp(\"\", \"\") → 0")             EXPECT(my_strcmp("", "") == 0);

    /* --- my_strcat tests --- */
    printf("\nmy_strcat:\n");
    {
        char buf[32] = "Hello";
        my_strcat(buf, ", World!");
        TEST("strcat \"Hello\" + \", World!\"")
            EXPECT(buf[0]=='H' && buf[5]==',' && buf[12]=='!' && buf[13]=='\0');
    }
    {
        char buf[32] = "Hello";
        my_strcat(buf, "");
        TEST("strcat empty string")            EXPECT(buf[0]=='H' && buf[5]=='\0');
    }

    /* --- my_strchr tests --- */
    printf("\nmy_strchr:\n");
    {
        const char *s = "Hello, World!";
        TEST("strchr 'W' in \"Hello, World!\"")
            EXPECT(my_strchr(s, 'W') == s + 7);
        TEST("strchr 'z' → NULL")
            EXPECT(my_strchr(s, 'z') == NULL);
        TEST("strchr '\\0' → end of string")
            EXPECT(my_strchr(s, '\0') == s + 13);
    }

    /* --- my_strrev tests --- */
    printf("\nmy_strrev:\n");
    {
        char s1[] = "Hello";
        my_strrev(s1);
        TEST("strrev \"Hello\" → \"olleH\"")
            EXPECT(s1[0]=='o' && s1[1]=='l' && s1[2]=='l' && s1[3]=='e' && s1[4]=='H');
    }
    {
        char s2[] = "A";
        my_strrev(s2);
        TEST("strrev single char")             EXPECT(s2[0] == 'A');
    }
    {
        char s3[] = "";
        my_strrev(s3);
        TEST("strrev empty string")            EXPECT(s3[0] == '\0');
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
