# Module 06 — Pointers & Arrays

Welcome to Phase 2! Everything changes here. Up until now, you've been working
with simple values — integers, floats, characters. Now you're going to learn
about **pointers**, the single most important concept in C.

Pointers let you reach into memory directly. They're what make C powerful enough
to build operating systems, databases, and game engines. They're also where most
bugs come from. Master pointers and you've mastered C.

---

## What Is a Pointer?

Every variable lives somewhere in memory. That "somewhere" is an **address** — a
number that identifies a specific byte in RAM.

A **pointer** is a variable that stores an address.

```
Regular variable:        Pointer variable:
┌──────────┐             ┌──────────┐
│ x = 42   │             │ p = 0x7A │──────┐
│ addr: 0x7A│            │ addr: 0x50│      │
└──────────┘             └──────────┘      │
      ▲                                     │
      └─────────────────────────────────────┘
                  p "points to" x
```

Think of it like a house and its address:
- The **variable** is the house (it holds stuff)
- The **address** is where the house is (a number on the street)
- The **pointer** is a piece of paper with that address written on it

---

## Declaring Pointers

```c
int x = 42;
int *p = &x;    // p holds the address of x
```

Breaking this down:
- `int *p` — declares `p` as a "pointer to int"
- `&x` — the **address-of** operator, gives you x's memory address
- `*p` — the **dereference** operator, gives you the value at the address

```
The two key operators:

    &  (address-of)     Gets the address of a variable
                        &x → 0x7FFF1234

    *  (dereference)    Gets the value at an address
                        *p → 42
```

### Reading pointer declarations

Read them **right to left**:

```
int *p;        → p is a pointer to int
char *s;       → s is a pointer to char
int **pp;      → pp is a pointer to a pointer to int
const int *p;  → p is a pointer to a const int (can't change the value)
int *const p;  → p is a const pointer to int (can't change where it points)
```

---

## Pointer Operations

### 1. Assign and dereference

```c
int x = 42;
int *p = &x;       // p points to x

printf("%d\n", *p);  // prints 42 (dereference: read through pointer)
*p = 100;             // write through pointer: x is now 100
printf("%d\n", x);    // prints 100
```

### 2. NULL pointer

A pointer that doesn't point anywhere:

```c
int *p = NULL;      // p points to nothing

if (p != NULL) {    // ALWAYS check before dereferencing
    printf("%d\n", *p);
}
```

Dereferencing NULL causes a **segmentation fault** (or "segfault") — the
operating system detects your program trying to access memory it's not allowed
to touch and kills it immediately. Always check for NULL.

### 3. Swap with pointers

Without pointers, you can't write a swap function that actually works:

```c
// This DOESN'T work — x and y are copies
void bad_swap(int x, int y) {
    int temp = x;
    x = y;
    y = temp;
}   // Changes are lost when function returns

// This WORKS — we modify the original variables through pointers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(void) {
    int x = 10, y = 20;
    swap(&x, &y);      // Pass addresses
    // x is now 20, y is now 10
}
```

Why? Because C is **pass-by-value**. When you call `bad_swap(x, y)`, the
function gets copies of `x` and `y`. It swaps the copies, then throws them
away. With pointers, you pass the *addresses* — the function reaches back
into main's variables and modifies them directly.

<pre>
Pass by value (bad_swap):         Pass by pointer (swap):

main's stack:                     main's stack:
┌──────────┐                      ┌──────────┐
│ x = 10   │ ← unchanged         │ x = 10→20│ ← modified!
│ y = 20   │ ← unchanged         │ y = 20→10│ ← modified!
└──────────┘                      └──────────┘
                                       ▲   ▲
bad_swap's stack:                      │   │
┌──────────┐                     swap's stack:
│ x = 10→20│ (copies!)           ┌──────────┐
│ y = 20→10│ (copies!)           │ a = &x   │──┘
└──────────┘                     │ b = &y   │──┘
  discarded ↑                    └──────────┘
</pre>

---

## Arrays

An array is a contiguous block of memory holding elements of the same type.

```c
int scores[5] = {90, 85, 77, 92, 88};
```

<pre>
Memory layout of scores[5]:

Address:   0x100  0x104  0x108  0x10C  0x110
         ┌──────┬──────┬──────┬──────┬──────┐
         │  90  │  85  │  77  │  92  │  88  │
         └──────┴──────┴──────┴──────┴──────┘
Index:     [0]    [1]    [2]    [3]    [4]

Each int is 4 bytes, so addresses increase by 4.
</pre>

