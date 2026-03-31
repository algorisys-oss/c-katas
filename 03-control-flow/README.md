# Module 03 — Control Flow

Your programs so far have been straight lines — they run every statement from top
to bottom, no exceptions. Real programs need to make **decisions** and **repeat**
things. That is what control flow is: choosing which instructions to run, and
how many times.

By the end of this module you will be able to:
- Branch your code with `if/else` and `switch`
- Loop with `while`, `for`, and `do-while`
- Combine conditions with boolean logic (AND, OR, NOT)
- Think algorithmically: break a problem into precise steps

---

## 1. if/else — Choosing a Path

The simplest decision: "if this is true, do A; otherwise, do B."

```c
if (temperature > 100) {
    printf("Water is boiling!\n");
} else {
    printf("Not boiling yet.\n");
}
```

You can chain decisions with `else if`:

```c
if (score >= 90) {
    printf("A\n");
} else if (score >= 80) {
    printf("B\n");
} else if (score >= 70) {
    printf("C\n");
} else {
    printf("F\n");
}
```

### How if/else Executes — Flowchart

```
            +---------------------+
            |   evaluate condition |
            +---------------------+
                    |
              +-----+-----+
              |           |
            true        false
              |           |
       +------+---+  +---+-------+
       | if-body  |  | else-body |
       +------+---+  +---+-------+
              |           |
              +-----+-----+
                    |
                    v
            (continue program)
```

The condition is evaluated **once**. Exactly one branch runs. Then execution
continues after the whole if/else block.

### Chained else-if Flowchart

```
     +------------------+
     | condition 1 true?|---yes---> [ body 1 ] ---+
     +------------------+                         |
             | no                                 |
     +------------------+                         |
     | condition 2 true?|---yes---> [ body 2 ] ---+
     +------------------+                         |
             | no                                 |
     +------------------+                         |
     | condition 3 true?|---yes---> [ body 3 ] ---+
     +------------------+                         |
             | no                                 |
        [ else body ] ----------------------------+
                                                  |
                                                  v
                                          (continue program)
```

C checks each condition **in order**. The first one that is true wins. If none
are true, the `else` body runs (if you wrote one).

---

## 2. switch — Choosing From Many Options

When you are comparing one value against many constants, `switch` is cleaner
than a chain of `else if`:

```c
switch (day) {
    case 1:
        printf("Monday\n");
        break;
    case 2:
        printf("Tuesday\n");
        break;
    case 3:
        printf("Wednesday\n");
        break;
    default:
        printf("Other day\n");
        break;
}
```

### Why `break` Matters

Without `break`, execution **falls through** to the next case. This is a common
source of bugs:

```c
switch (x) {
    case 1:
        printf("one\n");       /* no break! */
    case 2:
        printf("two\n");       /* this runs if x is 1 OR 2 */
        break;
    case 3:
        printf("three\n");
        break;
}
```

If `x` is 1, this prints **both** "one" and "two". The `break` statement tells
C to jump out of the switch block. Forget it and you get fall-through.

Sometimes fall-through is intentional (grouping cases that share code), but
always add a comment `/* fall through */` to make it clear.

---

## 3. Loops — Repeating Things

### 3a. while — "Keep Going While This Is True"

```c
int count = 0;
while (count < 5) {
    printf("%d\n", count);
    count++;
}
```

This prints 0, 1, 2, 3, 4. The loop checks the condition **before** each
iteration. If the condition is false from the start, the body never runs.

### while Loop Flowchart

```
            +-------------------+
            |  evaluate condition|
            +-------------------+
                    |
              +-----+-----+
              |           |
            true        false
              |           |
       +------+------+    |
       |  loop body  |    |
       +------+------+    |
              |           |
              +-----------+
                          |
                          v
                  (continue program)
```

The arrow from "loop body" goes **back up** to "evaluate condition". That is
what makes it a loop.

### 3b. for — Syntactic Sugar for Common Patterns

Most `while` loops follow a pattern: initialize something, check a condition,
update something. The `for` loop packages all three into one line:

```
    for (init; condition; update)
         |       |         |
         |       |         +-- runs after each iteration
         |       +------------ checked before each iteration
         +-------------------- runs once at the start
```

```c
for (int i = 0; i < 5; i++) {
    printf("%d\n", i);
}
```

This is **exactly equivalent** to:

```c
int i = 0;          /* init */
while (i < 5) {     /* condition */
    printf("%d\n", i);
    i++;            /* update */
}
```

The `for` version is just more compact. Use whichever is clearer.

### for Loop Flowchart

```
        +--------+
        |  init  |  (runs once)
        +--------+
            |
            v
    +-------------------+
    |  condition true?  |---no---> (continue program)
    +-------------------+
            |
           yes
            |
            v
    +-------------------+
    |     loop body     |
    +-------------------+
            |
            v
    +-------------------+
    |      update       |
    +-------------------+
            |
            +-----> (back to condition)
```

