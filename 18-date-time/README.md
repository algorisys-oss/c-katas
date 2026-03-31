# Module 18: Date & Time

## What You'll Learn
- How computers represent time as a single number (Unix timestamps)
- Breaking a timestamp into year, month, day, hour, minute, second
- Calendar math: leap years, days per month, day-of-week algorithms
- The C time API: `time_t`, `struct tm`, `gmtime`, `localtime`, `mktime`, `strftime`, `difftime`
- Time zones, UTC, and the nightmare of Daylight Saving Time
- Monotonic vs. wall clock time

---

## 1. Time Is Just a Number

Computers don't know what "March 17, 2026" means. Instead, they store time
as a single integer: **the number of seconds since midnight, January 1, 1970 UTC**.

That moment — `0` — is called the **Unix epoch**.

```
The Unix Epoch Timeline
========================

         epoch                                      "now"
           |                                          |
           v                                          v
  ---------+------------------------------------------+----------->
           0                                   1,774,000,000ish
       1970-01-01                                2026-03-17
       00:00:00 UTC

  Each tick = 1 second
  1 day     =    86,400 seconds
  1 year    ~= 31,536,000 seconds (365 days)
  1 leap yr ~= 31,622,400 seconds (366 days)
```

This "seconds since epoch" value is called a **Unix timestamp**. Right now,
the timestamp is somewhere around 1.77 billion. That's how many seconds have
passed since the start of 1970.

**Why 1970?** That's when Unix was being created at Bell Labs. They needed a
starting point and picked the nearest round date. It stuck.

