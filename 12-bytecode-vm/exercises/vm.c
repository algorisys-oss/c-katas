/*
 * Kata: Stack-Based Bytecode Virtual Machine
 *
 * Build a complete VM that executes bytecode programs.
 * This is the fetch-decode-execute cycle from Module 00 — made real.
 *
 * Your VM has:
 *   - 256 bytes of code memory (the program)
 *   - 256-entry operand stack (int32_t values)
 *   - An instruction pointer (IP) and stack pointer (SP)
 *   - A running flag and error code
 *
 * Instruction set:
 *   OP_NOP              — do nothing
 *   OP_PUSH  <int32>    — push a 32-bit value (4 bytes, little-endian)
 *   OP_POP              — discard top of stack
 *   OP_ADD              — pop two, push sum
 *   OP_SUB              — pop b, pop a, push a - b
 *   OP_MUL              — pop two, push product
 *   OP_DIV              — pop b, pop a, push a / b  (check div-by-zero!)
 *   OP_MOD              — pop b, pop a, push a %% b  (check div-by-zero!)
 *   OP_NEG              — negate top of stack
 *   OP_EQ               — pop two, push 1 if equal, else 0
 *   OP_LT               — pop b, pop a, push 1 if a < b, else 0
 *   OP_GT               — pop b, pop a, push 1 if a > b, else 0
 *   OP_NOT              — pop one, push 1 if zero, else 0
 *   OP_AND              — pop two, push logical AND (both nonzero => 1)
 *   OP_OR               — pop two, push logical OR  (either nonzero => 1)
 *   OP_JMP   <int32>    — set IP to absolute address
 *   OP_JZ    <int32>    — pop top; if zero, set IP to address
 *   OP_JNZ   <int32>    — pop top; if nonzero, set IP to address
 *   OP_DUP              — duplicate top of stack
 *   OP_SWAP             — swap top two stack entries
 *   OP_PRINT            — pop and print top of stack (prints to stdout)
 *   OP_HALT             — stop execution
 *
 * 25 tests total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ── Test Harness ─────────────────────────────────────────────────── */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* ── Opcodes ──────────────────────────────────────────────────────── */

typedef enum {
    OP_NOP   = 0x00,
    OP_PUSH  = 0x01,   /* followed by 4-byte little-endian int32 */
    OP_POP   = 0x02,
    OP_ADD   = 0x03,
    OP_SUB   = 0x04,
    OP_MUL   = 0x05,
    OP_DIV   = 0x06,
    OP_MOD   = 0x07,
    OP_NEG   = 0x08,
    OP_EQ    = 0x09,
    OP_LT    = 0x0A,
    OP_GT    = 0x0B,
    OP_NOT   = 0x0C,
    OP_AND   = 0x0D,
    OP_OR    = 0x0E,
    OP_JMP   = 0x0F,   /* followed by 4-byte little-endian address */
    OP_JZ    = 0x10,   /* followed by 4-byte little-endian address */
    OP_JNZ   = 0x11,   /* followed by 4-byte little-endian address */
    OP_DUP   = 0x12,
    OP_SWAP  = 0x13,
    OP_PRINT = 0x14,
    OP_HALT  = 0x15
} Opcode;

/* ── Error Codes ──────────────────────────────────────────────────── */

#define VM_OK              0
#define VM_ERR_OVERFLOW    1   /* stack overflow  */
#define VM_ERR_UNDERFLOW   2   /* stack underflow */
#define VM_ERR_DIV_ZERO    3   /* division by zero */
#define VM_ERR_BAD_OPCODE  4   /* unknown opcode  */
#define VM_ERR_IP_BOUNDS   5   /* IP out of range */

/* ── VM Structure ─────────────────────────────────────────────────── */

#define CODE_SIZE  256
#define STACK_SIZE 256

typedef struct {
    uint8_t  code[CODE_SIZE];       /* bytecode program             */
    int32_t  stack[STACK_SIZE];     /* operand stack                */
    int      ip;                    /* instruction pointer          */
    int      sp;                    /* stack pointer (next free slot)*/
    int      running;               /* 1 = running, 0 = halted     */
    int      error;                 /* error code (0 = ok)          */
} VM;

