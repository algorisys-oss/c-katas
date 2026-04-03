/*
 * Kata: Date & Time Utilities
 *
 * Implement calendar math from scratch using only integer arithmetic.
 * No <time.h> allowed!
 *
 * Functions to implement:
 *   - is_leap_year(year)
 *   - days_in_month(year, month)
 *   - day_of_year(year, month, day)
 *   - days_between(y1, m1, d1, y2, m2, d2)
 *   - day_of_week(year, month, day)
 *   - format_date(year, month, day, buf, size)
 *
 * 16 tests total.
 *
 * Hints:
 *   - Leap year: divisible by 4, but not 100, unless also 400
 *   - Use a lookup table for days per month (index 1-12)
 *   - For days_between, convert each date to an absolute day count
 *   - For day_of_week, use Tomohiko Sakamoto's algorithm
 *   - For format_date, use snprintf with "%04d-%02d-%02d"
 */

#include <stdio.h>
#include <string.h>

/* ── Days-per-month table (index 0 unused, months 1-12) ───────── */
static const int DAYS_IN_MONTH[] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* ── Exercise 1: Leap Year ─────────────────────────────────────── *
 * Return 1 if `year` is a leap year, 0 otherwise.
 *
 * Rules:
 *   - Divisible by 4         -> leap
 *   - BUT divisible by 100   -> NOT leap
 *   - UNLESS divisible by 400 -> leap
 */
int is_leap_year(int year)
{
    /* TODO: Implement the three-part leap year rule */
    (void)year;
    return 0;
}

/* ── Exercise 2: Days in Month ─────────────────────────────────── *
 * Return the number of days in the given month (1-12) of `year`.
 * February has 29 days in a leap year.
 */
int days_in_month(int year, int month)
{
    /* TODO: Look up from the table, adjust February for leap years */
    (void)year;
    (void)month;
    (void)DAYS_IN_MONTH;
    return 0;
}

/* ── Exercise 3: Day of Year ───────────────────────────────────── *
 * Return which day of the year the given date is (1-366).
 * January 1 = day 1, December 31 = day 365 or 366.
 */
int day_of_year(int year, int month, int day)
{
    /* TODO: Sum days of all months before `month`, then add `day` */
    (void)year;
    (void)month;
    (void)day;
    return 0;
}

/* ── Exercise 4: Days Between Dates ────────────────────────────── *
 * Return the absolute number of days between two dates.
 *
 * Hint: Convert each date to a total day count from some reference
 * point (e.g., year 1), then subtract. Use a helper function.
 */
int days_between(int y1, int m1, int d1, int y2, int m2, int d2)
{
    /* TODO: Implement using absolute day counts */
    (void)y1; (void)m1; (void)d1;
    (void)y2; (void)m2; (void)d2;
    return 0;
}

/* ── Exercise 5: Day of Week ───────────────────────────────────── *
 * Return the day of the week for the given date.
 * 0 = Sunday, 1 = Monday, ..., 6 = Saturday.
 *
 * Use Tomohiko Sakamoto's algorithm:
 *   - Lookup table t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}
 *   - If month < 3, decrement year
 *   - Result = (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7
 */
int day_of_week(int year, int month, int day)
{
    /* TODO: Implement Tomohiko Sakamoto's algorithm */
    (void)year;
    (void)month;
    (void)day;
    return 0;
}

/* ── Exercise 6: Format Date ───────────────────────────────────── *
 * Write the date as "YYYY-MM-DD" into buf (null-terminated).
 * Return 0 on success, -1 if buf is too small (need at least 11 bytes).
 */
int format_date(int year, int month, int day, char *buf, int size)
{
    /* TODO: Use snprintf to format as "%04d-%02d-%02d" */
    (void)year; (void)month; (void)day;
    (void)buf; (void)size;
    return -1;
}

/* ══════════════════════════════════════════════════════════════════
 * Tests — 16 total
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
    printf("=== Date & Time Utilities ===\n\n");

    /* --- is_leap_year --- */
    printf("is_leap_year:\n");
    TEST("2024 is a leap year (div by 4)")
        EXPECT(is_leap_year(2024) == 1);
    TEST("1900 is NOT a leap year (div by 100)")
        EXPECT(is_leap_year(1900) == 0);
    TEST("2000 IS a leap year (div by 400)")
        EXPECT(is_leap_year(2000) == 1);
    TEST("2023 is not a leap year")
        EXPECT(is_leap_year(2023) == 0);

    /* --- days_in_month --- */
    printf("\ndays_in_month:\n");
    TEST("January has 31 days")
        EXPECT(days_in_month(2024, 1) == 31);
    TEST("February 2024 (leap) has 29 days")
        EXPECT(days_in_month(2024, 2) == 29);
    TEST("February 2023 (non-leap) has 28 days")
        EXPECT(days_in_month(2023, 2) == 28);

    /* --- day_of_year --- */
    printf("\nday_of_year:\n");
    TEST("Jan 1 is day 1")
        EXPECT(day_of_year(2024, 1, 1) == 1);
    TEST("Mar 1 in leap year is day 61")
        EXPECT(day_of_year(2024, 3, 1) == 61);
    TEST("Dec 31 in leap year is day 366")
        EXPECT(day_of_year(2024, 12, 31) == 366);

    /* --- days_between --- */
    printf("\ndays_between:\n");
    TEST("same date = 0 days apart")
        EXPECT(days_between(2024, 3, 9, 2024, 3, 9) == 0);
    TEST("2024-01-01 to 2024-12-31 = 365 days")
        EXPECT(days_between(2024, 1, 1, 2024, 12, 31) == 365);
    TEST("2000-01-01 to 2024-03-09 = 8834 days")
        EXPECT(days_between(2000, 1, 1, 2024, 3, 9) == 8834);

    /* --- day_of_week --- */
    printf("\nday_of_week:\n");
    TEST("2024-03-09 is Saturday (6)")
        EXPECT(day_of_week(2024, 3, 9) == 6);
    TEST("1970-01-01 is Thursday (4)")
        EXPECT(day_of_week(1970, 1, 1) == 4);

    /* --- format_date --- */
    printf("\nformat_date:\n");
    {
        char buf[32];
        int rc = format_date(2024, 3, 9, buf, sizeof(buf));
        TEST("format 2024-03-09 correctly")
            EXPECT(rc == 0 && strcmp(buf, "2024-03-09") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
