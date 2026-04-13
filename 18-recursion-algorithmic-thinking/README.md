# Module 05 — Recursion & Algorithmic Thinking

You have written loops to repeat things. But there is another way to repeat:
**a function can call itself**. This is called **recursion**, and it is one of
the most powerful ideas in all of computer science.

By the end of this module you will be able to:
- Write recursive functions with correct base cases and recursive cases
- Visualize recursion on the call stack
- Understand why some recursive solutions are slow (and how to fix them)
- Analyze algorithm efficiency using Big-O notation
- Apply the divide-and-conquer strategy to solve problems

---

## 1. What Is Recursion?

A recursive function is a function that **calls itself**. That is the whole
definition. But to be useful, every recursive function needs two things:

1. **Base case** — a condition where the function stops calling itself and
   returns a value directly. Without this, the function would call itself
   forever.

2. **Recursive case** — the function calls itself with a **smaller** or
   **simpler** input, moving closer to the base case.

Here is the simplest example: factorial.

```
    n! = n * (n-1) * (n-2) * ... * 2 * 1
    0! = 1   (by definition)
```

In C:

```c
int factorial(int n)
{
    if (n == 0) {          /* base case */
        return 1;
    }
    return n * factorial(n - 1);   /* recursive case */
}
```

Read it aloud: "the factorial of n is n times the factorial of (n minus 1), and
the factorial of 0 is 1." That is all recursion is: defining something in terms
of a smaller version of itself, with a stopping point.

---

## 2. How Recursion Works on the Call Stack

Every time you call a function, the computer **pushes a frame** onto the call
stack. That frame holds the function's local variables, its arguments, and where
to return when it finishes. When the function returns, its frame is **popped**
off the stack.

Recursive calls are no different — each call gets its own frame. Let's trace
`factorial(4)`:

### Call Stack During factorial(4)

```
PHASE 1: PUSHING FRAMES (calling deeper)
==========================================

Call: factorial(4)              Call: factorial(3)

+---------------------+        +---------------------+
| factorial(4)        |        | factorial(3)        |
|   n = 4             |        |   n = 3             |
|   return 4 * ???    |        |   return 3 * ???    |
+---------------------+        +---------------------+
| main()              |        | factorial(4)        |
|   ...               |        |   n = 4             |
+---------------------+        |   return 4 * ???    |
                                +---------------------+
                                | main()              |
                                +---------------------+

Call: factorial(2)              Call: factorial(1)

+---------------------+        +---------------------+
| factorial(2)        |        | factorial(1)        |
|   n = 2             |        |   n = 1             |
|   return 2 * ???    |        |   return 1 * ???    |
+---------------------+        +---------------------+
| factorial(3)        |        | factorial(2)        |
|   n = 3             |        |   n = 2             |
|   return 3 * ???    |        |   return 2 * ???    |
+---------------------+        +---------------------+
| factorial(4)        |        | factorial(3)        |
|   n = 4             |        |   n = 3             |
|   return 4 * ???    |        |   return 3 * ???    |
+---------------------+        +---------------------+
| main()              |        | factorial(4)        |
+---------------------+        |   n = 4             |
                                +---------------------+
                                | main()              |
                                +---------------------+

Call: factorial(0)   <-- BASE CASE!

+---------------------+
| factorial(0)        |
|   n = 0             |
|   return 1          |  <-- does NOT call itself again
+---------------------+
| factorial(1)        |
|   n = 1             |
+---------------------+
| factorial(2)        |
|   n = 2             |
+---------------------+
| factorial(3)        |
|   n = 3             |
+---------------------+
| factorial(4)        |
|   n = 4             |
+---------------------+
| main()              |
+---------------------+

PHASE 2: POPPING FRAMES (returning results)
=============================================

factorial(0) returns 1
  -> factorial(1) now knows: return 1 * 1 = 1

+---------------------+
| factorial(1)        |
|   n = 1             |
|   return 1 * 1 = 1  |
+---------------------+
| factorial(2)        |
| factorial(3)        |
| factorial(4)        |
| main()              |
+---------------------+

factorial(1) returns 1
  -> factorial(2) now knows: return 2 * 1 = 2

factorial(2) returns 2
  -> factorial(3) now knows: return 3 * 2 = 6

factorial(3) returns 6
  -> factorial(4) now knows: return 4 * 6 = 24

factorial(4) returns 24
  -> main() receives 24


SUMMARY:
  factorial(4) = 4 * factorial(3)
               = 4 * 3 * factorial(2)
               = 4 * 3 * 2 * factorial(1)
               = 4 * 3 * 2 * 1 * factorial(0)
               = 4 * 3 * 2 * 1 * 1
               = 24
```