/* ── Bytecode Builder Helpers (pre-written) ───────────────────────── *
 *
 * These let you construct bytecode in a readable way:
 *     uint8_t code[CODE_SIZE];
 *     int pc = 0;
 *     pc = emit_push(code, pc, 42);
 *     pc = emit_op(code, pc, OP_PRINT);
 *     pc = emit_op(code, pc, OP_HALT);
 */

static int emit_op(uint8_t *code, int pc, uint8_t op)
{
    code[pc] = op;
    return pc + 1;
}

static int emit_int32(uint8_t *code, int pc, int32_t value)
{
    code[pc + 0] = (uint8_t)(value & 0xFF);
    code[pc + 1] = (uint8_t)((value >> 8) & 0xFF);
    code[pc + 2] = (uint8_t)((value >> 16) & 0xFF);
    code[pc + 3] = (uint8_t)((value >> 24) & 0xFF);
    return pc + 4;
}

static int emit_push(uint8_t *code, int pc, int32_t value)
{
    pc = emit_op(code, pc, OP_PUSH);
    pc = emit_int32(code, pc, value);
    return pc;
}

/* Read a little-endian int32 from a byte array */
static int32_t read_int32(const uint8_t *bytes)
{
    return (int32_t)(
        (uint32_t)bytes[0] |
        ((uint32_t)bytes[1] << 8) |
        ((uint32_t)bytes[2] << 16) |
        ((uint32_t)bytes[3] << 24)
    );
}

/* ══════════════════════════════════════════════════════════════════════
 *  EXERCISES — implement these five functions
 * ══════════════════════════════════════════════════════════════════════ */

/* ── Exercise 1: vm_init ──────────────────────────────────────────── *
 * Initialize all VM fields to a clean state.
 *   - Zero out code[] and stack[] arrays  (use memset)
 *   - Set ip and sp to 0
 *   - Set running to 0
 *   - Set error to VM_OK
 */
void vm_init(VM *vm)
{
    /* TODO: initialize all fields */
    (void)vm;
}

/* ── Exercise 2: vm_load ──────────────────────────────────────────── *
 * Copy bytecode into the VM's code memory and prepare for execution.
 *   - Copy 'length' bytes from 'bytecode' into vm->code  (use memcpy)
 *   - If length > CODE_SIZE, only copy CODE_SIZE bytes
 *   - Reset ip to 0, sp to 0
 *   - Set running to 1
 *   - Set error to VM_OK
 */
void vm_load(VM *vm, const uint8_t *bytecode, int length)
{
    /* TODO: copy bytecode, reset execution state */
    (void)vm; (void)bytecode; (void)length;
}

/* ── Exercise 3: vm_push ──────────────────────────────────────────── *
 * Push a value onto the operand stack.
 *   - If sp >= STACK_SIZE: set error = VM_ERR_OVERFLOW, running = 0
 *   - Otherwise: stack[sp] = value, then sp++
 */
void vm_push(VM *vm, int32_t value)
{
    /* TODO: push with overflow check */
    (void)vm; (void)value;
}

/* ── Exercise 4: vm_pop ───────────────────────────────────────────── *
 * Pop a value from the operand stack and return it.
 *   - If sp <= 0: set error = VM_ERR_UNDERFLOW, running = 0, return 0
 *   - Otherwise: sp--, return stack[sp]
 */
int32_t vm_pop(VM *vm)
{
    /* TODO: pop with underflow check */
    (void)vm;
    return 0; /* placeholder */
}

