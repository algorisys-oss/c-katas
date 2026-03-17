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
| Mutex              | Lock for exclusive access to shared data          |
| Condition Variable | Sleep until condition, used with mutex             |
| Deadlock           | Circular wait on locks --- all threads stuck       |

---

## Exercises

1. **producer_consumer.c** --- Build a thread-safe bounded buffer using mutex
   and condition variables. Implement `buffer_put` (blocks if full) and
   `buffer_get` (blocks if empty).

2. **parallel_sum.c** --- Sum a large array in parallel using multiple threads.
   Split the work into chunks, each thread sums its chunk, then combine.
