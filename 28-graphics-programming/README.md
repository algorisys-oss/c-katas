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

#### The Algorithm (All Octants, Single Loop)

```
  function draw_line(x0, y0, x1, y1):
      dx = abs(x1 - x0)
      dy = -abs(y1 - y0)        <- note: dy is negative!
      sx = (x0 < x1) ? 1 : -1   <- step direction in x
      sy = (y0 < y1) ? 1 : -1   <- step direction in y
      error = dx + dy            <- initial error term

      loop forever:
          plot(x0, y0)           <- draw this pixel
          if x0 == x1 AND y0 == y1: break   <- reached the end

          e2 = 2 * error
          if e2 >= dy:           <- step in x
              error += dy
              x0 += sx
          if e2 <= dx:           <- step in y
              error += dx
              y0 += sy
```

Why does this work? The `error` variable tracks how far the ideal line has
drifted from the pixels we've drawn. When the error accumulates enough in
one direction, we take a step in that direction. The `e2 = 2 * error` trick
avoids floating point entirely — we just compare integers.

#### Step-by-Step Trace

Let's trace a line from (0,0) to (4,2):

```
  dx = abs(4-0) = 4
  dy = -abs(2-0) = -2
  sx = 1 (moving right)
  sy = 1 (moving down)
  error = 4 + (-2) = 2

  Step   x   y   error  e2   e2>=dy(-2)?  e2<=dx(4)?  Action
  ----   --  --  -----  --   -----------  ----------  ------
  init   0   0    2      -       -            -       plot(0,0)
    1    -   -    -      4      yes          no       error+=dy -> 0, x+=1
         1   0    0      -       -            -       plot(1,0)
    2    -   -    -      0      yes          yes      error+=dy -> -2, x+=1
                                                      error+=dx -> 2, y+=1
         2   1    2      -       -            -       plot(2,1)
    3    -   -    -      4      yes          no       error+=dy -> 0, x+=1
         3   1    0      -       -            -       plot(3,1)
    4    -   -    -      0      yes          yes      error+=dy -> -2, x+=1
                                                      error+=dx -> 2, y+=1
         4   2    2      -       -            -       plot(4,2)
         x0==x1 AND y0==y1 -> break!

  Result on the grid:

     0   1   2   3   4
  0  *   *   .   .   .
  1  .   .   *   *   .
  2  .   .   .   .   *

  5 pixels drawn for a line from (0,0) to (4,2).
```

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

**Why y-down instead of the mathematical y-up convention?** Because of how
screens and image files work: pixels are stored row by row, starting from
the top-left. The first byte in a PPM file is the top-left pixel. When you
scan through memory sequentially, you're moving left-to-right, then
top-to-bottom — exactly matching the y-down convention. Using y-up would
mean the first pixel in memory is at the BOTTOM of the image, which is
confusing.

## From Pixels to Drawing Applications

So far we've drawn static images — set pixels, save a PPM file, done. But real
drawing applications (like sketching tools, mindmapping apps, or diagramming
tools) are **interactive**. Understanding the concepts below will let you build
applications where users draw, select, move, and resize shapes in real time.

These concepts are language-agnostic — the same algorithms power drawing apps
whether built in C, JavaScript, Rust, or any other language.

### The Render Loop

Interactive graphics programs use a **render loop**: clear the screen, draw
everything, wait for input, repeat. This runs fast enough (60 times per second)
that the user sees smooth, continuous motion.

```
  The Render Loop:
  ┌─────────────────────────────────────────────┐
  │                                             │
  │   ┌──────────┐   ┌──────────┐   ┌────────┐│
  │   │ Clear    │──→│ Draw all │──→│ Wait   ││
  │   │ screen   │   │ shapes   │   │ 16.7ms ││
  │   └──────────┘   └──────────┘   └───┬────┘│
  │                                     │      │
  │   ← ── ── ── ── ── ── ── ── ── ── ─┘      │
  └─────────────────────────────────────────────┘

  60 FPS = 60 frames per second = 16.7ms per frame
  If your drawing code takes longer than 16.7ms, animation stutters.
```

In C, you'd use a library like SDL2, raylib, or write directly to a
framebuffer. The loop looks like:

```c
while (!should_quit) {
    handle_input();           /* check mouse, keyboard */
    clear_framebuffer(fb);    /* erase everything */
    draw_all_shapes(fb);      /* redraw every shape */
    present_framebuffer(fb);  /* display to screen */
}
```

### Immediate Mode vs. Retained Mode

There are two approaches to drawing:

```
  Immediate Mode:                   Retained Mode:
  ┌────────────────────────┐        ┌────────────────────────┐
  │ You issue draw commands │        │ You create objects      │
  │ "draw rect at (10,20)" │        │ "add rect at (10,20)"  │
  │                        │        │                        │
  │ Screen has NO memory   │        │ System REMEMBERS them  │
  │ of what was drawn.     │        │ and redraws for you.   │
  │                        │        │                        │
  │ To move a shape:       │        │ To move a shape:       │
  │  1. Clear everything   │        │  1. Change its x,y     │
  │  2. Redraw everything  │        │  (system handles rest) │
  │  at new positions      │        │                        │
  └────────────────────────┘        └────────────────────────┘

  Analogy: painting on paper       Analogy: magnets on a whiteboard
  (you paint over mistakes)        (you pick up and move them)
```

