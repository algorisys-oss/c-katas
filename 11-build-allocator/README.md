# Module 11: Build Your Own Allocator

> "You stop writing code and start moving memory."

You know `malloc` and `free`. You know pointers. You know structs.
Now you build the thing that *gives* you memory.

This is your first real systems build. After this module, `malloc` stops
being magic — it's just code. Code you could write yourself.

---

## Why Custom Allocators?

`malloc` is a general-purpose allocator. It handles any size, any order of
alloc/free, any pattern. That generality has a cost:

- **Overhead**: malloc stores bookkeeping (headers, free lists) — you pay
  bytes per allocation you never asked for.
- **Fragmentation**: after many alloc/free cycles, free memory is scattered
  in small gaps that can't satisfy large requests.
- **Speed**: malloc must search for a suitable free block every time.

If you *know your allocation pattern*, you can beat malloc. Dramatically.

| Pattern                         | Best allocator |
|---------------------------------|----------------|
| Many allocs, free all at once   | Arena          |
| Many same-size objects          | Pool           |
| Alloc/free in LIFO order        | Stack          |
| General purpose (any pattern)   | Free-list      |

---

## Arena (Bump) Allocator

The simplest possible allocator. One big block. One pointer. Bump it forward.

### How It Works

```
arena_create(256):
  Allocate 256 bytes from malloc. Set pointer to start.

  Memory:
  +----------------------------------------------------------+
  |                      256 bytes                            |
  +----------------------------------------------------------+
  ^
  pointer (offset = 0)


arena_alloc(32):
  Return current pointer. Bump forward by 32.

  +----------+-----------------------------------------------+
  | 32 bytes |              224 bytes free                    |
  | (in use) |                                               |
  +----------+-----------------------------------------------+
              ^
              pointer (offset = 32)


arena_alloc(64):
  Return current pointer. Bump forward by 64.

  +----------+------------------+----------------------------+
  | 32 bytes |    64 bytes      |      160 bytes free        |
  | (in use) |    (in use)      |                            |
  +----------+------------------+----------------------------+
                                 ^
                                 pointer (offset = 96)


arena_reset():
  Move pointer back to start. All memory is "freed" at once.

  +----------------------------------------------------------+
  |                      256 bytes                            |
  |              (all available again)                        |
  +----------------------------------------------------------+
  ^
  pointer (offset = 0)
```

### Key Properties

- **Allocate** = bump a pointer forward. O(1), no searching.
- **Free individual items** = you can't. That's the trade-off.
- **Free everything** = reset the pointer to start. O(1), instant.
- **Fragmentation** = zero. Allocations are contiguous.

### Alignment

Real allocators must return aligned addresses. If you allocate 3 bytes,
the next allocation shouldn't start at an odd offset — CPUs want data at
addresses divisible by 4 or 8.

```
  Without alignment:              With 8-byte alignment:

  +---+--------+---+              +---+-----+--------+---+
  | 3 |   10   | 5 |             | 3 | pad |   10   | 5 |
  +---+--------+---+              +---+-----+--------+---+
  0   3        13                 0   3     8        18
                                        ^-- aligned to 8
```

To align, round up:
```c
aligned_offset = (offset + (alignment - 1)) & ~(alignment - 1);
```

### When to Use

- Game engines: allocate everything for one frame, reset at frame end
- Web servers: allocate per-request, reset when request is done
- Parsers: build an AST, then throw it all away
- Compilers: allocate IR nodes for one function, reset between functions

---

## Pool Allocator

For when you have *many objects of the same size*. Think: particles in a
game, network connections, database rows.

### How It Works

Allocate one big block. Divide it into fixed-size chunks. Thread a free
list *through the chunks themselves* (no extra memory needed!).

```
pool_create(block_size=32, count=4):

  Step 1: Allocate 128 bytes (4 x 32).

  Step 2: Build a free list through the blocks.
  Each free block's first 8 bytes hold a pointer to the next free block.

  +----------+----------+----------+----------+
  | next: ------> next: ------> next: ------> next: NULL |
  | (free)   | (free)   | (free)   | (free)   |
  +----------+----------+----------+----------+
  ^
  free_list head


pool_alloc():
  Pop the head of the free list. Return it.

  +----------+----------+----------+----------+
  | USER     | next: ------> next: ------> next: NULL |
  | DATA     | (free)   | (free)   | (free)   |
  +----------+----------+----------+----------+
               ^
               free_list head


pool_alloc() again:

  +----------+----------+----------+----------+
  | USER     | USER     | next: ------> next: NULL |
  | DATA     | DATA     | (free)   | (free)   |
  +----------+----------+----------+----------+
                          ^
                          free_list head


pool_free(block 0):
  Push it back onto the free list head.

  +----------+----------+----------+----------+
  | next: ---------------------> next: ------> next: NULL |
  | (free)   | USER     | (free)   | (free)   |
  +----------+----------+----------+----------+
  ^
  free_list head
```

### The Trick: Embedded Free List

The genius: free blocks aren't being used for anything. So we store the
free-list pointer *inside* the free block's own bytes. Zero overhead.

