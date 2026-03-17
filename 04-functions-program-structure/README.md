# Module 04 — Functions & Program Structure

So far every line of your code has lived inside `main()`. That is fine for small
programs, but as they grow you will find yourself copying and pasting the same
logic, losing track of what does what, and struggling to test anything in
isolation. Functions fix all of that.

By the end of this module you will be able to:
- Write your own functions with parameters and return values
- Explain pass-by-value and why C copies arguments
- Draw the call stack and trace function calls by hand
- Understand scope: local, global, `static`, `extern`
- Split code across multiple files using header files
- Compile and link multi-file programs

---

## 1. Why Functions?

A **function** is a named block of code that takes inputs, does some work, and
(optionally) produces an output. You already use functions every day:

```c
printf("hello\n");   /* printf is a function */
sqrt(16.0);          /* sqrt is a function */
```

Three reasons to write your own:

1. **Avoid repetition** — write the logic once, call it many times
2. **Organize code** — give each piece a name that says what it does
3. **Make it testable** — you can call a function with known inputs and check
   its output, without running the whole program

---

## 2. Declaration vs. Definition

A function **declaration** (also called a **prototype**) tells the compiler:
"this function exists, here is its name, what it takes, and what it returns."

```c
int add(int a, int b);    /* declaration — no body, just a semicolon */
```

A function **definition** provides the actual code:

```c
int add(int a, int b)     /* definition — has a body */
{
    return a + b;
}
```

Why the split? Because the compiler reads your file **top to bottom**. If you
call `add()` before the compiler has seen it, it does not know what `add` is.
A declaration at the top fixes that:

```c
#include <stdio.h>

int add(int a, int b);    /* declaration — compiler now knows about add */

int main(void)
{
    printf("%d\n", add(3, 4));   /* OK, compiler knows add's signature */
    return 0;
}

int add(int a, int b)     /* definition — the actual code */
{
    return a + b;
}
```

---

## 3. Parameters: Pass by Value

C passes arguments **by value**. This means the function gets a **copy** of the
value, not the original variable. Changes to the copy do not affect the caller.

```c
void try_to_change(int x)
{
    x = 99;    /* changes the LOCAL copy only */
}

int main(void)
{
    int num = 5;
    try_to_change(num);
    printf("%d\n", num);    /* still 5! */
    return 0;
}
```

### Pass-by-Value Diagram

```
    main()                        try_to_change()
    +-----------+                 +-----------+
    | num = 5   |----copy---->    | x = 5     |
    +-----------+                 +-----------+
                                        |
                                   x = 99  (only x changes)
                                        |
    +-----------+                 +-----------+
    | num = 5   |  (unchanged)   | x = 99    |
    +-----------+                 +-----------+
```

The value 5 is **copied** into `x`. When `x` is changed to 99, `num` in
`main()` is untouched. This is why you cannot write a `swap(a, b)` function
that actually swaps two variables — both values are copies. (We will fix this
when we learn pointers.)

---

## 4. Return Values

A function produces **one** result using `return`:

```c
int square(int n)
{
    return n * n;
}
```

If a function does not produce a result, declare it `void`:

```c
void greet(void)
{
    printf("Hello!\n");
    /* no return value */
}
```

---

## 5. The Call Stack — From First Principles

When you call a function, the computer needs to remember where it was, what the
local variables were, and where to go back to when the function is done. It does
this using a region of memory called **the stack**.

### How It Works

1. Every function call **pushes a frame** onto the stack. A frame holds:
   - The function's local variables
   - The return address (where to go back to)
   - The arguments passed to the function

2. When a function returns, its frame is **popped** off the stack. All its local
   variables cease to exist.

3. The stack grows downward in memory (by convention on most systems).

### Example: Tracing the Call Stack

```c
int multiply(int a, int b)
{
    int result = a * b;
    return result;
}

int square(int n)
{
    return multiply(n, n);
}

int main(void)
{
    int x = square(5);
    printf("%d\n", x);
    return 0;
}
```

Let's trace the stack at each step:

