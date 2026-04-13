# Module 30 — Capstone Project

You made it. Every module before this one taught you a piece of the puzzle:
types, pointers, memory management, data structures, file I/O, processes,
networking, parsing, graphics. Now you put it all together and build something
real.

A capstone project is different from a kata. There is no single right answer.
You choose the project, design the architecture, write the code, test it, and
debug it. This module gives you five project ideas, a planning template, and
one guided exercise (a mini shell) to get you started.

---

## How to Pick a Project

Pick based on what excites you, not what seems easiest. Every project below
will stretch you. That is the point.

| # | Project | Difficulty | Key Modules |
|---|---------|-----------|-------------|
| 1 | Mini Shell | ★★★☆☆ | 06, 16, 21 |
| 2 | HTTP File Server | ★★★★☆ | 16, 17, 26 |
| 3 | Markdown to HTML | ★★★☆☆ | 06, 16, 25 |
| 4 | Image Filter Pipeline | ★★★★☆ | 07, 16, 28, 28b |
| 5 | Mini Database with SQL | ★★★★★ | 07, 13, 24, 25 |

---

## Project 1: Mini Shell

Build a Unix shell that can:
- Read a command line and parse it into arguments
- Execute external programs with `fork()` + `execvp()`
- Handle pipes (`ls | grep .c`)
- Support built-in commands: `cd`, `exit`, `help`
- Handle I/O redirection (`>`, `<`) as a stretch goal

### Architecture

```
┌─────────────────────────────────────────┐
│                main loop                │
│  ┌─────────┐   ┌────────┐   ┌───────┐  │
│  │  read   │──>│ parse  │──>│execute│  │
│  │  line   │   │  line  │   │command│  │
│  └─────────┘   └────────┘   └───────┘  │
│                     │                   │
│              ┌──────┴──────┐            │
│              │             │            │
│          builtin?      external         │
│          (cd/exit)    fork + exec       │
│                        │                │
│                   has pipe?             │
│                   ┌────┴────┐           │
│                  no        yes          │
│                  │     pipe + fork×2    │
│              simple        │            │
│              exec     connect fds       │
└─────────────────────────────────────────┘
```

### Suggested Build Order

Don't try to build everything at once!

1. **Phase 1**: Read a line, split it into words, print them (parsing only)
2. **Phase 2**: Execute a single command (fork + exec)
3. **Phase 3**: Add built-in commands (cd, exit, pwd)
4. **Phase 4**: Add pipes (cmd1 | cmd2)
5. **Phase 5**: Add I/O redirection (> file, < file)
6. **Phase 6**: Add background processes (cmd &)

### Key Concepts from Earlier Modules

- **Module 06 (Pointers & Arrays)**: argv is an array of char pointers
- **Module 16 (File I/O)**: file descriptors, `dup2()` for redirection
- **Module 21 (Processes)**: `fork()`, `execvp()`, `waitpid()`, `pipe()`

### Building the Full Shell (Beyond the Exercise)

The exercise file (`exercises/mini_shell.c`) covers the parsing logic because
that is the part we can test with our harness. To build the full shell, add
these pieces after completing the exercise:

**Step 1: The REPL loop**
```c
int main(void)
{
    char line[1024];
    char *args[64];
    for (;;) {
        printf("mysh> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        /* strip newline */
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0') continue;
        int argc = shell_parse_line(line, args, 64);
        if (argc == 0) continue;
        if (strcmp(args[0], "exit") == 0) break;
        /* ... execute ... */
    }
    return 0;
}
```

**Step 2: Execute a simple command**
```c
void exec_simple(char *args[])
{
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
}
```

**Step 3: Execute a pipeline**
```c
void exec_pipeline(char *left[], char *right[])
{
    int fd[2];
    pipe(fd);
    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(left[0], left);
        _exit(127);
    }
    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(right[0], right);
        _exit(127);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
}
```

---

## Project 2: HTTP File Server

Serve files from a directory over HTTP/1.1. A browser connects, requests a
file, and your server sends it back.

### What You Build

- Listen on a TCP port (Module 26)
- Parse HTTP request lines: `GET /path HTTP/1.1`
- Map the path to a local file (Module 16)
- Send the file with correct `Content-Type` and `Content-Length` headers
- Handle 404 for missing files
- Stretch: directory listing, multiple concurrent clients with `fork()`

