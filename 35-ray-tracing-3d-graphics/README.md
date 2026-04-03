# Module 28c — Ray Tracing & 3D Graphics

## What Is Ray Tracing?

Ray tracing is a technique for generating realistic images by simulating how light
travels. Instead of tracing light from the source (which would waste most rays that
never reach the camera), we trace rays **backwards** — from the camera, through each
pixel on the screen, and into the scene. When a ray hits an object, we compute the
color at that point based on lighting.

This is the core insight: **one ray per pixel, traced from the eye into the world.**

## Vector Math Primer

Before we start, you need to understand **vectors**. A vector represents a
direction and magnitude — think "3 meters north-east." In 3D graphics, a
vector has three components: `(x, y, z)`.

**Key operations:**

- **Add/Subtract**: component-wise. `(1,2,3) + (4,5,6) = (5,7,9)`
- **Scale**: multiply each component by a number. `2 * (1,2,3) = (2,4,6)`
- **Length**: `|v| = sqrt(x^2 + y^2 + z^2)`. Example: `|(3,4,0)| = 5`
- **Normalize**: divide each component by the length to get a **unit vector**
  (length = 1). This keeps the direction but discards the magnitude.
  `normalize(3,4,0) = (0.6, 0.8, 0.0)`

**Dot product** — the most important operation in graphics:

```
  a . b = ax*bx + ay*by + az*bz       (result is a single number!)
```

The key insight: `a . b = |a| * |b| * cos(angle)`. If both vectors are
normalized (length 1), the dot product IS the cosine of the angle between
them. This is crucial for lighting — it tells you how much a surface faces
the light.

```
  a . b = 1.0   -> vectors point the same direction (angle = 0)
  a . b = 0.0   -> vectors are perpendicular (angle = 90)
  a . b = -1.0  -> vectors point opposite directions (angle = 180)

  Example with unit vectors:

       b          a . b = cos(45) ~ 0.71
      /           The surface partially faces
     / 45 deg     the light -> moderate brightness
    /-----> a
```

**Cross product**: produces a vector **perpendicular** to both inputs.
`a x b = (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)`. Used to find
surface normals from two edges of a triangle.

## The Ray

A ray is a half-line defined by an **origin** (a point in 3D space) and a
**direction** (a unit vector):

```
P(t) = origin + t * direction      (t >= 0)
```

As `t` increases from 0, the point `P(t)` moves along the ray away from the origin.

```
        direction
 origin --------->
   O . . . . . . . . . . . . >
   t=0   t=1   t=2   t=3
```

## Camera Model: Eye, Screen, Scene

The camera sits at a point in space (the "eye"). In front of it is an imaginary
rectangular screen (the "image plane"). For each pixel on this screen, we shoot a
ray from the eye through the pixel center and see what it hits.

```
                    Scene
                   .-------.
                  /  Sphere  \
                 |     @      |
                  \          /
    Screen         '-------'
   +--------+        /
   |  |  |  |       /
   |--+--+--|      /         <-- Ray through pixel
   |  |  |  |     /
   |--+--+--|    /
   |  |  |  |  /
   +--------+ /
              /
           Eye
            O
```

Each pixel's color is determined by what the ray hits (or the background color if
it hits nothing).

## Ray-Sphere Intersection

A sphere is defined by its **center** `C` and **radius** `r`. A point `P` is on
the sphere if:

```
|P - C|^2 = r^2
```

We substitute the ray equation `P(t) = O + t*D` into the sphere equation
and expand step by step:

```
|P - C|^2 = r^2                     <- sphere equation
|O + t*D - C|^2 = r^2               <- substitute P = O + t*D

Let oc = O - C (vector from sphere center to ray origin):

|oc + t*D|^2 = r^2                  <- rewrite O - C as oc
(oc + t*D) . (oc + t*D) = r^2       <- |v|^2 = v . v (definition of length)

Expand the dot product (just like (a+b)^2 = a^2 + 2ab + b^2):

t^2*(D . D) + 2*t*(oc . D) + (oc . oc) = r^2

Move r^2 to the left:

(D . D)*t^2 + 2*(oc . D)*t + (oc . oc - r^2) = 0
```

This is a **quadratic equation** in `t`:

```
(D . D) * t^2  +  2*(oc . D) * t  +  (oc . oc - r^2)  =  0
```

This is a **quadratic equation** `at^2 + bt + c = 0` where:
- `a = dot(D, D)` — always positive (direction dotted with itself)
- `b = 2 * dot(oc, D)`
- `c = dot(oc, oc) - r^2`

The discriminant `disc = b^2 - 4ac` tells us:
- `disc < 0` — ray **misses** the sphere (no real solutions)
- `disc = 0` — ray **grazes** the sphere (one tangent point)
- `disc > 0` — ray **pierces** the sphere (two intersection points)

