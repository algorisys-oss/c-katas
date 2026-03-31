# Module 08 — Mastering Memory Management

In Module 07, you learned the basics: `malloc`, `free`, and the heap. Now we go
deeper. This module answers the questions that separate beginners from systems
programmers:

- What is virtual memory, and why are your pointers "lying" to you?
- Why does accessing memory in order run faster than jumping around?
- When is `malloc` not good enough, and what do you use instead?
- How does `malloc` actually work inside?

By the end of this module, you'll have built **three custom memory allocators**
and a **memory profiler** — tools that give you control over memory that most
programmers never achieve.

---

## Virtual Memory — The Grand Illusion

When your program runs, it thinks it owns all of memory. Address 0x00000000
through 0xFFFFFFFFFFFFFFFF — all yours. But that's a lie. Your computer has
maybe 8 or 16 GB of RAM, shared among dozens of programs. How does this work?

**Virtual memory**: the OS gives each program its own private **virtual address
space**. Your pointers hold virtual addresses, not physical ones. The CPU and OS
work together to translate virtual addresses to real physical locations.

```
Your program sees:                    Reality (physical RAM):

Program A:                            ┌──────────────────┐
┌──────────────────┐                  │ Program B's data │ 0x0000
│ code   0x00400000│───┐              ├──────────────────┤
│ data   0x00600000│───┼──┐           │ Program A's code │ 0x1000
│ heap   0x01000000│───┼──┼──┐        ├──────────────────┤
│ stack  0x7FFF0000│───┼──┼──┼──┐     │ (free)           │ 0x2000
└──────────────────┘   │  │  │  │     ├──────────────────┤
                       │  │  │  │     │ Program A's stack│ 0x3000
Program B:             │  │  │  │     ├──────────────────┤
┌──────────────────┐   │  │  │  │     │ Program A's heap │ 0x4000
│ code   0x00400000│───┼──┼──┼──┼──┐  ├──────────────────┤
│ data   0x00600000│───┼──┼──┼──┼──┤  │ Program A's data │ 0x5000
└──────────────────┘   │  │  │  │  │  ├──────────────────┤
                       │  │  │  │  │  │ Program B's code │ 0x6000
     Page Table        │  │  │  │  │  └──────────────────┘
     translates ───────┘  │  │  │  │
     each address         └──┘──┘──┘
```

Both programs use virtual address `0x00400000` for their code, but the page
table maps them to different physical locations. Neither program can access
the other's memory — the OS enforces isolation.

### Pages — The Unit of Memory

Memory is managed in fixed-size chunks called **pages**, typically 4096 bytes
(4 KB).

```
Virtual address space:        Page table:              Physical RAM:
┌─────────┐ Page 0           ┌──────────┐            ┌─────────┐
│ 0x0000  │────────────────→ │ Phys: 0x5│──────────→ │ Page @5 │
├─────────┤ Page 1           ├──────────┤            ├─────────┤
│ 0x1000  │────────────────→ │ Phys: 0x2│──────────→ │ Page @2 │
├─────────┤ Page 2           ├──────────┤            ├─────────┤
│ 0x2000  │────────────────→ │ NOT IN   │            │ ...     │
├─────────┤                  │ RAM (on  │
│ ...     │                  │ disk!)   │ ← page fault triggers
└─────────┘                  └──────────┘   OS to load from disk
```

Key facts:
- The page table maps virtual pages to physical pages
- Not all pages need to be in RAM — unused pages can live on disk
- Accessing an unmapped page causes a **page fault** — the OS loads it from disk
- This is why your program can "use" more memory than physically exists

### Why This Matters to You

1. **Your pointer values are virtual** — the same pointer in two processes
   points to completely different physical memory
2. **Memory protection** — accessing addresses outside your allocation causes a
   segfault because the page table says "access denied"
3. **`malloc` uses virtual memory** — it asks the OS for pages via `mmap` or
   `sbrk`, then carves them into smaller allocations

---

## The Memory Hierarchy — Why Speed Varies

Not all memory access is equal. Accessing some memory is 100× faster than other
memory, even though your code looks the same.

```
┌──────────────────────────────────────────────────────────┐
│                     CPU                                  │
│  ┌──────────┐                                            │
│  │ Registers│  ~0.3 ns    (a few dozen, 64 bits each)    │
│  └────┬─────┘                                            │
│       ↓                                                  │
│  ┌──────────┐                                            │
│  │ L1 Cache │  ~1 ns      (32-64 KB, per core)           │
│  └────┬─────┘                                            │
│       ↓                                                  │
│  ┌──────────┐                                            │
│  │ L2 Cache │  ~4 ns      (256 KB - 1 MB, per core)      │
│  └────┬─────┘                                            │
│       ↓                                                  │
│  ┌──────────┐                                            │
│  │ L3 Cache │  ~10 ns     (4-32 MB, shared)               │
│  └────┬─────┘                                            │
└───────┼──────────────────────────────────────────────────┘
        ↓
   ┌──────────┐
   │   RAM    │  ~100 ns    (4-64 GB)
   └────┬─────┘
        ↓
   ┌──────────┐
   │ SSD/Disk │  ~100,000 ns (100-2000 GB)
   └──────────┘
```