Key facts:
- Arrays are **zero-indexed**: first element is `scores[0]`
- Array size is fixed at compile time (in C99, Variable-Length Arrays or VLAs
  exist but avoid them — they allocate on the stack and can cause stack overflows)
- There is **no bounds checking** — accessing `scores[5]` is **undefined behavior**
  (the C standard makes no guarantees about what happens: your program might crash,
  produce garbage, or appear to work today but fail tomorrow)
- The array name `scores` **decays** to a pointer to its first element — meaning C
  automatically converts the array name to a pointer to `scores[0]`, losing its
  size information

### Array and pointer equivalence

This is one of C's most important concepts:

```c
int arr[5] = {10, 20, 30, 40, 50};

// These are IDENTICAL:
arr[2]       // subscript notation
*(arr + 2)   // pointer arithmetic

// The array name IS a pointer to element 0:
int *p = arr;     // same as: int *p = &arr[0];
p[3]              // same as arr[3] → 40
*(p + 3)          // same as arr[3] → 40
```

<pre>
Array-pointer equivalence:

arr → points to first element
      │
      ▼
┌──────┬──────┬──────┬──────┬──────┐
│  10  │  20  │  30  │  40  │  50  │
└──────┴──────┴──────┴──────┴──────┘
  arr     arr    arr    arr    arr
  +0      +1     +2     +3     +4

arr[i]  is the same as  *(arr + i)
&arr[i] is the same as  (arr + i)
</pre>

### Pointer arithmetic

When you add 1 to a pointer, it moves forward by the **size of the pointed-to
type**, not by 1 byte:

```c
int *p = arr;       // p points to arr[0]
p++;                // p now points to arr[1] (moved 4 bytes, not 1!)

char *c = "hello";
c++;                // c now points to 'e' (moved 1 byte — chars are 1 byte)
```

<pre>
Pointer arithmetic with int* (4 bytes each):

p       p+1     p+2     p+3
│       │       │       │
▼       ▼       ▼       ▼
┌──────┬──────┬──────┬──────┐
│ byte │ byte │ byte │ byte │  ← 16 bytes total
│ 0-3  │ 4-7  │ 8-11 │12-15│
└──────┴──────┴──────┴──────┘

p+1 means "move forward by sizeof(int) = 4 bytes"
</pre>

### Passing arrays to functions

When you pass an array to a function, it **decays** to a pointer. The function
receives a pointer to the first element, NOT a copy of the array:

```c
void print_array(int *arr, int size) {    // or: int arr[]
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int nums[5] = {1, 2, 3, 4, 5};
    print_array(nums, 5);    // array decays to pointer
    // sizeof(nums) == 20 (5 * 4 bytes) — only works in defining scope
}
```

**Important**: Inside the function, `sizeof(arr)` gives you the pointer size
(usually 8 bytes on 64-bit), NOT the array size. That's why you always pass
the size separately.

### Why Arrays Are Blazingly Fast

Arrays aren't just convenient — they're the fastest data structure for
sequential access. The reason is **cache lines**.

Your CPU doesn't read one byte at a time from RAM. It reads **64 bytes at
once** (a "cache line") and stores them in a tiny, ultra-fast memory called
the **L1 cache**. When you access arr[0], the CPU loads arr[0] through
arr[15] (for 4-byte ints) into cache. When you access arr[1], it's already
there — a "cache hit" that takes ~1 nanosecond instead of ~100 nanoseconds.

```
Memory Hierarchy:
┌─────────────────────────────────────────────────────┐
│ Register  │  < 1 ns   │  64 bytes    │ In the CPU  │
│ L1 Cache  │  ~1 ns    │  32-64 KB    │ Per core    │
│ L2 Cache  │  ~4 ns    │  256 KB      │ Per core    │
│ L3 Cache  │  ~12 ns   │  8-32 MB     │ Shared      │
│ RAM       │  ~100 ns  │  8-64 GB     │ Main memory │
│ SSD       │  ~100 µs  │  256 GB+     │ Storage     │
└─────────────────────────────────────────────────────┘

L1 cache is ~100x faster than RAM!
```

This is why iterating an array is so fast — the data is laid out
contiguously in memory, and the CPU prefetches the next cache line while
you're processing the current one. This is called "spatial locality."

---

## Strings in C

A string in C is just an **array of `char`** terminated by a null byte `'\0'`.

```c
char greeting[] = "Hello";
```

<pre>
String "Hello" in memory:

Index:    [0]  [1]  [2]  [3]  [4]  [5]
        ┌────┬────┬────┬────┬────┬────┐
        │ 'H'│ 'e'│ 'l'│ 'l'│ 'o'│'\0'│
        └────┴────┴────┴────┴────┴────┘
                                    ▲
                              null terminator
                              (marks end of string)

