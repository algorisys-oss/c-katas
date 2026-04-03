# Module 15: BUILD — Mini Shell

> "A shell is just a loop that reads commands and runs programs.
> When you build one, you understand how Linux *actually* runs programs."

You already know the pieces:
- **File I/O** (module 13): reading/writing files, file descriptors, `dup2`
- **Processes** (module 14): `fork`, `exec`, `waitpid`, pipes, signals

Now you put them all together into something real: **your own Unix shell**.

---

## How a Shell Works — From First Principles

Every shell — bash, zsh, fish — does the same thing at its core:

```
┌──────────────────────────────────────────────────┐
│                  THE SHELL LOOP                  │
│                                                  │
│  while (1) {                                     │
│      1. Print prompt        ("$ ")               │
│      2. Read a line         (fgets from stdin)    │
│      3. Tokenize            ("ls -la" → ["ls",   │
│                               "-la"])             │
│      4. Is it a builtin?                         │
│         YES → execute directly (cd, exit, pwd)   │
│         NO  → fork + exec + wait                 │
│      5. Go back to step 1                        │
│  }                                               │
└──────────────────────────────────────────────────┘
```

That's it. Five steps, repeated forever. Let's break each one down.

---

## Step 1-2: The Prompt and Reading Input

The simplest possible shell:

```c
char line[1024];

while (1) {
    printf("$ ");            /* 1. Print prompt */
    if (!fgets(line, sizeof(line), stdin)) {
        break;               /* EOF (Ctrl+D) — exit */
    }
    /* ... parse and execute line ... */
}
```

`fgets` reads one line from stdin (including the newline `\n`).
If the user presses Ctrl+D (EOF), `fgets` returns NULL and we exit.

**Why fgets and not scanf?** Because `scanf("%s", ...)` stops at whitespace.
We need the *entire line* — `ls -la /home` is one command with arguments.

---

## Step 3: Tokenizing — Splitting Input into Words

The user types: `ls -la /home`

We need to split that into an array of strings (tokens):

```
Input:  "ls  -la   /home\n"
                                    ┌──────┐
Tokens: tokens[0] ──────────────────│ "ls" │
                                    └──────┘
                                    ┌───────┐
        tokens[1] ──────────────────│ "-la" │
                                    └───────┘
                                    ┌─────────┐
        tokens[2] ──────────────────│ "/home" │
                                    └─────────┘
        tokens[3] ──────────────────  NULL
```

The NULL at the end is required by `execvp` — it needs to know where the
argument list ends.

### Tokenizing Rules

1. **Whitespace** (spaces, tabs, newlines) separates tokens
2. **Multiple spaces** between tokens are ignored
3. **Quoted strings** are one token: `echo "hello world"` → `["echo", "hello world"]`
4. **Special characters** are their own tokens: `|`, `>`, `<`, `>>`

### Handling Quoted Strings

```
Input: echo "hello world" foo

Walk through character by character:
  'e' 'c' 'h' 'o'  → token "echo"
  ' '               → delimiter (end of token)
  '"'               → start of quoted string
  'h' 'e' ... 'd'   → inside quotes, spaces don't split
  '"'               → end of quoted string → token "hello world"
  ' '               → delimiter
  'f' 'o' 'o'      → token "foo"
```

---

## Step 4: Builtins vs External Commands

### Why Builtins Must Run In the Shell Process

Some commands **cannot** be run as external programs. The most important
example is `cd`:

```
What happens if cd were external:

    Shell (PID 100, cwd = /home)
       │
       ├── fork() → Child (PID 101, cwd = /home)
       │                │
       │                ├── chdir("/tmp")
       │                │   (Child's cwd is now /tmp)
       │                │
       │                └── exit
       │
       └── waitpid(101)
           Shell's cwd is STILL /home!
           The child changed its OWN directory, not the parent's.
```

Each process has its **own** current working directory. When the child
calls `chdir()`, it only changes the child's directory. The parent (the
shell) is unaffected. That's why `cd` must call `chdir()` directly in
the shell process — it's a **builtin**.

### Common Builtins

| Command | What it does              | Why it must be builtin         |
|---------|---------------------------|--------------------------------|
| `cd`    | Change directory          | Must change shell's own cwd    |
| `exit`  | Exit the shell            | Must terminate the shell itself |
| `pwd`   | Print working directory   | Could be external, but faster  |

