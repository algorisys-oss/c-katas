/*
 * Solution: Image Manipulation & Effects
 *
 * Pixel-level operations, color transforms, and convolution-based
 * filters (blur, sharpen, edge detection) on a simple RGB image.
 *
 * 14 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* ── Pixel & Image types ─────────────────────────────────────────── */

typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct {
    int width, height;
    Pixel *data;
} Image;

/* ── Helper: clamp a value to [0, 255] ───────────────────────────── */
static int clamp(int v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return v;
}

/* ── img_create ──────────────────────────────────────────────────── *
 * Allocate an image of the given dimensions. All pixels start at
 * (0, 0, 0) — black. Returns NULL on allocation failure.
 */
Image *img_create(int width, int height)
{
    Image *img = malloc(sizeof(Image));
    if (!img) return NULL;
    img->width = width;
    img->height = height;
    img->data = calloc((size_t)width * (size_t)height, sizeof(Pixel));
    if (!img->data) {
        free(img);
        return NULL;
    }
    return img;
}

/* ── img_destroy ─────────────────────────────────────────────────── */
void img_destroy(Image *img)
{
    if (img) {
        free(img->data);
        free(img);
    }
}

/* ── img_set_pixel ───────────────────────────────────────────────── */
void img_set_pixel(Image *img, int x, int y, Pixel p)
{
    if (x >= 0 && x < img->width && y >= 0 && y < img->height) {
        img->data[y * img->width + x] = p;
    }
}

/* ── img_get_pixel ───────────────────────────────────────────────── */
Pixel img_get_pixel(const Image *img, int x, int y)
{
    if (x >= 0 && x < img->width && y >= 0 && y < img->height) {
        return img->data[y * img->width + x];
    }
    Pixel black = {0, 0, 0};
    return black;
}

/* ── img_brightness ──────────────────────────────────────────────── *
 * Add delta to every channel of every pixel, clamping to [0, 255].
 */
void img_brightness(Image *img, int delta)
{
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        img->data[i].r = (uint8_t)clamp(img->data[i].r + delta);
        img->data[i].g = (uint8_t)clamp(img->data[i].g + delta);
        img->data[i].b = (uint8_t)clamp(img->data[i].b + delta);
    }
}

/* ── img_grayscale ───────────────────────────────────────────────── *
 * Convert to grayscale using the luminance formula:
 *   gray = 0.299*R + 0.587*G + 0.114*B
 */
void img_grayscale(Image *img)
{
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        uint8_t gray = (uint8_t)(0.299 * img->data[i].r +
                                  0.587 * img->data[i].g +
                                  0.114 * img->data[i].b + 0.5);
        img->data[i].r = gray;
        img->data[i].g = gray;
        img->data[i].b = gray;
    }
}

/* ── img_invert ──────────────────────────────────────────────────── *
 * Invert every channel: new = 255 - old.
 */
void img_invert(Image *img)
{
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        img->data[i].r = 255 - img->data[i].r;
        img->data[i].g = 255 - img->data[i].g;
        img->data[i].b = 255 - img->data[i].b;
    }
}

/* ── img_sepia ───────────────────────────────────────────────────── *
 * Apply the standard sepia transform:
 *   new_r = clamp(0.393*R + 0.769*G + 0.189*B)
 *   new_g = clamp(0.349*R + 0.686*G + 0.168*B)
 *   new_b = clamp(0.272*R + 0.534*G + 0.131*B)
 */
void img_sepia(Image *img)
{
    int n = img->width * img->height;
    for (int i = 0; i < n; i++) {
        int r = img->data[i].r;
        int g = img->data[i].g;
        int b = img->data[i].b;
        int new_r = (int)(0.393 * r + 0.769 * g + 0.189 * b + 0.5);
        int new_g = (int)(0.349 * r + 0.686 * g + 0.168 * b + 0.5);
        int new_b = (int)(0.272 * r + 0.534 * g + 0.131 * b + 0.5);
        img->data[i].r = (uint8_t)clamp(new_r);
        img->data[i].g = (uint8_t)clamp(new_g);
        img->data[i].b = (uint8_t)clamp(new_b);
    }
}

