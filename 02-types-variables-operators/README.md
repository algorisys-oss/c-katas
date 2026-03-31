# Module 02 — Types, Variables & Operators

Everything in your computer's memory is just bits — zeros and ones. A **type**
tells the compiler how to interpret those bits. This module builds up every
data type from first principles so you understand what is really happening
when you write `int x = 42;`.

---

## 1. Variables — Named Boxes in Memory

A variable is a named box in memory. Every box has three properties:

```
    ADDRESS         CONTENT (bytes)             WHAT WE SEE
    (where)         (what is stored)            (how we read it)

                    +----+----+----+----+
    0x7FFE1234 ---> | 00 | 00 | 00 | 2A |  =  42  (as an int)
                    +----+----+----+----+
                    byte byte byte byte
                     0    1    2    3

    int answer = 42;
    ^   ^        ^
    |   |        +-- value: what goes in the box
    |   +----------- name:  how you refer to the box in code
    +--------------- type:  how many bytes, how to interpret them
```

When you write `int answer = 42;`, the compiler:
1. Reserves 4 bytes of memory (because `int` is typically 4 bytes)
2. Stores the value 42 in those bytes (as `0x0000002A` in hex)
3. Remembers that the name `answer` refers to address `0x7FFE1234`

**The type matters because the same bits mean different things depending
on the type:**

```
  Same bits in memory: 1100 0000 0100 1001 0000 1111 1101 1011

  As unsigned int:  3,226,206,171
  As signed int:   -1,068,761,125
  As float:              -3.14159...

  Same bits. Different meaning. The type is the decoder ring.
```

---

## 2. How Integers Work from the Ground Up

### Unsigned Integers: Just Binary Numbers

You already know decimal (base 10). Binary (base 2) works the same way,
but with only two digits: 0 and 1.

```
  DECIMAL: each position is a power of 10

    hundreds  tens  ones
      10^2    10^1  10^0
       1       4     2    =  1*100 + 4*10 + 2*1  =  142

  BINARY: each position is a power of 2

    128s  64s  32s  16s  8s  4s  2s  1s
    2^7  2^6  2^5  2^4  2^3 2^2 2^1 2^0
     1    0    1    0    1   0   1   0   =  128+32+8+2  =  170

  An 8-bit unsigned integer can hold: 0 to 255  (that is 2^8 - 1)
```

### Signed Integers: Two's Complement

How do you represent negative numbers with only 0s and 1s? The answer is
**two's complement**, a clever scheme that every modern computer uses.

The rule: the leftmost bit is the **sign bit**. If it is 1, the number
is negative.