**Key insight**: the stack grows by one frame per recursive call, and shrinks
as each call returns. For `factorial(n)`, the stack reaches a depth of n+1
frames.

---

## 3. Stack Overflow

What happens if you forget the base case?

```c
int oops(int n)
{
    return n * oops(n - 1);   /* never stops! */
}
```

Each call pushes a new frame. The stack has a limited size (typically 1-8 MB).
When you run out of stack space: **stack overflow**. The program crashes with a
segmentation fault.

This is why the base case is not optional — it is what prevents infinite
recursion.

---

## 4. Recursion vs. Iteration

Every recursive function **can** be rewritten as a loop, and every loop **can**
be rewritten as recursion. They are equally powerful.

Compare factorial both ways:

```c
/* Recursive */                    /* Iterative */
int fact_r(int n)                  int fact_i(int n)
{                                  {
    if (n == 0) return 1;              int result = 1;
    return n * fact_r(n - 1);          for (int i = 1; i <= n; i++) {
}                                          result *= i;
                                       }
                                       return result;
                                   }
```

When to use which?

| Aspect         | Recursion                    | Iteration                  |
|----------------|------------------------------|----------------------------|
| Clarity        | Natural for trees, divide &  | Natural for simple counting|
|                | conquer, mathematical defs   | and sequential processing  |
| Stack usage    | One frame per call (risky    | Constant stack space       |
|                | for deep recursion)          |                            |
| Performance    | Function call overhead       | Usually faster             |
| Debugging      | Harder to trace              | Easier to trace            |

Rule of thumb: if the problem has a **recursive structure** (like trees, or
splitting a problem in half), recursion is natural. If you are just counting
from A to B, use a loop.

---

## 5. Big-O Notation — From First Principles

Before we look at fibonacci, we need a way to talk about how fast (or slow) an
algorithm is. That tool is **Big-O notation**.

### The Core Question

> "As the input gets bigger, how does the amount of work grow?"

We do not care about the exact number of steps. We care about the **growth
rate**. If doubling the input doubles the work, that is different from doubling
the input quadrupling the work.

### Why We Ignore Constants

Suppose algorithm A does `3n + 5` operations and algorithm B does `100n + 2`
operations. Both grow **linearly** — double the input, roughly double the work.
The constants (3, 5, 100, 2) do not change the *shape* of the growth. So we say
both are **O(n)**.

Big-O keeps only the **dominant term** and drops constants:
- `3n + 5` becomes O(n)
- `2n^2 + 10n + 7` becomes O(n^2) — the n^2 dwarfs everything else for large n
- `5` becomes O(1) — constant, no matter the input

### Counting Operations

To find the Big-O of a function, count how many times the core operation runs
relative to the input size n:

```c
/* O(1) — constant */
int first_element(int arr[], int n)
{
    return arr[0];     /* always 1 operation, regardless of n */
}

/* O(n) — linear */
int sum(int arr[], int n)
{
    int total = 0;
    for (int i = 0; i < n; i++) {   /* loop runs n times */
        total += arr[i];
    }
    return total;
}

/* O(n^2) — quadratic */
void bubble_sort(int arr[], int n)
{
    for (int i = 0; i < n; i++) {         /* n times */
        for (int j = 0; j < n - 1; j++) { /* n times each */
            /* ... */                      /* n * n = n^2 total */
        }
    }
}
```

### Common Complexity Classes

```
+------------+--------------------+-------------------------------------------+
| Big-O      | Name               | Example                                   |
+------------+--------------------+-------------------------------------------+
| O(1)       | Constant           | Array index lookup, checking if even      |
| O(log n)   | Logarithmic        | Binary search, fast power                 |
| O(n)       | Linear             | Linear search, sum of array               |
| O(n log n) | Linearithmic       | Merge sort (covered later)                |
| O(n^2)     | Quadratic          | Bubble sort, nested loops over same data  |
| O(2^n)     | Exponential        | Naive recursive fibonacci                 |
| O(n!)      | Factorial           | Generating all permutations of a list     |
+------------+--------------------+-------------------------------------------+
```

### Growth Comparison Table

How the number of operations grows with input size:

