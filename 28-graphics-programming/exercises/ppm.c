/*
 * Kata: PPM Image — Graphics Primitives
 *
 * Build a small graphics library from scratch:
 *   - Create/destroy images (heap-allocated pixel buffers)
 *   - Set/get individual pixels
 *   - Fill an image with a solid color
 *   - Draw lines using Bresenham's algorithm
 *   - Draw filled rectangles
 *   - Save images as PPM P6 files
 *
 * 14 tests total.
 *
 * Hints:
 *   - Pixels are stored as unsigned char (0–255), 3 bytes per pixel (R, G, B)
 *   - Pixel (x, y) is at byte offset: (y * width + x) * 3
 *   - Always bounds-check before reading/writing pixels
 *   - PPM P6 header: "P6\n<width> <height>\n255\n" followed by raw pixel bytes
 *   - Bresenham's: use dx, dy, sx, sy, and an error accumulator — no floats!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Image structure ──────────────────────────────────────────────── */

typedef struct {
    int width;
    int height;
    unsigned char *pixels;  /* RGB, 3 bytes per pixel */
} Image;

/* ── image_create ─────────────────────────────────────────────────
 * Allocate an image with the given dimensions.
 * All pixels should be initialized to black (0, 0, 0).
 * Returns NULL if allocation fails.
 *
 * Steps:
 *   1. malloc an Image struct
 *   2. Set width and height
 *   3. Allocate the pixel buffer: width * height * 3 bytes
 *      (use calloc to zero-initialize — that gives you black pixels)
 *   4. If any allocation fails, clean up and return NULL
 */
Image *image_create(int width, int height)
{
    /* TODO: Allocate the Image struct and its pixel buffer */
    (void)width; (void)height;
    return NULL;
}

/* ── image_destroy ────────────────────────────────────────────────
 * Free all memory associated with the image.
 * Should handle NULL gracefully.
 */
void image_destroy(Image *img)
{
    /* TODO: Free pixels, then the struct itself */
    (void)img;
}

/* ── image_set_pixel ──────────────────────────────────────────────
 * Set the color of pixel (x, y) to (r, g, b).
 * If (x, y) is out of bounds, do nothing (don't crash!).
 *
 * Formula: offset = (y * width + x) * 3
 */
void image_set_pixel(Image *img, int x, int y,
                     unsigned char r, unsigned char g, unsigned char b)
{
    /* TODO: Bounds check, then write r/g/b into the pixel buffer */
    (void)img; (void)x; (void)y; (void)r; (void)g; (void)b;
}

/* ── image_get_pixel ──────────────────────────────────────────────
 * Read the color of pixel (x, y) into *r, *g, *b.
 * Returns 0 on success, -1 if (x, y) is out of bounds.
 */
int image_get_pixel(const Image *img, int x, int y,
                    unsigned char *r, unsigned char *g, unsigned char *b)
{
    /* TODO: Bounds check, then read r/g/b from the pixel buffer */
    (void)img; (void)x; (void)y; (void)r; (void)g; (void)b;
    return -1;
}

/* ── image_fill ───────────────────────────────────────────────────
 * Fill the entire image with a single color.
 * Loop through every pixel and set it to (r, g, b).
 */
void image_fill(Image *img, unsigned char r, unsigned char g, unsigned char b)
{
    /* TODO: Loop over all pixels (width * height) and set each one */
    (void)img; (void)r; (void)g; (void)b;
}

/* ── image_draw_line ──────────────────────────────────────────────
 * Draw a line from (x0, y0) to (x1, y1) using Bresenham's algorithm.
 * Must handle all octants (horizontal, vertical, diagonal, any slope).
 * Out-of-bounds pixels should be silently skipped (use image_set_pixel).
 *
 * Algorithm outline:
 *   1. Compute dx = abs(x1 - x0), dy = -abs(y1 - y0)
 *   2. Compute sx = (x1 > x0) ? 1 : -1,  sy = (y1 > y0) ? 1 : -1
 *   3. Set err = dx + dy
 *   4. Loop:
 *      a. Plot current pixel
 *      b. If at endpoint, break
 *      c. e2 = 2 * err
 *      d. If e2 >= dy: err += dy, x0 += sx
 *      e. If e2 <= dx: err += dx, y0 += sy
 */
void image_draw_line(Image *img, int x0, int y0, int x1, int y1,
                     unsigned char r, unsigned char g, unsigned char b)
{
    /* TODO: Implement Bresenham's line algorithm */
    (void)img; (void)x0; (void)y0; (void)x1; (void)y1;
    (void)r; (void)g; (void)b;
}

/* ── image_draw_rect ──────────────────────────────────────────────
 * Draw a filled rectangle with top-left corner at (x, y), size w x h.
 * Clip to image bounds — don't write outside the pixel buffer.
 *
 * Steps:
 *   1. Clamp the rectangle to [0, width) x [0, height)
 *   2. Nested loop: for each row, for each column, set the pixel
 */
