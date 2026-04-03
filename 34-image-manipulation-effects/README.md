# Module 28b — Image Manipulation & Effects

## What You Will Learn

How to manipulate images at the pixel level: adjusting brightness and contrast,
converting to grayscale and sepia, inverting colors, and applying convolution
kernels for blur, sharpen, and edge detection. These are the building blocks
behind every image editor and camera filter.

---

## Images as 2D Pixel Arrays

A digital image is a rectangular grid of pixels. Each pixel has three color
channels: **Red**, **Green**, and **Blue** (RGB), each stored as a value from
0 (none) to 255 (full intensity).

```
   Image: 4 pixels wide x 3 pixels tall
   ┌──────────┬──────────┬──────────┬──────────┐
   │ (R,G,B)  │ (R,G,B)  │ (R,G,B)  │ (R,G,B)  │  row 0
   ├──────────┼──────────┼──────────┼──────────┤
   │ (R,G,B)  │ (R,G,B)  │ (R,G,B)  │ (R,G,B)  │  row 1
   ├──────────┼──────────┼──────────┼──────────┤
   │ (R,G,B)  │ (R,G,B)  │ (R,G,B)  │ (R,G,B)  │  row 2
   └──────────┴──────────┴──────────┴──────────┘
        col 0      col 1      col 2      col 3
```

In memory, we store the pixels in **row-major order** — all pixels of row 0,
then all of row 1, and so on:

```
   Memory layout (1D array, row-major):
   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
   │ p00 │ p01 │ p02 │ p03 │ p10 │ p11 │ p12 │ p13 │ p20 │ p21 │ p22 │ p23 │
   └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
   |<------- row 0 -------->|<------- row 1 -------->|<------- row 2 -------->|

   Index of pixel at (row, col) = row * width + col
```

Our pixel struct:

```c
typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct {
    int width, height;
    Pixel *data;          /* heap-allocated array of width * height pixels */
} Image;
```

---

## Pixel-Level Operations

### Brightness

Add a delta to each channel. If delta is positive, the image gets brighter;
if negative, darker. **Clamp** each channel to [0, 255] so it does not wrap
around.

```
   Original pixel: (100, 150, 200)
   Delta = +80

   R: 100 + 80 = 180  (ok)
   G: 150 + 80 = 230  (ok)
   B: 200 + 80 = 280  → clamped to 255

   Result: (180, 230, 255)
```

### Contrast

Shift each channel away from (or toward) the midpoint 128:

```
   new_value = clamp((value - 128) * factor + 128)
```

A factor > 1 increases contrast; < 1 decreases it.

### Gamma Correction

Non-linear brightness adjustment used by monitors:

```
   normalized = value / 255.0
   corrected  = pow(normalized, gamma)
   result     = corrected * 255
```

**Why is gamma correction needed?** Monitors don't display brightness
linearly. If you send a pixel value of 128 (half of 255), the monitor
doesn't show half-brightness — it shows about 22% brightness. This is
because CRT monitors (and LCDs that emulate them) have a power-law
response curve. Gamma correction compensates: by applying
`pow(value, gamma)`, you pre-distort the values so they look correct on
screen. A gamma of 2.2 is standard for most displays.

- Gamma < 1 brightens dark areas (lifts shadows)
- Gamma > 1 darkens (crushes shadows)

---

## Color Manipulation

### Grayscale (Luminance)

The human eye is most sensitive to green, less to red, least to blue.
The standard luminance formula weights accordingly:

```
   gray = 0.299 * R + 0.587 * G + 0.114 * B
```

**Why these specific weights?** These weights come from how the human eye
perceives color. Your eyes are MOST sensitive to green light (0.587),
moderately sensitive to red (0.299), and LEAST sensitive to blue (0.114).
If you used equal weights (0.333 each), bright blue and bright green would
look equally bright in grayscale — but in reality, green appears much
brighter to human eyes. These specific values come from the ITU-R BT.601
standard, based on decades of color perception research.

Set R = G = B = gray for a grayscale pixel.

```
   Before:  (255, 100, 50)
   gray = 0.299*255 + 0.587*100 + 0.114*50
        = 76.245 + 58.7 + 5.7
        = 140.645 ≈ 141

   After:   (141, 141, 141)
```

### Inversion (Negative)

Flip every channel: `new = 255 - old`.

```
   Before:  (100, 200, 50)   →   After:  (155, 55, 205)
```

Inverting twice gives back the original — a useful test!

### Sepia Tone

A warm, brownish tint that simulates old photographs.

The standard sepia transform applies fixed weights to the original RGB:

```
   new_r = clamp(0.393*R + 0.769*G + 0.189*B)
   new_g = clamp(0.349*R + 0.686*G + 0.168*B)
   new_b = clamp(0.272*R + 0.534*G + 0.131*B)
```

