# Module 07 — Dynamic Memory

In the last module, you learned about pointers — variables that store addresses.
Now you're going to learn the **most powerful and most dangerous** thing you can
do with pointers: allocate memory at runtime.

Up until now, every variable you've created has been **automatic** — the compiler
decides when it's born (entering a function) and when it dies (leaving a function).
But what if you don't know how much memory you need until the program is running?
What if you need memory that outlives the function that created it?

That's what **dynamic memory** is for.

---

## Stack vs. Heap

Your program's memory is divided into regions. The two most important ones:

```
High addresses
┌──────────────────────────┐
│         Stack            │ ← grows downward
│  ┌────────────────────┐  │
│  │ main() frame       │  │   Automatic variables
│  │   int x = 5;       │  │   Born when function enters
│  ├────────────────────┤  │   Die when function returns
│  │ read_input() frame │  │   Fixed size (compiler decides)
│  │   char buf[64];    │  │   FAST (just move a pointer)
│  └────────────────────┘  │
│            ↓             │
│                          │
│         (free)           │
│                          │
│            ↑             │
│  ┌────────────────────┐  │
│  │ Heap               │  │ ← grows upward
│  │  malloc'd blocks   │  │   Manual allocation
│  │  ┌──┐ ┌─────┐ ┌─┐ │  │   Born when you malloc()
│  │  └──┘ └─────┘ └─┘ │  │   Die when you free()
│  └────────────────────┘  │   Any size (you decide)
├──────────────────────────┤   Slower than stack
│ Global/Static variables  │
├──────────────────────────┤
│ Code (text segment)      │
└──────────────────────────┘
Low addresses
```

### The Stack — Automatic Memory

You've been using the stack the whole time:

```c
void example(void)
{
    int x = 10;        // ← lives on the stack
    char name[20];     // ← lives on the stack
    // When this function returns, x and name are gone
}
```

The stack is:
- **Fast**: allocating memory is just moving a pointer
- **Automatic**: the compiler handles everything
- **Limited**: typically 1-8 MB (overflow it and your program crashes)
- **Scoped**: memory dies when the function returns

**The problem:** What if you need to return an array from a function?

```c
int *make_array(int size)
{
    int arr[size];     // ← lives on the stack
    return arr;        // ← BUG! arr dies when this function returns
}                      //    The caller gets a dangling pointer!
```

This **does not work**. The array is gone by the time the caller uses it. This is
one of the most common bugs in C.

### The Heap — Manual Memory

The heap is a large pool of memory that you manage yourself:

```c
int *make_array(int size)
{
    int *arr = malloc(size * sizeof(int));  // ← lives on the heap
    return arr;     // ← This works! Heap memory persists
}                   //    Caller must free() it when done
```

