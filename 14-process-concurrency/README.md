# Module 21: Processes & Concurrency

Up until now, every program you've written does one thing at a time: it starts,
runs your code top to bottom, and exits. But real systems do many things at once
--- your OS runs hundreds of processes, a web server handles thousands of
requests simultaneously, and your text editor checks spelling *while* you type.

This module teaches you how to create multiple flows of execution --- first with
**processes** (separate programs), then with **threads** (multiple flows inside
one program). Along the way you'll learn about the dangers of shared memory and
the tools (mutexes, condition variables) that keep concurrent code correct.

---

## What is a Process?

A **process** is a running program. When you type `./my_program`, the OS:

1. Loads your executable into memory
2. Sets up a **virtual address space** (stack, heap, code, data)
3. Assigns a **Process ID** (PID) --- a unique number
4. Starts executing from `main()`

Every process gets its own *isolated* memory. Process A cannot read or write
Process B's memory --- the OS enforces this with virtual memory hardware.

> **What is virtual memory?** Each process thinks it has the entire memory to
> itself --- its own addresses starting from 0, its own stack, its own heap.
> But these are **virtual addresses**, not real physical memory locations.
>
> The CPU has a component called the **MMU** (Memory Management Unit) that
> translates virtual addresses to physical addresses on every memory access.
> The OS sets up a **page table** for each process that tells the MMU: "when
> process A says address 0x4000, that is really physical address 0x8A000."
>
> This is how isolation works: process A and process B have completely separate
> page tables. If process A tries to access an address that belongs to process
> B, the MMU finds no valid mapping and triggers a **page fault** --- the OS
> kills the offending process (that is the "segmentation fault" you have seen).
>
> ```
>   Process A                        Process B
>   Virtual Addr    Physical RAM     Virtual Addr
>   ┌──────────┐   ┌──────────────┐  ┌──────────┐
>   │ 0x1000   │──▶│ 0x8A000      │  │ 0x1000   │──▶ 0x5C000
>   │ 0x2000   │──▶│ 0x3F000      │  │ 0x2000   │──▶ 0x91000
>   │ 0x3000   │──▶│ 0x12000      │  │ 0x3000   │──▶ 0x44000
>   └──────────┘   └──────────────┘  └──────────┘
>        │              ▲                  │
>        └── MMU + page table ─────────────┘
>   Same virtual address 0x1000 maps to different physical memory!
> ```

```
  Process A (PID 1234)          Process B (PID 5678)
  ┌──────────────────┐          ┌──────────────────┐
  │      Stack       │          │      Stack       │
  │        ↓         │          │        ↓         │
  │                  │          │                  │
  │        ↑         │          │        ↑         │
  │       Heap       │          │       Heap       │
  │──────────────────│          │──────────────────│
  │   Data (globals) │          │   Data (globals) │
  │──────────────────│          │──────────────────│
  │    Code (text)   │          │    Code (text)   │
  └──────────────────┘          └──────────────────┘
  Completely separate!          Cannot see Process A!
```

You can see running processes with `ps aux` or `top`. Each has a PID, memory
usage, CPU time, and state (running, sleeping, stopped).

---

## fork() — Copying a Process

The `fork()` system call creates an **exact copy** of the current process. After
`fork()`, there are two processes running the same code:

```c
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    printf("Before fork: PID = %d\n", getpid());

    pid_t result = fork();

    if (result < 0) {
        /* fork failed */
        perror("fork");
        return 1;
    } else if (result == 0) {
        /* Child process */
        printf("Child:  PID = %d, parent PID = %d\n", getpid(), getppid());
    } else {
        /* Parent process — result is the child's PID */
        printf("Parent: PID = %d, child PID = %d\n", getpid(), result);
    }

    return 0;
}
```

### How fork() works

```
  Before fork():
  ┌────────────────────┐
  │   Process (PID 100)│
  │   int x = 42;      │
  │   pid_t r = fork(); │──────────┐
  └────────────────────┘           │
                                   │ OS copies everything
                                   ▼
  After fork():
  ┌────────────────────┐    ┌────────────────────┐
  │ Parent (PID 100)   │    │  Child (PID 101)   │
  │ int x = 42;        │    │  int x = 42;       │
  │ r = 101 (child PID)│    │  r = 0             │
  └────────────────────┘    └────────────────────┘
       ▲                         ▲
       │                         │
    fork() returns           fork() returns
    child's PID              0 (means "I'm the child")
```

