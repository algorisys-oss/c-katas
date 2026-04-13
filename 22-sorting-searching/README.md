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

## Part 5: Two-Pointer Technique

Now that you can sort and search, let's look at **algorithmic patterns** that
solve entire families of problems. The two-pointer technique is one of the
most useful.

### The Pattern

Use two indices (pointers) that move through an array or string in a
coordinated way. There are three main flavors:

**1. Opposite ends (converging)**:

```
Array: [ 1 | 3 | 5 | 7 | 11 ]
         ^                 ^
        left             right
        ──→             ←──

Move left right or right left based on a condition.
Stop when left >= right.
```

**2. Same direction (fast/slow)**:

```
Array: [ 1 | 1 | 2 | 2 | 3 ]
         ^   ^
       slow fast  ──→

slow = write position, fast = read position.
fast scans ahead; slow only advances when we find something useful.
```

**3. Separate arrays**: one pointer per array, both moving forward.

### Why It Works

The key insight: a naive approach uses nested loops to check all pairs —
O(n^2). Two pointers exploit **structure** (usually sorted order) to
eliminate impossible pairs without checking them.

For the sorted two-sum problem:

```
Find two numbers that add to 10 in [1, 3, 5, 7, 11]:

  Step 1: left=0, right=4 → 1 + 11 = 12 > 10
          ▲ Sum too big → move right left (decrease the larger value)

  Step 2: left=0, right=3 → 1 + 7  = 8  < 10
          ▲ Sum too small → move left right (increase the smaller value)

  Step 3: left=1, right=3 → 3 + 7  = 10 ✓ Found!
```

Why can we skip all other pairs? Because if `arr[left] + arr[right] > target`,
then `arr[left+1] + arr[right]` would be even bigger (since the array is
sorted). So we can safely move `right` left. This eliminates an entire row
of the pair matrix in one step.

### Container With Most Water

A classic two-pointer problem. Given heights, find the two lines that hold
the most water:

```
     8 |    #              #
     7 |    #  .  .  .  .  #  .  #
     6 |    #  #  .  .  .  #  .  #
     5 |    #  #  .  #  .  #  .  #
     4 |    #  #  .  #  #  #  .  #
     3 |    #  #  .  #  #  #  #  #
     2 |    #  #  #  #  #  #  #  #
     1 | #  #  #  #  #  #  #  #  #
       +----------------------------
         0  1  2  3  4  5  6  7  8

Water = min(height[L], height[R]) * (R - L)
```

**Greedy insight**: always move the pointer with the **shorter** height.
Moving the taller side can only make things worse — the width shrinks, and
the limiting height (the shorter side) stays the same or could only
decrease from the other side.

---

## Part 6: Sliding Window Technique

The sliding window is a pattern for computing something over **contiguous
subarrays** (or substrings) efficiently.

### Fixed-Size Window

When looking at all subarrays of exactly size k, don't recompute from
scratch. Slide the window by removing the element that leaves and adding
the element that enters:

```
Array: [ 2 | 1 | 5 | 1 | 3 | 2 ]     k = 3

Window 1: [ 2   1   5 ]  sum = 8
           ───────────
Window 2:     [ 1   5   1 ]  sum = 8 - 2 + 1 = 7
               ───────────
Window 3:         [ 5   1   3 ]  sum = 7 - 1 + 3 = 9  ← max!
                   ───────────
Window 4:             [ 1   3   2 ]  sum = 9 - 5 + 2 = 6
                       ───────────

Each slide: O(1) update instead of O(k) recomputation.
Total: O(n) instead of O(n*k).
```

### Variable-Size Window

When the window size isn't fixed, we **expand** the right side until some
condition breaks, then **shrink** from the left until the condition is
restored:

```
Longest substring without repeats in "abcabcbb":

freq[128] = {0}  (tracks character counts in window)

    a b c a b c b b
    ^               right expands →
    left

Step 1: "a"     freq[a]=1  OK, len=1
Step 2: "ab"    freq[b]=1  OK, len=2
Step 3: "abc"   freq[c]=1  OK, len=3
Step 4: "abca"  freq[a]=2  DUPLICATE! Shrink from left:
        "bca"   freq[a]=1  OK, len=3
Step 5: "bcab"  freq[b]=2  DUPLICATE! Shrink:
        "cab"   freq[b]=1  OK, len=3
Step 6: "cabc"  freq[c]=2  DUPLICATE! Shrink:
        "abc"   freq[c]=1  OK, len=3
Step 7: "abcb"  freq[b]=2  DUPLICATE! Shrink:
        "bcb"   freq[b]=2  Still! Shrink more:
        "cb"    freq[b]=1  OK, len=2
Step 8: "cbb"   freq[b]=2  DUPLICATE! Shrink:
        "bb"    freq[b]=2  Still! Shrink:
        "b"     freq[b]=1  OK, len=1

Maximum length seen = 3 ("abc" or "bca" or "cab" or "wke")
```

The pattern works because both left and right only move forward. Each
element is added at most once and removed at most once, so the total
work is O(n) even though the inner while loop might run multiple times
on some steps.

---

## Part 7: Merge Intervals

Interval problems appear in scheduling, IP range management, and many
other domains. The universal strategy: **sort first, then linear scan**.

### The Algorithm

1. Sort intervals by start time
2. Scan left to right, merging as you go

The overlap condition: `current.start <= previous.end`

```
Before merging:

  Interval:  [1,3]  [2,6]            [8,10]          [15,18]
             |---|
               |--------|
  Number line: 1  2  3  4  5  6  7  8  9  10 ... 15  16  17  18
                                      |---|
                                                       |---|

After sorting by start:  [1,3], [2,6], [8,10], [15,18]

Scan:
  result = [[1,3]]
  [2,6]: 2 <= 3? YES → merge → result = [[1,6]]
  [8,10]: 8 <= 6? NO → add → result = [[1,6],[8,10]]
  [15,18]: 15 <= 10? NO → add → result = [[1,6],[8,10],[15,18]]
```

### Insert Interval

When inserting a new interval into an already sorted, non-overlapping list:

1. **Before phase**: copy all intervals that end before the new one starts
2. **Overlap phase**: merge all intervals that overlap with the new one
3. **After phase**: copy all remaining intervals

This is a clean three-phase linear scan — no re-sorting needed.

---

## Part 8: String Problems

Strings in C are just arrays of characters with a null terminator. Many
string problems use the same patterns we've already learned:

### The Reverse-Twice Trick

To reverse the **words** in a string (not the characters):
1. Reverse the entire string
2. Reverse each individual word

```
"the sky is blue"
  → reverse all: "eulb si yks eht"
  → reverse each word: "blue is sky the"
```

This is a two-pointer technique in disguise — each reversal uses
converging left/right pointers.

### Frequency Arrays for Character Counting

Use a fixed-size array to count character occurrences:

```c
int seen[26] = {0};  /* One slot per letter */

For "The quick brown fox...":
  seen[0] = 1  (a)
  seen[1] = 1  (b)
  ...
  seen[25] = 1 (z)
  All 26 set → pangram!
```

This is O(n) time, O(1) space — the array size is constant regardless
of input length.

### Parsing and Validation

When validating structured strings (like IPv6 addresses), walk through
character by character:
- Track your position within the expected structure
- Count separators and group lengths
- Validate each character against the allowed set

---

## Part 9: Modified Binary Search

The standard binary search assumes a sorted array and looks for an exact
match. But the **binary search pattern** — cutting the search space in
half — applies much more broadly.

### The Core Invariant

At every step, you must know: **which half contains the answer?**

The standard version checks `arr[mid] vs target`. Modified versions check
different conditions, but the structure is the same:

```
while (lo < hi) {
    mid = lo + (hi - lo) / 2;
    if (go_right_condition) {
        lo = mid + 1;
    } else {
        hi = mid;
    }
}
```

### Rotated Array Search

A sorted array rotated at some unknown pivot:

```
Original: [0, 1, 2, 4, 5, 6, 7]

Rotated:  [4, 5, 6, 7, 0, 1, 2]
           ─────────  ────────
           sorted↑     sorted↑
                    ^
               rotation point

Key: when you pick mid, ONE half is always sorted.
     Check if target falls in the sorted half's range.
     If yes → search there. If no → search the other half.
```

### Peak Finding

Find a local maximum using binary search:

```
Values:    12
          /  \
        8      4
       /        \
     3            2
    /
  1

If arr[mid] < arr[mid+1]: peak is to the RIGHT
   (values are still increasing)
If arr[mid] > arr[mid+1]: peak is to the LEFT (or at mid)
   (values are decreasing, so we've passed the peak or we're on it)
```

