# Module 15 — Trees & Heaps

## What is a Tree?

A **tree** is a data structure where each element (called a **node**) has zero or more
children, forming a hierarchy. Think of a family tree or a file system:

```
         Documents/
        /    |    \
     Work  Photos  School
     /  \          |
  report  slides  essay
```

Every tree has these parts:

```
            [50]           <--- root (the top node, no parent)
           /    \
        [30]    [70]       <--- internal nodes (have children)
        /  \       \
     [20]  [40]   [80]    <--- leaves (no children)
```

### Terminology

| Term       | Meaning                                          |
|------------|--------------------------------------------------|
| **Root**   | The topmost node (no parent)                     |
| **Leaf**   | A node with no children                          |
| **Edge**   | A connection between a parent and child           |
| **Depth**  | Distance from root to a node (root = depth 0)    |
| **Height** | Distance from a node to its deepest leaf          |
| **Level**  | All nodes at the same depth                       |

```
  Depth:       Height:

    0  [50]        2  [50]
      /    \          /    \
    1  [30]  [70]  1  [30]  [70]
      /  \     \      /  \     \
    2  [20][40] [80] 0 [20][40] [80]  0
```

---

## Binary Trees

A **binary tree** is a tree where every node has **at most 2 children**: a left child
and a right child.

```c
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;
```

In memory, each node is a struct on the heap with two pointers:

```
  Node at 0x1000:
  +------+------+------+
  | data | left | right|
  |  50  |0x2000|0x3000|
  +------+------+------+
              |       |
              v       v
         0x2000:   0x3000:
         +--+--+--+ +--+--+--+
         |30|  |  | |70|  |  |
         +--+--+--+ +--+--+--+
```

---

## Tree Traversals

There are four standard ways to visit every node in a binary tree.
Consider this tree:

```
        [50]
       /    \
    [30]    [70]
    /  \       \
 [20]  [40]   [80]
```

### In-Order (Left, Root, Right)

Visit left subtree, then current node, then right subtree.
For a BST, this visits nodes in **sorted order**.

```
  Step:  1    2    3    4    5    6
  Node: [20] [30] [40] [50] [70] [80]
```

### Pre-Order (Root, Left, Right)

Visit current node first, then left, then right.
Useful for **copying** a tree or **serializing** it.

```
  Step:  1    2    3    4    5    6
  Node: [50] [30] [20] [40] [70] [80]
```

### Post-Order (Left, Right, Root)

Visit children first, then current node.
Useful for **deleting** a tree (free children before parent).

```
  Step:  1    2    3    4    5    6
  Node: [20] [40] [30] [80] [70] [50]
```

### Level-Order (Breadth-First)

Visit all nodes at depth 0, then depth 1, then depth 2, etc.
Uses a **queue** instead of recursion.

```
  Level 0:  [50]
  Level 1:  [30] [70]
  Level 2:  [20] [40] [80]

  Visit order: 50, 30, 70, 20, 40, 80
```

---

## Binary Search Tree (BST)

A BST is a binary tree with a **rule**: for every node, all values in the left
subtree are **less than** the node, and all values in the right subtree are
**greater than** the node.

```
         [50]
        /    \
     [30]    [70]        30 < 50 < 70  (correct)
     /  \       \
  [20]  [40]   [80]     20 < 30 < 40  (correct)
```

This ordering means we can **search** by always going left or right — like
binary search on a sorted array, but with a linked structure.

### BST Insert

To insert a value, walk down the tree: go left if the value is less than the
current node, go right if greater, until you find an empty spot.

**Inserting 5, 3, 7, 1, 4 into an empty BST:**

```
  Insert 5:     Insert 3:     Insert 7:     Insert 1:     Insert 4:

    [5]           [5]           [5]           [5]           [5]
                 /             /   \         /   \         /   \
               [3]           [3]   [7]     [3]   [7]     [3]   [7]
                                           /             /  \
                                         [1]           [1]  [4]
```

### BST Search

To search for a value, compare it to the current node:
- **Equal?** Found it.
- **Less?** Go left.
- **Greater?** Go right.
- **Hit NULL?** Not in the tree.

```
  Search for 4 in:
        [5]
       /   \
     [3]   [7]
     / \
   [1] [4]

  Step 1: 4 < 5  → go left  → [3]
  Step 2: 4 > 3  → go right → [4]
  Step 3: 4 == 4 → FOUND!
```

### BST Delete

Deleting is the trickiest operation. There are three cases:

**Case 1: Leaf node (no children)** — just remove it.

