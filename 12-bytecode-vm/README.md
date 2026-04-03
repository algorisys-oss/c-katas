# Module 12: Build a Machine Inside a Machine — Bytecode Virtual Machines

> "Any sufficiently advanced program contains an ad-hoc, informally-specified,
> bug-ridden, slow implementation of half of a virtual machine."
> — Paraphrased from Greenspun's Tenth Rule

This is the module where everything clicks. You are about to build a **computer
inside your computer** — a program that reads instructions and executes them,
just like a real CPU. If Module 00 taught you that a CPU is a loop that fetches,
decodes, and executes instructions, this module makes that loop **yours**.

---

## What Is a Virtual Machine?

A virtual machine (VM) is a **program that pretends to be a computer**.

Think about what a real CPU does:
1. **Fetch** — read the next instruction from memory
2. **Decode** — figure out what that instruction means
3. **Execute** — do the thing (add, compare, jump, etc.)
4. **Repeat** — go back to step 1

That's it. That's a computer. And you can write that loop in C.

```
    A real CPU                          Your VM (a C program)
    ┌──────────────┐                    ┌──────────────────────┐
    │ Fetch inst   │                    │ opcode = code[ip++]; │
    │ from memory  │                    │                      │
    ├──────────────┤                    ├──────────────────────┤
    │ Decode the   │                    │ switch (opcode) {    │
    │ opcode       │                    │                      │
    ├──────────────┤                    ├──────────────────────┤
    │ Execute it   │                    │   case OP_ADD: ...   │
    │              │                    │   case OP_JMP: ...   │
    ├──────────────┤                    ├──────────────────────┤
    │ Loop forever │                    │ } /* end switch */   │
    │ (until HALT) │                    │ } /* end while */    │
    └──────────────┘                    └──────────────────────┘
```

The key insight: **the fetch-decode-execute cycle you learned in Module 00 is
literally the same thing as a `while` + `switch` in C.** A CPU is just hardware
doing what your VM does in software.

---

## Connecting to Module 00

Remember the fetch-decode-execute cycle from Module 00?

```
    ┌─────────────────────────────────────────┐
    │              CPU Cycle                   │
    │                                          │
    │    ┌───────┐   ┌────────┐   ┌─────────┐ │
    │    │ FETCH │──▶│ DECODE │──▶│ EXECUTE │ │
    │    └───────┘   └────────┘   └─────────┘ │
    │         ▲                        │       │
    │         └────────────────────────┘       │
    │              (repeat)                    │
    └─────────────────────────────────────────┘
```

Back then it was abstract. Now you will implement it **for real**:

- **Fetch** = read `code[ip]` and increment `ip`
- **Decode** = the `switch` statement that checks which opcode it is
- **Execute** = the code inside each `case` that does the actual work

Every CPU you have ever used — your phone, your laptop, game consoles — is
doing exactly this, billions of times per second, in silicon. You are about to
do it in C, at your own pace, where you can watch every step.

---

## Stack-Based vs Register-Based VMs

There are two main designs for virtual machines:

### Stack-Based (what we will build)

Operands live on a **stack**. To add 2 + 3:
```
    PUSH 2      ← push 2 onto the stack
    PUSH 3      ← push 3 onto the stack
    ADD         ← pop two values, push the sum
```

The ADD instruction does not say *which* values to add — it always uses
whatever is on top of the stack. This makes instructions short (often just
1 byte for the opcode).

**Who uses this?** The Java Virtual Machine (JVM), Python's CPython, WebAssembly,
the .NET CLR, PostScript, Forth.

### Register-Based

Operands live in numbered **registers** (like variables). To add 2 + 3:
```
    LOAD R0, 2      ← put 2 in register 0
    LOAD R1, 3      ← put 3 in register 1
    ADD  R2, R0, R1 ← R2 = R0 + R1
```

Instructions are longer (must name the registers) but there is less pushing
and popping.

**Who uses this?** Lua 5.0+, Android's Dalvik VM, LuaJIT.

### Comparison

