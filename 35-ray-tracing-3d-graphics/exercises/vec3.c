/*
 * Exercise: 3D Vector Math Library
 *
 * Implement a Vec3 struct and common 3D vector operations.
 * These are the building blocks for all 3D graphics and ray tracing.
 *
 * All functions use float precision.
 */

#include <stdio.h>
#include <math.h>

/* ---- Test Harness ---- */

static int tests_total = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* Float comparison with tolerance */
#define APPROX(a, b) (fabsf((a) - (b)) < 0.001f)

/* ---- Vec3 Struct ---- */

typedef struct {
    float x, y, z;
} Vec3;

/* ---- TODO: Implement these functions ---- */

/*
 * vec3_create: Return a Vec3 with the given x, y, z components.
 */
Vec3 vec3_create(float x, float y, float z)
{
    /* TODO: implement */
    Vec3 v = {0, 0, 0};
    (void)x; (void)y; (void)z;
    return v;
}

/*
 * vec3_add: Return the component-wise sum of a and b.
 *   result.x = a.x + b.x, etc.
 */
Vec3 vec3_add(Vec3 a, Vec3 b)
{
    /* TODO: implement */
    (void)a; (void)b;
    Vec3 v = {0, 0, 0};
    return v;
}

/*
 * vec3_sub: Return the component-wise difference a - b.
 */
Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    /* TODO: implement */
    (void)a; (void)b;
    Vec3 v = {0, 0, 0};
    return v;
}

/*
 * vec3_scale: Multiply each component of v by scalar s.
 */
Vec3 vec3_scale(Vec3 v, float s)
{
    /* TODO: implement */
    (void)v; (void)s;
    Vec3 r = {0, 0, 0};
    return r;
}

/*
 * vec3_dot: Return the dot product of a and b.
 *   dot = a.x*b.x + a.y*b.y + a.z*b.z
 *
 * The dot product tells you how "aligned" two vectors are:
 *   positive = same general direction
 *   zero     = perpendicular
 *   negative = opposite directions
 */
float vec3_dot(Vec3 a, Vec3 b)
{
    /* TODO: implement */
    (void)a; (void)b;
    return 0.0f;
}

/*
 * vec3_cross: Return the cross product of a and b.
 *   result.x = a.y*b.z - a.z*b.y
 *   result.y = a.z*b.x - a.x*b.z
 *   result.z = a.x*b.y - a.y*b.x
 *
 * The cross product gives a vector perpendicular to both a and b.
 */
Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    /* TODO: implement */
    (void)a; (void)b;
    Vec3 v = {0, 0, 0};
    return v;
}

/*
 * vec3_length: Return the magnitude (length) of v.
 *   length = sqrt(x^2 + y^2 + z^2)
 *
 * Hint: Use sqrtf() for float square root.
 */
float vec3_length(Vec3 v)
{
    /* TODO: implement */
    (void)v;
    return 0.0f;
}

/*
 * vec3_normalize: Return a unit vector in the same direction as v.
 *   normalized = v / length(v)
 *
 * A unit vector has length 1.0. This is essential for lighting calculations.
 */
Vec3 vec3_normalize(Vec3 v)
{
    /* TODO: implement */
    (void)v;
    Vec3 r = {0, 0, 0};
    return r;
}

/*
 * vec3_reflect: Reflect vector v across a surface with the given normal.
 *   R = v - 2 * dot(v, normal) * normal
 *
 * Used for mirror reflections: the incoming ray bounces off the surface.
 *
 *         v (incoming)     R (reflected)
 *          \              /
 *           \    normal  /
 *            \    ^     /
 *             \   |    /
 *              \  |   /
 *    ===========\=|=/=========== surface
 */
Vec3 vec3_reflect(Vec3 v, Vec3 normal)
{
    /* TODO: implement */
    (void)v; (void)normal;
    Vec3 r = {0, 0, 0};
    return r;
}

/* ---- Tests ---- */