```
  Delete 4:
      [5]            [5]
     /   \    →     /   \
   [3]   [7]     [3]   [7]
   / \            /
 [1] [4]        [1]
```

**Case 2: One child** — replace the node with its child.

```
  Delete 3 (has left child only):
      [5]            [5]
     /   \    →     /   \
   [3]   [7]     [1]   [7]
   /
 [1]
```

**Case 3: Two children** — find the **in-order successor** (smallest node in
the right subtree), copy its value to the current node, then delete the
successor.

```
  Delete 5 (has two children):

      [5]         Find in-order        [7]
     /   \        successor: 7        /   \
   [3]   [8]     (leftmost in      [3]   [8]
         /        right subtree)          /
       [7]        Copy 7 to root,       [7.5]
         \        delete old 7
         [7.5]
```

### BST Complexity

| Operation | Average  | Worst (degenerate) |
|-----------|----------|--------------------|
| Search    | O(log n) | O(n)               |
| Insert    | O(log n) | O(n)               |
| Delete    | O(log n) | O(n)               |

The worst case happens when you insert sorted data — the tree degenerates
into a linked list:

```
  Insert 1, 2, 3, 4, 5:

  [1]
    \
    [2]
      \
      [3]        Height = n - 1
        \        Every operation walks
        [4]      the entire chain.
          \
          [5]
```

This is why we need **balanced** trees.

---

## AVL Trees — Self-Balancing BSTs

An **AVL tree** (named after Adelson-Velsky and Landis) is a BST that
automatically keeps itself balanced. The rule:

> For every node, the height difference between its left and right subtree
> (the **balance factor**) must be -1, 0, or +1.

```
  Balance Factor = height(left) - height(right)

       [50]  bf = 1 - 1 = 0
       /  \
    [30]  [70]  bf = 0 - 0 = 0
```

When an insert or delete causes a balance factor of +2 or -2, we fix it
with **rotations**.

### The Four Rotation Cases

#### Case 1: Left-Left (LL) — Right Rotation

The tree is heavy on the left-left side. Fix with a single right rotation.

```
  BEFORE (bf=+2 at 30):       AFTER right rotation:

        [30]                      [20]
        /                        /    \
      [20]                    [10]    [30]
      /
    [10]

  How it works:
    1. [20] becomes the new root
    2. [30] becomes [20]'s right child
    3. [20]'s old right child (if any) becomes [30]'s left child
```

#### Case 2: Right-Right (RR) — Left Rotation

Mirror of LL. The tree is heavy on the right-right side.

```
  BEFORE (bf=-2 at 10):       AFTER left rotation:

    [10]                          [20]
       \                         /    \
       [20]                   [10]    [30]
          \
          [30]

  How it works:
    1. [20] becomes the new root
    2. [10] becomes [20]'s left child
    3. [20]'s old left child (if any) becomes [10]'s right child
```

#### Case 3: Left-Right (LR) — Left then Right Rotation

The tree is heavy on the left side, but the left child is right-heavy.
A single right rotation would not fix it. Instead: rotate left on the left
child first, then rotate right on the root.

```
  BEFORE (bf=+2 at 30):       STEP 1: Left rotate       STEP 2: Right rotate
                               on left child [10]:       on root [30]:
        [30]                       [30]                      [20]
        /                          /                        /    \
      [10]                       [20]                    [10]    [30]
         \                       /
         [20]                  [10]
```

#### Case 4: Right-Left (RL) — Right then Left Rotation

Mirror of LR. The right child is left-heavy.

```
  BEFORE (bf=-2 at 10):       STEP 1: Right rotate      STEP 2: Left rotate
                               on right child [30]:      on root [10]:
    [10]                       [10]                          [20]
       \                          \                         /    \
       [30]                       [20]                   [10]    [30]
       /                             \
     [20]                            [30]
```

### AVL Insert Example

Insert 5, 3, 7, 2, 4, 6, 8, 1 into an AVL tree:

```
  After 5, 3, 7:        After 2, 4:          After 6, 8:

       [5]                  [5]                  [5]
      /   \                /   \                /   \
    [3]   [7]            [3]   [7]            [3]   [7]
                         / \                  / \   / \
                       [2] [4]              [2][4][6][8]

  All balanced! (every bf is -1, 0, or +1)

  After inserting 1:
          [5]  bf = 3-2 = +1
         /   \
       [3]   [7]  bf at [3] = 2-1 = +1
       / \   / \
     [2][4][6][8]  bf at [2] = 1-0 = +1
     /
   [1]

  Still balanced! (max bf is +1)
```

### AVL Complexity