### Architecture

```
Browser                      Your Server
  │                              │
  │──── GET /index.html ────────>│
  │                              │── open("./www/index.html")
  │                              │── read file contents
  │<──── HTTP/1.1 200 OK ───────│
  │      Content-Type: text/html │
  │      Content-Length: 1234    │
  │      <html>...</html>        │
  │                              │
  │──── GET /missing.txt ───────>│
  │<──── HTTP/1.1 404 Not Found─│
```

### Key Modules

- **Module 16 (File I/O)**: reading files, stat for file size
- **Module 17 (Text & Encoding)**: parsing request strings
- **Module 26 (Sockets)**: socket, bind, listen, accept, send

---

## Project 3: Markdown to HTML Converter

Read a Markdown file and output valid HTML. Start simple and add features.

### Feature Ladder

1. **Paragraphs**: text separated by blank lines becomes `<p>` tags
2. **Headings**: `# Heading` becomes `<h1>`, `##` becomes `<h2>`, etc.
3. **Bold/italic**: `**bold**` becomes `<strong>`, `*italic*` becomes `<em>`
4. **Code blocks**: triple backtick blocks become `<pre><code>`
5. **Links**: `[text](url)` becomes `<a href="url">text</a>`
6. **Lists**: `- item` becomes `<ul><li>`
7. Stretch: nested lists, images, tables

### Architecture

```
Input Markdown             Output HTML
┌──────────────┐          ┌──────────────────┐
│ # Hello      │          │ <h1>Hello</h1>   │
│              │   ───>   │ <p>This is a     │
│ This is a    │  parse   │ paragraph.</p>   │
│ paragraph.   │  +emit   │                  │
│              │          │ <ul>             │
│ - item 1     │          │ <li>item 1</li>  │
│ - item 2     │          │ <li>item 2</li>  │
└──────────────┘          │ </ul>            │
                          └──────────────────┘
```

### Key Modules

- **Module 06 (Pointers & Arrays)**: string processing
- **Module 16 (File I/O)**: read input, write output
- **Module 25 (Parsing)**: tokenizer/parser techniques

---

## Project 4: Image Filter Pipeline

Read a PPM image, apply a chain of filters, and write the result. Filters are
specified on the command line:

```
./imgpipe input.ppm grayscale blur sharpen output.ppm
```

### Filters to Implement

| Filter | Algorithm |
|--------|-----------|
| Grayscale | Average RGB or luminance formula |
| Blur | 3x3 box filter (average neighbors) |
| Sharpen | 3x3 sharpen kernel |
| Edge detect | Sobel operator |
| Invert | 255 - channel |
| Sepia | Weighted RGB transform |
| Brightness | Add/subtract from all channels |

### Architecture

```
┌───────┐   ┌──────────┐   ┌──────┐   ┌─────────┐   ┌───────┐
│ read  │──>│grayscale │──>│ blur │──>│ sharpen │──>│ write │
│  PPM  │   │  filter  │   │filter│   │  filter │   │  PPM  │
└───────┘   └──────────┘   └──────┘   └─────────┘   └───────┘
     Each filter reads a pixel buffer and writes a new one.
     Chain them: output of one is input to the next.
```

### Key Modules

- **Module 07 (Dynamic Memory)**: allocate pixel buffers
- **Module 16 (File I/O)**: PPM file reading/writing
- **Module 28 (Graphics)**: pixel manipulation
- **Module 28b (Image Effects)**: convolution kernels

---

## Project 5: Mini Database with SQL

Build a database that stores tables on disk, indexes them with a B-Tree, and
accepts a subset of SQL.

### Supported SQL

```sql
CREATE TABLE users (id INT, name TEXT, age INT);
INSERT INTO users VALUES (1, 'Alice', 30);
SELECT * FROM users;
SELECT name, age FROM users WHERE age > 25;
```

### Architecture