int main(void)
{
    printf("=== Vec3 Library Tests ===\n\n");

    /* Test 1: Create */
    {
        Vec3 v = vec3_create(1.0f, 2.0f, 3.0f);
        TEST("vec3_create sets components")
            EXPECT(APPROX(v.x, 1.0f) && APPROX(v.y, 2.0f) && APPROX(v.z, 3.0f));
    }

    /* Test 2: Add */
    {
        Vec3 r = vec3_add(vec3_create(1.0f, 2.0f, 3.0f), vec3_create(4.0f, 5.0f, 6.0f));
        TEST("vec3_add adds components")
            EXPECT(APPROX(r.x, 5.0f) && APPROX(r.y, 7.0f) && APPROX(r.z, 9.0f));
    }

    /* Test 3: Sub */
    {
        Vec3 r = vec3_sub(vec3_create(5.0f, 7.0f, 9.0f), vec3_create(1.0f, 2.0f, 3.0f));
        TEST("vec3_sub subtracts components")
            EXPECT(APPROX(r.x, 4.0f) && APPROX(r.y, 5.0f) && APPROX(r.z, 6.0f));
    }

    /* Test 4: Scale */
    {
        Vec3 r = vec3_scale(vec3_create(1.0f, 2.0f, 3.0f), 2.0f);
        TEST("vec3_scale multiplies by scalar")
            EXPECT(APPROX(r.x, 2.0f) && APPROX(r.y, 4.0f) && APPROX(r.z, 6.0f));
    }

    /* Test 5: Scale by negative */
    {
        Vec3 r = vec3_scale(vec3_create(1.0f, -2.0f, 3.0f), -1.0f);
        TEST("vec3_scale with negative scalar")
            EXPECT(APPROX(r.x, -1.0f) && APPROX(r.y, 2.0f) && APPROX(r.z, -3.0f));
    }

    /* Test 6: Dot product — perpendicular */
    TEST("vec3_dot of perpendicular vectors is 0")
        EXPECT(APPROX(vec3_dot(vec3_create(1, 0, 0), vec3_create(0, 1, 0)), 0.0f));

    /* Test 7: Dot product — parallel */
    TEST("vec3_dot of parallel vectors")
        EXPECT(APPROX(vec3_dot(vec3_create(1, 2, 3), vec3_create(1, 2, 3)), 14.0f));

    /* Test 8: Cross product — X cross Y = Z */
    {
        Vec3 r = vec3_cross(vec3_create(1, 0, 0), vec3_create(0, 1, 0));
        TEST("vec3_cross: X cross Y = Z")
            EXPECT(APPROX(r.x, 0.0f) && APPROX(r.y, 0.0f) && APPROX(r.z, 1.0f));
    }

    /* Test 9: Cross product — Y cross X = -Z */
    {
        Vec3 r = vec3_cross(vec3_create(0, 1, 0), vec3_create(1, 0, 0));
        TEST("vec3_cross: Y cross X = -Z")
            EXPECT(APPROX(r.x, 0.0f) && APPROX(r.y, 0.0f) && APPROX(r.z, -1.0f));
    }

    /* Test 10: Length */
    TEST("vec3_length of (3, 4, 0) is 5")
        EXPECT(APPROX(vec3_length(vec3_create(3, 4, 0)), 5.0f));

    /* Test 11: Length of unit vector */
    TEST("vec3_length of unit X is 1")
        EXPECT(APPROX(vec3_length(vec3_create(1, 0, 0)), 1.0f));

    /* Test 12: Normalize produces unit length */
    {
        Vec3 n = vec3_normalize(vec3_create(3.0f, 4.0f, 0.0f));
        TEST("vec3_normalize produces unit length")
            EXPECT(APPROX(vec3_length(n), 1.0f));
    }

    /* Test 13: Normalize preserves direction */
    {
        Vec3 n = vec3_normalize(vec3_create(0.0f, 0.0f, 5.0f));
        TEST("vec3_normalize preserves direction")
            EXPECT(APPROX(n.x, 0.0f) && APPROX(n.y, 0.0f) && APPROX(n.z, 1.0f));
    }

    /* Test 14: Reflect */
    {
        Vec3 r = vec3_reflect(vec3_create(1.0f, -1.0f, 0.0f), vec3_create(0.0f, 1.0f, 0.0f));
        TEST("vec3_reflect bounces off surface")
            EXPECT(APPROX(r.x, 1.0f) && APPROX(r.y, 1.0f) && APPROX(r.z, 0.0f));
    }

    /* Summary */
    printf("\nResults: %d/%d passed\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("ALL TESTS PASSED\n");
    }
    return tests_passed == tests_total ? 0 : 1;
}
