#!/bin/bash
#
# Kata: Explore the Compilation Pipeline
#
# This script walks you through each stage of compilation so you can see
# what happens at every step. Run it after you have completed hello.c.
#
# Usage: bash exercises/explore_compilation.sh
#
# You will fill in the gcc commands for each stage.

set -e  # Stop on first error

SOURCE="exercises/hello.c"

echo "=== Exploring the Compilation Pipeline ==="
echo ""
echo "Source file: $SOURCE"
echo ""

# -----------------------------------------------------------------------
# Stage 1: Preprocessor
# The preprocessor handles #include and #define — it is just text editing.
# The output is still C code, but with all headers pasted in.
# -----------------------------------------------------------------------
echo "--- Stage 1: Preprocessor (gcc -E) ---"

# TODO: Run the preprocessor on $SOURCE and save the output to hello.i
#       Command: gcc -E $SOURCE -o hello.i

echo "Created hello.i (preprocessed source)"
echo "Lines in original source:"
wc -l < "$SOURCE"
echo "Lines after preprocessing:"
# TODO: Count lines in hello.i using: wc -l < hello.i
echo ""
echo "Notice how much bigger it got? That is because #include <stdio.h>"
echo "pasted thousands of lines of declarations into your file."
echo ""

# -----------------------------------------------------------------------
# Stage 2: Compiler (C --> Assembly)
# The compiler translates C into assembly language — human-readable
# instructions that are very close to what the CPU actually executes.
# -----------------------------------------------------------------------
echo "--- Stage 2: Compiler (gcc -S) ---"

# TODO: Compile to assembly and save the output to hello.s
#       Command: gcc -S -std=c99 $SOURCE -o hello.s

echo "Created hello.s (assembly language)"
echo ""
echo "Here is the assembly output:"
echo "-----------------------------"
# TODO: Display the contents of hello.s using: cat hello.s
echo "-----------------------------"
echo ""
echo "Look for:"
echo "  - Your string \"Hello, world!\" somewhere in the output"
echo "  - The 'main:' label — that is where your main function starts"
echo "  - A 'call' instruction — that is where printf/puts gets called"
echo ""

# -----------------------------------------------------------------------
# Stage 3: Assembler (Assembly --> Object file)
# The assembler turns assembly into machine code (binary).
# The result is an object file — binary, not human-readable.
# -----------------------------------------------------------------------
echo "--- Stage 3: Assembler (gcc -c) ---"

# TODO: Assemble into an object file and save to hello.o
#       Command: gcc -c -std=c99 $SOURCE -o hello.o

echo "Created hello.o (object file — binary machine code)"
echo ""
echo "If you try to read it as text, it looks like garbage:"
head -c 100 hello.o | cat -v
echo ""
echo ""
echo "That is expected — it is binary, meant for the CPU, not your eyes."
echo ""

# -----------------------------------------------------------------------
# Stage 4: Linker (Object file --> Executable)
# The linker combines your object file with library code (like printf)
# to produce the final runnable program.
# -----------------------------------------------------------------------
echo "--- Stage 4: Linker (gcc hello.o --> executable) ---"

# TODO: Link the object file into a final executable called hello
#       Command: gcc hello.o -o hello

echo "Created hello (final executable)"
echo ""
echo "Let's run it:"
./hello
echo ""

# -----------------------------------------------------------------------
# Cleanup
# -----------------------------------------------------------------------
echo "--- Summary of files created at each stage ---"
echo ""
ls -la hello.i hello.s hello.o hello 2>/dev/null
echo ""
echo "You can inspect each file to see what that stage produced."
echo "When you are done exploring, run: rm -f hello.i hello.s hello.o hello"
