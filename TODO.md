# C Katas — Progress Tracker

## Phase 1: How Computers Work & C Basics

### 00 — How a Computer Works
- [x] README lesson (CPU, RAM, binary, number systems)
- [x] Exercise: base_converter.c (21 tests)
- [x] Exercise: binary_printer.c
- [x] Solutions & Makefile

### 01 — Setup & First Program
- [x] README lesson (compilation pipeline, printf, streams)
- [x] Exercise: hello.c
- [x] Exercise: about_me.c
- [x] Exercise: explore_compilation.sh
- [x] Solutions & Makefile

### 02 — Types, Variables & Operators
- [x] README lesson (two's complement, IEEE 754, bitwise)
- [x] Exercise: type_explorer.c
- [x] Exercise: bit_operations.c (18 tests)
- [x] Exercise: float_mysteries.c
- [x] Solutions & Makefile

### 03 — Control Flow
- [x] README lesson (if/else, loops, boolean logic)
- [x] Exercise: fizzbuzz.c
- [x] Exercise: number_guess.c
- [x] Exercise: patterns.c
- [x] Exercise: prime_checker.c (28 tests)
- [x] Solutions & Makefile

### 04 — Functions & Program Structure
- [x] README lesson (call stack, scope, headers, separate compilation)
- [x] Exercise: calculator.c
- [x] Exercise: temperature.c
- [x] Exercise: math_lib.h / math_lib.c / math_lib_test.c (17 tests)
- [x] Solutions & Makefile

### 05 — Recursion & Algorithmic Thinking
- [x] README lesson (recursion, Big-O, divide and conquer)
- [x] Exercise: factorial.c (8 tests)
- [x] Exercise: fibonacci.c
- [x] Exercise: power.c (9 tests)
- [x] Exercise: hanoi.c
- [x] Exercise: recursive_strings.c (12 tests)
- [x] Solutions & Makefile

---

## Phase 2: Pointers & Memory

### 06 — Pointers & Arrays
- [x] README lesson (memory model, pointer syntax, arrays, strings)
- [x] Exercise: pointers.c (pointer basics, swap, dereference — 14 tests)
- [x] Exercise: strings.c (custom strlen/strcpy/strcmp/strcat/strchr/strrev — 20 tests)
- [x] Exercise: arrays.c (reverse, search, rotate, merge, transpose, remove dups — 15 tests)
- [x] Exercise: string_tokenizer.c (split by delimiter — 12 tests)
- [x] Solutions & Makefile

### 07 — Dynamic Memory
- [x] README lesson (stack vs heap, malloc/free, valgrind)
- [x] Exercise: string_builder.c (15 tests — grow with realloc)
- [x] Exercise: dynamic_array.c (17 tests — push/pop/get with auto-resize)
- [x] Exercise: leak_detective.c (10 tests — find leaks with valgrind)
- [x] Solutions & Makefile

### 08 — Mastering Memory Management
- [x] README lesson (virtual memory, cache hierarchy, custom allocators, fragmentation)
- [x] Exercise: arena.c (13 tests — bump allocator with alignment, reset, destroy)
- [x] Exercise: pool.c (13 tests — pool allocator for particle system)
- [x] Exercise: mini_malloc.c (11 tests — free-list allocator with coalescing)
- [x] Exercise: memory_profiler.c (9 tests — wrap malloc/free, report leaks)
- [x] Solutions & Makefile

---

## Phase 3: Data Structures & Algorithms

### 09 — Structs, Unions & Compound Types
- [x] README lesson (structs, padding, unions, enums, typedef, opaque types, ADTs)
- [x] Exercise: structs.c (14 tests — student record CRUD)
- [x] Exercise: tagged_union.c (13 tests — variant type: int/float/string)
- [x] Exercise: point.c (14 tests — 2D point/vector library with dot product)
- [x] Solutions & Makefile

### 10 — Function Pointers & Callbacks
- [x] README lesson (function pointers, callbacks, qsort, generic ops, polymorphism)
- [x] Exercise: callbacks.c (10 tests — event system with register/emit/clear)
- [x] Exercise: generic_sort.c (12 tests — bubble sort with comparator callbacks)
- [x] Exercise: command_dispatch.c (9 tests — string→function mapping)
- [x] Solutions & Makefile

### 11 — Linked Lists
- [x] README lesson (singly/doubly/circular linked lists, vs arrays, Big-O comparison)
- [x] Exercise: linked_list.c (18 tests — insert, delete, search, reverse, print)
- [x] Exercise: lru_cache.c (10 tests — doubly linked list LRU eviction)
- [x] Solutions & Makefile

### 12 — Stacks & Queues
- [x] README lesson (LIFO, FIFO, circular buffer, Shunting-Yard)
- [x] Exercise: stack.c (18 tests — balanced parentheses checker)
- [x] Exercise: queue.c (15 tests — circular buffer queue)
- [x] Exercise: expression_eval.c (8 tests — infix → postfix → evaluate)
- [x] Solutions & Makefile

### 13 — Hash Tables
- [x] README lesson (hash functions, chaining, open addressing, load factor, rehashing)
- [x] Exercise: hash_table.c (14 tests — chaining with collision handling)
- [x] Exercise: word_freq.c (8 tests — word frequency counter)
- [x] Solutions & Makefile

### 14 — Sorting & Searching
- [x] README lesson (binary search, all sorts, stability, Big-O deeper dive)
- [x] Exercise: sorting.c (15 tests — bubble/selection/insertion/merge/quicksort with comparison counts)
- [x] Exercise: binary_search.c (13 tests — basic, first/last occurrence, insert position)
- [x] Solutions & Makefile

### 15 — Trees & Heaps
- [x] README lesson (BST, AVL rotations, heaps, heap sort, priority queues)
- [x] Exercise: bst.c (16 tests — insert, search, delete all cases, inorder, height)
- [x] Exercise: heap.c (14 tests — min-heap, priority queue, heap sort)
- [x] Solutions & Makefile

---

## Phase 4: Systems Building Blocks

### 16 — File I/O & the Unix Philosophy
- [x] README lesson (file descriptors, buffered I/O, random access, Unix philosophy)
- [x] Exercise: hexdump.c (10 tests — hex dump with ASCII display)
- [x] Exercise: wc.c (10 tests — line/word/char counter)
- [x] Exercise: todo_app.c (12 tests — CRUD with fixed-size records and fseek)
- [x] Solutions & Makefile

### 17 — Text, Unicode & Encoding
- [x] README lesson (ASCII, Unicode, UTF-8 bit patterns, endianness)
- [x] Exercise: utf8.c (17 tests — encode/decode/strlen/validate from scratch)
- [x] Solutions & Makefile

### 18 — Date & Time
- [x] README lesson (Unix timestamps, time zones, calendar math)
- [x] Exercise: datetime.c (16 tests — leap year, days_in_month, day_of_week, days_between)
- [x] Solutions & Makefile

### 19 — The Preprocessor & Build Systems
- [x] README lesson (macros, conditional compilation, Makefiles, libraries)
- [x] Exercise: macros.c (14 tests — MIN/MAX/SWAP/ARRAY_SIZE/CLAMP/STRINGIFY/LOG)
- [x] Solutions & Makefile

### 20 — Graphs
- [x] README lesson (adjacency list, BFS, DFS, Dijkstra)
- [x] Exercise: graph.c (15 tests — BFS/DFS traversal, shortest path, has_path)
- [x] Solutions & Makefile

### 21 — Process & Concurrency
- [x] README lesson (fork/exec, signals, pipes, pthreads, mutex, condvar)
- [x] Exercise: producer_consumer.c (12 tests — bounded buffer with mutex + condvar)
- [x] Exercise: parallel_sum.c (8 tests — parallel array sum with pthreads)
- [x] Solutions & Makefile

---

## Phase 5: Building Real Systems

### 22 — Building a Text Editor
- [x] README lesson (raw mode, ANSI escape codes, gap buffer, event loop)
- [x] Exercise: gap_buffer.c (16 tests — insert/delete/move/extract)
- [x] Exercise: screen_buffer.c (10 tests — 2D character buffer rendering)
- [x] Solutions & Makefile

### 23 — Git Internals
- [x] README lesson (content-addressable storage, SHA-1, objects, DAGs, Merkle trees)
- [x] Exercise: sha1.c (12 tests — hash function, hex conversion, git blob format)
- [x] Exercise: objects.c (10 tests — in-memory object store with dedup)
- [x] Solutions & Makefile

### 24 — Database: Key-Value Store
- [x] README lesson (page-based storage, B-Tree with splitting, WAL, ACID)
- [x] Exercise: btree.c (14 tests — order-3 B-Tree with insert/search/split)
- [x] Exercise: kvstore.c (12 tests — sorted-array key-value store)
- [x] Solutions & Makefile

### 25 — Parsing & the SQL Engine
- [x] README lesson (tokenizer→parser→AST pipeline, recursive descent, SQL basics)
- [x] Exercise: tokenizer.c (12 tests — SQL tokenizer with keywords/idents/operators)
- [x] Exercise: eval.c (10 tests — arithmetic expression evaluator with precedence)
- [x] Solutions & Makefile

### 26 — Network & Socket Programming
- [x] README lesson (TCP/UDP, sockets API, HTTP, DNS, client-server)
- [x] Exercise: http_parser.c (14 tests — parse requests, extract headers, build responses)
- [x] Exercise: protocol.c (8 tests — text protocol parser/builder)
- [x] Solutions & Makefile

### 27 — Encryption & Security
- [x] README lesson (Caesar/XOR/Vigenere, RSA, Diffie-Hellman, TLS)
- [x] Exercise: ciphers.c (13 tests — Caesar encrypt/decrypt/crack, XOR, Vigenere)
- [x] Exercise: rsa_mini.c (10 tests — mod_pow, gcd, RSA keygen/encrypt/decrypt)
- [x] Solutions & Makefile

### 28 — Graphics Programming
- [x] README lesson (pixels, RGB, PPM format, Bresenham's line, transformations)
- [x] Exercise: ppm.c (14 tests — image create, draw line/rect, save PPM)
- [x] Solutions & Makefile

### 28b — Image Manipulation & Effects
- [x] README lesson (brightness/contrast, grayscale, convolution, blur, edge detection)
- [x] Exercise: image_ops.c (14 tests — brightness, grayscale, invert, sepia, convolve, blur, edge detect)
- [x] Solutions & Makefile

### 28c — Ray Tracing & 3D Graphics
- [x] README lesson (rays, sphere intersection, Phong lighting, reflections)
- [x] Exercise: vec3.c (14 tests — 3D vector math with reflect)
- [x] Exercise: raytracer.c (10 tests — ray-sphere intersection, diffuse shading)
- [x] Solutions & Makefile

---

## Phase 6: Advanced Topics & Capstone

### 29 — Machine Learning in C
- [x] README lesson (linear algebra, gradient descent, neural networks, backpropagation)
- [x] Exercise: matrix.c (14 tests — create, multiply, transpose, add, scale)
- [x] Exercise: neural_net.c (10 tests — sigmoid, forward pass, XOR training)
- [x] Solutions & Makefile

### 30 — Capstone Project
- [x] README (5 project ideas with architecture guides, planning template)
- [x] Exercise: mini_shell.c (12 tests — command parsing, pipe detection, builtins)
- [x] Solutions & Makefile
