# Module 00 — How a Computer Works

> Before you write a single line of code, let's crack open the machine and see
> what's actually going on in there. Every program you'll ever write boils down
> to the ideas in this module.

---

## Table of Contents

1. [What's Inside a Computer](#whats-inside-a-computer)
2. [The Fetch-Decode-Execute Cycle](#the-fetch-decode-execute-cycle)
3. [Why Binary?](#why-binary)
4. [Number Systems from Scratch](#number-systems-from-scratch)
5. [Bits, Bytes, and Beyond](#bits-bytes-and-beyond)
6. [What Is a Program?](#what-is-a-program)
7. [What Is an Operating System?](#what-is-an-operating-system)
8. [Katas](#katas)

---

## What's Inside a Computer

If you ripped the lid off a computer, you'd find a few key parts. Here's the
big picture:

```
    +-------------------------------------------------------+
    |                     COMPUTER                          |
    |                                                       |
    |   +----------+                    +-----------+       |
    |   |          |                    |           |       |
    |   |   CPU    |<======BUS========>|    RAM    |       |
    |   |  (brain) |                    | (workspace)|      |
    |   |          |                    |           |       |
    |   +----------+                    +-----------+       |
    |        ^                               ^              |
    |        |                               |              |
    |        +============BUS================+              |
    |        |                                              |
    |        v                                              |
    |   +-----------+          +------------------+         |
    |   |           |          |                  |         |
    |   |  STORAGE  |          |  INPUT / OUTPUT  |         |
    |   | (hard     |          |  (keyboard,      |         |
    |   |  drive /  |          |   screen, mouse) |         |
    |   |  SSD)     |          |                  |         |
    |   +-----------+          +------------------+         |
    |                                                       |
    +-------------------------------------------------------+
```

### CPU — The Brain

The **CPU** (Central Processing Unit) is where all the actual *thinking*
happens. But "thinking" is a generous word — the CPU is really just incredibly
fast at doing simple things:

- Add two numbers
- Compare two numbers
- Move data from one place to another
- Jump to a different instruction

That's basically it. The magic is that it does these billions of times per
second.

```
    +------------------------------------+
    |              CPU                   |
    |                                    |
    |  +------------+  +-------------+  |
    |  |  Control   |  | Arithmetic  |  |
    |  |   Unit     |  | Logic Unit  |  |
    |  |   (CU)     |  |   (ALU)     |  |
    |  |            |  |             |  |
    |  | "what do   |  | "do the     |  |
    |  |  I do      |  |  math and   |  |
    |  |  next?"    |  |  logic"     |  |
    |  +------------+  +-------------+  |
    |                                    |
    |  +-----+ +-----+ +-----+ +-----+ |
    |  | R0  | | R1  | | R2  | | R3  | |
    |  +-----+ +-----+ +-----+ +-----+ |
    |       Registers (tiny fast        |
    |        storage inside CPU)        |
    +------------------------------------+
```

**Registers** are tiny storage slots *inside* the CPU itself. They hold the
data the CPU is working on right this moment. There are usually only a handful
of them (maybe 16 or 32), and each one holds a single number.

### RAM — The Workspace

**RAM** (Random Access Memory) is the computer's short-term memory. It's where
the computer keeps:

- The program it's currently running (the instructions)
- The data that program is working with

RAM is *fast* but *temporary* — when you turn the computer off, everything in
RAM vanishes. Think of it as a huge whiteboard that gets erased every night.

```
    RAM: A long row of numbered boxes
    +------+------+------+------+------+------+------+------+
    | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | ... |
    |  72  | 101  | 108  | 108  | 111  |  0   |  42  |     |
    +------+------+------+------+------+------+------+------+
      ^                                          ^
      |                                          |
    Each box has an ADDRESS      Each box holds one BYTE
    (its position number)        (a number from 0 to 255)
```

Every box in RAM has an **address** — its position number. When the CPU wants
to read or write data, it sends an address over the bus and says "give me
what's in box number 3" or "put the value 42 into box number 6."

### Storage — The Filing Cabinet

**Storage** (hard drive, SSD) is the computer's long-term memory. It keeps
your files, programs, photos, and everything else even when the power is off.
It's much slower than RAM but much bigger and permanent.

When you open a program, the computer copies it from storage into RAM so the
CPU can work with it at full speed.

### The Bus — The Highway

The **bus** is just wires that connect everything together. Data travels along
the bus between the CPU, RAM, and storage. Think of it as a highway between
the city's buildings. There are actually several buses:

- **Data bus**: carries the actual data being moved around
- **Address bus**: carries the location ("where should this data go?")
- **Control bus**: carries signals like "read" or "write"

```
    CPU <=== Address Bus ===> RAM
        <=== Data Bus    ===>
        <=== Control Bus ===>

    "Hey RAM, give me the byte at address 0x03"
         CPU ---[address: 0x03]---[control: READ]---> RAM
         CPU <--[data: 108]---------------------------RAM
    "Got it. That's the letter 'l'."
```

---

## The Fetch-Decode-Execute Cycle

This is the heartbeat of every computer. Every single thing a computer does
comes down to this loop repeating over and over, billions of times per second:

```
         +------------------+
         |                  |
         v                  |
    +---------+             |
    |  FETCH  |  Get the next instruction     |
    |         |  from memory (RAM)            |
    +---------+             |
         |                  |
         v                  |
    +---------+             |
    | DECODE  |  Figure out what the          |
    |         |  instruction means            |
    +---------+             |
         |                  |
         v                  |
    +---------+             |
    | EXECUTE |  Do it!     |
    |         |             |
    +---------+             |
         |                  |
         +------------------+
              (repeat forever)
```

Let's walk through an example. Say the CPU needs to add 3 + 5:

```
    STEP 1 — FETCH
    +---------+         +----------------------------------+
    |   CPU   | ------> | RAM                              |
    |         |  "give  | ...                              |
    | PC: 100 |  me     | [100]: ADD R0, 3, 5              |
    |         |  addr   | [101]: STORE R0, 0x50            |
    |         |  100"   | ...                              |
    +---------+         +----------------------------------+
    The CPU uses the Program Counter (PC) to know which
    instruction to fetch. Right now, PC = 100.

    STEP 2 — DECODE
    +---------+
    |   CPU   |  "ADD R0, 3, 5"
    |         |  Okay, I need to:
    | Control |  - add the numbers 3 and 5
    |  Unit   |  - store the result in register R0
    +---------+

    STEP 3 — EXECUTE
    +---------+
    |   CPU   |
    |   ALU   |  3 + 5 = 8
    |  R0: 8  |  Done! Result is in R0.
    +---------+

    Now PC becomes 101, and the cycle starts again.
```

The CPU doesn't "know" what adding means in any deep sense. It's just
electrical circuits wired to combine signals in a way that produces the right
output. But do it billions of times a second with the right instructions, and
you get video games, websites, and AI chatbots.

---

## Why Binary?

Here's the thing: computers run on electricity. And electricity through a wire
is either **on** or **off**. There's no "sort of on." This gives us two states:

```
    ON  = 1          OFF = 0
    +-------+        +-------+
    | LLLLL |        |       |
    | LLLLL |        |       |
    | LIGHT |        | DARK  |
    +-------+        +-------+
```

A single on-or-off value is called a **bit** (binary digit). It's the smallest
possible unit of information: yes/no, true/false, 1/0.

One bit isn't very useful — it can only represent two things. But string a
bunch of bits together, and you can represent anything:

```
    1 bit  =  2 possible values    (0, 1)
    2 bits =  4 possible values    (00, 01, 10, 11)
    3 bits =  8 possible values    (000 ... 111)
    4 bits = 16 possible values    (0000 ... 1111)
    8 bits = 256 possible values   (00000000 ... 11111111)
```

With 8 bits (one **byte**), you can represent any number from 0 to 255. That's
enough for a letter of the alphabet, a pixel's color channel, or a small
number. String billions of bytes together, and you can store anything.

---

## Number Systems from Scratch

### Decimal (Base 10) — What You Already Know

You've been using base 10 your whole life. Let's look at *why* it works.

The number **347** means:

```
    Hundreds    Tens    Ones
      10^2      10^1    10^0
       100       10       1
    +--------+--------+--------+
    |   3    |   4    |   7    |
    +--------+--------+--------+

    3 x 100  +  4 x 10  +  7 x 1  =  347
```

Each position is worth 10 times more than the one to its right. That's why
it's called "base 10" — each place is a power of 10.

The digits you can use are **0 through 9** (that's 10 digits — one fewer than
the base).

### Binary (Base 2) — How Computers Count

Binary works exactly the same way, but each position is worth **2** times more
than the one to its right, and you only have two digits: **0** and **1**.

The binary number **1011** means:

```
    Eights    Fours    Twos    Ones
     2^3      2^2      2^1     2^0
      8        4        2       1
    +--------+--------+--------+--------+
    |   1    |   0    |   1    |   1    |
    +--------+--------+--------+--------+

    1 x 8  +  0 x 4  +  1 x 2  +  1 x 1  =  11 (in decimal)
```

Let's count from 0 to 15 in binary:

```
    Decimal  |  Binary  |  Breakdown
    ---------|----------|----------------------------
       0     |   0000   |  0+0+0+0
       1     |   0001   |  0+0+0+1
       2     |   0010   |  0+0+2+0
       3     |   0011   |  0+0+2+1
       4     |   0100   |  0+4+0+0
       5     |   0101   |  0+4+0+1
       6     |   0110   |  0+4+2+0
       7     |   0111   |  0+4+2+1
       8     |   1000   |  8+0+0+0
       9     |   1001   |  8+0+0+1
      10     |   1010   |  8+0+2+0
      11     |   1011   |  8+0+2+1
      12     |   1100   |  8+4+0+0
      13     |   1101   |  8+4+0+1
      14     |   1110   |  8+4+2+0
      15     |   1111   |  8+4+2+1
    ---------|----------|----------------------------
```

Notice the pattern? The rightmost column alternates every number. The next
column alternates every 2 numbers. Then every 4. Then every 8. Just like how
in decimal, the ones digit cycles 0-9, then the tens digit ticks up.

### Hexadecimal (Base 16) — Shorthand for Binary

Binary is great for computers but painful for humans. Who wants to read
`11111111` when you could just write `FF`?

Hexadecimal (hex) uses base 16. Since we need 16 digits but only have 10
number symbols (0-9), we borrow the first six letters: **A=10, B=11, C=12,
D=13, E=14, F=15**.

```
    Hex digit:   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    Value:       0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
```

Why is hex so handy? Because each hex digit maps *perfectly* to exactly 4 bits:

```
    Hex  |  Binary  |  Decimal
    -----|----------|----------
      0  |  0000    |    0
      1  |  0001    |    1
      2  |  0010    |    2
      3  |  0011    |    3
      4  |  0100    |    4
      5  |  0101    |    5
      6  |  0110    |    6
      7  |  0111    |    7
      8  |  1000    |    8
      9  |  1001    |    9
      A  |  1010    |   10
      B  |  1011    |   11
      C  |  1100    |   12
      D  |  1101    |   13
      E  |  1110    |   14
      F  |  1111    |   15
    -----|----------|----------
```

So converting between binary and hex is just grouping bits into fours:

```
    Binary:  1111 1111
    Hex:      F    F      =  0xFF  =  255

    Binary:  1010 0011
    Hex:      A    3      =  0xA3  =  163

    Binary:  0000 1101
    Hex:      0    D      =  0x0D  =  13
```

The `0x` prefix is just a convention that means "this number is in
hexadecimal." You'll see it everywhere in C.

### Converting Between Bases — The Algorithm

**Decimal to Binary (repeated division by 2):**

Let's convert 42 to binary:

```
    42 / 2 = 21  remainder 0   <-- least significant bit (rightmost)
    21 / 2 = 10  remainder 1
    10 / 2 =  5  remainder 0
     5 / 2 =  2  remainder 1
     2 / 2 =  1  remainder 0
     1 / 2 =  0  remainder 1   <-- most significant bit (leftmost)

    Read remainders bottom-to-top: 101010

    Check: 32 + 0 + 8 + 0 + 2 + 0 = 42  ✓
```

This works for any base. Decimal to hex? Divide by 16 instead:

```
    255 / 16 = 15  remainder 15 (F)
     15 / 16 =  0  remainder 15 (F)

    Read bottom-to-top: FF

    0xFF = 255  ✓
```

**Binary to Decimal (add up the place values):**

Convert `110101` to decimal:

```
    Position:  5    4    3    2    1    0
    Power:    2^5  2^4  2^3  2^2  2^1  2^0
    Value:     32   16    8    4    2    1
    Digit:      1    1    0    1    0    1

    32 + 16 + 0 + 4 + 0 + 1 = 53
```

---

## Bits, Bytes, and Beyond

```
    1 bit          =  a single 0 or 1
    8 bits         =  1 byte   (can hold 0-255)
    1024 bytes     =  1 kilobyte  (KB)
    1024 KB        =  1 megabyte  (MB)
    1024 MB        =  1 gigabyte  (GB)
    1024 GB        =  1 terabyte  (TB)
```

**Why 8 bits in a byte?** It's partly historical — early computers used
various sizes (6, 7, 8 bits). 8 won out because it's a clean power of 2, it's
enough to hold a character of text (the ASCII standard uses 7 bits, and 8
gives a spare bit), and it divides evenly into larger units.

**Why 1024 instead of 1000?** Because 1024 is 2^10 — it's the power of 2
closest to 1000. Computers think in powers of 2, so the units follow suit.
(Technically the precise term is "kibibyte" for 1024 bytes, but most people
just say kilobyte.)

```
    How much can different amounts of memory hold?

    1 byte      "A"                  (one character)
    10 bytes    "Hello, Mom"         (a short message)
    1 KB        A few paragraphs     (a short email)
    1 MB        A book               (~500 pages)
    1 GB        ~250 songs           (in MP3 format)
    1 TB        ~500 hours of video  (in HD)
```

---

## What Is a Program?

A program is just a **list of instructions** that tells the CPU what to do.

When you write code in C (or any language), it looks like something a human
can read:

```c
    int x = 3 + 5;
```

But the CPU can't read that. It only understands binary — its own specific
**machine language**. So there's a translation step:

```
    Your Code              Compiler            Machine Code
    (human-readable)   ->  (translator)    ->  (CPU-readable)

    int x = 3 + 5;    ->    gcc           ->  10110000 00000011
                                               00000100 00000101
                                               10100010 ...
```

The **compiler** is a program that translates your human-readable code into
machine instructions. For C, the compiler we'll use is `gcc`. You write the
instructions in English-ish syntax, and `gcc` turns it into binary the CPU
can execute.

```
    Source code          What you write     (.c files)
         |
         v
    Compiler (gcc)       The translator
         |
         v
    Machine code         What the CPU runs  (binary executable)
```

---

## What Is an Operating System?

The **operating system** (OS) is the first program that runs when you turn on
your computer. It's the boss — it manages everything else:

```
    +---------------------------------------------------+
    |  Your programs (browser, game, text editor...)    |
    +---------------------------------------------------+
    |           Operating System (Linux, macOS, Windows) |
    |                                                   |
    |  - Manages PROCESSES (running programs)           |
    |  - Manages MEMORY (who gets which RAM)            |
    |  - Manages FILES (reading/writing to disk)        |
    |  - Manages DEVICES (keyboard, screen, network)    |
    +---------------------------------------------------+
    |           Hardware (CPU, RAM, storage, etc.)       |
    +---------------------------------------------------+
```

### Processes — Programs in Motion

A **process** is a program that's currently running. Your code sitting on disk
is just a file. When you run it, the OS loads it into RAM, and it becomes a
process:

```
    File on disk            Process in RAM
    +------------+          +------------------+
    | my_program |  ---->   | my_program       |
    | (binary)   |  "run    | Code: ...        |
    +------------+   it!"   | Data: x=8, y=3   |
                            | Stack: ...       |
                            +------------------+
```

The OS can run multiple processes at the same time (or really fast switching
between them so it *looks* simultaneous — that's called multitasking).

### Files — Named Chunks of Data

A **file** is just a sequence of bytes on your storage device with a name
attached. That's it. A text file is bytes that happen to represent letters.
An image file is bytes that represent pixel colors. The OS keeps track of
where each file lives on disk.

### Memory — The Workspace

The OS decides how RAM is divided up among all running processes. Each process
gets its own section and can't accidentally mess with another's. This is
called **memory protection**, and it's why one crashed program doesn't bring
down your whole computer (usually).

```
    RAM Layout (simplified):
    +---------------------------+
    |   Operating System        |
    +---------------------------+
    |   Process A (browser)     |
    +---------------------------+
    |   Process B (your game)   |
    +---------------------------+
    |   Process C (music player)|
    +---------------------------+
    |   Free space              |
    +---------------------------+
```

---

## Katas

Time to make your hands do the thinking. Here are your exercises:

### Pencil-and-Paper Challenges (No Computer Needed)

1. **Convert these decimal numbers to binary:** 7, 13, 42, 100, 255
2. **Convert these binary numbers to decimal:** 1010, 11100, 10000001
3. **Convert these to hexadecimal:** 200, 255, 1024
4. **Convert these hex values to decimal:** 0x1A, 0x7F, 0xFF, 0x100
5. **Binary finger counting:** Hold up your hands. Each finger is a bit
   (up=1, down=0). Your right thumb is 2^0 (the ones place), right index is
   2^1, and so on. Count to 31 on one hand, and to 1023 on both. Seriously,
   try it!

```
    Finger counting in binary (right hand):

    Pinky  Ring  Middle  Index  Thumb
     2^4    2^3   2^2    2^1    2^0
      16     8      4      2      1

    All down = 00000 = 0
    Thumb up = 00001 = 1
    Index up = 00010 = 2
    Both up  = 00011 = 3
    ...
    All up   = 11111 = 31
```

### Coding Katas

These are in the `exercises/` directory. Each file has `TODO` markers where
you need to write code.

1. **`binary_printer.c`** — Write a function that takes an integer and prints
   its binary representation. You'll practice bit manipulation and see how
   numbers really look under the hood.

2. **`base_converter.c`** — Build a complete base converter that can go between
   decimal, binary, and hexadecimal. This is the "repeated division" algorithm
   brought to life in C code.

When you're stuck, the `solutions/` directory has reference implementations.
But try first! You learn more from struggling.

### Building and Testing

```bash
# Build the exercises (once you fill in the TODOs):
make exercises

# Build the reference solutions:
make solutions

# Build everything and run the solutions to see expected output:
make test
```

---

*Next up: Module 01 — where you'll actually write and compile your first C
program!*