/* ── Exercise 5: vm_run ───────────────────────────────────────────── *
 * The main dispatch loop — the heart of the VM.
 *
 * while (vm->running && vm->error == VM_OK):
 *   1. Bounds check: if ip < 0 or ip >= CODE_SIZE, set error and stop.
 *   2. FETCH: read opcode = code[ip], then ip++.
 *   3. DECODE + EXECUTE: switch on opcode, handle each instruction.
 *
 * For opcodes with operands (PUSH, JMP, JZ, JNZ):
 *   - Read a 4-byte int32 at code[ip] using read_int32(&vm->code[vm->ip])
 *   - Advance ip by 4 after reading
 *
 * For binary ops (ADD, SUB, MUL, DIV, MOD, EQ, LT, GT, AND, OR):
 *   - Pop b first (top of stack), then pop a (second on stack)
 *   - Compute result using a and b, push result
 *   - For SUB: push a - b     (a was pushed first, b second)
 *   - For LT:  push a < b ? 1 : 0
 *   - For GT:  push a > b ? 1 : 0
 *   - For AND: push (a && b) ? 1 : 0
 *   - For OR:  push (a || b) ? 1 : 0
 *
 * For DIV and MOD:
 *   - If b == 0: set error = VM_ERR_DIV_ZERO, running = 0, return
 *
 * For JZ / JNZ:
 *   - Pop the condition value FIRST
 *   - Read the target address
 *   - If condition is met, set ip = target (don't advance ip further)
 *   - If not met, ip already advanced past the 4-byte operand
 *
 * For PRINT:
 *   - Pop the value, print it with printf("%d\n", value)
 *
 * For unknown opcodes:
 *   - Set error = VM_ERR_BAD_OPCODE, running = 0
 */
void vm_run(VM *vm)
{
    /* TODO: implement the fetch-decode-execute loop
     *
     * Skeleton:
     *
     *   while (vm->running && vm->error == VM_OK) {
     *       if (vm->ip < 0 || vm->ip >= CODE_SIZE) {
     *           vm->error = VM_ERR_IP_BOUNDS;
     *           vm->running = 0;
     *           break;
     *       }
     *
     *       uint8_t opcode = vm->code[vm->ip++];
     *
     *       switch (opcode) {
     *           case OP_NOP:
     *               break;
     *
     *           case OP_PUSH: {
     *               int32_t value = read_int32(&vm->code[vm->ip]);
     *               vm->ip += 4;
     *               vm_push(vm, value);
     *               break;
     *           }
     *
     *           case OP_POP:
     *               vm_pop(vm);
     *               break;
     *
     *           // TODO: implement all remaining opcodes ...
     *
     *           case OP_HALT:
     *               vm->running = 0;
     *               break;
     *
     *           default:
     *               vm->error = VM_ERR_BAD_OPCODE;
     *               vm->running = 0;
     *               break;
     *       }
     *   }
     */
    (void)vm;
}

/* ══════════════════════════════════════════════════════════════════════
 *  TEST HELPERS (pre-written)
 * ══════════════════════════════════════════════════════════════════════ */

/* Run a program and return the VM state for inspection */
static VM run_program(const uint8_t *code, int length)
{
    VM vm;
    vm_init(&vm);
    vm_load(&vm, code, length);
    vm_run(&vm);
    return vm;
}

/* Run a program and return the top of stack (or -99999 on error) */
static int32_t run_and_get_top(const uint8_t *code, int length)
{
    VM vm = run_program(code, length);
    if (vm.sp > 0 && vm.error == VM_OK) {
        return vm.stack[vm.sp - 1];
    }
    return -99999;
}

/* ══════════════════════════════════════════════════════════════════════
 *  TESTS — 25 tests
 * ══════════════════════════════════════════════════════════════════════ */

static void test_push_and_pop(void)
{
    printf("\n── Push and Pop ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_HALT);

        TEST("push 42 leaves 42 on stack");
        EXPECT(run_and_get_top(code, pc) == 42);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 99);
        pc = emit_op(code, pc, OP_POP);
        pc = emit_op(code, pc, OP_HALT);

        TEST("push then pop leaves empty stack");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 0 && vm.error == VM_OK);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 20);
        pc = emit_push(code, pc, 30);
        pc = emit_op(code, pc, OP_HALT);

        TEST("push 10, 20, 30 — top is 30, sp is 3");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 3 && vm.stack[0] == 10
            && vm.stack[1] == 20 && vm.stack[2] == 30);
    }
}

