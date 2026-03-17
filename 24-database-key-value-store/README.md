# Module 24 — Database: Key-Value Store

## What Is a Database?

At its simplest, a database is a program that:

1. **Stores** data so it survives a restart (persistence)
2. **Retrieves** data quickly given a key or query
3. **Modifies** data safely, even when things go wrong (crash, power loss)

Every database, from SQLite to PostgreSQL to Redis, solves these three
problems. The differences are in *how* they solve them and what trade-offs
they make.

Let's build one from scratch.

---

## Page-Based Storage

Real databases don't read and write individual bytes to disk. Disks are
block devices — they transfer data in fixed-size chunks (typically 4 KB).
Databases embrace this by organizing their data into **pages**.

```
┌─────────────────────────────────────────────────────┐
│                    Disk File                         │
│                                                     │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐            │
│  │  Page 0  │ │  Page 1  │ │  Page 2  │  ...       │
│  │ (4096 B) │ │ (4096 B) │ │ (4096 B) │            │
│  └──────────┘ └──────────┘ └──────────┘            │
│                                                     │
│  Page 0: metadata (magic number, page count, etc.)  │
│  Page 1+: data pages (keys and values)              │
└─────────────────────────────────────────────────────┘
```

### Page Layout

Each data page has a header and a body:

```
  Offset   Size    Field
  ──────   ────    ─────────────────────────
  0        2       num_entries (how many key-value pairs)
  2        2       free_space_offset (where free space starts)
  4        ...     Entry 0: [key_len][key][value_len][value]
                   Entry 1: [key_len][key][value_len][value]
                   ...
  4092     4       next_page pointer (0 = no next page)
```

This is the basic idea behind SQLite's file format, PostgreSQL's heap
pages, and many others. For our in-memory kata, we skip disk pages and
work with malloc'd data, but the concept is the same.

---

## B-Trees from First Principles

### The Problem

Imagine you have a million key-value pairs. How do you find one quickly?

- **Unsorted array**: scan every element. O(n). Too slow.
- **Sorted array**: binary search. O(log n). Fast reads, but inserting in
  the middle means shifting everything after it. O(n) for writes.