This is "gradient-based" binary search — we follow the slope uphill.

### 2D Matrix Search

A row-sorted matrix where each row starts after the previous row ends can
be treated as a flat sorted array:

```
Matrix:               Flat view:
┌────┬────┬────┬────┐
│  1 │  3 │  5 │  7 │  [1, 3, 5, 7, 10, 11, 16, 20, 23, 30, 34, 50]
├────┼────┼────┼────┤   0  1  2  3   4   5   6   7   8   9  10  11
│ 10 │ 11 │ 16 │ 20 │
├────┼────┼────┼────┤  flat_index 7 → row = 7/4 = 1, col = 7%4 = 3
│ 23 │ 30 │ 34 │ 50 │              → matrix[1][3] = 20
└────┴────┴────┴────┘
```

### First/Last with Duplicates

Two binary searches with different "bias":

```
Array: [1, 3, 5, 5, 5, 5, 5, 8, 9]
                ^           ^
             first(5)=2  last(5)=6

Leftmost:  when you find target, record it and search LEFT (hi = mid)
Rightmost: when you find target, record it and search RIGHT (lo = mid + 1)
```

---

## Part 10: Greedy Algorithms

A **greedy algorithm** makes the choice that looks best RIGHT NOW at each
step, without worrying about the future. Sometimes this gives the optimal
answer. Sometimes it does not (and you need dynamic programming instead).

### What Makes an Algorithm "Greedy"

The problem must have two properties:

1. **Greedy choice property** -- a locally optimal choice leads to a globally
   optimal solution. Taking the best option now never prevents you from
   reaching the best overall answer.

2. **Optimal substructure** -- the optimal solution contains optimal solutions
   to its subproblems.

### Greedy vs Dynamic Programming

```
Greedy: "Take the best NOW"     → Activity selection, Jump game
DP:     "Try ALL options, pick best" → Knapsack, Edit distance

Rule of thumb: if the problem says "maximum number" or "minimum number"
and making a locally best choice never hurts the future → try greedy.
If choices interact (taking item A affects what items B,C are worth) → use DP.
```

### Activity Selection

Given activities with start/end times, pick the maximum number of
non-overlapping activities. **Greedy: always pick the activity that ends
earliest** -- it leaves the most room for future activities.

```
Timeline:
0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
|  [====]                                            (1,4)  *
|     [====]                                         (3,5)
|  [=========]                                       (0,6)
|              [==]                                  (5,7)  *
|     [==========]                                   (3,9)
|              [======]                              (5,9)
|                 [======]                           (6,10)
|                       [====]                       (8,11) *
|                       [======]                     (8,12)
|  [========================]                        (2,14)
|                                   [======]         (12,16)*

Greedy picks: (1,4), (5,7), (8,11), (12,16) = 4 activities
```

Why does "earliest end time" work? By finishing as early as possible, we
leave the maximum remaining time for future activities. Any other choice
either finishes later (leaving less room) or skips an option that finishes
sooner.

### Jump Game

Given an array where `arr[i]` = max jump length from position i, can you
reach the last index? **Greedy: track the farthest reachable position.**

```
arr = [2, 3, 1, 1, 4]

Index:    0    1    2    3    4
Value:    2    3    1    1    4
Reach:    2    4    3    4    8
          ^    ^              ^
          |    farthest jumps to 4
          starts at 2         goal reached!

arr = [3, 2, 1, 0, 4]

Index:    0    1    2    3    4
Value:    3    2    1    0    4
Reach:    3    3    3    3    -
                         ^
                         stuck! can't pass index 3
```

### Task Scheduler

Given tasks with a cooldown period, find the minimum time to execute all.
**Greedy: the most frequent task determines the frame size.**

```
Tasks: [A,A,A,B,B,B], cooldown n=2

Slot:   1    2    3    4    5    6    7    8
Task:   A    B   idle   A    B   idle   A    B

Frames:  [A B _] [A B _] [A B]
          frame1   frame2   final

Formula: (max_freq - 1) * (n + 1) + count_of_max_freq_tasks
       = (3 - 1) * (2 + 1) + 2 = 8
```

### Gas Station