The key insight: **RAM is 100× slower than L1 cache**. And disk is 1000× slower
than RAM. Programs that keep their data in cache run dramatically faster.

### Locality — The Secret to Fast Programs

The cache works automatically — the hardware loads data near what you just
accessed, betting you'll need it next. This bet pays off when your program
has good **locality**:

**Spatial locality** — accessing data that's near other recently accessed data:

```c
/* GOOD: sequential access — cache-friendly */
int sum = 0;
for (int i = 0; i < 1000000; i++) {
    sum += arr[i];    /* each access is next to the previous one */
}

/* BAD: random access — cache-unfriendly */
int sum = 0;
for (int i = 0; i < 1000000; i++) {
    sum += arr[random_indices[i]];    /* jumping all over memory */
}
```

The sequential version can be 5-10× faster because the cache pre-loads nearby
elements. The random version causes constant **cache misses** — the data isn't
in cache, so the CPU waits for RAM.

**Temporal locality** — accessing the same data repeatedly:

```c
/* GOOD: reuse the same variable */
int total = 0;
for (int i = 0; i < n; i++) {
    total += arr[i];    /* total stays in cache */
}

/* BAD: touching many variables once */
for (int i = 0; i < n; i++) {
    totals[i] = arr[i] + other[i];    /* totals array may not fit in cache */
}
```

**Why this matters for data structures:**
- Arrays are cache-friendly (contiguous memory)
- Linked lists are cache-unfriendly (nodes scattered across heap)
- This is why arrays often beat linked lists in practice, even when Big-O
  says the linked list should win

---

## Why Custom Allocators?

`malloc` is general-purpose: it handles any size allocation, any pattern of
alloc/free, any number of threads. But generality costs performance.

If you know your specific allocation pattern, you can build a custom allocator
that's much faster:

```
┌──────────────────────────────────────────────────────────────────┐
│ Allocator      │ Alloc  │ Free   │ Best for                     │
├──────────────────────────────────────────────────────────────────┤
│ malloc         │ varies │ varies │ General purpose               │
│ Arena (bump)   │ O(1)   │ N/A*   │ Temporary data, parsing       │
│ Pool           │ O(1)   │ O(1)   │ Many same-size objects         │
│ Free-list      │ varies │ O(1)   │ Variable sizes, like malloc    │
│ Stack          │ O(1)   │ O(1)   │ LIFO pattern, scoped temps    │
├──────────────────────────────────────────────────────────────────┤
│ * Arena frees everything at once                                 │
└──────────────────────────────────────────────────────────────────┘
```

---

## Arena Allocator (Bump Allocator)

The simplest and fastest custom allocator. The idea:

1. Allocate one big block of memory upfront
2. Keep a pointer to the "next free byte"
3. To allocate: return the current pointer, then bump it forward
4. To free: reset the pointer back to the start (frees everything at once)

```
After arena_create(64):
┌────────────────────────────────────────────────────────────────┐
│ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ │
└────────────────────────────────────────────────────────────────┘
↑
offset = 0

After arena_alloc(arena, 12):    (allocate 12 bytes)
┌────────────────────────────────────────────────────────────────┐
│ X X X X X X X X X X X X _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ │
└────────────────────────────────────────────────────────────────┘
                          ↑
                          offset = 12

After arena_alloc(arena, 8):     (allocate 8 more bytes)
┌────────────────────────────────────────────────────────────────┐
│ X X X X X X X X X X X X Y Y Y Y Y Y Y Y _ _ _ _ _ _ _ _ _ _ _ │
└────────────────────────────────────────────────────────────────┘
                                          ↑
                                          offset = 20

After arena_reset(arena):        (free everything at once)
┌────────────────────────────────────────────────────────────────┐
│ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ │
└────────────────────────────────────────────────────────────────┘
↑
offset = 0    (same memory, just reuse it)
```

Why is this fast?
- **Allocation is O(1)** — just bump a pointer, no searching
- **No per-object free** — you can't free individual allocations
- **No fragmentation** — allocations are packed sequentially
- **Great cache behavior** — allocations are contiguous

When to use:
- Per-frame game allocations (allocate during frame, reset after)
- HTTP request handling (allocate during request, reset after response)
- Parsing (allocate AST nodes, reset after done)
- Any "allocate a bunch of stuff, then throw it all away" pattern