**Why seconds?** Seconds are small enough to be precise for most uses, and
large enough that a 32-bit integer can hold about 68 years of them (we'll
come back to this — it's called the Y2038 problem).

---

## 2. From Timestamp to Human-Readable Date

How do you turn `1,710,000,000` into `"2024-03-09"`? You peel off layers:

```
 Unix Timestamp: 1,710,000,000 seconds
 ═══════════════════════════════════════

 Step 1: How many DAYS is that?
 ─────────────────────────────────
   1,710,000,000 / 86,400 = 19,791 days  (remainder = 46,400 seconds)

 Step 2: What TIME is that remainder?
 ─────────────────────────────────────
   46,400 / 3600 = 12 hours  (remainder = 3,200 seconds)
   3,200  / 60   = 53 minutes (remainder = 20 seconds)

   Time: 12:53:20 UTC

 Step 3: What DATE is day 19,791?
 ─────────────────────────────────
   Start at 1970-01-01.
   Count forward 19,791 days:
     1970: 365 days (not leap)  -> 19,791 - 365 = 19,426 remaining
     1971: 365 days             -> 19,426 - 365 = 19,061 remaining
     1972: 366 days (LEAP)      -> 19,061 - 366 = 18,695 remaining
     ...keep going year by year...
     2024: day 69 of the year   -> March 9

   Result: 2024-03-09  12:53:20 UTC
```

This is exactly what `gmtime()` does inside the C library. There's no magic
— just division and subtraction.

### The Reverse: Date to Timestamp

Going backwards (what `mktime()` does):

```
 Date: 2024-03-09  12:53:20 UTC
 ═══════════════════════════════

 Step 1: Count days from epoch to 2024-01-01
 ────────────────────────────────────────────
   Sum up days in each year 1970..2023:
     54 years, of which 13 are leap years
     = (54 * 365) + 13 = 19,723 days

 Step 2: Add days within 2024
 ────────────────────────────
   Jan: 31, Feb: 29 (leap!), Mar 1-9: 9
   = 31 + 29 + 9 = 69 days
   Total: 19,723 + 68 = 19,791 days  (day 69 means +68 full days)

 Step 3: Convert to seconds
 ──────────────────────────
   19,791 * 86,400 = 1,709,942,400
   + 12*3600 + 53*60 + 20 = 46,400
   = 1,709,988,800
```

---

## 3. Leap Year Rules

The calendar isn't simple. Earth takes approximately 365.2422 days to orbit
the sun, so we need corrections:

| Rule | Example | Leap? |
|------|---------|-------|
| Divisible by 4 | 2024 | Yes |
| ...but NOT by 100 | 1900 | No |
| ...unless ALSO by 400 | 2000 | Yes |

In code:
```c
int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
```

This gives us an average year length of 365.2425 days — close enough that
the error is only 1 day in 3,236 years.

### Days Per Month

```
 Month:  Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
 Days:    31  28  31  30  31  30  31  31  30  31  30  31
              ^^
              29 in leap years
```

The mnemonic "30 days hath September, April, June, and November" works,
but a lookup table in code is cleaner:

```c
static const int DAYS_IN_MONTH[] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
/* Index 0 unused — months are 1-12 */
/* For February, add 1 if is_leap_year(year) */
```

---

## 4. Day of the Week

Given any date, what day of the week is it? There are several algorithms.
We'll use **Tomohiko Sakamoto's algorithm** — it's elegant and fits in a
few lines:

```c
/* Returns 0 = Sunday, 1 = Monday, ..., 6 = Saturday */
int day_of_week(int year, int month, int day)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (month < 3) {
        year--;
    }
    return (year + year/4 - year/100 + year/400 + t[month - 1] + day) % 7;
}
```

**How does it work?** The lookup table `t[]` encodes the cumulative day-shift
for each month. The `if (month < 3) year--` trick treats January and February
as months 13 and 14 of the *previous* year, which simplifies the leap year
correction (since the leap day falls at the end of "February" = month 14).

Let's verify: **2024-03-09** should be a Saturday (day 6).

```
year = 2024, month = 3, day = 9
month >= 3, so year stays 2024

(2024 + 2024/4 - 2024/100 + 2024/400 + t[2] + 9) % 7
= (2024 + 506 - 20 + 5 + 2 + 9) % 7
= 2526 % 7
= 6  -> Saturday  ✓
```

---

## 5. The C Time API

The C standard library provides these tools in `<time.h>`:

### Core Types

```
 time_t              A number (usually int64_t) holding seconds since epoch.

 struct tm {         Broken-down time:
     int tm_sec;       Seconds      (0-60, 60 = leap second)
     int tm_min;       Minutes      (0-59)
     int tm_hour;      Hours        (0-23)
     int tm_mday;      Day of month (1-31)
     int tm_mon;       Month        (0-11)  <-- Watch out! 0-based!
     int tm_year;      Years since 1900     <-- Not the actual year!
     int tm_wday;      Day of week  (0-6, Sunday=0)
     int tm_yday;      Day of year  (0-365)
     int tm_isdst;     DST flag     (-1, 0, or 1)
 };
```

**Gotchas:**
- `tm_mon` is 0-based (January = 0), unlike how humans think
- `tm_year` is years since 1900, so 2024 is stored as `124`

### Key Functions

| Function | What it does |
|----------|-------------|
| `time(&t)` | Get current Unix timestamp |
| `gmtime(&t)` | Timestamp -> struct tm (UTC) |
| `localtime(&t)` | Timestamp -> struct tm (local time zone) |
| `mktime(&tm)` | struct tm (local) -> timestamp |
| `strftime(buf, size, fmt, &tm)` | Format time as string |
| `difftime(t2, t1)` | Difference in seconds (as double) |

### strftime Format Codes

```
 %Y  Four-digit year         2024
 %m  Month (01-12)           03
 %d  Day of month (01-31)    09
 %H  Hour (00-23)            14
 %M  Minute (00-59)          30
 %S  Second (00-59)          00
 %A  Full weekday name       Saturday
 %B  Full month name         March
 %F  Short for %Y-%m-%d      2024-03-09
 %T  Short for %H:%M:%S      14:30:00
```

---

## 6. Time Zones

UTC (Coordinated Universal Time) is the reference. Every time zone is
expressed as an offset from UTC:

```
 UTC-12  UTC-5   UTC   UTC+1  UTC+5:30  UTC+9   UTC+12
   |      |       |      |       |        |        |
   v      v       v      v       v        v        v
 ──+──────+───────+──────+───────+────────+────────+──>
 Baker  New York  London Paris  Mumbai   Tokyo   Auckland
 Island  (EST)    (GMT)  (CET)  (IST)    (JST)   (NZST)
```

### Daylight Saving Time (DST)

Twice a year, many regions shift their clocks by 1 hour:
- **Spring forward**: 2:00 AM jumps to 3:00 AM (one hour vanishes)
- **Fall back**: 2:00 AM jumps to 1:00 AM (one hour repeats)

This means:
- Some local times **don't exist** (the skipped hour in spring)
- Some local times are **ambiguous** (the repeated hour in fall)
- Time zone offsets **change** during the year

**This is why you should always store timestamps in UTC** and convert to
local time only for display.

---

## 7. Monotonic vs. Wall Clock Time

Your computer has two kinds of clocks:

| Clock | What it measures | Affected by NTP/DST? | Use for |
|-------|-----------------|---------------------|---------|
| Wall clock | "What time is it?" | Yes — can jump | Display |
| Monotonic | "How long since boot?" | No — always moves forward | Timing/benchmarks |

If you're measuring how long an operation takes, **always use the monotonic
clock**. The wall clock can jump backwards (NTP correction) or forward
(DST change).

In C (POSIX):
```c
#include <time.h>

struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
/* ... do work ... */
clock_gettime(CLOCK_MONOTONIC, &end);
double elapsed = (end.tv_sec - start.tv_sec)
               + (end.tv_nsec - start.tv_nsec) / 1e9;
```

---

## 8. The Y2038 Problem

On systems where `time_t` is a 32-bit signed integer, the maximum
representable time is:

```
 2^31 - 1 = 2,147,483,647 seconds
 = January 19, 2038, 03:14:07 UTC

 After this moment, the counter overflows to a negative number,
 and the system thinks it's December 13, 1901.
```

This is the same class of bug as Y2K. The fix: use 64-bit `time_t` (which
most modern 64-bit systems already do — good for 292 billion years).

---

## Exercises

### Kata: Date & Time Utilities (`exercises/datetime.c`)

Implement calendar math from scratch — no `<time.h>` allowed! Pure integer
arithmetic only. You'll build:

1. **`is_leap_year`** — the three-part leap year rule
2. **`days_in_month`** — lookup table + leap year adjustment
3. **`day_of_year`** — sum days of previous months + day
4. **`days_between`** — absolute difference between two dates
5. **`day_of_week`** — Tomohiko Sakamoto's algorithm
6. **`format_date`** — format as "YYYY-MM-DD" string

16 tests covering edge cases: century years, leap day, known weekdays,
cross-year spans, and date formatting.

---

[← Previous: Module 17 — Text, Unicode & Encoding](../17-text-unicode-encoding/README.md) | [Next: Module 19 — The Preprocessor & Build Systems →](../19-preprocessor-build-systems/README.md)
