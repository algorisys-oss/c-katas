# Module 05 — Tool Mastery: GDB, Valgrind, Make, and the Debugging Mindset

> "Give me six hours to chop down a tree and I will spend the first four
> sharpening the axe." — Abraham Lincoln

The best programmers are not the ones who write the most code.
They are the ones who **master their tools early**, and it makes everything
else 10x faster. This module is about sharpening your axe.

---

## Why Tools Matter: The Force Multiplier

Imagine two programmers hit the same bug — a crash that only happens on the
500th iteration of a loop.

**Programmer A** (no tools): adds `printf` statements, recompiles, runs,
reads output, adds more `printf`, recompiles... 45 minutes later, maybe
finds it.

**Programmer B** (tool master): sets a conditional breakpoint in GDB:
`break loop.c:42 if i == 499`, runs the program, inspects the exact state
at the moment of failure. **3 minutes.**

That is a **15x speedup** on a single bug. Over a career of thousands of
bugs, tool mastery is the single highest-leverage skill you can build.

```
    Time to fix bugs
    ┌──────────────────────────────────────────┐
    │                                          │
    │  Without tools:  ████████████████████ 45m│
    │                                          │
    │  With tools:     ██ 3m                   │
    │                                          │
    │  Difference:     15x faster              │
    └──────────────────────────────────────────┘
```

---

## GDB — The GNU Debugger (From First Principles)

### What Is a Debugger?

A debugger is a program that **controls another program**. It can:
- **Pause** execution at any line
- **Inspect** the value of any variable
- **Step** through code one line at a time
- **Watch** a variable and stop when it changes
- **Trace** back through the chain of function calls

Think of it as a **time machine** for your code. You can freeze time,
look around, and step forward one instruction at a time.

### How to Use GDB

**Step 0: Compile with debug info**

```bash
gcc -std=c99 -Wall -Wextra -pedantic -g -o my_program my_program.c
```

The `-g` flag tells the compiler to embed a map between machine code and
your source code. Without it, GDB cannot show you your C code.

**Step 1: Launch GDB**

```bash
gdb ./my_program
```

You are now inside GDB. Your program has NOT started yet. You are in the
"planning phase" — set breakpoints, configure things, then run.

### Core GDB Commands

| Command                   | Short | What It Does                              |
|---------------------------|-------|-------------------------------------------|
| `run`                     | `r`   | Start (or restart) the program            |
| `break main`              | `b`   | Set breakpoint at function `main`         |
| `break file.c:42`         | `b`   | Set breakpoint at line 42                 |
| `next`                    | `n`   | Execute current line, step OVER functions |
| `step`                    | `s`   | Execute current line, step INTO functions |
| `continue`                | `c`   | Resume until next breakpoint              |
| `print x`                 | `p`   | Print value of variable `x`              |
| `print *ptr`              | `p`   | Print what pointer points to             |
| `print arr[0]@10`         | `p`   | Print 10 elements starting at arr[0]    |
| `watch x`                 |       | Stop whenever `x` changes                |
| `backtrace`               | `bt`  | Show the call stack                       |
| `frame 2`                 | `f 2` | Switch to stack frame #2                 |
| `info locals`             |       | Show all local variables                 |
| `info breakpoints`        |       | List all breakpoints                     |
| `delete 3`                | `d 3` | Delete breakpoint #3                     |
| `quit`                    | `q`   | Exit GDB                                 |

### Breakpoints: Stopping Time

A breakpoint says: "pause the program when execution reaches this line."

```
    Source code                 Execution
    ┌─────────────────┐
    │ int main() {    │         ──► runs normally
    │   int x = 5;    │ ● ◄── BREAKPOINT: execution pauses here
    │   x = x + 1;    │         (you inspect x, it's 5)
    │   x = x * 2;    │         type 'next' ──► now x is 6
    │   return x;     │         type 'next' ──► now x is 12
    │ }               │
    └─────────────────┘

    ●  = breakpoint (execution pauses here)
```

**Conditional breakpoints** — stop only when a condition is true:

```
(gdb) break loop.c:15 if i == 499
```

This is incredibly powerful for bugs that only appear on specific
iterations.

### Stepping: Walking Through Code