/* ── img_convolve ────────────────────────────────────────────────── *
 * Apply a 3x3 convolution kernel to src, writing the result to dst.
 * Border pixels (1-pixel edge) are set to black.
 * Each channel is processed independently:
 *   sum = sum of (neighbor * kernel_weight) for the 3x3 neighborhood
 *   result = clamp(sum / divisor)
 */
void img_convolve(const Image *src, Image *dst,
                  int kernel[3][3], int divisor)
{
    int w = src->width;
    int h = src->height;

    /* Clear destination (borders will be black) */
    memset(dst->data, 0, (size_t)w * (size_t)h * sizeof(Pixel));

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    Pixel p = src->data[(y + ky) * w + (x + kx)];
                    int k = kernel[ky + 1][kx + 1];
                    sum_r += p.r * k;
                    sum_g += p.g * k;
                    sum_b += p.b * k;
                }
            }
            dst->data[y * w + x].r = (uint8_t)clamp(sum_r / divisor);
            dst->data[y * w + x].g = (uint8_t)clamp(sum_g / divisor);
            dst->data[y * w + x].b = (uint8_t)clamp(sum_b / divisor);
        }
    }
}

/* ── img_blur ────────────────────────────────────────────────────── *
 * Box blur: average of the 3x3 neighborhood. Uses img_convolve
 * with a kernel of all 1s and divisor 9.
 */
void img_blur(const Image *src, Image *dst)
{
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    img_convolve(src, dst, kernel, 9);
}

/* ── img_edge_detect ─────────────────────────────────────────────── *
 * Sobel edge detection. Computes horizontal and vertical gradients,
 * then combines: magnitude = sqrt(gx*gx + gy*gy), clamped to [0,255].
 * Output is grayscale (R=G=B=magnitude). Border pixels are black.
 */
void img_edge_detect(const Image *src, Image *dst)
{
    int w = src->width;
    int h = src->height;

    int sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int sobel_y[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    /* Clear destination */
    memset(dst->data, 0, (size_t)w * (size_t)h * sizeof(Pixel));

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            int gx_r = 0, gx_g = 0, gx_b = 0;
            int gy_r = 0, gy_g = 0, gy_b = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    Pixel p = src->data[(y + ky) * w + (x + kx)];
                    int kval_x = sobel_x[ky + 1][kx + 1];
                    int kval_y = sobel_y[ky + 1][kx + 1];
                    gx_r += p.r * kval_x;
                    gx_g += p.g * kval_x;
                    gx_b += p.b * kval_x;
                    gy_r += p.r * kval_y;
                    gy_g += p.g * kval_y;
                    gy_b += p.b * kval_y;
                }
            }
            int mag_r = (int)(sqrt(gx_r * gx_r + gy_r * gy_r) + 0.5);
            int mag_g = (int)(sqrt(gx_g * gx_g + gy_g * gy_g) + 0.5);
            int mag_b = (int)(sqrt(gx_b * gx_b + gy_b * gy_b) + 0.5);
            /* Average the three channel magnitudes for grayscale output */
            int mag = (mag_r + mag_g + mag_b) / 3;
            uint8_t v = (uint8_t)clamp(mag);
            dst->data[y * w + x].r = v;
            dst->data[y * w + x].g = v;
            dst->data[y * w + x].b = v;
        }
    }
}