static void test_arithmetic(void)
{
    printf("\n── Arithmetic ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 2);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_ADD);
        pc = emit_op(code, pc, OP_HALT);

        TEST("2 + 3 = 5");
        EXPECT(run_and_get_top(code, pc) == 5);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_SUB);
        pc = emit_op(code, pc, OP_HALT);

        TEST("10 - 3 = 7");
        EXPECT(run_and_get_top(code, pc) == 7);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 4);
        pc = emit_push(code, pc, 5);
        pc = emit_op(code, pc, OP_MUL);
        pc = emit_op(code, pc, OP_HALT);

        TEST("4 * 5 = 20");
        EXPECT(run_and_get_top(code, pc) == 20);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_DIV);
        pc = emit_op(code, pc, OP_HALT);

        TEST("10 / 3 = 3 (integer division)");
        EXPECT(run_and_get_top(code, pc) == 3);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_MOD);
        pc = emit_op(code, pc, OP_HALT);

        TEST("10 %% 3 = 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_NEG);
        pc = emit_op(code, pc, OP_HALT);

        TEST("neg 42 = -42");
        EXPECT(run_and_get_top(code, pc) == -42);
    }
}

static void test_comparison(void)
{
    printf("\n── Comparison ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 5);
        pc = emit_push(code, pc, 5);
        pc = emit_op(code, pc, OP_EQ);
        pc = emit_op(code, pc, OP_HALT);

        TEST("5 == 5 => 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 5);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_EQ);
        pc = emit_op(code, pc, OP_HALT);

        TEST("5 == 3 => 0");
        EXPECT(run_and_get_top(code, pc) == 0);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 3);
        pc = emit_push(code, pc, 5);
        pc = emit_op(code, pc, OP_LT);
        pc = emit_op(code, pc, OP_HALT);

        TEST("3 < 5 => 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 7);
        pc = emit_push(code, pc, 2);
        pc = emit_op(code, pc, OP_GT);
        pc = emit_op(code, pc, OP_HALT);

        TEST("7 > 2 => 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }
}

static void test_logical(void)
{
    printf("\n── Logical ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 0);
        pc = emit_op(code, pc, OP_NOT);
        pc = emit_op(code, pc, OP_HALT);

        TEST("NOT 0 => 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 7);
        pc = emit_op(code, pc, OP_NOT);
        pc = emit_op(code, pc, OP_HALT);

        TEST("NOT 7 => 0");
        EXPECT(run_and_get_top(code, pc) == 0);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 1);
        pc = emit_push(code, pc, 1);
        pc = emit_op(code, pc, OP_AND);
        pc = emit_op(code, pc, OP_HALT);

        TEST("1 AND 1 => 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 0);
        pc = emit_push(code, pc, 1);
        pc = emit_op(code, pc, OP_OR);
        pc = emit_op(code, pc, OP_HALT);

        TEST("0 OR 1 => 1");
        EXPECT(run_and_get_top(code, pc) == 1);
    }
}

static void test_stack_ops(void)
{
    printf("\n── Stack Operations ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_DUP);
        pc = emit_op(code, pc, OP_HALT);

        TEST("DUP 42 => stack has [42, 42]");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 2 && vm.stack[0] == 42 && vm.stack[1] == 42);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 20);
        pc = emit_op(code, pc, OP_SWAP);
        pc = emit_op(code, pc, OP_HALT);

        TEST("SWAP [10, 20] => [20, 10]");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 2 && vm.stack[0] == 20 && vm.stack[1] == 10);
    }
}

static void test_jumps(void)
{
    printf("\n── Jumps ──\n");

    /* JMP: skip over a PUSH */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        /*  0: JMP to 10 */
        pc = emit_op(code, pc, OP_JMP);
        pc = emit_int32(code, pc, 10);
        /*  5: PUSH 99 (skipped) */
        pc = emit_push(code, pc, 99);
        /* 10: HALT */
        pc = emit_op(code, pc, OP_HALT);

        TEST("JMP skips PUSH — stack empty");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 0 && vm.error == VM_OK);
    }

    /* JZ: jump when zero */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        /*  0: PUSH 0 */
        pc = emit_push(code, pc, 0);
        /*  5: JZ to 15 */
        pc = emit_op(code, pc, OP_JZ);
        pc = emit_int32(code, pc, 15);
        /* 10: PUSH 99 (skipped) */
        pc = emit_push(code, pc, 99);
        /* 15: PUSH 42 */
        pc = emit_push(code, pc, 42);
        /* 20: HALT */
        pc = emit_op(code, pc, OP_HALT);

        TEST("JZ when zero — jumps, result is 42");
        EXPECT(run_and_get_top(code, pc) == 42);
    }

    /* JNZ: jump when non-zero */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        /*  0: PUSH 1 */
        pc = emit_push(code, pc, 1);
        /*  5: JNZ to 15 */
        pc = emit_op(code, pc, OP_JNZ);
        pc = emit_int32(code, pc, 15);
        /* 10: PUSH 99 (skipped) */
        pc = emit_push(code, pc, 99);
        /* 15: PUSH 77 */
        pc = emit_push(code, pc, 77);
        /* 20: HALT */
        pc = emit_op(code, pc, OP_HALT);

        TEST("JNZ when non-zero — jumps, result is 77");
        EXPECT(run_and_get_top(code, pc) == 77);
    }
}