Key points:
- The **parent** gets the child's PID as the return value
- The **child** gets 0 as the return value
- Both processes continue from the *same point* (the line after `fork()`)
- Each has its own *copy* of all variables --- changes in one don't affect the other
- If `fork()` fails (system out of resources), it returns -1

### Under the Hood: What ls | grep .c Actually Does

When you type `ls | grep .c` in the shell, here's exactly what happens:

```
  Shell process (PID 100)
      │
      ├── pipe() → creates pipe [read_fd, write_fd]
      │
      ├── fork() → child A (PID 101) — will become 'ls'
      │    │
      │    ├── close(read_fd)          ← ls doesn't read from pipe
      │    ├── dup2(write_fd, STDOUT)  ← redirect stdout to pipe
      │    ├── close(write_fd)         ← original fd no longer needed
      │    └── exec("ls")             ← replace with ls program
      │
      ├── fork() → child B (PID 102) — will become 'grep'
      │    │
      │    ├── close(write_fd)         ← grep doesn't write to pipe
      │    ├── dup2(read_fd, STDIN)    ← redirect stdin from pipe
      │    ├── close(read_fd)
      │    └── exec("grep", ".c")     ← replace with grep program
      │
      ├── close(read_fd)              ← shell doesn't use the pipe
      ├── close(write_fd)
      ├── wait(PID 101)               ← wait for ls to finish
      └── wait(PID 102)               ← wait for grep to finish

  Data flow:
  ls writes filenames to pipe → grep reads from pipe, filters, prints matches
```

This is the Unix philosophy in action: small programs connected by pipes.
Every `|` in a shell command creates this exact pattern.

---

## exec() — Replacing a Process Image

`fork()` creates a copy, but the copy runs the *same code*. To run a
**different** program, use `exec()`:

```c
#include <unistd.h>

/* Replace this process with /bin/ls */
execlp("ls", "ls", "-la", NULL);

/* If exec returns, it failed */
perror("exec failed");
```

The `exec` family replaces the current process's code, data, and stack with a
new program. The PID stays the same --- it's the same process, just running
different code.

Common pattern: **fork + exec**

```c
pid_t pid = fork();
if (pid == 0) {
    /* Child: become a new program */
    execlp("ls", "ls", "-la", NULL);
    perror("exec");  /* only reached if exec fails */
    _exit(1);
} else {
    /* Parent: wait for child to finish */
    int status;
    waitpid(pid, &status, 0);
    printf("Child exited with status %d\n", WEXITSTATUS(status));
    /*
     * Why WEXITSTATUS(status) instead of just status?
     *
     * The status integer from wait()/waitpid() is NOT just the exit code.
     * The OS packs multiple pieces of information into that one int:
     *   - Did the child exit normally, or was it killed by a signal?
     *   - If it exited normally, what was the exit code? (upper 8 bits)
     *   - If it was killed, which signal killed it?
     *   - Was the child stopped (e.g., by SIGSTOP)?
     *
     * WIFEXITED(status)   — true if the child exited normally
     * WEXITSTATUS(status) — extracts the exit code (the value from return/exit)
     * WIFSIGNALED(status) — true if killed by a signal
     * WTERMSIG(status)    — which signal killed it
     */
}
```

---

## wait() and waitpid() — Waiting for Children

When a child process finishes, the parent should **wait** for it. This does two
things:

1. Gets the child's exit status
2. Cleans up the child's entry in the process table (prevents "zombie" processes)

```c
#include <sys/wait.h>

int status;
pid_t child = waitpid(-1, &status, 0);  /* wait for any child */

if (WIFEXITED(status)) {
    printf("Child %d exited with code %d\n", child, WEXITSTATUS(status));
}
```

---

## Signals — Interrupting Processes

**Signals** are asynchronous notifications sent to a process. Think of them as
software interrupts.

