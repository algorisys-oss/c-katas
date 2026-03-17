# Module 10 — Function Pointers & Callbacks

Every variable has an address. Your code lives in memory too — every function
has an address. A **function pointer** stores that address, letting you choose
*which* function to call at runtime instead of hardcoding it.

This is one of C's most powerful features. It's how `qsort` works, how event
systems work, how plugins work, and how C programs achieve polymorphism without
classes.

---

## Functions Have Addresses

When you compile your program, each function's machine code is placed at some
address in memory:

```
Memory:
┌──────────────────────┐
│ 0x00401000: add()    │  ← function code lives here
│   mov, add, ret...   │
├──────────────────────┤
│ 0x00401040: mul()    │  ← and here
│   mov, imul, ret...  │
├──────────────────────┤
│ 0x00401080: main()   │
│   ...                │
└──────────────────────┘
```

You can get a function's address the same way you get a variable's address:

```c
int add(int a, int b) { return a + b; }

printf("add lives at address %p\n", (void *)add);
/* prints something like: add lives at address 0x401000 */
```

---

## Function Pointer Syntax

To declare a variable that stores a function's address:

```c
int (*fp)(int, int);
```

Reading this declaration:
- `fp` is the name
- `*` means it's a pointer
- `(int, int)` is the parameter list
- `int` (at the start) is the return type

So `fp` is a "pointer to a function that takes two ints and returns an int."

```c
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

int (*fp)(int, int);    /* declare a function pointer */

fp = add;               /* point to the add function */
int result = fp(3, 4);  /* call through the pointer → 7 */

fp = mul;               /* now point to mul instead */
result = fp(3, 4);      /* call through the pointer → 12 */
```

The key insight: `fp` can point to **any** function with the matching signature
(two ints in, one int out). You can change which function gets called at runtime.

### Why the Syntax Is Ugly

C declaration syntax reads "inside-out." The parentheses around `*fp` are
essential:

```c
int (*fp)(int, int);    /* fp is a POINTER to a function */
int  *fp (int, int);    /* fp is a FUNCTION that returns int* (different!) */
```

The parentheses bind `*` to `fp` before the function call operator `()`.

---

## typedef — Making Function Pointers Readable

The syntax is hard to read. `typedef` fixes this:

```c
/* Define a type: "BinaryOp" is a pointer to a function (int, int) → int */
typedef int (*BinaryOp)(int, int);

/* Now use it like any other type */
BinaryOp op = add;
int result = op(3, 4);    /* 7 */

/* Function that takes a function pointer as argument */
int apply(BinaryOp fn, int a, int b)
{
    return fn(a, b);
}

apply(add, 3, 4);    /* 7 */
apply(mul, 3, 4);    /* 12 */
```

Always use `typedef` for function pointers. Your future self will thank you.

---

## The Callback Pattern

A **callback** is a function you pass to another function, saying "call this
when you need to make a decision."

The most famous example: `qsort` from the C standard library.

```c
#include <stdlib.h>

/* qsort needs you to provide a comparison function:
   - Returns negative if a < b
   - Returns 0 if a == b
   - Returns positive if a > b */

int compare_ints(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ia - ib;
}

int arr[] = {5, 2, 8, 1, 9, 3};
qsort(arr, 6, sizeof(int), compare_ints);
/* arr is now {1, 2, 3, 5, 8, 9} */
```

`qsort` doesn't know what type of data it's sorting — it works with `void *`.
**You** provide the comparison logic through a callback. This is how one
function can sort ints, strings, structs, or anything else.

### The void* Pattern

`void *` is a generic pointer — it can point to any type. Combined with function
pointers, it enables **generic programming** in C:

```
┌───────────────────────────────────────────────────────────────┐
│                    qsort (generic)                            │
│                                                               │
│  Input: void *base     ← any array                           │
│         size_t nmemb   ← number of elements                  │
│         size_t size    ← size of each element                 │
│         int (*cmp)()   ← YOU provide comparison logic         │
│                                                               │
│  qsort doesn't know the type — the callback does!            │
└───────────────────────────────────────────────────────────────┘
```