The heap is:
- **Flexible**: allocate any size, any time
- **Persistent**: memory lives until you explicitly free it
- **Manual**: *you* are responsible for freeing it (the compiler won't help)
- **Slower**: the allocator must search for free space

---

## malloc — Allocating Memory

`malloc` (memory allocate) is the fundamental heap allocation function:

```c
#include <stdlib.h>

void *malloc(size_t size);
```

`size_t` is an unsigned integer type used for sizes — it's what `sizeof` returns.
On a 64-bit system, it's typically 8 bytes. It's unsigned because sizes can't be
negative.

- Takes: number of **bytes** to allocate
- Returns: a pointer to the allocated block, or **NULL** if it fails
- The memory is **uninitialized** (contains whatever garbage was there before)

```c
// Allocate space for 10 integers
int *arr = malloc(10 * sizeof(int));

// ALWAYS check if malloc succeeded
if (arr == NULL) {
    printf("Out of memory!\n");
    return 1;
}

// Now use it like a normal array
arr[0] = 42;
arr[9] = 100;
```

### Why `sizeof` matters

**Never** hardcode sizes. Sizes vary across platforms:

```c
// BAD — assumes int is 4 bytes (not portable)
int *arr = malloc(10 * 4);

// GOOD — sizeof always gives the right answer
int *arr = malloc(10 * sizeof(int));

// EVEN BETTER — sizeof the variable, not the type
// If you change arr's type later, this still works
int *arr = malloc(10 * sizeof(*arr));
```

### Why `void *`?

`malloc` returns `void *` — a "generic" pointer that can point to any type.
In C, `void *` is automatically converted to any pointer type:

```c
int *p = malloc(sizeof(int));       // void* → int* automatically
char *s = malloc(100);              // void* → char* automatically
```

---

## calloc — Allocating Zeroed Memory

```c
void *calloc(size_t count, size_t size);
```

Like malloc, but:
1. Takes count and size separately (count × size bytes total)
2. **Zeros all the memory** (every byte is 0)

```c
// Allocate 10 ints, all initialized to 0
int *arr = calloc(10, sizeof(int));
// arr[0] through arr[9] are all 0
```

When to use calloc vs malloc:
- Use `calloc` when you want everything zeroed (arrays, structs)
- Use `malloc` when you'll initialize everything yourself anyway (saves time)

---

## realloc — Resizing Memory

What if you allocated 10 slots but now need 20?

```c
void *realloc(void *ptr, size_t new_size);
```

`realloc` changes the size of a previously allocated block:

```c
int *arr = malloc(10 * sizeof(int));
// Fill arr[0] through arr[9]...

// Now I need 20 slots
int *temp = realloc(arr, 20 * sizeof(int));
if (temp == NULL) {
    // realloc failed — arr is still valid and unchanged!
    printf("Couldn't resize!\n");
} else {
    arr = temp;  // success — arr may have moved to a new address
}
// arr[0] through arr[9] still have their old values
// arr[10] through arr[19] are uninitialized
```

**Critical rule**: realloc may **move** your data to a new location:

```
Before realloc (no room to grow in place):
┌───────────────┐   ┌──────┐
│ arr (10 ints) │   │ used │   ← something else is right after arr
└───────────────┘   └──────┘

After realloc (moved to a bigger spot):
┌───────────────┐   ┌──────┐           ┌──────────────────────────────┐
│   (freed)     │   │ used │           │ arr (20 ints, data copied)   │
└───────────────┘   └──────┘           └──────────────────────────────┘
```

**The realloc trap** — never do this:

```c
// BAD! If realloc fails, you've lost the original pointer
arr = realloc(arr, new_size);  // if NULL, arr is lost → memory leak!

// GOOD — use a temporary pointer
int *temp = realloc(arr, new_size);
if (temp != NULL) {
    arr = temp;
}
```

---

## free — Releasing Memory

When you're done with heap memory, give it back:

```c
void free(void *ptr);
```

```c
int *arr = malloc(10 * sizeof(int));
// ... use arr ...
free(arr);       // Give the memory back
arr = NULL;      // Good practice: prevent accidental use
```

Setting the pointer to NULL after freeing is a safety net. If you accidentally
dereference it later, you'll get a clean crash (segfault on NULL) instead of
silently reading garbage.

---

## The Four Deadly Memory Bugs

### 1. Memory Leak — malloc without free

```c
void leak(void)
{
    int *p = malloc(1000 * sizeof(int));
    // oops, we return without freeing p
}   // ← p is gone, but the 4000 bytes are still allocated!
```

The pointer `p` (on the stack) is destroyed, but the heap memory it pointed to
is still reserved. Nobody can free it now — it's leaked.

```
Stack:                  Heap:
┌─────────┐            ┌─────────────────┐
│ p = 0x─ ├───────────→│ 4000 bytes      │
└─────────┘            │ (allocated)     │
                       │ (no pointer     │
   p is gone!          │  to it anymore) │  ← LEAKED!
                       └─────────────────┘
```

In short-lived programs, leaks don't matter much. In long-running programs
(servers, embedded systems), leaked memory accumulates until the program crashes.

### 2. Double Free — freeing the same memory twice

```c
int *p = malloc(sizeof(int));
free(p);
free(p);    // ← BUG! Corrupts the heap allocator's bookkeeping
```

The second `free` corrupts the allocator's internal data structures. This can
cause crashes, silent corruption, or security vulnerabilities.

### 3. Use After Free — using memory you already freed

```c
int *p = malloc(sizeof(int));
*p = 42;
free(p);
printf("%d\n", *p);    // ← BUG! p points to freed memory
```

After `free`, the memory might be reused for something else. Reading it gives
garbage. Writing it corrupts other data.

### 4. Buffer Overflow — writing past the end

```c
int *arr = malloc(5 * sizeof(int));
arr[5] = 999;     // ← BUG! Only arr[0] through arr[4] are valid
```

Writing past the allocated region corrupts adjacent heap data — the allocator's
metadata, other allocations, anything.

---

## The Ownership Rule

The hardest part of manual memory management is knowing **who frees what**.

Establish a clear rule for every allocation:

```
╔══════════════════════════════════════════════════════════╗
║  Whoever allocates should document who frees.           ║
║                                                         ║
║  Common patterns:                                       ║
║  1. Creator frees:  the function that mallocs also      ║
║     frees when done                                     ║
║  2. Caller frees:   function returns malloc'd memory,   ║
║     caller is responsible for freeing                   ║
║  3. Transfer:       ownership passes from one part      ║
║     of the code to another (document clearly!)          ║
╚══════════════════════════════════════════════════════════╝
```

For this module, we'll use "caller frees" and always document it in comments.

---

## A Practical Pattern: The Dynamic String Builder

Here's a real use case that combines everything. A string builder that grows
as needed:

We haven't formally covered `struct` yet (that's Module 09), but we need a
**quick preview** here because string builders bundle several related values
together. Don't worry — Module 09 will cover all the details. Here's just
enough to follow along:

### Quick Preview: What is a struct?

So far, every variable holds a single value: one `int`, one `char *`, etc.
But a string builder needs **three things** working together: a pointer to
the buffer, the current length, and the total capacity. Without `struct`,
you'd need three separate variables — and you'd have to pass all three to
every function. That's messy and error-prone.

A `struct` solves this by grouping related variables into **one named bundle**:

```
  Without struct:                  With struct:
  ┌──────────────────────────┐     ┌──────────────────────────┐
  │ char *data;              │     │ StringBuilder sb;        │
  │ int   length;            │     │   sb.data     → [...]    │
  │ int   capacity;          │     │   sb.length   = 3        │
  │                          │     │   sb.capacity = 8        │
  │ (3 separate variables)   │     │ (1 variable, 3 fields)   │
  └──────────────────────────┘     └──────────────────────────┘
```

The syntax is:

```c
/* Define the struct type */
typedef struct {
    char *data;      /* first field  */
    int length;      /* second field */
    int capacity;    /* third field  */
} StringBuilder;

/* Create one and access its fields with '.' */
StringBuilder sb;
sb.data     = malloc(8);
sb.length   = 0;
sb.capacity = 8;
```

- `typedef` lets you write `StringBuilder` instead of `struct StringBuilder`
- Access fields with a dot: `sb.length`
- If you have a *pointer* to a struct, use arrow: `ptr->length`
  (this is shorthand for `(*ptr).length`)

That's all you need for now. Module 09 covers memory layout, padding,
nested structs, unions, and more.

```c
typedef struct {
    char *data;      /* heap-allocated buffer */
    int length;      /* current string length (not counting '\0') */
    int capacity;    /* total buffer size */
} StringBuilder;
```

```
Initial state (capacity = 8):
┌─────────────────────────────────┐
│ data → [ h | e | l | \0 | _ | _ | _ | _ ]
│ length = 3                               │
│ capacity = 8                             │
└─────────────────────────────────┘

After appending "lo world" (grew to capacity = 16):
┌───────────────────────────────────────────────────────────┐
│ data → [ h | e | l | l | o |   | w | o | r | l | d | \0 | _ | _ | _ | _ ]
│ length = 11                                                              │
│ capacity = 16                                                            │
└───────────────────────────────────────────────────────────┘
```