```
    next (n)                    step (s)
    ┌──────────────┐            ┌──────────────┐
    │ a = foo(5);  │──► skip    │ a = foo(5);  │──► enter foo()
    │ b = a + 1;   │   into     │   int r = x; │
    │              │   foo()    │   r = r * 2; │
    │              │            │   return r;  │
    │              │            │ b = a + 1;   │
    └──────────────┘            └──────────────┘

    "next" treats function calls as a single step.
    "step" descends into the function body.
```

### Stack Frames in GDB

When function A calls function B, and B calls function C, the stack
looks like this:

```
    ┌─────────────────────────────────┐  High addresses
    │         main()                  │  Frame #2
    │  local vars: argc, argv         │
    ├─────────────────────────────────┤
    │         process_data()          │  Frame #1
    │  local vars: buf, len           │
    ├─────────────────────────────────┤
    │         parse_line()            │  Frame #0 (current)
    │  local vars: line, pos          │
    └─────────────────────────────────┘  Low addresses
           ▲
           │
      You are here (GDB shows frame #0 by default)

    (gdb) backtrace
    #0  parse_line (line=0x7fff..., pos=42) at parser.c:15
    #1  process_data (buf=0x7fff..., len=1024) at data.c:88
    #2  main (argc=1, argv=0x7fff...) at main.c:10

    (gdb) frame 1        ← jump to process_data's frame
    (gdb) print buf       ← see process_data's local variables
```

Use `backtrace` (or `bt`) whenever your program crashes. It tells you
**exactly** what chain of calls led to the crash.

### Watchpoints: Catching Changes

A watchpoint says: "pause whenever this variable's value changes."

```
(gdb) watch x
```

This is perfect for tracking down where a variable gets corrupted.
GDB will stop and tell you both the old and new values.

```
    Hardware watchpoint 2: x
    Old value = 42
    New value = -2147483648    ← overflow! Something went wrong here
```

---

## Valgrind — The Memory Detective

### What Valgrind Does

Valgrind runs your program inside a **virtual CPU** that tracks every
single byte of memory. It watches for:

1. **Reading memory you haven't written to** (uninitialized values)
2. **Reading/writing memory you don't own** (buffer overflows)
3. **Using memory after you freed it** (use-after-free)
4. **Freeing memory twice** (double free)
5. **Memory you allocated but never freed** (leaks)

Think of valgrind as a **security camera for your memory**. Nothing
escapes its notice.

### How to Use Valgrind

```bash
# Compile with -g (debug info) and NO optimization
gcc -std=c99 -Wall -Wextra -pedantic -g -o my_program my_program.c

# Run under valgrind
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./my_program
```

Key flags:
- `--leak-check=full` — show details for each leak
- `--show-leak-kinds=all` — report all leak types
- `--track-origins=yes` — trace uninitialized values to their source

### Reading Valgrind Output (Annotated Example)

```
==12345== Invalid read of size 4                    ← What happened
==12345==    at 0x4005F2: process (buggy.c:15)      ← Where (file:line)
==12345==    by 0x400620: main (buggy.c:22)         ← Call chain
==12345==  Address 0x5204048 is 0 bytes after a     ← Details about
==12345==  block of size 40 alloc'd                    the memory
==12345==    at 0x4C2AB80: malloc (vg_replace...)
==12345==    by 0x4005D0: process (buggy.c:12)
         │                │           │
         │                │           └── Source file and line number
         │                └── Function name
         └── "Invalid read of size 4" = read 4 bytes from memory you
             don't own (probably past the end of an array)
```

### Common Valgrind Errors

```
    ┌────────────────────────────────────────────────────────────┐
    │  Error Type              │  What It Means                  │
    ├────────────────────────────────────────────────────────────┤
    │  Invalid read of size N  │  Read N bytes you don't own     │
    │  Invalid write of size N │  Wrote N bytes you don't own    │
    │  Use of uninitialized    │  Used a value before setting it │
    │  Invalid free            │  free() on bad pointer or twice │
    │  Conditional jump...     │  Branch depends on uninit value │
    │  depends on uninitialised│                                 │
    └────────────────────────────────────────────────────────────┘
```

### The Leak Summary

At the end, valgrind prints a leak summary:

```
==12345== LEAK SUMMARY:
==12345==    definitely lost: 40 bytes in 1 blocks    ← YOU must fix these
==12345==    indirectly lost: 0 bytes in 0 blocks
==12345==      possibly lost: 0 bytes in 0 blocks
==12345==    still reachable: 0 bytes in 0 blocks
==12345==         suppressed: 0 bytes in 0 blocks
```

- **definitely lost** — memory you allocated and then lost all pointers to.
  This is a real leak. Fix it.
- **indirectly lost** — memory reachable only through "definitely lost"
  memory. Fixing the definite loss usually fixes these too.
- **still reachable** — memory still pointed to at exit. Often not a bug
  (e.g., global data structures), but good practice to free.

### Memory Map: What Valgrind Watches

```
    Process Address Space
    ┌─────────────────────────────┐  0xFFFFFFFF
    │         Stack               │  ← valgrind watches
    │    (local variables)        │     stack overflows
    │           │                 │
    │           ▼                 │
    │                             │
    │           ▲                 │
    │           │                 │
    │         Heap                │  ← valgrind watches every
    │    (malloc/free)            │     malloc, free, read, write
    ├─────────────────────────────┤
    │    Data Segment             │
    │    (globals, statics)       │
    ├─────────────────────────────┤
    │    Text Segment             │
    │    (your code)              │
    └─────────────────────────────┘  0x00000000
```

---

## AddressSanitizer (ASan) — Compile-Time Instrumentation

### What Is ASan?

AddressSanitizer is a **compiler feature** (built into gcc and clang) that
inserts checking code directly into your program at compile time.

```bash
gcc -std=c99 -Wall -Wextra -pedantic -g -fsanitize=address -o my_program my_program.c
```

### ASan vs. Valgrind

```
    ┌──────────────────┬─────────────────┬──────────────────┐
    │                  │  Valgrind       │  ASan            │
    ├──────────────────┼─────────────────┼──────────────────┤
    │  Speed           │  10-50x slower  │  ~2x slower      │
    │  Leak detection  │  Yes            │  Yes (with LSan) │
    │  Uninit values   │  Yes            │  No (use MSan)   │
    │  Stack bugs      │  Limited        │  Excellent       │
    │  Recompile?      │  No             │  Yes             │
    │  Precision       │  Byte-level     │  Byte-level      │
    └──────────────────┴─────────────────┴──────────────────┘
```

**Rule of thumb:**
- During development: compile with `-fsanitize=address` always
- For leak hunting and uninitialized values: use valgrind
- For maximum coverage: use both at different times (never together)

---

## Make Beyond Basics

### Why Make?

Every time you compile, you type something like:
```bash
gcc -std=c99 -Wall -Wextra -pedantic -g -o my_program file1.c file2.c file3.c
```

That gets old fast. Make automates it AND is smart enough to only
recompile files that changed.

### Anatomy of a Makefile

```makefile
# Variables (like #define for your build)
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

# Rule format:
# target: dependencies
# [TAB] command

my_program: main.o utils.o data.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c utils.h data.h
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $<

data.o: data.c data.h
	$(CC) $(CFLAGS) -c $<
```

### Automatic Variables

These are Make's shorthand. Memorize them — you will use them constantly.

```
    ┌──────┬────────────────────────────────────────┐
    │  $@  │  The target name         (my_program)  │
    │  $<  │  The first dependency    (main.c)      │
    │  $^  │  ALL dependencies        (main.o ...)  │
    │  $*  │  The stem of a pattern   (main)        │
    └──────┴────────────────────────────────────────┘
```

### Pattern Rules

Instead of writing a rule for every `.c` → `.o`, write ONE pattern rule:

```makefile
%.o: %.c
	$(CC) $(CFLAGS) -c $<
```

The `%` is a wildcard. `%.o: %.c` means "any `.o` file depends on the
matching `.c` file."

### Dependency Graph

Make builds a **directed acyclic graph** (DAG) of dependencies and only
rebuilds what changed:

```
                    my_program
                   /    |     \
                  /     |      \
              main.o  utils.o  data.o
              / |       |        |  \
           main.c |   utils.c  data.c \
              utils.h  utils.h       data.h
```

If you edit `utils.c`, Make rebuilds `utils.o` and then re-links
`my_program`. It does NOT recompile `main.c` or `data.c`.

