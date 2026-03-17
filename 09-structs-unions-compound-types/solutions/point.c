/*
 * Solution: 2D Point/Vector Library
 */

#include <stdio.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Point;

Point point_create(float x, float y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

Point point_add(Point a, Point b)
{
    return point_create(a.x + b.x, a.y + b.y);
}

Point point_sub(Point a, Point b)
{
    return point_create(a.x - b.x, a.y - b.y);
}

Point point_scale(Point p, float s)
{
    return point_create(p.x * s, p.y * s);
}

float point_magnitude(Point p)
{
    return sqrtf(p.x * p.x + p.y * p.y);
}

float point_distance(Point a, Point b)
{
    return point_magnitude(point_sub(a, b));
}

float point_dot(Point a, Point b)
{
    return a.x * b.x + a.y * b.y;
}

Point point_normalize(Point p)
{
    float mag = point_magnitude(p);
    if (mag < 0.0001f) {
        return point_create(0.0f, 0.0f);
    }
    return point_create(p.x / mag, p.y / mag);
}

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

static int fequal(float a, float b)
{
    float diff = a - b;
    if (diff < 0) diff = -diff;
    return diff < 0.001f;
}

int main(void)
{
    printf("=== 2D Point/Vector Library (Solution) ===\n\n");

    printf("point_create:\n");
    {
        Point p = point_create(3.0f, 4.0f);
        TEST("create (3,4) → x=3") EXPECT(fequal(p.x, 3.0f));
        TEST("create (3,4) → y=4") EXPECT(fequal(p.y, 4.0f));
    }

    printf("\npoint_add/sub:\n");
    {
        Point a = point_create(1.0f, 2.0f);
        Point b = point_create(3.0f, 4.0f);
        Point sum = point_add(a, b);
        TEST("(1,2)+(3,4) = (4,6)") EXPECT(fequal(sum.x, 4.0f) && fequal(sum.y, 6.0f));

        Point diff = point_sub(a, b);
        TEST("(1,2)-(3,4) = (-2,-2)") EXPECT(fequal(diff.x, -2.0f) && fequal(diff.y, -2.0f));
    }

    printf("\npoint_scale:\n");
    {
        Point p = point_create(3.0f, 4.0f);
        Point s = point_scale(p, 2.0f);
        TEST("(3,4)*2 = (6,8)") EXPECT(fequal(s.x, 6.0f) && fequal(s.y, 8.0f));

        Point s2 = point_scale(p, -1.0f);
        TEST("(3,4)*-1 = (-3,-4)") EXPECT(fequal(s2.x, -3.0f) && fequal(s2.y, -4.0f));
    }

    printf("\npoint_magnitude:\n");
    {
        Point p = point_create(3.0f, 4.0f);
        TEST("|(3,4)| = 5") EXPECT(fequal(point_magnitude(p), 5.0f));

        Point z = point_create(0.0f, 0.0f);
        TEST("|(0,0)| = 0") EXPECT(fequal(point_magnitude(z), 0.0f));
    }

    printf("\npoint_distance:\n");
    {
        Point a = point_create(0.0f, 0.0f);
        Point b = point_create(3.0f, 4.0f);
        TEST("distance (0,0)→(3,4) = 5") EXPECT(fequal(point_distance(a, b), 5.0f));

        TEST("distance (3,4)→(3,4) = 0") EXPECT(fequal(point_distance(b, b), 0.0f));
    }

    printf("\npoint_dot:\n");
    {
        Point a = point_create(1.0f, 0.0f);
        Point b = point_create(0.0f, 1.0f);
        TEST("(1,0)·(0,1) = 0 (perpendicular)") EXPECT(fequal(point_dot(a, b), 0.0f));

        Point c = point_create(2.0f, 3.0f);
        Point d = point_create(4.0f, 5.0f);
        TEST("(2,3)·(4,5) = 23") EXPECT(fequal(point_dot(c, d), 23.0f));
    }

    printf("\npoint_normalize:\n");
    {
        Point p = point_create(3.0f, 4.0f);
        Point n = point_normalize(p);
        TEST("normalize (3,4) → magnitude ≈ 1")
            EXPECT(fequal(point_magnitude(n), 1.0f));
        TEST("normalize (3,4) → direction preserved")
            EXPECT(fequal(n.x, 0.6f) && fequal(n.y, 0.8f));
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
