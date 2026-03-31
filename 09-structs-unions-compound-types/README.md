# Module 09 — Structs, Unions & Compound Types

Welcome to Phase 3! You now know how to store individual values, use pointers,
and manage memory. But real programs work with **related data** — a student has a
name, an age, and a grade; a pixel has red, green, and blue values; a network
packet has a header and a payload.

Keeping related data in separate variables is messy and error-prone. **Structs**
let you group related variables into a single named type. This is one of the most
important features in C — it's how you build real data structures.

---

## The Problem: Scattered Data

Without structs, related data lives in separate variables:

```c
/* A student's data — scattered across 3 variables */
char name1[50] = "Alice";
int age1 = 17;
float gpa1 = 3.8;

char name2[50] = "Bob";
int age2 = 18;
float gpa2 = 3.5;

/* What if we have 100 students? 300 variables? */
/* What if we want to pass a "student" to a function? Pass 3 arguments? */
```

This doesn't scale. We need a way to bundle related data together.

---

## Structs — Grouping Related Data

A **struct** (short for "structure") lets you define a new type that groups
multiple variables together:

```c
struct student {
    char name[50];
    int age;
    float gpa;
};
```

Now `struct student` is a type, just like `int` or `char`. You can create
variables of this type:

```c
struct student alice;
alice.age = 17;
alice.gpa = 3.8;
strcpy(alice.name, "Alice");    /* can't use = for arrays, use strcpy */

printf("%s is %d years old with GPA %.1f\n", alice.name, alice.age, alice.gpa);
```

The **dot operator** (`.`) accesses a field inside a struct.

### Initializing Structs

Several ways to initialize:

```c
/* Method 1: field by field */
struct student s;
strcpy(s.name, "Alice");
s.age = 17;
s.gpa = 3.8;

/* Method 2: initializer list (must match field order) */
struct student s = {"Alice", 17, 3.8};

/* Method 3: designated initializers (C99) — can be in any order */
struct student s = {.name = "Alice", .gpa = 3.8, .age = 17};

/* Method 4: zero-initialize everything */
struct student s = {0};    /* all bytes zero: name="", age=0, gpa=0.0 */
```

Designated initializers (Method 3) are the clearest — you can see which value
goes to which field. They're also safe if you add fields later.

---

## Struct Memory Layout & Padding

Here's where it gets interesting. Consider this struct:

```c
struct example {
    char a;     /* 1 byte */
    int b;      /* 4 bytes */
    char c;     /* 1 byte */
};

printf("sizeof = %zu\n", sizeof(struct example));
/* You might expect 6 (1 + 4 + 1). But it's 12! */
```

Why? **Alignment**. The CPU accesses 4-byte values most efficiently when they're
at addresses divisible by 4. The compiler inserts invisible **padding** bytes to
ensure this:

```
Byte:    0    1    2    3    4    5    6    7    8    9   10   11
       ┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
       │ a  │pad │pad │pad │ b  │ b  │ b  │ b  │ c  │pad │pad │pad │
       └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
        char  (3 bytes     int (at offset 4,   char  (3 bytes
              padding)      aligned!)                 tail padding)

Total: 12 bytes (not 6!)
```

The rules:
1. Each field is placed at an offset that's a multiple of its alignment
   (int needs offset divisible by 4, double needs offset divisible by 8)
2. The struct's total size is a multiple of its largest field's alignment
   (so arrays of structs keep everything aligned)

### Reordering Fields Saves Memory

If you put the larger fields first, you can avoid wasted padding:

```c
/* BAD: 12 bytes (lots of padding) */
struct bad {
    char a;     /* 1 + 3 padding */
    int b;      /* 4 */
    char c;     /* 1 + 3 padding */
};              /* Total: 12 */

/* GOOD: 8 bytes (minimal padding) */
struct good {
    int b;      /* 4 */
    char a;     /* 1 */
    char c;     /* 1 + 2 padding */
};              /* Total: 8 */
```