The key insight: when `length` reaches `capacity`, double the capacity with
`realloc`. This is called **amortized doubling** — each append is O(1) on
average because resizes happen less and less frequently. "Amortized" means
"averaged over many operations" — most appends are cheap (just put data in the
next slot), and the rare expensive resize gets spread across all the cheap ones.

---

## A Practical Pattern: The Dynamic Array

Same idea, but for any type of data:

```c
typedef struct {
    int *data;       /* heap-allocated buffer */
    int size;        /* number of elements in use */
    int capacity;    /* total slots available */
} DynArray;
```

Operations:
- **push**: add to the end (resize if full)
- **pop**: remove from the end
- **get/set**: access by index (bounds-check!)
- **free**: release the buffer

```
Push sequence with doubling:

push(1): size=1, cap=4   [1 | _ | _ | _]
push(2): size=2, cap=4   [1 | 2 | _ | _]
push(3): size=3, cap=4   [1 | 2 | 3 | _]
push(4): size=4, cap=4   [1 | 2 | 3 | 4]        ← full!
push(5): size=5, cap=8   [1 | 2 | 3 | 4 | 5 | _ | _ | _]  ← doubled!
```

---

## Valgrind — Your Memory Bug Detective

Valgrind is a tool that watches every memory operation your program makes
and reports bugs:

```bash
# Compile with debug symbols (so valgrind can show line numbers)
gcc -g -Wall -Wextra -std=c99 -o myprogram myprogram.c

# Run under valgrind
valgrind --leak-check=full ./myprogram
```

### Reading Valgrind Output

Here's what valgrind reports for a leak:

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 40 bytes in 1 blocks
==12345==   total heap usage: 3 allocs, 2 frees, 1,064 bytes allocated
==12345==
==12345== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/...)
==12345==    by 0x108681: make_array (main.c:5)
==12345==    by 0x1086A3: main (main.c:12)
```

Breaking it down:
- **3 allocs, 2 frees** → you forgot one free!
- **definitely lost** → memory leaked, nobody has a pointer to it
- **make_array (main.c:5)** → the leak was allocated at line 5 of main.c

### Clean Valgrind Output

This is what you're aiming for:

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 3 allocs, 3 frees, 1,064 bytes allocated
==12345==
==12345== All heap blocks were freed -- no leaks are possible
```

### AddressSanitizer — A Faster Alternative

Compile with `-fsanitize=address` to instrument your program with runtime memory checks:

```bash
gcc -g -Wall -Wextra -std=c99 -fsanitize=address -o myprogram myprogram.c
./myprogram   # crashes immediately with a clear error on any memory bug
```

AddressSanitizer is faster than valgrind but uses more memory. Both are
invaluable. Use them from day one.

---

## GDB — The GNU Debugger

When your program crashes, `printf` debugging is slow and incomplete. GDB lets
you **pause your program, inspect every variable, and step through code line by
line**. It's the most important tool in a systems programmer's toolkit.

### Getting Started

```bash
# Always compile with -g (debug symbols) — otherwise GDB can't show your code
gcc -g -Wall -Wextra -std=c99 -o myprogram myprogram.c

# Run inside GDB
gdb ./myprogram
```

### The 10 Commands You Need

```
Command              What it does
──────────────────   ─────────────────────────────────────────────
break main           Set a breakpoint at the start of main()
break file.c:42      Set a breakpoint at line 42
run                  Start the program (stops at first breakpoint)
next                 Execute current line, step OVER function calls
step                 Execute current line, step INTO function calls
continue             Resume running until next breakpoint or crash
print x              Show the value of variable x
print *ptr           Dereference a pointer and show the value
print arr[0]@10      Show 10 elements of array arr
bt                   Backtrace — show the call stack (where am I?)
watch var            Break whenever var changes (data breakpoint)
quit                 Exit GDB
```

### Typical Debugging Session