| Operation | Time     | Why                               |
|-----------|----------|-----------------------------------|
| Search    | O(log n) | Tree height is always O(log n)    |
| Insert    | O(log n) | Walk down + at most 2 rotations   |
| Delete    | O(log n) | Walk down + rotations up the path |

---

## Heaps

A **heap** is a complete binary tree stored in an **array**. There are two types:
- **Min-heap**: parent <= both children (root is the minimum)
- **Max-heap**: parent >= both children (root is the maximum)

### The Array Representation

The key insight: a complete binary tree can be stored in an array without any
pointers. The parent-child relationships are computed from array indices:

```
  Tree view:                     Array view:

         [10]                    Index:  0   1   2   3   4   5   6
        /    \                   Value: [10][20][15][30][40][50][25]
     [20]    [15]
     /  \    /  \
  [30] [40][50] [25]

  For node at index i:
    Left child:   2*i + 1
    Right child:  2*i + 2
    Parent:       (i - 1) / 2
```

Let's verify:
```
  Node [10] at index 0:  left = 2*0+1 = 1 ([20])   right = 2*0+2 = 2 ([15])
  Node [20] at index 1:  left = 2*1+1 = 3 ([30])   right = 2*1+2 = 4 ([40])
  Node [15] at index 2:  left = 2*2+1 = 5 ([50])   right = 2*2+2 = 6 ([25])
  Node [30] at index 3:  parent = (3-1)/2 = 1 ([20])
```

### Heap Insert (Bubble Up / Sift Up)

To insert into a min-heap:
1. Put the new element at the **end** of the array (next available slot).
2. **Bubble up**: compare with parent, swap if smaller, repeat.

```
  Insert 5 into min-heap:

  Step 1: Add at end           Step 2: Bubble up (5 < 40)
  [10][20][15][30][40][50][25][5]  [10][20][15][30][ 5][50][25][40]
                            ^                   ^   ^
                           new                swap  swap

  Step 3: Bubble up (5 < 20)    Step 4: Done (5 > 10, stop)
  [10][ 5][15][30][20][50][25][40]
        ^       ^
       swap    swap

  Final tree:
         [ 5]
        /    \
     [10]    [15]
     /  \    /  \
  [30] [20][50] [25]
  /
[40]
```

Wait, that's not right. Let me re-trace:

```
  Start:                         Insert 5 at end (index 7):
  [10][20][15][30][40][50][25]   [10][20][15][30][40][50][25][ 5]

         [10]                            [10]
        /    \                          /    \
     [20]    [15]                    [20]    [15]
     /  \    /  \                    /  \    /  \
  [30] [40][50] [25]              [30] [40][50] [25]
                                  /
                                [5]

  5's parent is at (7-1)/2 = 3 → value 30.  5 < 30, swap.
  [10][20][15][ 5][40][50][25][30]

  5 is now at index 3. Parent is at (3-1)/2 = 1 → value 20.  5 < 20, swap.
  [10][ 5][15][20][40][50][25][30]

  5 is now at index 1. Parent is at (1-1)/2 = 0 → value 10.  5 < 10, swap.
  [ 5][10][15][20][40][50][25][30]

  5 is now at index 0 (root). Done.

  Final:
         [ 5]
        /    \
     [10]    [15]
     /  \    /  \
  [20] [40][50] [25]
  /
[30]
```

### Heap Extract Min (Bubble Down / Sift Down)

To remove the minimum (root) from a min-heap:
1. Replace root with the **last** element.
2. **Bubble down**: compare with children, swap with the smaller child, repeat.

```
  Extract min from:
  [ 5][10][15][20][40][50][25][30]

  Step 1: Move last element (30) to root, shrink array:
  [30][10][15][20][40][50][25]

         [30]
        /    \
     [10]    [15]
     /  \    /  \
  [20] [40][50] [25]

  Step 2: Bubble down. Children of 30: 10, 15. Min child = 10. 30 > 10, swap.
  [10][30][15][20][40][50][25]

  Step 3: Children of 30 (now at index 1): 20, 40. Min child = 20. 30 > 20, swap.
  [10][20][15][30][40][50][25]

  Step 4: 30 is at index 3. Children: none within bounds. Done.

  Final:
         [10]
        /    \
     [20]    [15]
     /  \    /  \
  [30] [40][50] [25]

  Extracted value: 5
```

### Heap Complexity

| Operation  | Time     | Why                              |
|------------|----------|----------------------------------|
| Insert     | O(log n) | Bubble up at most height levels  |
| Extract    | O(log n) | Bubble down at most height levels|
| Peek (min) | O(1)     | Just return root                 |
| Build heap | O(n)     | Bottom-up heapify                |

