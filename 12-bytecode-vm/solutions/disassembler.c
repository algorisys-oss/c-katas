/*
 * Kata: Bytecode Disassembler
 *
 * Build a disassembler that takes raw bytecode and prints
 * human-readable assembly. This is the reverse of an assembler:
 * bytes in, readable text out.
 *
 * Example output:
 *   0000: PUSH       42
 *   0005: PUSH       3
 *   000a: ADD
 *   000b: PRINT
 *   000c: HALT
 *
 * For each instruction:
 *   - Print the address (hex, 4 digits, zero-padded)
 *   - Print the opcode name
 *   - For PUSH/JMP/JZ/JNZ: also print the operand
 *
 * 6 tests total.
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

/* ── Opcodes (same as vm.c) ───────────────────────────────────────── */

typedef enum {
    OP_NOP   = 0x00,
    OP_PUSH  = 0x01,
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
    OP_JMP   = 0x0F,
    OP_JZ    = 0x10,
    OP_JNZ   = 0x11,
    OP_DUP   = 0x12,
    OP_SWAP  = 0x13,
    OP_PRINT = 0x14,
    OP_HALT  = 0x15
} Opcode;

/* ── Bytecode Builder Helpers (same as vm.c) ──────────────────────── */

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

static int32_t read_int32(const uint8_t *bytes)
{
    return (int32_t)(
        (uint32_t)bytes[0] |
        ((uint32_t)bytes[1] << 8) |
        ((uint32_t)bytes[2] << 16) |
        ((uint32_t)bytes[3] << 24)
    );
}

/* ── Exercise 1: opcode_name ──────────────────────────────────────── */
const char *opcode_name(uint8_t op)
{
    switch (op) {
        case OP_NOP:   return "NOP";
        case OP_PUSH:  return "PUSH";
        case OP_POP:   return "POP";
        case OP_ADD:   return "ADD";
        case OP_SUB:   return "SUB";
        case OP_MUL:   return "MUL";
        case OP_DIV:   return "DIV";
        case OP_MOD:   return "MOD";
        case OP_NEG:   return "NEG";
        case OP_EQ:    return "EQ";
        case OP_LT:    return "LT";
        case OP_GT:    return "GT";
        case OP_NOT:   return "NOT";
        case OP_AND:   return "AND";
        case OP_OR:    return "OR";
        case OP_JMP:   return "JMP";
        case OP_JZ:    return "JZ";
        case OP_JNZ:   return "JNZ";
        case OP_DUP:   return "DUP";
        case OP_SWAP:  return "SWAP";
        case OP_PRINT: return "PRINT";
        case OP_HALT:  return "HALT";
        default:       return "???";
    }
}

/* ── Exercise 2: opcode_has_operand ───────────────────────────────── */
int opcode_has_operand(uint8_t op)
{
    return (op == OP_PUSH || op == OP_JMP || op == OP_JZ || op == OP_JNZ)
        ? 1 : 0;
}

/* ── Exercise 3: disassemble ──────────────────────────────────────── */
int disassemble(const uint8_t *code, int length)
{
    int offset = 0;
    int count = 0;

    while (offset < length) {
        printf("%04x: ", offset);
        uint8_t op = code[offset++];
        printf("%-10s", opcode_name(op));

        if (opcode_has_operand(op)) {
            int32_t value = read_int32(&code[offset]);
            offset += 4;
            if (op == OP_PUSH) {
                printf("%d", value);
            } else {
                printf("0x%04x", value);
            }
        }
        printf("\n");
        count++;
    }

    return count;
}

/* ── Exercise 4: disassemble_to_buffer ────────────────────────────── */
int disassemble_to_buffer(const uint8_t *code, int length,
                          char *buf, int buf_size)
{
    if (buf_size > 0) buf[0] = '\0';
    if (code == NULL || length <= 0) return 0;

    int offset = 0;
    int count = 0;
    int pos = 0;

    while (offset < length) {
        uint8_t op = code[offset];
        int written = 0;

        if (opcode_has_operand(op)) {
            int32_t value = read_int32(&code[offset + 1]);
            if (op == OP_PUSH) {
                written = snprintf(buf + pos, buf_size - pos,
                    "%04x: %-10s%d\n", offset, opcode_name(op), value);
            } else {
                written = snprintf(buf + pos, buf_size - pos,
                    "%04x: %-10s0x%04x\n", offset, opcode_name(op), value);
            }
            offset += 5;
        } else {
            written = snprintf(buf + pos, buf_size - pos,
                "%04x: %-10s\n", offset, opcode_name(op));
            offset += 1;
        }

        if (written > 0 && pos + written < buf_size) {
            pos += written;
        }
        count++;
    }

    return count;
}

/* ══════════════════════════════════════════════════════════════════════
 *  TESTS — 6 tests
 * ══════════════════════════════════════════════════════════════════════ */

static void test_empty_program(void)
{
    printf("\n── Empty Program ──\n");

    {
        char buf[1024] = {0};
        int count = disassemble_to_buffer(NULL, 0, buf, sizeof(buf));

        TEST("empty program: 0 instructions");
        EXPECT(count == 0 && buf[0] == '\0');
    }
}