- **Hash table**: O(1) average, but no range queries ("give me all keys
  between 100 and 200") and terrible disk performance (random access).

We need a structure that gives us O(log n) for *both* reads and writes,
works well with disk pages, and supports range queries.

**Enter the B-Tree.**

### What Is a B-Tree?

A B-Tree is a balanced search tree where each node can hold *multiple* keys
and has *multiple* children. This is the key difference from a binary tree
(which holds 1 key and has 2 children).

A B-Tree of **order t** (minimum degree) has these rules:

1. Every node holds between **t-1** and **2t-1** keys (except root, which
   can have fewer).
2. Every internal node with **k** keys has exactly **k+1** children.
3. All leaves are at the **same depth** (the tree is perfectly balanced).
4. Keys within a node are **sorted**.

### Order 3 B-Tree (t=2)

For our kata, we use order 3 — meaning each node holds **up to 2 keys**
and has **up to 3 children**. (This is sometimes called t=2 or
"minimum degree 2".)

```
  Rules for order 3 (t=2):
    - Each node: 1 to 2 keys
    - Each internal node: 2 to 3 children
    - Root: 1 to 2 keys (or 0 if tree is empty)
    - All leaves at same level
```

### B-Tree Search

Searching is like binary search, but at each node you scan its keys to
decide which child to follow:

```
  Search for key 17 in this tree:

       [10 | 20]              ← 10 < 17 < 20, go to middle child
      /    |    \
  [5|8] [13|17] [25|30]      ← found 17 in this node!
```

At each level, you compare the target against the node's keys and choose
the right child pointer. Since the tree is balanced and each level
eliminates a large fraction of nodes, search is **O(log n)**.

### B-Tree Insert (with Node Splitting)

Insertion always happens at a leaf. If the leaf is full, we **split** it.

Let's walk through inserting keys 10, 20, 5, 15, 25, 30 into an empty
order-3 B-Tree (max 2 keys per node).

**Step 1: Insert 10**
```
  [10]
```
Single node, done.

**Step 2: Insert 20**
```
  [10 | 20]
```
Node has room, just insert in sorted position.

**Step 3: Insert 5 — THE SPLIT**

We want to insert 5, but the root [10|20] is already full (2 keys).
We must split:

```
  Before:  [5, 10, 20]  ← 3 keys, too many!

  Split: take the median (10), push it up to a new root.
         Left child gets keys < median.
         Right child gets keys > median.

  After:
         [10]           ← new root (median)
        /    \
     [5]    [20]        ← split halves
```

**Step 4: Insert 15**

15 > 10, so go right to [20]. Room in that node.
```
         [10]
        /    \
     [5]    [15 | 20]
```

**Step 5: Insert 25**

25 > 10, go right to [15|20]. That node is full! Split:

```
  [15, 20, 25] → median is 20, push up to parent

         [10 | 20]        ← 20 pushed up
        /    |    \
     [5]   [15]  [25]
```

**Step 6: Insert 30**

30 > 20, go right to [25]. Room in that node.
```
         [10 | 20]
        /    |    \
     [5]   [15]  [25 | 30]
```

### Why B-Trees for Databases?

Each B-Tree node maps perfectly to a **disk page**. A real database might
use order 100+ so each node holds hundreds of keys and fits in one 4 KB
page. This means a tree of a billion keys is only 4-5 levels deep — just
4-5 disk reads to find any key.

---

## Write-Ahead Logging (WAL)

### The Crash Problem

Imagine the database is updating a B-Tree node on disk. Power cuts out
mid-write. The node is now half-old, half-new — **corrupted**.

### The Solution: Write First, Apply Later

Before modifying any data page, write a **log entry** describing what
you're about to do:

```
  WAL File (append-only):
  ┌─────────────────────────────────────────────┐
  │ LSN=1  SET key=42 value=100                 │
  │ LSN=2  SET key=17 value=200                 │
  │ LSN=3  DELETE key=42                        │
  │ LSN=4  COMMIT                               │
  └─────────────────────────────────────────────┘

  LSN = Log Sequence Number (always increasing)
```

The process:
1. Write the operation to the WAL (append to end of log file)
2. `fsync()` the WAL (force it to disk)
3. Apply the change to the actual data pages (in memory or on disk)
4. Periodically **checkpoint**: flush all dirty pages to disk, then
   truncate the WAL

### WAL Replay After Crash

On startup, the database checks: "Did I crash last time?" If yes, it
**replays** the WAL:

```
  Crash Recovery:

  1. Open WAL file
  2. Read entries from last checkpoint
  3. For each entry:
     - If COMMIT seen → replay all ops in that transaction
     - If no COMMIT   → discard (transaction was incomplete)
  4. Data is now consistent again
  5. Truncate WAL, resume normal operation

  Timeline:
  ────────────────────────────────────────────
  WAL: [SET a=1] [SET b=2] [COMMIT] [SET c=3]
                                          ↑
                                        CRASH!

  Replay: SET a=1 ✓  SET b=2 ✓  (committed)
          SET c=3 ✗  (no COMMIT, discard)
  ────────────────────────────────────────────
```

This is exactly how SQLite's WAL mode, PostgreSQL's WAL, and MySQL's
redo log work.

---

## ACID Properties

Every serious database guarantees ACID:

| Property      | Meaning                                          | How We Achieve It       |
|---------------|--------------------------------------------------|-------------------------|
| **Atomicity** | All-or-nothing: a transaction either fully        | WAL + rollback          |
|               | commits or fully rolls back                      |                         |
| **Consistency** | Data always satisfies defined rules (constraints)| Application logic       |
| **Isolation** | Concurrent transactions don't interfere          | Locks or MVCC           |
| **Durability**| Once committed, data survives crashes             | WAL + fsync             |

For our single-threaded, in-memory store, we focus on **Atomicity** (via
the WAL concept) and **Durability** (via write-ahead logging). Isolation
matters when you have multiple concurrent readers/writers — a topic for a
future module.

---

## Katas

### Kata 1: `btree.c` — In-Memory B-Tree

Build an order-3 B-Tree (max 2 keys per node, max 3 children) that maps
`int` keys to `int` values.

Functions to implement:
- `btree_create()` — allocate and return a new empty tree
- `btree_destroy(tree)` — free all nodes
- `btree_insert(tree, key, value)` — insert with splitting
- `btree_search(tree, key, *out_value)` — return 0 if found, -1 if not
- `btree_size(tree)` — number of key-value pairs

### Kata 2: `kvstore.c` — Simple Key-Value Store

Build a key-value store using a sorted array. String keys, string values.

Functions to implement:
- `kv_create(capacity)` — allocate a store with given max capacity
- `kv_destroy(store)` — free everything
- `kv_set(store, key, value)` — insert or update
- `kv_get(store, key, out_buf, buf_size)` — lookup, return 0 if found
- `kv_delete(store, key)` — remove a key
- `kv_count(store)` — number of entries

---

## Further Reading

- *Database Internals* by Alex Petrov — excellent deep dive into storage
  engines
- SQLite file format documentation: https://www.sqlite.org/fileformat.html
- The original B-Tree paper: Bayer & McCreight, 1972
- PostgreSQL WAL documentation: https://www.postgresql.org/docs/current/wal-intro.html
