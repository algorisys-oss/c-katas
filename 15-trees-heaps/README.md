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

## Exercises

1. **`bst.c`** — Implement a binary search tree with insert, search, delete,
   in-order traversal, height, and size. ~16 tests.

2. **`heap.c`** — Implement a min-heap with insert, extract-min, peek, and
   heap sort. ~14 tests.

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

[← Previous: Module 14: Sorting & Searching](../14-sorting-searching/README.md) | [Next: Module 16: File I/O & the Unix Philosophy →](../16-file-io-unix-philosophy/README.md)
