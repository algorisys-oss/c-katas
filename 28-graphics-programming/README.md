# Module 28: Graphics Programming

## What Is a Pixel?

A **pixel** (short for "picture element") is the smallest unit of a digital image.
Every image on your screen is a grid of pixels — thousands of tiny colored dots
arranged in rows and columns.

```
A 4x3 image = 12 pixels

    col 0   col 1   col 2   col 3
  ┌───────┬───────┬───────┬───────┐
  │ (0,0) │ (1,0) │ (2,0) │ (3,0) │  row 0
  ├───────┼───────┼───────┼───────┤
  │ (0,1) │ (1,1) │ (2,1) │ (3,1) │  row 1
  ├───────┼───────┼───────┼───────┤
  │ (0,2) │ (1,2) │ (2,2) │ (3,2) │  row 2
  └───────┴───────┴───────┴───────┘

  Coordinates: (x, y) where x = column, y = row
  Origin (0,0) is top-left
```

Each pixel stores a **color**. The most common encoding is **RGB** — three numbers
that represent how much Red, Green, and Blue light to mix together.

## RGB Color

Each channel (R, G, B) is stored as one byte: 0–255.

```
  0 = none of that color        255 = maximum of that color

  Examples:
  ┌──────────────────┬─────┬─────┬─────┐
  │ Color            │  R  │  G  │  B  │
  ├──────────────────┼─────┼─────┼─────┤
  │ Black            │   0 │   0 │   0 │
  │ White            │ 255 │ 255 │ 255 │
  │ Pure Red         │ 255 │   0 │   0 │
  │ Pure Green       │   0 │ 255 │   0 │
  │ Pure Blue        │   0 │   0 │ 255 │
  │ Yellow (R+G)     │ 255 │ 255 │   0 │
  │ Cyan (G+B)       │   0 │ 255 │ 255 │
  │ Magenta (R+B)    │ 255 │   0 │ 255 │
  │ Gray             │ 128 │ 128 │ 128 │
  │ Orange           │ 255 │ 165 │   0 │
  └──────────────────┴─────┴─────┴─────┘
```

### Pixel Memory Layout

One pixel = 3 consecutive bytes. An entire image is a flat array:

```
  A 3x2 image in memory (width=3, height=2, 6 pixels, 18 bytes):

  Byte index: 0  1  2  │ 3  4  5  │ 6  7  8  │ 9 10 11 │12 13 14 │15 16 17
              R  G  B  │ R  G  B  │ R  G  B  │ R  G  B │ R  G  B │ R  G  B
              ─────────┼──────────┼──────────┼─────────┼─────────┼─────────
              pixel(0,0)│pixel(1,0)│pixel(2,0)│pixel(0,1)│pixel(1,1)│pixel(2,1)
              row 0                           │ row 1

  Index of pixel (x, y) = (y * width + x) * 3
```

## PPM Image Format

PPM (Portable Pixel Map) is the **simplest** image format. No compression, no
headers to decode — just raw pixel data with a tiny text header.

### PPM P6 (Binary) Format

```
  P6\n            ← magic number (identifies format)
  <width> <height>\n
  255\n           ← max color value
  RGBRGBRGB...   ← raw pixel bytes, row by row, top to bottom
```

Example: a 2x2 image (red, green, blue, white):

```
  Header (ASCII text):    "P6\n2 2\n255\n"
  Pixel data (12 bytes):  [255,0,0] [0,255,0] [0,0,255] [255,255,255]
                           red       green      blue      white
```

That's it. No compression, no checksums. You can write one in a few lines of C.
Any image viewer on Linux can open `.ppm` files.

### Why PPM?

- Zero dependencies — just `fopen` and `fwrite`
- Perfect for learning — you understand every byte
- Trivially debuggable — you can hexdump the file and read it

## Drawing Primitives

### Setting a Single Pixel

The simplest operation: given (x, y) and a color, write 3 bytes into the buffer.
Always check bounds — writing outside the buffer is undefined behavior.

```c
// Index into the flat pixel array
int offset = (y * width + x) * 3;
pixels[offset + 0] = r;
pixels[offset + 1] = g;
pixels[offset + 2] = b;
```