```
bad layout (12 bytes):
┌────┬───────┬─────────────┬────┬───────┐
│ a  │ pad×3 │     b       │ c  │ pad×3 │
└────┴───────┴─────────────┴────┴───────┘

good layout (8 bytes):
┌─────────────┬────┬────┬───────┐
│     b       │ a  │ c  │ pad×2 │
└─────────────┴────┴────┴───────┘
```

Rule of thumb: **order fields from largest to smallest** to minimize padding.

---

## Pointers to Structs & the Arrow Operator

When you have a pointer to a struct, you use the **arrow operator** (`->`)
to access fields:

```c
struct student alice = {"Alice", 17, 3.8};
struct student *p = &alice;

/* These two are equivalent: */
printf("%s\n", (*p).name);    /* dereference, then access field */
printf("%s\n", p->name);      /* shorthand — much cleaner */
```

`p->name` is syntactic sugar for `(*p).name`. The parentheses in `(*p).name`
are necessary because `.` has higher precedence than `*`.

### Passing Structs to Functions

By default, C copies the entire struct when you pass it:

```c
/* Copies the whole struct — slow for large structs! */
void print_student(struct student s)
{
    printf("%s: age %d, gpa %.1f\n", s.name, s.age, s.gpa);
}

/* Better: pass a pointer — only copies 8 bytes (the pointer) */
void print_student(const struct student *s)
{
    printf("%s: age %d, gpa %.1f\n", s->name, s->age, s->gpa);
}
```

Use `const` when the function doesn't need to modify the struct — this tells
the reader (and the compiler) that the data is read-only.

---

## typedef — Giving Types Shorter Names

Writing `struct student` everywhere is verbose. `typedef` creates an alias:

```c
typedef struct {
    char name[50];
    int age;
    float gpa;
} Student;

/* Now you can use Student instead of struct student */
Student alice = {"Alice", 17, 3.8};
```

This is the pattern you've already seen in Modules 07 and 08 (StringBuilder,
DynArray, Arena, Pool). Now you know the full story.

---

## Enums — Named Integer Constants

An `enum` defines a set of named integer values:

```c
enum color {
    COLOR_RED,      /* 0 */
    COLOR_GREEN,    /* 1 */
    COLOR_BLUE      /* 2 */
};
```

By default, values start at 0 and increment. You can set explicit values:

```c
enum http_status {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_SERVER_ERROR = 500
};
```

Enums make code readable — `COLOR_RED` is clearer than `0`.

```c
enum color pixel = COLOR_GREEN;
if (pixel == COLOR_RED) {
    /* ... */
}
```

Under the hood, enums are just `int` values. C doesn't prevent you from
assigning out-of-range values — it's your responsibility to use them correctly.

---

## Unions — Shared Memory

A **union** looks like a struct, but all fields **share the same memory**.
Only one field is valid at a time.

```c
union value {
    int i;
    float f;
    char s[20];
};

printf("sizeof = %zu\n", sizeof(union value));  /* 20 — size of largest field */
```

```
Struct layout (all fields get their own space):
┌───────┬───────┬────────────────────┐
│ int i │float f│    char s[20]      │
└───────┴───────┴────────────────────┘
 0       4       8                   28   (28 bytes)

Union layout (all fields share the same space):
┌────────────────────┐
│ int i              │
│ float f            │  ← all three occupy the same 20 bytes
│ char s[20]         │
└────────────────────┘
 0                   20   (20 bytes)
```