void image_draw_rect(Image *img, int x, int y, int w, int h,
                     unsigned char r, unsigned char g, unsigned char b)
{
    /* TODO: Clip bounds and fill the rectangular region */
    (void)img; (void)x; (void)y; (void)w; (void)h;
    (void)r; (void)g; (void)b;
}

/* ── image_save_ppm ───────────────────────────────────────────────
 * Save the image as a PPM P6 file.
 * Returns 0 on success, -1 on failure.
 *
 * PPM P6 format:
 *   1. Open file in binary write mode ("wb")
 *   2. Write header: "P6\n<width> <height>\n255\n"  (use fprintf)
 *   3. Write pixel data: fwrite(pixels, 1, width*height*3, f)
 *   4. Close file
 */
int image_save_ppm(const Image *img, const char *filename)
{
    /* TODO: Write the PPM P6 header and pixel data to a file */
    (void)img; (void)filename;
    return -1;
}

/* ══════════════════════════════════════════════════════════════════
 * Tests — 14 total
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

/* Helper: check if pixel (x,y) matches given color */
static int pixel_eq(const Image *img, int x, int y,
                    unsigned char er, unsigned char eg, unsigned char eb)
{
    unsigned char r, g, b;
    if (image_get_pixel(img, x, y, &r, &g, &b) != 0) return 0;
    return (r == er && g == eg && b == eb);
}