### 3c. do-while — "Do It At Least Once"

Sometimes you need the body to run **at least once** before checking the
condition. That is `do-while`:

```c
int input;
do {
    printf("Enter a positive number: ");
    /* scanf reads input from the keyboard.
       %d means "read an integer."
       The & before input gives scanf the memory address where it should
       store the value — we'll fully explain & when we learn pointers,
       but for now just know you need it with scanf. */
    scanf("%d", &input);
} while (input <= 0);
```

This always prompts once, then keeps prompting until the user enters a positive
number. Notice the semicolon after `while(...)` — easy to forget.

### 3d. break — Exit the Loop Early

```c
for (int i = 0; i < 100; i++) {
    if (i == 42) {
        break;    /* stop the loop right now */
    }
    printf("%d\n", i);
}
/* execution continues here after break */
```

`break` immediately exits the **innermost** loop that contains it.

### 3e. continue — Skip to the Next Iteration

```c
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue;    /* skip even numbers */
    }
    printf("%d\n", i);    /* only prints odd: 1 3 5 7 9 */
}
```

`continue` skips the rest of the loop body and jumps to the next iteration
(back to the condition check, after running the update in a `for` loop).

---

## 4. Nested Loops — Loops Inside Loops

When you put a loop inside a loop, the **inner loop runs completely** for each
iteration of the outer loop:

```c
for (int row = 1; row <= 3; row++) {
    for (int col = 1; col <= 4; col++) {
        printf("%d*%d=%-4d", row, col, row * col);
    }
    printf("\n");
}
```

### Execution Trace

Let's trace exactly which values `row` and `col` take:

```
Step  | row | col | What Happens
------+-----+-----+--------------------------------------------
  1   |  1  |  1  | inner loop starts, print "1*1=1   "
  2   |  1  |  2  | print "1*2=2   "
  3   |  1  |  3  | print "1*3=3   "
  4   |  1  |  4  | print "1*4=4   "
  5   |  1  |  5  | col < 4 is false, inner loop ends, print \n
  6   |  2  |  1  | outer loop iteration 2, inner restarts
  7   |  2  |  2  | print "2*2=4   "
  8   |  2  |  3  | print "2*3=6   "
  9   |  2  |  4  | print "2*4=8   "
 10   |  2  |  5  | inner loop ends, print \n
 11   |  3  |  1  | outer loop iteration 3, inner restarts
 12   |  3  |  2  | print "3*2=6   "
 13   |  3  |  3  | print "3*3=9   "
 14   |  3  |  4  | print "3*4=12  "
 15   |  3  |  5  | inner loop ends, print \n
 16   |  4  |  -  | row <= 3 is false, outer loop ends
```

Output:

```
1*1=1   1*2=2   1*3=3   1*4=4
2*1=2   2*2=4   2*3=6   2*4=8
3*1=3   3*2=6   3*3=9   3*4=12
```

**Key insight**: if the outer loop runs N times and the inner loop runs M times,
the body executes N x M times total. Nested loops multiply.

---

## 5. Boolean Logic — From First Principles

Every condition in an `if` or `while` is either **true** (non-zero in C) or
**false** (zero in C). Boolean logic is the math of true and false values.

### 5a. Truth Tables — Enumerate All Possibilities

With 2 inputs, there are exactly 4 possible combinations. A truth table lists
every one.

### AND (`&&`) — Both Must Be True

"You can go to the party if you finished homework AND cleaned your room."

```
    A     |    B     |  A && B
  --------+----------+---------
   false  |  false   |  false
   false  |  true    |  false
   true   |  false   |  false
   true   |  true    |  true       <-- only this row
```

AND is strict: **both** must be true.

### OR (`||`) — At Least One Must Be True

"You get dessert if you ate vegetables OR you ate all your rice."

```
    A     |    B     |  A || B
  --------+----------+---------
   false  |  false   |  false      <-- only this row
   false  |  true    |  true
   true   |  false   |  true
   true   |  true    |  true
```

OR is generous: **at least one** must be true.

### NOT (`!`) — Flip It

```
    A     |   !A
  --------+--------
   false  |  true
   true   |  false
```

NOT takes one input and flips it.

### 5b. De Morgan's Laws — Simplifying Conditions

These two rules let you rewrite complex conditions:

```
    !(A && B)  ==  (!A || !B)
    !(A || B)  ==  (!A && !B)
```

In English:
- "It's NOT the case that (A AND B)" is the same as "either A is false OR B is false"
- "It's NOT the case that (A OR B)" is the same as "A is false AND B is false"

**Why this matters**: when you negate a compound condition, AND becomes OR and
vice versa. Let's verify with a truth table:

```
    A   |  B   | A && B | !(A && B) | !A  | !B  | !A || !B
  ------+------+--------+-----------+-----+-----+---------
  false | false|  false |   true    | true| true|   true     MATCH
  false | true |  false |   true    | true|false|   true     MATCH
  true  | false|  false |   true    |false| true|   true     MATCH
  true  | true |  true  |   false   |false|false|   false    MATCH
```