```
    Miss (disc < 0)       Graze (disc = 0)      Hit (disc > 0)
         ___                    ___                   ___
        /   \                  /   \                 /   \
  ---> |     |           ---->|     |           --->/ t1  \---> t2
        \___/                  \___/                \_____/
                                 t
```

When we have a hit, the nearest intersection is:

```
t = (-b - sqrt(disc)) / (2a)
```

We only care about `t > 0` (intersections in front of the camera, not behind).

## Surface Normal

The **normal** at a point `P` on a sphere centered at `C` is the unit vector
pointing outward:

```
normal = normalize(P - C)
```

```
              normal
                ^
                |
         .------+------.
        /       |       \
       |        @ C      |    <-- P is on the surface
        \               /         normal points outward from C to P
         '-------------'
```

The normal is crucial for lighting — it tells us which direction the surface is
"facing" at the hit point.

## Lighting: The Phong Model

The Phong reflection model combines three components to approximate how
real surfaces look. Each component models a different physical phenomenon:

### 1. Ambient Light
A constant minimum brightness so nothing is completely black:
```
ambient = ambient_strength * object_color
```

**Why do we need this?** Without ambient light, any surface facing away from
the light would be completely black. In real life, light bounces around
rooms (indirect illumination) — a wall facing away from a lamp is dim but
not pitch black. Computing all those bounces is extremely expensive.
Ambient light is a cheap approximation: just add a small constant brightness
everywhere.

### 2. Diffuse (Lambertian) Shading
A matte surface scatters light equally in all directions. The brightness depends
on the angle between the surface normal and the direction to the light source.

```
light_dir = normalize(light_position - hit_point)
diffuse = max(0, dot(normal, light_dir)) * object_color
```

**Why the dot product?** When a surface directly faces the light (normal
parallel to light direction), it receives maximum illumination. When tilted
at an angle, the same light is spread across a larger area, so each point
gets less. The dot product `cos(angle)` gives exactly this falloff.

**Why `max(0, ...)`?** If the dot product is negative, the light is behind
the surface. Negative light makes no physical sense, so we clamp to 0.

When `dot(normal, light_dir)` is positive, the surface faces the light. When
negative or zero, the surface faces away and receives no diffuse light.

```
     Light
       *
      /
     / light_dir
    /
   v
   ----+---- surface         dot(N, L) = cos(angle)
        \                    = 1.0 when N and L are parallel (brightest)
         \ N (normal)        = 0.0 when perpendicular (no light)
          v                  < 0.0 when facing away (clamped to 0)
```

### 3. Specular (Shiny Highlight)
A bright spot where you'd see the reflection of the light source. Uses the
**reflect** vector:

```
reflect_dir = reflect(-light_dir, normal)
spec = pow(max(0, dot(reflect_dir, view_dir)), shininess)
specular = spec * light_color
```

**Why a separate specular term?** Shiny surfaces (metal, plastic, wet
surfaces) reflect light like a mirror. The brightness depends on whether
the reflection direction aligns with your eye direction — not just the
surface-to-light angle. The `shininess` exponent controls how mirror-like
the surface is: a low value (e.g., 8) gives a broad, soft highlight (like
plastic), while a high value (e.g., 256) gives a tiny, sharp highlight
(like polished metal). Again, `max(0, ...)` clamps negative values because
light behind the surface contributes nothing.

Higher `shininess` = smaller, sharper highlight.

### Combined
```
final_color = ambient + diffuse + specular
```

## Shadows

To check if a point is in shadow, shoot a **shadow ray** from the hit point
toward the light source. If it hits any object before reaching the light, the
point is in shadow and receives only ambient light.

```
     Light
       *
      /|
     / |
    /  |  Shadow ray blocked!
   /   |
  / Blocker
 /  [===]
/     |
      |   <-- This point is in shadow
  ----+---- surface
```

## Reflections

When a ray hits a reflective surface, compute the **reflection direction** and
shoot a new ray. The reflected ray's color is blended with the surface's own
color.

The reflection formula:
```
R = V - 2 * dot(V, N) * N
```

Where `V` is the incoming ray direction and `N` is the surface normal.

```
         incoming V        reflected R
            \              /
             \     N      /
              \    ^     /
               \   |    /
                \  |   /
                 \ |  /
    ==============\|/=============== surface
```

The angle of incidence equals the angle of reflection (just like a mirror).

## Exercises

1. **`exercises/vec3.c`** — Build a 3D vector math library: create, add, subtract,
   scale, dot product, cross product, length, normalize, reflect.

2. **`exercises/raytracer.c`** — Implement ray-sphere intersection (solving the
   quadratic) and diffuse shading. The building blocks for a full ray tracer.

## Building

```bash
make exercises    # Build student exercises
make solutions    # Build reference solutions
make test         # Build and run all solution tests
```

---

[← Previous: Module 28b — Image Manipulation & Effects](../28b-image-manipulation-effects/README.md) | [Next: Module 29: Machine Learning in C →](../29-machine-learning-in-c/README.md)