---

## Convolution from First Principles

Convolution is the technique behind blur, sharpen, and edge detection.
The idea: for each pixel, compute a **weighted average** of its neighbors
using a small matrix called a **kernel** (typically 3x3).

### The Kernel

A 3x3 kernel is a grid of weights:

```
   ┌────┬────┬────┐
   │ k00│ k01│ k02│
   ├────┼────┼────┤
   │ k10│ k11│ k12│     kernel[3][3]
   ├────┼────┼────┤
   │ k20│ k21│ k22│
   └────┴────┴────┘
```

### How the Sliding Window Works

Place the kernel centered on each pixel. Multiply each neighbor by the
corresponding kernel weight. Sum and divide by a divisor.

```
   Image (5x5):                    Kernel (3x3, identity):
   ┌────┬────┬────┬────┬────┐      ┌────┬────┬────┐
   │ 10 │ 20 │ 30 │ 40 │ 50 │      │  0 │  0 │  0 │
   ├────┼────┼────┼────┼────┤      ├────┼────┼────┤
   │ 60 │ 70 │ 80 │ 90 │100 │      │  0 │  1 │  0 │
   ├────┼────┼────┼────┼────┤      ├────┼────┼────┤
   │110 │120 │[130]│140│150 │      │  0 │  0 │  0 │
   ├────┼────┼────┼────┼────┤      └────┴────┴────┘
   │160 │170 │180 │190 │200 │
   ├────┼────┼────┼────┼────┤      Center pixel [130]
   │210 │220 │230 │240 │250 │      Result = 0*80 + 0*90 + 0*100
   └────┴────┴────┴────┴────┘             + 0*120 + 1*130 + 0*140
                                           + 0*170 + 0*180 + 0*190
   The kernel slides across every                   = 130  (unchanged!)
   interior pixel of the image.
```

**For each pixel at (y, x):**

```
   sum = 0
   for ky = -1 to 1:
       for kx = -1 to 1:
           sum += image[y+ky][x+kx] * kernel[ky+1][kx+1]
   result[y][x] = clamp(sum / divisor)
```

**Border handling:** We skip edge pixels (1-pixel border) since they have no
complete 3x3 neighborhood. Set them to black (0,0,0) in the output.

### Convolution Kernel Sliding Over Pixels

```
   Step 1: Kernel at (1,1)           Step 2: Kernel at (1,2)
   ┌─────────────┐                   ┌─────────────┐
   │[10  20  30] 40  50│             │ 10 [20  30  40] 50│
   │[60  70  80] 90 100│             │ 60 [70  80  90]100│
   │[110 120 130]140 150│            │110 [120 130 140]150│
   │ 160 170 180 190 200│            │ 160 170 180 190 200│
   │ 210 220 230 240 250│            │ 210 220 230 240 250│
   └─────────────┘                   └─────────────┘
    Kernel covers the 3x3             Kernel slides right one pixel
    neighborhood around (1,1)         now centered on (1,2)
```

---

## Common Kernels

### How to Choose the Divisor

The divisor for a convolution kernel should equal the **sum of all kernel
values**. This keeps the overall brightness unchanged. For box blur (all
1s), the sum is 9, so divide by 9. For a sharpen kernel (values like -1,
-1, 5, -1, -1), the sum is 1, so divide by 1 (no change needed). If the
sum is 0 (like edge detection kernels), the output represents *change*
rather than brightness — no division needed, but you may need to add 128
to shift the range so that "no change" appears as medium gray instead of
black.

### Box Blur (Average)

Every neighbor contributes equally. Divides by 9 to normalize.

```
   ┌───┬───┬───┐
   │ 1 │ 1 │ 1 │
   ├───┼───┼───┤
   │ 1 │ 1 │ 1 │     divisor = 9
   ├───┼───┼───┤
   │ 1 │ 1 │ 1 │
   └───┴───┴───┘
```

Before/After blur:

```
   Before (sharp edge):       After box blur:
   ┌───┬───┬───┬───┬───┐     ┌───┬───┬───┬───┬───┐
   │ 0 │ 0 │ 0 │ 0 │ 0 │     │   │ 28│ 28│ 28│   │
   ├───┼───┼───┼───┼───┤     ├───┼───┼───┼───┼───┤
   │ 0 │ 0 │ 0 │ 0 │ 0 │     │   │ 85│ 85│ 85│   │
   ├───┼───┼───┼───┼───┤     ├───┼───┼───┼───┼───┤
   │255│255│255│255│255│     │   │170│170│170│   │
   ├───┼───┼───┼───┼───┤     ├───┼───┼───┼───┼───┤
   │255│255│255│255│255│     │   │227│227│227│   │
   ├───┼───┼───┼───┼───┤     ├───┼───┼───┼───┼───┤
   │255│255│255│255│255│     │   │255│255│255│   │
   └───┴───┴───┴───┴───┘     └───┴───┴───┴───┴───┘
   Hard boundary at row 2     Boundary is now gradual (blurred)
   Edges (border pixels) set to 0 in output.
```

