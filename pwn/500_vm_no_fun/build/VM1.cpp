#include <unistd.h>
#include <cstdio>
#include <cstdint>
#include <csignal>

#include "IO.h"
#include "VM1.h"

#define MEM_SIZE 0xffff

#define OPERAND_REG   0
#define OPERAND_IMM   1
#define OPERAND_MEM   2
#define OPERAND_STACK 3

#define OP_MASK 31

#define OP_WORD 32
#define OP_BYTE 64

#define OPCODE_NOP       0x90
#define OPCODE_MOV       0x89
#define OPCODE_ADD       0x1
#define OPCODE_SUB       0x29
#define OPCODE_XOR       0x31
#define OPCODE_MUL       0xf6
#define OPCODE_DIV       0xf7
#define OPCODE_PUSH_WORD 0x50
#define OPCODE_POP_WORD  0x57
#define OPCODE_PUSH_BYTE 0x51
#define OPCODE_POP_BYTE  0x58
#define OPCODE_HLT       0xf4
#define OPCODE_OUT       0x0b
#define OPCODE_IN        0x0c
#define OPCODE_CMP_WORD  0x38
#define OPCODE_CMP_BYTE  0x39
#define OPCODE_CALL      0xe8
#define OPCODE_RET       0xc3
#define OPCODE_JE        0x4f
#define OPCODE_JNE       0x5f
#define OPCODE_JMP       0xe9

#define REG_AX    0
#define REG_BX    1
#define REG_CX    2
#define REG_DX    3
#define REG_CS    4
#define REG_DS    5
#define REG_SS    6
#define REG_ES    7
#define REG_SI    8
#define REG_DI    9
#define REG_BP    10
#define REG_SP    11
#define REG_FLAGS 12
#define REG_IP    13
#define GPREG_COUNT 11

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define ax (vm1_regs[REG_AX])
#define bx (vm1_regs[REG_BX])
#define cx (vm1_regs[REG_CX])
#define dx (vm1_regs[REG_DX])
#define cs (vm1_regs[REG_CS])
#define ds (vm1_regs[REG_DS])
#define ss (vm1_regs[REG_SS])
#define es (vm1_regs[REG_ES])
#define si (vm1_regs[REG_SI])
#define di (vm1_regs[REG_DI])
#define bp (vm1_regs[REG_BP])
#define sp (vm1_regs[REG_SP])
#define flags (vm1_regs[REG_FLAGS])
#define ip (vm1_regs[REG_IP])
#pragma clang diagnostic pop

uint8_t vm1_mem[MEM_SIZE];
uint16_t vm1_regs[14];

int vm1_do_output = 0;
int vm1_do_input = 0;

int vm1_execute_one_instruction();

uint16_t *vm1_dest_op_word(uint8_t type, uint16_t val);

uint16_t vm1_src_op_word(uint8_t type, uint16_t val);

uint8_t *vm1_dest_op_byte(uint8_t type, uint16_t val);

uint8_t vm1_src_op_byte(uint8_t type, uint16_t val);

void init_VM1() {
    cs = 0;
    ip = 0;

    ds = 0x400;

    es = 0x700;

    ss = 0x900;
    sp = 0x7000;
}

void run_VM1() {
    puts("A");

    readAll(0, vm1_mem, 0x4000);

    cs = 0;
    ip = 0;

    while (vm1_execute_one_instruction()) {
        if (vm1_do_output) {
            if (es * 16 > MEM_SIZE)
                raise(SIGSEGV);
            if (es * 16 + ax > MEM_SIZE)
                raise(SIGSEGV);

            write(1, &vm1_mem[es * 16], ax);

            vm1_do_output = 0;
        } else if (vm1_do_input) {
            if (es * 16 + 0x1000 + 0x1000 > MEM_SIZE)
                raise(SIGSEGV);

            if (!readAll(0, &vm1_mem[es * 16 + 0x1000], 0x1000))
                break;

            vm1_do_input = 0;
        }
    }

    puts("\xff"); // NOLINT
}

