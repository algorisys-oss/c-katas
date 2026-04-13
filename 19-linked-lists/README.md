# Module 11 — Linked Lists

Arrays store elements in contiguous memory — every element sits right next to the
previous one. This is great for random access but terrible for inserting or
deleting elements in the middle, because you have to shift everything over.

A **linked list** solves this problem with a completely different approach: each
element is its own separate chunk of memory, and they're connected by pointers.
You can insert or remove an element anywhere in the list by rewiring a few
pointers — no shifting required.

---

## The Node: Building Block of a Linked List

A linked list is made of **nodes**. Each node holds two things:
1. The **data** (the value you care about)
2. A **pointer** to the next node

```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;
```

Why `struct Node *next` instead of `Node *next`? Because the typedef `Node`
doesn't exist yet when the compiler is inside the struct definition. You have
to use the struct tag `struct Node` to refer to the type being defined.

In memory, a node looks like this:

```
  Node
┌──────────┬──────────┐
│ data: 42 │ next: ─────→  (points to another Node, or NULL)
└──────────┴──────────┘
    4 bytes    8 bytes
```

---

## Singly Linked List

A **singly linked list** is a chain of nodes where each node points to the next
one, and the last node points to `NULL`:

```
 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘
                                        NULL
```

You access the list through a **head pointer**. To reach any element, you start
at the head and follow the `next` pointers — this is called **traversal**.

### The List Structure

We wrap the head pointer in a struct so we can pass the list around:

```c
typedef struct {
    Node *head;
    int size;
} LinkedList;
```

---

## Core Operations

### Insert at Head (Push Front)

The fastest insertion — O(1). Create a new node, point it at the current head,
then update head to point at the new node:

```
Before:
 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐
│ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘

Step 1: Create new node, point next to current head

  new           head
  │              │
  ▼              ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 2: Update head to point to new node

 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘
```

```c
void list_push_front(LinkedList *list, int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}
```

### Insert at Tail (Push Back)

You must walk to the end of the list first — O(n):

```
Before:
 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ / │
└──────┴───┘    └──────┴───┘

Step 1: Walk to the last node (next == NULL)

                  curr
                   │
                   ▼
┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ / │
└──────┴───┘    └──────┴───┘

Step 2: Create new node, set last->next to new node

 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘
```

### Remove from Head (Pop Front)

Save the head, advance head to the next node, free the old head — O(1):

```
Before:
 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 1: Save head, advance head to head->next

  old           head
  │              │
  ▼              ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 2: Free old head

 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐
│ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘
```

### Delete by Value

Find the node with the matching value, then rewire the previous node's `next`
pointer to skip over it:

```
Delete node with value 20:

 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 1: Find node with value 20, keep track of previous node

  prev          target
   │              │
   ▼              ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 2: Set prev->next = target->next, free target

 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘
```

The tricky part: deleting the **head** node is a special case because there's
no "previous" node. You have to update the head pointer directly.

### Search (Find)

Walk the list, comparing each node's data to the target — O(n):

```c
Node *list_find(LinkedList *list, int value)
{
    Node *curr = list->head;
    while (curr != NULL) {
        if (curr->data == value) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
```

### Reverse

Reversing a linked list is a classic interview question and a great exercise
in pointer manipulation. You walk through the list, reversing each `next`
pointer as you go:

```
Before:
 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Use three pointers: prev, curr, next_node

Step 1: prev=NULL, curr=10
   Set next_node = curr->next (20)
   Set curr->next = prev (NULL)
   Move prev = curr, curr = next_node

  prev          curr
   │              │
   ▼              ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ / │    │ 20   │ ──┼──→ │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 2: prev=10, curr=20
   Set next_node = curr->next (30)
   Set curr->next = prev (10)
   Move prev = curr, curr = next_node

                  prev          curr
                   │              │
                   ▼              ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ / │ ←──┼── 20 │   │    │ 30   │ / │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Step 3: prev=20, curr=30
   Set next_node = curr->next (NULL)
   Set curr->next = prev (20)
   Move prev = curr, curr = next_node (NULL)

                                  prev     curr=NULL
                                   │
                                   ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ / │ ←──┼── 20 │   │←──┼── 30 │   │
└──────┴───┘    └──────┴───┘    └──────┴───┘

curr is NULL, so we're done. Set head = prev.

                                  head
                                   │
                                   ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ / │ ←──┼── 20 │   │←──┼── 30 │   │
└──────┴───┘    └──────┴───┘    └──────┴───┘

Now: 30 → 20 → 10 → NULL
```

---

## Doubly Linked List

A **doubly linked list** adds a `prev` pointer to each node, so you can
traverse in both directions:

```c
typedef struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;
```

```
        head                                    tail
         │                                        │
         ▼                                        ▼
   ┌───┬──────┬───┐    ┌───┬──────┬───┐    ┌───┬──────┬───┐
   │ / │  10  │ ──┼──→ │ ←─┼  20  │ ──┼──→ │ ←─┼  30  │ / │
   └───┴──────┴───┘    └───┴──────┴───┘    └───┴──────┴───┘
   prev data  next     prev data  next     prev data  next
```