---

## Heap Sort

**Heap sort** uses the heap property to sort an array in-place.

For sorting in **ascending** order, we use a **max-heap**:

1. **Build a max-heap** from the array (in-place).
2. Repeatedly **extract the max** (swap root with last unsorted element,
   shrink the heap, sift down).

```
  Array: [4, 1, 3, 2, 5]

  Step 1: Build max-heap (heapify from bottom up):
  [5, 4, 3, 2, 1]

         [5]
        /   \
      [4]   [3]
      / \
    [2] [1]

  Step 2: Extract max repeatedly:

  Swap 5 with last → [1, 4, 3, 2, | 5]   Sift down → [4, 2, 3, 1, | 5]
  Swap 4 with last → [1, 2, 3, | 4, 5]   Sift down → [3, 2, 1, | 4, 5]
  Swap 3 with last → [1, 2, | 3, 4, 5]   Sift down → [2, 1, | 3, 4, 5]
  Swap 2 with last → [1, | 2, 3, 4, 5]   Done!

  Result: [1, 2, 3, 4, 5]
```

Heap sort complexity: **O(n log n)** time, **O(1)** extra space.

---

## Priority Queues

A **priority queue** is an abstract data type where each element has a priority.
The element with the highest priority (lowest value in a min-heap) is served first.

A min-heap is the perfect implementation:
- **Insert**: O(log n)
- **Get highest priority**: O(1) — just peek at root
- **Remove highest priority**: O(log n) — extract min

Real-world uses:
- Dijkstra's shortest path algorithm
- Task scheduling (OS process scheduler)
- Huffman encoding (data compression)
- Event-driven simulation

---

## Prefix Sums

### The Problem

You have an array of numbers and someone keeps asking: "What is the sum from
index 2 to index 7?" "What about index 0 to 3?" Each query scans the range —
O(n) per query. If you answer 1000 queries on an array of 10,000 elements,
that's 10 million operations.

### The Insight: Precompute Running Totals

Build a **prefix sum** array where `prefix[i]` = sum of the first `i` elements:

```
  Original:    a[] = [3,  1,  4,  1,  5]
  Prefix:  prefix[] = [0,  3,  4,  8,  9, 14]
                       ^                    ^
                  empty sum           sum of all 5

  prefix[0] = 0                  (no elements)
  prefix[1] = 3                  (just a[0])
  prefix[2] = 3 + 1 = 4         (a[0] + a[1])
  prefix[3] = 3 + 1 + 4 = 8     (a[0] + a[1] + a[2])
  prefix[4] = 3 + 1 + 4 + 1 = 9
  prefix[5] = 3 + 1 + 4 + 1 + 5 = 14
```

Now any range sum is a single subtraction:

```
  sum of a[l..r] = prefix[r+1] - prefix[l]

  Example: sum of a[1..3] = prefix[4] - prefix[1] = 9 - 3 = 6

  Visually:
    index:   0    1    2    3    4
    value: [ 3 ][ 1 ][ 4 ][ 1 ][ 5 ]
                 |<-- sum = 6 -->|
                 l=1             r=3

    prefix[4] covers a[0..3] = 9
  - prefix[1] covers a[0..0] = 3
    ──────────────────────────────
    result = a[1..3]         = 6
```

### Complexity

| Operation      | Naive   | Prefix Sums |
|----------------|---------|-------------|
| Build          | —       | O(n)        |
| Range query    | O(n)    | O(1)        |
| Update element | O(1)    | O(n) !!     |

The limitation: if you change an element, you must rebuild the entire prefix
array — O(n). This motivates segment trees and Fenwick trees.

---

## Bit Manipulation

### Review: Binary Numbers and Two's Complement

You learned binary in Module 00 and two's complement in Module 02. Quick recap:

```
  Decimal 12 in binary:  0000 1100
  Two's complement of -12:
    Step 1: Flip all bits:  1111 0011
    Step 2: Add 1:          1111 0100
```

### Core Bit Operations

```
  AND (&):  1 & 1 = 1, all others = 0    (both must be 1)
  OR  (|):  0 | 0 = 0, all others = 1    (either can be 1)
  XOR (^):  same = 0, different = 1       (must differ)
  NOT (~):  flip every bit
  <<  :     shift left  (multiply by 2)
  >>  :     shift right (divide by 2)
```

### The Lowest Set Bit Trick: n & (-n)

This is THE key operation for Fenwick trees. It extracts the lowest 1-bit:

```
     n  = 12 = 0000 1100
    -n  =      1111 0100    (flip all bits, add 1)
   n&-n =      0000 0100 = 4

  Why? Flipping turns every bit below the lowest 1 into 1s, and the
  lowest 1 into 0. Adding 1 ripples through those 1s, setting exactly
  the lowest 1 back. Everything above it cancels in the AND.

  Another example:
     n  = 10 = 1010
    -n  =      0110
   n&-n =      0010 = 2
```

### Brian Kernighan's Trick: n & (n-1) Clears the Lowest Set Bit

```
  n   = 13 = 1101
  n-1 = 12 = 1100    (borrow flips lowest 1 and everything below)
  n & (n-1) = 1100 = 12

  Next iteration:
  n   = 12 = 1100
  n-1 = 11 = 1011
  n & (n-1) = 1000 = 8

  Next:
  n   = 8  = 1000
  n-1 = 7  = 0111
  n & (n-1) = 0000 = 0    → done after 3 iterations = 3 set bits
```

This counts set bits in O(k) where k is the number of 1-bits — faster than
checking all 32 bits.

### XOR Properties

```
  XOR Truth Table:
    a | b | a^b
   ---+---+-----
    0 | 0 |  0
    0 | 1 |  1
    1 | 0 |  1
    1 | 1 |  0

  Key properties:
    a ^ a = 0    (anything XOR itself is zero)
    a ^ 0 = a    (XOR with zero gives itself)
    a ^ b ^ a = b  (XOR is its own inverse)
```

These properties let you swap two values without a temporary variable and
are the basis for many cryptographic operations.

---

## Segment Tree

### The Problem: Range Queries WITH Updates

Prefix sums give O(1) range queries but O(n) updates. What if we need both
to be fast? A **segment tree** gives O(log n) for both.

### The Idea

Build a binary tree where each node stores the answer for a **range** of the
array. The root covers the whole array. Each node's children split the range
in half.

For array `[1, 3, 5, 7, 9, 11]`:

```
                      [36]                   range [0,5] (sum of all)
                    /      \
                [9]          [27]            [0,2] and [3,5]
               /   \        /    \
            [4]    [5]   [16]   [11]         [0,1],[2,2],[3,4],[5,5]
            / \          / \
          [1] [3]     [7] [9]               leaves = original values
```

### Storage: Array-Based (Like Heaps)

Node `i` has children at `2*i+1` and `2*i+2`. An array of size `4*n` is
always sufficient.

### Query: Which Nodes to Visit?

For range `[1,4]` (sum = 3 + 5 + 7 + 9 = 24):

```
  At each node, ask: does my range overlap [1,4]?

  [36] range [0,5] — partial overlap → recurse both children
    ├─ [9] range [0,2] — partial overlap → recurse
    │   ├─ [4] range [0,1] — partial → recurse
    │   │   ├─ [1] range [0,0] — no overlap → return 0
    │   │   └─ [3] range [1,1] — complete overlap → return 3
    │   └─ [5] range [2,2] — complete overlap → return 5
    └─ [27] range [3,5] — partial overlap → recurse
        ├─ [16] range [3,4] — complete overlap → return 16
        └─ [11] range [5,5] — no overlap → return 0

  Result: 0 + 3 + 5 + 16 + 0 = 24
```

### Update: Leaf to Root

To change index 2 from 5 to 10, walk from the leaf to the root:

```
  Leaf [5] → [10]
  Parent [9] → [14]  (4 + 10)
  Grandparent [36] → [41]  (14 + 27)
```

### Complexity Comparison

| Operation      | Naive   | Prefix Sums | Segment Tree |
|----------------|---------|-------------|--------------|
| Build          | —       | O(n)        | O(n)         |
| Range query    | O(n)    | O(1)        | O(log n)     |
| Point update   | O(1)    | O(n)        | O(log n)     |

Segment trees trade slightly slower queries for much faster updates.

---

## Fenwick Tree (Binary Indexed Tree)

### The Elegant Alternative

A Fenwick tree (also called a Binary Indexed Tree or BIT) achieves the same
O(log n) query and update as a segment tree, but with simpler code, less
memory, and better cache performance.

### The Trick: Lowest Set Bit Determines Range

Each index `i` stores a partial sum covering a range whose **length equals
the lowest set bit of i**:

```
  Index (binary)  Responsible for range    # elements (= lowest set bit)
  1  (0001)       [1, 1]                   1
  2  (0010)       [1, 2]                   2
  3  (0011)       [3, 3]                   1
  4  (0100)       [1, 4]                   4
  5  (0101)       [5, 5]                   1
  6  (0110)       [5, 6]                   2
  7  (0111)       [7, 7]                   1
  8  (1000)       [1, 8]                   8
```

