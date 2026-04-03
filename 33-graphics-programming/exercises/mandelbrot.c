/*
 * Kata: Mandelbrot Set Renderer
 *
 * Render the Mandelbrot set to a PPM image. The Mandelbrot set is
 * the set of complex numbers c for which the iteration
 *   z(n+1) = z(n)^2 + c    (starting from z(0) = 0)
 * does NOT diverge (|z| stays bounded).
 *
 * Functions to implement:
 *   - image_create / image_destroy / image_set_pixel / image_get_pixel
 *     (same as ppm.c — provided here for self-containment)
 *   - image_save_ppm        (save to file)
 *   - mandelbrot_iterations (compute escape iteration count)
 *   - iteration_to_color    (map iteration count to RGB color)
 *   - render_mandelbrot     (render the full set to an image)
 *
 * 8 tests total.
 *
 * Hints:
 *   - z = a + bi, c = cr + ci*i
 *   - z^2 = (a^2 - b^2) + 2ab*i
 *   - |z|^2 = a^2 + b^2  (escape when > 4.0, i.e. |z| > 2)
 *   - Points inside the Mandelbrot set: (0,0), (-1,0)
 *   - Points outside: (2,0) escapes in 1 iteration, (1,0) escapes quickly
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Pixel and Image structures ──────────────────────────────────── */

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int width;
    int height;
    unsigned char *pixels;
} Image;

/* ── image_create ────────────────────────────────────────────────── */
Image *image_create(int width, int height)
{
    /* TODO: allocate Image and pixel buffer */
    (void)width; (void)height;
    return NULL;
}

/* ── image_destroy ───────────────────────────────────────────────── */
void image_destroy(Image *img)
{
    /* TODO: free pixels, then struct */
    (void)img;
}

/* ── image_set_pixel ─────────────────────────────────────────────── */
void image_set_pixel(Image *img, int x, int y,
                     unsigned char r, unsigned char g, unsigned char b)
{
    /* TODO: bounds check, write RGB */
    (void)img; (void)x; (void)y; (void)r; (void)g; (void)b;
}

/* ── image_get_pixel ─────────────────────────────────────────────── */
int image_get_pixel(const Image *img, int x, int y,
                    unsigned char *r, unsigned char *g, unsigned char *b)
{
    /* TODO: bounds check, read RGB */
    (void)img; (void)x; (void)y; (void)r; (void)g; (void)b;
    return -1;
}

/* ── image_save_ppm ──────────────────────────────────────────────── */
int image_save_ppm(const Image *img, const char *filename)
{
    /* TODO: write PPM P6 header + pixel data */
    (void)img; (void)filename;
    return -1;
}

/* ── Exercise 1: mandelbrot_iterations ───────────────────────────
 * Compute how many iterations the point c = (cr, ci) takes to
 * escape the Mandelbrot set (|z| > 2), up to max_iter.
 *
 * Algorithm:
 *   a = 0, b = 0   (z starts at 0)
 *   for iter = 0 to max_iter - 1:
 *       if a*a + b*b > 4.0: return iter   (escaped!)
 *       new_a = a*a - b*b + cr
 *       new_b = 2*a*b + ci
 *       a = new_a, b = new_b
 *   return max_iter   (didn't escape — point is in the set)
 *
 * Known values:
 *   c = (0, 0)    → max_iter (in the set, z stays at 0)
 *   c = (2, 0)    → 2 (z1 = 2, |z|^2 = 4 not > 4; z2 = 6, |z|^2 = 36 > 4)
 *   c = (-1, 0)   → max_iter (in the set, z oscillates: 0, -1, 0, -1, ...)
 *   c = (0.25, 0) → max_iter (right at the boundary, very slow escape)
 *   c = (1, 0)    → 3 (z: 0 → 1 → 2 → 5, |5| > 2)
 */
int mandelbrot_iterations(double cr, double ci, int max_iter)
{
    /* TODO: iterate z = z^2 + c until escape or max_iter */
    (void)cr; (void)ci; (void)max_iter;
    return 0;
}

