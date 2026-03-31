/*
 * Kata: Drawing Shapes to a PPM Image
 *
 * Build on the PPM image library to draw circles, triangles, and
 * gradients using fundamental graphics algorithms.
 *
 * Functions to implement:
 *   - image_create / image_destroy / image_set_pixel / image_get_pixel
 *     (same as ppm.c — provided here for self-containment)
 *   - image_draw_line     (Bresenham — needed for triangles)
 *   - image_save_ppm      (save to file)
 *   - draw_circle         (midpoint circle algorithm — outline)
 *   - fill_circle         (filled circle)
 *   - draw_triangle       (outline using 3 lines)
 *   - draw_gradient       (R increases left→right, G increases top→bottom)
 *
 * 10 tests total.
 *
 * Hints:
 *   - Midpoint circle algorithm: similar to Bresenham but for circles
 *     Start at (r, 0), step around the first octant, mirror to all 8
 *   - fill_circle: for each row in [cy-r, cy+r], compute the x-range
 *     and fill the horizontal span
 *   - draw_triangle: just draw 3 lines connecting the vertices
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
    unsigned char *pixels;  /* RGB, 3 bytes per pixel */
} Image;

/* ── image_create ────────────────────────────────────────────────── */
Image *image_create(int width, int height)
{
    /* TODO: allocate Image and pixel buffer (calloc for black init) */
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
    /* TODO: bounds check, then write RGB */
    (void)img; (void)x; (void)y; (void)r; (void)g; (void)b;
}

/* ── image_get_pixel ─────────────────────────────────────────────── */
int image_get_pixel(const Image *img, int x, int y,
                    unsigned char *r, unsigned char *g, unsigned char *b)
{
    /* TODO: bounds check, read RGB, return 0 on success, -1 on OOB */
    (void)img; (void)x; (void)y; (void)r; (void)g; (void)b;
    return -1;
}

/* ── image_draw_line (Bresenham) ─────────────────────────────────── */
void image_draw_line(Image *img, int x0, int y0, int x1, int y1,
                     unsigned char r, unsigned char g, unsigned char b)
{
    /* TODO: Bresenham's line algorithm (all octants) */
    (void)img; (void)x0; (void)y0; (void)x1; (void)y1;
    (void)r; (void)g; (void)b;
}

/* ── image_save_ppm ──────────────────────────────────────────────── */
int image_save_ppm(const Image *img, const char *filename)
{
    /* TODO: write PPM P6 header + pixel data */
    (void)img; (void)filename;
    return -1;
}

/* ── Exercise 1: draw_circle ─────────────────────────────────────
 * Draw a circle outline using the midpoint circle algorithm.
 *
 * The midpoint circle algorithm traces one octant and mirrors to
 * all eight octants. Starting at (radius, 0), it decides whether
 * the next pixel is (x, y+1) or (x-1, y+1).
 *
 * Algorithm:
 *   x = radius, y = 0
 *   d = 1 - radius   (decision variable)
 *   while x >= y:
 *       plot all 8 symmetric points:
 *           (cx+x, cy+y), (cx-x, cy+y), (cx+x, cy-y), (cx-x, cy-y)
 *           (cx+y, cy+x), (cx-y, cy+x), (cx+y, cy-x), (cx-y, cy-x)
 *       y++
 *       if d <= 0:
 *           d = d + 2*y + 1
 *       else:
 *           x--
 *           d = d + 2*(y - x) + 1
 */
void draw_circle(Image *img, int cx, int cy, int radius, Pixel color)
{
    /* TODO: midpoint circle algorithm */
    (void)img; (void)cx; (void)cy; (void)radius; (void)color;
}

/* ── Exercise 2: fill_circle ─────────────────────────────────────
 * Draw a filled circle.
 *
 * For each row from cy-radius to cy+radius:
 *   Compute how far the circle extends horizontally at that row:
 *     dx = sqrt(radius^2 - dy^2)   where dy = row - cy
 *   Fill pixels from (cx - dx) to (cx + dx).
 *
 * To avoid floating point, compare dx*dx: fill while dx*dx + dy*dy <= r*r
 */
