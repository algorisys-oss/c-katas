# C Katas — Systems Programming from First Principles

A ground-up C (C99) systems programming curriculum for building deep, intuitive understanding of how computers work. 30 modules over 1–2 years — from "what is a computer" to building real systems.

## Prerequisites

- A Linux machine (or WSL on Windows)
- Basic computer literacy (using a terminal, editing text files)
- No programming experience required

## Setup

### Install the toolchain

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install gcc make valgrind

# Verify
gcc --version      # should show gcc 9+ (C99 support)
make --version     # GNU Make
valgrind --version # memory checker
```

### Clone this repo

```bash
git clone https://github.com/rajeshpillai/c-katas.git
cd c-katas
```

### Browse lessons in the browser

The website needs a local HTTP server (browsers block file:// fetch requests for security).

```bash
# Serve the website locally using Python (built-in, no install needed)
cd c-katas
python3 -m http.server 8080

# Open in browser
# http://localhost:8080
```

**Why not just open the HTML file directly?**
The website fetches module README.md files via JavaScript `fetch()`. Browsers block
these requests from `file://` URLs (CORS policy). A local HTTP server fixes this.

**Alternative servers** (if you don't have Python):

```bash
# Node.js
npx serve -l 8080

# PHP
php -S localhost:8080

# Ruby
ruby -run -e httpd . -p 8080
```

## How to use

### Working through a module

```bash
# 1. Read the lesson
cat 00-how-computers-work/README.md
# (or open in your editor / the website)

# 2. Navigate to the module
cd 00-how-computers-work

# 3. Build the exercises
make exercises

# 4. Edit the exercise file — fill in the TODOs
#    e.g., exercises/base_converter.c

# 5. Compile and run your solution
make exercises
./base_converter

# 6. Check against the reference solution
make solutions
make test

# 7. Check for memory errors (from Module 07 onwards)
valgrind ./base_converter
```

### Compiler flags

All code is compiled with:

```
gcc -Wall -Wextra -pedantic -std=c99
```

- `-Wall -Wextra`: enable most warnings (treat warnings as learning opportunities)
- `-pedantic`: strict C99 conformance
- `-std=c99`: use the C99 standard

## Curriculum Overview

| Phase | Modules | Focus |
|-------|---------|-------|
| **1: How Computers Work & C Basics** | 00–05 | Binary, types, control flow, functions, recursion, Big-O |
| **2: Pointers & Memory** | 06–08 | Pointers, arrays, malloc/free, arena allocators |
| **3: Data Structures & Algorithms** | 09–15 | Structs, linked lists, hash tables, sorting, trees, heaps |
| **4: Systems Building Blocks** | 16–21 | File I/O, Unicode, datetime, preprocessor, graphs, concurrency |
| **5: Building Real Systems** | 22–28c | Text editor, git, database, networking, encryption, graphics, image processing, ray tracing |
| **6: Advanced & Capstone** | 29–30 | Machine learning in C, capstone project |

See [AGENDA.md](AGENDA.md) for the full detailed curriculum.
See [TODO.md](TODO.md) to track progress.

## Repository Structure

```
c-katas/
├── README.md           # This file
├── AGENDA.md           # Full curriculum with all topics
├── CLAUDE.md           # Project conventions & teaching approach
├── TODO.md             # Progress tracker
├── site/               # Lesson navigation website
│   └── index.html      # Open in browser to browse lessons
├── 00-how-computers-work/
│   ├── README.md       # Lesson: concepts, diagrams, explanations
│   ├── exercises/      # Kata files with TODO markers
│   ├── solutions/      # Reference solutions
│   └── Makefile        # Build & test targets
├── 01-setup-first-program/
│   ├── README.md
│   ├── exercises/
│   ├── solutions/
│   └── Makefile
└── ...                 # More modules as we build them
```

## License

Educational use. Built for learning.
