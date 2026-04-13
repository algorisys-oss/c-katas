# Module 16: File I/O & the Unix Philosophy

## What Is a File?

Forget everything you think you know about files. At the lowest level, a file is
just a **sequence of bytes** stored on disk. That's it. There's no inherent
"type" — a `.txt` file and a `.jpg` file are both just bytes. The *program*
reading those bytes decides what they mean.

```
A file on disk:

  Byte 0   Byte 1   Byte 2   Byte 3   Byte 4   Byte 5   ...
┌────────┬────────┬────────┬────────┬────────┬────────┬─────
│  0x48  │  0x65  │  0x6C  │  0x6C  │  0x6F  │  0x0A  │ ...
└────────┴────────┴────────┴────────┴────────┴────────┴─────
    H        e        l        l        o       \n

Same bytes, different interpretation:
  - A text editor shows: "Hello\n"
  - A hex editor shows: 48 65 6C 6C 6F 0A
  - An image viewer says: "not a valid image"
```

## Understanding the Operating System (Quick Primer)

Before we talk about files, you need to understand a few OS concepts that
everything else builds on. These ideas come up again in Modules 21, 26, and
beyond, so learning them now will pay off.

### What is the Kernel?

The **kernel** is the core of your operating system (Linux, macOS, Windows).
It is the one program that runs with full access to the hardware — CPU, memory,
disk, network. Every other program (your C code, your browser, your editor)
runs in **user space** with restricted access.

```
  ┌─────────────────────────────────────────────────┐
  │                  User Space                      │
  │  ┌──────────┐  ┌──────────┐  ┌──────────┐      │
  │  │ Your     │  │ Browser  │  │ Editor   │      │
  │  │ Program  │  │          │  │          │      │
  │  └────┬─────┘  └────┬─────┘  └────┬─────┘      │
  │       │              │              │            │
  │───────┼──────────────┼──────────────┼────────────│
  │       ↓              ↓              ↓            │
  │  ┌────────────────────────────────────────┐      │
  │  │              KERNEL                    │      │
  │  │  - Manages memory (who gets what)      │      │
  │  │  - Manages files (read/write disk)     │      │
  │  │  - Manages processes (who runs when)   │      │
  │  │  - Manages devices (keyboard, screen)  │      │
  │  └────────────────────────────────────────┘      │
  │                      │                           │
  │──────────────────────┼───────────────────────────│
  │                      ↓                           │
  │  ┌────────────────────────────────────────┐      │
  │  │           HARDWARE                     │      │
  │  │   CPU  │  RAM  │  Disk  │  Network     │      │
  │  └────────────────────────────────────────┘      │
  └─────────────────────────────────────────────────┘
```

### What is a System Call?

Your program cannot touch the disk directly — only the kernel can. So when
your code calls `open()`, `read()`, or `write()`, it makes a **system call**
(or "syscall"): a request to the kernel to do something on its behalf.

Think of it like a restaurant: you (user program) give your order to the
waiter (system call), who takes it to the kitchen (kernel). You never walk
into the kitchen yourself.

### What is a Process?