static void test_single_instruction(void)
{
    printf("\n── Single Instruction ──\n");

    {
        uint8_t code[16];
        int pc = 0;
        pc = emit_op(code, pc, OP_HALT);

        char buf[1024] = {0};
        int count = disassemble_to_buffer(code, pc, buf, sizeof(buf));

        TEST("HALT: 1 instruction, correct format");
        EXPECT(count == 1 && strstr(buf, "0000:") != NULL
            && strstr(buf, "HALT") != NULL);
    }
}

static void test_arithmetic_program(void)
{
    printf("\n── Arithmetic Program ──\n");

    {
        uint8_t code[64];
        int pc = 0;
        pc = emit_push(code, pc, 42);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_ADD);
        pc = emit_op(code, pc, OP_PRINT);
        pc = emit_op(code, pc, OP_HALT);

        char buf[1024] = {0};
        int count = disassemble_to_buffer(code, pc, buf, sizeof(buf));

        TEST("PUSH 42; PUSH 3; ADD; PRINT; HALT = 5 instructions");
        EXPECT(count == 5);

        TEST("output contains PUSH with value 42");
        EXPECT(strstr(buf, "PUSH") != NULL && strstr(buf, "42") != NULL);
    }
}

static void test_jump_program(void)
{
    printf("\n── Jump Program ──\n");

    {
        uint8_t code[64];
        int pc = 0;
        pc = emit_push(code, pc, 0);
        pc = emit_op(code, pc, OP_JZ);
        pc = emit_int32(code, pc, 15);
        pc = emit_push(code, pc, 99);
        pc = emit_push(code, pc, 42);
        pc = emit_op(code, pc, OP_HALT);

        char buf[1024] = {0};
        int count = disassemble_to_buffer(code, pc, buf, sizeof(buf));

        TEST("JZ program: address shows as hex, 5 instructions");
        EXPECT(count == 5 && strstr(buf, "JZ") != NULL
            && strstr(buf, "0x000f") != NULL);
    }
}

static void test_all_simple_opcodes(void)
{
    printf("\n── All Simple Opcodes ──\n");

    {
        uint8_t code[64];
        int pc = 0;
        pc = emit_op(code, pc, OP_NOP);
        pc = emit_op(code, pc, OP_POP);
        pc = emit_op(code, pc, OP_ADD);
        pc = emit_op(code, pc, OP_SUB);
        pc = emit_op(code, pc, OP_MUL);
        pc = emit_op(code, pc, OP_DIV);
        pc = emit_op(code, pc, OP_MOD);
        pc = emit_op(code, pc, OP_NEG);
        pc = emit_op(code, pc, OP_EQ);
        pc = emit_op(code, pc, OP_LT);
        pc = emit_op(code, pc, OP_GT);
        pc = emit_op(code, pc, OP_NOT);
        pc = emit_op(code, pc, OP_AND);
        pc = emit_op(code, pc, OP_OR);
        pc = emit_op(code, pc, OP_DUP);
        pc = emit_op(code, pc, OP_SWAP);
        pc = emit_op(code, pc, OP_PRINT);
        pc = emit_op(code, pc, OP_HALT);

        char buf[2048] = {0};
        int count = disassemble_to_buffer(code, pc, buf, sizeof(buf));

        TEST("all 18 simple opcodes disassemble correctly");
        EXPECT(count == 18
            && strstr(buf, "NOP") != NULL
            && strstr(buf, "ADD") != NULL
            && strstr(buf, "HALT") != NULL
            && strstr(buf, "SWAP") != NULL);
    }
}

static void test_disassemble_prints(void)
{
    printf("\n── Disassemble to stdout ──\n");

    {
        uint8_t code[64];
        int pc = 0;
        pc = emit_push(code, pc, 2);
        pc = emit_push(code, pc, 3);
        pc = emit_op(code, pc, OP_ADD);
        pc = emit_op(code, pc, OP_HALT);

        printf("\n    Expected output:\n");
        printf("    0000: PUSH      2\n");
        printf("    0005: PUSH      3\n");
        printf("    000a: ADD\n");
        printf("    000b: HALT\n");
        printf("\n    Actual output:\n    ");

        int count = disassemble(code, pc);
        printf("\n");

        TEST("disassemble() returns correct count");
        EXPECT(count == 4);
    }
}

/* ── Main ─────────────────────────────────────────────────────────── */

int main(void)
{
    printf("======================================================\n");
    printf("  Module 12: Bytecode Disassembler\n");
    printf("======================================================\n");

    test_empty_program();
    test_single_instruction();
    test_arithmetic_program();
    test_jump_program();
    test_all_simple_opcodes();
    test_disassemble_prints();

    printf("\n======================================================\n");
    printf("  Results: %d / %d tests passed\n", tests_passed, tests_total);
    printf("======================================================\n");

    return (tests_passed == tests_total) ? 0 : 1;
}