/* ── Exercise 2: iteration_to_color ──────────────────────────────
 * Map an iteration count to an RGB color.
 *
 * Rules:
 *   - If iter == max_iter, return black (0, 0, 0) — point is in the set
 *   - Otherwise, map iter to a color gradient.
 *     A simple scheme:
 *       t = (iter * 255) / max_iter
 *       r = t
 *       g = t / 2
 *       b = 255 - t
 *     This gives a gradient from blue (fast escape) to red (slow escape).
 */
Pixel iteration_to_color(int iter, int max_iter)
{
    /* TODO: map iteration count to color */
    Pixel p = {0, 0, 0};
    (void)iter; (void)max_iter;
    return p;
}

/* ── Exercise 3: render_mandelbrot ───────────────────────────────
 * Render the Mandelbrot set to an image.
 *
 * For each pixel (px, py) in the image:
 *   1. Map pixel coordinates to complex plane:
 *      cr = x_min + (px / (width - 1.0)) * (x_max - x_min)
 *      ci = y_min + (py / (height - 1.0)) * (y_max - y_min)
 *   2. Compute iter = mandelbrot_iterations(cr, ci, max_iter)
 *   3. Convert to color: iteration_to_color(iter, max_iter)
 *   4. Set the pixel
 *
 * Default view: x_min=-2.0, x_max=1.0, y_min=-1.5, y_max=1.5
 */
void render_mandelbrot(Image *img, double x_min, double x_max,
                       double y_min, double y_max, int max_iter)
{
    /* TODO: render the Mandelbrot set */
    (void)img; (void)x_min; (void)x_max; (void)y_min; (void)y_max;
    (void)max_iter;
}

/* ══════════════════════════════════════════════════════════════════
 * Tests — 8 total
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Mandelbrot Set Renderer ===\n\n");

    int max_iter = 100;

    /* --- mandelbrot_iterations tests --- */
    printf("mandelbrot_iterations:\n");

    TEST("(0, 0) is in the set — returns max_iter")
        int i1 = mandelbrot_iterations(0.0, 0.0, max_iter);
    EXPECT(i1 == max_iter);

    TEST("(2, 0) escapes in 2 iterations")
        /* z: 0 -> 2 -> 6 (|6|^2 = 36 > 4) */
        int i2 = mandelbrot_iterations(2.0, 0.0, max_iter);
    EXPECT(i2 == 2);

    TEST("(-1, 0) is in the set — returns max_iter")
        int i3 = mandelbrot_iterations(-1.0, 0.0, max_iter);
    EXPECT(i3 == max_iter);

    TEST("(1, 0) escapes in 3 iterations")
        /* z: 0 -> 1 -> 2 -> 5 (|5| > 2) */
        int i4 = mandelbrot_iterations(1.0, 0.0, max_iter);
    EXPECT(i4 == 3);

    TEST("(0.5, 0.5) escapes (not in the set)")
        int i5 = mandelbrot_iterations(0.5, 0.5, max_iter);
    EXPECT(i5 < max_iter);

    /* --- iteration_to_color tests --- */
    printf("\niteration_to_color:\n");

    TEST("max_iter maps to black (in the set)")
        Pixel p1 = iteration_to_color(max_iter, max_iter);
    EXPECT(p1.r == 0 && p1.g == 0 && p1.b == 0);

    TEST("iter < max_iter maps to non-black")
        Pixel p2 = iteration_to_color(50, max_iter);
    EXPECT(p2.r != 0 || p2.g != 0 || p2.b != 0);

    /* --- render_mandelbrot test --- */
    printf("\nrender_mandelbrot:\n");

    TEST("rendered image — center pixel is black (in the set)")
        /* Center of default view (-2..1, -1.5..1.5) is (-0.5, 0)
         * which is inside the Mandelbrot set */
        Image *img = image_create(300, 200);
        render_mandelbrot(img, -2.0, 1.0, -1.5, 1.5, max_iter);
        /* Pixel at center of image maps to roughly (-0.5, 0) */
        unsigned char r, g, b;
        image_get_pixel(img, 150, 100, &r, &g, &b);
        int ok = (r == 0 && g == 0 && b == 0);
        /* Also save a test image */
        image_save_ppm(img, "/tmp/mandelbrot_test.ppm");
        image_destroy(img);
    EXPECT(ok);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