A **process** is a running program. Each process gets:
- Its own **memory space** (it cannot see other programs' memory)
- Its own **file descriptor table** (its own list of open files)
- A **Process ID** (PID) — a unique number assigned by the kernel

When you run `./my_program`, the kernel creates a new process for it.
We'll cover processes deeply in Module 21.

### What is an Inode?

On disk, the kernel tracks each file using a data structure called an
**inode** (index node). An inode stores the file's metadata:

```
  Inode #42:
  ┌───────────────────────────────┐
  │ Size: 1,024 bytes             │
  │ Owner: you                    │
  │ Permissions: rw-r--r--        │
  │ Created: 2026-01-15 09:30    │
  │ Data blocks: [102, 103, 107]  │  ← actual byte locations on disk
  └───────────────────────────────┘
```

The *name* of the file (like `data.txt`) is NOT stored in the inode — it's
stored in the directory, which maps names to inode numbers. This is why you
can have multiple names (hard links) pointing to the same file.

---

## File Descriptors — How Programs Talk to Files

When your program opens a file, the operating system doesn't give you the file
directly. Instead, it gives you a small integer called a **file descriptor** (fd).
This number is an index into a per-process table that the kernel maintains.

Every process starts with three file descriptors already open:

```
  Your Process                         Kernel
 ┌──────────────┐       ┌───────────────────────────────────┐
 │              │       │  File Descriptor Table (per-proc)  │
 │  fd = 0 ─────┼──────>│  [0] ──> stdin  (keyboard)        │
 │  fd = 1 ─────┼──────>│  [1] ──> stdout (terminal)        │
 │  fd = 2 ─────┼──────>│  [2] ──> stderr (terminal)        │
 │              │       │  [3] ──> (empty)                   │
 │  open("f")   │       │  [4] ──> (empty)                   │
 │  returns 3 ──┼──────>│  [3] ──> /home/you/f.txt ───┐     │
 │              │       └──────────────────────────────┼─────┘
 └──────────────┘                                      │
                                                       v
                                               ┌──────────────┐
                                               │  Disk/FS     │
                                               │  inode, data │
                                               │  blocks ...  │
                                               └──────────────┘
```

When you call `open()`, the kernel:
1. Finds the file on disk (follows the path through directories)
2. Creates an entry in the process's file descriptor table
3. Returns the fd number (the lowest available slot — here, 3)

When you call `close(fd)`, that slot is freed.

## Low-Level I/O: `open`, `read`, `write`, `close`

These are the **system calls** — they talk directly to the kernel. They use file
descriptors (integers), not `FILE *` pointers.

```c
#include <fcntl.h>    /* open, O_RDONLY, O_WRONLY, O_CREAT */
#include <unistd.h>   /* read, write, close */

/* Open a file for reading */
int fd = open("data.txt", O_RDONLY);
if (fd == -1) {
    perror("open");  /* prints: "open: No such file or directory" */
    return 1;
}

/* Read up to 100 bytes */
char buf[100];
ssize_t n = read(fd, buf, sizeof(buf));
/* n = number of bytes actually read, or -1 on error, or 0 at EOF */

/* Write to stdout (fd 1) */
write(1, buf, n);

/* Always close when done */
close(fd);
```

**Key insight**: `read` and `write` work with raw bytes. They don't know about
lines, strings, or null terminators. You get exactly what's on disk.

### Why Low-Level I/O Matters

- It's what *everything else* is built on — `fopen`/`fread` use these underneath
- You need it for: sockets, pipes, device files, non-blocking I/O
- It gives you full control over buffering

## Buffered I/O: `fopen`, `fread`, `fwrite`, `fprintf`

Raw `read`/`write` system calls are expensive — each one traps into the kernel.
Reading a file one byte at a time with `read` would be painfully slow.

The C standard library wraps file descriptors in a `FILE *` struct that adds a
**buffer** — typically 4096 or 8192 bytes. When you call `fgetc()`, it doesn't
do a system call for one byte. It reads a big chunk into the buffer, then serves
bytes from that buffer until it's empty.

```
  Buffering Layers

  Your Program                    C Library                   Kernel
 ┌─────────────┐     ┌──────────────────────────┐     ┌──────────────┐
 │             │     │  FILE struct              │     │              │
 │  fgetc(f)   │────>│  ┌──────────────────────┐ │     │              │
 │  fgets(f,..)│     │  │ Buffer (4096 bytes)  │ │     │              │
 │  fprintf(f) │     │  │ [H][e][l][l][o][\n]  │ │     │              │
 │             │     │  │  ^                   │ │     │              │
 │  "give me   │     │  │  read pointer        │ │     │  Disk blocks │
 │   1 byte"   │     │  └──────────────────────┘ │     │  (actual     │
 │             │     │                            │     │   file data) │
 │             │     │  When buffer is empty:     │     │              │
 │             │     │    read(fd, buf, 4096) ────┼────>│              │
 │             │     │  fills the whole buffer    │     │              │
 │             │     └──────────────────────────┘ │     └──────────────┘
 └─────────────┘
```

### Under the Hood: The Journey of a printf

When you call `printf("Hello\n")`, the bytes travel through multiple layers:

```
  Your code:    printf("Hello\n")
      │
      ▼
  stdio buffer: [H][e][l][l][o][\n]     ← buffered in user space
      │                                    (flushed on \n or fflush)
      ▼
  write(1, "Hello\n", 6)                ← system call to kernel
      │
      ▼
  Kernel buffer: [H][e][l][l][o][\n]    ← kernel's internal buffer
      │
      ▼
  Terminal driver                        ← interprets escape codes
      │
      ▼
  Screen                                 ← pixels appear!
```

Why all the buffering? System calls are expensive (~1000 ns each).
Buffering combines many small writes into fewer large ones. That's
why printf doesn't always appear immediately — it's waiting in the
stdio buffer. Use fflush(stdout) to force it out.

### Opening and Closing Files

```c
#include <stdio.h>

FILE *f = fopen("data.txt", "r");   /* "r" = read, "w" = write, "a" = append */
if (f == NULL) {
    perror("fopen");
    return 1;
}

/* ... use the file ... */

fclose(f);  /* flushes buffer, closes fd */
```

### Reading Text

```c
/* Read one character */
int ch = fgetc(f);  /* returns int, not char! (EOF is -1) */

/* Read a line (up to size-1 chars + null terminator) */
char line[256];
fgets(line, sizeof(line), f);  /* includes the '\n' if present */

/* Read formatted data */
int age;
char name[50];
fscanf(f, "%s %d", name, &age);
```

### Writing Text

```c
FILE *out = fopen("output.txt", "w");

fputc('A', out);                       /* write one char */
fputs("Hello, world!\n", out);         /* write a string */
fprintf(out, "x = %d\n", 42);         /* formatted output */

fclose(out);  /* IMPORTANT: flushes buffer to disk */
```

### Binary I/O: `fread` and `fwrite`

For non-text data (structs, images, databases), use `fread`/`fwrite`:

```c
/* Write a struct to a file */
typedef struct {
    int id;
    char name[32];
    double score;
} Record;

Record r = {1, "Alice", 95.5};
FILE *f = fopen("records.bin", "wb");   /* "wb" = write binary */
fwrite(&r, sizeof(Record), 1, f);      /* write 1 record */
fclose(f);

/* Read it back */
Record loaded;
FILE *g = fopen("records.bin", "rb");   /* "rb" = read binary */
fread(&loaded, sizeof(Record), 1, g);  /* read 1 record */
fclose(g);
/* loaded.id == 1, loaded.name == "Alice", loaded.score == 95.5 */
```

## Binary vs Text Mode

On Linux, there's no difference between `"r"` and `"rb"`. But on Windows,
text mode translates `\r\n` (carriage return + newline) to `\n` on read, and
the reverse on write. Binary mode reads/writes bytes exactly as they are.

**Rule of thumb**: Use `"rb"`/`"wb"` for binary files (images, structs, databases).
Use `"r"`/`"w"` for text files.

## Seeking: Random Access with `fseek` and `ftell`

Normally, you read a file from start to finish (sequential access). But sometimes
you need to jump to a specific position — for example, to read record #47 in a
database file without reading the first 46 records.

```c
/* fseek(file, offset, whence) */
fseek(f, 0, SEEK_SET);    /* go to beginning */
fseek(f, 100, SEEK_SET);  /* go to byte 100 from start */
fseek(f, -10, SEEK_CUR);  /* go back 10 bytes from current position */
fseek(f, 0, SEEK_END);    /* go to end of file */

/* ftell(file) — returns current position */
long pos = ftell(f);       /* "where am I?" */
```

### Sequential vs Random Access

```
Sequential Access (reading a log file):

  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
  │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │  bytes
  └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
    ^───>───>───>───>───>───>───>───>───>
    read from start to end, one chunk at a time


Random Access (database with fixed-size records):

  Record 0      Record 1      Record 2      Record 3
  ┌────────────┬────────────┬────────────┬────────────┐
  │  64 bytes  │  64 bytes  │  64 bytes  │  64 bytes  │
  └────────────┴────────────┴────────────┴────────────┘
  offset 0      offset 64     offset 128    offset 192

  To read record N: fseek(f, N * RECORD_SIZE, SEEK_SET)
  No need to read records 0 through N-1!
```

## Fixed-Size Records

A powerful pattern for simple databases: store structs of the same size
sequentially in a binary file. Because every record has the same size, you can
jump to any record instantly.

```
  Fixed-Size Record Layout (64 bytes each):

  ┌──────────────────────────────────────────────────────────────────┐
  │  Bytes 0-3     │  Bytes 4-55             │  56-59  │  60-63     │
  │  int id        │  char title[52]         │  int done│  int active│
  │  (4 bytes)     │  (52 bytes)             │ (4 bytes)│ (4 bytes) │
  └──────────────────────────────────────────────────────────────────┘
                            = 64 bytes total

  File layout:
  ┌─────────┬─────────┬─────────┬─────────┬─────────┐
  │ Rec #0  │ Rec #1  │ Rec #2  │ Rec #3  │  ...    │
  │ 64 bytes│ 64 bytes│ 64 bytes│ 64 bytes│         │
  └─────────┴─────────┴─────────┴─────────┴─────────┘
  ^         ^         ^
  offset 0  offset 64 offset 128

  Seek to record N: fseek(f, N * 64, SEEK_SET)
```

### Fixed-Size Records as a Mini Database

A powerful pattern: store data as fixed-size records in a binary file. Because
every record is the same size, you can jump directly to record N using:
`fseek(fp, N * record_size, SEEK_SET)`. This gives **O(1) random access** —
no scanning needed!

```
  Record layout (each record = 72 bytes):
  ┌──────┬──────┬─────┬──────────────────────────────┐
  │  id  │ done │ len │         text (64 bytes)       │
  │ u32  │ bool │ u8  │  (fixed, padded with zeros)   │
  │4 byte│1 byte│1 byte│                               │
  └──────┴──────┴─────┴──────────────────────────────┘

  File with 3 records:
  ┌────────────┬────────────┬────────────┐
  │ Record 0   │ Record 1   │ Record 2   │
  │ (72 bytes) │ (72 bytes) │ (72 bytes) │
  └────────────┴────────────┴────────────┘

  To read record 2:  fseek(fp, 2 * 72, SEEK_SET); fread(&rec, 72, 1, fp);
  To update record 1: fseek(fp, 1 * 72, SEEK_SET); fwrite(&rec, 72, 1, fp);
```

This is how simple databases work before they need B-Trees. SQLite, Redis,
and many databases started with exactly this pattern. When your data has a
fixed structure and you know the record number, you get instant access without
scanning the entire file.

## Buffering Strategies

The C library supports three buffering modes:

| Mode | Macro | Behavior | Default for |
|------|-------|----------|-------------|
| Full buffering | `_IOFBF` | Flush when buffer is full | Regular files |
| Line buffering | `_IOLBF` | Flush on newline | stdout (when terminal) |
| No buffering | `_IONBF` | Every write goes to kernel immediately | stderr |

You can control buffering with `setvbuf()`:

```c
/* Make stderr line-buffered (instead of unbuffered) */
setvbuf(stderr, NULL, _IOLBF, 0);

/* Use a custom 16KB buffer for a file */
char mybuf[16384];
setvbuf(f, mybuf, _IOFBF, sizeof(mybuf));

/* Disable buffering entirely */
setvbuf(f, NULL, _IONBF, 0);

/* Force flush the buffer right now */
fflush(f);  /* writes buffered data to kernel */
```

**Why does this matter?** If your program crashes, un-flushed data in the buffer
is lost. That's why `stderr` is unbuffered by default — error messages should
appear immediately, even if the program crashes right after.

## The Unix Philosophy: Everything Is a File

Unix has a beautiful idea: **almost everything looks like a file**. This means
the same `read`/`write` system calls work on:

- Regular files on disk
- The terminal (stdin/stdout/stderr)
- Pipes between programs (`ls | grep foo`)
- Network sockets
- Hardware devices (`/dev/null`, `/dev/random`, `/dev/sda`)
- Process information (`/proc/self/status`)

```
  Everything Is a File

  ┌──────────────┐
  │  Your Code   │
  │              │
  │  read(fd,..) │
  │  write(fd,..)│
  └──────┬───────┘
         │
         │  Same system calls!
         │
    ┌────┴────┬──────────┬───────────┬──────────┐
    │         │          │           │          │
    v         v          v           v          v
  ┌─────┐ ┌─────┐  ┌────────┐ ┌────────┐ ┌────────┐
  │Disk │ │Pipe │  │Terminal│ │Socket  │ │/proc/  │
  │file │ │     │  │(tty)   │ │(network│ │(kernel │
  │     │ │     │  │        │ │connect)│ │ info)  │
  └─────┘ └─────┘  └────────┘ └────────┘ └────────┘
```

This is why Unix command-line tools are so composable. A program that reads from
stdin and writes to stdout can be plugged into any pipeline:

```bash
cat file.txt | grep "error" | wc -l
```

Each program doesn't know (or care) whether its input comes from a file, a pipe,
or a keyboard.

## Reading Different File Types

### Text Files

Text files are sequences of printable characters and whitespace (newlines, tabs).
Read them line by line with `fgets`:

```c
char line[1024];
while (fgets(line, sizeof(line), f) != NULL) {
    printf("%s", line);  /* line already includes '\n' */
}
```

### Binary File Headers

Many binary formats start with a "magic number" — a few bytes that identify the
file type:

| Format | Magic Bytes | ASCII |
|--------|-------------|-------|
| PNG    | `89 50 4E 47` | `.PNG` |
| PDF    | `25 50 44 46` | `%PDF` |
| ELF    | `7F 45 4C 46` | `.ELF` |
| JPEG   | `FF D8 FF`    | (none) |
| GIF    | `47 49 46 38` | `GIF8` |

You can read these bytes and identify a file's real type regardless of its
extension:

```c
unsigned char header[4];
fread(header, 1, 4, f);

if (header[0] == 0x89 && header[1] == 'P' &&
    header[2] == 'N'  && header[3] == 'G') {
    printf("This is a PNG file\n");
}
```

## Directory Traversal

So far we've been working with individual files. But what if you want to list
all the files in a directory, or walk an entire directory tree? The C library
gives you `opendir()`, `readdir()`, and `closedir()` for this.

```c
#include <dirent.h>
#include <stdio.h>

DIR *dir = opendir("/home/you/projects");
if (dir == NULL) {
    perror("opendir");
    return 1;
}

struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
}

closedir(dir);
```

The `struct dirent` gives you two useful fields:
- `d_name` — the filename (just the name, not the full path)
- `d_type` — the type of entry: `DT_REG` (regular file), `DT_DIR` (directory),
  `DT_LNK` (symlink), etc.

### Recursive Directory Walking

To walk an entire directory tree, you recurse into subdirectories:

```
  function walk(path):
    dir = opendir(path)
    for each entry in dir:
      if entry is "." or "..": skip
      full_path = path + "/" + entry.d_name
      if entry is directory:
        walk(full_path)    ← recurse!
      else:
        process(full_path)
    closedir(dir)
```

**Important**: You must skip `.` (current directory) and `..` (parent
directory), or your recursion will loop forever!

In C, this looks like:

```c
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void walk(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        /* Skip "." and ".." to avoid infinite recursion */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        /* Build the full path */
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            printf("[DIR]  %s\n", full_path);
            walk(full_path);    /* recurse into subdirectory */
        } else {
            printf("[FILE] %s\n", full_path);
        }
    }
    closedir(dir);
}
```

This is how `find`, `ls -R`, and `tree` work under the hood. Every tool that
searches or processes files in a directory tree uses this same pattern.

## Common Pitfalls

1. **Forgetting to close files**: Every open file uses a file descriptor. If you
   keep opening without closing, you'll hit the system limit (~1024 by default).

2. **Not checking return values**: `fopen` returns `NULL` on failure. `fread`
   returns the number of items read, which might be less than you asked for.

3. **Buffer overflow with `fgets`**: Always pass the correct buffer size.
   `fgets(buf, sizeof(buf), f)` is safe; `fgets(buf, 9999, f)` is a bug if
   `buf` is smaller than 9999.

4. **Forgetting `fflush`/`fclose` before reading back**: Data might still be
   in the buffer. Always `fclose` (or `fflush`) before trying to read what you
   just wrote.

5. **Text mode on binary files**: On Windows, opening a binary file in text mode
   corrupts data because `\r\n` gets translated.

## Exercises

1. **hexdump** — Build a hex dump tool that displays file contents in hex and
   ASCII, like the real `hexdump -C` command.

2. **wc** — Build a `wc` clone that counts lines, words, and characters.

3. **todo_app** — Build a todo app backed by a binary file with fixed-size
   records and random access — a tiny database!

---

[← Previous: Module 15 — Trees & Heaps](../23-trees-heaps/README.md) | [Next: Module 17 — Text, Unicode & Encoding →](../26-text-unicode-encoding/README.md)
