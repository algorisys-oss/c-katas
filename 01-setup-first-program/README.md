# Module 01 — Setup & First Program

You are about to write your first C program. By the end of this module, you will
understand exactly what happens between typing code and seeing output on your
screen — no magic, no hand-waving, every step explained.

---

## 1. Installing Your Tools

You need three programs. Open a terminal and run:

```bash
# On Ubuntu/Debian:
sudo apt update
sudo apt install gcc make valgrind

# Verify they installed:
gcc --version       # The compiler — turns your C code into a program
make --version      # The build tool — automates compiling for you
valgrind --version  # The memory checker — finds bugs you can't see
```

What each tool does:

| Tool       | Job                                                        |
|------------|------------------------------------------------------------|
| `gcc`      | Translates C source code into an executable program        |
| `make`     | Reads a `Makefile` and runs the right compile commands     |
| `valgrind` | Runs your program and reports memory errors (we will use this a lot later) |

---

## 2. Hello World — The Ritual

Every programmer's first program prints "Hello, world!" to the screen.
Here is the complete program:

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, world!\n");
    return 0;
}
```

Three commands to go from text file to running program:

```
$ gcc -Wall -Wextra -pedantic -std=c99 -o hello hello.c
$ ./hello
Hello, world!
```

That is the ritual: **write, compile, run.** You will do it thousands of times.

---

## 3. Anatomy of a C Program

Let's break that tiny program apart. Every single piece matters:

```
    What it does
    |
    v
