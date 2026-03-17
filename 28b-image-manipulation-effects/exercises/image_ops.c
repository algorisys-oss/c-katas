/*
 * Kata: Image Manipulation & Effects
 *
 * Implement pixel-level operations and convolution-based filters
 * on a simple RGB image buffer:
 *
 *   - img_create / img_destroy — allocate and free images
 *   - img_set_pixel / img_get_pixel — read/write individual pixels
 *   - img_brightness — adjust brightness with clamping
 *   - img_grayscale — luminance formula (0.299R + 0.587G + 0.114B)
 *   - img_invert — 255 - each channel
 *   - img_sepia — warm tint using standard sepia matrix
 *   - img_convolve — apply a 3x3 convolution kernel
 *   - img_blur — box blur (3x3 average)
 *   - img_edge_detect — Sobel edge detection
 *
 * 14 tests total.
 *
 * Hints:
 *   - Always clamp channel values to [0, 255] after arithmetic
 *   - For convolution, skip the 1-pixel border (set to black)
 *   - Sobel uses two kernels (horizontal and vertical gradients)
 *     combined with sqrt(gx*gx + gy*gy)
 *   - Use (int) casts before arithmetic to avoid uint8_t overflow
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

/* ── Exercise 1: img_create ──────────────────────────────────────── *
 * Allocate an Image struct and a pixel buffer of width*height pixels.
 * Initialize all pixels to (0, 0, 0) — black.
 * Return NULL on allocation failure.
 *
 * Hint: use malloc for the Image, calloc for the pixel data.
 */
Image *img_create(int width, int height)
{
    /* TODO: Allocate the Image struct */
    /* TODO: Set width and height */
    /* TODO: Allocate the pixel data array (calloc zeros it) */
    /* TODO: Handle allocation failure — free img and return NULL */
    (void)width; (void)height; (void)clamp;
    return NULL;
}

/* ── Exercise 2: img_destroy ─────────────────────────────────────── *
 * Free the pixel data and the Image struct.
 * Handle NULL gracefully.
 */
void img_destroy(Image *img)
{
    /* TODO: Free data, then the struct (check for NULL) */
    (void)img;
}

/* ── Exercise 3: img_set_pixel ───────────────────────────────────── *
 * Set the pixel at (x, y) to p.
 * Do nothing if (x, y) is out of bounds.
 *
 * Remember: data is stored row-major → index = y * width + x
 */
void img_set_pixel(Image *img, int x, int y, Pixel p)
{
    /* TODO: Bounds check, then set img->data[y * width + x] */
    (void)img; (void)x; (void)y; (void)p;
}

/* ── Exercise 4: img_get_pixel ───────────────────────────────────── *
 * Return the pixel at (x, y).
 * Return black (0,0,0) if out of bounds.
 */
Pixel img_get_pixel(const Image *img, int x, int y)
{
    /* TODO: Bounds check, return pixel or black */
    (void)img; (void)x; (void)y;
    Pixel black = {0, 0, 0};
    return black;
}

/* ── Exercise 5: img_brightness ──────────────────────────────────── *
 * Add delta to every channel of every pixel.
 * Clamp each channel to [0, 255].
 *
 * Example: pixel (200, 100, 50) with delta=80 → (255, 180, 130)
 */
void img_brightness(Image *img, int delta)
{
    /* TODO: Loop over all pixels, add delta to each channel, clamp */
    (void)img; (void)delta;
}

/* ── Exercise 6: img_grayscale ───────────────────────────────────── *
 * Convert each pixel to grayscale using the luminance formula:
 *   gray = 0.299*R + 0.587*G + 0.114*B
 * Set R = G = B = gray.
 *
 * Hint: add 0.5 before casting to uint8_t for proper rounding.
 */
void img_grayscale(Image *img)
{
    /* TODO: Loop over all pixels, compute gray, set R=G=B=gray */
    (void)img;
}

/* ── Exercise 7: img_invert ──────────────────────────────────────── *
 * Invert every channel: new = 255 - old.
 *
 * Note: inverting twice should return the original pixel.
 */
void img_invert(Image *img)
{
    /* TODO: Loop over all pixels, invert each channel */
    (void)img;
}

