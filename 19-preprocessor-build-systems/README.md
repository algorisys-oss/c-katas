# Module 19 — The Preprocessor & Build Systems

Before the compiler ever sees your code, another program runs first: the
**preprocessor**. It's a simple text-manipulation tool that copies, pastes,
and transforms your source code based on directives that start with `#`.

Understanding the preprocessor is essential because:
- Every `#include` you've ever written is a preprocessor directive
- Macros are one of C's most powerful (and dangerous) features
- Conditional compilation lets you write code for multiple platforms
- Build systems like `make` orchestrate the entire process

---

## The Compilation Pipeline (Revisited)

Remember the compilation pipeline from Module 01? Let's zoom in on the first
step:

```
  Your Code         Preprocessor           Compiler        Assembler       Linker
 ┌──────────┐      ┌──────────────┐      ┌──────────┐    ┌──────────┐   ┌──────────┐
 │ main.c   │ ──── │ Text         │ ──── │ Compiles │ ── │ Converts │ ─ │ Combines │
 │          │  #   │ substitution │  .i  │ to asm   │ .s │ to       │.o │ object   │
 │ #include │      │ #include →   │      │          │    │ machine  │   │ files    │
 │ #define  │      │   paste file │      │          │    │ code     │   │          │
 │ #ifdef   │      │ #define →    │      │          │    │          │   │          │
 │          │      │   replace    │      │          │    │          │   │          │
 └──────────┘      └──────────────┘      └──────────┘    └──────────┘   └──────────┘
   source              expanded             assembly        object       executable
   code                source               code            file
```

You can see the preprocessor output yourself:

```bash
gcc -E main.c -o main.i    # Stop after preprocessing
```

Try it! You'll see that `#include <stdio.h>` gets replaced with *thousands*
of lines of declarations from the standard library header.

---

## `#include` — Copy-Paste a File

`#include` is the simplest directive. It literally copies the contents of
another file and pastes them in place of the `#include` line.

```c
/* Before preprocessing: */
#include <stdio.h>

int main(void) {
    printf("hello\n");
}

/* After preprocessing (simplified): */
/* ... 800+ lines from stdio.h ... */
typedef struct _IO_FILE FILE;
extern FILE *stdout;
int printf(const char *format, ...);
/* ... more declarations ... */

int main(void) {
    printf("hello\n");
}
```

### Angle brackets vs. quotes

```c
#include <stdio.h>      /* Search system include paths (/usr/include, etc.) */
#include "my_header.h"  /* Search current directory first, then system paths */
```

Use `<>` for standard library headers, `""` for your own headers.

### Include Guards

What happens if two headers both `#include` the same file? You get duplicate
declarations — a compile error. Include guards prevent this:

```c
/* utils.h */
#ifndef UTILS_H
#define UTILS_H

int add(int a, int b);
void print_array(int *arr, int n);

#endif /* UTILS_H */
```

The first time `utils.h` is included, `UTILS_H` is not defined, so the
preprocessor defines it and includes the declarations. The second time,
`UTILS_H` is already defined, so everything between `#ifndef` and `#endif`
is skipped.

---

## `#define` — Object-Like Macros

The simplest macro defines a name that gets replaced with a value:

```c
#define MAX_SIZE 100
#define PI 3.14159

int buffer[MAX_SIZE];        /* becomes: int buffer[100]; */
double area = PI * r * r;    /* becomes: double area = 3.14159 * r * r; */
```

This is pure text replacement. The preprocessor doesn't know about types,
scope, or expressions — it just finds `MAX_SIZE` and replaces it with `100`.

### Why not just use variables?

- Macros are resolved at compile time — no runtime cost
- They can be used in array sizes: `int arr[MAX_SIZE];` (C99 allows VLAs,
  but `#define` constants work in all contexts)
- They can be used in `#if` conditions
- They have no type — which is both a feature and a danger

---

## `#define` — Function-Like Macros

Macros can take parameters, making them look like functions:

```c
#define SQUARE(x) ((x) * (x))

int result = SQUARE(5);      /* becomes: int result = ((5) * (5)); */
int result2 = SQUARE(a + 1); /* becomes: int result2 = ((a + 1) * (a + 1)); */
```

### The Parentheses Are Critical

Watch what happens without proper parentheses:

```c
/* BAD: no parentheses */
#define SQUARE_BAD(x) x * x

int result = SQUARE_BAD(3 + 1);
/* becomes: int result = 3 + 1 * 3 + 1;
   = 3 + 3 + 1 = 7  (not 16!) */

/* GOOD: fully parenthesized */
#define SQUARE(x) ((x) * (x))

int result = SQUARE(3 + 1);
/* becomes: int result = ((3 + 1) * (3 + 1));
   = 4 * 4 = 16  (correct!) */
```

**Rule**: Always parenthesize every parameter use AND the whole expression.

### The Double Evaluation Pitfall

Macro parameters are substituted textually, so they get evaluated every time
they appear:

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int x = 5, y = 3;
int m = MAX(x++, y);
/* becomes: int m = ((x++) > (y) ? (x++) : (y));
   x gets incremented TWICE if x > y! */
```

This is a fundamental limitation of macros. Real functions evaluate their
arguments once. Macros are just text substitution.

**When to use macros vs. functions:**
- Use macros for simple constants and type-generic operations
- Use functions when you need guaranteed single evaluation
- Use `inline` functions (C99) when you want both

---

## Macro Pitfalls — A Summary

| Pitfall              | Example                             | Fix                        |
|----------------------|-------------------------------------|----------------------------|
| Missing parens       | `#define SQ(x) x*x`                | `#define SQ(x) ((x)*(x))` |
| Double evaluation    | `MAX(i++, j)`                       | Use a real function        |
| Semicolon in macro   | `#define FOO(x) { bar(x); };`      | Use do-while trick         |
| Multi-statement      | `if (c) FOO(x); else ...` breaks   | `do { ... } while(0)`     |

### The do-while(0) Trick

For multi-statement macros, wrap in `do { ... } while(0)`:

```c
/* BAD: breaks if/else */
#define LOG(msg) printf("LOG: "); printf("%s\n", msg);

if (error)
    LOG("oops");   /* Only first printf is in the if! */
else
    handle_ok();   /* Compile error: else without if */

/* GOOD: safe in any context */
#define LOG(msg) do { printf("LOG: "); printf("%s\n", msg); } while(0)

if (error)
    LOG("oops");   /* Both printfs are in the if */
else
    handle_ok();   /* Works correctly */
```

---

## Advanced Macros

### Stringification: `#`

The `#` operator turns a macro parameter into a string literal:

```c
#define STRINGIFY(x) #x

printf("%s\n", STRINGIFY(hello));    /* prints: hello */
printf("%s\n", STRINGIFY(3 + 4));    /* prints: 3 + 4 */
```

This is useful for debug macros that print both the expression and its value:

```c
#define DEBUG_INT(expr) printf(#expr " = %d\n", (expr))

DEBUG_INT(x + y);  /* becomes: printf("x + y" " = %d\n", (x + y)); */
                    /* prints: x + y = 8 */
```

### Token Pasting: `##`

The `##` operator glues two tokens together:

```c
#define CONCAT(a, b) a##b

int CONCAT(my, _var) = 42;  /* becomes: int my_var = 42; */
```

This is used for generating function names, variable names, and enum values
programmatically.

### Variadic Macros: `__VA_ARGS__`

Macros can accept a variable number of arguments (C99):

```c
#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

LOG("error: %s at line %d\n", msg, line);
/* becomes: fprintf(stderr, "error: %s at line %d\n", msg, line); */
```

A common pattern for debug logging:

```c
#define DEBUG(fmt, ...) \
    fprintf(stderr, "[%s:%d] " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

DEBUG("x = %d, y = %d", x, y);
/* prints: [main.c:42] x = 10, y = 20 */
```

### Useful Predefined Macros

| Macro         | Value                              |
|---------------|------------------------------------|
| `__FILE__`    | Current source file name (string)  |
| `__LINE__`    | Current line number (integer)      |
| `__func__`    | Current function name (string)     |
| `__DATE__`    | Compilation date (string)          |
| `__TIME__`    | Compilation time (string)          |

---

## Conditional Compilation

The preprocessor can include or exclude blocks of code based on conditions:

```c
#ifdef DEBUG
    printf("debug: x = %d\n", x);
#endif
```

You define `DEBUG` by passing it to the compiler:

```bash
gcc -DDEBUG main.c -o main      # DEBUG is defined
gcc main.c -o main               # DEBUG is not defined
```

### `#ifdef` / `#ifndef` / `#if` / `#elif` / `#else` / `#endif`

```c
/* Check if a macro is defined */
#ifdef DEBUG
    /* code compiled only when DEBUG is defined */
#endif

/* Check if a macro is NOT defined */
#ifndef UTILS_H
    /* include guard pattern */
#endif

/* Check a value */
#if MAX_SIZE > 100
    /* ... */
#elif MAX_SIZE > 50
    /* ... */
#else
    /* ... */
#endif
```

### Platform-Specific Code

```c
#ifdef __linux__
    #include <unistd.h>
    void platform_sleep(int ms) { usleep(ms * 1000); }
#elif defined(_WIN32)
    #include <windows.h>
    void platform_sleep(int ms) { Sleep(ms); }
#elif defined(__APPLE__)
    #include <unistd.h>
    void platform_sleep(int ms) { usleep(ms * 1000); }
#else
    #error "Unsupported platform"
#endif
```

---

## Makefiles — From First Principles

You've been using Makefiles in every module. Now let's understand how they
actually work.

### Why Make?

When your project has multiple source files:

```bash
gcc -c util.c -o util.o          # Compile util.c → util.o
gcc -c main.c -o main.o          # Compile main.c → main.o
gcc main.o util.o -o program     # Link → program
```

If you change only `util.c`, you shouldn't have to recompile `main.c`. Make
tracks these dependencies and only rebuilds what changed.

### Rules

A Makefile is a set of **rules**:

```makefile
target: prerequisites
	recipe
```

- **target**: the file to create (or a phony name)
- **prerequisites**: files the target depends on
- **recipe**: shell commands to build the target (MUST be indented with a TAB)

```makefile
program: main.o util.o
	gcc main.o util.o -o program

main.o: main.c util.h
	gcc -c main.c -o main.o

util.o: util.c util.h
	gcc -c util.c -o util.o
```

### Makefile Dependency Graph

```
                    program
                   /       \
                  /         \
              main.o       util.o
             /     \      /     \
          main.c  util.h  util.c  util.h
```

Make walks this graph bottom-up. If `util.c` is newer than `util.o`, it
rebuilds `util.o`, then `program`. But `main.o` is untouched.

### How Make Decides What to Rebuild — Timestamps

Make uses a simple rule: **if a source file is newer than its target, rebuild
the target.** "Newer" means the file's last-modified timestamp is more recent.

Let's trace through a real scenario:

```
  Scenario: You edit util.c. What does `make` rebuild?

  File               Last Modified     Status
  ──────────────     ───────────────   ──────────────
  main.c             10:00 AM          unchanged
  util.h             10:00 AM          unchanged
  util.c             10:15 AM          ← YOU EDITED THIS
  main.o             10:01 AM          newer than main.c ✓ (skip)
  util.o             10:01 AM          OLDER than util.c ✗ (rebuild!)
  program            10:02 AM          depends on util.o (rebuild!)

  Make's actions:
  1. Check main.o: main.c (10:00) < main.o (10:01) → up to date, skip
  2. Check util.o: util.c (10:15) > util.o (10:01) → STALE, rebuild
     → runs: gcc -c util.c -o util.o
  3. Check program: util.o just changed → STALE, rebuild
     → runs: gcc main.o util.o -o program

  Result: only 2 commands run instead of 3. That's the point of Make!
```

### Debugging Make: Why Isn't This Rebuilding?

When Make doesn't rebuild something you expected, use these tools:

```bash
# Dry run — show what WOULD be rebuilt, without actually doing it
make -n

# Force rebuild everything (ignore timestamps)
make -B

# Show Make's reasoning (very verbose — pipe to less)
make -d | less

# Check timestamps yourself
ls -l --time=ctime main.c main.o
```

Common gotchas:
- **Forgot to list a header as a dependency**: If `main.c` includes `util.h`
  but the Makefile doesn't list `util.h` as a prerequisite of `main.o`, then
  editing `util.h` won't trigger a rebuild of `main.o`. Always list headers!
- **Clock skew**: If your files have future timestamps (e.g., from a VM or
  network drive), Make gets confused. Use `touch` to fix timestamps.

### Variables

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lm