| Signal    | Number | Default Action | Meaning                        |
|-----------|--------|----------------|--------------------------------|
| SIGINT    | 2      | Terminate      | Ctrl+C from terminal           |
| SIGTERM   | 15     | Terminate      | Polite "please exit"           |
| SIGKILL   | 9      | Terminate      | Forced kill (cannot be caught) |
| SIGSEGV   | 11     | Core dump      | Segmentation fault             |
| SIGCHLD   | 17     | Ignore         | Child process exited           |
| SIGPIPE   | 13     | Terminate      | Write to broken pipe           |

You can install a **signal handler** to run custom code when a signal arrives:

```c
#include <signal.h>

volatile sig_atomic_t got_sigint = 0;
/*
 * volatile   — tells the compiler: "this variable can change at any time from
 *              outside normal code flow (like a signal handler), so do NOT
 *              optimize away reads of it." Without volatile, the compiler might
 *              see the while(!got_sigint) loop and think "got_sigint never
 *              changes in this loop, so I'll just read it once" — and your
 *              program would loop forever.
 *
 * sig_atomic_t — a type guaranteed to be read and written in a single CPU
 *              instruction (atomically). This matters because a signal can
 *              fire between ANY two instructions. If writing to a variable
 *              took two instructions and a signal fired in between, you'd
 *              get a half-written, corrupted value.
 */

void handle_sigint(int sig)
{
    (void)sig;           /* suppress unused warning */
    got_sigint = 1;      /* set a flag — keep handlers simple! */
}

int main(void)
{
    signal(SIGINT, handle_sigint);

    while (!got_sigint) {
        printf("Working... (Ctrl+C to stop)\n");
        sleep(1);
    }
    printf("\nCaught SIGINT, cleaning up...\n");
    return 0;
}
```

Rules for signal handlers:
- Keep them **short** --- just set a flag
- Only use **async-signal-safe** functions (no `printf`, no `malloc`)
- Use `volatile sig_atomic_t` for shared flags
- SIGKILL and SIGSTOP **cannot** be caught

---

## Pipes — Communication Between Processes

Processes have isolated memory, so how do they communicate? One way is
**pipes**: a one-way data channel.

```c
int fd[2];
pipe(fd);
/*  fd[0] = read end
 *  fd[1] = write end  */
```

```
  ┌──────────────┐          pipe           ┌──────────────┐
  │   Process A  │     ┌────────────┐      │   Process B  │
  │              │     │            │      │              │
  │  write(fd[1])│────▶│  fd[0]────▶│─────▶│  read(fd[0]) │
  │              │     │   buffer   │      │              │
  └──────────────┘     └────────────┘      └──────────────┘
     (producer)        OS-managed buffer      (consumer)
```

Typical pattern: fork, then parent writes and child reads (or vice versa):

```c
int fd[2];
pipe(fd);

pid_t pid = fork();
if (pid == 0) {
    /* Child reads from pipe */
    close(fd[1]);           /* close write end */
    char buf[256];
    int n = read(fd[0], buf, sizeof(buf) - 1);
    buf[n] = '\0';
    printf("Child received: %s\n", buf);
    close(fd[0]);
    _exit(0);
} else {
    /* Parent writes to pipe */
    close(fd[0]);           /* close read end */
    const char *msg = "Hello from parent!";
    write(fd[1], msg, strlen(msg));
    close(fd[1]);
    wait(NULL);
}
```

When you write `ls | grep .c` in the shell, the shell creates a pipe, forks
twice, connects `ls`'s stdout to the pipe's write end, and `grep`'s stdin to
the read end.

---

## Threads — Lightweight Concurrency

A **thread** is a flow of execution *within* a process. Unlike processes,
threads **share** the same memory space:

```
  Process (PID 100)
  ┌──────────────────────────────────────┐
  │           Shared Memory              │
  │  ┌──────────────────────────────┐    │
  │  │  Heap (malloc'd data)        │    │
  │  │  Global variables            │    │
  │  │  Code (text segment)         │    │
  │  └──────────────────────────────┘    │
  │                                      │
  │  ┌──────────┐  ┌──────────┐  ┌──────────┐
  │  │ Thread 1 │  │ Thread 2 │  │ Thread 3 │
  │  │ (main)   │  │          │  │          │
  │  │          │  │          │  │          │
  │  │ Stack    │  │ Stack    │  │ Stack    │
  │  │ Registers│  │ Registers│  │ Registers│
  │  │ PC       │  │ PC       │  │ PC       │
  │  └──────────┘  └──────────┘  └──────────┘
  │   Each thread has its own stack       │
  │   but shares everything else          │
  └──────────────────────────────────────┘
```

