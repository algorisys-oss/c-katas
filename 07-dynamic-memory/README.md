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

We haven't formally covered `struct` yet (that's Module 09), but the idea is
simple: a struct groups related variables together under one name. `typedef`
gives it a shorter name so you can write `StringBuilder` instead of
`struct StringBuilder`. For now, just read these as "a bundle of related data."

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
