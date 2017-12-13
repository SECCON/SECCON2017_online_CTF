#include <unistd.h>
#include <cstdio>
#include <cstdint>
#include <csignal>
#include <cstring>

#include "IO.h"
#include "VM2.h"

#define MEM_SIZE 0xffff

#define OPERAND_REG   0
#define OPERAND_IMM   1
#define OPERAND_MEM   2
#define OPERAND_STACK 3

#define OP_MASK 31

#define OP_WORD 32
#define OP_BYTE 64

#define OPCODE_NOP       0x92
#define OPCODE_MOV       0x28
#define OPCODE_ADD       0x82
#define OPCODE_SUB       0xb4
#define OPCODE_XOR       0x29
#define OPCODE_MUL       0x60
#define OPCODE_DIV       0x7e
#define OPCODE_PUSH_WORD 0x88
#define OPCODE_POP_WORD  0x20
#define OPCODE_PUSH_BYTE 0x93
#define OPCODE_POP_BYTE  0xda
#define OPCODE_HLT       0x83
#define OPCODE_OUT       0x85
#define OPCODE_IN        0xdb
#define OPCODE_CMP_WORD  0xec
#define OPCODE_CMP_BYTE  0x04
#define OPCODE_CALL      0x8a
#define OPCODE_RET       0xbc
#define OPCODE_FAR_RET   0xbd
#define OPCODE_JE        0x51
#define OPCODE_JNE       0x57
#define OPCODE_JMP       0x75
#define OPCODE_STOSB     0xc0
#define OPCODE_MOVSB     0xc1

#define REG_AX    11
#define REG_BX    3
#define REG_CX    13
#define REG_DX    5
#define REG_CS    8
#define REG_DS    10
#define REG_SS    1
#define REG_ES    7
#define REG_SI    0
#define REG_DI    2
#define REG_BP    12
#define REG_SP    9
#define REG_FLAGS 4
#define REG_IP    6
#define GPREG_COUNT 11

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define ax (vm2_regs[REG_AX])
#define bx (vm2_regs[REG_BX])
#define cx (vm2_regs[REG_CX])
#define dx (vm2_regs[REG_DX])
#define cs (vm2_regs[REG_CS])
#define ds (vm2_regs[REG_DS])
#define ss (vm2_regs[REG_SS])
#define es (vm2_regs[REG_ES])
#define si (vm2_regs[REG_SI])
#define di (vm2_regs[REG_DI])
#define bp (vm2_regs[REG_BP])
#define sp (vm2_regs[REG_SP])
#define flags (vm2_regs[REG_FLAGS])
#define ip (vm2_regs[REG_IP])
#pragma clang diagnostic pop

uint8_t vm2_mem[MEM_SIZE];
uint32_t vm2_regs[14];

int vm2_do_output = 0;
int vm2_do_input = 0;

int vm2_execute_one_instruction();

uint32_t *vm2_dest_op_word(uint8_t type, uint32_t val);

uint32_t vm2_src_op_word(uint8_t type, uint32_t val);

uint8_t *vm2_dest_op_byte(uint8_t type, uint32_t val);

uint8_t vm2_src_op_byte(uint8_t type, uint32_t val);

void init_VM2() {
    cs = 0;
    ip = 0;

    ds = 0x400;

    es = 0x700;

    ss = 0x900;
    sp = 0x7000;
}

void run_VM2() {
    puts("B");

    cs = 0;
    ip = 0;

    readAll_vm1(vm2_mem, 0x1000);

    while (vm2_execute_one_instruction()) {
        if (vm2_do_output) {
            if (es * 16 > MEM_SIZE)
                raise(SIGSEGV);
            if (es * 16 + ax > MEM_SIZE)
                raise(SIGSEGV);

            write_vm1(&vm2_mem[es * 16], ax);

            vm2_do_output = 0;
        } else if (vm2_do_input) {
            if (es * 16 + 0x1000 + 0x1000 > MEM_SIZE)
                raise(SIGSEGV);

            readAll_vm1(&vm2_mem[es * 16 + 0x1000], 0x1000);

            vm2_do_input = 0;
        }
    }

    puts("\xfe"); // NOLINT
}

