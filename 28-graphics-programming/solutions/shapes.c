/*
 * Solution: Drawing Shapes to a PPM Image
 *
 * Circles (midpoint algorithm), triangles (3 lines), and gradients.
 *
 * 10 tests total.
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

/* ── image_draw_line (Bresenham) ─────────────────────────────────── */
void image_draw_line(Image *img, int x0, int y0, int x1, int y1,
                     unsigned char r, unsigned char g, unsigned char b)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int sx = (dx > 0) ? 1 : -1;
    int sy = (dy > 0) ? 1 : -1;
    dx = abs(dx);
    dy = -abs(dy);
    int err = dx + dy;

    for (;;) {
        image_set_pixel(img, x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
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

/* ── draw_circle (midpoint circle algorithm) ─────────────────────── */
void draw_circle(Image *img, int cx, int cy, int radius, Pixel color)
{
    int x = radius;
    int y = 0;
    int d = 1 - radius;

    while (x >= y) {
        image_set_pixel(img, cx + x, cy + y, color.r, color.g, color.b);
        image_set_pixel(img, cx - x, cy + y, color.r, color.g, color.b);
        image_set_pixel(img, cx + x, cy - y, color.r, color.g, color.b);
        image_set_pixel(img, cx - x, cy - y, color.r, color.g, color.b);
        image_set_pixel(img, cx + y, cy + x, color.r, color.g, color.b);
        image_set_pixel(img, cx - y, cy + x, color.r, color.g, color.b);
        image_set_pixel(img, cx + y, cy - x, color.r, color.g, color.b);
        image_set_pixel(img, cx - y, cy - x, color.r, color.g, color.b);

        y++;
        if (d <= 0) {
            d = d + 2 * y + 1;
        } else {
            x--;
            d = d + 2 * (y - x) + 1;
        }
    }
}

/* ── fill_circle ─────────────────────────────────────────────────── */
void fill_circle(Image *img, int cx, int cy, int radius, Pixel color)
{
    int r2 = radius * radius;
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= r2) {
                image_set_pixel(img, cx + dx, cy + dy,
                                color.r, color.g, color.b);
            }
        }
    }
}

/* ── draw_triangle ───────────────────────────────────────────────── */
void draw_triangle(Image *img, int x0, int y0, int x1, int y1,
                   int x2, int y2, Pixel color)
{
    image_draw_line(img, x0, y0, x1, y1, color.r, color.g, color.b);
    image_draw_line(img, x1, y1, x2, y2, color.r, color.g, color.b);
    image_draw_line(img, x2, y2, x0, y0, color.r, color.g, color.b);
}

/* ── draw_gradient ───────────────────────────────────────────────── */
void draw_gradient(Image *img)
{
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            unsigned char r = (unsigned char)((x * 255) / (img->width - 1));
            unsigned char g = (unsigned char)((y * 255) / (img->height - 1));
            image_set_pixel(img, x, y, r, g, 0);
        }
    }
}

