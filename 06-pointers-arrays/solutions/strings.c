/*
 * Solution: String Functions from Scratch
 */

#include <stdio.h>
#include <assert.h>

int my_strlen(const char *s)
{
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

char *my_strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++) != '\0')
        ;
    return dest;
}

int my_strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

char *my_strcat(char *dest, const char *src)
{
    char *d = dest;
    while (*d) d++;         /* find end of dest */
    while ((*d++ = *src++) != '\0')
        ;
    return dest;
}

char *my_strchr(const char *s, int c)
{
    while (*s != (char)c) {
        if (*s == '\0') return NULL;
        s++;
    }
    return (char *)s;
}

void my_strrev(char *s)
{
    if (*s == '\0') return;
    char *left = s;
    char *right = s;
    while (*(right + 1)) right++;   /* find last char */
    while (left < right) {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        left++;
        right--;
    }
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
    printf("=== String Functions (Solution) ===\n\n");

    printf("my_strlen:\n");
    TEST("strlen(\"Hello\") → 5")              EXPECT(my_strlen("Hello") == 5);
    TEST("strlen(\"\") → 0")                   EXPECT(my_strlen("") == 0);
    TEST("strlen(\"A\") → 1")                  EXPECT(my_strlen("A") == 1);
    TEST("strlen(\"Hello, World!\") → 13")     EXPECT(my_strlen("Hello, World!") == 13);

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

    printf("\nmy_strcmp:\n");
    TEST("strcmp(\"abc\", \"abc\") → 0")       EXPECT(my_strcmp("abc", "abc") == 0);
    TEST("strcmp(\"abc\", \"abd\") → negative") EXPECT(my_strcmp("abc", "abd") < 0);
    TEST("strcmp(\"abd\", \"abc\") → positive") EXPECT(my_strcmp("abd", "abc") > 0);
    TEST("strcmp(\"ab\", \"abc\") → negative")  EXPECT(my_strcmp("ab", "abc") < 0);
    TEST("strcmp(\"\", \"\") → 0")             EXPECT(my_strcmp("", "") == 0);

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
