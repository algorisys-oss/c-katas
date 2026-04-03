# Module 13 — Hash Tables

You've built linked lists, stacks, and queues. All of them have one weakness:
**finding something takes O(n) time** — you have to walk through element after
element until you find what you're looking for.

What if you could look something up **instantly**? Not O(n), not O(log n), but
O(1) — constant time, no matter how many elements you have?

That's what **hash tables** give you. They're one of the most important data
structures in all of computer science. Every language has them built in:
Python's `dict`, JavaScript's objects, Java's `HashMap`, Go's `map`. In C,
you get to build one from scratch.

---

## The Key Insight: From Keys to Indices

An array gives you O(1) access — if you know the index. The problem is that
real-world data doesn't come with convenient integer indices. You want to look
up a student by name, a word by its spelling, a config setting by its key.

A **hash table** bridges this gap with a simple idea:

> Use a **hash function** to convert any key into an array index.

```
                    hash function
    key ──────────────────────────────> index
   "alice"    hash("alice") % 8    ──>   3
   "bob"      hash("bob") % 8     ──>   6
   "carol"    hash("carol") % 8   ──>   1

    Array (capacity = 8):
    ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
    │       │ carol │       │ alice │       │       │  bob  │       │
    │  [0]  │  [1]  │  [2]  │  [3]  │  [4]  │  [5]  │  [6]  │  [7]  │
    └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
```

Instead of searching through every element, you compute the index directly.
**That's the magic** — you skip the search entirely.

---

## Hash Functions: From First Principles

A **hash function** takes input data (of any size) and produces a fixed-size
number. For hash tables, we use this number as an array index.

### What Makes a Good Hash Function?

1. **Deterministic** — same input always gives the same output
2. **Uniform distribution** — spreads values evenly across the array
3. **Fast to compute** — we call it on every lookup, insert, and delete

### A Simple String Hash

The simplest approach: add up the character values.

```c
unsigned int hash(const char *key, int capacity)
{
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += key[i];  /* ASCII value of each character */
    }
    return sum % capacity;
}
```

For `"cat"`: `99 + 97 + 116 = 312`. If capacity is 8: `312 % 8 = 0`.

This works, but it's not great — `"cat"` and `"act"` produce the same hash
(both sum to 312). Better hash functions consider character position:

```c
/* djb2 — a classic string hash by Dan Bernstein */
unsigned int djb2(const char *key, int capacity)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = hash * 33 + c;
    }
    return hash % capacity;
}
```

For our exercises, the simple sum is fine — it lets us focus on the data
structure rather than the hash function.

---

## Collisions: The Unavoidable Problem

What happens when two different keys hash to the same index?

```
    hash("cat") % 8  = 0
    hash("act") % 8  = 0     ← same index!
```

This is called a **collision**, and it's unavoidable. You're mapping an
infinite set of possible keys into a finite array — by the **pigeonhole
principle**, collisions must happen.

There are two main strategies for handling collisions.

---

## Strategy 1: Separate Chaining

Each bucket in the array holds a **linked list** of entries. When two keys
hash to the same index, they both go into that bucket's list.

```
    Hash Table (capacity = 8, separate chaining):

    buckets[]
    ┌───┐
  0 │ ─→│──→ ["cat": 3] ──→ ["act": 7] ──→ NULL
    ├───┤
  1 │ ─→│──→ ["dog": 5] ──→ NULL
    ├───┤
  2 │   │  (empty — NULL)
    ├───┤
  3 │ ─→│──→ ["fox": 2] ──→ NULL
    ├───┤
  4 │   │  (empty — NULL)
    ├───┤
  5 │   │  (empty — NULL)
    ├───┤
  6 │ ─→│──→ ["emu": 1] ──→ NULL
    ├───┤
  7 │   │  (empty — NULL)
    └───┘

    To find "act":
      1. hash("act") % 8 = 0
      2. Go to buckets[0]
      3. Walk the linked list: "cat" ≠ "act", next → "act" = "act" ✓
      4. Return value: 7
```