### Gaussian Blur (Concept)

Like box blur, but center pixels get more weight. Approximation:

```
   ┌───┬───┬───┐
   │ 1 │ 2 │ 1 │
   ├───┼───┼───┤
   │ 2 │ 4 │ 2 │     divisor = 16
   ├───┼───┼───┤
   │ 1 │ 2 │ 1 │
   └───┴───┴───┘
```

This produces smoother results than box blur because nearby pixels matter more.

### Sharpen

Emphasizes the center pixel and subtracts neighbors:

```
   ┌────┬────┬────┐
   │  0 │ -1 │  0 │
   ├────┼────┼────┤
   │ -1 │  5 │ -1 │     divisor = 1
   ├────┼────┼────┤
   │  0 │ -1 │  0 │
   └────┴────┴────┘
```

Before/After sharpen:

```
   Before (slightly blurred):   After sharpen:
   ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
   │100│100│100│100│100│       │   │100│100│100│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │100│110│120│110│100│       │   │100│110│100│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │100│120│150│120│100│       │   │110│190│110│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │100│110│120│110│100│       │   │100│110│100│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │100│100│100│100│100│       │   │100│100│100│   │
   └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
   Soft bump in center          Center is more pronounced (sharper)
```

---

## Edge Detection (Sobel)

Edges are where pixel values change rapidly. The Sobel operator uses two
kernels to detect horizontal and vertical gradients:

```
   Sobel X (horizontal edges):    Sobel Y (vertical edges):
   ┌────┬────┬────┐               ┌────┬────┬────┐
   │ -1 │  0 │ +1 │               │ -1 │ -2 │ -1 │
   ├────┼────┼────┤               ├────┼────┼────┤
   │ -2 │  0 │ +2 │               │  0 │  0 │  0 │
   ├────┼────┼────┤               ├────┼────┼────┤
   │ -1 │  0 │ +1 │               │ +1 │ +2 │ +1 │
   └────┴────┴────┘               └────┴────┴────┘
```

For each pixel, compute both gradients and combine them:

```
   gx = convolve with Sobel_X (no divisor)
   gy = convolve with Sobel_Y (no divisor)
   magnitude = sqrt(gx*gx + gy*gy)
   result = clamp(magnitude, 0, 255)
```

Before/After edge detection:

```
   Before (rectangle):          After Sobel:
   ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
   │  0│  0│  0│  0│  0│       │   │  0│  0│  0│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │  0│255│255│255│  0│       │   │255│255│255│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │  0│255│255│255│  0│       │   │255│  0│255│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │  0│255│255│255│  0│       │   │255│255│255│   │
   ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
   │  0│  0│  0│  0│  0│       │   │  0│  0│  0│   │
   └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
   Solid white rectangle         Only edges remain visible
   on black background
```

---

## Image Resizing

### Nearest-Neighbor

The simplest resize: for each pixel in the output, pick the closest pixel
from the source.

```
   src_x = dst_x * (src_width / dst_width)
   src_y = dst_y * (src_height / dst_height)
```

Fast but produces blocky/pixelated results.

### Bilinear Interpolation

Instead of picking one source pixel, average the four nearest source pixels
weighted by distance. Produces smoother results.

```
   Source pixel grid:
         x
   ┌─────┼──────┐
   │ A   │   B  │
   │     ● P    │    P = point we want to sample
   ├─────┼──────┤    P's value = weighted avg of A, B, C, D
   │ C   │   D  │    based on distance to each
   └─────┴──────┘
```

---

## Exercises

In `exercises/image_ops.c` you will implement:

1. **img_create / img_destroy** — allocate and free an image
2. **img_set_pixel / img_get_pixel** — read and write individual pixels
3. **img_brightness** — adjust brightness with clamping
4. **img_grayscale** — convert to grayscale using luminance weights
5. **img_invert** — invert all channels
6. **img_sepia** — apply sepia tone transform
7. **img_convolve** — apply an arbitrary 3x3 convolution kernel
8. **img_blur** — box blur using convolution
9. **img_edge_detect** — Sobel edge detection

Build and test:

```bash
make test
```

All 14 tests should pass.

---

[← Previous: Module 28: Graphics Programming](../28-graphics-programming/README.md) | [Next: Module 28c — Ray Tracing & 3D Graphics →](../28c-ray-tracing-3d-graphics/README.md)
