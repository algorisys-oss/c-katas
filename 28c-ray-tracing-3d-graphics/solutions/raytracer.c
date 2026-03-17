/*
 * Solution: Basic Ray Tracer — Ray-Sphere Intersection & Shading
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

/* ---- Vec3 ---- */

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

/* ---- Ray ---- */

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

Ray ray_create(Vec3 origin, Vec3 direction)
{
    Ray r;
    r.origin = origin;
    r.direction = direction;
    return r;
}

/*
 * sphere_intersect: Solve the quadratic at^2 + bt + c = 0
 *
 *   oc = ray.origin - center
 *   a  = dot(D, D)
 *   b  = 2 * dot(oc, D)
 *   c  = dot(oc, oc) - r^2
 *   disc = b^2 - 4ac
 *
 * Returns 1 if hit (t > 0), 0 if miss.
 */
int sphere_intersect(Ray ray, Vec3 center, float radius, float *out_t)
{
    Vec3 oc = vec3_sub(ray.origin, center);
    float a = vec3_dot(ray.direction, ray.direction);
    float b = 2.0f * vec3_dot(oc, ray.direction);
    float c = vec3_dot(oc, oc) - radius * radius;
    float disc = b * b - 4.0f * a * c;

    if (disc < 0.0f) {
        return 0;
    }

    float t = (-b - sqrtf(disc)) / (2.0f * a);
    if (t > 0.0f) {
        *out_t = t;
        return 1;
    }

    /* Try the far intersection (in case we're inside the sphere) */
    t = (-b + sqrtf(disc)) / (2.0f * a);
    if (t > 0.0f) {
        *out_t = t;
        return 1;
    }

    return 0;
}

/*
 * shade_diffuse: Lambertian diffuse shading.
 *   intensity = max(0, dot(normal, light_dir))
 *   result = color * intensity
 */
Vec3 shade_diffuse(Vec3 hit_point, Vec3 normal, Vec3 light_pos, Vec3 color)
{
    Vec3 light_dir = vec3_normalize(vec3_sub(light_pos, hit_point));
    float intensity = vec3_dot(normal, light_dir);
    if (intensity < 0.0f) {
        intensity = 0.0f;
    }
    return vec3_scale(color, intensity);
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

    printf("\nResults: %d/%d passed\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("ALL TESTS PASSED\n");
    }
    return tests_passed == tests_total ? 0 : 1;
}
