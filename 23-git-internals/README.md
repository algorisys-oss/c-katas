# Module 23: Git Internals

## Overview

Git is one of the most important tools in a programmer's toolkit, but most people
treat it as a black box. In this module, we crack it open and build the core data
structures that make Git work. You'll discover that Git is, at its heart, a
**content-addressable filesystem** — a beautifully simple idea with profound
consequences.

By the end of this module, you'll understand:
- How Git stores files as **objects** identified by their content hash
- The three object types: **blob**, **tree**, **commit**
- How branches and HEAD are just **pointers** (refs) to commits
- Why Git's history forms a **directed acyclic graph** (DAG)
- How **Merkle trees** guarantee data integrity

---

## 1. Version Control from First Principles

Before Git, let's think about the fundamental problem: **how do you track changes
to files over time?**

### The Naive Approach: Copy Everything

```
project_v1/
project_v2/
project_v2_final/
project_v2_final_REAL/
project_v2_final_REAL_fixed/
```

This is what people actually do without version control. Problems:
- Wastes enormous amounts of disk space (full copies every time)
- No way to know *what* changed between versions
- No way to merge two people's work
- No way to go back to a specific state reliably

### The Diff Approach: Store Changes

What if we only stored the *differences* between versions? This is what older VCS
tools (like RCS, CVS) did. Better, but still has problems with branching and
merging.

### Git's Insight: Store Snapshots, Deduplicate by Content

Git takes a radically different approach: it stores **complete snapshots** of every
file at every commit, but it uses **content-addressable storage** to avoid
duplicating identical files. If two files have the same content, they're stored
only once.

---

## 2. Content-Addressable Storage

This is the single most important idea in Git. Instead of naming files by their
path or a sequential number, we name them by their **content**:

```
content  ──→  hash function  ──→  unique ID (hash)
```

The hash is computed from the file's content. Same content always produces the same
hash. Different content (almost certainly) produces a different hash.

```
┌──────────────────────────────────────────────────────────┐
│              CONTENT-ADDRESSABLE STORAGE                  │
│                                                          │
│  "Hello, world!\n"  ──hash──→  af5626b4a114ab...         │
│  "Goodbye!\n"        ──hash──→  2b7e26b89c45de...         │
│  "Hello, world!\n"  ──hash──→  af5626b4a114ab...  (same!)│
│                                                          │
│  ┌───────────┐     ┌───────────┐                         │
│  │ af5626b4  │     │ 2b7e26b8  │                         │
│  │───────────│     │───────────│                         │
│  │ Hello,    │     │ Goodbye!  │     Only 2 objects      │
│  │ world!    │     │           │     even though we       │
│  └───────────┘     └───────────┘     stored 3 things!    │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

**Key properties:**
1. **Deterministic** — same input always gives same hash
2. **Deduplication** — identical content is stored once
3. **Integrity** — if even one bit changes, the hash changes
4. **Immutable** — you can't modify an object without changing its hash

Git uses **SHA-1** (a 160-bit / 20-byte cryptographic hash) to compute these IDs.
The hash is displayed as a 40-character hexadecimal string like
`af5626b4a114abcb82d63db7c8082c3c4756e51b`.

---

## 3. SHA-1 Hashing

SHA-1 (Secure Hash Algorithm 1) processes data in 512-bit (64-byte) blocks:

1. **Pad** the message to a multiple of 512 bits
2. **Initialize** five 32-bit state variables (h0–h4)
3. For each 512-bit block:
   - **Expand** the 16 words into 80 words
   - Run 80 **rounds** of mixing (bitwise ops, rotations, additions)
4. Output the final state as 160 bits (20 bytes, 40 hex chars)

The key insight: each bit of the output depends on *every* bit of the input. Change
one letter and the hash changes completely:

```
"Hello"  → aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d
"hello"  → aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d  ← wait, that's wrong
"hello"  → aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d  ← actually different!

