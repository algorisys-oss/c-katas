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

## Two Sum — Why Data Structures Matter

The Two Sum problem: given an array and a target, find two elements that
add up to the target. It's simple to state, but it beautifully demonstrates
why choosing the right data structure matters.

### Three Approaches, Same Problem

```
    ┌──────────────────┬──────────┬──────────┬─────────────────────┐
    │ Approach          │ Time     │ Space    │ Requirement         │
    ├──────────────────┼──────────┼──────────┼─────────────────────┤
    │ Brute force       │ O(n²)    │ O(1)     │ None                │
    │ Hash table        │ O(n)     │ O(n)     │ None                │
    │ Two pointers      │ O(n)     │ O(1)     │ Array must be sorted│
    └──────────────────┴──────────┴──────────┴─────────────────────┘
```

### Brute Force: Check Every Pair — O(n²)

The obvious approach: for each element, check every other element.
With n elements, that's roughly n²/2 comparisons.

### Hash Table: The Complement Trick — O(n)

The insight: if you're looking for two numbers that sum to `target`, then
for any number `x`, you need `target - x` (the **complement**). If you've
already seen the complement, you're done.

```
    Array: [2, 7, 11, 15], target = 9

    i=0: arr[0]=2, complement = 9-2 = 7.  Hash table: {}. Not found. Store 2→0.
    i=1: arr[1]=7, complement = 9-7 = 2.  Hash table: {2:0}. Found! Return (0,1).
```

One pass, O(1) per lookup — O(n) total. But uses O(n) extra space for the table.

### Two Pointers: O(n) Time, O(1) Space — But Needs Sorted Input

If the array is sorted, use two pointers: one at the start, one at the end.
If their sum is too small, move the left pointer right. Too big, move the
right pointer left.

```
    Sorted array: [1, 3, 5, 7, 9], target = 12

    left=0, right=4: 1+9=10 < 12, move left →
    left=1, right=4: 3+9=12 = 12 ✓ Found! Return (1,4)
```

---

## LRU Cache — Hash Table Meets Linked List

### What is Caching?

You already use caches every day. Your browser caches images so it doesn't
download them again. Your CPU caches frequently used memory locations in L1/L2
cache (thousands of times faster than main RAM). A cache is just **fast storage
for recently/frequently used data**.

### The Eviction Problem

Caches have limited space. When full, something must go. **LRU** (Least
Recently Used) evicts the item that hasn't been touched for the longest time.
The bet: if you haven't used it recently, you probably won't need it soon.

### Why Two Data Structures?

We need two operations to be fast:
- **Find an item by key**: hash table gives O(1)
- **Track usage order** (move to front on access, remove from back on evict):
  doubly linked list gives O(1)

Neither alone is enough. A hash table can't track order. A linked list
can't find items by key in O(1). Together, they're perfect.

### The Architecture

```
    Hash Table                     Doubly Linked List
    (key → node pointer)          (most recent → least recent)

    ┌───────────────┐
    │ key=1 ──────────────→ ┌─────────────────────────────────────────┐
    ├───────────────┤       │                                         │
    │ key=2 ────────────┐   │ [DUMMY] ↔ [1:A] ↔ [3:C] ↔ [2:B] ↔ [DUMMY] │
    ├───────────────┤   │   │  HEAD    most recent   least recent  TAIL   │
    │ key=3 ──────────┐ │   └─────────────────────────────────────────┘
    ├───────────────┤ │ │
    │  (empty)      │ │ └──→ points to node [2:B] in the list
    └───────────────┘ └────→ points to node [3:C] in the list

    Dummy head/tail nodes simplify edge cases — you never need
    to check for NULL when inserting or removing at the ends.
```

### Operations Step by Step

**GET(key=3)**:
```
    Before: [HEAD] ↔ [1:A] ↔ [3:C] ↔ [2:B] ↔ [TAIL]
    1. Hash lookup: key=3 → node [3:C]          O(1)
    2. Remove [3:C] from current position        O(1)
    3. Insert [3:C] right after HEAD             O(1)
    After:  [HEAD] ↔ [3:C] ↔ [1:A] ↔ [2:B] ↔ [TAIL]
```

**PUT(key=4, val=D) when at capacity**:
```
    Before: [HEAD] ↔ [3:C] ↔ [1:A] ↔ [2:B] ↔ [TAIL]
                                        ↑ LRU — evict this
    1. Remove [2:B] (TAIL->prev)                 O(1)
    2. Delete key=2 from hash table              O(1)
    3. Free node [2:B]
    4. Create node [4:D], insert after HEAD      O(1)
    5. Add key=4 to hash table                   O(1)
    After:  [HEAD] ↔ [4:D] ↔ [3:C] ↔ [1:A] ↔ [TAIL]
```

---

## Exercises

### 1. `hash_table.c` — Build a Hash Table (14 tests)

Implement a hash table with separate chaining (linked list per bucket).
Functions: `ht_create`, `ht_destroy`, `ht_set`, `ht_get`, `ht_delete`,
`ht_size`. String keys, integer values.

### 2. `word_freq.c` — Word Frequency Counter (8 tests)

Use your hash table concepts to count word frequencies in a string.
Tokenize text into words and count occurrences of each word.

### 3. `two_sum.c` — Two Sum, Three Ways (10 tests)

Solve the classic Two Sum problem three different ways: brute force O(n²),
hash table O(n), and two pointers on sorted arrays O(n). See how the same
problem gets dramatically faster with better data structures.

### 4. `lru_cache.c` — LRU Cache (15 tests)

Build a Least Recently Used cache combining a hash table with a doubly
linked list. Implement the linked list helpers, then create, get, put, and
destroy operations with proper eviction.

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

[← Previous: Module 12: Stacks & Queues](../20-stacks-queues/README.md) | [Next: Module 14: Sorting & Searching →](../22-sorting-searching/README.md)
