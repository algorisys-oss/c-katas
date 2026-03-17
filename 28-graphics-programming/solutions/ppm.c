/*
 * Solution: PPM Image — Graphics Primitives
 *
 * Create images from scratch, draw pixels, lines, and rectangles,
 * and save to the PPM P6 format.
 *
 * 14 tests total.
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
 * All pixels are initialized to black (0, 0, 0).
 * Returns NULL if allocation fails.
 */
Image *image_create(int width, int height)
{
    Image *img = malloc(sizeof(Image));
    if (!img) {
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->pixels = calloc((size_t)width * (size_t)height * 3, 1);
    if (!img->pixels) {
        free(img);
        return NULL;
    }
    return img;
}

/* ── image_destroy ────────────────────────────────────────────────
 * Free all memory associated with the image.
 */
void image_destroy(Image *img)
{
    if (img) {
        free(img->pixels);
        free(img);
    }
}

/* ── image_set_pixel ──────────────────────────────────────────────
 * Set the color of pixel (x, y). Silently ignores out-of-bounds.
 */
void image_set_pixel(Image *img, int x, int y,
                     unsigned char r, unsigned char g, unsigned char b)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return;
    }
    int offset = (y * img->width + x) * 3;
    img->pixels[offset + 0] = r;
    img->pixels[offset + 1] = g;
    img->pixels[offset + 2] = b;
}

/* ── image_get_pixel ──────────────────────────────────────────────
 * Read the color of pixel (x, y).
 * Returns 0 on success, -1 if out of bounds.
 */
int image_get_pixel(const Image *img, int x, int y,
                    unsigned char *r, unsigned char *g, unsigned char *b)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return -1;
    }
    int offset = (y * img->width + x) * 3;
    *r = img->pixels[offset + 0];
    *g = img->pixels[offset + 1];
    *b = img->pixels[offset + 2];
    return 0;
}

/* ── image_fill ───────────────────────────────────────────────────
 * Fill the entire image with a single color.
 */
void image_fill(Image *img, unsigned char r, unsigned char g, unsigned char b)
{
    int total = img->width * img->height;
    for (int i = 0; i < total; i++) {
        img->pixels[i * 3 + 0] = r;
        img->pixels[i * 3 + 1] = g;
        img->pixels[i * 3 + 2] = b;
    }
}

/* ── image_draw_line ──────────────────────────────────────────────
 * Draw a line from (x0,y0) to (x1,y1) using Bresenham's algorithm.
 * Handles all octants. Out-of-bounds pixels are silently clipped.
 */
void image_draw_line(Image *img, int x0, int y0, int x1, int y1,
                     unsigned char r, unsigned char g, unsigned char b)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int sx = (dx > 0) ? 1 : -1;
    int sy = (dy > 0) ? 1 : -1;

    dx = abs(dx);
    dy = -abs(dy);

    int err = dx + dy;  /* note: dy is negative here */

    for (;;) {
        image_set_pixel(img, x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/* ── image_draw_rect ──────────────────────────────────────────────
 * Draw a filled rectangle. Top-left corner at (x, y), size w x h.
 * Clipped to image bounds.
 */
void image_draw_rect(Image *img, int x, int y, int w, int h,
                     unsigned char r, unsigned char g, unsigned char b)
{
    /* Compute clipped bounds */
    int x0 = (x < 0) ? 0 : x;
    int y0 = (y < 0) ? 0 : y;
    int x1 = (x + w > img->width) ? img->width : x + w;
    int y1 = (y + h > img->height) ? img->height : y + h;

    for (int row = y0; row < y1; row++) {
        for (int col = x0; col < x1; col++) {
            int offset = (row * img->width + col) * 3;
            img->pixels[offset + 0] = r;
            img->pixels[offset + 1] = g;
            img->pixels[offset + 2] = b;
        }
    }
}

/* ── image_save_ppm ───────────────────────────────────────────────
 * Save the image as a PPM P6 file.
 * Returns 0 on success, -1 on failure.
 */
int image_save_ppm(const Image *img, const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);
    size_t pixel_count = (size_t)img->width * (size_t)img->height * 3;
    size_t written = fwrite(img->pixels, 1, pixel_count, f);
    fclose(f);
    return (written == pixel_count) ? 0 : -1;
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