static void test_countdown_loop(void)
{
    printf("\n── Loop: JNZ Countdown ──\n");

    /*
     * Countdown from 5 to 0 using JNZ.
     *
     *   0: PUSH 5
     * loop = 5:
     *   5: PUSH 1
     *  10: SUB            (counter - 1)
     *  11: DUP            (copy for JNZ test — JNZ pops the copy)
     *  12: JNZ 5          (if non-zero, loop back)
     *  17: HALT
     *
     * After loop finishes, stack has [0].
     */
    uint8_t code[CODE_SIZE];
    int pc = 0;

    pc = emit_push(code, pc, 5);
    int loop_addr = pc;
    pc = emit_push(code, pc, 1);
    pc = emit_op(code, pc, OP_SUB);
    pc = emit_op(code, pc, OP_DUP);
    pc = emit_op(code, pc, OP_JNZ);
    pc = emit_int32(code, pc, loop_addr);
    pc = emit_op(code, pc, OP_HALT);

    TEST("countdown 5..0 via JNZ loop — top is 0");
    EXPECT(run_and_get_top(code, pc) == 0);
}

static void test_factorial(void)
{
    printf("\n── Factorial (straight-line) ──\n");

    /*
     * Compute 5! = 120 using sequential multiplication.
     *
     * Note: a loop-based factorial requires stack operations we don't have
     * (OVER or ROT) to manipulate items deeper than the top two. Real VMs
     * like Forth and JVM include these. For now, straight-line code proves
     * the arithmetic works correctly in sequence.
     *
     *   PUSH 5, PUSH 4, MUL, PUSH 3, MUL, PUSH 2, MUL, PUSH 1, MUL
     */
    uint8_t code[CODE_SIZE];
    int pc = 0;

    pc = emit_push(code, pc, 5);
    pc = emit_push(code, pc, 4);
    pc = emit_op(code, pc, OP_MUL);
    pc = emit_push(code, pc, 3);
    pc = emit_op(code, pc, OP_MUL);
    pc = emit_push(code, pc, 2);
    pc = emit_op(code, pc, OP_MUL);
    pc = emit_push(code, pc, 1);
    pc = emit_op(code, pc, OP_MUL);
    pc = emit_op(code, pc, OP_HALT);

    TEST("5! = 120");
    EXPECT(run_and_get_top(code, pc) == 120);
}

static void test_sum_1_to_10(void)
{
    printf("\n── Sum 1..10 ──\n");

    /*
     * Sum 1..10 = 55 using sequential addition.
     * Same note as factorial: a loop version needs OVER/ROT.
     */
    uint8_t code[CODE_SIZE];
    int pc = 0;

    pc = emit_push(code, pc, 1);
    pc = emit_push(code, pc, 2);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 3);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 4);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 5);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 6);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 7);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 8);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 9);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_push(code, pc, 10);
    pc = emit_op(code, pc, OP_ADD);
    pc = emit_op(code, pc, OP_HALT);

    TEST("sum 1..10 = 55");
    EXPECT(run_and_get_top(code, pc) == 55);
}

static void test_compound_expression(void)
{
    printf("\n── Compound Expression ──\n");

    /*
     * Compute (8 + 3) * 5 = 55.
     * Demonstrates how a stack VM evaluates nested expressions:
     * push operands, apply operators in the right order.
     */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 8);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_ADD);
        pc = emit_push(code, pc, 5);
        pc = emit_op(code, pc, OP_MUL);
        pc = emit_op(code, pc, OP_HALT);

        TEST("(8 + 3) * 5 = 55");
        EXPECT(run_and_get_top(code, pc) == 55);
    }

    /*
     * Compute (10 - 3) * (2 + 4) = 42.
     * Both sub-expressions evaluate on the stack, then MUL combines them.
     */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_SUB);
        pc = emit_push(code, pc, 2);
        pc = emit_push(code, pc, 4);
        pc = emit_op(code, pc, OP_ADD);
        pc = emit_op(code, pc, OP_MUL);
        pc = emit_op(code, pc, OP_HALT);

        TEST("(10 - 3) * (2 + 4) = 42");
        EXPECT(run_and_get_top(code, pc) == 42);
    }
}