Our PPM approach is immediate mode — we're painting pixels. For interactive
apps, you typically store a list of shape objects and redraw them each frame.

### Bounding Boxes and Hit Detection

Every shape, no matter how complex, has a **bounding box** — the smallest
rectangle that contains it. This is essential for selecting shapes with the
mouse.

```
  Even complex shapes have a bounding box:

  ┌─────────────┐         ┌─────────┐
  │     ★       │         │   /\    │
  │   ★   ★     │         │  /  \   │
  │ ★   ★   ★   │         │ /    \  │
  │   ★   ★     │         │/______\ │
  │     ★       │         │         │
  └─────────────┘         └─────────┘
  Star's bounding box      Triangle's bounding box
```

**Point-in-rectangle test** (was the click inside this shape?):

```c
bool point_in_rect(int px, int py, int rx, int ry, int rw, int rh)
{
    return px >= rx && px <= rx + rw &&
           py >= ry && py <= ry + rh;
}
```

**Point-in-circle test**:

```c
bool point_in_circle(int px, int py, int cx, int cy, int radius)
{
    int dx = px - cx;
    int dy = py - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}
```

### World Space vs. Screen Space

When you add panning (scrolling) and zooming, you need two coordinate systems:

```
  World Space (where shapes actually are — never changes):

  Shape A at (100, 200)
  Shape B at (500, 300)

  Screen Space (what the user sees — changes with pan/zoom):

  screenX = (worldX * zoom) + panX
  screenY = (worldY * zoom) + panY

  When the user clicks at screen position (250, 225), convert BACK
  to world coordinates to find which shape they clicked:

  worldX = (screenX - panX) / zoom
  worldY = (screenY - panY) / zoom
```

### Snapping

Align shapes to a grid or to other shapes for precise layouts:

```c
/* Snap a coordinate to the nearest grid line */
int snap_to_grid(int value, int grid_size)
{
    return ((value + grid_size / 2) / grid_size) * grid_size;
}

/* Example: snap_to_grid(137, 20) = 140 */
```

### Drawing Order (Z-Index)

Shapes drawn later appear on top. To change order, rearrange your shapes
array:

```
  Draw order:  Shape A first, then B

  ┌──────────┐
  │ A  ┌─────┤──────┐
  │    │  B  │      │    B appears on top because
  └────┤     │      │    it was drawn AFTER A
       └─────┴──────┘
```

### Undo/Redo with a History Stack

Save snapshots of your shapes after each action. Undo = go back one snapshot.

```
  History stack:
  ┌─────────┬─────────┬─────────┐
  │ State 0 │ State 1 │ State 2 │
  │ (empty) │ (+rect) │ (+circle)│
  └─────────┴─────────┴────↑────┘
                        current

  After undo:
  ┌─────────┬─────────┬─────────┐
  │ State 0 │ State 1 │ State 2 │
  └─────────┴────↑────┴─────────┘
              current (circle removed)

  New action after undo discards State 2:
  ┌─────────┬─────────┬─────────┐
  │ State 0 │ State 1 │ State 3 │
  └─────────┴─────────┴────↑────┘
                        current (+triangle)
```

### Where to Go from Here

These algorithms are universal — they power everything from simple sketch apps
to professional CAD tools. A suggested learning path:

1. Draw static shapes to a PPM file (this module)
2. Add a render loop with SDL2 or raylib
3. Handle mouse input — click to place shapes
4. Implement drag-to-draw (mouse down, drag, mouse up)
5. Add selection — click to select, show resize handles
6. Implement resize — drag handles to resize shapes
7. Add snapping — grid snap, then snap to other objects
8. Add pan and zoom — infinite canvas
9. Add undo/redo — history management
10. Add connectors — lines between shapes with anchor points

---

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

| File            | Description                                           | Tests |
|-----------------|-------------------------------------------------------|-------|
| `ppm.c`         | PPM image creation, pixel ops, Bresenham, rectangles  | 14    |
| `shapes.c`      | Circle (midpoint), filled circle, triangle, gradient  | 10    |
| `mandelbrot.c`  | Mandelbrot set iteration, coloring, rendering         | 8     |

## Key Takeaways

1. Images are just arrays of bytes — 3 bytes per pixel (R, G, B)
2. PPM is the simplest image format: a text header followed by raw pixel data
3. Bresenham's line algorithm draws lines using only integer math
4. Always check bounds before writing pixels — buffer overflows corrupt memory
5. The pixel at (x, y) lives at byte offset `(y * width + x) * 3`

---

[← Previous: Module 27: Encryption & Security](../27-encryption-security/README.md) | [Next: Module 28b — Image Manipulation & Effects →](../28b-image-manipulation-effects/README.md)