/* ── Exercise 8: img_sepia ───────────────────────────────────────── *
 * Apply the standard sepia transform to each pixel:
 *   new_r = clamp(0.393*R + 0.769*G + 0.189*B)
 *   new_g = clamp(0.349*R + 0.686*G + 0.168*B)
 *   new_b = clamp(0.272*R + 0.534*G + 0.131*B)
 *
 * Hint: read R, G, B into ints first, compute all three new values,
 *       then write them back (so you don't overwrite R before using it).
 *       Add 0.5 before casting to int for rounding.
 */
void img_sepia(Image *img)
{
    /* TODO: Loop over all pixels, apply sepia formula, clamp */
    (void)img;
}

/* ── Exercise 9: img_convolve ────────────────────────────────────── *
 * Apply a 3x3 convolution kernel to src, writing result to dst.
 *
 * For each interior pixel (skip 1-pixel border):
 *   1. Sum: for each of the 9 neighbors, multiply pixel channel
 *      by corresponding kernel weight
 *   2. Divide sum by divisor
 *   3. Clamp to [0, 255]
 *
 * Border pixels in dst should be black (0,0,0).
 * Process each channel (R, G, B) independently.
 *
 * Hint: Use nested loops ky=-1..1, kx=-1..1 to walk the 3x3 window.
 *       Kernel index: kernel[ky+1][kx+1]
 *       Source index: src->data[(y+ky) * w + (x+kx)]
 */
void img_convolve(const Image *src, Image *dst,
                  int kernel[3][3], int divisor)
{
    /* TODO: Clear dst with memset (borders will be black) */
    /* TODO: Loop over interior pixels (y: 1..h-2, x: 1..w-2) */
    /* TODO: For each pixel, compute weighted sum of 3x3 neighborhood */
    /* TODO: Divide by divisor, clamp, write to dst */
    (void)src; (void)dst; (void)kernel; (void)divisor;
}

/* ── Exercise 10: img_blur ───────────────────────────────────────── *
 * Box blur: average of the 3x3 neighborhood.
 *
 * This is just a convolution with kernel of all 1s and divisor 9.
 * Call img_convolve with the right kernel.
 */
void img_blur(const Image *src, Image *dst)
{
    /* TODO: Define 3x3 kernel of all 1s, call img_convolve with divisor 9 */
    (void)src; (void)dst;
}

/* ── Exercise 11: img_edge_detect ────────────────────────────────── *
 * Sobel edge detection.
 *
 * Use two kernels:
 *   Sobel X: {{-1,0,1}, {-2,0,2}, {-1,0,1}}
 *   Sobel Y: {{-1,-2,-1}, {0,0,0}, {1,2,1}}
 *
 * For each interior pixel:
 *   1. Compute gx = convolve neighborhood with Sobel_X (each channel)
 *   2. Compute gy = convolve neighborhood with Sobel_Y (each channel)
 *   3. magnitude per channel = sqrt(gx*gx + gy*gy)
 *   4. Average the three channel magnitudes for grayscale output
 *   5. Clamp to [0, 255], set R=G=B=magnitude
 *
 * Border pixels are black.
 *
 * Hint: Do NOT call img_convolve here — you need both gx and gy
 *       for the same pixel, so compute them together in one pass.
 */
void img_edge_detect(const Image *src, Image *dst)
{
    /* TODO: Define Sobel X and Sobel Y kernels */
    /* TODO: Clear dst with memset */
    /* TODO: Loop over interior pixels */
    /* TODO: Compute gx and gy for each channel */
    /* TODO: Combine with sqrt(gx^2 + gy^2), average channels, clamp */
    (void)src; (void)dst;
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
        /* new_r = 0.393*100+0.769*150+0.189*200 = 192.45 -> 192 */
        /* new_g = 0.349*100+0.686*150+0.168*200 = 171.4  -> 171 */
        /* new_b = 0.272*100+0.534*150+0.131*200 = 133.5  -> 134 */
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
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                uint8_t v = (uint8_t)((x + y) * 20);
                Pixel px = {v, v, v};
                img_set_pixel(src, x, y, px);
            }
        }
        int box[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
        img_convolve(src, dst, box, 9);
        /* Center pixel (2,2): sum of 3x3 neighborhood = 720, /9 = 80 */
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