### Drawing Lines: Bresenham's Algorithm

How do you draw a straight line between two points on a pixel grid?
You can't — pixels are discrete squares, not infinitely small points.
You have to **approximate** the ideal line by choosing which pixels to light up.

Bresenham's line algorithm does this using only **integer arithmetic** — no
floating point, no division. This made it fast enough to run on 1960s hardware,
and it's still the basis of line drawing today.

#### The Key Insight

For a line with slope between 0 and 1 (gentle slope going right and slightly down),
at each step you move one pixel to the right. The question is: do you also move
one pixel down, or stay on the same row?

```
  The ideal line passes through continuous coordinates,
  but we must pick one pixel per column:

     0   1   2   3   4   5   6   7   8
  0  ·   ·   ·   ·   ·   ·   ·   ·   ·
  1  ■───■   ·   ·   ·   ·   ·   ·   ·    Step right, stay on row?
  2  ·   ·───■───■   ·   ·   ·   ·   ·    Step right + step down?
  3  ·   ·   ·   ·───■───■   ·   ·   ·
  4  ·   ·   ·   ·   ·   ·───■───■   ·
  5  ·   ·   ·   ·   ·   ·   ·   ·   ·

  Line from (0,1) to (8,4): dx=8, dy=3
  At each column, Bresenham accumulates an error term.
  When the error crosses a threshold, we step in y.
```

#### All Octants

A naive Bresenham handles only lines going right and slightly down (one octant).
A real implementation must handle all 8 directions:

```
          \  2  |  1  /
           \    |    /
        3   \   |   /   0        Octant 0: dx > 0, dy >= 0, |dx| > |dy|
     ────────\  |  /────────     Octant 1: dx > 0, dy > 0,  |dy| > |dx|
        4    /  |  \    7        ... and so on.
           /    |    \
          /  5  |  6  \
```

The trick: compute `dx = abs(x1-x0)`, `dy = -abs(y1-y0)`, and step in x/y
using sign-based increments. This handles all octants in a single loop.

### Filled Rectangle

A filled rectangle is straightforward: two nested loops over rows and columns,
setting each pixel. Clip to image bounds to avoid out-of-bounds writes.

```
  image_draw_rect(img, x=2, y=1, w=4, h=3, red):

     0   1   2   3   4   5   6   7
  0  ·   ·   ·   ·   ·   ·   ·   ·
  1  ·   ·   ■   ■   ■   ■   ·   ·
  2  ·   ·   ■   ■   ■   ■   ·   ·
  3  ·   ·   ■   ■   ■   ■   ·   ·
  4  ·   ·   ·   ·   ·   ·   ·   ·
```

## Coordinate System

We use the **screen convention**:

```
  (0,0) ──────────────→ x (increases right)
    │
    │
    │
    │
    ↓
    y (increases down)
```

This is standard for image formats and screen rendering. Mathematical
convention (y-up) is used in some 3D graphics contexts, but for 2D
pixel-based work, y-down is the norm.

## 2D Transformations (Bonus Concepts)

Once you can draw shapes, you can transform them using matrix math:

### Translation (move)

```
  x' = x + tx
  y' = y + ty
```

### Scaling

```
  x' = x * sx
  y' = y * sy
```

### Rotation (around origin)

```
  x' = x * cos(θ) - y * sin(θ)
  y' = x * sin(θ) + y * cos(θ)
```

These can be combined into a single 3x3 transformation matrix (using
homogeneous coordinates), but that's a topic for a later module.

## Exercises

| File    | Description                                           | Tests |
|---------|-------------------------------------------------------|-------|
| ppm.c   | PPM image creation, pixel ops, Bresenham, rectangles  | 14    |

## Key Takeaways

1. Images are just arrays of bytes — 3 bytes per pixel (R, G, B)
2. PPM is the simplest image format: a text header followed by raw pixel data
3. Bresenham's line algorithm draws lines using only integer math
4. Always check bounds before writing pixels — buffer overflows corrupt memory
5. The pixel at (x, y) lives at byte offset `(y * width + x) * 3`