int vm2_execute_one_instruction() {
    uint8_t opcode;
    uint8_t operand_1_type, operand_2_type;
    uint32_t operand_1, operand_2;

    uint32_t *dest_word = nullptr;
    uint32_t src_word = 0;

    uint8_t *dest = nullptr;
    uint8_t src = 0;

    if (cs * 16 + ip > MEM_SIZE)
        raise(SIGSEGV);

    opcode = vm2_mem[cs * 16 + ip++];

    operand_1 = vm2_mem[cs * 16 + ip++] |
                vm2_mem[cs * 16 + ip++] << 8 |
                vm2_mem[cs * 16 + ip++] << 16 |
                vm2_mem[cs * 16 + ip++] << 24;
    operand_1_type = vm2_mem[cs * 16 + ip++];

    operand_2 = vm2_mem[cs * 16 + ip++] |
                vm2_mem[cs * 16 + ip++] << 8 |
                vm2_mem[cs * 16 + ip++] << 16 |
                vm2_mem[cs * 16 + ip++] << 24;
    operand_2_type = vm2_mem[cs * 16 + ip++];

    if (operand_1_type & OP_WORD)
        dest_word = vm2_dest_op_word((uint8_t) (operand_1_type & OP_MASK), operand_1);
    else if (operand_1_type & OP_BYTE)
        dest = vm2_dest_op_byte((uint8_t) (operand_1_type & OP_MASK), operand_1);

    if (operand_2_type & OP_WORD)
        src_word = vm2_src_op_word((uint8_t) (operand_2_type & OP_MASK), operand_2);
    else if (operand_2_type & OP_BYTE)
        src = vm2_src_op_byte((uint8_t) (operand_2_type & OP_MASK), operand_2);

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

        case OPCODE_PUSH_WORD:
            sp -= 4;

            if (ss * 16 + sp < 0)
                raise(SIGSEGV);

            *((uint32_t *) &vm2_mem[ss * 16 + sp]) = src_word;
            break;

        case OPCODE_POP_WORD:
            if (ss * 16 + (int64_t)(int)sp > MEM_SIZE)
                raise(SIGSEGV);

            if (dest_word)
                *dest_word = *((uint32_t *) &vm2_mem[ss * 16 + (int64_t)(int)sp]);
            else
                raise(SIGILL);

            sp += 4;
            break;

        case OPCODE_PUSH_BYTE:
            sp -= 1;

            if (ss * 16 + sp < 0)
                raise(SIGSEGV);

            vm2_mem[ss * 16 + sp] = (uint8_t) src_word;
            break;

        case OPCODE_POP_BYTE:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            if (dest)
                *dest = vm2_mem[ss * 16 + sp];
            else
                raise(SIGILL);
            sp += 1;
            break;

        case OPCODE_HLT:
            return 0;

        case OPCODE_OUT:
            vm2_do_output = 1;
            break;

        case OPCODE_IN:
            vm2_do_input = 1;
            break;

        case OPCODE_CMP_WORD:
            if (dest_word)
                flags = (uint32_t) (*dest_word == src_word);
            break;

        case OPCODE_CMP_BYTE:
            if (dest)
                flags = (uint32_t) (*dest == src);
            break;

        case OPCODE_CALL:
            sp -= 4;

            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            *((uint32_t *) &vm2_mem[ss * 16 + sp]) = ip;
            ip = src_word;
            break;

        case OPCODE_RET:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            ip = *((uint32_t *) &vm2_mem[ss * 16 + sp]);
            sp += 4;
            break;

        case OPCODE_FAR_RET:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            cs = *((uint32_t *) &vm2_mem[ss * 16 + sp]);
            sp += 4;

            ip = *((uint32_t *) &vm2_mem[ss * 16 + sp]);
            sp += 4;
            break;

        case OPCODE_JE:
            if (flags)
                ip = src_word;
            break;

        case OPCODE_JNE:
            if (!flags)
                ip = src_word;
            break;

        case OPCODE_JMP:
            ip = src_word;
            break;

        case OPCODE_STOSB:
            if (ds * 16 + (int16_t)di < 0 || ds * 16 + (int16_t)di > MEM_SIZE)
                raise(SIGSEGV);

            memset(&vm2_mem[ds * 16 + (int16_t)di], ax & 0xff, 1);
            di++;
            break;

        case OPCODE_MOVSB:
            if (ds * 16 + (int16_t)di < 0 || ds * 16 + (int16_t)di > MEM_SIZE)
                raise(SIGSEGV);
            if (ds * 16 + (int16_t)si < 0 || ds * 16 + (int16_t)si > MEM_SIZE)
                raise(SIGSEGV);

            memcpy(&vm2_mem[ds * 16 + (int16_t)di], &vm2_mem[ds * 16 + (int16_t)si], 1);
            di++;
            si++;
            break;

        default:
            raise(SIGILL);
    }

    if (cs * 16 + ip >= MEM_SIZE)
        raise(SIGSEGV);

    return 1;
}

uint32_t *vm2_dest_op_word(uint8_t type, uint32_t val) {
    uint32_t *tmp = nullptr;

    switch (type) {
        case OPERAND_REG:
            if (val > 11)
                raise(SIGILL);
            tmp = &vm2_regs[val];
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint32_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = (uint32_t *) &vm2_mem[ds * 16 + val];
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint32_t vm2_src_op_word(uint8_t type, uint32_t val) {
    uint32_t tmp = 0;

    switch (type) {
        case OPERAND_REG:
            if (val > 11)
                raise(SIGILL);
            tmp = vm2_regs[val];
            break;

        case OPERAND_IMM:
            tmp = val;
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint32_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = *((uint32_t *) &vm2_mem[ds * 16 + val]);
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint8_t *vm2_dest_op_byte(uint8_t type, uint32_t val) {
    uint8_t *tmp = nullptr;

    switch (type) {
        case OPERAND_REG:
            if (val > GPREG_COUNT)
                raise(SIGILL);
            tmp = (uint8_t *) &vm2_regs[val];
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint8_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = &vm2_mem[ds * 16 + val];
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint8_t vm2_src_op_byte(uint8_t type, uint32_t val) {
    uint8_t tmp = 0;

    switch (type) {
        case OPERAND_REG:
            if (val > GPREG_COUNT)
                raise(SIGILL);
            tmp = (uint8_t) vm2_regs[val];
            break;

        case OPERAND_IMM:
            tmp = (uint8_t) val;
            break;

        case OPERAND_MEM:
            if (ds * 16 + val + sizeof(uint8_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = vm2_mem[ds * 16 + val];
            break;

        case OPERAND_STACK:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}
