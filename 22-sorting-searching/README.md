# Module 14: Sorting & Searching

## Overview

Sorting and searching are the bread and butter of algorithms. Nearly every
real program does one or both — databases sort query results, search engines
find matching documents, even your filesystem keeps directory entries in order
so it can look things up fast.

In this module you will:
- Implement **linear search** and **binary search** (with tricky variations)
- Build five classic sorting algorithms from scratch
- Understand **why** some sorts are fast and others are slow
- Learn the language of Big-O, Big-Omega, and Big-Theta
- Discover the **mathematical lower bound** that no comparison sort can beat

---

## Part 1: Searching

### Linear Search — The Obvious Way

Walk through the array one element at a time. If you find the target, return
its index. If you reach the end, return -1.

```c
int linear_search(const int *arr, int n, int target)
{
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}
```

**Time complexity**: O(n) — in the worst case you check every element.

This is fine for small arrays or unsorted data. But what if the array is
**sorted**? We can do much, much better.

### Binary Search — The Halving Trick

The key insight: if the array is sorted and you look at the **middle**
element, you instantly know which **half** the target must be in. Then you
repeat on that half. Each step cuts the problem in half.

```
Searching for 23 in a sorted array of 16 elements:

Step 1: Look at all 16 elements
[1  3  5  7  9  11  13  15 | 17  19  21  23  25  27  29  31]
                             ^
                     middle = 15, 23 > 15 → go RIGHT

Step 2: Look at 8 elements
                             [17  19  21  23 | 25  27  29  31]
                                               ^
                                       middle = 25, 23 < 25 → go LEFT

Step 3: Look at 4 elements
                             [17  19 | 21  23]
                                       ^
                               middle = 21, 23 > 21 → go RIGHT

Step 4: Look at 2 elements
                                      [21 | 23]
                                              ^
                                      middle = 23, FOUND at index 11!

16 → 8 → 4 → 2 → 1    (4 steps = log2(16))
```

**How many steps?** Each step halves the array. Starting from n elements:
- After 1 step: n/2
- After 2 steps: n/4
- After k steps: n/2^k
- Done when n/2^k = 1, so k = log2(n)

For 1,000,000 elements: log2(1,000,000) ≈ 20 steps. Twenty!

### Binary Search: The Off-by-One Minefield

Binary search is famously tricky to get right. The first correct binary search
was published in 1946. The first **bug-free** version? Not until 1962.
Even Java's standard library had a binary search bug for 9 years.

The danger zones:

**1. How to compute mid:**
```c
/* WRONG — overflows for large lo + hi */
int mid = (lo + hi) / 2;

/* CORRECT — no overflow */
int mid = lo + (hi - lo) / 2;
```

**2. Inclusive vs exclusive bounds:**
```c
/* Convention: lo is inclusive, hi is exclusive */
/* Array[lo..hi) means indices lo, lo+1, ..., hi-1 */
int lo = 0, hi = n;  /* NOT hi = n-1 */
while (lo < hi) {    /* NOT lo <= hi */
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] < target) {
        lo = mid + 1;  /* target is in [mid+1, hi) */
    } else {
        hi = mid;      /* target is in [lo, mid) — or mid IS the target */
    }
}
/* lo == hi — this is the answer (if arr[lo] == target) */
```

**3. Finding the FIRST occurrence** (when duplicates exist):
```
Array: [1, 3, 5, 5, 5, 5, 7, 9]
                  ^
       We want THIS 5 (index 2), not just any 5

Key: when you find the target, do NOT return immediately.
     Instead, record it and keep searching LEFT.
```

**4. Finding the INSERT POSITION** (where would target go?):
```
Array: [1, 3, 7, 9]   target = 5
             ^
      Insert at index 2 to keep sorted: [1, 3, 5, 7, 9]
```

This is the same as "find the first element >= target."

---

## Part 2: Simple Sorts (O(n^2))

These sorts are slow for large data but easy to understand and useful for
small arrays (< 20 elements or so).

### Bubble Sort

Walk through the array repeatedly. Each pass, compare adjacent pairs and swap
if they are out of order. After each pass, the largest unsorted element
"bubbles up" to its final position.