```
$ gdb ./myprogram
(gdb) break main
Breakpoint 1 at 0x1149: file main.c, line 8.

(gdb) run
Starting program: ./myprogram
Breakpoint 1, main () at main.c:8
8       int *arr = malloc(10 * sizeof(int));

(gdb) next
9       arr[0] = 42;

(gdb) print arr
$1 = (int *) 0x5555555592a0

(gdb) print arr[0]
$2 = 42

(gdb) next
10      int *p = NULL;

(gdb) next
11      *p = 5;        ← about to crash!

(gdb) next
Program received signal SIGSEGV, Segmentation fault.

(gdb) bt
#0  0x0000555555555178 in main () at main.c:11
                                         ↑
                            GDB tells you EXACTLY which line crashed
```

### When to Use GDB

- **Segfault**: run in GDB, let it crash, type `bt` to see where
- **Wrong output**: set breakpoints, inspect variables at each step
- **Infinite loop**: run in GDB, press Ctrl+C to pause, type `bt` to see where
  you're stuck, type `print` to check loop variables

---

## strace — See Every System Call

`strace` shows you every **system call** your program makes — every file opened,
every byte read/written, every memory allocation. It answers: "what is my
program actually DOING?"

```bash
# Basic usage
strace ./myprogram

# Filter to specific calls (less noise)
strace -e open,read,write ./myprogram

# Show timestamps
strace -t ./myprogram

# Follow child processes (useful with fork)
strace -f ./myprogram
```

### Reading strace Output

```
openat(AT_FDCWD, "data.txt", O_RDONLY) = 3
       ↑ filename                        ↑ returned fd=3

read(3, "Hello, world!\n", 4096)       = 14
     ↑ fd=3                              ↑ read 14 bytes

write(1, "Hello, world!\n", 14)        = 14
      ↑ fd=1 = stdout                    ↑ wrote 14 bytes

close(3)                                = 0

openat(AT_FDCWD, "missing.txt", O_RDONLY) = -1 ENOENT (No such file)
                                            ↑ FAILED — file not found!
```

### When to Use strace

- **"File not found" but it exists**: strace shows you exactly which path the
  program tried to open
- **"Permission denied"**: strace shows the permission check that failed
- **Slow program**: strace -c shows a summary of time spent in each syscall
- **Network debugging**: strace shows connect(), send(), recv() calls

```bash
# Summary mode — which syscalls take the most time?
strace -c ./myprogram

% time     seconds  calls    syscall
------ ----------- --------- --------
 89.32    0.045123       42  read      ← 89% of time in read!
  5.21    0.002631        3  write
  3.47    0.001752       45  mmap
  2.00    0.001011        1  execve
```

### The Debugging Toolkit Summary

```
Tool              Best for                      Speed
────────────────  ──────────────────────────     ─────
valgrind          Memory leaks, invalid reads    Slow (20-50x)
AddressSanitizer  Same as valgrind               Fast (2x)
gdb               Crashes, wrong logic, loops    No overhead
strace            File/network/syscall issues    Minimal overhead
```

Use them ALL. A professional C programmer reaches for these tools instinctively,
the same way a carpenter reaches for a level or a square.

---

## Error Handling Patterns in C

C has no exceptions, no try/catch, no Result type. Error handling is YOUR
responsibility. Here are the patterns that professional C code uses.

### Pattern 1: Return Codes

The simplest pattern: return 0 for success, -1 (or another value) for failure.

```c
int open_file(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        return -1;  /* caller checks this */
    }
    return fd;      /* success: return the fd */
}
```

Convention used by almost all C libraries:

```
  0 or positive  = success (often the actual result)
  -1 or NULL     = failure
  errno          = WHY it failed
```

### Pattern 2: errno — The Global Error Code

When a system call fails, it sets a global variable called `errno` to a code
explaining what went wrong. You can read it with `strerror()` or `perror()`:

```c
#include <errno.h>
#include <string.h>

FILE *fp = fopen("data.txt", "r");
if (fp == NULL) {
    printf("Error: %s\n", strerror(errno));
    /* might print: "Error: No such file or directory" */
}

/* Or use perror() which adds the function name: */
perror("fopen");
/* prints: "fopen: No such file or directory" */
```

Common errno values:

```
  ENOENT  — file not found
  EACCES  — permission denied
  ENOMEM  — out of memory
  EINVAL  — invalid argument
  EEXIST  — file already exists
```

### Pattern 3: The goto cleanup Pattern

This is the ONE good use of `goto` in C. When a function allocates multiple
resources, you need to clean them all up on any error. Without goto, you
get deeply nested if/else pyramids. With goto, the cleanup is clean:

```c
int process_file(const char *path)
{
    int result = -1;
    FILE *fp = NULL;
    char *buf = NULL;
    char *line = NULL;

    fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        goto cleanup;
    }

    buf = malloc(4096);
    if (!buf) {
        perror("malloc");
        goto cleanup;
    }

    line = malloc(256);
    if (!line) {
        perror("malloc");
        goto cleanup;
    }

    /* ... do work with fp, buf, and line ... */

    result = 0;  /* success — only reached if nothing failed */

cleanup:
    free(line);      /* free(NULL) is safe — does nothing */
    free(buf);
    if (fp) fclose(fp);
    return result;
}
```

Why this works:
- Every resource is initialized to NULL at the top
- On ANY error, we jump to cleanup which frees everything
- `free(NULL)` is safe by the C standard — no need to check
- `fclose(NULL)` is NOT safe, so we check `fp` before closing
- The function has ONE exit point, making it easy to verify
  that nothing is leaked

This pattern is used extensively in the Linux kernel, SQLite, PostgreSQL,
and most professional C codebases. It's not "bad" goto — it's a clean
resource management pattern.

### Pattern 4: Error Output Parameter

When a function returns a value AND needs to report errors:

```c
/* Returns the parsed number, sets *err to 1 on failure */
int parse_int(const char *str, int *err)
{
    *err = 0;
    char *end;
    long val = strtol(str, &end, 10);
    if (end == str || *end != '\0') {
        *err = 1;
        return 0;
    }
    return (int)val;
}

/* Usage: */
int err;
int n = parse_int("42abc", &err);
if (err) {
    printf("Invalid number!\n");
}
```

### The Golden Rule

**Always check return values.** Every malloc, every fopen, every read.
Ignoring errors in C doesn't cause exceptions — it causes silent
corruption, crashes hours later, or security vulnerabilities.

---

## Common Patterns

### Pattern 1: Allocate, use, free

```c
int *data = malloc(n * sizeof(*data));
if (data == NULL) { /* handle error */ }

// use data...

free(data);
data = NULL;
```

### Pattern 2: Function returns allocated memory (caller frees)

```c
// Returns a newly allocated string. Caller must free().
char *duplicate_string(const char *s)
{
    int len = strlen(s);
    char *copy = malloc(len + 1);  // +1 for '\0'
    if (copy == NULL) return NULL;
    strcpy(copy, s);
    return copy;
}

// Caller:
char *s = duplicate_string("hello");
// ... use s ...
free(s);
```

### Pattern 3: Init/destroy pair

```c
DynArray *dynarray_create(void);    // allocates
void dynarray_destroy(DynArray *a); // frees everything

// Usage:
DynArray *a = dynarray_create();
// ... use a ...
dynarray_destroy(a);
```

This is a common C pattern: every `_create` function has a matching `_destroy`
function, ensuring you never forget to free memory. Every `_init` has a matching
`_cleanup`.

---

## Exercises

You have three katas for this module:

1. **string_builder.c** — Build a dynamic string that grows with `realloc`.
   Practice the append + double-when-full pattern.

2. **dynamic_array.c** — Build a resizable integer array with push, pop, get,
   and set. The foundation of `std::vector` in C++ and `ArrayList` in Java.

3. **leak_detective.c** — Given buggy code with memory leaks, double frees,
   and use-after-free bugs, find and fix them all. Then verify with valgrind.

Compile and test:
```bash
make exercises    # build your code
make test         # build and run solutions to verify
```

---

[← Previous: Module 06 — Pointers & Arrays](../07-pointers-arrays/README.md) | [Next: Module 08 — Mastering Memory Management →](../30-mastering-memory-management/README.md)
