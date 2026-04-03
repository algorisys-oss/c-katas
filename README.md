# C Katas — Systems Programming from First Principles

**30 modules. 1100+ tests. From "what is a computer" to building a database, ray tracer, and neural network — all in C.**

A ground-up C99 systems programming curriculum designed for absolute beginners who want deep, intuitive understanding of how computers actually work. No prior programming experience required.

## Why C?

C is not just a language — it's a lens into how computers work. When you write C, there are no hidden abstractions: you see the memory, you manage the pointers, you understand the machine. Every operating system, database, and game engine is built on these foundations.

This tutorial doesn't just teach C syntax. It teaches **systems thinking** — the ability to reason about memory layouts, data flow, and performance at the hardware level.

## What You'll Build

| Project | Module | What You'll Learn |
|---------|--------|-------------------|
| Binary converter | 00 | How computers represent numbers |
| Memory allocator | 08 | Arena and pool allocators from scratch |
| Hash table | 13 | The world's most useful data structure |
| Text editor | 22 | Gap buffers, terminal raw mode, ANSI escape codes |
| Git internals | 23 | SHA-1 hashing, content-addressable storage |
| Key-value database | 24 | B-trees, persistence, crash recovery |
| SQL engine | 25 | Tokenizer, recursive descent parser, evaluator |
| HTTP parser | 26 | TCP sockets, protocol parsing |
| Ray tracer | 28c | 3D vector math, Phong lighting, reflections |
| LRU cache | 13 | Hash tables + doubly linked lists, O(1) get/put |
| Segment tree | 15 | Range queries, point updates, prefix sums |
| Neural network | 29 | Matrix math, backpropagation, gradient descent |

## Prerequisites

- A Linux machine (or WSL on Windows)
- Basic computer literacy (using a terminal, editing text files)
- **No programming experience required** — Module 00 starts from binary

## Quick Start

```bash
# Install the toolchain (Ubuntu/Debian)
sudo apt update && sudo apt install gcc make valgrind

# Clone and start
git clone https://github.com/rajeshpillai/c-katas.git
cd c-katas

# Start with Module 00
cd 00-how-computers-work
cat README.md                    # Read the lesson
make exercises                   # Build the exercises
vim exercises/binary_printer.c   # Fill in the TODOs
make exercises && ./exercises/binary_printer  # Test your code
make test                        # Compare against reference solution
```

### Browse lessons in the browser

```bash
# Serve the website locally
python3 -m http.server 8080
# Open http://localhost:8080
```

## How Each Module Works

```
Every module:
┌─────────────────────────────────────────────────┐
│  README.md     — Concept notes, diagrams,       │
│                  first-principles explanations   │
│  exercises/    — Code with TODO markers          │
│                  (your job: fill them in)        │
│  solutions/    — Reference implementations       │
│  Makefile      — Build, test, and clean targets  │
└─────────────────────────────────────────────────┘

Workflow:
  1. Read the README (understand the concept)
  2. Edit the exercise file (fill in TODOs)
  3. make exercises && ./exercises/<name>  (run your code)
  4. make test  (compare against reference)
  5. valgrind ./exercises/<name>  (check for memory bugs)
```

### Compiler Flags

All code is compiled with:

```
gcc -Wall -Wextra -pedantic -std=c99
```

## Curriculum (33 Modules, 6 Phases)

### Phase 1: Foundations (Modules 00–05)
| # | Module | Key Concepts | Exercises |
|---|--------|-------------|-----------|
| 00 | How Computers Work | Binary, CPU, fetch-decode-execute | 2 |
| 01 | Setup & First Program | Compilation pipeline, printf, stdin/stdout | 2 |
| 02 | Types, Variables & Operators | int/float/char, two's complement, IEEE 754, bitwise | 3 |
| 03 | Control Flow | if/else, loops, flowcharts, De Morgan's laws | 4 |
| 04 | Functions & Program Structure | Call stack, scope, headers, pass-by-value | 4 |
| 05 | Recursion & Algorithmic Thinking | Base cases, Big-O, divide and conquer, memoization, DP, backtracking | 8 |

### Phase 2: Pointers & Memory (Modules 06–08)
| # | Module | Key Concepts | Exercises |
|---|--------|-------------|-----------|
| 06 | Pointers & Arrays | Address-of, dereference, pointer arithmetic, decay | 5 |
| 07 | Dynamic Memory | malloc/free, stack vs heap, ownership, valgrind, gdb | 4 |
| 08 | Mastering Memory Management | Virtual memory, arena allocators, pool allocators | 4 |

### Phase 3: Data Structures (Modules 09–15)
| # | Module | Key Concepts | Exercises |
|---|--------|-------------|-----------|
| 09 | Structs, Unions & Compound Types | Memory layout, padding, tagged unions, opaque types | 3 |
| 10 | Function Pointers & Callbacks | qsort, dispatch tables, polymorphism in C | 3 |
| 11 | Linked Lists | Singly/doubly linked, Y-shape intersection, Floyd's cycle, reverse K-group, palindrome | 5 |
| 12 | Stacks & Queues | LIFO/FIFO, monotonic stack, decode string, Min Stack, Queue from Stacks, deque | 6 |
| 13 | Hash Tables | Hash functions, collisions, open addressing, two sum, LRU cache | 5 |
| 14 | Sorting & Searching | Binary search, two pointers, sliding window, intervals, greedy, matrix, KMP/Rabin-Karp | 10 |
| 15 | Trees & Heaps | BST, AVL, heap, trie, segment/Fenwick tree, top-K, LCA, serialize/deserialize | 11 |