**Pros**: Simple to implement. Never runs out of space.
**Cons**: Uses extra memory for linked list pointers. Cache-unfriendly
(nodes scattered in memory).

### Insert (chaining)
1. Compute `index = hash(key) % capacity`
2. Walk the list at `buckets[index]` — if key exists, update the value
3. Otherwise, prepend a new node to the list

### Lookup (chaining)
1. Compute `index = hash(key) % capacity`
2. Walk the list at `buckets[index]`, comparing keys
3. Return the value if found, or indicate "not found"

### Delete (chaining)
1. Compute `index = hash(key) % capacity`
2. Walk the list, find the node with matching key
3. Unlink it from the list and free it

---

## Strategy 2: Open Addressing (Linear Probing)

Instead of linked lists, store everything directly in the array. When a
collision occurs, look for the **next empty slot**.

```
    Linear probing: if slot is taken, try slot+1, then slot+2, ...

    Insert "cat" → hash = 0 → slot 0 is empty → place at 0
    Insert "act" → hash = 0 → slot 0 taken → try 1 → empty → place at 1
    Insert "bat" → hash = 1 → slot 1 taken → try 2 → empty → place at 2

    Array:
    ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
    │  cat  │  act  │  bat  │       │       │       │       │       │
    │  [0]  │  [1]  │  [2]  │  [3]  │  [4]  │  [5]  │  [6]  │  [7]  │
    └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
      hash=0  hash=0  hash=1
              probed  probed
```