---

## Building Generic Operations: map, filter, reduce

These higher-order patterns work on any array type:

### map — Apply a function to every element

```c
typedef void (*MapFn)(void *element);

void array_map(void *arr, int count, int elem_size, MapFn fn)
{
    for (int i = 0; i < count; i++) {
        fn((char *)arr + i * elem_size);
    }
}

/* Usage: double every int */
void double_int(void *elem)
{
    *(int *)elem *= 2;
}

int nums[] = {1, 2, 3, 4, 5};
array_map(nums, 5, sizeof(int), double_int);
/* nums is now {2, 4, 6, 8, 10} */
```

### filter — Keep elements that pass a test

```c
typedef int (*Predicate)(const void *element);

int array_filter(void *arr, int count, int elem_size, Predicate pred)
{
    int write = 0;
    for (int i = 0; i < count; i++) {
        void *elem = (char *)arr + i * elem_size;
        if (pred(elem)) {
            if (write != i) {
                memcpy((char *)arr + write * elem_size, elem, elem_size);
            }
            write++;
        }
    }
    return write;    /* new count */
}
```

### reduce — Combine all elements into one value

```c
typedef void (*ReduceFn)(void *accumulator, const void *element);

void array_reduce(const void *arr, int count, int elem_size,
                  void *initial, ReduceFn fn)
{
    for (int i = 0; i < count; i++) {
        fn(initial, (const char *)arr + i * elem_size);
    }
}
```

---

## Concept: Polymorphism in C

In object-oriented languages, different types can respond to the same method
call differently. C achieves this with function pointers in structs:

```c
typedef struct {
    const char *name;
    float (*area)(void *self);
    void (*describe)(void *self);
} Shape;

typedef struct {
    Shape base;    /* "inherits" from Shape */
    float radius;
} Circle;

typedef struct {
    Shape base;
    float width, height;
} Rectangle;

float circle_area(void *self) {
    Circle *c = (Circle *)self;
    return 3.14159f * c->radius * c->radius;
}

float rect_area(void *self) {
    Rectangle *r = (Rectangle *)self;
    return r->width * r->height;
}
```

Both `Circle` and `Rectangle` have an `area` function pointer — calling
`shape->base.area(shape)` does the right thing for each type. This is the
C equivalent of virtual methods.

---

## Concept: Higher-Order Functions

A **higher-order function** is a function that takes a function as an argument
or returns a function. You've already seen examples:

- `qsort` takes a comparator function
- `array_map` takes a transform function
- `array_filter` takes a predicate function

This concept appears in every programming language (JavaScript's `.map()`,
Python's `sorted(key=...)`, Rust's `.iter().filter()`). Learning it in C gives
you the deepest understanding of how it works — there's no magic, just function
pointers.

---

## Event Systems — A Real-World Callback Pattern

Event systems let you register callback functions that fire when something
happens:

```
┌─────────────────────────────────────────────────────┐
│                   Event System                      │
│                                                     │
│  Events:                                            │
│  ┌──────────┬───────────────────────────────┐       │
│  │ "click"  │ [handler_a, handler_b]        │       │
│  ├──────────┼───────────────────────────────┤       │
│  │ "keydown"│ [handler_c]                   │       │
│  ├──────────┼───────────────────────────────┤       │
│  │ "quit"   │ [handler_d, handler_e]        │       │
│  └──────────┴───────────────────────────────┘       │
│                                                     │
│  on("click", handler_a)  → register                 │
│  emit("click")           → calls handler_a, b       │
└─────────────────────────────────────────────────────┘
```

---

## Exercises

You have three katas for this module:

1. **callbacks.c** — Build an event system where you can register callback
   functions for named events and fire all callbacks when an event occurs.

2. **generic_sort.c** — Implement a generic sort function (like qsort) that
   works with any data type through a comparator callback. Sort ints, strings,
   and structs using the same function.

3. **command_dispatch.c** — Build a command dispatcher that maps string commands
   to handler functions. Type a command name, it runs the matching function.

Compile and test:
```bash
make exercises    # build your code
make test         # build and run solutions to verify
```