```
    Stack-based                      Register-based
    ┌────────────────────┐           ┌────────────────────┐
    │ + Simpler to build │           │ + Fewer instructions│
    │ + Shorter bytecode │           │ + Less stack traffic│
    │ + Easier to compile│           │ + Often faster      │
    │ - More instructions│           │ - Bigger bytecodes  │
    │ - Stack overhead   │           │ - Harder to compile │
    └────────────────────┘           └────────────────────┘
```

We will build **stack-based** because it is simpler to implement — and once
you understand one, the other is a straightforward variation.

---

## VM Architecture

Here is the complete architecture of the VM you will build:

```
    ┌───────────────────────────────────────────────┐
    │                  BytecodeVM                    │
    │                                                │
    │  ┌──────────────────────────────────────────┐  │
    │  │  Code Memory (256 bytes)                 │  │
    │  │  ┌────┬────┬────┬────┬────┬────┬─────┐   │  │
    │  │  │OP  │arg │arg │arg │arg │OP  │ ... │   │  │
    │  │  │PUSH│ 0  │ 0  │ 0  │ 2  │ADD │     │   │  │
    │  │  └────┴────┴────┴────┴────┴────┴─────┘   │  │
    │  │       ▲                                   │  │
    │  │       │  IP (instruction pointer)         │  │
    │  └──────────────────────────────────────────┘  │
    │                                                │
    │  ┌──────────────────────────────────────────┐  │
    │  │  Operand Stack (256 entries, int32_t)    │  │
    │  │  ┌─────┬─────┬─────┬─────┬─────┬─────┐  │  │
    │  │  │  2  │  3  │     │     │     │     │  │  │
    │  │  └─────┴─────┴─────┴─────┴─────┴─────┘  │  │
    │  │          ▲                                │  │
    │  │          │  SP (stack pointer)            │  │
    │  └──────────────────────────────────────────┘  │
    │                                                │
    │  running: 1  (1 = executing, 0 = halted)       │
    │  error:   0  (0 = ok, nonzero = error code)    │
    └───────────────────────────────────────────────┘
```

- **Code Memory**: the bytecode program. An array of `uint8_t` bytes.
- **IP (Instruction Pointer)**: index into code memory. Points to the next
  instruction to execute.
- **Operand Stack**: where values are pushed and popped during computation.
  An array of `int32_t` values.
- **SP (Stack Pointer)**: index into the stack. Points to the *next free slot*
  (i.e., the number of items currently on the stack).
- **running**: a flag. The dispatch loop runs while this is 1.
- **error**: an error code (0 = no error). Set on stack overflow, underflow,
  division by zero, etc.

---

## Instruction Encoding

Each instruction starts with a **1-byte opcode** (a number from our enum).
Some instructions are followed by additional bytes (operands).

For example, `OP_PUSH` is followed by a **4-byte (32-bit) value** stored in
little-endian order:

```
    Byte:  [0x01] [0x02] [0x00] [0x00] [0x00]
            ▲      ▲      ▲      ▲      ▲
            │      └──────┴──────┴──────┘
            │         value = 2 (little-endian int32)
            │
          opcode = OP_PUSH
```

`OP_JMP` and its variants are followed by a **4-byte absolute address**:

```
    Byte:  [0x0F] [0x00] [0x00] [0x00] [0x00]
            ▲      ▲      ▲      ▲      ▲
            │      └──────┴──────┴──────┘
            │         target address = 0
            │
          opcode = OP_JMP
```

Most other instructions (ADD, SUB, POP, PRINT, HALT, etc.) are just the
**1-byte opcode** with no operands:

```
    Byte:  [0x03]
            ▲
          opcode = OP_ADD
```

---

## The Dispatch Loop

The core of every VM is the dispatch loop. Here it is in pseudocode:

```c
    while (vm->running) {
        uint8_t opcode = vm->code[vm->ip++];    // FETCH

        switch (opcode) {                        // DECODE + EXECUTE
            case OP_PUSH: {
                int32_t value = read_int32(&vm->code[vm->ip]);
                vm->ip += 4;
                vm_push(vm, value);
                break;
            }
            case OP_ADD: {
                int32_t b = vm_pop(vm);
                int32_t a = vm_pop(vm);
                vm_push(vm, a + b);
                break;
            }
            case OP_HALT:
                vm->running = 0;
                break;
            // ... more cases ...
        }
    }
```