---

## Pool Allocator

For when you need many objects of the **same size** and want O(1) alloc and free.

The idea:
1. Allocate a big block, divide it into fixed-size slots
2. Link all free slots into a **free list** (using the slot memory itself!)
3. To allocate: pop the first slot from the free list
4. To free: push the slot back onto the free list

```
After pool_create(4 slots of 16 bytes each):

Slot 0      Slot 1      Slot 2      Slot 3
┌─────────┬─────────┬─────────┬─────────┐
│ next→1  │ next→2  │ next→3  │ next→∅  │
└─────────┴─────────┴─────────┴─────────┘
↑
free_head

After pool_alloc (returns Slot 0):

Slot 0      Slot 1      Slot 2      Slot 3
┌─────────┬─────────┬─────────┬─────────┐
│ [USED]  │ next→2  │ next→3  │ next→∅  │
└─────────┴─────────┴─────────┴─────────┘
            ↑
            free_head

After pool_alloc (returns Slot 1), pool_alloc (returns Slot 2):

Slot 0      Slot 1      Slot 2      Slot 3
┌─────────┬─────────┬─────────┬─────────┐
│ [USED]  │ [USED]  │ [USED]  │ next→∅  │
└─────────┴─────────┴─────────┴─────────┘
                                ↑
                                free_head

After pool_free(Slot 1):    (Slot 1 goes back to front of free list)

Slot 0      Slot 1      Slot 2      Slot 3
┌─────────┬─────────┬─────────┬─────────┐
│ [USED]  │ next→3  │ [USED]  │ next→∅  │
└─────────┴─────────┴─────────┴─────────┘
            ↑
            free_head
```

The clever trick: when a slot is free, we store the "next" pointer **inside the
slot's own memory**. No extra memory needed for bookkeeping!

When to use:
- Particle systems (thousands of same-size particles created/destroyed rapidly)
- Game entities, network connections, database cursors
- Anywhere you create/destroy many objects of the same type

---

## Free-List Allocator — How malloc Works Inside

This is a simplified version of what `malloc` actually does. It manages
variable-size allocations using a linked list of free blocks.

```
Initial state (one big free block):
┌──────────────────────────────────────────────────────────┐
│ [HDR: size=100, free=yes, next→∅]     free space...     │
└──────────────────────────────────────────────────────────┘
↑
free_list

After alloc(20):
┌────────────────────┬─────────────────────────────────────┐
│ [HDR: 20, used]    │ [HDR: size=76, free, next→∅]       │
│ user data (20)     │          free space...              │
└────────────────────┴─────────────────────────────────────┘
                      ↑
                      free_list

After alloc(10):
┌────────────────────┬──────────────┬──────────────────────┐
│ [HDR: 20, used]    │ [HDR:10,used]│ [HDR: 62, free]     │
│ user data (20)     │ data (10)    │   free space...      │
└────────────────────┴──────────────┴──────────────────────┘
                                     ↑
                                     free_list

After free(first block):
┌────────────────────┬──────────────┬──────────────────────┐
│ [HDR: 20, free]    │ [HDR:10,used]│ [HDR: 62, free]     │
│ next→third block   │ data (10)    │   next→∅             │
└────────────────────┴──────────────┴──────────────────────┘
↑                                    ↑
free_list                            (linked from first)
```

Each block has a **header** storing its size and whether it's free. Free blocks
are linked together so we can search them.

### Allocation strategies

When searching for a free block:
- **First-fit**: use the first block that's big enough (fast, but can cause fragmentation)
- **Best-fit**: use the smallest block that's big enough (less waste, but slower to find)
- **Worst-fit**: use the largest block (leaves bigger remnants, rarely useful)

### Coalescing — Fighting Fragmentation

When you free a block, check if the neighboring blocks are also free. If so,
merge them into one big block:

```
Before coalescing:              After coalescing:
┌──────┬──────┬──────┐          ┌────────────────────┐
│ free │ free │ free │    →     │      free          │
│  20  │  30  │  50  │          │      100           │
└──────┴──────┴──────┘          └────────────────────┘
```

Without coalescing, you get **external fragmentation**: plenty of total free
memory, but split into pieces too small to use.

---

## Alignment

CPUs are most efficient when data is at aligned addresses:
- A 4-byte `int` should be at an address divisible by 4
- An 8-byte `double` should be at an address divisible by 8

```
Aligned (fast):               Misaligned (slow or crashes):
Address: 0  4  8  12          Address: 0  3  7  10
         ┌──┬──┬──┐                    ┌──┬──┬──┐
         │A │B │C │  (ints)            │A─│B─│C─│
         └──┴──┴──┘                    └──┴──┴──┘
         ↑  ↑  ↑                       ↑ ↑  ↑
         4  4  4  (aligned)            3 4  3  (misaligned!)
```