Writing to one field and reading another is **undefined behavior** in most cases
(though it's commonly used for type punning on some compilers).

### Tagged Unions — The Safe Pattern

Since a union doesn't track which field is active, you pair it with an enum
**tag** that says which field is currently valid:

```c
enum value_type { VAL_INT, VAL_FLOAT, VAL_STRING };

typedef struct {
    enum value_type type;    /* the tag — says which union field is valid */
    union {
        int i;
        float f;
        char s[32];
    } data;                  /* the union — holds the actual value */
} Value;

/* Create an int value */
Value v;
v.type = VAL_INT;
v.data.i = 42;

/* Later, check the tag before reading */
switch (v.type) {
    case VAL_INT:    printf("int: %d\n", v.data.i);    break;
    case VAL_FLOAT:  printf("float: %f\n", v.data.f);  break;
    case VAL_STRING: printf("string: %s\n", v.data.s);  break;
}
```

Tagged unions are incredibly useful. They appear in:
- JSON parsers (value can be number, string, array, object, bool, null)
- AST nodes in compilers (each node type has different fields)
- Network protocols (different message types in the same format)
- Configuration values (different types of settings)

---

## Opaque Types — Hiding Implementation Details

Sometimes you want users to work with a type **without seeing its internals**.
This is how you build libraries in C.

The idea: put only a pointer declaration in the header file. Put the actual
struct definition in the `.c` file. Users can only work with pointers — they
can't directly access fields.

```c
/* stack.h — the public interface */
typedef struct Stack Stack;    /* forward declaration — no fields visible */

Stack *stack_create(void);
void stack_push(Stack *s, int value);
int stack_pop(Stack *s);
void stack_destroy(Stack *s);

/* stack.c — the private implementation */
#include "stack.h"

struct Stack {       /* actual definition — only visible in this file */
    int *data;
    int size;
    int capacity;
};

Stack *stack_create(void) { /* ... */ }
/* etc. */
```

Users of `stack.h` can call `stack_push` and `stack_pop`, but they **cannot**
access `s->data` or `s->size` directly — the compiler doesn't know the struct's
layout from just the header.

This pattern is called **encapsulation**: hiding internal details behind an
interface. It's the foundation of good library design in C.

---

## Concept: Abstract Data Types (ADTs)

An ADT separates **what** something does from **how** it does it:

```
┌─────────────────────────────────────────────────────────────┐
│                    The Interface (header)                     │
│                                                              │
│  Stack *stack_create(void);                                  │
│  void   stack_push(Stack *s, int value);                     │
│  int    stack_pop(Stack *s);                                 │
│  void   stack_destroy(Stack *s);                             │
│                                                              │
│  User sees: WHAT it does                                     │
├──────────────────────────────────────────────────────────────┤
│                The Implementation (.c file)                   │
│                                                              │
│  Could use: array, linked list, tree, whatever               │
│  User doesn't know and doesn't care                          │
│                                                              │
│  Developer controls: HOW it works                            │
└─────────────────────────────────────────────────────────────┘
```

You can change the implementation without changing any code that uses the ADT.
This is one of the most powerful ideas in software engineering.

---

## Nested Structs

Structs can contain other structs:

```c
typedef struct {
    float x, y;
} Point;

typedef struct {
    Point position;
    Point velocity;
    float mass;
} Particle;

Particle p = {
    .position = {10.0f, 20.0f},
    .velocity = {1.0f, -0.5f},
    .mass = 5.0f
};

printf("x = %.1f\n", p.position.x);    /* 10.0 */
```

---

## Exercises

You have three katas for this module:

1. **structs.c** — Build a student record system with CRUD operations
   (create, read, update, delete). Practice struct initialization, passing by
   pointer, and array-of-structs patterns.

2. **tagged_union.c** — Build a variant type that can hold an int, a float, or
   a string. Practice tagged unions, switch statements, and safe type handling.

3. **point.c** — Build a 2D point/vector library with add, subtract, scale,
   distance, and dot product. Practice math with structs and `const` pointers.

Compile and test:
```bash
make exercises    # build your code
make test         # build and run solutions to verify
```

---

[← Previous: Module 08 — Mastering Memory Management](../08-mastering-memory-management/README.md) | [Next: Module 10 — Function Pointers & Callbacks →](../10-function-pointers-callbacks/README.md)