sizeof(greeting) = 6   (includes '\0')
strlen(greeting) = 5   (does NOT include '\0')
</pre>

### String literals vs. character arrays

```c
char arr[] = "Hello";    // Array on stack — you CAN modify it
char *ptr = "Hello";     // Pointer to read-only string literal — do NOT modify

arr[0] = 'J';   // OK: arr is now "Jello"
ptr[0] = 'J';   // UNDEFINED BEHAVIOR: string literals are read-only!
```

### Standard string functions

The `<string.h>` header provides:

| Function | Purpose | Example |
|----------|---------|---------|
| `strlen(s)` | Length (not counting `'\0'`) | `strlen("Hi")` → 2 |
| `strcpy(dst, src)` | Copy src into dst | `strcpy(buf, "Hello")` |
| `strncpy(dst, src, n)` | Copy at most n chars | Safer than `strcpy` |
| `strcmp(a, b)` | Compare: <0, 0, >0 | `strcmp("abc", "abd")` → negative |
| `strcat(dst, src)` | Append src to dst | Needs enough space! |
| `strchr(s, c)` | Find first occurrence of c | Returns pointer or NULL |

**Danger**: `strcpy` and `strcat` don't check buffer sizes. They're the #1
cause of buffer overflows. In real code, use `strncpy`/`strncat` or better
alternatives.

---

## Multi-Dimensional Arrays

```c
int matrix[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};
```

<pre>
2D array in memory (row-major order):

Logical view:               Memory layout (contiguous):
┌───┬───┬───┬───┐          ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ 4 │  row 0  │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │10 │11 │12 │
├───┼───┼───┼───┤          └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
│ 5 │ 6 │ 7 │ 8 │  row 1    row 0          row 1          row 2
├───┼───┼───┼───┤
│ 9 │10 │11 │12 │  row 2  matrix[r][c] is at offset: r * 4 + c
└───┴───┴───┴───┘
</pre>

Accessing elements: `matrix[row][col]`

---

## Common Pitfalls

### 1. Dangling pointer
```c
int *get_value(void) {
    int x = 42;
    return &x;        // BAD: x is destroyed when function returns
}                      // The returned pointer points to garbage
```

### 2. Uninitialized pointer
```c
int *p;               // p contains garbage — some random address
*p = 42;              // CRASH: writing to random memory
```

### 3. Buffer overflow
```c
char buf[5];
strcpy(buf, "Hello, World!");   // Writes past end of buf — undefined behavior
```

### 4. Off-by-one with strings
```c
char buf[5];
strcpy(buf, "Hello");   // BUG: "Hello" is 6 bytes (including '\0')
                         // but buf only holds 5
```

### 5. sizeof on pointer vs array
```c
int arr[10];
int *p = arr;
sizeof(arr);    // 40 (10 * 4 bytes)
sizeof(p);      // 8 (just the pointer size on 64-bit)
```

---

## Exercises

1. **pointers.c** — Pointer basics: address-of, dereference, swap, pointer
   arithmetic. (15 tests)

2. **strings.c** — Implement your own `strlen`, `strcpy`, `strcmp`, and
   `strcat` from scratch, then test them. (20 tests)

3. **arrays.c** — Array operations: reverse, search, rotate, merge, and
   2D matrix transpose. (18 tests)

4. **string_tokenizer.c** — Build a tokenizer that splits a string by a
   delimiter (like a simple `strtok`). (12 tests)

---

## Debug Challenge

| File | Description | Bugs |
|------|-------------|------|
| `debug_pointers.c` | Find and fix 5 common pointer and array bugs | 5 |

These exercises contain **intentionally broken code**. Your job is to find and
fix each bug. Each function has a comment explaining what it SHOULD do and a
HINT about the bug class. Run the program — failing tests tell you which
functions are still broken.

```bash
make debug    # compile the buggy version
./exercises/debug_pointers   # see which tests fail
# ... fix bugs ...
# recompile and rerun until all tests pass
```

---

## Key Takeaways

- A pointer stores an address; `&` gets the address; `*` follows the address
- Arrays decay to pointers when passed to functions — always pass the size too
- C strings are null-terminated `char` arrays — always account for `'\0'`
- Pointer arithmetic moves by `sizeof(type)`, not by 1 byte
- Never dereference NULL, uninitialized, or dangling pointers
- Never write past the end of an array — there are no guardrails

---

[← Previous: Module 05 — Recursion & Algorithmic Thinking](../05-recursion-algorithmic-thinking/README.md) | [Next: Module 07 — Dynamic Memory →](../07-dynamic-memory/README.md)