### Phony Targets

Some targets are not files — they are actions:

```makefile
.PHONY: all clean test

all: my_program

clean:
	rm -f *.o my_program

test: my_program
	./my_program --test
```

`.PHONY` tells Make these targets are commands, not files. Without it,
if a file named `clean` existed, `make clean` would say "already up to
date" and do nothing.

---

## The Debugging Mindset

This is the most important section. Tools are only as good as the
**process** you use them in.

### The 5-Step Process

```
    ┌──────────────┐
    │ 1. REPRODUCE │ ──► Can you make the bug happen reliably?
    └──────┬───────┘     If not, gather more info until you can.
           │
    ┌──────▼───────┐
    │  2. ISOLATE  │ ──► Narrow down: which function? Which input?
    └──────┬───────┘     Remove code until the bug disappears,
           │             then add the last piece back.
    ┌──────▼───────┐
    │ 3. UNDERSTAND│ ──► WHY does it happen? Don't just find the
    └──────┬───────┘     line — understand the root cause.
           │
    ┌──────▼───────┐
    │   4. FIX     │ ──► Fix the root cause, not the symptom.
    └──────┬───────┘     A band-aid fix will come back.
           │
    ┌──────▼───────┐
    │  5. VERIFY   │ ──► Does the fix work? Did it break anything
    └──────────────┘     else? Run ALL your tests.
```

### Common Debugging Traps

1. **"It works on my machine"** — always test with the same flags,
   same compiler version, same inputs.

2. **Fixing the symptom** — adding `if (ptr != NULL)` to hide a crash
   instead of finding why `ptr` is NULL.

3. **Not reading the error message** — the compiler and valgrind are
   trying to TELL you what is wrong. Read every word.

4. **Changing multiple things at once** — change ONE thing, test, repeat.
   If you change 5 things and it works, you do not know which fix
   mattered.

5. **Assuming the bug is elsewhere** — the bug is in YOUR code, not the
   compiler, not the OS. Start from that assumption.

### The Printf Debugging Trap

Printf debugging works, but it is the **slowest** approach:

```
    printf debugging loop:
    ┌──────────────────────────────────────────────┐
    │                                              │
    │   Add printf ──► Compile ──► Run ──► Read ───┤
    │       ▲                                 │    │
    │       └── Not enough info? ◄────────────┘    │
    │                                              │
    │   Average iterations: 5-10                   │
    │   Time per iteration: 30-60 seconds          │
    │   Total: 5-10 MINUTES per bug                │
    └──────────────────────────────────────────────┘

    GDB approach:
    ┌──────────────────────────────────────────────┐
    │                                              │
    │   Set breakpoint ──► Run ──► Inspect state ──┤
    │       ▲                            │         │
    │       └── Need more? Step/watch ◄──┘         │
    │                                              │
    │   Average iterations: 1-2                    │
    │   Time per iteration: 30-60 seconds          │
    │   Total: 1-2 MINUTES per bug                 │
    └──────────────────────────────────────────────┘
```

---

## Exercises

| File | What You Practice |
|---|---|
| `gdb_detective.c` | Using GDB to find and fix 5 different bugs |
| `valgrind_hunt.c` | Using valgrind to find and fix 6 memory errors |
| `segfault_safari.c` | Diagnosing and fixing 5 crash-causing bugs |

### How to Work Through These

1. **Read the code** — try to spot the bug by reading first
2. **Compile and run** — see what happens (crash? wrong output?)
3. **Use the tool** — GDB, valgrind, or ASan as directed
4. **Fix the bug** — make the tests pass
5. **Verify** — run again, make sure all tests pass and valgrind is clean

### Compilation Quick Reference

```bash
# Normal build (with debug info)
gcc -std=c99 -Wall -Wextra -pedantic -g -o exercise exercise.c

# Build with AddressSanitizer
gcc -std=c99 -Wall -Wextra -pedantic -g -fsanitize=address -o exercise exercise.c

# Run under valgrind
valgrind --leak-check=full --track-origins=yes ./exercise

# Run under GDB
gdb ./exercise
```

Or just use the Makefile:

```bash
make all          # build everything
make debug        # build with ASan
make valgrind     # run valgrind on valgrind_hunt
make clean        # remove binaries
```