**To negate a number (two's complement):**
1. Flip all bits (0 becomes 1, 1 becomes 0)
2. Add 1

```
  Example: represent -5 as an 8-bit signed integer

  Step 1: Start with +5 in binary
     0000 0101       (+5)

  Step 2: Flip all bits
     1111 1010       (ones' complement)

  Step 3: Add 1
     1111 1011       (-5 in two's complement!)

  Verify:   0000 0101    (+5)
          + 1111 1011    (-5)
          ----------
         1 0000 0000    = 0 (the carry-out 1 is discarded)
                          It works!
```

**8-bit two's complement number line:**

```
  BINARY         DECIMAL
  0111 1111  =   +127   <-- maximum positive value
  0111 1110  =   +126
  ...
  0000 0010  =   +2
  0000 0001  =   +1
  0000 0000  =    0
  1111 1111  =   -1
  1111 1110  =   -2
  ...
  1000 0001  =   -127
  1000 0000  =   -128   <-- minimum negative value

  Range: -128 to +127  (that is -2^7 to 2^7 - 1)
```

### Overflow: When You Exceed the Range

What happens when you go past the maximum value?

```
  Unsigned 8-bit:
    255 + 1 = ?

    1111 1111    (255)
  + 0000 0001    (1)
  -----------
  1 0000 0000    But we only have 8 bits!
    0000 0000    = 0     <-- it wraps around!

  255 + 1 = 0  for an unsigned 8-bit integer.
  This is called OVERFLOW.

  Signed 8-bit:
    127 + 1 = ?

    0111 1111    (127)
  + 0000 0001    (1)
  -----------
    1000 0000    = -128  <-- positive became negative!

  127 + 1 = -128  for a signed 8-bit integer.
  This is a BUG in most programs. Overflow is dangerous.
```

### Integer Types in C: Same Idea, Different Sizes

| Type               | Typical Size | Unsigned Range     | Signed Range                |
|--------------------|-------------|--------------------|-----------------------------|
| `char`             | 1 byte      | 0 to 255           | -128 to 127                 |
| `short`            | 2 bytes     | 0 to 65,535        | -32,768 to 32,767           |
| `int`              | 4 bytes     | 0 to 4,294,967,295 | -2,147,483,648 to 2,147,483,647 |
| `long`             | 8 bytes*    | 0 to 2^64-1        | -2^63 to 2^63-1             |
| `long long`        | 8 bytes     | 0 to 2^64-1        | -2^63 to 2^63-1             |

*`long` is 4 bytes on some systems, 8 on others. Use `sizeof` to check.

The C standard only guarantees **minimum** sizes:
- `char` >= 1 byte, `short` >= 2 bytes, `int` >= 2 bytes, `long` >= 4 bytes

Your exercises will use `sizeof` to discover the actual sizes on your system.

---

## 3. How Floating Point Works from the Ground Up

### The Problem

How do you represent 3.14 in binary? You cannot just count in whole numbers.
You need a way to express fractions and very large or very small numbers.

### Scientific Notation Analogy

In decimal, you use scientific notation to handle big and small numbers:

```
  3.14        = 3.14  x 10^0
  314,000     = 3.14  x 10^5
  0.000314    = 3.14  x 10^(-4)

  Format:  significand  x  base ^ exponent
```

Computers do the same thing, but in binary. This is the **IEEE 754** standard.

### IEEE 754: How a float is Stored (32-bit single precision)

```
  32 bits total:

   sign    exponent (8 bits)        mantissa / fraction (23 bits)
    |      |                        |
    v      v                        v
  +----+--------+-----------------------------------------------+
  | 0  |01111110|01000000000000000000000                        |
  +----+--------+-----------------------------------------------+
  bit   bits     bits
  31    30-23    22-0

  sign:      0 = positive, 1 = negative
  exponent:  biased by 127 (stored value - 127 = actual exponent)
  mantissa:  the fractional part after "1." (the leading 1 is implied)
```

**Example: how is 0.625 stored?**

```
  Step 1: Convert 0.625 to binary
    0.625 x 2 = 1.25  --> 1  (take the integer part)
    0.25  x 2 = 0.5   --> 0
    0.5   x 2 = 1.0   --> 1
    0.625 in binary = 0.101

  Step 2: Normalize (like scientific notation)
    0.101 = 1.01 x 2^(-1)

  Step 3: Encode
    sign     = 0        (positive)
    exponent = -1 + 127 = 126 = 0111 1110
    mantissa = 01000... (drop the leading "1.", store the rest)

  Result:  0  01111110  01000000000000000000000
```

### Why 0.1 + 0.2 != 0.3

Some decimals cannot be exactly represented in binary, just like 1/3 cannot
be exactly represented in decimal (0.33333... forever).

Let's see *exactly* why by converting 0.1 to binary step by step.

#### Step-by-Step: Converting 0.1 (Decimal) to Binary

To convert a decimal fraction to binary, you **repeatedly multiply by 2**
and take the integer part:

```
  Converting 0.1 to binary:

  0.1 × 2 = 0.2  → integer part = 0
  0.2 × 2 = 0.4  → integer part = 0
  0.4 × 2 = 0.8  → integer part = 0
  0.8 × 2 = 1.6  → integer part = 1    ← first '1' bit!
  0.6 × 2 = 1.2  → integer part = 1
  0.2 × 2 = 0.4  → integer part = 0    ← we're back to 0.2!
  0.4 × 2 = 0.8  → integer part = 0       the pattern repeats
  0.8 × 2 = 1.6  → integer part = 1       forever: 0011 0011 0011...
  ...

  Result: 0.1 (decimal) = 0.0 0011 0011 0011 0011... (binary, repeating)
                                ────  ────  ────
                                repeating block: 0011
```

The same thing happens with 0.2:

```
  0.2 (decimal) = 0.0011 0011 0011 0011... (binary, repeating)
```

**The key insight**: just like 1/3 = 0.333... can never be written exactly
in decimal (you'd need infinite digits), 1/10 = 0.1 can never be written
exactly in binary. The pattern `0011` repeats forever, but the computer only
has 23 bits (float) or 52 bits (double) of mantissa to store it. It must
**truncate** — chop off the infinite tail.

```
  What the computer actually stores:

  0.1 ≈ 0.0001100110011001100110011  (23 mantissa bits, then STOP)
                                  ↑
                              truncated here — lost precision!

  0.2 ≈ 0.0011001100110011001100110  (23 mantissa bits, then STOP)

  When you add these truncated values:

    0.1 (truncated) + 0.2 (truncated) = 0.30000000000000004...

  NOT exactly 0.3. This is NOT a bug in C. It is a fundamental
  limitation of representing base-10 fractions in base-2.
```

**Why does this matter?** Because a tiny rounding error in each number
compounds when you add them. The stored 0.1 is slightly more than 0.1,
and the stored 0.2 is slightly more than 0.2, so their sum overshoots 0.3
by a tiny amount — about 0.00000000000000004.

**Rule of thumb:**
- Money = use integers in cents, not floats in dollars
- Precise comparison = never use `==` with floats; check if the difference
  is smaller than a tiny threshold (called epsilon)

### Double Precision (64-bit)

| Format   | Size     | Sign | Exponent | Mantissa | Decimal digits |
|----------|----------|------|----------|----------|----------------|
| `float`  | 32 bits  | 1    | 8 bits   | 23 bits  | ~7 digits      |
| `double` | 64 bits  | 1    | 11 bits  | 52 bits  | ~15 digits     |

Use `double` by default. Only use `float` when memory matters (graphics,
large arrays of millions of values).

---

## 4. Boolean: True and False

In C, there is no built-in `true` / `false` before C99. The rule is simple:

```
  0           = false
  anything else = true  (1, -1, 42, 0.5 -- all true)
```

C99 added `<stdbool.h>`, which gives you `bool`, `true`, and `false`:

```c
#include <stdbool.h>

bool is_ready = true;    /* stored as 1 */
bool is_done  = false;   /* stored as 0 */
```

Under the hood, `bool` is just an integer (typically 1 byte).

---

## 5. Constants: Values That Never Change

Three ways to define constants in C:

```c
/* 1. const -- compiler-enforced, has a type */
const int MAX_STUDENTS = 30;

/* 2. #define -- preprocessor text replacement, no type checking */
#define PI 3.14159

/* 3. enum -- named integer constants */
enum color { RED = 0, GREEN = 1, BLUE = 2 };
```

| Method    | Type-safe? | Scope     | Debugger sees it? |
|-----------|-----------|-----------|-------------------|
| `const`   | Yes       | Normal    | Yes               |
| `#define` | No        | File-wide | No (replaced before compile) |
| `enum`    | Yes (int) | Normal    | Yes               |

---

## 6. Operators from First Principles

### Arithmetic Operators

```c
int a = 17, b = 5;
a + b    /* 22   addition */
a - b    /* 12   subtraction */
a * b    /* 85   multiplication */
a / b    /* 3    integer division (truncates, no remainder) */
a % b    /* 2    modulo (remainder after division: 17 = 3*5 + 2) */
```

**Careful:** integer division truncates. `17 / 5` is `3`, not `3.4`.
If you want the decimal, use floats: `17.0 / 5.0` gives `3.4`.

### Comparison Operators (result is always 0 or 1)

```c
a == b    /* 0  (false) -- equal? */
a != b    /* 1  (true)  -- not equal? */
a < b     /* 0  (false) -- less than? */
a > b     /* 1  (true)  -- greater than? */
a <= b    /* 0  (false) -- less than or equal? */
a >= b    /* 1  (true)  -- greater than or equal? */
```

### Logical Operators

```c
int x = 1, y = 0;
x && y    /* 0  (true AND false = false) */
x || y    /* 1  (true OR  false = true)  */
!x        /* 0  (NOT true = false)       */
!y        /* 1  (NOT false = true)       */
```

**Short-circuit evaluation:** `&&` and `||` stop as soon as the result
is known. `if (x != 0 && 100 / x > 5)` is safe because if `x` is zero,
the division is never evaluated — C already knows the whole expression is false.

### Bitwise Operators

These operate on individual bits. They are the tools for low-level
programming: flags, masks, hardware registers, encryption.

**Truth tables:**

```
  AND (&)        OR (|)         XOR (^)        NOT (~)
  A  B | A&B     A  B | A|B     A  B | A^B     A | ~A
  -----+----     -----+----     -----+----     ---+---
  0  0 |  0      0  0 |  0      0  0 |  0      0 |  1
  0  1 |  0      0  1 |  1      0  1 |  1      1 |  0
  1  0 |  0      1  0 |  1      1  0 |  1
  1  1 |  1      1  1 |  1      1  1 |  0

  AND: both must be 1
  OR:  at least one must be 1
  XOR: exactly one must be 1 (exclusive or)
  NOT: flip every bit
```

**Shift operators:**

```
  Left shift (<<):  shift bits left, fill right with 0s
    0000 1010 << 2  =  0010 1000     (10 << 2 = 40, same as 10 * 4)

  Right shift (>>): shift bits right, fill left with 0s (unsigned)
    0010 1000 >> 2  =  0000 1010     (40 >> 2 = 10, same as 40 / 4)

  Left shift by N  = multiply by 2^N
  Right shift by N = divide by 2^N
```

**Why bitwise matters:**

```
  1. Flags:     store 8 booleans in 1 byte (bit 0 = readable, bit 1 = writable, ...)
  2. Masks:     extract specific bits from a value
  3. Fast math: << 1 is multiply by 2 (faster than * 2 on old hardware)
  4. Hardware:  device registers use individual bits for different settings
  5. Crypto:    XOR is the basis of many encryption algorithms
```

---

## 7. sizeof — How Many Bytes?

`sizeof` tells you how many bytes a type or variable occupies:

```c
printf("char:      %zu bytes\n", sizeof(char));       /* always 1 */
printf("short:     %zu bytes\n", sizeof(short));      /* usually 2 */
printf("int:       %zu bytes\n", sizeof(int));        /* usually 4 */
printf("long:      %zu bytes\n", sizeof(long));       /* 4 or 8   */
printf("long long: %zu bytes\n", sizeof(long long));  /* usually 8 */
printf("float:     %zu bytes\n", sizeof(float));      /* usually 4 */
printf("double:    %zu bytes\n", sizeof(double));     /* usually 8 */
```

`sizeof` is evaluated at **compile time**, not runtime. It costs nothing.

---

## 8. Type Casting — Reinterpreting Bits

Sometimes you need to tell the compiler "treat this value as a different type":

```c
int a = 7, b = 2;
double result = (double)a / (double)b;   /* 3.5, not 3 */
```

Without the cast, `7 / 2` is integer division and gives `3`.

### Implicit Conversions: The Integer Promotion Trap

C automatically converts smaller types to larger types in expressions.
This is called **integer promotion** and it can surprise you:

```c
char a = 100;
char b = 100;
char c = a + b;   /* Bug! a + b = 200, but char max is 127 (signed) */

/* What actually happens:
 *   1. a is promoted to int (value: 100)
 *   2. b is promoted to int (value: 100)
 *   3. 100 + 100 = 200 (as int, this is fine)
 *   4. 200 is stored back in a char -- OVERFLOW!
 *   5. c = -56 (on most systems, with signed char)
 */
```

**Rule:** `char + char = int`, not `char`. The compiler promotes both
operands to `int` before doing arithmetic. This is part of the C standard.

---

## 9. The Big Idea: Data Representation

Everything in memory is just bits. A `char`, an `int`, a `float`, a pixel
color, a sound sample, a network packet — all just patterns of 0s and 1s.

The **type** is what gives meaning to the bits. Change the type, and the
same bits tell a completely different story.

This is the most important idea in systems programming.

---

## Katas

The exercises are in the `exercises/` directory:

1. **type_explorer.c** — Use `sizeof` to discover type sizes on your system,
   demonstrate overflow behavior, and print type ranges using `<limits.h>`.

2. **bit_operations.c** — Implement four bit manipulation functions:
   `set_bit`, `clear_bit`, `toggle_bit`, `check_bit`. A test harness is
   provided.

3. **float_mysteries.c** — Explore floating point quirks: why 0.1 + 0.2
   is not 0.3, why equality comparison fails, and how precision is lost.

Build all exercises with `make` from this directory. Solutions are in
`solutions/`. Build them with `make solutions`.

---

[← Previous: Module 01 — Setup & First Program](../01-setup-first-program/README.md) | [Next: Module 03 — Control Flow →](../03-control-flow/README.md)
