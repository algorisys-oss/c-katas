# Module 06 — Build CLI Tools: Your First Real Programs

You have learned variables, control flow, and functions. Now you build something
real. This module is about the Unix philosophy of small, composable tools — and
you will write four of them from scratch.

By the end of this module you will be able to:
- Parse command-line arguments (argc/argv)
- Read input from files or stdin (line by line)
- Write output to stdout and errors to stderr
- Use exit codes to signal success or failure
- Build tools that work together via pipes

---

## 1. The Unix Philosophy

In the 1970s, the creators of Unix settled on a powerful idea:

> Write programs that do one thing and do it well.
> Write programs to work together.
> Write programs that handle text streams, because that is a universal interface.

Instead of one giant program that does everything, you build small tools and
**compose** them. The glue that holds them together is the **pipe** (`|`).

You already use this every day without thinking about it:

```
cat file.txt | grep "error" | wc -l
```

This says: read a file, keep only lines containing "error", count those lines.
Three tiny tools, combined into something powerful.

---

## 2. The Three Standard Streams

Every process on a Unix system is born with three open data streams:

```
                    ┌─────────────────┐
   stdin (0)  ───►  │                 │  ───►  stdout (1)
  (keyboard         │   Your Program  │        (terminal
   or pipe)         │                 │         or pipe)
                    │                 │  ───►  stderr (2)
                    └─────────────────┘        (terminal,
                                                always)
```

- **stdin** (file descriptor 0) — where your program reads input.
  By default it is the keyboard. When you pipe data in, stdin comes from the
  previous program's stdout.

- **stdout** (file descriptor 1) — where your program writes normal output.
  By default it goes to the terminal. When you pipe data out, stdout goes to
  the next program's stdin.

- **stderr** (file descriptor 2) — where your program writes error messages.
  This ALWAYS goes to the terminal, even when stdout is piped. This is how
  error messages do not corrupt your data pipeline.

In C, these are pre-opened for you:

```c
#include <stdio.h>

fprintf(stdout, "normal output\n");   /* same as printf(...) */
fprintf(stderr, "error: bad input\n");
fgets(buf, sizeof(buf), stdin);       /* read a line from stdin */
```

### How Pipes Connect Processes

When you write `cat file | grep pattern | wc -l`, the shell creates:

```
  ┌──────────┐  pipe   ┌──────────┐  pipe   ┌──────────┐
  │   cat     │ ──────► │   grep   │ ──────► │    wc    │
  │          1│→fd[1]   │0←    →1  │→fd[1]   │0←        │
  └──────────┘         └──────────┘         └──────────┘
   reads file    stdout of cat         stdout of grep
                 becomes stdin         becomes stdin
                 of grep               of wc

  Data flows left to right, one line at a time.
```

Each `|` creates a pipe: a small kernel buffer that connects the stdout of one
process to the stdin of the next. The operating system handles the plumbing —
your programs just read from stdin and write to stdout.

---

## 3. Command-Line Arguments: argc and argv

When you run `./mini_wc -l myfile.txt`, the shell splits the command into words
and passes them to your program through two parameters:

```c
int main(int argc, char *argv[])
```

- `argc` — the count of arguments (including the program name). Here: 3.
- `argv` — an array of strings (char pointers), one per argument.

```
  argc = 3

  argv:
  ┌─────────┐     ┌──────────────────┐
  │ argv[0] ─┼────►│ "./mini_wc\0"    │
  ├─────────┤     └──────────────────┘
  │ argv[1] ─┼────►│ "-l\0"           │
  ├─────────┤     └──────────────────┘
  │ argv[2] ─┼────►│ "myfile.txt\0"   │
  ├─────────┤     └──────────────────┘
  │ argv[3] ─┼────► NULL
  └─────────┘
```

Key facts:
- `argv[0]` is always the program name
- `argv[argc]` is always NULL (guaranteed by the C standard)
- Each `argv[i]` is a null-terminated string
- Flags (like `-l`, `-n 5`) are just strings — you parse them yourself

### Parsing Flags — The Simple Way

For our small tools, we parse flags with a loop:

```c
int show_lines = 0;
int i = 1;  /* skip argv[0] */

while (i < argc && argv[i][0] == '-') {
    if (strcmp(argv[i], "-l") == 0) {
        show_lines = 1;
    } else if (strcmp(argv[i], "-n") == 0) {
        i++;  /* next arg is the number */
        count = atoi(argv[i]);
    } else {
        fprintf(stderr, "Unknown flag: %s\n", argv[i]);
        return 1;
    }
    i++;
}
/* argv[i] is now the first non-flag argument (the filename) */
```

This is exactly how real Unix tools parse arguments. (Later you will learn about
`getopt`, but understanding the manual approach first is important.)

---

## 4. Exit Codes

Every program returns a number to the shell when it finishes:

- **0** means success
- **1** means general error
- **2** means usage/argument error (convention)

The shell stores this in the special variable `$?`:

```bash
$ grep "hello" file.txt
$ echo $?
0              # found it — success

$ grep "zzzzz" file.txt
$ echo $?
1              # not found — failure
```

In C, you return from main or call `exit()`:

```c
return 0;         /* success */
return 1;         /* error */
exit(EXIT_FAILURE); /* same as return 1, but works from any function */
```

Exit codes are how pipes and shell scripts know if a step succeeded. The `&&`
operator runs the next command only if the previous one succeeded (exit 0):

```bash
gcc -o prog prog.c && ./prog    # only runs prog if compilation succeeded
```

---

## 5. File I/O Basics

To read from a file instead of stdin, you open it with `fopen`:

```c
FILE *fp = fopen("data.txt", "r");  /* "r" = read mode */
if (fp == NULL) {
    fprintf(stderr, "Error: cannot open data.txt\n");
    return 1;
}

char line[1024];
while (fgets(line, sizeof(line), fp) != NULL) {
    /* process each line */
    printf("%s", line);
}

fclose(fp);  /* always close what you open */
```

The beautiful thing: `fgets` works the same way whether `fp` is a file you
opened or `stdin`. So you can write one loop that handles both:

```c
FILE *input = stdin;  /* default: read from keyboard/pipe */

if (filename != NULL) {
    input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "Error: cannot open %s\n", filename);
        return 1;
    }
}

/* This loop works for both stdin and files — same code! */
char line[1024];
while (fgets(line, sizeof(line), input) != NULL) {
    process(line);
}

if (input != stdin) {
    fclose(input);
}
```

This pattern — "default to stdin, open file if given" — is used in almost every
Unix tool. You will use it in every exercise.

---

## 6. The Line-Processing Pattern

Almost every text-processing tool follows the same skeleton:

```
  ┌──────────────────┐
  │   Parse args      │  ← which flags? which file?
  └────────┬─────────┘
           ▼
  ┌──────────────────┐
  │   Open input      │  ← file or stdin
  └────────┬─────────┘
           ▼
  ┌──────────────────┐
  │   Read line       │◄─┐
  │   Process line    │   │  ← the core loop
  │   Write output    │   │
  │   More lines? ────┼───┘
  └────────┬─────────┘
           ▼
  ┌──────────────────┐
  │   Clean up        │  ← close files, print summary
  └──────────────────┘
```

You will implement this pattern four times in this module, and each time it
will feel more natural.

---

## Exercises

Build these four tools, in order. Each one teaches a new aspect of the pattern:

| Exercise       | What You Learn                              |
|----------------|---------------------------------------------|
| `mini_wc.c`   | Counting, basic arg parsing, the core loop  |
| `mini_grep.c`  | String matching, multiple flags, -n/-i/-c   |
| `mini_head.c`  | Numeric flag parsing, early exit from loop   |
| `mini_uniq.c`  | Comparing consecutive lines, state tracking  |

Build with:
```bash
cd exercises
make all        # compile everything
make test       # run all tests
./mini_wc file  # test manually
```

After completing all four, try piping them together:
```bash
cat some_file.txt | ./mini_grep "pattern" | ./mini_wc
echo -e "a\na\nb\nb\nb\nc" | ./mini_uniq -c
./mini_head -n 5 bigfile.txt | ./mini_grep "error" -n
```

That is the Unix philosophy in action — your tools, composing together.