Real values:
"Hello"  → f7ff9e8b7bb2e09b70935a5d785e0cc5d9d0abf0
"hello"  → aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d
```

Completely different outputs for a one-character change! This is called the
**avalanche effect**.

---

## 4. Git Objects

Git has three core object types. Each is stored as:
`<type> <size>\0<content>`, then SHA-1 hashed.

Let's break down this format:
- `<type>` is the literal ASCII string `"blob"`, `"tree"`, or `"commit"`
- `<size>` is the ASCII decimal representation of the content length in bytes
  (e.g., the string `"14"`, not the binary number 14)
- `\0` is a single **null byte** (`0x00`), not the two characters backslash and
  zero — it separates the header from the content
- `<content>` is the raw bytes of the object

For example, a blob containing `"Hello, world!\n"` (14 bytes) is stored as:
`blob 14\0Hello, world!\n` — Git then SHA-1 hashes this **entire** string
(header + null byte + content) to produce the object's 40-character hex ID.

### Blob — File Content

A blob stores the raw content of a file. It does NOT store the filename — just the
bytes.

```
blob 14\0Hello, world!\n
     ^^                    ← size in decimal ASCII
        ^                  ← null byte separator
         ^^^^^^^^^^^^^^^   ← raw content
```

### Tree — Directory Listing

A tree maps filenames to blobs (or other trees for subdirectories):

```
tree <size>\0
  100644 hello.txt\0<20-byte blob hash>
  100644 readme.md\0<20-byte blob hash>
  040000 src\0<20-byte tree hash>
```

The numbers before each filename are **Unix file mode permissions**:
- `100644` = regular file, readable and writable by owner (standard Unix file permissions)
- `040000` = directory (the leading `04` means "directory" in Unix)
- `100755` = executable file (you'd see this for scripts and compiled programs)

The `\0` between the filename and the 20-byte hash is a literal **null byte** (byte
value `0x00`), used as a separator. Since filenames can't contain null bytes, this
guarantees unambiguous parsing — the parser reads characters until it hits `0x00`,
and knows the next 20 bytes are the binary SHA-1 hash.

### Commit — Snapshot + Metadata

A commit points to a tree (the snapshot) and includes metadata:

```
commit <size>\0
  tree <tree-hash>
  parent <parent-commit-hash>    ← (omitted for first commit)
  author Alice <alice@dev> 1710000000 +0000
  committer Alice <alice@dev> 1710000000 +0000

  Initial commit
```

### The Object Graph

```
┌─────────────────────────────────────────────────────────────────┐
│                       GIT OBJECT GRAPH                          │
│                                                                 │
│  commit c1a2b3                                                  │
│  ┌──────────────────┐                                           │
│  │ tree    → a4f5e6 │──────┐                                    │
│  │ parent  → (none) │      │                                    │
│  │ author  → Alice  │      ▼                                    │
│  │ message → "init" │   tree a4f5e6                              │
│  └──────────────────┘   ┌──────────────────────┐                │
│                         │ hello.txt → blob b7c8 │               │
│                         │ src/      → tree d9e0 │──┐            │
│                         └──────────────────────┘  │            │
│                              │                     │            │
│                              ▼                     ▼            │
│                         blob b7c8d9           tree d9e0f1       │
│                         ┌──────────┐          ┌─────────────┐   │
│                         │ Hello,   │          │ main.c → e2 │   │
│                         │ world!   │          └─────────────┘   │
│                         └──────────┘               │            │
│                                                    ▼            │
│                                               blob e2f3g4       │
│                                               ┌───────────┐    │
│                                               │ #include   │    │
│                                               │ <stdio.h>  │    │
│                                               └───────────┘    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## 5. The Object Database (.git/objects)

Git stores objects on disk under `.git/objects/`, using the first two hex characters
as a directory name and the rest as the filename:

```
.git/
├── objects/
│   ├── af/
│   │   └── 5626b4a114abcb82d63db7c8082c3c4756e51b   ← blob
│   ├── b3/
│   │   └── 4f2e8a9c01d7... ← tree
│   ├── c1/
│   │   └── a2b3e4f5d6a7... ← commit
│   └── ...
├── refs/
│   ├── heads/
│   │   └── main            ← contains: c1a2b3e4f5...
│   └── tags/
└── HEAD                    ← contains: ref: refs/heads/main
```

This two-level directory structure prevents any single directory from having too
many entries (which would slow down the filesystem).

---

## 6. Refs, Branches, and HEAD

Objects are immutable and identified by hash. But humans don't want to remember
40-character hex strings. **Refs** are human-readable names that point to commits.

- **Branch** (`refs/heads/main`): a file containing a commit hash. When you make a
  new commit, the branch ref is updated to point to it.
- **HEAD**: usually contains `ref: refs/heads/main`, meaning "I'm on the main
  branch." It can also point directly to a commit hash (detached HEAD).
- **Tag** (`refs/tags/v1.0`): like a branch that never moves.