void fill_circle(Image *img, int cx, int cy, int radius, Pixel color)
{
    /* TODO: fill circle row by row */
    (void)img; (void)cx; (void)cy; (void)radius; (void)color;
}

/* ── Exercise 3: draw_triangle ───────────────────────────────────
 * Draw a triangle outline by connecting three vertices with lines.
 *
 * Just call image_draw_line three times:
 *   (x0,y0) -> (x1,y1)
 *   (x1,y1) -> (x2,y2)
 *   (x2,y2) -> (x0,y0)
 */
void draw_triangle(Image *img, int x0, int y0, int x1, int y1,
                   int x2, int y2, Pixel color)
{
    /* TODO: draw 3 lines */
    (void)img; (void)x0; (void)y0; (void)x1; (void)y1; (void)x2; (void)y2;
    (void)color;
}

/* ── Exercise 4: draw_gradient ───────────────────────────────────
 * Fill the image with a gradient:
 *   Red channel   increases from 0 (left) to 255 (right)
 *   Green channel increases from 0 (top)  to 255 (bottom)
 *   Blue channel  is 0 everywhere
 *
 * Formula for pixel (x, y):
 *   r = (x * 255) / (width - 1)
 *   g = (y * 255) / (height - 1)
 *   b = 0
 */
void draw_gradient(Image *img)
{
    /* TODO: fill with R/G gradient */
    (void)img;
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
        /* Top of circle: (25, 15) should be red */
        int ok1 = pixel_eq(img1, 25, 15, 255, 0, 0);
        image_destroy(img1);
    EXPECT(ok1);

    TEST("circle outline — bottom of circle is drawn")
        Image *img2 = image_create(50, 50);
        Pixel blue = {0, 0, 255};
        draw_circle(img2, 25, 25, 10, blue);
        /* Bottom of circle: (25, 35) should be blue */
        int ok2 = pixel_eq(img2, 25, 35, 0, 0, 255);
        image_destroy(img2);
    EXPECT(ok2);

    TEST("circle outline — center is NOT drawn")
        Image *img3 = image_create(50, 50);
        Pixel green = {0, 255, 0};
        draw_circle(img3, 25, 25, 10, green);
        /* Center should remain black */
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
        /* (30, 25) is 5 pixels from center, inside radius 10 */
        int ok5 = pixel_eq(img5, 30, 25, 0, 255, 255);
        image_destroy(img5);
    EXPECT(ok5);

    TEST("filled circle — point outside is black")
        Image *img6 = image_create(50, 50);
        Pixel magenta = {255, 0, 255};
        fill_circle(img6, 25, 25, 5, magenta);
        /* (0, 0) is far outside radius 5 */
        int ok6 = pixel_eq(img6, 0, 0, 0, 0, 0);
        image_destroy(img6);
    EXPECT(ok6);

    /* --- draw_triangle tests --- */
    printf("\ndraw_triangle:\n");

    TEST("triangle — vertex pixels are drawn")
        Image *img7 = image_create(50, 50);
        Pixel white = {255, 255, 255};
        draw_triangle(img7, 25, 5, 5, 45, 45, 45, white);
        /* All 3 vertices should be white */
        int ok7 = pixel_eq(img7, 25, 5, 255, 255, 255) &&
                  pixel_eq(img7, 5, 45, 255, 255, 255) &&
                  pixel_eq(img7, 45, 45, 255, 255, 255);
        image_destroy(img7);
    EXPECT(ok7);

    TEST("triangle — center is NOT drawn (outline only)")
        Image *img8 = image_create(50, 50);
        Pixel orange = {255, 165, 0};
        draw_triangle(img8, 10, 5, 5, 40, 40, 40, orange);
        /* Center area should be black (outline only) */
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