**Advantages over singly linked:**
- Delete a node in O(1) if you have a pointer to it (no need to find prev)
- Traverse backwards
- Pop from tail in O(1)

**Disadvantages:**
- Extra memory per node (one more pointer)
- More pointer manipulation on insert/delete (more bookkeeping)

---

## Circular Linked List

In a **circular list**, the last node points back to the first instead of NULL:

```
 head
  │
  ▼
┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐
│ 10   │ ──┼──→ │ 20   │ ──┼──→ │ 30   │ ──┼──┐
└──────┴───┘    └──────┴───┘    └──────┴───┘  │
  ▲                                            │
  └────────────────────────────────────────────┘
```

Use cases:
- Round-robin scheduling (CPU time slicing)
- Circular buffers
- Turn-based games (cycle through players)

To detect the end of a circular list, check if `curr->next == head` instead
of `curr->next == NULL`.

---

## Linked Lists vs Arrays: Big-O Comparison

```
┌─────────────────────────┬──────────────┬──────────────┐
│ Operation               │    Array     │ Linked List  │
├─────────────────────────┼──────────────┼──────────────┤
│ Access by index         │    O(1)      │    O(n)      │
│ Search (unsorted)       │    O(n)      │    O(n)      │
│ Insert at beginning     │    O(n)      │    O(1)      │
│ Insert at end           │    O(1)*     │    O(n)**    │
│ Insert in middle        │    O(n)      │    O(1)***   │
│ Delete at beginning     │    O(n)      │    O(1)      │
│ Delete at end           │    O(1)*     │    O(n)**    │
│ Delete in middle        │    O(n)      │    O(1)***   │
│ Memory per element      │  sizeof(T)   │ sizeof(T)+8  │
│ Cache performance       │  Excellent   │    Poor      │
└─────────────────────────┴──────────────┴──────────────┘

 *   Amortized for dynamic arrays
 **  O(1) if you maintain a tail pointer
 *** O(1) once you have a pointer to the location;
     finding the location is O(n)
```

**When to use arrays:** random access needed, cache performance matters,
size is known or changes rarely.

**When to use linked lists:** frequent insertion/deletion at arbitrary
positions, size changes constantly, you don't need random access.

### The Cache Reality

In practice, arrays are often faster than linked lists even for operations
where linked lists have better Big-O. Why? **Cache locality**. Array elements
sit next to each other in memory, so the CPU prefetcher loads them into cache
efficiently. Linked list nodes are scattered across the heap — every `next`
pointer dereference is potentially a cache miss.

This is why you should measure performance, not just count Big-O. Algorithms
with better theoretical complexity can be slower in practice due to cache effects.

### The Performance Reality

In Big-O terms, linked lists and arrays both have O(n) traversal. But in
practice, traversing a linked list can be 10-100x slower than an array of
the same size. Why?

Each linked list node was allocated separately with malloc(). They can be
scattered anywhere in memory:

```
Array:     [0][1][2][3][4][5][6][7]   ← all in one cache line!

Linked list nodes in memory:
addr 0x1000: [data|next] ──→ addr 0x5420: [data|next] ──→ addr 0x2108: ...
             ↑ cache line 1  ↑ different cache line!       ↑ yet another!
```

Every node->next might be a cache MISS — a trip to RAM that takes 100ns
instead of 1ns. Multiply by thousands of nodes and the difference is huge.

This doesn't mean linked lists are bad — they're great for O(1) insert/delete
at known positions. But for iteration-heavy workloads, arrays win by a
landslide. This is why performance-critical code often uses arrays even when
the Big-O suggests a linked list would be "better."

---

## Common Pitfalls

1. **Forgetting to handle the empty list.** Always check if `head == NULL`
   before dereferencing.

2. **Losing nodes.** If you update `head` before saving the old head's `next`
   pointer, you've lost the rest of the list (memory leak).

3. **Not freeing memory.** Every `malloc` needs a matching `free`. When
   destroying a list, you must walk through and free each node.

4. **Off-by-one on traversal.** Walking with `curr != NULL` visits all nodes.
   Walking with `curr->next != NULL` stops one short of the end.

5. **Forgetting the edge case when deleting the head.** Deleting the first
   node requires updating the `head` pointer, not just rewiring `prev->next`.

---

## The Intersection Problem (Y-Shape)

Two singly linked lists can **merge** into one — they share a common tail. This
creates a Y shape:

```
  List A:  1 -> 2 -> 3 \
                         -> 6 -> 7 -> NULL
  List B:       4 -> 5 /
```

Node 6 is the **intersection** — it is literally the **same node in memory**,
not just a node with the same value. Both lists' `next` pointers converge at
this point, and from there they share every subsequent node.

### Why Pointer Comparison Matters

