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

## Exercises

| # | File | Description |
|---|------|-------------|
| 1 | `exercises/linked_list.c` | Singly linked list library — 18 tests |
| 2 | `exercises/lru_cache.c` | LRU cache with doubly linked list — 10 tests |