### Why 1-Indexed?

The bit trick `i & (-i)` gives 0 when `i = 0`, which would cause an infinite
loop. Starting at index 1 avoids this.

### Query: Strip the Lowest Set Bit

To compute prefix sum [1..7]:

```
  i = 7 (0111): sum += tree[7]   → i -= 0001 → i = 6
  i = 6 (0110): sum += tree[6]   → i -= 0010 → i = 4
  i = 4 (0100): sum += tree[4]   → i -= 0100 → i = 0
  Done! sum = tree[7] + tree[6] + tree[4]

  tree[7] covers [7,7], tree[6] covers [5,6], tree[4] covers [1,4]
  Together: [1,7] — exactly what we wanted.
```

### Update: Add the Lowest Set Bit

To update index 3 (add a delta):

```
  i = 3 (0011): tree[3] += delta → i += 0001 → i = 4
  i = 4 (0100): tree[4] += delta → i += 0100 → i = 8
  i = 8 (1000): tree[8] += delta → i += 1000 → i = 16
  i > n, done. Updated tree[3], tree[4], tree[8] — all nodes whose
  range includes index 3.
```

### When to Use Which?

```
  ┌──────────────────┬──────────────┬──────────────┐
  │                  │ Segment Tree │ Fenwick Tree │
  ├──────────────────┼──────────────┼──────────────┤
  │ Point update     │ O(log n)     │ O(log n)     │
  │ Range query      │ O(log n)     │ O(log n)     │
  │ Space            │ 4n           │ n + 1        │
  │ Code complexity  │ ~40 lines    │ ~10 lines    │
  │ Supports min/max │ Yes          │ No           │
  │ Lazy propagation │ Yes          │ No           │
  │ Cache friendly   │ Less         │ More         │
  └──────────────────┴──────────────┴──────────────┘

  Use Fenwick when: you need prefix-sum-style queries (sum, XOR, count)
  Use Segment tree when: you need min, max, GCD, or range updates
```

---

## Tries (Prefix Trees)

A **trie** (from "reTRIEval") is a tree where each edge represents a character
and paths from root to nodes spell out strings. It's the go-to data structure
for prefix-based operations: autocomplete, spell checking, IP routing.

### What a Trie Looks Like

After inserting "app", "apple", "application", "bat", "ball":

```
               (root)
              /      \
            a          b
            |          |
            p          a
            |         / \
            p        l   t*
           /|\       |
          *  l  i    l*
             |  |
             e  c
             |  |
             *  a
                |
                t
                |
                i
                |
                o
                |
                n*

  * = is_end_of_word (a complete word ends here)
```

### Insert Walkthrough: Adding "apple"

```
  Start with empty trie (just root):

  Step 1: 'a' — root has no child 'a', create node → move to it
          prefix_count at 'a' node: 1

  Step 2: 'p' — 'a' node has no child 'p', create node → move to it
          prefix_count at 'p' node: 1

  Step 3: 'p' — first 'p' node has no child 'p', create node → move to it
          prefix_count at second 'p' node: 1

  Step 4: 'l' — second 'p' node has no child 'l', create node → move to it
          prefix_count at 'l' node: 1

  Step 5: 'e' — 'l' node has no child 'e', create node → move to it
          prefix_count at 'e' node: 1
          Mark is_end_of_word = 1

  Result:
      (root) → [a] → [p] → [p] → [l] → [e]*
               pc=1  pc=1  pc=1  pc=1  pc=1
```

### Search vs starts_with

```
  Trie contains: "app", "apple"

  search("app")    → Walk a→p→p.  End of word? YES → return 1
  search("ap")     → Walk a→p.    End of word? NO  → return 0
  search("apt")    → Walk a→p.    Child 't'? NULL   → return 0

  starts_with("ap")  → Walk a→p.  Reached end of prefix? YES → return 1
  starts_with("ax")  → Walk a.    Child 'x'? NULL → return 0
```

The difference: `search` needs `is_end_of_word` to be set at the last node.
`starts_with` only cares that the path exists.

### prefix_count: Why It's Useful

Each node tracks how many words pass through it. This lets you answer
"how many words start with this prefix?" in O(L) time without scanning.

```
  After inserting "app", "apple", "application":

  root → [a] → [p] → [p] → ...
         pc=3  pc=3  pc=3

  count_prefix("app") = 3  (walk to the second 'p' node, return its count)
  count_prefix("apple") = 1
```

### Trie Data Structure