/* ══════════════════════════════════════════════════════════════════
 * Tests — 10 total
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

static int pixel_eq(const Image *img, int x, int y,
                    unsigned char er, unsigned char eg, unsigned char eb)
{
    unsigned char r, g, b;
    if (image_get_pixel(img, x, y, &r, &g, &b) != 0) return 0;
    return (r == er && g == eg && b == eb);
}

int main(void)
{
    printf("=== Drawing Shapes ===\n\n");

    /* --- draw_circle tests --- */
    printf("draw_circle (midpoint algorithm):\n");

    TEST("circle outline — top of circle is drawn")
        Image *img1 = image_create(50, 50);
        Pixel red = {255, 0, 0};
        draw_circle(img1, 25, 25, 10, red);
        int ok1 = pixel_eq(img1, 25, 15, 255, 0, 0);
        image_destroy(img1);
    EXPECT(ok1);

    TEST("circle outline — bottom of circle is drawn")
        Image *img2 = image_create(50, 50);
        Pixel blue = {0, 0, 255};
        draw_circle(img2, 25, 25, 10, blue);
        int ok2 = pixel_eq(img2, 25, 35, 0, 0, 255);
        image_destroy(img2);
    EXPECT(ok2);

    TEST("circle outline — center is NOT drawn")
        Image *img3 = image_create(50, 50);
        Pixel green = {0, 255, 0};
        draw_circle(img3, 25, 25, 10, green);
        int ok3 = pixel_eq(img3, 25, 25, 0, 0, 0);
        image_destroy(img3);
    EXPECT(ok3);

    /* --- fill_circle tests --- */
    printf("\nfill_circle:\n");

    TEST("filled circle — center is filled")
        Image *img4 = image_create(50, 50);
        Pixel yellow = {255, 255, 0};
        fill_circle(img4, 25, 25, 10, yellow);
        int ok4 = pixel_eq(img4, 25, 25, 255, 255, 0);
        image_destroy(img4);
    EXPECT(ok4);

    TEST("filled circle — point inside is filled")
        Image *img5 = image_create(50, 50);
        Pixel cyan = {0, 255, 255};
        fill_circle(img5, 25, 25, 10, cyan);
        int ok5 = pixel_eq(img5, 30, 25, 0, 255, 255);
        image_destroy(img5);
    EXPECT(ok5);

    TEST("filled circle — point outside is black")
        Image *img6 = image_create(50, 50);
        Pixel magenta = {255, 0, 255};
        fill_circle(img6, 25, 25, 5, magenta);
        int ok6 = pixel_eq(img6, 0, 0, 0, 0, 0);
        image_destroy(img6);
    EXPECT(ok6);

    /* --- draw_triangle tests --- */
    printf("\ndraw_triangle:\n");

    TEST("triangle — vertex pixels are drawn")
        Image *img7 = image_create(50, 50);
        Pixel white = {255, 255, 255};
        draw_triangle(img7, 25, 5, 5, 45, 45, 45, white);
        int ok7 = pixel_eq(img7, 25, 5, 255, 255, 255) &&
                  pixel_eq(img7, 5, 45, 255, 255, 255) &&
                  pixel_eq(img7, 45, 45, 255, 255, 255);
        image_destroy(img7);
    EXPECT(ok7);

    TEST("triangle — center is NOT drawn (outline only)")
        Image *img8 = image_create(50, 50);
        Pixel orange = {255, 165, 0};
        draw_triangle(img8, 10, 5, 5, 40, 40, 40, orange);
        int ok8 = pixel_eq(img8, 20, 25, 0, 0, 0);
        image_destroy(img8);
    EXPECT(ok8);

    /* --- draw_gradient tests --- */
    printf("\ndraw_gradient:\n");

    TEST("gradient — top-left is black (0,0,0)")
        Image *img9 = image_create(100, 100);
        draw_gradient(img9);
        int ok9 = pixel_eq(img9, 0, 0, 0, 0, 0);
        image_destroy(img9);
    EXPECT(ok9);

    TEST("gradient — bottom-right is (255,255,0)")
        Image *img10 = image_create(100, 100);
        draw_gradient(img10);
        int ok10 = pixel_eq(img10, 99, 99, 255, 255, 0);
        image_destroy(img10);
    EXPECT(ok10);

    /* --- Save test image with all shapes --- */
    {
        Image *demo = image_create(200, 200);
        draw_gradient(demo);
        Pixel white = {255, 255, 255};
        Pixel red = {255, 0, 0};
        Pixel blue = {0, 0, 255};
        draw_circle(demo, 100, 100, 60, white);
        fill_circle(demo, 50, 50, 20, red);
        draw_triangle(demo, 100, 20, 150, 80, 50, 80, blue);
        image_save_ppm(demo, "/tmp/shapes_test.ppm");
        image_destroy(demo);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
