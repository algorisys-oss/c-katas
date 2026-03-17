# Module 12: Stacks & Queues

## What Are Stacks and Queues?

You already know arrays and linked lists — they store collections of elements.
Stacks and queues are **restricted** versions of these: they limit *how* you
access data to enforce a specific order. This restriction isn't a weakness —
it's a **superpower**. By limiting what you can do, you make it impossible to
use the data structure wrong.

---

## Stacks — Last In, First Out (LIFO)

A stack works like a stack of plates in a cafeteria:
- You can only add a plate to the **top** (push)
- You can only remove the plate from the **top** (pop)
- You can look at the top plate without removing it (peek)

```
    push(10)    push(20)    push(30)    pop() → 30   pop() → 20

    ┌────┐
    │ 30 │ ← top
    ├────┤      ├────┤
    │ 20 │ ← top     │ 20 │ ← top     │ 20 │ ← top
    ├────┤      ├────┤      ├────┤      ├────┤      ├────┤
    │ 10 │      │ 10 │      │ 10 │      │ 10 │      │ 10 │ ← top
    └────┘      └────┘      └────┘      └────┘      └────┘
```

### Stack Operations

| Operation      | What it does                        | Time  |
|----------------|-------------------------------------|-------|
| `push(value)`  | Add value to the top                | O(1)  |
| `pop()`        | Remove and return the top value     | O(1)  |
| `peek()`       | Return the top value (don't remove) | O(1)  |
| `is_empty()`   | Check if the stack has no elements  | O(1)  |

All operations are O(1) — that's what makes stacks so efficient.

### Array-Based Stack

The simplest implementation: use an array and track the top index.

```
    Array:    [ 10 | 20 | 30 |    |    |    ]
    Index:      0    1    2    3    4    5
                               ↑
                              top = 3 (next empty slot)

    push(40): array[top] = 40; top++;
              [ 10 | 20 | 30 | 40 |    |    ]
                                    ↑
                                   top = 4

    pop():    top--; return array[top];
              [ 10 | 20 | 30 | 40 |    |    ]
                               ↑
                              top = 3   → returns 40
```

Key idea: `top` is the index of the next empty slot.
- Push: write at `top`, then increment.
- Pop: decrement `top`, then read.
- Peek: read at `top - 1` without changing anything.

### Linked-List-Based Stack

You can also use a linked list where push/pop happen at the **head**:

```
    push(30):
        ┌────┬───┐   ┌────┬───┐   ┌────┬───┐
        │ 30 │ ──┼──→│ 20 │ ──┼──→│ 10 │ / │
        └────┴───┘   └────┴───┘   └────┴───┘
          ↑
         head (top of stack)

    pop():  save head->data (30), move head to head->next
        ┌────┬───┐   ┌────┬───┐
        │ 20 │ ──┼──→│ 10 │ / │
        └────┴───┘   └────┴───┘
          ↑
         head (new top)
```

**Array vs. Linked List Stack:**

| Factor          | Array-based              | Linked-list-based         |
|-----------------|--------------------------|---------------------------|
| Memory          | Pre-allocated (may waste) | Grows on demand           |
| Cache           | Excellent (contiguous)   | Poor (scattered nodes)    |
| Max size        | Fixed (or needs resize)  | Unlimited (until OOM)     |
| Simplicity      | Very simple              | Slightly more complex     |
| Best for        | Known max size           | Unknown/variable size     |

---

## Queues — First In, First Out (FIFO)

A queue works like a line at a ticket counter:
- New people join at the **back** (enqueue)
- The person at the **front** gets served first (dequeue)

```
    enqueue(A)  enqueue(B)  enqueue(C)  dequeue()→A  dequeue()→B

    front→  ┌───┐
            │ A │
            └───┘

    front→  ┌───┐  ┌───┐
            │ A │──│ B │  ←back
            └───┘  └───┘

    front→  ┌───┐  ┌───┐  ┌───┐
            │ A │──│ B │──│ C │  ←back
            └───┘  └───┘  └───┘

    front→  ┌───┐  ┌───┐
            │ B │──│ C │  ←back
            └───┘  └───┘

    front→  ┌───┐
            │ C │  ←back
            └───┘
```

### Queue Operations

| Operation         | What it does                        | Time  |
|-------------------|-------------------------------------|-------|
| `enqueue(value)`  | Add value to the back               | O(1)  |
| `dequeue()`       | Remove and return the front value   | O(1)  |
| `peek()`          | Return the front value (don't remove) | O(1) |
| `is_empty()`      | Check if the queue has no elements  | O(1)  |
| `is_full()`       | Check if the queue is at capacity   | O(1)  |

### The Problem with a Naive Array Queue

If you use a plain array with a `front` index, dequeue shifts everything left
(O(n)) or wastes space:

```
    After enqueue A, B, C, D:     After dequeue A, B:

    [ A | B | C | D |   |   ]     [   |   | C | D |   |   ]
      ↑               ↑             ↑       ↑
     front           back          (wasted) front

    The space at indices 0 and 1 is wasted forever!
```

### Circular Buffer — The Smart Fix

A **circular buffer** wraps around: when you reach the end of the array,
you loop back to the beginning. This way, no space is ever wasted.

```
    Capacity = 6, enqueue A, B, C, D, then dequeue A, B:

    Array:  [   |   | C | D |   |   ]
             ↑               ↑
            front=2        back=4

    Now enqueue E, F, G:

    Array:  [ G |   | C | D | E | F ]
                 ↑   ↑
               back=1 front=2

    The buffer has WRAPPED AROUND!
```

The magic formula: use modular arithmetic for wrapping.
```
    next index = (current index + 1) % capacity
```

Visual of the circular nature:
```
              ┌───┐
         ┌────│ 5 │────┐
         │    └───┘    │
       ┌───┐        ┌───┐
       │ 4 │        │ 0 │
       └───┘        └───┘
         │    ┌───┐    │
         └────│ 3 │────┘
              └───┘
           ┌───┐ ┌───┐
           │ 2 │ │ 1 │
           └───┘ └───┘

    Indices go: 0 → 1 → 2 → 3 → 4 → 5 → 0 → 1 → ...
```

### How to Tell Full from Empty?

Both full and empty have `front == back` if you're not careful!

**Solution: track the count separately.**

```
    Empty:  count == 0
    Full:   count == capacity

    This is simpler and clearer than the "waste one slot" trick.
```

---

## When to Use Stacks vs. Queues

| Use a **Stack** when...                  | Use a **Queue** when...                  |
|------------------------------------------|------------------------------------------|
| Processing in reverse order              | Processing in arrival order              |
| Matching nested things (parens, tags)    | Scheduling (tasks, print jobs)           |
| Undo/redo                                | Breadth-first search (BFS)               |
| Function call tracking (the call stack!) | Message passing between components       |
| Expression evaluation                    | Buffering (keyboard input, network data) |
| Depth-first search (DFS)                 | Fair resource sharing (round-robin)      |

---

## Classic Stack Application: Balanced Parentheses

One of the most famous uses of a stack: checking if brackets match.

```
    Input: "{ [ ( ) ] }"

    Step 1: see '{'  → push '{'    Stack: [ { ]
    Step 2: see '['  → push '['    Stack: [ { [ ]
    Step 3: see '('  → push '('    Stack: [ { [ ( ]
    Step 4: see ')'  → pop '(' ✓   Stack: [ { [ ]     (matches '(')
    Step 5: see ']'  → pop '[' ✓   Stack: [ { ]        (matches '[')
    Step 6: see '}'  → pop '{' ✓   Stack: [ ]           (matches '{')

    End: stack is empty → BALANCED ✓
```

```
    Input: "( [ ) ]"

    Step 1: see '('  → push '('    Stack: [ ( ]
    Step 2: see '['  → push '['    Stack: [ ( [ ]
    Step 3: see ')'  → pop '[' ✗   MISMATCH! '[' ≠ ')'  → NOT BALANCED
```

---

## Classic Stack Application: Expression Evaluation

Evaluating `3 + 4 * 2` correctly requires understanding **operator precedence**.
The standard approach uses two steps:

### Step 1: Infix to Postfix (Shunting-Yard Algorithm)

Infix: `3 + 4 * 2`  (operators between operands — how humans write)
Postfix: `3 4 2 * +`  (operators after operands — how machines evaluate)

The algorithm uses an operator stack:
```
    Token   Action                  Output Queue     Operator Stack
    ─────   ──────────────────────  ───────────────  ──────────────
    3       number → output         3                (empty)
    +       push to stack           3                +
    4       number → output         3 4              +
    *       * > + precedence, push  3 4              + *
    2       number → output         3 4 2            + *
    (end)   pop all to output       3 4 2 * +        (empty)
```

Key rule: before pushing an operator, pop any operators with **higher or equal
precedence** to the output.

### Step 2: Evaluate Postfix

Use a value stack:
```
    Token   Action                  Stack
    ─────   ──────────────────────  ─────────
    3       push                    [3]
    4       push                    [3, 4]
    2       push                    [3, 4, 2]
    *       pop 4,2 → push 4*2=8   [3, 8]
    +       pop 3,8 → push 3+8=11  [11]

    Result: 11 ✓
```

---

## Exercises

| # | File                 | What You'll Build                                |
|---|----------------------|--------------------------------------------------|
| 1 | `stack.c`            | Array-based stack + balanced parentheses checker |
| 2 | `queue.c`            | Circular buffer queue                            |
| 3 | `expression_eval.c`  | Infix → postfix converter + postfix evaluator    |

Build with `make exercises` and test solutions with `make test`.
