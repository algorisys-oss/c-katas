# Module 28c — Ray Tracing & 3D Graphics

## What Is Ray Tracing?

Ray tracing is a technique for generating realistic images by simulating how light
travels. Instead of tracing light from the source (which would waste most rays that
never reach the camera), we trace rays **backwards** — from the camera, through each
pixel on the screen, and into the scene. When a ray hits an object, we compute the
color at that point based on lighting.

This is the core insight: **one ray per pixel, traced from the eye into the world.**

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

Substituting our ray equation `P(t) = O + t*D` into the sphere equation:

```
|O + t*D - C|^2 = r^2
```

Let `oc = O - C` (vector from sphere center to ray origin). Expanding:

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

The Phong reflection model combines three components:

### 1. Ambient Light
A constant minimum brightness so nothing is completely black:
```
ambient = ambient_strength * object_color
```

### 2. Diffuse (Lambertian) Shading
A matte surface scatters light equally in all directions. The brightness depends
on the angle between the surface normal and the direction to the light source.

```
light_dir = normalize(light_position - hit_point)
diffuse = max(0, dot(normal, light_dir)) * object_color
```

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