Every row matches. The law holds.

**Practical example**: say you have:

```c
if (!(age >= 18 && has_id)) {
    printf("Cannot enter.\n");
}
```

Apply De Morgan's: `!(A && B)` becomes `(!A || !B)`:

```c
if (age < 18 || !has_id) {
    printf("Cannot enter.\n");
}
```

The second version is easier to read.

### 5c. Short-Circuit Evaluation

C evaluates boolean expressions **left to right** and **stops as soon as it
knows the answer**.

**AND short-circuits on false:**

```c
if (ptr != NULL && ptr->value > 0) {
    /* safe! */
}
```

If `ptr` is NULL, the left side is false. Since `false && anything` is always
false, C **never evaluates** `ptr->value > 0`. This prevents a crash.

**OR short-circuits on true:**

```c
if (is_admin || has_permission(user, resource)) {
    /* allow access */
}
```

If `is_admin` is true, C **never calls** `has_permission()`. Since
`true || anything` is always true, it already knows the answer.

**Why this matters**:
1. It prevents crashes (the NULL pointer example above)
2. It can skip expensive function calls
3. The **order** of your conditions matters

---

## 6. Algorithmic Thinking

### What Is an Algorithm?

An algorithm is a **recipe**: a precise, finite, unambiguous sequence of steps
that solves a problem. Like a cooking recipe, but every step must be so clear
that a machine (which has zero common sense) can follow it.

Three requirements:
1. **Precise** — every step is exactly defined
2. **Finite** — it eventually stops
3. **Unambiguous** — there is only one way to interpret each step

### Pseudocode: Think Before You Code

Before writing C, write the steps in plain English. This is **pseudocode**:

**Problem**: Is a number prime?

```
Pseudocode:
    1. Read a number n
    2. If n < 2, it is NOT prime (by definition)
    3. For each number i from 2 to sqrt(n):
        a. If n is divisible by i, it is NOT prime
    4. If we checked all numbers and found no divisor, it IS prime
```

Now translate to C:

```c
#include <stdio.h>
#include <math.h>

int is_prime(int n)
{
    if (n < 2) {
        return 0;    /* not prime */
    }
    for (int i = 2; i <= (int)sqrt((double)n); i++) {
        if (n % i == 0) {
            return 0;    /* found a divisor, not prime */
        }
    }
    return 1;    /* no divisor found, prime */
}
```

The pseudocode maps directly to the C code. Each English step became one or two
lines of C.

### Tracing: Walk Through Your Code by Hand

Tracing means becoming the computer. Pick inputs, then go line by line, writing
down what each variable holds.

**Trace `is_prime(12)`:**

```
Step | Line                        | n  | i  | n % i | Result
-----+-----------------------------+----+----+-------+--------
  1  | if (n < 2) -> 12 < 2? no   | 12 |  - |   -   |  -
  2  | i = 2, i <= 3? yes          | 12 |  2 |   -   |  -
  3  | 12 % 2 == 0? yes            | 12 |  2 |   0   | return 0
```

We found a divisor on the first check. 12 is not prime. Done in 3 steps.

**Trace `is_prime(7)`:**

```
Step | Line                        | n  | i  | n % i | Result
-----+-----------------------------+----+----+-------+--------
  1  | if (n < 2) -> 7 < 2? no    |  7 |  - |   -   |  -
  2  | i = 2, i <= 2? yes          |  7 |  2 |   -   |  -
  3  | 7 % 2 == 0? no              |  7 |  2 |   1   |  -
  4  | i = 3, i <= 2? no           |  7 |  3 |   -   |  -
  5  | loop ends, return 1         |  7 |  - |   -   | return 1
```

No divisor found. 7 is prime.

---

## 7. Katas

Now put it all together. You have four exercises:

| Kata            | Concepts Practiced                                  |
|-----------------|-----------------------------------------------------|
| `fizzbuzz.c`    | if/else chains, modulo operator, loops              |
| `number_guess.c`| while loop, break, scanf, rand(), if/else           |
| `patterns.c`    | nested loops, inner/outer loop relationship         |
| `prime_checker.c`| for loop, break, algorithmic thinking, test harness |

### Building and Running

```bash
cd 03-control-flow/

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

1. **fizzbuzz.c** — the classic warmup
2. **number_guess.c** — interactive, uses a while loop
3. **patterns.c** — challenges your nested-loop thinking
4. **prime_checker.c** — algorithmic thinking with a test harness

Good luck. Take your time, trace through your code by hand before running it,
and remember: the compiler is your friend, not your enemy.

---

[← Previous: Module 02 — Types, Variables & Operators](../02-types-variables-operators/README.md) | [Next: Module 04 — Functions & Program Structure →](../04-functions-program-structure/README.md)