/* ── Test Harness ────────────────────────────────────────────────── */

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
    printf("=== Image Manipulation & Effects ===\n\n");

    /* ── Test 1: Create and destroy ──────────────────────────────── */
    {
        Image *img = img_create(10, 5);
        TEST("img_create allocates correctly")
            EXPECT(img != NULL && img->width == 10 && img->height == 5);
        img_destroy(img);
    }

    /* ── Test 2: Set/get pixel ───────────────────────────────────── */
    {
        Image *img = img_create(3, 3);
        Pixel p = {100, 150, 200};
        img_set_pixel(img, 1, 2, p);
        Pixel got = img_get_pixel(img, 1, 2);
        TEST("img_set/get_pixel round-trip")
            EXPECT(got.r == 100 && got.g == 150 && got.b == 200);
        img_destroy(img);
    }

    /* ── Test 3: Brightness positive, no clamp ───────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {100, 100, 100};
        img_set_pixel(img, 0, 0, px);
        img_brightness(img, 50);
        Pixel p = img_get_pixel(img, 0, 0);
        TEST("img_brightness +50 on (100,100,100)")
            EXPECT(p.r == 150 && p.g == 150 && p.b == 150);
        img_destroy(img);
    }

    /* ── Test 4: Brightness clamps to 255 ────────────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {200, 230, 250};
        img_set_pixel(img, 0, 0, px);
        img_brightness(img, 80);
        Pixel p = img_get_pixel(img, 0, 0);
        TEST("img_brightness clamps to 255")
            EXPECT(p.r == 255 && p.g == 255 && p.b == 255);
        img_destroy(img);
    }

    /* ── Test 5: Brightness clamps to 0 ──────────────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {30, 10, 5};
        img_set_pixel(img, 0, 0, px);
        img_brightness(img, -50);
        Pixel p = img_get_pixel(img, 0, 0);
        TEST("img_brightness clamps to 0")
            EXPECT(p.r == 0 && p.g == 0 && p.b == 0);
        img_destroy(img);
    }

    /* ── Test 6: Grayscale known pixel ───────────────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {255, 100, 50};
        img_set_pixel(img, 0, 0, px);
        img_grayscale(img);
        Pixel p = img_get_pixel(img, 0, 0);
        /* 0.299*255 + 0.587*100 + 0.114*50 = 76.245+58.7+5.7 = 140.645 -> 141 */
        TEST("img_grayscale luminance formula")
            EXPECT(p.r == 141 && p.g == 141 && p.b == 141);
        img_destroy(img);
    }

    /* ── Test 7: Invert once ─────────────────────────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {100, 200, 50};
        img_set_pixel(img, 0, 0, px);
        img_invert(img);
        Pixel p = img_get_pixel(img, 0, 0);
        TEST("img_invert (100,200,50) -> (155,55,205)")
            EXPECT(p.r == 155 && p.g == 55 && p.b == 205);
        img_destroy(img);
    }

    /* ── Test 8: Invert round-trip ───────────────────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {37, 142, 203};
        img_set_pixel(img, 0, 0, px);
        img_invert(img);
        img_invert(img);
        Pixel p = img_get_pixel(img, 0, 0);
        TEST("img_invert twice restores original")
            EXPECT(p.r == 37 && p.g == 142 && p.b == 203);
        img_destroy(img);
    }

    /* ── Test 9: Sepia on a known pixel ──────────────────────────── */
    {
        Image *img = img_create(1, 1);
        Pixel px = {100, 150, 200};
        img_set_pixel(img, 0, 0, px);
        img_sepia(img);
        Pixel p = img_get_pixel(img, 0, 0);
        /* new_r = 0.393*100+0.769*150+0.189*200 = 39.3+115.35+37.8 = 192.45 -> 192 */
        /* new_g = 0.349*100+0.686*150+0.168*200 = 34.9+102.9+33.6 = 171.4 -> 171 */
        /* new_b = 0.272*100+0.534*150+0.131*200 = 27.2+80.1+26.2 = 133.5 -> 134 */
        TEST("img_sepia on (100, 150, 200)")
            EXPECT(p.r == 192 && p.g == 171 && p.b == 134);
        img_destroy(img);
    }

    /* ── Test 10: Convolve with identity kernel ──────────────────── */
    {
        Image *src = img_create(3, 3);
        Image *dst = img_create(3, 3);
        Pixel center = {80, 120, 200};
        img_set_pixel(src, 1, 1, center);
        img_set_pixel(src, 0, 0, (Pixel){10, 20, 30});
        img_set_pixel(src, 1, 0, (Pixel){40, 50, 60});
        img_set_pixel(src, 2, 0, (Pixel){70, 80, 90});
        img_set_pixel(src, 0, 1, (Pixel){100, 110, 120});
        img_set_pixel(src, 2, 1, (Pixel){130, 140, 150});
        img_set_pixel(src, 0, 2, (Pixel){160, 170, 180});
        img_set_pixel(src, 1, 2, (Pixel){190, 200, 210});
        img_set_pixel(src, 2, 2, (Pixel){220, 230, 240});
        int identity[3][3] = {{0,0,0},{0,1,0},{0,0,0}};
        img_convolve(src, dst, identity, 1);
        Pixel p = img_get_pixel(dst, 1, 1);
        TEST("img_convolve identity kernel preserves center")
            EXPECT(p.r == 80 && p.g == 120 && p.b == 200);
        img_destroy(src);
        img_destroy(dst);
    }

    /* ── Test 11: Convolve with known kernel on 5x5 ──────────────── */
    {
        Image *src = img_create(5, 5);
        Image *dst = img_create(5, 5);
        /* Fill with gradient: pixel value = (x+y)*20 in all channels */
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                uint8_t v = (uint8_t)((x + y) * 20);
                Pixel px = {v, v, v};
                img_set_pixel(src, x, y, px);
            }
        }
        int box[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
        img_convolve(src, dst, box, 9);
        /* Center pixel (2,2): neighbors (x+y)*20 for x in [1..3], y in [1..3]
         * Values: 40,60,80, 60,80,100, 80,100,120 -> sum=720, /9=80 */
        Pixel p = img_get_pixel(dst, 2, 2);
        TEST("img_convolve 5x5 image, known output pixel")
            EXPECT(p.r == 80 && p.g == 80 && p.b == 80);
        img_destroy(src);
        img_destroy(dst);
    }

    /* ── Test 12: Blur reduces contrast ──────────────────────────── */
    {
        Image *src = img_create(3, 3);
        Image *dst = img_create(3, 3);
        /* All black except center is white */
        Pixel white = {255, 255, 255};
        img_set_pixel(src, 1, 1, white);
        img_blur(src, dst);
        Pixel p = img_get_pixel(dst, 1, 1);
        /* 255/9 = 28 */
        TEST("img_blur reduces contrast (center moves toward avg)")
            EXPECT(p.r == 28 && p.g == 28 && p.b == 28);
        img_destroy(src);
        img_destroy(dst);
    }

    /* ── Test 13: Edge detect uniform area -> zero ───────────────── */
    {
        Image *src = img_create(5, 5);
        Image *dst = img_create(5, 5);
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                Pixel px = {100, 100, 100};
                img_set_pixel(src, x, y, px);
            }
        }
        img_edge_detect(src, dst);
        Pixel p = img_get_pixel(dst, 2, 2);
        TEST("img_edge_detect: uniform area -> zero")
            EXPECT(p.r == 0 && p.g == 0 && p.b == 0);
        img_destroy(src);
        img_destroy(dst);
    }

    /* ── Test 14: Edge detect finds boundary ─────────────────────── */
    {
        Image *src = img_create(5, 5);
        Image *dst = img_create(5, 5);
        /* Left half black, right half white (boundary at col 2/3) */
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                uint8_t v = (x >= 3) ? 255 : 0;
                Pixel px = {v, v, v};
                img_set_pixel(src, x, y, px);
            }
        }
        img_edge_detect(src, dst);
        Pixel p = img_get_pixel(dst, 2, 2);
        TEST("img_edge_detect: sharp boundary -> nonzero")
            EXPECT(p.r > 100);
        img_destroy(src);
        img_destroy(dst);
    }

    /* ── Summary ─────────────────────────────────────────────────── */
    printf("\nPassed %d / %d tests\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
