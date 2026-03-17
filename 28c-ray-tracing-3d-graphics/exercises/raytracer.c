/*
 * Exercise: Basic Ray Tracer — Ray-Sphere Intersection & Shading
 *
 * Implement the core of a ray tracer:
 *   1. Create rays (origin + direction)
 *   2. Test ray-sphere intersection by solving the quadratic equation
 *   3. Compute surface normals at intersection points
 *   4. Apply Lambertian diffuse shading
 *
 * All math uses float precision.
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

#define APPROX(a, b) (fabsf((a) - (b)) < 0.001f)

/* ---- Vec3 (provided — same as your vec3.c exercise) ---- */

typedef struct {
    float x, y, z;
} Vec3;

Vec3 vec3_create(float x, float y, float z)
{
    Vec3 v = {x, y, z};
    return v;
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
    return vec3_create(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_scale(Vec3 v, float s)
{
    return vec3_create(v.x * s, v.y * s, v.z * s);
}

float vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(Vec3 v)
{
    return sqrtf(vec3_dot(v, v));
}

Vec3 vec3_normalize(Vec3 v)
{
    float len = vec3_length(v);
    return vec3_scale(v, 1.0f / len);
}

/* ---- Ray Struct ---- */

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

/* ---- TODO: Implement these functions ---- */

/*
 * ray_create: Return a Ray with the given origin and direction.
 *
 * The direction should be stored as-is (caller is responsible for
 * normalizing if desired).
 */
Ray ray_create(Vec3 origin, Vec3 direction)
{
    /* TODO: implement */
    Ray r = {{0, 0, 0}, {0, 0, 0}};
    (void)origin; (void)direction;
    return r;
}

/*
 * sphere_intersect: Test if a ray intersects a sphere.
 *
 * Sphere defined by center and radius.
 * Solve the quadratic: at^2 + bt + c = 0, where:
 *   oc = ray.origin - center
 *   a  = dot(ray.direction, ray.direction)
 *   b  = 2 * dot(oc, ray.direction)
 *   c  = dot(oc, oc) - radius * radius
 *   discriminant = b*b - 4*a*c
 *
 * If discriminant < 0: no intersection, return 0.
 * Otherwise: compute t = (-b - sqrt(disc)) / (2*a)
 *   If t > 0: write t into *out_t, return 1 (hit).
 *   If t <= 0: try t = (-b + sqrt(disc)) / (2*a).
 *   If that t > 0: write it and return 1.
 *   Otherwise: return 0 (intersection is behind the ray).
 *
 * Returns: 1 if hit (and *out_t set), 0 if miss.
 */
int sphere_intersect(Ray ray, Vec3 center, float radius, float *out_t)
{
    /* TODO: implement */
    (void)ray; (void)center; (void)radius; (void)out_t;
    return 0;
}

/*
 * shade_diffuse: Compute Lambertian diffuse shading at a surface point.
 *
 * Given:
 *   hit_point — the point on the surface where the ray hit
 *   normal    — the unit surface normal at hit_point
 *   light_pos — position of the light source
 *   color     — the base color of the object (Vec3 with r, g, b in [0,1])
 *
 * Steps:
 *   1. Compute light_dir = normalize(light_pos - hit_point)
 *   2. Compute intensity = max(0, dot(normal, light_dir))
 *   3. Return color scaled by intensity: (color.x * intensity, ...)
 *
 * When the surface faces the light, intensity is positive.
 * When it faces away, intensity is 0 (clamped, no negative light).
 */
Vec3 shade_diffuse(Vec3 hit_point, Vec3 normal, Vec3 light_pos, Vec3 color)
{
    /* TODO: implement */
    (void)hit_point; (void)normal; (void)light_pos; (void)color;
    return vec3_create(0.0f, 0.0f, 0.0f);
}

/* ---- Tests ---- */

int main(void)
{
    printf("=== Ray Tracer Tests ===\n\n");

    /* Test 1: Create ray */
    {
        Ray r = ray_create(vec3_create(0, 0, 0), vec3_create(0, 0, -1));
        TEST("ray_create stores origin and direction")
            EXPECT(APPROX(r.origin.x, 0.0f) && APPROX(r.direction.z, -1.0f));
    }

    /* Test 2: Ray hits sphere */
    {
        Ray r = ray_create(vec3_create(0, 0, 0), vec3_create(0, 0, -1));
        float t;
        int hit = sphere_intersect(r, vec3_create(0, 0, -5), 1.0f, &t);
        TEST("ray hits sphere dead center")
            EXPECT(hit == 1);
    }

    /* Test 3: Intersection distance */
    {
        Ray r = ray_create(vec3_create(0, 0, 0), vec3_create(0, 0, -1));
        float t;
        sphere_intersect(r, vec3_create(0, 0, -5), 1.0f, &t);
        TEST("intersection t is correct (sphere at z=-5, r=1)")
            EXPECT(APPROX(t, 4.0f));
    }

    /* Test 4: Ray misses */
    {
        Ray r = ray_create(vec3_create(0, 0, 0), vec3_create(0, 0, -1));
        float t;
        int hit = sphere_intersect(r, vec3_create(5, 0, -5), 1.0f, &t);
        TEST("ray misses sphere entirely")
            EXPECT(hit == 0);
    }

    /* Test 5: Sphere behind ray */
    {
        Ray r = ray_create(vec3_create(0, 0, 0), vec3_create(0, 0, -1));
        float t;
        int hit = sphere_intersect(r, vec3_create(0, 0, 5), 1.0f, &t);
        TEST("sphere behind ray origin returns miss")
            EXPECT(hit == 0);
    }

    /* Test 6: Large sphere */
    {
        Ray r = ray_create(vec3_create(0, 0, 20), vec3_create(0, 0, -1));
        float t;
        int hit = sphere_intersect(r, vec3_create(0, 0, 0), 10.0f, &t);
        TEST("ray hits large sphere (radius 10)")
            EXPECT(hit == 1 && APPROX(t, 10.0f));
    }

    /* Test 7: Normal at intersection */
    {
        Vec3 normal = vec3_normalize(vec3_sub(vec3_create(0, 0, -4), vec3_create(0, 0, -5)));
        TEST("normal at hit point is correct")
            EXPECT(APPROX(normal.x, 0.0f) && APPROX(normal.y, 0.0f) && APPROX(normal.z, 1.0f));
    }

    /* Test 8: Diffuse — surface facing light */
    {
        Vec3 result = shade_diffuse(
            vec3_create(0, 0, -4), vec3_create(0, 0, 1),
            vec3_create(0, 0, 0), vec3_create(1, 0, 0));
        TEST("diffuse shading when surface faces light")
            EXPECT(APPROX(result.x, 1.0f) && APPROX(result.y, 0.0f) && APPROX(result.z, 0.0f));
    }

    /* Test 9: Diffuse — surface facing away */
    {
        Vec3 result = shade_diffuse(
            vec3_create(0, 0, -4), vec3_create(0, 0, -1),
            vec3_create(0, 0, 0), vec3_create(1, 1, 1));
        TEST("diffuse shading when surface faces away")
            EXPECT(APPROX(result.x, 0.0f) && APPROX(result.y, 0.0f) && APPROX(result.z, 0.0f));
    }

    /* Test 10: Diffuse — angled light */
    {
        Vec3 result = shade_diffuse(
            vec3_create(0, 0, 0), vec3_create(0, 1, 0),
            vec3_create(0, 1, 1), vec3_create(1, 1, 1));
        float expected = 1.0f / sqrtf(2.0f);
        TEST("diffuse shading at 45-degree angle")
            EXPECT(APPROX(result.x, expected) && APPROX(result.y, expected) && APPROX(result.z, expected));
    }

    /* Summary */
    printf("\nResults: %d/%d passed\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("ALL TESTS PASSED\n");
    }
    return tests_passed == tests_total ? 0 : 1;
}