```
+--------+---------+----------+---------+-----------+----------+-----------+
|  n     |  O(1)   | O(log n) |  O(n)   | O(n log n)|  O(n^2)  |  O(2^n)   |
+--------+---------+----------+---------+-----------+----------+-----------+
|     10 |       1 |        3 |      10 |        33 |      100 |     1,024 |
|    100 |       1 |        7 |     100 |       664 |   10,000 |  1.27e+30 |
|  1,000 |       1 |       10 |   1,000 |     9,966 |1,000,000 | TOO BIG   |
+--------+---------+----------+---------+-----------+----------+-----------+

Note: O(2^n) for n=100 is already 1,267,650,600,228,229,401,496,703,205,376.
      For n=1000, it has over 300 digits. This is why exponential is terrifying.
```

The lesson: an O(n^2) algorithm might be fine for 100 items, but an O(2^n)
algorithm is unusable for anything beyond about 40-50 items. **The growth rate
matters more than the constant**.

---

## 6. Fibonacci and Why Naive Recursion Is Terrible

The Fibonacci sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...

Each number is the sum of the previous two:

```
    fib(0) = 0
    fib(1) = 1
    fib(n) = fib(n-1) + fib(n-2)   for n >= 2
```

The recursive code writes itself:

```c
int fib(int n)
{
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
```

Beautiful. Simple. **Horribly slow.**

### The Call Tree for fib(5)

Draw out every call. Watch the duplication:

```
                              fib(5)
                            /        \
                       fib(4)          fib(3)
                      /      \         /     \
                 fib(3)     fib(2)  fib(2)  fib(1)
                /     \     /    \   /    \     |
           fib(2) fib(1) fib(1) fib(0) fib(1) fib(0)  1
           /    \    |     |      |      |      |
       fib(1) fib(0) 1     1      0      1      0
          |      |
          1      0

Total calls: 15
```

**Look at the duplication:**

```
    fib(3) is computed 2 times    ***
    fib(2) is computed 3 times    ***
    fib(1) is computed 5 times    ***
    fib(0) is computed 3 times    ***
```

For fib(5), we make 15 calls. For fib(10), it is 177 calls. For fib(30), it is
over 2.6 million calls. For fib(50), it is over 40 billion calls.

The number of calls roughly **doubles** every time n increases by 1. That is
O(2^n) — exponential growth.

### The Fix: Iterative Fibonacci

Instead of recomputing everything, just walk forward and remember the last two
values:

```c
int fib_iterative(int n)
{
    if (n <= 1) return n;
    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
```

This is **O(n)** — one loop, n steps, done. The difference is staggering:

```
    n = 30:  Recursive ~2,692,537 calls   vs.  Iterative 29 loop iterations
    n = 50:  Recursive ~40 BILLION calls  vs.  Iterative 49 loop iterations
```

### Memoization: Caching + Recursion (Top-Down DP)

There is a third approach: keep the recursive structure, but **remember** results
you have already computed. This is called **memoization** (not memorization — it
comes from "memo," as in writing yourself a note).

The idea is dead simple. Before computing fib(n):
1. Check if you already have the answer in a cache array
2. If yes, return it immediately — no recursion needed
3. If no, compute it recursively, **store** the result, then return it

Look at the call tree for fib(5) again, but now with memoization. Crossed-out
nodes are cache hits — they return instantly without recursing further:

```
                       fib(5)
                      /      \
                 fib(4)       fib(3) ← CACHE HIT (already computed)
                /     \
            fib(3)   fib(2) ← CACHE HIT
           /     \
       fib(2)   fib(1)
       /    \
   fib(1)  fib(0)
```

Without memoization: fib(5) makes 15 calls.
With memoization: fib(5) makes 9 calls — and for larger n the savings are massive.

Each value fib(k) is computed **exactly once** and then looked up from the cache
every subsequent time. This turns O(2^n) into O(n).

```c
int fib_memo(int n, int cache[]) {
    if (n <= 1) return n;
    if (cache[n] != -1) return cache[n];    /* cache hit! */
    cache[n] = fib_memo(n-1, cache) + fib_memo(n-2, cache);
    return cache[n];
}
```

### Tabulation: Iteration + Table (Bottom-Up DP)

Memoization is "top-down" — you start with the big problem and recurse down.
**Tabulation** is "bottom-up" — you start with the smallest subproblems and build
up to the answer, filling a table iteratively.