**Pros**: Better cache performance (data is contiguous in memory).
**Cons**: Clustering — runs of occupied slots slow things down. Deletion
is tricky (can't just empty a slot — it breaks the probe chain).

We'll implement **separate chaining** in our exercises because it's simpler
and teaches linked list manipulation at the same time.

### Performance: Open Addressing vs. Chaining in Practice

In theory, both have O(1) average case. In practice, open addressing is
often faster because probes stay within a contiguous array — the CPU cache
prefetches nearby slots for free. Chaining follows pointers to scattered
heap nodes, causing cache misses.

This is why many high-performance hash tables (Google's dense_hash_map,
Rust's HashMap, Python's dict since 3.6) use open addressing variants.

---

## Load Factor and Rehashing

The **load factor** measures how full the hash table is:

```
    load factor = number of entries / capacity

    Example: 6 entries in a table with 8 buckets
    load factor = 6/8 = 0.75
```

As the load factor increases, performance degrades:
- With chaining: chains get longer, approaching O(n)
- With probing: more collisions, more probing

### Rehashing

When the load factor crosses a threshold (commonly 0.75), **rehash**:

1. Allocate a new, larger array (typically 2x the old capacity)
2. Recompute the hash for every existing entry (because `% capacity` changed)
3. Insert each entry into the new array
4. Free the old array

```
    Before rehashing (capacity = 4, entries = 3, load = 0.75):

    ┌───┐
  0 │ ─→│──→ ["a": 1] ──→ ["e": 5] ──→ NULL
    ├───┤
  1 │   │  NULL
    ├───┤
  2 │ ─→│──→ ["b": 2] ──→ NULL
    ├───┤
  3 │   │  NULL
    └───┘

    After rehashing (capacity = 8):

    ┌───┐
  0 │   │  NULL
    ├───┤
  1 │ ─→│──→ ["a": 1] ──→ NULL        (hash("a") % 8 = 1)
    ├───┤
  2 │ ─→│──→ ["b": 2] ──→ NULL        (hash("b") % 8 = 2)
    ├───┤
  3 │   │  NULL
    ├───┤
  4 │   │  NULL
    ├───┤
  5 │ ─→│──→ ["e": 5] ──→ NULL        (hash("e") % 8 = 5)
    ├───┤
  6 │   │  NULL
    ├───┤
  7 │   │  NULL
    └───┘

    Notice: "a" and "e" no longer collide — more buckets = fewer collisions.
```

---

## Time Complexity: Amortized O(1)

Hash table operations are **amortized O(1)** — here's what that means:

| Operation | Average Case | Worst Case |
|-----------|-------------|------------|
| Insert    | O(1)        | O(n)       |
| Lookup    | O(1)        | O(n)       |
| Delete    | O(1)        | O(n)       |

The **worst case** happens when every key hashes to the same bucket — you're
essentially searching a linked list. But with a good hash function and
reasonable load factor, this almost never happens.

**Amortized** means "averaged over many operations." Occasionally, an insert
triggers a rehash (O(n) to copy everything), but this happens so rarely that
it averages out to O(1) per operation — the same reasoning behind dynamic
arrays (like C++'s `std::vector`).

### Comparison with other data structures

| Operation     | Array   | Linked List | Sorted Array | Hash Table |
|---------------|---------|-------------|--------------|------------|
| Insert        | O(n)    | O(1)*       | O(n)         | O(1) avg   |
| Search        | O(n)    | O(n)        | O(log n)     | O(1) avg   |
| Delete        | O(n)    | O(n)        | O(n)         | O(1) avg   |

*O(1) insert at head, but finding where to insert is O(n).

---

## The Space-Time Tradeoff

Hash tables are a perfect example of **trading space for speed**:

- **More memory** (the array, plus linked list nodes, plus empty slots) →
  **faster lookups** (fewer collisions)
- **Less memory** (small array, high load factor) →
  **slower lookups** (more collisions, longer chains)

This tradeoff appears everywhere in computing:
- Caching: use memory to avoid recomputation
- Lookup tables: precompute values to avoid calculation
- Indexes in databases: extra storage for faster queries

---

## Exercises

### 1. `hash_table.c` — Build a Hash Table (14 tests)

Implement a hash table with separate chaining (linked list per bucket).
Functions: `ht_create`, `ht_destroy`, `ht_set`, `ht_get`, `ht_delete`,
`ht_size`. String keys, integer values.

### 2. `word_freq.c` — Word Frequency Counter (8 tests)

Use your hash table concepts to count word frequencies in a string.
Tokenize text into words and count occurrences of each word.

---

## Debug Challenge

| File | Description | Bugs |
|------|-------------|------|
| `debug_hashtable.c` | Find and fix 5 hash table bugs (open addressing) | 5 |

These exercises contain **intentionally broken code**. Your job is to find and
fix each bug. Each function has a comment explaining what it SHOULD do and a
HINT about the bug class. Run the program — failing tests tell you which
functions are still broken.

```bash
make debug    # compile the buggy version
./exercises/debug_hashtable   # see which tests fail
# ... fix bugs ...
# recompile and rerun until all tests pass
```

---

## Key Takeaways

1. **Hash tables map keys to array indices** using a hash function
2. **Collisions are inevitable** — handle them with chaining or probing
3. **Load factor** determines performance — rehash when it gets too high
4. **Amortized O(1)** for insert, lookup, and delete — the best average case
5. **Space-time tradeoff** — more memory means fewer collisions means faster lookups
6. Hash tables are the backbone of dictionaries, caches, symbol tables, and databases

### Reading Real Code: Redis dict.c

Redis — the world's most popular in-memory database — has a beautifully
written hash table implementation in `dict.c` (~800 lines). Key things
to look for:

- **Incremental rehashing**: Redis doesn't resize all at once (that would
  freeze the server). It moves a few entries per operation, spreading the
  cost over time. Look for `dictRehash()` and `rehashidx`.
- **Two hash tables**: During rehashing, Redis keeps both the old and new
  tables, gradually migrating entries. Look for `ht[0]` and `ht[1]`.
- **Hash function**: Redis uses SipHash for security (resistant to
  hash-flooding attacks).

Source: https://github.com/redis/redis/blob/unstable/src/dict.c

---

[← Previous: Module 12: Stacks & Queues](../12-stacks-queues/README.md) | [Next: Module 14: Sorting & Searching →](../14-sorting-searching/README.md)