```
Sorting [5, 3, 8, 1, 2]:

Pass 1:  [5, 3, 8, 1, 2]
          ^--^              5 > 3 → swap
         [3, 5, 8, 1, 2]
             ^--^           5 < 8 → ok
         [3, 5, 8, 1, 2]
                ^--^        8 > 1 → swap
         [3, 5, 1, 8, 2]
                   ^--^     8 > 2 → swap
         [3, 5, 1, 2, 8]   ← 8 is in its final spot

Pass 2:  [3, 5, 1, 2, 8]
          ^--^              ok
         [3, 5, 1, 2, 8]
             ^--^           swap
         [3, 1, 5, 2, 8]
                ^--^        swap
         [3, 1, 2, 5, 8]   ← 5 is in its final spot

Pass 3:  [3, 1, 2, 5, 8]
          ^--^              swap
         [1, 3, 2, 5, 8]
             ^--^           swap
         [1, 2, 3, 5, 8]   ← 3 is in its final spot

Pass 4:  [1, 2, 3, 5, 8]
          ^--^              ok
         [1, 2, 3, 5, 8]   ← Sorted!
```

- **Best case**: O(n) — already sorted (with early-exit optimization)
- **Worst case**: O(n^2) — reverse sorted
- **Stable**: Yes (equal elements keep their relative order)

### Selection Sort

Find the minimum of the unsorted portion, swap it into position.

```
[5, 3, 8, 1, 2]
 ^        ^        min is 1 at index 3 → swap with index 0
[1, 3, 8, 5, 2]
    ^        ^     min is 2 at index 4 → swap with index 1
[1, 2, 8, 5, 3]
       ^     ^     min is 3 at index 4 → swap with index 2
[1, 2, 3, 5, 8]
          ^  ^     min is 5 at index 3 → already in place
[1, 2, 3, 5, 8]   Sorted!
```

- **Always O(n^2)** — even if already sorted, it still scans for the min
- **Not stable** — the swap can change relative order of equal elements

### Insertion Sort

Build up the sorted portion one element at a time. Take the next unsorted
element and slide it left into its correct position.

```
[5, 3, 8, 1, 2]
 ^  sorted: [5]     take 3, slide: [3, 5]
[3, 5, 8, 1, 2]
 ^--^  sorted: [3,5]  take 8, already in place: [3, 5, 8]
[3, 5, 8, 1, 2]
 ^-----^  sorted: [3,5,8]  take 1, slide all the way left: [1, 3, 5, 8]
[1, 3, 5, 8, 2]
 ^--------^  sorted: [1,3,5,8]  take 2, slide: [1, 2, 3, 5, 8]
[1, 2, 3, 5, 8]   Sorted!
```

- **Best case**: O(n) — already sorted (inner loop never runs)
- **Worst case**: O(n^2) — reverse sorted
- **Stable**: Yes
- **Adaptive**: Fast on nearly-sorted data (few inversions)

Insertion sort is the go-to choice for small sub-arrays inside faster
algorithms (e.g., switching to insertion sort when quicksort partitions get
small).

---

## Part 3: Divide-and-Conquer Sorts (O(n log n))

The big idea: split the problem into smaller pieces, solve each piece, then
combine the results. The "split in half, recurse, combine" pattern gives us
O(n log n) — vastly faster than O(n^2) for large n.

### Merge Sort

**Strategy**: Split the array in half, sort each half (recursively), then
**merge** the two sorted halves into one sorted whole.

```
Split phase (top-down):

        [5, 3, 8, 1, 2, 7, 4, 6]
               /            \
      [5, 3, 8, 1]      [2, 7, 4, 6]
        /      \           /      \
    [5, 3]   [8, 1]   [2, 7]   [4, 6]
     / \       / \       / \       / \
   [5] [3]  [8] [1]  [2] [7]  [4] [6]

Merge phase (bottom-up):

   [5] [3]  [8] [1]  [2] [7]  [4] [6]
     \ /       \ /       \ /       \ /
    [3, 5]   [1, 8]   [2, 7]   [4, 6]
        \      /           \      /
      [1, 3, 5, 8]      [2, 4, 6, 7]
               \            /
        [1, 2, 3, 4, 5, 6, 7, 8]
```