```c
#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];  /* one slot per letter */
    int is_end_of_word;     /* 1 if a complete word ends here */
    int prefix_count;       /* how many words pass through this node */
} TrieNode;
```

Each node has 26 child pointers (a-z). Most will be NULL — that's fine.
To map a character to an index: `index = c - 'a'`.

### Complexity

| Operation      | Time    | Why                                          |
|----------------|---------|----------------------------------------------|
| Insert         | O(L)    | Walk/create L nodes (L = word length)        |
| Search         | O(L)    | Walk L nodes                                 |
| Starts with    | O(L)    | Walk L nodes                                 |
| Count prefix   | O(L)    | Walk L nodes, read prefix_count              |
| Autocomplete   | O(L+R)  | Walk L nodes + DFS over R result characters  |
| Delete         | O(L)    | Walk L nodes, clean up                       |

### Trie vs Hash Table

```
  ┌──────────────────┬────────────┬─────────────────────┐
  │ Operation        │ Hash Table │ Trie                │
  ├──────────────────┼────────────┼─────────────────────┤
  │ Exact lookup     │ O(1) avg   │ O(L)                │
  │ Prefix search    │ O(n)       │ O(L)                │
  │ Autocomplete     │ O(n)       │ O(L + results)      │
  │ Memory           │ Lower      │ Higher (26 ptrs/node│
  │ Ordered iteration│ No         │ Yes (alphabetical)  │
  └──────────────────┴────────────┴─────────────────────┘

  L = word length, n = total words in structure
```

Hash tables win on exact lookup. Tries win on anything prefix-related.

---

## Top-K and Heap Applications

### The Pattern

When you need the "k best" or "k worst" of something, think **heap**.

The key insight for "top k largest": use a **min-heap of size k**. The root
is the SMALLEST of the k largest elements seen so far. Any new element bigger
than the root deserves a spot — pop the root and push the new element.

```
  Stream: 7, 2, 9, 1, 5, 8, 3    (find top 3 largest)

  Min-heap of size 3:

  After 7:       After 2:       After 9:       After 1:
    [7]           [2]            [2]            [2]
                  / \            / \            / \
                [7]            [7] [9]        [7] [9]
                                              (1 < 2, skip)

  After 5:       After 8:       After 3:
    [5]            [7]            [7]
    / \            / \            / \
  [7] [9]       [8] [9]        [8] [9]
  (5>2, replace) (8>5, replace) (3 < 7, skip)

  Final heap root = 7 = the 3rd largest.  Heap contains {7, 8, 9}.
```

### K-Way Merge: The Conveyor Belt Analogy

Imagine k conveyor belts, each carrying items in sorted order. You always
pick the smallest item visible across all belts, then advance that belt.

A min-heap of size k makes this efficient — always O(log k) to find the
smallest:

```
  Array 1: [1, 4, 5]     Array 2: [1, 3, 4]     Array 3: [2, 6]
             ^                      ^                      ^

  Heap: [1(arr1), 1(arr2), 2(arr3)]

  Pop 1(arr1) → output: [1]     advance arr1
  Heap: [1(arr2), 2(arr3), 4(arr1)]

  Pop 1(arr2) → output: [1,1]   advance arr2
  Heap: [2(arr3), 4(arr1), 3(arr2)]

  Pop 2(arr3) → output: [1,1,2] advance arr3
  ...and so on until all arrays are exhausted.

  Final: [1, 1, 2, 3, 4, 4, 5, 6]
```

### Median of a Stream: The Two-Heap Trick

Keep all numbers split into two halves using two heaps:

```
  max-heap (lower half)         min-heap (upper half)
  Root = largest of small       Root = smallest of large

       [3]                           [5]
       / \                           / \
     [1] [2]                       [7] [8]

  Lower half: {1, 2, 3}         Upper half: {5, 7, 8}

  If same size: median = (max_root + min_root) / 2 = (3 + 5) / 2 = 4.0
  If max-heap bigger: median = max_root
  If min-heap bigger: median = min_root
```

Balancing rule: after each insertion, if one heap has 2+ more elements
than the other, move its root to the other heap.

### Complexity

```
  ┌─────────────────────┬──────────────┬──────────────────────────────┐
  │ Problem             │ Time         │ Space                        │
  ├─────────────────────┼──────────────┼──────────────────────────────┤
  │ Top-K frequent      │ O(n + m·logk)│ O(n) freq array + O(k) heap │
  │ Kth largest         │ O(n·log k)   │ O(k)                        │
  │ Merge K sorted      │ O(N·log k)   │ O(k) heap + O(N) output     │
  │ Median of stream    │ O(n·log n)   │ O(n)                        │
  └─────────────────────┴──────────────┴──────────────────────────────┘

  n = input size, k = parameter, N = total elements across all arrays
  m = number of unique values (for top-K frequent)
```