```
Step 1: main() is called
+---------------------------+
|  main()                   |
|    x = ???  (not yet set) |
+---------------------------+  <-- top of stack

Step 2: main() calls square(5)
+---------------------------+
|  square()                 |
|    n = 5                  |
|    return addr -> main    |
+---------------------------+  <-- top of stack
|  main()                   |
|    x = ???                |
+---------------------------+

Step 3: square() calls multiply(5, 5)
+---------------------------+
|  multiply()               |
|    a = 5                  |
|    b = 5                  |
|    result = 25            |
|    return addr -> square  |
+---------------------------+  <-- top of stack
|  square()                 |
|    n = 5                  |
|    return addr -> main    |
+---------------------------+
|  main()                   |
|    x = ???                |
+---------------------------+

Step 4: multiply() returns 25 — its frame is POPPED
+---------------------------+
|  square()                 |
|    n = 5                  |
|    return addr -> main    |
|    (receives 25)          |
+---------------------------+  <-- top of stack
|  main()                   |
|    x = ???                |
+---------------------------+

Step 5: square() returns 25 — its frame is POPPED
+---------------------------+
|  main()                   |
|    x = 25                 |
+---------------------------+  <-- top of stack
```

**Key insight**: each function's local variables live **only** as long as their
frame is on the stack. When the function returns, those variables are gone.

**Draw it!** Literally draw boxes on paper for each call. This is the single best
way to understand function calls, and you will need this skill forever.

---

## 6. Scope — Where a Name Is Visible

**Scope** determines where in your code a variable name can be used.

### Scope Diagram

```
+------------------------------------------------------------------+
|  FILE LEVEL (global scope)                                       |
|  int global_count = 0;  <-- visible everywhere in this file      |
|                              (and other files if not static)     |
|  +------------------------------------------------------------+  |
|  |  FUNCTION: main()                                          |  |
|  |  int x = 10;  <-- visible only inside main                |  |
|  |                                                            |  |
|  |  +------------------------------------------------------+  |  |
|  |  |  BLOCK: if (x > 5)                                  |  |  |
|  |  |  int y = 20;  <-- visible only inside this block    |  |  |
|  |  |  /* can see: y, x, global_count */                  |  |  |
|  |  +------------------------------------------------------+  |  |
|  |                                                            |  |
|  |  /* can see: x, global_count.  CANNOT see: y */           |  |
|  +------------------------------------------------------------+  |
|                                                                  |
|  +------------------------------------------------------------+  |
|  |  FUNCTION: helper()                                        |  |
|  |  int z = 30;  <-- visible only inside helper              |  |
|  |  /* can see: z, global_count.  CANNOT see: x, y */        |  |
|  +------------------------------------------------------------+  |
+------------------------------------------------------------------+
```

### Four Kinds of Scope

**1. Local (block) scope** — born when the block is entered, dies when exited:

```c
void example(void)
{
    int a = 1;          /* lives for the whole function */
    if (a > 0) {
        int b = 2;      /* lives only inside this if-block */
        printf("%d\n", b);
    }
    /* b does not exist here — using it is an error */
}
```

**2. Global (file) scope** — lives for the entire program. Avoid these:

```c
int counter = 0;    /* global — any function can read/write it */

void increment(void) { counter++; }
void reset(void) { counter = 0; }
```

Globals make programs hard to reason about because any function anywhere can
change them. Prefer passing values as arguments.

**3. `static` local** — persists between calls but is only visible in its function:

```c
void count_calls(void)
{
    static int calls = 0;   /* initialized once, persists between calls */
    calls++;
    printf("Called %d times\n", calls);
}

int main(void)
{
    count_calls();   /* "Called 1 times" */
    count_calls();   /* "Called 2 times" */
    count_calls();   /* "Called 3 times" */
    return 0;
}
```

The `static` keyword here means "keep this variable alive even after the
function returns, but don't let anyone else see it."

**4. `extern`** — "this variable exists somewhere else, trust me":

```c
/* file_a.c */
int shared_value = 42;     /* definition — memory is allocated here */

/* file_b.c */
extern int shared_value;   /* declaration — no memory, just a promise */
```

You will see `extern` when we split code across multiple files.

---

## 7. Header Files — From First Principles

### The Problem

Suppose you write a useful function `add()` in file `math_lib.c`. Now you want
to use it in `main.c`. But `main.c` does not know that `add()` exists — the
compiler processes each file independently.

### The Solution

Put the **declaration** (prototype) in a `.h` (header) file. Put the
**definition** (code) in a `.c` file. Then `#include` the header wherever you
need to call the function.