### Implementing Builtins

```c
int execute_builtin(char **tokens)
{
    if (strcmp(tokens[0], "cd") == 0) {
        if (tokens[1] == NULL) {
            chdir(getenv("HOME"));  /* cd with no args → go home */
        } else {
            if (chdir(tokens[1]) != 0) {
                perror("cd");
            }
        }
        return 1;  /* handled */
    }
    if (strcmp(tokens[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(tokens[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd))) {
            printf("%s\n", cwd);
        }
        return 1;
    }
    return 0;  /* not a builtin */
}
```

---

## Step 5: The Fork + Exec Pattern

For external commands, the shell does three things:

```
┌─────────────────────────────────────────────────────────────────┐
│                     FORK + EXEC + WAIT                         │
│                                                                 │
│  Shell (parent)                                                 │
│    │                                                            │
│    ├── fork() ──────────────────┐                               │
│    │                            │                               │
│    │  Parent:                   │  Child:                       │
│    │  waitpid(child_pid)        │  execvp("ls", ["ls","-la"])  │
│    │  (blocked, waiting...)     │  ┌─────────────────────┐     │
│    │                            │  │ Child process is     │     │
│    │                            │  │ REPLACED by "ls"     │     │
│    │                            │  │ program. execvp      │     │
│    │                            │  │ never returns        │     │
│    │                            │  │ (on success).        │     │
│    │                            │  └─────────────────────┘     │
│    │                            │         │                     │
│    │                            │     ls prints output          │
│    │                            │         │                     │
│    │                            │     ls exits                  │
│    │                            │                               │
│    ├── waitpid returns ←────────┘                               │
│    │   (child finished)                                         │
│    │                                                            │
│    └── print next prompt                                        │
└─────────────────────────────────────────────────────────────────┘
```

Key insight: `execvp` **replaces** the child process with the new program.
The child's code after `execvp` only runs if `execvp` fails (e.g., command
not found).

```c
pid_t pid = fork();
if (pid == 0) {
    /* Child process */
    execvp(tokens[0], tokens);
    /* If we get here, execvp failed */
    perror(tokens[0]);
    exit(1);
} else {
    /* Parent process */
    int status;
    waitpid(pid, &status, 0);
}
```

**Why fork before exec?** Because `exec` replaces the current process. If
the shell called `exec` directly, it would be gone. Fork creates a copy
that we can safely replace.

---

## Pipes — Connecting Commands

`ls | grep .c` means: take the output of `ls` and feed it as input to `grep`.

### How a Pipe Works

```
┌─────────────────────────────────────────────────────────────┐
│                     PIPE INTERNALS                          │
│                                                             │
│  pipe(fd)  creates:                                         │
│                                                             │
│     fd[1] (write end)          fd[0] (read end)             │
│         │                          ▲                        │
│         ▼                          │                        │
│    ┌──────────────────────────────────┐                     │
│    │          kernel buffer           │                     │
│    │  (bytes flow left to right →)    │                     │
│    └──────────────────────────────────┘                     │
│                                                             │
│  Process A writes to fd[1]  → data → Process B reads fd[0] │
└─────────────────────────────────────────────────────────────┘
```

### Implementing `ls | grep .c`

Step by step:

```
1. Create the pipe:        pipe(fd)    → fd[0] = read end, fd[1] = write end

2. Fork child 1 (ls):
   ┌──────────────────────────────────┐
   │  close(fd[0])     ← don't need  │    ls doesn't read from the pipe.
   │  dup2(fd[1], 1)   ← stdout → pipe│    stdout (fd 1) now goes to pipe.
   │  close(fd[1])     ← cleanup     │    We used dup2, so fd[1] is a spare.
   │  execvp("ls", ...)              │
   └──────────────────────────────────┘

3. Fork child 2 (grep):
   ┌──────────────────────────────────┐
   │  close(fd[1])     ← don't need  │    grep doesn't write to the pipe.
   │  dup2(fd[0], 0)   ← stdin ← pipe│    stdin (fd 0) now reads from pipe.
   │  close(fd[0])     ← cleanup     │
   │  execvp("grep", ...)            │
   └──────────────────────────────────┘

4. Parent:
   close(fd[0])        ← parent doesn't use either end
   close(fd[1])
   waitpid(child1)     ← wait for both
   waitpid(child2)
```