For Fibonacci, bottom-up is just the iterative solution you already wrote:
start from fib(0) and fib(1), compute fib(2), then fib(3), and so on up to fib(n).
No recursion, no stack overflow risk, and often faster in practice.

### When Does DP Apply?

Dynamic programming works when a problem has these two properties:

1. **Overlapping subproblems** — the same subproblem appears multiple times.
   Fibonacci has this: fib(3) is needed by both fib(4) and fib(5).

2. **Optimal substructure** — the optimal solution to the big problem contains
   optimal solutions to its subproblems. The best way to compute fib(5) uses
   the best (only) way to compute fib(4) and fib(3).

If a problem has both properties, you can use DP. If subproblems don't overlap
(like in merge sort, where each half is independent), plain divide-and-conquer
is enough — no caching needed.

### Climbing Stairs: A Hidden Fibonacci

Problem: you have n stairs. Each step, you can climb 1 or 2 stairs. How many
distinct ways can you reach the top?

Think about it from the top: to reach step n, you either came from step n-1
(took 1 step) or step n-2 (took 2 steps). So:

```
    ways(n) = ways(n-1) + ways(n-2)
```

That IS the Fibonacci recurrence! With base cases ways(0)=1, ways(1)=1.
The same memoization technique applies directly.

### Coin Change: True DP

Problem: given coin denominations [1, 5, 10, 25], find the minimum number of
coins to make a given amount.

Key insight: if you use a coin of value c, you need **1 + min_coins(amount - c)**
more coins. Try every coin and pick the minimum:

```
    min_coins(amount) = 1 + min( min_coins(amount - c)  for each coin c )
```

This is best solved bottom-up with a table:

```
    amount:  0   1   2   3   4   5   6   7   8   9   10
    coins:   0   1   2   3   4   1   2   3   4   5    1
                                 ↑               ↑    ↑
                              use 5           use 5  use 10
```

Each entry dp[i] = 1 + min(dp[i - c]) for each valid coin c.

### Longest Common Subsequence: 2D DP

Problem: given two strings, find the length of their longest common subsequence
(LCS). A subsequence keeps the order but can skip characters.

Example: LCS of "ABCDE" and "ACE" is "ACE" (length 3).

Build a 2D table dp[i][j] = LCS length of the first i characters of string A
and the first j characters of string B:

```
        ""  A  C  E
    ""   0  0  0  0
    A    0  1  1  1      ← A matches A, so dp[1][1] = dp[0][0] + 1 = 1
    B    0  1  1  1      ← B doesn't match anything new
    C    0  1  2  2      ← C matches C, so dp[3][2] = dp[2][1] + 1 = 2
    D    0  1  2  2
    E    0  1  2  3      ← E matches E, so dp[5][3] = dp[4][2] + 1 = 3
```

Rules:
- If characters match: dp[i][j] = dp[i-1][j-1] + 1
- If they don't match:  dp[i][j] = max(dp[i-1][j], dp[i][j-1])
- Base case: dp[0][j] = dp[i][0] = 0

The answer is in dp[len_a][len_b].

### Big-O Summary

| Problem           | Naive             | With DP    | Space      |
|-------------------|-------------------|------------|------------|
| Fibonacci         | O(2^n)            | O(n)       | O(n)       |
| Climbing stairs   | O(2^n)            | O(n)       | O(n)       |
| Coin change       | O(k^amount)       | O(amount*k)| O(amount)  |
| LCS               | O(2^(n+m))        | O(n*m)     | O(n*m)     |

(k = number of coin denominations, n and m = string lengths)

---

## 7. Divide and Conquer

Divide and conquer is a strategy for solving problems:

1. **Divide** the problem into smaller subproblems
2. **Conquer** each subproblem (recursively)
3. **Combine** the results

```
            +---------------------+
            |   Original Problem  |
            |       size n        |
            +---------------------+
                   /        \
                DIVIDE       DIVIDE
                /                \
    +------------+          +------------+
    | Subproblem |          | Subproblem |
    |   size n/2 |          |   size n/2 |
    +------------+          +------------+
       /      \                /      \
    DIVIDE   DIVIDE        DIVIDE   DIVIDE
     /          \            /          \
  +------+  +------+    +------+  +------+
  | n/4  |  | n/4  |    | n/4  |  | n/4  |
  +------+  +------+    +------+  +------+
     |          |           |          |
   (base)    (base)      (base)    (base)
     |          |           |          |
     v          v           v          v
  +------+  +------+    +------+  +------+
  |result|  |result|    |result|  |result|
  +------+  +------+    +------+  +------+
       \      /                \      /
       COMBINE                 COMBINE
          \                      /
    +------------+          +------------+
    |  Combined  |          |  Combined  |
    +------------+          +------------+
              \                /
              COMBINE
                 |
         +-------------+
         | Final Result|
         +-------------+
```

