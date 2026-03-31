/*
 * Solution: Mandelbrot Set Renderer
 *
 * Compute the Mandelbrot set and render it to a PPM image with
 * color mapping based on escape iteration count.
 *
 * 8 tests total.
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
    Image *img = malloc(sizeof(Image));
    if (!img) return NULL;
    img->width = width;
    img->height = height;
    img->pixels = calloc((size_t)width * (size_t)height * 3, 1);
    if (!img->pixels) {
        free(img);
        return NULL;
    }
    return img;
}

/* ── image_destroy ───────────────────────────────────────────────── */
void image_destroy(Image *img)
{
    if (img) {
        free(img->pixels);
        free(img);
    }
}

/* ── image_set_pixel ─────────────────────────────────────────────── */
void image_set_pixel(Image *img, int x, int y,
                     unsigned char r, unsigned char g, unsigned char b)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) return;
    int offset = (y * img->width + x) * 3;
    img->pixels[offset + 0] = r;
    img->pixels[offset + 1] = g;
    img->pixels[offset + 2] = b;
}

/* ── image_get_pixel ─────────────────────────────────────────────── */
int image_get_pixel(const Image *img, int x, int y,
                    unsigned char *r, unsigned char *g, unsigned char *b)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) return -1;
    int offset = (y * img->width + x) * 3;
    *r = img->pixels[offset + 0];
    *g = img->pixels[offset + 1];
    *b = img->pixels[offset + 2];
    return 0;
}

/* ── image_save_ppm ──────────────────────────────────────────────── */
int image_save_ppm(const Image *img, const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);
    size_t pixel_count = (size_t)img->width * (size_t)img->height * 3;
    size_t written = fwrite(img->pixels, 1, pixel_count, f);
    fclose(f);
    return (written == pixel_count) ? 0 : -1;
}

/* ── mandelbrot_iterations ───────────────────────────────────────── */
int mandelbrot_iterations(double cr, double ci, int max_iter)
{
    double a = 0.0, b = 0.0;
    for (int iter = 0; iter < max_iter; iter++) {
        if (a * a + b * b > 4.0) {
            return iter;
        }
        double new_a = a * a - b * b + cr;
        double new_b = 2.0 * a * b + ci;
        a = new_a;
        b = new_b;
    }
    return max_iter;
}

/* ── iteration_to_color ──────────────────────────────────────────── */
Pixel iteration_to_color(int iter, int max_iter)
{
    Pixel p;
    if (iter == max_iter) {
        p.r = 0;
        p.g = 0;
        p.b = 0;
        return p;
    }
    int t = (iter * 255) / max_iter;
    p.r = (unsigned char)t;
    p.g = (unsigned char)(t / 2);
    p.b = (unsigned char)(255 - t);
    return p;
}

/* ── render_mandelbrot ───────────────────────────────────────────── */
void render_mandelbrot(Image *img, double x_min, double x_max,
                       double y_min, double y_max, int max_iter)
{
    for (int py = 0; py < img->height; py++) {
        for (int px = 0; px < img->width; px++) {
            int dw = img->width > 1 ? img->width - 1 : 1;
            int dh = img->height > 1 ? img->height - 1 : 1;
            double cr = x_min + ((double)px / dw) * (x_max - x_min);
            double ci = y_min + ((double)py / dh) * (y_max - y_min);
            int iter = mandelbrot_iterations(cr, ci, max_iter);
            Pixel color = iteration_to_color(iter, max_iter);
            image_set_pixel(img, px, py, color.r, color.g, color.b);
        }
    }
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
        Image *img = image_create(300, 200);
        render_mandelbrot(img, -2.0, 1.0, -1.5, 1.5, max_iter);
        unsigned char r, g, b;
        image_get_pixel(img, 150, 100, &r, &g, &b);
        int ok = (r == 0 && g == 0 && b == 0);
        image_save_ppm(img, "/tmp/mandelbrot_test.ppm");
        image_destroy(img);
    EXPECT(ok);

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