Custom allocators must respect alignment. The typical approach: round up
allocation sizes to the nearest multiple of 8 (or 16 for stricter alignment).

```c
/* Round up to nearest multiple of 'align' */
size_t align_up(size_t size, size_t align)
{
    return (size + align - 1) & ~(align - 1);
}

/* Examples:
   align_up(1, 8) = 8
   align_up(9, 8) = 16
   align_up(16, 8) = 16
*/
```

The `& ~(align - 1)` trick works because powers of 2 in binary have a single
1-bit. Subtracting 1 creates a mask, and `~` flips it to clear the lower bits.
This is faster than division and modulo.

---

## Fragmentation

Two types that waste memory:

**External fragmentation** — free memory is scattered in small pieces:

```
Total free: 30 bytes. But can't allocate 20 bytes!

┌──────┬────┬──────┬──────┬────┬──────┐
│ used │free│ used │ used │free│ used │
│  20  │ 10 │  15  │  25  │ 20 │  10  │
└──────┴────┴──────┴──────┴────┴──────┘
         ↑                  ↑
      10 bytes           20 bytes  (30 total, but split up)
```

**Internal fragmentation** — allocated blocks have wasted padding:

```
Requested 5 bytes, but allocator rounds up to 8:

┌──────────┐
│ data│pad │  ← 3 bytes wasted (internal fragmentation)
│  5  │ 3  │
└──────────┘
```

Arenas have minimal fragmentation (just alignment padding). Pools have zero
external fragmentation (all slots same size). Free-list allocators fight
fragmentation with coalescing.

---

## Debugging Memory Issues — Your Toolkit

### Valgrind Memcheck

The most thorough memory debugger. Catches:
- Memory leaks (allocated but never freed)
- Invalid reads/writes (accessing freed or unallocated memory)
- Use of uninitialized values
- Double frees

```bash
gcc -g -Wall -Wextra -std=c99 -o program program.c
valgrind --leak-check=full --show-leak-kinds=all ./program
```

### AddressSanitizer (ASan)

Faster than valgrind (2-3× slowdown vs 20-30×). Compile-time instrumentation
that adds runtime checks:

```bash
gcc -g -Wall -Wextra -std=c99 -fsanitize=address -o program program.c
./program    # crashes with a clear report on any memory bug
```

ASan catches:
- Buffer overflows (stack and heap)
- Use after free
- Double free
- Memory leaks (with `-fsanitize=leak`)

**When to use which:**
- ASan for daily development (fast)
- Valgrind for release checks (more thorough, catches uninitialized reads)

---

## How malloc Really Works (Simplified)

Understanding malloc's internals helps you reason about performance:

```
Your program calls malloc(32):

1. malloc checks its internal bins for a free block of size >= 32

   Small bins (exact sizes):     Large bins (ranges):
   ┌────┬────┬────┬────┐        ┌─────────┬─────────┐
   │ 16 │ 32 │ 48 │ 64 │  ...   │ 512-1K  │ 1K-2K   │ ...
   │    │ ●  │    │    │        │         │         │
   └────┴─┼──┴────┴────┘        └─────────┴─────────┘
          ↓
     free block!

2. If found: remove from bin, return to user
3. If not found: ask the OS for more pages
   - Small: extend the heap via sbrk()
   - Large: map new pages via mmap()

4. When you free():
   - Block goes back into the appropriate bin
   - Adjacent free blocks are merged (coalesced)
   - Large blocks may be returned to the OS via munmap()
```

Key takeaway: `malloc` is already a sophisticated free-list allocator with
bins for different sizes. Custom allocators beat it only when you know your
specific pattern.

---

## Exercises

You have four katas for this module:

1. **arena.c** — Build a bump allocator with `arena_create`, `arena_alloc`,
   `arena_reset`, and `arena_destroy`. The simplest allocator possible.

2. **pool.c** — Build a pool allocator for a particle system. Fixed-size slots,
   free list using the slot memory itself.

3. **mini_malloc.c** — Build a free-list allocator with `my_malloc` and
   `my_free`. Supports variable-size allocations with first-fit search and
   coalescing on free.

4. **memory_profiler.c** — Build a profiler that wraps `malloc` and `free`
   using macros, tracks every allocation, and reports leaks at program exit.

Compile and test:
```bash
make exercises    # build your code
make test         # build and run solutions to verify
```

---

[← Previous: Module 07 — Dynamic Memory](../07-dynamic-memory/README.md) | [Next: Module 09 — Structs, Unions & Compound Types →](../09-structs-unions-compound-types/README.md)