### Header File Include Diagram

```
    math_lib.h                 math_lib.c              main.c
   +--------------+          +-----------------+     +-----------------+
   | /* declares */|         | #include         |     | #include         |
   | int add(     |         |   "math_lib.h"   |     |   "math_lib.h"   |
   |   int a,     |<--------|                  |     |                  |
   |   int b);    |         | /* defines */     |     | int main(void)  |
   |              |<--------|                  |---->|   add(3, 4);     |
   +--------------+         | int add(int a,   |     +-----------------+
                            |         int b)   |
                            | {                |
                            |   return a + b;  |
                            | }                |
                            +-----------------+
```

Both `.c` files include the same `.h` file:
- `math_lib.c` includes it so the compiler can verify its definition matches
  the declaration
- `main.c` includes it so the compiler knows `add()` exists and what it looks
  like

### `#include "file.h"` vs `#include <stdio.h>`

- `#include "file.h"` — searches the current directory first (your own headers)
- `#include <stdio.h>` — searches the system include paths (standard library)

### Include Guards

If two headers both include the same header, you get **duplicate declarations**.
Include guards prevent this:

```c
#ifndef MATH_LIB_H
#define MATH_LIB_H

int add(int a, int b);
int subtract(int a, int b);

#endif /* MATH_LIB_H */
```

The first time the preprocessor sees this file, `MATH_LIB_H` is not defined, so
it processes everything between `#ifndef` and `#endif`. It also defines
`MATH_LIB_H`. The second time, `MATH_LIB_H` IS defined, so the preprocessor
skips the entire block. The declarations appear only once.

---

## 8. Separate Compilation

Each `.c` file is compiled **independently** into an object file (`.o`). Then the
linker combines all object files into the final executable.

### Separate Compilation Diagram

```
    Source Files              Object Files           Executable
   +-------------+          +-------------+
   | math_lib.c  |--gcc -c->| math_lib.o  |---+
   +-------------+          +-------------+    |    +----------+
                                               +--->| program  |
   +-------------+          +-------------+    |    +----------+
   | main.c      |--gcc -c->| main.o      |---+
   +-------------+          +-------------+
                                             (linker)

   Step 1: Compile each .c file separately
      gcc -c math_lib.c -o math_lib.o
      gcc -c main.c -o main.o

   Step 2: Link all .o files together
      gcc math_lib.o main.o -o program
```

Why bother? Because if you change `main.c`, you only need to recompile
`main.o` — `math_lib.o` stays the same. For big projects with hundreds of files,
this saves a lot of time.

The `Makefile` in this module handles all of this for you. Read it and understand
what each rule does.

---

## 9. Abstraction — Hiding Details Behind an Interface

When you write a function like `max_of_array(arr, n)`, the caller does not need
to know **how** you find the maximum. They just call the function and get the
answer. The function's name, parameters, and return type form its **interface**.
The code inside is the **implementation**.

This is **abstraction**: hiding complexity behind a simple interface. It is one
of the most important ideas in all of programming.

Good abstraction means:
- The function name says **what** it does (not **how**)
- The caller does not need to read the function's code to use it
- You can change the implementation without breaking the caller

---

## 10. Katas

Now put it all together. You have four exercises:

| Kata                  | Concepts Practiced                                    |
|-----------------------|-------------------------------------------------------|
| `calculator.c`        | Writing functions, if/else dispatch, return values    |
| `temperature.c`       | Functions with float params, conversion formulas      |
| `math_lib.h/.c`       | Header files, include guards, multi-file compilation  |
| `math_lib_test.c`     | Using your library, assert-style testing              |

### Building and Running

```bash
cd 04-functions-program-structure/

# Build all exercises:
make exercises

# Build all solutions:
make solutions

# Build and run tests on solutions:
make test

# Clean up:
make clean
```

Each exercise file has `TODO` markers where you write your code. Read the
comments carefully — they tell you exactly what to do.

### Recommended Order

1. **calculator.c** — write basic arithmetic as functions
2. **temperature.c** — practice with floats and conversion formulas
3. **math_lib.h** + **math_lib.c** — your first multi-file project
4. **math_lib_test.c** — already written, but read it to understand testing

Good luck. Trace through your function calls by hand, draw the stack frames,
and remember: every argument is a copy.