```
SQL String
    │
    ▼
┌─────────┐   ┌────────┐   ┌──────────┐   ┌─────────┐
│Tokenizer│──>│ Parser │──>│ Executor │──>│ Storage │
│         │   │  (AST) │   │          │   │ (B-Tree │
│         │   │        │   │          │   │  + File)│
└─────────┘   └────────┘   └──────────┘   └─────────┘

Storage Layer:
┌──────────────────────────────────────┐
│           B-Tree Index               │
│      ┌───┐                           │
│      │ 5 │                           │
│     ╱     ╲                          │
│  ┌───┐   ┌───┐                       │
│  │2,3│   │7,9│                       │
│  └───┘   └───┘                       │
│   Leaf nodes point to row data       │
│   stored in a flat file.             │
└──────────────────────────────────────┘
```

### Suggested Build Order

This is the most ambitious project. Build it in layers:

1. **Phase 1**: In-memory table with INSERT and SELECT (no WHERE clause)
2. **Phase 2**: Add WHERE clause filtering
3. **Phase 3**: Add persistence (save/load table to/from disk)
4. **Phase 4**: Replace linear scan with B-Tree index
5. **Phase 5**: Add CREATE TABLE and DROP TABLE

### Key Modules

- **Module 07 (Dynamic Memory)**: managing dynamic structures
- **Module 13 (Hash Tables)**: symbol tables in the parser
- **Module 24 (B-Tree/KV Store)**: on-disk indexing
- **Module 25 (Parsing/SQL)**: tokenizer, parser, AST

---

## Project Planning Template

Before writing code, fill this out. Seriously. Even one page of planning saves
hours of rewriting.

### 1. Requirements

What does the program do? Write 5-10 bullet points. Be specific.

```
Example (Mini Shell):
- Read commands from stdin, one per line
- Parse command into program name and arguments
- Execute programs using fork/exec
- Support pipes: cmd1 | cmd2
- Built-in commands: cd, exit, help
- Print an error if the command is not found
- Handle Ctrl-C without exiting the shell (stretch)
```

### 2. Data Structures

What structs do you need? What arrays? Draw them.

```
Example (Mini Shell):
- char line[1024]     — raw input line
- char *args[64]      — parsed argument pointers
- No structs needed for the basic version
```

### 3. Module Breakdown

Split your code into files. Each file does one thing.

```
Example (Mini Shell):
- main.c       — REPL loop
- parse.c      — shell_parse_line, shell_find_pipe
- execute.c    — fork/exec logic, pipe handling
- builtin.c    — cd, exit, help
- shell.h      — shared declarations
```

### 4. Implementation Plan

Order matters. Build the simplest working version first, then add features.

```
Example (Mini Shell):
1. Parse a line into args (this is the exercise)
2. REPL loop that prints parsed args (verify parsing works)
3. fork + execvp for simple commands
4. Built-in: exit, cd
5. Pipe detection and execution
6. Error handling and edge cases
7. Stretch: I/O redirection, Ctrl-C handling
```

### 5. Testing Strategy

How will you know it works?

```
Example (Mini Shell):
- Unit tests for parsing (the exercise file)
- Manual testing: run ls, cat, echo
- Pipe test: ls | wc -l
- Edge cases: empty input, unknown command, cd with no args
- Compare output with /bin/bash for the same commands
```

#### Testing Examples by Project

**Mini Shell:**
- Compare output with `/bin/bash` for 20 commands
- Test edge cases: empty input, spaces only, very long commands
- Test pipes: `echo hello | wc -c` should output 6

**HTTP Server:**
- `curl http://localhost:8080/` should return index.html
- `curl http://localhost:8080/nonexistent` should return 404
- Test with `ab` (Apache Bench) for concurrent requests

**Markdown to HTML:**
- Create test.md files with known output
- `diff` your output against expected HTML
- Test nested formatting: `**bold *italic* bold**`

---

## The Exercise

The `exercises/mini_shell.c` file implements the parsing layer of a mini shell.
This is the testable core: splitting a command line into arguments, finding
pipes, and detecting built-in commands.

After completing the exercise, use the code snippets above to build the full
shell with `fork()`, `exec()`, and `pipe()`. That part cannot be tested with
our simple harness (it creates real processes), but the parsing foundation you
build here is what makes the full shell work.

Good luck. You have earned this.

---

[← Previous: Module 29: Machine Learning in C](../37-machine-learning-in-c/README.md)