**The merge operation** is the heart of merge sort:

```
Merging [1, 3, 5, 8] and [2, 4, 6, 7]:

Left:  [1, 3, 5, 8]    Right: [2, 4, 6, 7]    Result: []
        ^                       ^
        1 < 2 → take 1                         Result: [1]

Left:  [1, 3, 5, 8]    Right: [2, 4, 6, 7]    Result: [1]
           ^                    ^
           3 > 2 → take 2                      Result: [1, 2]

Left:  [1, 3, 5, 8]    Right: [2, 4, 6, 7]    Result: [1, 2]
           ^                       ^
           3 < 4 → take 3                      Result: [1, 2, 3]

... and so on until both are exhausted.
```

- **Always O(n log n)** — guaranteed, no matter the input
- **Stable**: Yes
- **Extra space**: O(n) — needs a temporary buffer for merging
- **Great for**: linked lists, external sorting (files too big for RAM)

### Quicksort

**Strategy**: Pick a **pivot** element. **Partition** the array so everything
less than the pivot goes left and everything greater goes right. Then
recursively sort each side.

```
Quicksort with pivot = last element:

[5, 3, 8, 1, 2, 7, 4, 6]
                        ^  pivot = 6

Partition: everything ≤ 6 goes left, everything > 6 goes right

 ≤6: [5, 3, 1, 2, 4]   pivot: [6]   >6: [8, 7]

Now recursively sort each side:

[5, 3, 1, 2, 4]         [6]         [8, 7]
       ^  pivot=4                       ^ pivot=7
[3, 1, 2] [4] [5]       [6]        [7] [8]
    ^  pivot=2
[1] [2] [3] [4] [5]     [6]        [7] [8]

Concatenate: [1, 2, 3, 4, 5, 6, 7, 8]
```

**The partition operation** (Lomuto scheme):

```
Partitioning [5, 3, 8, 1, 2, 7, 4, 6] with pivot = 6:

i = -1 (tracks boundary of "≤ pivot" section)

j=0: arr[0]=5 ≤ 6 → i=0, swap arr[0]↔arr[0]  [5, 3, 8, 1, 2, 7, 4, 6]
j=1: arr[1]=3 ≤ 6 → i=1, swap arr[1]↔arr[1]  [5, 3, 8, 1, 2, 7, 4, 6]
j=2: arr[2]=8 > 6 → skip                       [5, 3, 8, 1, 2, 7, 4, 6]
j=3: arr[3]=1 ≤ 6 → i=2, swap arr[2]↔arr[3]  [5, 3, 1, 8, 2, 7, 4, 6]
j=4: arr[4]=2 ≤ 6 → i=3, swap arr[3]↔arr[4]  [5, 3, 1, 2, 8, 7, 4, 6]
j=5: arr[5]=7 > 6 → skip                       [5, 3, 1, 2, 8, 7, 4, 6]
j=6: arr[6]=4 ≤ 6 → i=4, swap arr[4]↔arr[6]  [5, 3, 1, 2, 4, 7, 8, 6]

Final: swap pivot into position: arr[5]↔arr[7]
       [5, 3, 1, 2, 4, 6, 8, 7]
                        ^ pivot in its final position (index 5)
```

- **Average case**: O(n log n)
- **Worst case**: O(n^2) — when pivot is always min or max (already sorted!)
- **Not stable**: Partition swaps break relative order
- **In-place**: O(log n) stack space (no extra array)
- **Great for**: general-purpose sorting (fast in practice due to cache locality)

### Why Quicksort Often Beats Mergesort in Practice

Both are O(n log n), but quicksort is usually faster in practice. Why?

1. **Cache friendliness**: Quicksort partitions in-place, accessing memory
   sequentially. Mergesort copies to auxiliary arrays, jumping between
   memory locations — more cache misses.

2. **Less memory**: Quicksort sorts in-place (O(log n) stack space).
   Mergesort needs O(n) extra memory for the merge step.

