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

### Pre-emptive Splitting

In the examples above, we inserted into a leaf and then split if it was
overfull. But there is a smarter approach: **pre-emptive splitting**
(also called "proactive" or "top-down" splitting).

When walking down the tree to find where to insert, if you encounter a
**full node** (one with 2t-1 keys), split it **before** descending into
it. This way, when you finally reach the leaf, you are guaranteed it has
room — no need to walk back up the tree to fix things.

```
  Pre-emptive split algorithm (pseudocode):

  insert(tree, key, value):
      if root is full:
          create new_root
          make old root a child of new_root
          split_child(new_root, 0)    // split the old root
          root = new_root

      node = root
      while node is not a leaf:
          i = find correct child index for key
          if child[i] is full:
              split_child(node, i)    // split BEFORE descending
              if key > node.keys[i]:
                  i++                 // might need the new right child
          node = child[i]

      insert key into node (guaranteed to have room)
```

The benefit: this is a **single-pass, top-down** algorithm. You walk
from root to leaf exactly once, splitting full nodes as you encounter
them. No backtracking needed.

### Why B-Trees for Databases?

#### Disk Page I/O

Disks read and write data in fixed-size blocks called **pages** (usually
4 KB). You cannot read just 10 bytes from disk — the hardware always
reads the entire 4 KB page containing those bytes.

A B-Tree node is designed to fit exactly in one page:
- **Reading a node** = reading one page from disk (one I/O operation)
- **Writing a node** = writing one page to disk (one I/O operation)

This is why B-Trees minimize tree height — fewer levels means fewer disk
reads, which means faster lookups. A binary tree with a billion keys
would be ~30 levels deep (30 disk reads per lookup). A B-Tree with
order 500 fits hundreds of keys per node and is only 3-4 levels deep
(3-4 disk reads per lookup).

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

## Database Wire Protocols

So far we've built an in-memory database. Real databases also need a way for
client programs to **talk** to the database over a network. This is called a
**wire protocol** — the exact format of bytes that travel between client and
server over a TCP connection.

### How PostgreSQL's Wire Protocol Works

PostgreSQL uses a binary protocol (version 3). Here's what happens when a
client connects:

```
  Wire Protocol = how bytes travel between client and server

  PostgreSQL Protocol v3:

  1. Client sends Startup Message:
     ┌──────────┬──────────────┬──────────────────────┐
     │ Length   │ Protocol     │ Key-Value Pairs       │
     │ (4 bytes)│ (0x00030000) │ user=bob\0database=... │
     │ big-end  │ = version 3  │ terminated by \0\0    │
     └──────────┴──────────────┴──────────────────────┘

  2. Server responds with message type + length + payload:
     ┌──────┬──────────┬───────────┐
     │ Type │ Length   │ Payload   │
     │ 'R'  │ (4 bytes)│ auth type │
     │1 byte│ big-end  │           │
     └──────┴──────────┴───────────┘

  Common message types:
    'R' = Authentication request/response
    'Z' = ReadyForQuery (server is ready for next command)
    'T' = RowDescription (column names and types)
    'D' = DataRow (one row of results)
```

Every message after startup follows the same pattern: one byte for the type,
four bytes for the length, then the payload. This makes it straightforward to
parse — you always know how many bytes to read next.

### Redis: A Simpler Approach

Redis uses a text-based protocol called **RESP** (Redis Serialization Protocol)
that is human-readable and easy to debug:

```
  Client sends:     *3\r\n$3\r\nSET\r\n$4\r\nname\r\n$5\r\nAlice\r\n
  Server responds:  +OK\r\n

  Breakdown:
    *3     = array of 3 elements
    $3     = next string is 3 bytes
    SET    = the command
    $4     = next string is 4 bytes
    name   = the key
    $5     = next string is 5 bytes
    Alice  = the value
```

You can even test Redis by hand with `telnet` or `nc` — try it!

### Why This Matters

Building a wire protocol is a great exercise in binary serialization, byte
order handling, and TCP socket programming — it ties together everything
from Modules 16, 17, and 26. PostgreSQL's binary protocol is more efficient
but harder to implement. Redis's text-based protocol is simpler and easier
to debug. Both are valid design choices with different trade-offs.

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

### Reading Real Code: SQLite btree.c

SQLite is the most widely deployed database in the world (in every phone,
browser, and operating system). Its B-tree implementation in `btree.c`
(~10,000 lines) is a masterclass in systems programming.

What to look for:
- **Header comments**: The first 200 lines explain the entire page format
  and B-tree structure. Some of the best documentation in open source.
- **Page layout**: How data, pointers, and free space are organized within
  a fixed-size disk page.
- **Cell format**: How key-value pairs are encoded within B-tree nodes.
- **Overflow pages**: What happens when a value is too large for one page.

Don't try to read all 10,000 lines — start with the header comments and
the `sqlite3BtreeInsert()` function.

Source: https://github.com/sqlite/sqlite/blob/master/src/btree.c

---

[← Previous: Module 23: Git Internals](../28-git-internals/README.md) | [Next: Module 25: Parsing & the SQL Engine →](../25-build-parsing-sql-engine/README.md)
