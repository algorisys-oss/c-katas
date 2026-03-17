#!/bin/bash
#
# Solution: Explore the Compilation Pipeline
#
# Usage: bash solutions/explore_compilation.sh

set -e

SOURCE="solutions/hello.c"

echo "=== Exploring the Compilation Pipeline ==="
echo ""
echo "Source file: $SOURCE"
echo ""

# Stage 1: Preprocessor
echo "--- Stage 1: Preprocessor (gcc -E) ---"
gcc -E "$SOURCE" -o hello.i
echo "Created hello.i (preprocessed source)"
echo "Lines in original source:"
wc -l < "$SOURCE"
echo "Lines after preprocessing:"
wc -l < hello.i
echo ""
echo "Notice how much bigger it got? That is because #include <stdio.h>"
echo "pasted thousands of lines of declarations into your file."
echo ""

# Stage 2: Compiler (C --> Assembly)
echo "--- Stage 2: Compiler (gcc -S) ---"
gcc -S -std=c99 "$SOURCE" -o hello.s
echo "Created hello.s (assembly language)"
echo ""
echo "Here is the assembly output:"
echo "-----------------------------"
cat hello.s
echo "-----------------------------"
echo ""
echo "Look for:"
echo "  - Your string \"Hello, world!\" somewhere in the output"
echo "  - The 'main:' label — that is where your main function starts"
echo "  - A 'call' instruction — that is where printf/puts gets called"
echo ""

# Stage 3: Assembler (Assembly --> Object file)
echo "--- Stage 3: Assembler (gcc -c) ---"
gcc -c -std=c99 "$SOURCE" -o hello.o
echo "Created hello.o (object file — binary machine code)"
echo ""
echo "If you try to read it as text, it looks like garbage:"
head -c 100 hello.o | cat -v
echo ""
echo ""
echo "That is expected — it is binary, meant for the CPU, not your eyes."
echo ""

# Stage 4: Linker (Object file --> Executable)
echo "--- Stage 4: Linker (gcc hello.o --> executable) ---"
gcc hello.o -o hello
echo "Created hello (final executable)"
echo ""
echo "Let's run it:"
./hello
echo ""

# Summary
echo "--- Summary of files created at each stage ---"
echo ""
ls -la hello.i hello.s hello.o hello 2>/dev/null
echo ""
echo "You can inspect each file to see what that stage produced."
echo "When you are done exploring, run: rm -f hello.i hello.s hello.o hello"