int main(void)
{
    printf("=== PPM Image — Graphics Primitives ===\n\n");

    /* --- image_create / image_destroy --- */
    printf("image_create / image_destroy:\n");

    TEST("create returns non-NULL for valid dimensions")
        Image *img1 = image_create(10, 10);
        int ok1 = (img1 != NULL && img1->width == 10 && img1->height == 10);
        image_destroy(img1);
    EXPECT(ok1);

    TEST("newly created image pixels are all zero (black)")
        Image *img2 = image_create(4, 4);
        int all_zero = 1;
        for (int i = 0; i < 4 * 4 * 3; i++) {
            if (img2->pixels[i] != 0) { all_zero = 0; break; }
        }
        image_destroy(img2);
    EXPECT(all_zero);

    /* --- image_set_pixel / image_get_pixel --- */
    printf("\nimage_set_pixel / image_get_pixel:\n");

    TEST("set and get a pixel at (2, 3)")
        Image *img3 = image_create(10, 10);
        image_set_pixel(img3, 2, 3, 255, 128, 64);
        unsigned char r3, g3, b3;
        image_get_pixel(img3, 2, 3, &r3, &g3, &b3);
        int ok3 = (r3 == 255 && g3 == 128 && b3 == 64);
        image_destroy(img3);
    EXPECT(ok3);

    TEST("set pixel at origin (0, 0)")
        Image *img4 = image_create(5, 5);
        image_set_pixel(img4, 0, 0, 10, 20, 30);
        unsigned char r4, g4, b4;
        image_get_pixel(img4, 0, 0, &r4, &g4, &b4);
        int ok4 = (r4 == 10 && g4 == 20 && b4 == 30);
        image_destroy(img4);
    EXPECT(ok4);

    TEST("out-of-bounds set_pixel is silently ignored")
        Image *img5 = image_create(5, 5);
        image_set_pixel(img5, -1, 0, 255, 0, 0);
        image_set_pixel(img5, 5, 0, 255, 0, 0);
        image_set_pixel(img5, 0, -1, 255, 0, 0);
        image_set_pixel(img5, 0, 5, 255, 0, 0);
        int all_zero5 = 1;
        for (int i = 0; i < 5 * 5 * 3; i++) {
            if (img5->pixels[i] != 0) { all_zero5 = 0; break; }
        }
        image_destroy(img5);
    EXPECT(all_zero5);

    TEST("out-of-bounds get_pixel returns -1")
        Image *img6 = image_create(5, 5);
        unsigned char r6, g6, b6;
        int ok6 = (image_get_pixel(img6, -1, 0, &r6, &g6, &b6) == -1 &&
                   image_get_pixel(img6, 5, 0, &r6, &g6, &b6) == -1 &&
                   image_get_pixel(img6, 0, 5, &r6, &g6, &b6) == -1);
        image_destroy(img6);
    EXPECT(ok6);

    /* --- image_fill --- */
    printf("\nimage_fill:\n");

    TEST("fill sets every pixel to the given color")
        Image *img7 = image_create(3, 3);
        image_fill(img7, 100, 150, 200);
        int ok7 = 1;
        for (int i = 0; i < 3 * 3; i++) {
            if (img7->pixels[i * 3 + 0] != 100 ||
                img7->pixels[i * 3 + 1] != 150 ||
                img7->pixels[i * 3 + 2] != 200) {
                ok7 = 0; break;
            }
        }
        image_destroy(img7);
    EXPECT(ok7);

    /* --- image_draw_line (Bresenham) --- */
    printf("\nimage_draw_line:\n");

    TEST("horizontal line from (1,2) to (5,2)")
        Image *img8 = image_create(10, 10);
        image_draw_line(img8, 1, 2, 5, 2, 255, 0, 0);
        int ok8 = 1;
        for (int x = 1; x <= 5; x++) {
            if (!pixel_eq(img8, x, 2, 255, 0, 0)) { ok8 = 0; break; }
        }
        if (!pixel_eq(img8, 0, 2, 0, 0, 0)) ok8 = 0;
        if (!pixel_eq(img8, 6, 2, 0, 0, 0)) ok8 = 0;
        image_destroy(img8);
    EXPECT(ok8);

    TEST("vertical line from (3,0) to (3,4)")
        Image *img9 = image_create(10, 10);
        image_draw_line(img9, 3, 0, 3, 4, 0, 255, 0);
        int ok9 = 1;
        for (int y = 0; y <= 4; y++) {
            if (!pixel_eq(img9, 3, y, 0, 255, 0)) { ok9 = 0; break; }
        }
        if (!pixel_eq(img9, 3, 5, 0, 0, 0)) ok9 = 0;
        image_destroy(img9);
    EXPECT(ok9);

    TEST("diagonal line from (0,0) to (4,4)")
        Image *img10 = image_create(10, 10);
        image_draw_line(img10, 0, 0, 4, 4, 0, 0, 255);
        int ok10 = 1;
        for (int i = 0; i <= 4; i++) {
            if (!pixel_eq(img10, i, i, 0, 0, 255)) { ok10 = 0; break; }
        }
        image_destroy(img10);
    EXPECT(ok10);

    TEST("reverse diagonal line from (4,4) to (0,0)")
        Image *img11 = image_create(10, 10);
        image_draw_line(img11, 4, 4, 0, 0, 128, 128, 128);
        int ok11 = 1;
        for (int i = 0; i <= 4; i++) {
            if (!pixel_eq(img11, i, i, 128, 128, 128)) { ok11 = 0; break; }
        }
        image_destroy(img11);
    EXPECT(ok11);

    /* --- image_draw_rect --- */
    printf("\nimage_draw_rect:\n");

    TEST("filled rectangle at (1,1) size 3x2")
        Image *img12 = image_create(10, 10);
        image_draw_rect(img12, 1, 1, 3, 2, 255, 200, 100);
        int ok12 = 1;
        for (int y = 1; y < 3; y++) {
            for (int x = 1; x < 4; x++) {
                if (!pixel_eq(img12, x, y, 255, 200, 100)) ok12 = 0;
            }
        }
        if (!pixel_eq(img12, 0, 0, 0, 0, 0)) ok12 = 0;
        if (!pixel_eq(img12, 4, 1, 0, 0, 0)) ok12 = 0;
        image_destroy(img12);
    EXPECT(ok12);

    /* --- image_save_ppm --- */
    printf("\nimage_save_ppm:\n");

    TEST("save PPM produces valid P6 header and pixel data")
        Image *img13 = image_create(2, 2);
        image_set_pixel(img13, 0, 0, 255, 0, 0);
        image_set_pixel(img13, 1, 0, 0, 255, 0);
        image_set_pixel(img13, 0, 1, 0, 0, 255);
        image_set_pixel(img13, 1, 1, 255, 255, 255);
        const char *tmp13 = "/tmp/test_ppm_output.ppm";
        int save_ok13 = image_save_ppm(img13, tmp13);
        int ok13 = (save_ok13 == 0);
        if (ok13) {
            FILE *f13 = fopen(tmp13, "rb");
            ok13 = (f13 != NULL);
            if (ok13) {
                char hdr[64];
                int hlen = 0;
                int nl = 0;
                while (nl < 3 && hlen < 63) {
                    int ch = fgetc(f13);
                    if (ch == EOF) { ok13 = 0; break; }
                    hdr[hlen++] = (char)ch;
                    if (ch == '\n') nl++;
                }
                hdr[hlen] = '\0';
                if (ok13) ok13 = (strncmp(hdr, "P6\n", 3) == 0);
                if (ok13) {
                    unsigned char px[3];
                    size_t n = fread(px, 1, 3, f13);
                    ok13 = (n == 3 && px[0] == 255 && px[1] == 0 && px[2] == 0);
                }
                fclose(f13);
            }
        }
        remove(tmp13);
        image_destroy(img13);
    EXPECT(ok13);

    TEST("save PPM file has correct total size")
        Image *img14 = image_create(3, 2);
        image_fill(img14, 128, 64, 32);
        const char *tmp14 = "/tmp/test_ppm_size.ppm";
        image_save_ppm(img14, tmp14);
        FILE *f14 = fopen(tmp14, "rb");
        int ok14 = (f14 != NULL);
        if (ok14) {
            fseek(f14, 0, SEEK_END);
            long file_size = ftell(f14);
            fclose(f14);
            /* Header: "P6\n3 2\n255\n" = 11 bytes, Pixels: 3*2*3 = 18 */
            ok14 = (file_size == 11 + 3 * 2 * 3);
        }
        remove(tmp14);
        image_destroy(img14);
    EXPECT(ok14);

    /* --- Summary --- */
    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