### Processes vs Threads

| Feature           | Process                | Thread                  |
|-------------------|------------------------|-------------------------|
| Memory            | Separate (isolated)    | Shared                  |
| Creation cost     | Heavy (copy everything)| Light (just new stack)  |
| Communication     | Pipes, sockets, files  | Shared variables        |
| Crash isolation   | One crash, others live | One crash, all die      |
| Synchronization   | Not usually needed     | Mutexes, condvars, etc. |

Threads are cheaper but **dangerous** --- shared memory means you need careful
synchronization.

---

## POSIX Threads (pthreads)

The C API for threads on Unix systems:

```c
#include <pthread.h>

void *worker(void *arg)
{
    int id = *(int *)arg;
    printf("Thread %d working\n", id);
    return NULL;
}

int main(void)
{
    pthread_t threads[4];
    int ids[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);  /* wait for each thread */
    }

    return 0;
}
```

Compile with `-pthread` flag: `gcc -pthread my_program.c`

Key functions:
- `pthread_create(thread, attr, start_routine, arg)` --- start a new thread
- `pthread_join(thread, retval)` --- wait for a thread to finish
- `pthread_exit(retval)` --- exit current thread (without killing process)

### Thread Lifecycle

A thread goes through a simple set of states during its life:

```
  Thread Lifecycle:

  Created ──→ Running ──→ Terminated
                ↑   ↓
                └── Blocked (waiting for mutex/condition/I/O)

  join() blocks the CALLING thread until the target thread terminates.
  detach() lets the thread run independently (no join needed, but
  you can't get its return value).
```

- `pthread_join(thread, retval)` --- the calling thread **blocks** until `thread`
  finishes. You get back the return value from the thread function.
- `pthread_detach(thread)` --- marks the thread as "detached". When it finishes,
  its resources are automatically cleaned up. You cannot `join` a detached thread.

Use `join` when you need the thread's result or want to wait for it. Use `detach`
for "fire and forget" background work (like logging).

---

## Race Conditions

When two threads access the same data and at least one is writing, you have a
**race condition**. The result depends on unpredictable timing:

```c
int counter = 0;  /* shared between threads */

void *increment(void *arg)
{
    (void)arg;
    for (int i = 0; i < 1000000; i++) {
        counter++;  /* NOT atomic! This is: load, add 1, store */
    }
    return NULL;
}
```

If two threads run this, you'd expect `counter = 2000000`. But you'll get
something less, because `counter++` is actually three steps:

```
  Thread A                    Thread B
  ────────                    ────────
  load counter (= 0)
                              load counter (= 0)
  add 1 → 1
                              add 1 → 1
  store 1
                              store 1     ← LOST UPDATE!
  counter is 1, not 2!
```

---

## Mutex — Mutual Exclusion Lock

A **mutex** ensures only one thread enters a **critical section** at a time:

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void *increment(void *arg)
{
    (void)arg;
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);    /* acquire — blocks if another holds it */
        counter++;                     /* critical section — safe! */
        pthread_mutex_unlock(&lock);  /* release — let others in */
    }
    return NULL;
}
```

```
  Thread A                        Thread B
  ────────                        ────────
  mutex_lock(lock) → acquired
  counter++ (safe)                mutex_lock(lock) → BLOCKED
  mutex_unlock(lock)              (unblocked, acquires lock)
                                  counter++ (safe)
                                  mutex_unlock(lock)
```

Rules:
- Always unlock what you lock (in the same thread)
- Keep critical sections **short**
- Lock in a consistent order to avoid deadlock

---

## Atomic Operations

Mutexes work but they're heavy --- lock, do work, unlock. For simple operations
like incrementing a counter, there's a lighter approach: **atomic operations**.

Atomic operations are CPU instructions that complete in **one step** --- no other
thread can interrupt them. The CPU hardware guarantees this. No lock needed.

C11 provides `<stdatomic.h>` for portable atomic operations:

```c
#include <stdatomic.h>