```c
// Each free block's first bytes are a pointer to the next free block:
typedef struct {
    void *next;   // overlaid on the free block's memory
} free_node_t;
```

This only works because `block_size >= sizeof(void*)`. We enforce that.

### Key Properties

- **Allocate** = pop from free list. O(1).
- **Free** = push to free list. O(1).
- **No fragmentation** — all blocks are the same size.
- **No searching** — the free list gives you a block instantly.

### When to Use

- Particle systems (thousands of same-size particles)
- Entity component systems in games
- Connection pools in servers
- Any "object pool" pattern

---

## Stack (LIFO) Allocator

Like an arena, but you *can* free — as long as you free in reverse order.

### How It Works

```
stack_alloc_create(256):

  +----------------------------------------------------------+
  |                      256 bytes                            |
  +----------------------------------------------------------+
  ^
  top (offset = 0)


stack_push(32):   (returns pointer, stores header before data)

  +------+----------+-------------------------------------------+
  | hdr  | 32 bytes |              free                          |
  | (sz) | (in use) |                                            |
  +------+----------+-------------------------------------------+
                     ^
                     top


stack_push(64):

  +------+----------+------+------------------+-----------------+
  | hdr  | 32 bytes | hdr  |    64 bytes      |     free        |
  | (sz) | (in use) | (sz) |    (in use)      |                 |
  +------+----------+------+------------------+-----------------+
                                               ^
                                               top


stack_pop():   (reads header to know size, moves top back)

  +------+----------+-------------------------------------------+
  | hdr  | 32 bytes |              free                          |
  | (sz) | (in use) |                                            |
  +------+----------+-------------------------------------------+
                     ^
                     top
```

Each allocation stores a small header (the size) so `pop` knows how far
to rewind.

### Key Properties

- **Allocate** = bump pointer forward (like arena). O(1).
- **Free** = move pointer back by last allocation's size. O(1).
- **Constraint**: must free in reverse order (LIFO).

### When to Use

- Temporary scratch buffers
- Recursive algorithms that allocate per level
- Undo systems (allocate forward, undo backward)

---

## Free-List Allocator (Conceptual)

This is essentially what `malloc` does. The most flexible, most complex.

```
  Heap memory with free-list:

  +--------+------+--------+------+--------+------+--------+
  | USED   | FREE |  USED  | FREE | USED   | FREE |  USED  |
  | 64B    | 32B  |  128B  | 16B  | 48B    | 64B  | 96B    |
  +--------+------+--------+------+--------+------+--------+
             |                |                |
             v                v                v
           [32B] ---------> [16B] ---------> [64B] --> NULL
           (free list links the free blocks together)
```

On `alloc(size)`:
- **First-fit**: walk the free list, return the first block >= size
- **Best-fit**: walk the entire list, return the smallest block >= size
- **Worst-fit**: return the largest block (reduces leftover fragments)

On `free(ptr)`:
- Add the block back to the free list
- **Coalesce**: merge with adjacent free blocks to reduce fragmentation

We won't build this one yet — it's what you'll do in a later module. But
now you understand the idea behind malloc.

---

## Fragmentation

### External Fragmentation

Free memory exists but is scattered in small pieces:

```
  Total free: 96 bytes.  But largest contiguous free: 32 bytes.
  Can't satisfy a 64-byte request!

  +------+------+------+------+------+------+------+------+
  | USED | free | USED | free | USED | free | USED | free |
  | 64B  | 32B  | 32B  | 16B  | 48B  | 16B  | 16B  | 32B |
  +------+------+------+------+------+------+------+------+
```

### Internal Fragmentation

You requested 10 bytes but the allocator gives you 16 (for alignment or
because pool blocks are fixed-size). Those 6 bytes are wasted *inside*
your allocation.

```
  Requested: 10 bytes.  Got: 16 bytes.

  +----------------------------+
  | 10 bytes used | 6B wasted  |
  +----------------------------+
  |<------- 16 byte block ---->|
```

### How Each Allocator Handles Fragmentation

| Allocator  | External Frag | Internal Frag     |
|------------|---------------|-------------------|
| Arena      | None          | Alignment padding |
| Pool       | None          | If objects < block size |
| Stack      | None          | Alignment padding |
| Free-list  | Yes (biggest problem) | Alignment padding |

---

## Exercises

Build these allocators yourself:

1. **`arena.c`** — Arena (bump) allocator with alignment
2. **`pool.c`** — Pool allocator with embedded free list
3. **`stack_alloc.c`** — Stack (LIFO) allocator

Each file has a test harness. Implement the TODOs, compile, and run.
All tests should print `[PASS]`.

```bash
cd exercises/
make all
./arena
./pool
./stack_alloc
```

---

## What You'll Learn

After completing this module, you will:

- Understand that memory allocation is just pointer arithmetic
- Know when malloc is overkill and a simpler allocator wins
- Be able to build an arena, pool, and stack allocator from scratch
- Understand fragmentation and alignment at a visceral level
- See that "systems programming" means controlling memory directly