You might think: "just compare values." But two nodes can hold the same integer
and still be completely different allocations at different addresses. Intersection
means the **same address** — use `==` on pointers, not on data values.

### The Length-Difference Algorithm

The elegant O(n+m) solution uses no extra memory:

1. **Get the length** of both lists by walking each one to NULL.
2. **Compute the difference**: `diff = |len_a - len_b|`.
3. **Advance the longer list** by `diff` steps. Now both pointers are the same
   distance from the end.
4. **Walk both pointers forward** one step at a time. The first time they point
   to the **same node**, that is the intersection.

Step by step:

```
  A:  1 -> 2 -> 3 -> 6 -> 7 -> NULL    (length 5)
  B:       4 -> 5 -> 6 -> 7 -> NULL    (length 4)

  diff = 5 - 4 = 1
  Advance A by 1:  pA starts at node 2

  Step 1:  pA = 2,  pB = 4   (different nodes)
  Step 2:  pA = 3,  pB = 5   (different nodes)
  Step 3:  pA = 6,  pB = 6   (SAME NODE — found it!)
```

If you reach NULL without a match, the lists don't intersect.

**Time**: O(n + m) — two passes: one to measure, one to find.
**Space**: O(1) — only a few pointer variables.

---

## Advanced Linked List Patterns

These are classic interview problems that build on the basic operations above.

### Floyd's Cycle Detection — The Tortoise and Hare

Use two pointers: slow moves 1 step, fast moves 2 steps. If there is a cycle,
fast MUST eventually catch slow (pigeonhole principle — fast closes the gap by
1 each step, so it laps slow inside the cycle).

### Finding Where a Cycle Starts — The Distance Proof

Once you detect a cycle (slow and fast meet), you can find WHERE it starts:

```
  Head ──a──> Cycle start ──b──> Meeting point
                    ^                    |
                    └────── c ───────────┘

  When slow and fast meet:
    slow traveled: a + b
    fast traveled: a + b + (b + c) = a + 2b + c
    fast = 2 * slow:  a + 2b + c = 2(a + b)
    Simplify:  a = c

  So: distance from head to cycle start (a) =
      distance from meeting point to cycle start (c)
```

Reset one pointer to head, keep the other at the meeting point. Move both
one step at a time — they meet at the cycle start.

### Reverse In-Place — The Three-Pointer Technique

```
  prev=NULL   curr=1
     |          |
     v          v
  [NULL]   [1]→[2]→[3]→NULL

  Step 1: save next=2, point 1→NULL, advance prev=1, curr=2
  Step 2: save next=3, point 2→1, advance prev=2, curr=3
  Step 3: save next=NULL, point 3→2, advance prev=3, curr=NULL
  Done! Return prev (3).  List: 3→2→1→NULL
```

### Reverse K-Group — Chunk by Chunk

Check if k nodes remain. If yes, reverse exactly k nodes, then recursively
process the rest. The original head of each group becomes the tail — connect
it to the result of the recursive call.

### Palindrome Check — Find Middle, Reverse Half, Compare

1. Find the middle using slow/fast pointers
2. Reverse the second half of the list
3. Compare the first half with the reversed second half
4. Restore the list (reverse back) — good practice

### Complexity Table

```
┌─────────────────────────────┬────────────┬──────────┐
│ Operation                   │    Time    │  Space   │
├─────────────────────────────┼────────────┼──────────┤
│ Detect cycle (Floyd's)      │    O(n)    │   O(1)   │
│ Find cycle start            │    O(n)    │   O(1)   │
│ Reverse list                │    O(n)    │   O(1)   │
│ Reverse K-group             │    O(n)    │   O(n/k) │
│ Merge two sorted            │   O(n+m)   │   O(1)   │
│ Palindrome check            │    O(n)    │   O(1)   │
└─────────────────────────────┴────────────┴──────────┘
```

---

## Exercises

| # | File | Description |
|---|------|-------------|
| 1 | `exercises/linked_list.c` | Singly linked list library — 18 tests |
| 2 | `exercises/lru_cache.c` | LRU cache with doubly linked list — 10 tests |
| 3 | `exercises/list_intersection.c` | Y-shape intersection problem — 8 tests |
| 4 | `exercises/linked_list_advanced.c` | Advanced patterns: cycles, reverse, merge, palindrome — 20 tests |

---

## Debug Challenge

| File | Description | Bugs |
|------|-------------|------|
| `debug_list.c` | Find and fix 5 common linked list bugs | 5 |

These exercises contain **intentionally broken code**. Your job is to find and
fix each bug. Each function has a comment explaining what it SHOULD do and a
HINT about the bug class. Run the program — failing tests tell you which
functions are still broken.

```bash
make debug    # compile the buggy version
./exercises/debug_list   # see which tests fail
# ... fix bugs ...
# recompile and rerun until all tests pass
```

---

[← Previous: Module 10 — Function Pointers & Callbacks](../10-function-pointers-callbacks/README.md) | [Next: Module 12: Stacks & Queues →](../20-stacks-queues/README.md)
