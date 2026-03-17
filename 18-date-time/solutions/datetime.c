/*
 * Solution: Date & Time Utilities
 *
 * Calendar math implemented from scratch using only integer arithmetic.
 * No <time.h> used.
 *
 * 16 tests total.
 */

#include <stdio.h>
#include <string.h>

/* ── Days-per-month table (index 0 unused, months 1-12) ───────── */
static const int DAYS_IN_MONTH[] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* ── Leap Year ─────────────────────────────────────────────────── */
int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/* ── Days in Month ─────────────────────────────────────────────── */
int days_in_month(int year, int month)
{
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return DAYS_IN_MONTH[month];
}

/* ── Day of Year ───────────────────────────────────────────────── */
int day_of_year(int year, int month, int day)
{
    int total = 0;
    for (int m = 1; m < month; m++) {
        total += days_in_month(year, m);
    }
    return total + day;
}

/* ── Helper: absolute day count from year 1 ────────────────────── *
 * Count total days from a reference point (year 1, Jan 1) to the
 * given date. This makes subtraction between any two dates simple.
 */
static long absolute_days(int year, int month, int day)
{
    long days = 0;

    /* Days contributed by complete years before `year` */
    long y = year - 1;
    days = y * 365 + y / 4 - y / 100 + y / 400;

    /* Add days for complete months in the current year */
    for (int m = 1; m < month; m++) {
        days += days_in_month(year, m);
    }

    /* Add remaining days */
    days += day;

    return days;
}

/* ── Days Between Dates ────────────────────────────────────────── */
int days_between(int y1, int m1, int d1, int y2, int m2, int d2)
{
    long a = absolute_days(y1, m1, d1);
    long b = absolute_days(y2, m2, d2);
    long diff = b - a;
    return (int)(diff < 0 ? -diff : diff);
}

/* ── Day of Week (Tomohiko Sakamoto's algorithm) ───────────────── */
int day_of_week(int year, int month, int day)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (month < 3) {
        year--;
    }
    return (year + year / 4 - year / 100 + year / 400
            + t[month - 1] + day) % 7;
}

/* ── Format Date ───────────────────────────────────────────────── */
int format_date(int year, int month, int day, char *buf, int size)
{
    /* "YYYY-MM-DD" needs 10 chars + null terminator = 11 bytes */
    if (size < 11) {
        return -1;
    }
    snprintf(buf, (size_t)size, "%04d-%02d-%02d", year, month, day);
    return 0;
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