**Key insight**: by splitting the problem in half at each level, the tree is
only **log n** levels deep. If combining is fast, the whole algorithm can be
O(n log n) — much better than O(n^2).

Examples of divide and conquer:
- **Binary search**: split the search space in half each time -> O(log n)
- **Merge sort**: split the array, sort each half, merge -> O(n log n)
- **Fast power**: x^n = (x^(n/2))^2 -> O(log n)

The power function kata below uses this strategy: instead of multiplying x by
itself n times (O(n)), we square the result and halve the exponent (O(log n)).

---

## 8. Backtracking

Backtracking is the most important recursion pattern for problem-solving. The key
idea: **build a solution incrementally, and if the current path can't lead to a
valid solution, UNDO the last choice and try a different path**.

Think of exploring a maze: go forward until you hit a dead end, then retrace
your steps and try a different turn.

### The Pattern

Every backtracking algorithm follows this template:

```
backtrack(choices_made_so_far):
    if we have a complete solution:
        record/print it
        return
    for each possible next choice:
        if this choice is valid:
            make the choice
            backtrack(choices_made_so_far + this choice)
            UNDO the choice    <-- this is the "backtrack" step
```

The UNDO step is critical: after the recursive call returns, the state must be
exactly as it was before so the next loop iteration can try a different choice
from the same starting point.

### Decision Tree

For generating subsets of [1, 2], every element has two choices (include or skip):

```
                        {}
                      /    \
                skip 1      include 1
                /    \        /    \
          skip 2  incl 2  skip 2  incl 2
            |       |       |       |
           {}      {2}     {1}    {1,2}     <-- 4 subsets = 2^2
```

Each path from root to leaf represents one complete set of decisions. The number
of leaves is 2^n because each element has 2 choices.

### Pruning: Why Backtracking Beats Brute Force

Brute force generates ALL possibilities, then filters out invalid ones.
Backtracking filters AS IT GOES, cutting off entire subtrees early.

N-Queens example (4x4 board): after placing a queen in row 0 column 0, we
discover that row 1 columns 0 and 1 are both attacked. Instead of generating
all possible boards with queens at (0,0) and (1,0), we immediately skip that
entire branch. This is called **pruning**.

```
                             []
                     /     |     |     \
               Q@col0  Q@col1  Q@col2  Q@col3     <- row 0
               /  |  \
          Q@1  Q@2  Q@3                            <- row 1
           X    |    X          X = pruned (attacked)
               ...
```

### Time Complexity

Backtracking problems are usually exponential — and that's expected:
- Subsets: O(2^n) — you MUST generate 2^n subsets
- Permutations: O(n!) — there ARE n! permutations
- N-Queens: much less than n^n thanks to pruning, but still exponential

The point isn't to avoid exponential time (impossible when the output is
exponential). The point is to **not waste time on branches that can never work**.

---

## 9. Advanced Dynamic Programming

In the memoization kata, you learned that DP applies when a problem has
**overlapping subproblems** and **optimal substructure**. You solved Fibonacci,
stair climbing, coin change, and LCS.

Now we cover five classic DP patterns that appear throughout computer science.
Almost every DP problem you'll encounter fits one of these patterns.

### The Five Classic DP Patterns

1. **0/1 Knapsack** — for each item, choose to take it or skip it.
   Table: `dp[i][w]` = best value using items 0..i-1 with capacity w.

2. **Sequence DP** — `dp[i]` depends on all `dp[j]` where j < i.
   Example: Longest Increasing Subsequence (LIS).

3. **Two-string DP** — 2D table indexed by positions in two strings.
   Example: Edit Distance (extends LCS you already know).

4. **Interval DP** — `dp[i][j]` represents a substring or subarray range.
   Example: Longest Palindromic Subsequence (LPS).

5. **String Partition DP** — can we split s[0..i] into valid pieces?
   Example: Word Break.

### How to Identify the Pattern

