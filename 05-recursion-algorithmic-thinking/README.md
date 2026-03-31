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

### Memoization Preview

There is a third approach: keep the recursive structure, but **remember** results
you have already computed. This is called **memoization** (not memorization). We
will explore this in a later module, but the idea is simple: before computing
fib(n), check if you already know the answer. If so, return it immediately.

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

## 8. Katas

Work through these exercises in order:

| Kata                  | Concepts Practiced                                 |
|-----------------------|----------------------------------------------------|
| `factorial.c`         | Basic recursion, base/recursive case, iteration    |
| `fibonacci.c`         | Call tree, O(2^n) vs O(n), counting calls          |
| `power.c`             | Divide and conquer, O(log n) algorithm             |
| `hanoi.c`             | Classic recursion problem, building intuition       |
| `recursive_strings.c` | Recursion on arrays and strings                    |

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

[← Previous: Module 04 — Functions & Program Structure](../04-functions-program-structure/README.md) | [Next: Module 06 — Pointers & Arrays →](../06-pointers-arrays/README.md)