atomic_int counter = 0;

/* Thread-safe increment — no mutex needed! */
atomic_fetch_add(&counter, 1);

/* Thread-safe read */
int value = atomic_load(&counter);
```

> **Note:** `<stdatomic.h>` is C11, not C99. If you're compiling with `-std=c99`,
> you can use GCC built-ins instead: `__atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST)`.
> The concepts are the same --- only the spelling differs.

### Compare-And-Swap (CAS)

CAS is the foundation of **lock-free programming**. It atomically checks a value
and updates it only if it hasn't changed since you last read it:

```
CAS(location, expected, new_value):
  atomically:
    if *location == expected:
      *location = new_value
      return true (success)
    else:
      return false (someone else changed it)
```

CAS lets you update a value **only if no one else changed it** since you last
read it. If it fails, you retry with the new value. Here's the C11 version:

```c
atomic_int counter = 0;

/* Increment using CAS — retry if another thread changed it */
int old = atomic_load(&counter);
while (!atomic_compare_exchange_strong(&counter, &old, old + 1)) {
    /* old is updated to the current value, so we retry with it */
}
```

### When to Use Atomics vs Mutexes

```
Simple counter/flag  → atomic (faster, no blocking)
Multiple related updates → mutex (need to keep them consistent)
Complex data structure → mutex (atomics can't protect multiple fields)
```

If you need to update a single integer or a single pointer, atomics are the right
tool. If you need to update two variables that must stay in sync (like a linked
list's head pointer and its size counter), use a mutex.

---

## Condition Variables — Waiting for Events

A **condition variable** lets a thread **sleep** until some condition becomes
true. It's always used with a mutex.

The classic use case is **producer-consumer**:

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t  not_full  = PTHREAD_COND_INITIALIZER;

int buffer[10];
int count = 0;

void *producer(void *arg)
{
    (void)arg;
    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&lock);
        while (count == 10) {            /* buffer full? wait */
            pthread_cond_wait(&not_full, &lock);
        }
        buffer[count++] = i;            /* produce an item */
        pthread_cond_signal(&not_empty); /* wake a consumer */
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
```

Key: `pthread_cond_wait` **atomically** unlocks the mutex and sleeps. When
signaled, it re-locks the mutex before returning. Always use `while` (not `if`)
to check the condition --- spurious wakeups can happen.

---

## Producer-Consumer Pattern

The producer-consumer pattern is one of the most common concurrency patterns. One
thread **produces** data, another **consumes** it, with a shared queue in between:

```
┌──────────┐     ┌─────────────────┐     ┌──────────┐
│ Producer │────→│  Message Queue  │────→│ Consumer │
│ Thread   │     │ (mutex-locked)  │     │ Thread   │
└──────────┘     └─────────────────┘     └──────────┘
```

The queue is just an array protected by a mutex + condition variable. The flow
looks like this:

- **Producer**: lock mutex -> add to queue -> signal condition -> unlock
- **Consumer**: lock mutex -> while (queue empty) wait on condition -> remove from queue -> unlock

```c
#define QUEUE_SIZE 16

typedef struct {
    int items[QUEUE_SIZE];
    int head, tail, count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} message_queue_t;

void queue_push(message_queue_t *q, int item)
{
    pthread_mutex_lock(&q->lock);
    while (q->count == QUEUE_SIZE) {          /* full? wait */
        pthread_cond_wait(&q->not_full, &q->lock);
    }
    q->items[q->tail] = item;
    q->tail = (q->tail + 1) % QUEUE_SIZE;    /* circular buffer */
    q->count++;
    pthread_cond_signal(&q->not_empty);       /* wake a consumer */
    pthread_mutex_unlock(&q->lock);
}

int queue_pop(message_queue_t *q)
{
    pthread_mutex_lock(&q->lock);
    while (q->count == 0) {                   /* empty? wait */
        pthread_cond_wait(&q->not_empty, &q->lock);
    }
    int item = q->items[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    pthread_cond_signal(&q->not_full);        /* wake a producer */
    pthread_mutex_unlock(&q->lock);
    return item;
}
```

This pattern appears everywhere: web servers queue incoming requests, databases
queue transactions, and operating systems queue I/O operations. The earlier
condition variable example above shows the same idea in a simpler form --- this
version wraps it into a reusable data structure.

---

## Concurrency vs Parallelism

These are related but distinct concepts:

- **Concurrency**: multiple tasks make progress over time (could be interleaved
  on one core)
- **Parallelism**: multiple tasks execute *simultaneously* on multiple cores

```
  Concurrency (1 core):
  ──────────────────────────────────────▶ time
  Thread A: ███░░░███░░░███
  Thread B: ░░░███░░░███░░░
  (interleaved — OS switches between them)

  Parallelism (2 cores):
  ──────────────────────────────────────▶ time
  Core 1: Thread A ██████████████████
  Core 2: Thread B ██████████████████
  (truly simultaneous)
```

With pthreads on a multi-core machine, you get **both** --- the OS schedules
threads across cores.

---

## Deadlock and Starvation

### Deadlock

**Deadlock** occurs when two or more threads are each waiting for the other to
release a resource:

```
  ┌──────────────┐              ┌──────────────┐
  │   Thread A   │              │   Thread B   │
  │              │              │              │
  │ holds lock_1 │──── wants ──▶│ holds lock_2 │
  │              │◀── wants ────│              │
  │ waits lock_2 │              │ waits lock_1 │
  └──────────────┘              └──────────────┘
       ▲                              │
       │        DEADLOCK CYCLE        │
       └──────────────────────────────┘
```

```c
/* Thread A */                  /* Thread B */
pthread_mutex_lock(&lock_1);   pthread_mutex_lock(&lock_2);
pthread_mutex_lock(&lock_2);   pthread_mutex_lock(&lock_1);
/* DEADLOCK — both block forever! */
```

**Prevention**: always lock mutexes in the **same order** across all threads.

### Starvation

**Starvation** is when a thread never gets to run because other threads keep
acquiring the resources it needs. Unlike deadlock, the system is making progress
--- just not fairly.

---

## Summary

| Concept            | Key Idea                                          |
|--------------------|---------------------------------------------------|
| Process            | Running program with isolated memory              |
| fork()             | Copy current process; returns PID or 0            |
| exec()             | Replace process image with new program            |
| wait()/waitpid()   | Wait for child, get exit status, prevent zombies  |
| Signals            | Async notifications (SIGINT, SIGTERM, SIGKILL)    |
| Pipes              | One-way byte stream between processes             |
| Thread             | Lightweight execution within a process            |
| join/detach        | Wait for thread vs fire-and-forget                |
| Mutex              | Lock for exclusive access to shared data          |
| Atomic operations  | CPU-level indivisible ops; lighter than mutex      |
| CAS                | Update only if unchanged; basis of lock-free code |
| Condition Variable | Sleep until condition, used with mutex             |
| Producer-Consumer  | Queue between threads: produce, signal, consume   |
| Deadlock           | Circular wait on locks --- all threads stuck       |

---

## Exercises

1. **producer_consumer.c** --- Build a thread-safe bounded buffer using mutex
   and condition variables. Implement `buffer_put` (blocks if full) and
   `buffer_get` (blocks if empty).

2. **parallel_sum.c** --- Sum a large array in parallel using multiple threads.
   Split the work into chunks, each thread sums its chunk, then combine.

---

## Debug Challenge

| File | Description | Bugs |
|------|-------------|------|
| `debug_concurrency.c` | Find and fix 4 concurrency bugs (races, deadlocks) | 4 |

These exercises contain **intentionally broken code**. Your job is to find and
fix each bug. Each function has a comment explaining what it SHOULD do and a
HINT about the bug class. Run the program — failing tests tell you which
functions are still broken.

```bash
make debug    # compile the buggy version
./exercises/debug_concurrency   # see which tests fail
# ... fix bugs ...
# recompile and rerun until all tests pass
```

---

[← Previous: Module 20 — Graphs](../29-graphs/README.md) | [Next: Module 22 — Building a Text Editor →](../27-build-text-editor/README.md)