Ask yourself:
- Am I choosing to **take or skip** each item? -> Knapsack
- Does `dp[i]` depend on **earlier elements** in a sequence? -> Sequence DP
- Am I comparing **two strings** character by character? -> Two-string DP
- Am I working with **substrings** s[i..j]? -> Interval DP
- Am I **splitting a string** into valid segments? -> String Partition DP

### Edit Distance DP Table

Converting "kit" to "sit" (replace k with s = 1 operation):

```
          ""  s   i   t
      ""   0  1   2   3      <- converting "" to "sit" costs 3 inserts
      k    1  1   2   3      <- k!=s: 1+min(0,1,1) = 1
      i    2  2   1   2      <- i==i: copy diagonal = 1
      t    3  3   2   1      <- t==t: copy diagonal = 1
                        ^
                   answer = 1
```

Each cell considers three operations (insert, delete, replace) and takes the
minimum. When characters match, we copy the diagonal (no operation needed).

### All DP Problems in This Curriculum

| Problem                       | Pattern           | Dimensions | Complexity   |
|-------------------------------|-------------------|------------|--------------|
| Fibonacci / Stair Climbing    | Linear DP         | 1D         | O(n)         |
| Coin Change                   | Unbounded Knapsack| 1D         | O(n * coins) |
| LCS (Longest Common Subseq)  | Two-string DP     | 2D         | O(m * n)     |
| 0/1 Knapsack                 | 0/1 Knapsack      | 2D         | O(n * W)     |
| LIS (Longest Increasing Sub) | Sequence DP       | 1D         | O(n^2)       |
| Edit Distance                 | Two-string DP     | 2D         | O(m * n)     |
| Longest Palindromic Subseq   | Interval DP       | 2D         | O(n^2)       |
| Word Break                    | String Partition   | 1D         | O(n * dict)  |

---

## 10. Katas

Work through these exercises in order:

| Kata                  | Concepts Practiced                                 |
|-----------------------|----------------------------------------------------|
| `factorial.c`         | Basic recursion, base/recursive case, iteration    |
| `fibonacci.c`         | Call tree, O(2^n) vs O(n), counting calls          |
| `power.c`             | Divide and conquer, O(log n) algorithm             |
| `hanoi.c`             | Classic recursion problem, building intuition       |
| `recursive_strings.c` | Recursion on arrays and strings                    |
| `memoization.c`       | Memoization, tabulation, 2D DP, coin change, LCS   |
| `backtracking.c`      | Subsets, permutations, N-Queens, Sudoku, word search|
| `advanced_dp.c`       | Knapsack, LIS, edit distance, LPS, word break      |

### Building and Running

```bash
cd 05-recursion-algorithmic-thinking/

# Build all exercises:
make exercises

# Build all solutions:
make solutions

# Build and run tests on solutions:
make test

# Clean up:
make clean
```

Each exercise file has `TODO` markers where you write your code. Read the
comments carefully — they tell you exactly what to do.

### Recommended Order

1. **factorial.c** — warmup, get comfortable with recursion
2. **fibonacci.c** — see why naive recursion is slow, then fix it
3. **power.c** — your first O(log n) algorithm using divide and conquer
4. **hanoi.c** — a classic that is hard iteratively but elegant recursively
5. **recursive_strings.c** — apply recursion to strings and arrays
6. **memoization.c** — fix the exponential problem with caching, then go further
7. **backtracking.c** — the most important recursion pattern for problem-solving
8. **advanced_dp.c** — five classic DP patterns that cover most interview problems

Take your time. Draw the call stack on paper for each function before running
it. Recursion clicks once you can see the frames in your head.

---

## Phase 1 Complete — Review & Connect

You've finished the foundations. Let's connect the dots:

- **Module 00**: Computers execute simple instructions on bits
- **Module 01**: C source → preprocessor → compiler → assembler → linker → executable
- **Module 02**: Every value is bits — int, float, char are just different interpretations
- **Module 03**: Control flow directs which instructions execute
- **Module 04**: Functions organize code; the call stack tracks who called whom
- **Module 05**: Recursion is functions calling themselves; Big-O measures growth

**You can now**: write programs that take input, compute results, and produce
output. Every concept from here builds on these foundations.

**What's next**: Phase 2 introduces pointers — the concept that unlocks
everything from data structures to operating systems.

---

[← Previous: Module 04 — Functions & Program Structure](../04-functions-program-structure/README.md) | [Next: Module 06 — Pointers & Arrays →](../07-pointers-arrays/README.md)