3. **Smaller constants**: Quicksort's inner loop is tighter (fewer
   comparisons and swaps per element on average).

Mergesort has one advantage: guaranteed O(n log n) worst case. Quicksort's
worst case is O(n^2) — but this almost never happens with random pivot
selection.

---

## Part 4: Stability

A sort is **stable** if elements with equal keys keep their original relative
order.

```
Sorting students by grade (stable):
  Input:  [("Alice", B), ("Bob", A), ("Charlie", B), ("Diana", A)]
  Output: [("Bob", A), ("Diana", A), ("Alice", B), ("Charlie", B)]
                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                       Alice still before Charlie (stable!)

Unstable sort might produce:
  Output: [("Diana", A), ("Bob", A), ("Charlie", B), ("Alice", B)]
                                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                       Charlie before Alice — order changed!
```

| Algorithm      | Stable? | Why                                          |
|----------------|---------|----------------------------------------------|
| Bubble sort    | Yes     | Only swaps adjacent elements                 |
| Selection sort | No      | Long-range swap can reorder equals           |
| Insertion sort | Yes     | Slides elements, never jumps over equals     |
| Merge sort     | Yes     | Merge takes from left first on ties          |
| Quicksort      | No      | Partition moves elements across pivot         |

---

## Part 5: Big-O Deeper Dive

You have seen O(n), O(n^2), O(n log n). Let us be more precise.

### Three Notations

| Notation   | Meaning                  | Analogy              |
|------------|--------------------------|----------------------|
| O(f(n))    | At most f(n) (upper bound) | "No worse than"   |
| Omega(f(n))  | At least f(n) (lower bound) | "No better than" |
| Theta(f(n))  | Exactly f(n) (tight bound) | "Grows like"     |

**Example**: Insertion sort
- Best case: Omega(n) — already sorted, one pass
- Worst case: O(n^2) — reverse sorted
- We say: "Insertion sort is O(n^2)" meaning it is **never worse** than n^2
- We say: "Insertion sort is Omega(n)" meaning it **always does at least** n work

If best and worst are the same, we use Theta:
- Merge sort is Theta(n log n) — always n log n regardless of input

### Comparison Table

| Algorithm      | Best       | Average    | Worst      | Space  |
|----------------|------------|------------|------------|--------|
| Bubble sort    | O(n)       | O(n^2)     | O(n^2)     | O(1)   |
| Selection sort | O(n^2)     | O(n^2)     | O(n^2)     | O(1)   |
| Insertion sort | O(n)       | O(n^2)     | O(n^2)     | O(1)   |
| Merge sort     | O(n log n) | O(n log n) | O(n log n) | O(n)   |
| Quicksort      | O(n log n) | O(n log n) | O(n^2)     | O(log n) |

### The Comparison-Based Lower Bound

Here is a beautiful theoretical result: **no comparison-based sort can do
better than O(n log n) in the worst case.**

Why? A comparison sort works by asking "is a < b?" and branching. This forms
a binary decision tree. The tree must have at least n! leaves (one for each
possible permutation of the input). A binary tree with n! leaves has height
at least log2(n!).

By Stirling's approximation: log2(n!) ≈ n log2(n)

So any comparison sort must make at least n log n comparisons in the worst
case. Merge sort achieves this — it is **optimal**.

```
Decision tree for sorting 3 elements [a, b, c]:

                    a < b?
                   /      \
               yes/        \no
              b < c?      a < c?
             /    \       /    \
          yes/   no\   yes/   no\
        [a,b,c] a<c? [b,a,c] b<c?
                / \          / \
            [a,c,b][c,a,b] [b,c,a][c,b,a]

3! = 6 leaves → height = 3 = ceil(log2(6))
At least 3 comparisons needed to sort 3 elements.
```

---

## Exercises

1. **exercises/sorting.c** — Implement all five sorts with comparison counters
2. **exercises/binary_search.c** — Binary search and its variations

---

[← Previous: Module 13 — Hash Tables](../13-hash-tables/README.md) | [Next: Module 15 — Trees & Heaps →](../15-trees-heaps/README.md)