program: main.o util.o
	$(CC) $(LDFLAGS) main.o util.o -o program

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
```

### Automatic Variables

| Variable | Meaning                                 |
|----------|-----------------------------------------|
| `$@`     | The target name                         |
| `$<`     | The first prerequisite                  |
| `$^`     | All prerequisites (space-separated)     |
| `$*`     | The stem of a pattern rule match        |

```makefile
program: main.o util.o
	$(CC) $^ -o $@
# $@ = program, $^ = main.o util.o
```

### Pattern Rules

Instead of writing a rule for every `.c` → `.o` conversion:

```makefile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```

This says: "To build any `.o` file, compile the corresponding `.c` file."

### Phony Targets

Targets that don't represent files:

```makefile
.PHONY: clean test all

clean:
	rm -f *.o program

test: program
	./program

all: program
```

Without `.PHONY`, if a file named `clean` existed, `make clean` would say
"clean is up to date" and do nothing.

### A Complete Example Makefile

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

SRCS = main.c util.c parser.c
OBJS = $(SRCS:.c=.o)       # Replace .c with .o in SRCS
TARGET = program

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
```

---

## Separate Compilation & Linking

### Why Separate Compilation?

For large projects, compiling everything from scratch every time is slow.
Separate compilation lets you:

1. Compile each `.c` file independently into an **object file** (`.o`)
2. Link all object files together into the final executable

```
  util.c ──── gcc -c ────► util.o ──┐
                                     ├── gcc ──► program
  main.c ──── gcc -c ────► main.o ──┘
```

### Object Files

An object file (`.o`) contains:
- Machine code for the functions in that `.c` file
- A **symbol table** listing functions/variables it defines and needs
- Relocation info so the linker can fix up addresses

```bash
gcc -c util.c -o util.o     # Compile only, don't link
nm util.o                    # List symbols in the object file
```

### Static Libraries (`.a`)

A static library is an archive of object files:

```bash
gcc -c util.c -o util.o
gcc -c math_helpers.c -o math_helpers.o
ar rcs libutils.a util.o math_helpers.o     # Create library

gcc main.c -L. -lutils -o program           # Link against it
```

The linker copies the needed code from the library into your executable.
Your program has no dependency on the `.a` file at runtime.

### Shared Libraries (`.so`)

A shared library is loaded at runtime:

```bash
gcc -fPIC -c util.c -o util.o               # Position-independent code
gcc -shared util.o -o libutils.so            # Create shared lib

gcc main.c -L. -lutils -o program           # Link against it
```

The program stores a reference to the library. At runtime, the dynamic linker
(`ld-linux.so`) loads `libutils.so` into memory. Multiple programs can share
the same copy.

### Static vs. Shared Libraries

| Property          | Static (`.a`)       | Shared (`.so`)       |
|-------------------|---------------------|----------------------|
| Linked at         | Compile time        | Runtime              |
| Executable size   | Larger              | Smaller              |
| Runtime dependency| None                | Needs `.so` present  |
| Memory sharing    | No                  | Yes (across procs)   |
| Update library    | Must recompile      | Just replace `.so`   |

---

## Exercises

| File              | What You'll Build                                    | Tests |
|-------------------|------------------------------------------------------|-------|
| `macros.c`        | Utility macros: MIN, MAX, SWAP, ARRAY_SIZE, and more | 14    |

Build and test:

```bash
make exercises    # Build your solutions
make test         # Build and run reference solutions
make clean        # Remove compiled files
```

---

## Phase 4 Complete — Review & Connect

You now have all the building blocks of a professional C programmer:

- **Data structures**: lists, stacks, queues, hash tables, trees, heaps
- **Algorithms**: sorting, searching, graph traversal
- **File I/O**: reading/writing files, binary formats
- **Text processing**: Unicode, encoding, string manipulation
- **Build systems**: Makefiles, separate compilation, linking

**The key insight**: Every program you use daily is built from these
pieces. A database is B-trees + file I/O + parsing. A web browser is
networking + parsing + rendering. A game is data structures + graphics
+ input handling.

**What's next**: Phase 5 puts it all together — you'll build real
systems: a text editor, a database, a network server, and more.

---

[← Previous: Module 18: Date & Time](../18-date-time/README.md) | [Next: Module 20 — Graphs →](../20-graphs/README.md)