### Phase 4: Systems Building Blocks (Modules 16–21)
| # | Module | Key Concepts | Exercises |
|---|--------|-------------|-----------|
| 16 | File I/O & Unix Philosophy | File descriptors, buffered I/O, pipes, binary files | 3 |
| 17 | Text, Unicode & Encoding | ASCII, UTF-8, encoding/decoding, validation | 3 |
| 18 | Date & Time | Unix epoch, leap years, timezones, formatting | 1 |
| 19 | Preprocessor & Build Systems | Macros, conditional compilation, Makefiles | 1 |
| 20 | Graphs | BFS, DFS, Dijkstra, Bellman-Ford, Kruskal MST, union-find, cycle detection | 7 |
| 21 | Processes & Concurrency | fork/exec, threads, mutexes, atomics, producer-consumer | 3 |

### Phase 5: Building Real Systems (Modules 22–28c)
| # | Module | Key Concepts | Exercises |
|---|--------|-------------|-----------|
| 22 | Building a Text Editor | Gap buffer, raw terminal, ANSI escapes, event loop | 2 |
| 23 | Git Internals | SHA-1, blobs/trees/commits, content-addressable storage | 2 |
| 24 | Database & Key-Value Store | B-trees, disk pages, WAL, ACID, wire protocols | 2 |
| 25 | Parsing & SQL Engine | Tokenizer, recursive descent, AST, operator precedence | 2 |
| 26 | Network & Socket Programming | TCP/UDP, HTTP, Redis RESP, PostgreSQL protocol | 3 |
| 27 | Encryption & Security | Caesar, XOR, RSA, Diffie-Hellman, TLS overview | 2 |
| 28 | Graphics Programming | Pixels, PPM, Bresenham, circles, Mandelbrot | 3 |
| 28b | Image Manipulation & Effects | Convolution, blur, sharpen, edge detection, Sobel | 1 |
| 28c | Ray Tracing & 3D Graphics | Vectors, ray-sphere intersection, Phong lighting | 2 |

### Phase 6: Advanced & Capstone (Modules 29–30)
| # | Module | Key Concepts | Exercises |
|---|--------|-------------|-----------|
| 29 | Machine Learning in C | Linear algebra, gradient descent, backpropagation | 2 |
| 30 | Capstone Project | Mini shell, HTTP server, or SQL database | 1 |

## Estimated Completion Time

Total estimated effort: **~500 hours** (reading + coding + debugging).

Time splits roughly as: **40% reading concepts** | **35% writing code** | **25% debugging & tooling**

### By Weekly Commitment

| Hours/Week | Total Duration | Pace |
|:----------:|:--------------:|------|
| 4 hrs | ~2.5 years | Relaxed — good alongside school |
| 6 hrs | ~1.5–2 years | Steady — 1 module every ~10 days |
| 8 hrs | ~1–1.5 years | Focused — roughly 1 module/week |
| 10 hrs | ~1 year | Intensive — fast but sustainable |
| 12 hrs | ~10 months | Sprint — summer/gap-year pace |

### By Phase

| Phase | Modules | Est. Hours | 4 hr/wk | 6 hr/wk | 8 hr/wk | 10 hr/wk | 12 hr/wk |
|-------|---------|:----------:|:-------:|:-------:|:-------:|:--------:|:--------:|
| 1. Foundations | 00–05 | ~70 hrs | 18 wks | 12 wks | 9 wks | 7 wks | 6 wks |
| 2. Pointers & Memory | 06–08 | ~45 hrs | 11 wks | 8 wks | 6 wks | 5 wks | 4 wks |
| 3. Data Structures | 09–15 | ~90 hrs | 23 wks | 15 wks | 11 wks | 9 wks | 8 wks |
| 4. Systems Building Blocks | 16–21 | ~80 hrs | 20 wks | 13 wks | 10 wks | 8 wks | 7 wks |
| 5. Real Systems | 22–28c | ~170 hrs | 43 wks | 28 wks | 21 wks | 17 wks | 14 wks |
| 6. Advanced & Capstone | 29–30 | ~45 hrs | 11 wks | 8 wks | 6 wks | 5 wks | 4 wks |
| **Total** | **33 modules** | **~500 hrs** | **~126 wks** | **~84 wks** | **~63 wks** | **~51 wks** | **~43 wks** |

> **Note**: Build-heavy modules (text editor, database, SQL engine, ray tracer) take 2–3x longer than concept modules. Phase 5 is the heaviest — don't rush it. Estimates include time for reading, re-reading, and debugging with valgrind/GDB.

## Teaching Approach

- **First principles** — every concept is built from what's already known
- **Visuals for everything** — ASCII diagrams for memory layouts, data structures, algorithms
- **Why before how** — understand the motivation before seeing the code
- **Debug exercises** — intentionally broken code teaches you to find real bugs
- **Real systems** — build a text editor, database, and ray tracer, not toy examples
- **Tool mastery** — GDB, strace, valgrind taught alongside the code

See [CLAUDE.md](CLAUDE.md) for the full teaching philosophy and [AGENDA.md](AGENDA.md) for the detailed curriculum plan.

## Repository Structure

```
c-katas/
├── README.md           # This file
├── AGENDA.md           # Full curriculum with all topics
├── CLAUDE.md           # Teaching philosophy & conventions
├── site/               # Lesson browser (local web UI)
│   └── index.html
├── 00-how-computers-work/
│   ├── README.md       # Lesson with diagrams
│   ├── exercises/      # Your code (fill in TODOs)
│   ├── solutions/      # Reference implementations
│   └── Makefile        # Build & test
├── 01-setup-first-program/
│   └── ...
└── 30-capstone-project/
    └── ...
```

## License

[MIT](LICENSE)