static void test_error_handling(void)
{
    printf("\n── Error Handling ──\n");

    /* Stack underflow */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_op(code, pc, OP_POP);
        pc = emit_op(code, pc, OP_HALT);

        TEST("stack underflow on empty pop");
        VM vm = run_program(code, pc);
        EXPECT(vm.error == VM_ERR_UNDERFLOW);
    }

    /* Stack overflow: DUP in a tight loop */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 1);
        int loop = pc;
        pc = emit_op(code, pc, OP_DUP);
        pc = emit_op(code, pc, OP_JNZ);
        pc = emit_int32(code, pc, loop);

        TEST("stack overflow via DUP loop");
        VM vm = run_program(code, pc);
        EXPECT(vm.error == VM_ERR_OVERFLOW);
    }

    /* Division by zero */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 0);
        pc = emit_op(code, pc, OP_DIV);
        pc = emit_op(code, pc, OP_HALT);

        TEST("division by zero");
        VM vm = run_program(code, pc);
        EXPECT(vm.error == VM_ERR_DIV_ZERO);
    }

    /* Modulo by zero */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 10);
        pc = emit_push(code, pc, 0);
        pc = emit_op(code, pc, OP_MOD);
        pc = emit_op(code, pc, OP_HALT);

        TEST("modulo by zero");
        VM vm = run_program(code, pc);
        EXPECT(vm.error == VM_ERR_DIV_ZERO);
    }

    /* HALT stops cleanly */
    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_HALT);
        pc = emit_push(code, pc, 99);   /* should never execute */
        pc = emit_op(code, pc, OP_ADD);

        TEST("HALT stops — only 42 on stack");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 1 && vm.stack[0] == 42 && vm.error == VM_OK);
    }
}

static void test_print(void)
{
    printf("\n── Print ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_PRINT);
        pc = emit_op(code, pc, OP_HALT);

        printf("    (expect VM to print: 42)\n    VM output: ");
        fflush(stdout);

        TEST("PRINT pops value — stack empty after");
        VM vm = run_program(code, pc);
        EXPECT(vm.sp == 0 && vm.error == VM_OK);
    }
}

static void test_negative_numbers(void)
{
    printf("\n── Negative Numbers ──\n");

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, -7);
        pc = emit_op(code, pc, OP_HALT);

        TEST("push -7");
        EXPECT(run_and_get_top(code, pc) == -7);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 3);
        pc = emit_push(code, pc, 10);
        pc = emit_op(code, pc, OP_SUB);
        pc = emit_op(code, pc, OP_HALT);

        TEST("3 - 10 = -7");
        EXPECT(run_and_get_top(code, pc) == -7);
    }

    {
        uint8_t code[CODE_SIZE];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_NEG);
        pc = emit_op(code, pc, OP_NEG);
        pc = emit_op(code, pc, OP_HALT);

        TEST("neg(neg(42)) = 42");
        EXPECT(run_and_get_top(code, pc) == 42);
    }
}

/* ── Main ─────────────────────────────────────────────────────────── */

int main(void)
{
    printf("======================================================\n");
    printf("  Module 12: Bytecode Virtual Machine\n");
    printf("======================================================\n");

    test_push_and_pop();
    test_arithmetic();
    test_comparison();
    test_logical();
    test_stack_ops();
    test_jumps();
    test_countdown_loop();
    test_factorial();
    test_sum_1_to_10();
    test_compound_expression();
    test_error_handling();
    test_print();
    test_negative_numbers();

    printf("\n======================================================\n");
    printf("  Results: %d / %d tests passed\n", tests_passed, tests_total);
    printf("======================================================\n");

    return (tests_passed == tests_total) ? 0 : 1;
}