N stations in a circle. Fuel balance at each station:

```
Station:   0     1     2     3     4
Gas:       1     2     3     4     5
Cost:      3     4     5     1     2
Balance:  -2    -2    -2    +3    +3

Total balance = 0 → solution exists!

Key insight: if you run out traveling from station i to j,
then no station between i and j can be the starting point.
Skip directly to j+1.

Start at 3: tank = 0
  Station 3: +4 -1 = 3
  Station 4: +5 -2 = 6
  Station 0: +1 -3 = 4
  Station 1: +2 -4 = 2
  Station 2: +3 -5 = 0  ✓ made it!
```

---

## Matrix Patterns

Classic 2D array manipulation problems that appear constantly in interviews.
The key insight for most matrix problems: find a clever traversal order
or an in-place transformation trick.

### Spiral Traversal — The Four-Boundary Approach

Walk around the matrix in a spiral using four boundaries that shrink inward:

```
    top=0
    ┌──────────────────────┐
    │  1 → 2 → 3 → 4 → 5  │  Sweep right along top row, then top++
    │                   ↓  │
    │  16  17  18  19   6  │  Sweep down along right col, then right--
    │  ↑               ↓  │
    │  15  24  25  20   7  │
    │  ↑               ↓  │
    │  14  23  22  21   8  │  Sweep left along bottom row, then bottom--
    │  ↑                   │
    │  13 ← 12 ← 11 ← 10 ← 9  │  Sweep up along left col, then left++
    └──────────────────────┘
    left=0             right=4
                       bottom=4

    Repeat until boundaries cross.
```

### Rotate 90 — The Transpose-Then-Reverse Trick

Why does transpose + reverse-rows = 90-degree clockwise rotation?

```
    Original:       Transpose:         Reverse rows:
    1 2 3           1 4 7              7 4 1
    4 5 6     →     2 5 8       →      8 5 2
    7 8 9           3 6 9              9 6 3

    Transpose mirrors across the main diagonal (\).
    Reversing rows mirrors left-to-right (|).
    Composing \ then | = 90° clockwise rotation!
```

Geometrically: reflecting across the diagonal swaps (row, col) positions,
then reflecting horizontally puts everything in the right spot for a
clockwise rotation. Two reflections whose axes are 45° apart always
produce a 90° rotation.

### Set Zeroes — Using First Row/Column as Markers

The naive approach uses O(m+n) extra space to track which rows/columns
need zeroing. The clever approach uses the first row and first column
of the matrix itself as markers — O(1) extra space.

```
    Step 1: Save whether first row/col have zeros (two booleans)
    Step 2: Scan rest of matrix — if [i][j]==0, mark [i][0]=0 and [0][j]=0
    Step 3: Use markers to zero out rows and columns
    Step 4: Zero first row/col if needed (using saved booleans)

    Original:        After marking:      After zeroing:
    0  1  2  0       0  0  2  0          0  0  0  0
    3  4  5  2   →   0  4  5  2    →     0  4  5  0
    1  3  1  5       1  3  1  5          0  3  1  0
```

### Sorted Matrix Search — The Staircase from Top-Right

When every row and every column is sorted, start at the **top-right corner**.
Each comparison eliminates an entire row or column:

```
    1   4   7  [11]     11 > 5 → go LEFT (eliminate column)
    2   5   8   12
    3   6   9   16
   10  13  14   17

    1   4  [7]  11      7 > 5 → go LEFT
    2   5   8   12
    3   6   9   16
   10  13  14   17

    1  [4]  7   11      4 < 5 → go DOWN (eliminate row)
    2   5   8   12
    3   6   9   16
   10  13  14   17

    1   4   7   11
    2  [5]  8   12      5 == 5 → FOUND at (1,1)
    3   6   9   16
   10  13  14   17

    Time: O(m + n) — at most m + n steps
```

---

## String Matching Algorithms

Given a text of length n and a pattern of length m, find where the pattern
occurs in the text. This is one of the most fundamental problems in CS.

### Brute Force — Why O(n * m) Hurts

For each position in the text, compare every character of the pattern:

```
    Text:    "AAAAAAAAB"
    Pattern: "AAAB"

    Position 0: AAAA vs AAAB → match,match,match,FAIL (3 wasted comparisons!)
    Position 1:  AAAA vs AAAB → match,match,match,FAIL
    Position 2:   AAAA vs AAAB → match,match,match,FAIL
    ...
    Position 5:      AAAB vs AAAB → MATCH!

    Each attempt does m comparisons before failing. That's O(n * m).
```

### KMP — The Prefix Function Insight

The key insight: when a mismatch occurs, we've already matched some characters.
If the matched portion has a **prefix that's also a suffix**, we can skip ahead
instead of starting over.

**Building the prefix table:**

```
    Pattern: "ABABAC"
    Index:    0 1 2 3 4 5

    j=0: "A"      → no proper prefix = suffix           → table[0] = 0
    j=1: "AB"     → "A" ≠ "B"                           → table[1] = 0
    j=2: "ABA"    → prefix "A" == suffix "A"             → table[2] = 1
    j=3: "ABAB"   → prefix "AB" == suffix "AB"           → table[3] = 2
    j=4: "ABABA"  → prefix "ABA" == suffix "ABA"         → table[4] = 3
    j=5: "ABABAC" → no prefix matches suffix ending in C → table[5] = 0

    Result: [0, 0, 1, 2, 3, 0]
```

**Using the table for search:**

```
    Text:    "ABABABACABC"
    Pattern: "ABABAC"
    Table:   [0, 0, 1, 2, 3, 0]

    i=0: A==A, i=1: B==B, i=2: A==A, i=3: B==B, i=4: A==A
    i=5: B≠C → mismatch at j=5, table[4]=3, so j jumps to 3
         (we know "ABA" already matches — don't re-check it!)
    i=5: B==B, i=6: A==A, i=7: C==C → j==6 → MATCH at position 2!
```

**Why O(n + m):** each character of the text is visited at most twice (once
by advancing i, once by the prefix table fallback). We never go backwards
in the text!

### Rabin-Karp — The Rolling Hash Trick

Instead of comparing characters, compare **hashes**. If the hash doesn't
match, the strings definitely don't match (skip immediately). If the hash
does match, verify character by character (hash collisions are possible).

```
    Hash function: hash = (c0 * 256^(m-1) + c1 * 256^(m-2) + ... + c_{m-1}) % prime

    Rolling update when sliding from "ABC" to "BCD":

    hash("BCD") = (hash("ABC") - 'A' * 256^2) * 256 + 'D') % prime
                   ─────────────────────────   ─────   ───
                   remove leftmost character    shift   add new character

    This update is O(1) — no need to rehash the entire window!
```

**Spurious hits:** When hashes match but strings don't (hash collision),
we must verify with character comparison. With a good prime, this is rare,
giving O(n + m) average time.

### Algorithm Comparison

| Algorithm | Time | Space | Best For |
|-----------|------|-------|----------|
| Brute force | O(nm) | O(1) | Short patterns |
| KMP | O(n+m) | O(m) | Single pattern, guaranteed |
| Rabin-Karp | O(n+m) avg | O(1) | Multiple patterns |

---

## Exercises

| # | File | Topic | Tests |
|---|------|-------|-------|
| 1 | **exercises/sorting.c** | Five sorting algorithms with comparison counters | 15 |
| 2 | **exercises/binary_search.c** | Binary search and its variations | 13 |
| 3 | **exercises/two_pointers.c** | Two-pointer technique (converging and fast/slow) | 16 |
| 4 | **exercises/sliding_window.c** | Fixed and variable sliding windows | 13 |
| 5 | **exercises/merge_intervals.c** | Sort, merge, and insert intervals | 11 |
| 6 | **exercises/string_problems.c** | Reverse words, pangrams, IPv6 validation | 14 |
| 7 | **exercises/modified_binary_search.c** | Rotated array, peak, 2D matrix, first/last | 17 |
| 8 | **exercises/greedy.c** | Activity selection, jump game, task scheduler, gas station | 19 |
| 9 | **exercises/matrix_patterns.c** | Spiral order, rotate, set zeroes, sorted search, transpose | 17 |
| 10 | **exercises/string_matching.c** | Brute force, KMP, Rabin-Karp string matching | 14 |

**Total: 149 tests across 10 exercise files.**

---

[← Previous: Module 13 — Hash Tables](../21-hash-tables/README.md) | [Next: Module 15 — Trees & Heaps →](../23-trees-heaps/README.md)