---

## Binary Tree Interview Patterns

Beyond basic traversals, these patterns appear constantly in technical interviews.

### Height vs Depth

**Height** is measured bottom-up: the distance from a node to its deepest leaf.
**Depth** is measured top-down: the distance from the root to a node.
The height of the tree = the depth of its deepest leaf.

### Diameter — The "Global Variable" Trick

The diameter is the longest path between any two nodes (measured in edges).
The key insight: the longest path through any node is `left_height + right_height`.
Use a helper that computes height but also tracks the maximum diameter seen:

```
  int diameter_helper(TreeNode *root, int *max_diameter) {
      // returns height, but updates *max_diameter as a side effect
      int left_h = diameter_helper(root->left, max_diameter);
      int right_h = diameter_helper(root->right, max_diameter);
      *max_diameter = max(*max_diameter, left_h + right_h);
      return 1 + max(left_h, right_h);
  }
```

### Lowest Common Ancestor — The Elegant Recursive Solution

```
  Search left, search right:
  - Both found   -> current node is the LCA
  - Only left    -> LCA is in left subtree
  - Only right   -> LCA is in right subtree
  - Neither      -> neither p nor q is in this subtree
```

Why it works: if p and q are in different subtrees of a node, that node MUST
be their LCA. If they're in the same subtree, the recursion finds the LCA deeper.

### Level-Order BFS — Using a Queue

Process one level at a time by recording the queue size before dequeuing:

```
  while queue not empty:
      level_size = queue.size
      for i in 0..level_size-1:
          node = dequeue
          process(node)
          if node->left: enqueue(node->left)
          if node->right: enqueue(node->right)
```

### Right Side View

Just the LAST element of each BFS level. Process all nodes in a level,
but only record the final one.

### Zigzag Level Order

Normal BFS, but reverse every odd-numbered level before storing it.

### Serialization — Preorder with NULL Markers

A preorder traversal with explicit "null" markers is sufficient to uniquely
reconstruct any binary tree. The key: "null" markers encode the STRUCTURE
(which children are missing), not just the values.

```
       1
      / \
     2   3  =>  "1,2,null,null,3,null,null"
```

To deserialize: read tokens left to right, recursively build left child
then right child. "null" tokens become NULL leaves.

---

## Exercises

1. **`bst.c`** — Implement a binary search tree with insert, search, delete,
   in-order traversal, height, and size. ~16 tests.

2. **`heap.c`** — Implement a min-heap with insert, extract-min, peek, and
   heap sort. ~14 tests.

3. **`prefix_sums.c`** — Build prefix sums, answer range queries in O(1),
   count evens in a range, find equilibrium index, count subarrays with
   target sum. ~15 tests.

4. **`bit_tricks.c`** — Lowest set bit, popcount, power-of-two check, XOR
   swap, next power of two. Foundation for Fenwick trees. ~17 tests.

5. **`segment_tree.c`** — Build, query, and update a segment tree for range
   sum and range minimum queries. ~14 tests.

6. **`fenwick_tree.c`** — Build, update, query, range query, and point query
   on a Fenwick tree (Binary Indexed Tree). ~14 tests.

7. **`trie.c`** — Implement a trie with insert, search, starts_with,
   count_prefix, autocomplete, and destroy. ~18 tests.

8. **`top_k.c`** — Top-K frequent elements, kth largest, merge K sorted
   arrays, and median of a stream using heaps. ~15 tests.

9. **`tree_patterns.c`** — Classic binary tree interview patterns: height,
   diameter, LCA, right side view, zigzag traversal, serialization. ~20 tests.

---

## Debug Challenge

| File | Description | Bugs |
|------|-------------|------|
| `debug_bst.c` | Find and fix 5 binary search tree bugs | 5 |

These exercises contain **intentionally broken code**. Your job is to find and
fix each bug. Each function has a comment explaining what it SHOULD do and a
HINT about the bug class. Run the program — failing tests tell you which
functions are still broken.

```bash
make debug    # compile the buggy version
./exercises/debug_bst   # see which tests fail
# ... fix bugs ...
# recompile and rerun until all tests pass
```

---

[← Previous: Module 14: Sorting & Searching](../22-sorting-searching/README.md) | [Next: Module 16: File I/O & the Unix Philosophy →](../13-file-io-unix-philosophy/README.md)