+-- #include <stdio.h> ---- "Copy the contents of stdio.h into this file."
|                            stdio.h declares printf so the compiler knows
|                            what printf looks like.
|
+-- int main(void) -------- "This is where the program starts."
|   {                        int   = main returns an integer to the OS
|   |                        main  = special name; the OS calls this function
|   |                        void  = this function takes no arguments
|   |                        {     = start of main's body
|   |
|   +-- printf("Hello, world!\n");
|   |   |       |              |
|   |   |       |              +-- \n = newline character (press Enter)
|   |   |       +-- the string to print
|   |   +-- a function from stdio.h that prints formatted text
|   |
|   +-- return 0; ---------- "Tell the OS: everything went fine."
|   |                        0 = success. Any other number = something went wrong.
|   |                        The OS can check this value.
|   |
|   +-- } ------------------ end of main's body
|
+-- ; ---------------------- Every statement ends with a semicolon.
                             Think of it like a period at the end of a sentence.
```

**Key rules:**
- Every C program must have a `main` function. It is the entry point.
- `#include` lines go at the top. They pull in declarations you need.
- Braces `{ }` group statements together into a block.
- Every statement ends with `;` — forget one and the compiler will yell at you.
- `return 0;` at the end of main tells the operating system "I finished successfully."

---

## 4. What Happens When You Compile?

When you run `gcc -o hello hello.c`, four separate steps happen in sequence.
Most people treat this as a black box. You will not.

```
  YOUR CODE                                    RUNNING PROGRAM
  hello.c                                      ./hello
     |                                              ^
     v                                              |
+----------------------------------------------------+
|                COMPILATION PIPELINE                 |
|                                                     |
|  +-------------+    +-----------+    +-----------+  |
|  |             |    |           |    |           |  |
|  | PREPROCESSOR|--->| COMPILER  |--->| ASSEMBLER |  |
|  |             |    |           |    |           |  |
|  +-------------+    +-----------+    +-----------+  |
|    hello.c           hello.i          hello.s       |
|       |                 |                |          |
|       v                 v                v          |
|  "Copy #include    "Translate C      "Turn assembly |
|   files in,         into assembly     into binary   |
|   replace           (human-readable   machine code" |
|   #defines"         machine           hello.o       |
|                      instructions)"       |         |
|                                           v         |
|                                     +-----------+   |
|                                     |           |   |
|                                     |  LINKER   |   |
|                                     |           |   |
|                                     +-----------+   |
|                                          |          |
|                                          v          |
|                                       hello         |
|                                    (executable)     |
+-----------------------------------------------------+
```

Let's walk through each step.

### Step 1: Preprocessor (text substitution)

```
gcc -E hello.c -o hello.i
```

The preprocessor is a **text editor**, not a compiler. It does not understand C.
It just follows simple rules:

- `#include <stdio.h>` --> Find the file `stdio.h` and paste its entire
  contents right here (thousands of lines of declarations).
- `#define PI 3.14` --> Every time you see `PI`, replace it with `3.14`.

The output `hello.i` is still C code, just with all the includes expanded
and all the defines substituted. Open it — you will see it is huge because
stdio.h pulls in a lot of declarations.

### Step 2: Compiler (C --> assembly)

```
gcc -S hello.c -o hello.s
```

Now the actual compiler reads the C code and translates it into **assembly
language** — human-readable instructions that map closely to what the CPU
actually does.

Your hello world might produce something like this (simplified x86-64):

```asm
    .section .rodata
.LC0:
    .string "Hello, world!"     # Your string, stored in read-only data

    .text
main:
    pushq   %rbp                # Save the old base pointer
    movq    %rsp, %rbp          # Set up new stack frame
    leaq    .LC0(%rip), %rdi    # Load address of "Hello, world!" into 1st argument
    call    puts                # Call the puts function (simpler than printf here)
    movl    $0, %eax            # Put 0 in the return register
    popq    %rbp                # Restore old base pointer
    ret                         # Return to whoever called main
```

You do not need to memorize this. Just notice: each line is one simple
instruction. The CPU executes them one at a time, top to bottom.

### Step 3: Assembler (assembly --> object file)

```
gcc -c hello.c -o hello.o
```

The assembler translates assembly into **machine code** — raw binary numbers
that the CPU can execute. This is an "object file." It contains your compiled
code, but it is not a complete program yet because it references things
(like `printf`) that live in other files.

If you try to read `hello.o` in a text editor, you will see garbage — it is
binary, not text. That is expected.

### Step 4: Linker (object files --> executable)

```
gcc hello.o -o hello
```

The linker's job: **connect the dots.** Your code calls `printf`, but `printf`
lives in the C standard library (libc). The linker finds `printf`'s code and
wires your call to it.

```
  Your code (hello.o)         C library (libc)
  +------------------+        +------------------+
  | main:            |        | printf:          |
  |   ...            |        |   ...            |
  |   call printf ---+------->|   (actual code)  |
  |   ...            |        |   ...            |
  +------------------+        +------------------+
          |                            |
          +------- LINKER combines ----+
                       |
                       v
              hello (executable)
              +------------------+
              | main:            |
              |   ...            |
              |   call printf    |
              |   ...            |
              | printf:          |
              |   (actual code)  |
              +------------------+
```

**Summary of the four stages:**

```
  +-----------+     +----------+     +-----------+     +--------+
  |           |     |          |     |           |     |        |
  | hello.c   |---->| hello.i  |---->| hello.s   |---->| hello.o|----> hello
  |           |     |          |     |           |     |        |  (executable)
  | C source  |     | Expanded |     | Assembly  |     | Object |
  |           |     | C source |     | language  |     | file   |
  +-----------+     +----------+     +-----------+     +--------+
   (text you        (still text,     (human-readable   (binary,
    write)          much bigger)      CPU instructions)  not runnable
                                                        alone)
```

### Under the Hood: What Happens When You Run ./hello

When you type `./hello` and press Enter, an incredible chain of events occurs:

1. **Shell reads your command** — bash parses "./hello" as "run this file"
2. **fork()** — the shell creates a copy of itself (a new process)
3. **exec()** — the new process replaces itself with your program
4. **Loader** — the OS reads the ELF binary, maps code and data into memory
5. **Dynamic linker** — resolves shared library references (libc, etc.)
6. **_start** — the C runtime startup code runs BEFORE main()
7. **main()** — YOUR code finally executes
8. **printf** — writes to a buffer, which eventually calls write(1, ...)
9. **write()** — system call transfers bytes to the terminal driver
10. **Terminal** — displays the characters on your screen
11. **exit()** — your process terminates, OS reclaims all memory

All of this happens in under a millisecond.

---

## 5. printf — Formatted Printing

`printf` is the function you will use most in your first weeks of C. It takes
a **format string** and fills in values where you put **format specifiers**.

```c
printf("format string with %specifiers", value1, value2, ...);
```

### Format Specifiers

| Specifier | What it prints               | Example                          |
|-----------|------------------------------|----------------------------------|
| `%d`      | Integer (decimal)            | `printf("%d", 42)` --> `42`      |
| `%s`      | String (text)                | `printf("%s", "hi")` --> `hi`    |
| `%f`      | Float (decimal number)       | `printf("%f", 3.14)` --> `3.140000` |
| `%x`      | Integer (hexadecimal)        | `printf("%x", 255)` --> `ff`     |
| `%c`      | Single character             | `printf("%c", 'A')` --> `A`      |
| `%%`      | A literal percent sign       | `printf("100%%")` --> `100%`     |

### How printf works, step by step

```c
printf("Name: %s, Age: %d\n", "Alice", 17);
```

```
  Format string: "Name: %s, Age: %d\n"
                         |          |
                         v          v
                       "Alice"     17

  printf walks through the format string left to right:
    'N' 'a' 'm' 'e' ':' ' '  --> print these as-is
    '%s'                       --> grab next argument ("Alice"), print it
    ',' ' ' 'A' 'g' 'e' ':' ' '  --> print as-is
    '%d'                       --> grab next argument (17), print it
    '\n'                       --> print a newline

  Output: Name: Alice, Age: 17
```

**Warning:** If you put `%d` but pass a string, or get the number of arguments
wrong, C will NOT warn you at runtime. It will just print garbage or crash.
The compiler flags `-Wall -Wextra` help catch these mistakes.

---

## 6. stdin, stdout, stderr — The Three Streams

Every program that runs on your computer automatically gets three connections
to the outside world. You do not create them — the operating system sets them
up before your program starts.

```
                    +---------------------+
   KEYBOARD         |                     |         SCREEN
   (typing)         |   YOUR PROGRAM      |        (normal output)
       |            |                     |            ^
       v            |                     |            |
  +---------+       |   int main(void)    |       +---------+
  |  stdin  |------>|   {                 |------>| stdout  |
  | (0)     |       |       ...           |       | (1)     |
  +---------+       |   }                 |       +---------+
                    |                     |
                    |                     |       +---------+
                    |                     |------>| stderr  |
                    |                     |       | (2)     |
                    +---------------------+       +---------+
                                                       |
                                                       v
                                                    SCREEN
                                                   (error output)
```

| Stream   | Number | Direction | What it is for                        |
|----------|--------|-----------|---------------------------------------|
| `stdin`  | 0      | Input     | Reading input (keyboard by default)   |
| `stdout` | 1      | Output    | Normal output (`printf` writes here)  |
| `stderr` | 2      | Output    | Error messages (separate from stdout) |

**Why two outputs?** So you can separate normal results from errors. Example:

```bash
# Send normal output to a file, but still see errors on screen:
./my_program > output.txt
# stdout goes to output.txt, stderr still prints to your terminal
```

When you write `printf("hello")`, it writes to **stdout**.
When you write `fprintf(stderr, "error!")`, it writes to **stderr**.

---

## 7. Your First Makefile

Typing `gcc -Wall -Wextra -pedantic -std=c99 -o hello hello.c` every time
gets old fast. A **Makefile** automates this.

A Makefile is a recipe book. Each recipe (called a **rule**) has three parts:

```
target: dependencies
	command
```

```
  +-- target ---------- The thing you want to build (the output file)
  |
  +-- dependencies ---- The files needed to build it (the input files)
  |
  +-- command ---------- How to build it (the shell command to run)
                         MUST be indented with a TAB character, not spaces!
```

Here is the simplest useful Makefile:

```makefile
hello: hello.c
	gcc -Wall -Wextra -pedantic -std=c99 -o hello hello.c
```

Now you just type `make hello` (or just `make`, since it runs the first rule
by default) and it compiles for you.

**The rule says:** "To build `hello`, I need `hello.c`. If `hello.c` is
newer than `hello` (or `hello` does not exist), run the gcc command."

This is make's superpower: it only recompiles when something changed.

### A Makefile with a clean target

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

clean:
	rm -f hello
```

- `CC` and `CFLAGS` are **variables**. `$(CC)` expands to `gcc`.
- `clean` has no dependencies — it always runs. It deletes the compiled program.
- Run `make clean` to remove compiled files and start fresh.

---

## 8. The Compiler Flags Explained

We always compile with `-Wall -Wextra -pedantic -std=c99`. Here is why:

| Flag         | What it does                                                  |
|--------------|---------------------------------------------------------------|
| `-Wall`      | Enable **a**ll common warnings (not literally all, but most)  |
| `-Wextra`    | Enable even more warnings beyond -Wall                        |
| `-pedantic`  | Warn about anything that is not strict C99 standard           |
| `-std=c99`   | Use the C99 version of the language                           |
| `-o name`    | Name the output file (without this, you get `a.out`)          |

Warnings are your friend. They catch bugs before you even run the program.
**Never ignore warnings.** Fix every single one.

---

## Katas

Time to practice. The exercises are in the `exercises/` directory:

1. **hello.c** — Write the classic hello world program from scratch.
2. **about_me.c** — Use printf with multiple format specifiers to print info
   about yourself (%d, %s, %f, %x).
3. **explore_compilation.sh** — Run each compilation stage (gcc -E, -S, -c)
   and observe what each one produces.

When you are stuck, reference solutions are in `solutions/`.

Build all exercises with `make` from this directory. Run `make clean` to
start fresh.

---

[← Previous: Module 00 — How a Computer Works](../00-how-computers-work/README.md) | [Next: Module 02 — Types, Variables & Operators →](../02-types-variables-operators/README.md)