**Critical rule**: Always close pipe ends you don't use. If the parent
doesn't close `fd[1]`, the reader (grep) will never see EOF because there's
still an open write end. It will hang forever.

---

## I/O Redirection

Redirection reuses the same trick as pipes: `dup2`.

### Output Redirect: `ls > files.txt`

```
┌──────────────────────────────────┐
│  Fork child:                     │
│    int fd = open("files.txt",    │
│       O_WRONLY | O_CREAT | O_TRUNC, 0644);   │
│    dup2(fd, 1);   ← stdout → file│
│    close(fd);                    │
│    execvp("ls", ...);            │
└──────────────────────────────────┘
```

`dup2(fd, 1)` makes file descriptor 1 (stdout) point to the file.
When `ls` writes to stdout, it goes to `files.txt`.

### Append Redirect: `echo hi >> log.txt`

Same, but use `O_APPEND` instead of `O_TRUNC`:
```c
int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
```

### Input Redirect: `sort < data.txt`

```c
int fd = open("data.txt", O_RDONLY);
dup2(fd, 0);   /* stdin ← file */
close(fd);
execvp("sort", ...);
```

---

## Signal Handling — Ctrl+C

When you press Ctrl+C, the kernel sends `SIGINT` to the **foreground
process group**. Without signal handling, Ctrl+C would kill the shell.

What we want:
- Ctrl+C should kill the **child** process (the running command)
- Ctrl+C should **not** kill the shell itself

```c
/* In the shell (parent): ignore SIGINT */
signal(SIGINT, SIG_IGN);

/* In the child (after fork, before exec): restore default */
signal(SIGINT, SIG_DFL);
```

The child inherits the parent's signal handlers across `fork`, but `exec`
resets them to default (for security). Still, it's good practice to
explicitly set `SIG_DFL` in the child before `exec`.

---

## Putting It All Together

Here's the architecture of the mini shell you'll build:

```
┌─────────────────────────────────────────────────────┐
│                    mini_shell.c                      │
│                                                      │
│  main()                                              │
│    │                                                 │
│    ├── signal(SIGINT, SIG_IGN)    ← protect shell    │
│    │                                                 │
│    └── while (1):                                    │
│          │                                           │
│          ├── print_prompt()       ← "$ "             │
│          │                                           │
│          ├── fgets(line)          ← read input       │
│          │                                           │
│          ├── tokenize(line, tokens)                  │
│          │     │                                     │
│          │     └── handles: whitespace, quotes,      │
│          │         pipes, redirects                   │
│          │                                           │
│          ├── has pipe? ──── YES ──→ execute_pipe()   │
│          │                             │             │
│          │                             ├── pipe()    │
│          │                             ├── fork x2   │
│          │                             ├── dup2      │
│          │                             └── waitpid   │
│          │                                           │
│          ├── is builtin? ─ YES ──→ execute_builtin() │
│          │                             │             │
│          │                             ├── cd        │
│          │                             ├── pwd       │
│          │                             └── exit      │
│          │                                           │
│          └── else ───────────────→ execute_cmd()     │
│                                        │             │
│                                        ├── fork      │
│                                        ├── handle    │
│                                        │   redirects │
│                                        ├── execvp    │
│                                        └── waitpid   │
└─────────────────────────────────────────────────────┘
```

---

## Exercises

| # | File               | What You Build                              |
|---|--------------------|---------------------------------------------|
| 1 | `tokenizer.c`      | Command-line tokenizer with quote handling  |
| 2 | `mini_shell.c`     | Complete working shell with pipes & redirects|

Build the tokenizer first — the shell depends on it.

---

## Key Concepts Checklist

After completing this module, you should be able to explain:

- [ ] Why the shell is just a read-parse-execute loop
- [ ] Why `cd` must be a builtin (cannot run in a child process)
- [ ] How `fork + exec + wait` runs an external command
- [ ] Why `execvp` never returns on success
- [ ] How `pipe()` + `dup2()` connects two processes
- [ ] Why you must close unused pipe ends
- [ ] How `>`, `<`, and `>>` work using `dup2` and `open`
- [ ] Why the shell ignores SIGINT but children don't