That is it. That is a virtual machine. A `while` loop, a `switch`, and some
stack operations. Everything else is just adding more instructions.

---

## Step-by-Step Trace: Computing 2 + 3

Let us trace the execution of this program byte by byte:

```
    Bytecode: [PUSH][2][PUSH][3][ADD][PRINT][HALT]
              (where PUSH includes 4 bytes for the value)

    Step 1: IP=0   Fetch: OP_PUSH   Read value: 2     Push 2
            Stack: [ 2 ]
                     ▲ SP=1

    Step 2: IP=5   Fetch: OP_PUSH   Read value: 3     Push 3
            Stack: [ 2 | 3 ]
                         ▲ SP=2

    Step 3: IP=10  Fetch: OP_ADD    Pop 3, Pop 2       Push 2+3=5
            Stack: [ 5 ]
                     ▲ SP=1

    Step 4: IP=11  Fetch: OP_PRINT  Pop 5              Output: 5
            Stack: [ ]
                   ▲ SP=0

    Step 5: IP=12  Fetch: OP_HALT   Set running=0      (done)
```

Notice how:
- PUSH advances IP by 5 (1 for opcode + 4 for value)
- ADD, PRINT, HALT advance IP by 1 (just the opcode)
- The stack grows and shrinks as values are pushed and popped

---

## How Real VMs Work

The VM you are building follows the exact same architecture as production VMs:

### Java Virtual Machine (JVM)
- Stack-based, bytecode compiled from Java/Kotlin/Scala
- ~200 opcodes
- Has a constant pool (like our code memory but fancier)
- Runs on billions of devices worldwide

### CPython (Python's default interpreter)
- Stack-based, bytecode compiled from `.py` files
- You can see the bytecode: `python3 -c "import dis; dis.dis(lambda: 2+3)"`
- Each `.pyc` file IS bytecode for the CPython VM

### Lua 5.0+
- Register-based (switched from stack-based in 5.0)
- One of the fastest scripting language VMs ever built
- Entire implementation is ~20,000 lines of C

### WebAssembly (WASM)
- Stack-based, designed to run in web browsers
- Near-native speed through JIT compilation
- The bytecode format the web is converging on

**You are building the same thing they built.** Simpler, yes, but the core
ideas are identical.

---

## Why VMs Matter

1. **Portability**: Write bytecode once, run it on any platform that has the VM.
   Java's "write once, run anywhere" is this idea.

2. **Safety**: The VM can check every operation — array bounds, stack overflow,
   memory access — before executing it. This is how web browsers safely run
   untrusted code (JavaScript VMs, WebAssembly).

3. **Language implementation**: Want to create your own programming language?
   You need a VM to run it on. Compile your language to bytecode, run it on
   your VM. Most modern languages work this way.

4. **Understanding computers**: Once you have built a VM, you truly understand
   what a computer *is* — a machine that reads and follows instructions. The
   silicon is just a very fast version of your `while` + `switch`.

---

## Exercises

| # | File               | What You Build                              |
|---|--------------------|---------------------------------------------|
| 1 | `exercises/vm.c`   | A complete stack-based bytecode VM          |
| 2 | `exercises/disassembler.c` | A disassembler that prints bytecode |

### Building and Testing

```bash
cd exercises/
make all       # build everything
./vm           # run VM tests
./disassembler # run disassembler tests
make clean     # remove binaries
```

---

## What Comes Next

After this module you will have built a working virtual machine — a program
that executes programs. In later modules, you can:

- Build a **compiler** that translates a simple language into your VM's bytecode
- Add **variables** (local/global) to the instruction set
- Add **functions** with call/return and a call stack
- Add **garbage collection** for automatic memory management
- JIT compile hot bytecode paths to native machine code

But first: build the machine. Everything else follows from here.