```
HEAD ──→ refs/heads/main ──→ commit c3
                                │
                                ▼
                             commit c2
                                │
                                ▼
                             commit c1
```

When you `git commit`, Git:
1. Creates blob objects for changed files
2. Creates a tree object for the directory
3. Creates a commit object pointing to the tree + parent commit
4. Updates the current branch ref to point to the new commit

---

## 7. Directed Acyclic Graph (DAG)

Git commits form a **DAG** — Directed Acyclic Graph. Each commit points to its
parent(s), creating a graph that:
- Is **directed**: edges go from child to parent
- Is **acyclic**: you can never follow parent pointers and end up where you started
- Can have **multiple parents** (merge commits)
- Can have **multiple children** (branches)

```
┌──────────────────────────────────────────────────────────┐
│                     DAG OF COMMITS                        │
│                                                          │
│         c1 ◄── c2 ◄── c3 ◄── c4 ◄── c7    main         │
│                  │                     ▲                  │
│                  │              merge──┘                  │
│                  │                     │                  │
│                  └──── c5 ◄── c6 ─────┘    feature       │
│                                                          │
│  Reading the graph:                                      │
│  • c7 is a merge commit with TWO parents: c4 and c6     │
│  • c5 branched off from c2                               │
│  • Every commit except c1 has at least one parent        │
│  • c1 is the "root" commit (no parents)                  │
│                                                          │
│  Time flows left → right (older → newer)                 │
│  Arrows point backward (child → parent)                  │
└──────────────────────────────────────────────────────────┘
```

The DAG structure is what makes Git's branching and merging so powerful. Branches
are just pointers into the DAG, and merging creates a new commit with two parents.

---

## 8. Merkle Trees — Integrity Guarantees

Git's object graph is actually a **Merkle tree** — a tree where every node's hash
depends on the hashes of its children. This creates a chain of trust:

```
┌──────────────────────────────────────────────────────────┐
│                     MERKLE TREE                           │
│                                                          │
│              commit abc123                                │
│              hash = SHA1(tree + parent + metadata)        │
│                     │                                    │
│                     ▼                                    │
│              tree def456                                  │
│              hash = SHA1(entries)                         │
│              ┌──────┴──────┐                             │
│              │             │                             │
│              ▼             ▼                             │
│         blob 789abc   tree fedcba                         │
│         "Hello!\n"    hash = SHA1(entries)                │
│                            │                             │
│                            ▼                             │
│                       blob 112233                         │
│                       "int main..."                       │
│                                                          │
│  INTEGRITY: If ANYONE changes blob 112233, then:         │
│    • tree fedcba gets a different hash                    │
│    • tree def456 gets a different hash                    │
│    • commit abc123 gets a different hash                  │
│                                                          │
│  → A single bit change ANYWHERE invalidates the          │
│    ENTIRE chain up to the commit!                        │
│                                                          │
│  This is how Git detects corruption and tampering.       │
│                                                          │
│  The dependency chain step by step:                      │
│  Changing a blob changes its SHA-1 hash. The tree that   │
│  lists that blob stores the old hash, so its content     │
│  changes too — giving it a new hash. The commit that     │
│  points to that tree now has a different tree hash, so   │
│  the commit hash also changes. One changed byte          │
│  cascades through the entire chain.                      │
└──────────────────────────────────────────────────────────┘
```

This is the same principle behind blockchain, certificate transparency, and other
systems that need to guarantee data hasn't been tampered with.

---

## Exercises

1. **`sha1.c`** — Implement a hash function and Git's blob hashing format.
   Learn how content-addressable storage works by computing hashes and seeing
   that same content always produces the same identifier.

2. **`objects.c`** — Build an in-memory Git object store. Store blobs, retrieve
   them by hash, and construct tree objects that reference blobs by name.

---

## Key Takeaways

- Git is a **content-addressable filesystem** — objects are named by their hash
- Three object types: **blob** (file), **tree** (directory), **commit** (snapshot)
- Branches are just **pointers** (refs) to commits — creating/deleting branches is instant
- History forms a **DAG** — directed acyclic graph of commits
- The Merkle tree structure guarantees **integrity** — any change is detectable
- Understanding Git internals makes you *much* better at using Git day-to-day

---

[← Previous: Module 22 — Building a Text Editor](../22-building-text-editor/README.md) | [Next: Module 24 — Database: Key-Value Store →](../24-database-key-value-store/README.md)