int vm1_execute_one_instruction() {
    uint8_t opcode, operand_count;
    uint8_t operand_1_type, operand_2_type;
    uint16_t operand_1, operand_2;

    uint16_t *dest_word = nullptr;
    uint16_t src_word = 0;

    uint8_t *dest = nullptr;
    uint8_t src = 0;

    if (cs * 16 + ip > MEM_SIZE)
        raise(SIGSEGV);
    opcode = vm1_mem[cs * 16 + ip++];

    if (cs * 16 + ip > MEM_SIZE)
        raise(SIGSEGV);
    operand_count = vm1_mem[cs * 16 + ip++];

    if (operand_count >= 1) {
        if (cs * 16 + ip > MEM_SIZE)
            raise(SIGSEGV);
        operand_1_type = vm1_mem[cs * 16 + ip++];

        if (cs * 16 + ip > MEM_SIZE)
            raise(SIGSEGV);
        operand_1 = vm1_mem[cs * 16 + ip++] | vm1_mem[cs * 16 + ip++] << 8;

        if (operand_1_type & OP_WORD)
            dest_word = vm1_dest_op_word((uint8_t) (operand_1_type & OP_MASK), operand_1);
        else if (operand_1_type & OP_BYTE)
            dest = vm1_dest_op_byte((uint8_t) (operand_1_type & OP_MASK), operand_1);
    }
    if (operand_count >= 2) {
        if (cs * 16 + ip > MEM_SIZE)
            raise(SIGSEGV);
        operand_2_type = vm1_mem[cs * 16 + ip++];

        if (cs * 16 + ip > MEM_SIZE)
            raise(SIGSEGV);
        operand_2 = vm1_mem[cs * 16 + ip++] | vm1_mem[cs * 16 + ip++] << 8;

        if (operand_2_type & OP_WORD)
            src_word = vm1_src_op_word((uint8_t) (operand_2_type & OP_MASK), operand_2);
        else if (operand_2_type & OP_BYTE)
            src = vm1_src_op_byte((uint8_t) (operand_2_type & OP_MASK), operand_2);
    }

    if (cs * 16 + ip > MEM_SIZE)
        raise(SIGSEGV);

    switch (opcode) {
        case OPCODE_NOP:
            break;

        case OPCODE_MOV:
            if (dest_word)
                *dest_word = src_word;
            else if (dest)
                *dest = src;
            else
                raise(SIGILL);
            break;

        case OPCODE_ADD:
            if (dest_word)
                *dest_word += src_word;
            else if (dest)
                *dest += src;
            else
                raise(SIGILL);
            break;

        case OPCODE_SUB:
            if (dest_word)
                *dest_word -= src_word;
            else if (dest)
                *dest -= src;
            else
                raise(SIGILL);
            break;

        case OPCODE_XOR:
            if (dest_word)
                *dest_word ^= src_word;
            else if (dest)
                *dest ^= src;
            else
                raise(SIGILL);
            break;

        case OPCODE_PUSH_WORD:
            sp -= 2;

            if (ss * 16 + sp < 0)
                raise(SIGSEGV);

            *((uint16_t *) &vm1_mem[ss * 16 + sp]) = src_word;
            break;

        case OPCODE_POP_WORD:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            if (dest_word)
                *dest_word = *((uint16_t *) &vm1_mem[ss * 16 + sp]);
            else
                raise(SIGILL);

            sp += 2;
            break;

        case OPCODE_HLT:
            return 0;

        case OPCODE_OUT:
            vm1_do_output = 1;
            break;

        case OPCODE_IN:
            vm1_do_input = 1;
            break;

        case OPCODE_CMP_WORD:
            if (dest_word)
                flags = (uint16_t) (*dest_word == src_word);
            break;

        case OPCODE_CALL:
            sp -= 2;

            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            *((uint16_t *) &vm1_mem[ss * 16 + sp]) = ip;
            ip += src_word;
            break;

        case OPCODE_RET:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            ip = *((uint16_t *) &vm1_mem[ss * 16 + sp]);
            sp += 2;
            break;

        case OPCODE_JE:
            if (flags)
                ip += src_word;
            break;

        case OPCODE_JNE:
            if (!flags)
                ip += src_word;
            break;

        case OPCODE_JMP:
            ip += src_word;
            break;

        default:
            raise(SIGILL);
    }

    if (cs * 16 + ip >= MEM_SIZE)
        raise(SIGSEGV);

    return 1;
}

uint16_t *vm1_dest_op_word(uint8_t type, uint16_t val) {
    uint16_t *tmp = nullptr;

    switch (type) {
        case OPERAND_REG:
            if (val > 11)
                raise(SIGILL);
            tmp = &vm1_regs[val];
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint16_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = (uint16_t *) &vm1_mem[ds * 16 + val];
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint16_t vm1_src_op_word(uint8_t type, uint16_t val) {
    uint16_t tmp = 0;

    switch (type) {
        case OPERAND_REG:
            if (val > 11)
                raise(SIGILL);
            tmp = vm1_regs[val];
            break;

        case OPERAND_IMM:
            tmp = val;
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint16_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = *((uint16_t *) &vm1_mem[ds * 16 + val]);
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint8_t *vm1_dest_op_byte(uint8_t type, uint16_t val) {
    uint8_t *tmp = nullptr;

    switch (type) {
        case OPERAND_REG:
            if (val > GPREG_COUNT)
                raise(SIGILL);
            tmp = (uint8_t *) &vm1_regs[val];
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint8_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = &vm1_mem[ds * 16 + val];
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint8_t vm1_src_op_byte(uint8_t type, uint16_t val) {
    uint8_t tmp = 0;

    switch (type) {
        case OPERAND_REG:
            if (val > GPREG_COUNT)
                raise(SIGILL);
            tmp = (uint8_t) vm1_regs[val];
            break;

        case OPERAND_IMM:
            tmp = (uint8_t) val;
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint8_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = vm1_mem[ds * 16 + val];
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}
