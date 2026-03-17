/*
 * Kata: 2D Point/Vector Library
 *
 * Build a 2D vector math library using structs:
 *   - point_add, point_sub: vector addition/subtraction
 *   - point_scale: multiply by a scalar
 *   - point_magnitude: length of the vector (distance from origin)
 *   - point_distance: distance between two points
 *   - point_dot: dot product of two vectors
 *   - point_normalize: unit vector (length 1) in the same direction
 *
 * 14 tests total.
 *
 * Hints:
 *   - Use sqrtf() from <math.h> for square roots
 *   - Compile with -lm flag for math library
 *   - Dot product: a.x*b.x + a.y*b.y
 *   - Magnitude: sqrtf(x*x + y*y)
 *   - Normalize: divide each component by magnitude
 *   - Handle zero vector in normalize (return {0,0} if magnitude is 0)
 */

#include <stdio.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Point;

/* ── Exercise 1: point_create ─────────────────────────────────────────
 * Create a point with given coordinates.
 */
Point point_create(float x, float y)
{
    Point p;
    /* TODO: set x and y */
    p.x = 0; p.y = 0;
    (void)x; (void)y;
    return p;
}

/* ── Exercise 2: point_add ────────────────────────────────────────────
 * Return a + b (component-wise addition).
 */
Point point_add(Point a, Point b)
{
    /* TODO: return {a.x + b.x, a.y + b.y} */
    (void)a; (void)b;
    return point_create(0, 0); /* placeholder */
}

/* ── Exercise 3: point_sub ────────────────────────────────────────────
 * Return a - b (component-wise subtraction).
 */
Point point_sub(Point a, Point b)
{
    /* TODO: return {a.x - b.x, a.y - b.y} */
    (void)a; (void)b;
    return point_create(0, 0); /* placeholder */
}

/* ── Exercise 4: point_scale ──────────────────────────────────────────
 * Return p scaled by factor s: {p.x * s, p.y * s}.
 */
Point point_scale(Point p, float s)
{
    /* TODO: multiply each component by s */
    (void)p; (void)s;
    return point_create(0, 0); /* placeholder */
}

/* ── Exercise 5: point_magnitude ──────────────────────────────────────
 * Return the length of the vector: sqrt(x^2 + y^2).
 */
float point_magnitude(Point p)
{
    /* TODO: use sqrtf */
    (void)p;
    return 0.0f; /* placeholder */
}

/* ── Exercise 6: point_distance ───────────────────────────────────────
 * Return the distance between two points.
 * (Hint: distance = magnitude of (a - b))
 */
float point_distance(Point a, Point b)
{
    /* TODO: subtract then get magnitude */
    (void)a; (void)b;
    return 0.0f; /* placeholder */
}

/* ── Exercise 7: point_dot ────────────────────────────────────────────
 * Return the dot product: a.x*b.x + a.y*b.y.
 */
float point_dot(Point a, Point b)
{
    /* TODO: compute dot product */
    (void)a; (void)b;
    return 0.0f; /* placeholder */
}

/* ── Exercise 8: point_normalize ──────────────────────────────────────
 * Return a unit vector (length 1) in the same direction.
 * If the vector has zero magnitude, return {0, 0}.
 */
Point point_normalize(Point p)
{
    /* TODO: divide by magnitude (check for zero first!) */
    (void)p;
    return point_create(0, 0); /* placeholder */
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
    printf("=== 2D Point/Vector Library ===\n\n");

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
