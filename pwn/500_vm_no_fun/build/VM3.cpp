#include <unistd.h>
#include <cstdio>
#include <cstdint>
#include <csignal>
#include <cstdlib>

#include "IO.h"
#include "VM3.h"

#define MEM_SIZE 0xffff

#define OP1_TYPE  ((uint16_t)(3 << 8))
#define OP1_REG   ((uint16_t)(1 << 8))
#define OP1_IMM   ((uint16_t)(2 << 8))
#define OP1_MEM   ((uint16_t)(3 << 8))

#define OP1_SIZE  ((uint16_t)(3 << 10))
#define OP1_WORD  ((uint16_t)(1 << 10))
#define OP1_BYTE  ((uint16_t)(2 << 10))

#define OP2_TYPE  ((uint16_t)(3 << 12))
#define OP2_REG   ((uint16_t)(1 << 12))
#define OP2_IMM   ((uint16_t)(2 << 12))
#define OP2_MEM   ((uint16_t)(3 << 12))

#define OP2_SIZE  ((uint16_t)(3 << 14))
#define OP2_WORD  ((uint16_t)(1 << 14))
#define OP2_BYTE  ((uint16_t)(2 << 14))

#define OPCODE_NOP       0
#define OPCODE_MOV       1
#define OPCODE_ADD       2
#define OPCODE_SUB       3
#define OPCODE_XOR       4
#define OPCODE_MUL       5
#define OPCODE_DIV       6
#define OPCODE_PUSH_WORD 7
#define OPCODE_POP_WORD  8
#define OPCODE_PUSH_BYTE 9
#define OPCODE_POP_BYTE  10
#define OPCODE_HLT       11
#define OPCODE_OUT       12
#define OPCODE_IN        13
#define OPCODE_CMP_WORD  14
#define OPCODE_CMP_BYTE  15
#define OPCODE_CALL      16
#define OPCODE_RET       17
#define OPCODE_JE        18
#define OPCODE_JNE       19
#define OPCODE_JMP       20
#define OPCODE_RDRAND    21

#define REG_AX    10
#define REG_BX    6
#define REG_CX    1
#define REG_DX    2
#define REG_CS    9
#define REG_DS    3
#define REG_SS    0
#define REG_ES    4
#define REG_SI    13
#define REG_DI    11
#define REG_BP    8
#define REG_SP    7
#define REG_FLAGS 12
#define REG_IP    5
#define GPREG_COUNT 11

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define ax (vm3_regs[REG_AX])
#define bx (vm3_regs[REG_BX])
#define cx (vm3_regs[REG_CX])
#define dx (vm3_regs[REG_DX])
#define cs (vm3_regs[REG_CS])
#define ds (vm3_regs[REG_DS])
#define ss (vm3_regs[REG_SS])
#define es (vm3_regs[REG_ES])
#define si (vm3_regs[REG_SI])
#define di (vm3_regs[REG_DI])
#define bp (vm3_regs[REG_BP])
#define sp (vm3_regs[REG_SP])
#define flags (vm3_regs[REG_FLAGS])
#define ip (vm3_regs[REG_IP])
#pragma clang diagnostic pop

uint8_t vm3_mem[MEM_SIZE];
uint16_t vm3_regs[14];

int vm3_do_output = 0;
int vm3_do_input = 0;

int vm3_execute_one_instruction();

uint16_t *vm3_dest_op_word(uint16_t type, uint16_t val);

uint16_t vm3_src_op_word(uint16_t type, uint16_t val);

uint8_t *vm3_dest_op_byte(uint16_t type, uint16_t val);

uint8_t vm3_src_op_byte(uint16_t type, uint16_t val);

void init_VM3() {
    cs = 0;
    ip = 0;

    ds = 0x400;

    es = 0x700;

    ss = 0x900;
    sp = 0x7000;
}

void run_VM3() {
    puts("C");

    readAll_vm2(vm3_mem, 0x1000);

    cs = 0;
    ip = 0;

    while (vm3_execute_one_instruction()) {
        if (vm3_do_output) {
            if (es * 16 > MEM_SIZE)
                raise(SIGSEGV);
            if (es * 16 + ax > MEM_SIZE)
                raise(SIGSEGV);

            write_vm2(&vm3_mem[es * 16], ax);

            vm3_do_output = 0;
        } else if (vm3_do_input) {
            if (es * 16 + 0x1000 + 0x1000 > MEM_SIZE)
                raise(SIGSEGV);

            readAll_vm2(&vm3_mem[es * 16 + 0x1000], 0x1000);

            vm3_do_input = 0;
        }
    }

    puts("\xfd"); // NOLINT
}

int vm3_execute_one_instruction() {
    uint16_t opcode;
    uint16_t operand_1_type, operand_2_type;
    uint16_t operand_1_size, operand_2_size;
    uint16_t operand_1, operand_2;

    uint16_t *dest_word = nullptr;
    uint8_t *dest = nullptr;
    uint16_t src_word = 0;
    uint8_t src = 0;

    if (cs * 16 + ip > MEM_SIZE)
        raise(SIGSEGV);

    opcode = vm3_mem[cs * 16 + ip++] | vm3_mem[cs * 16 + ip++] << 8;

    operand_1_type = opcode & OP1_TYPE;
    operand_1_size = opcode & OP1_SIZE;
    operand_2_type = opcode & OP2_TYPE;
    operand_2_size = opcode & OP2_SIZE;

    if (operand_1_type) {
        operand_1 = vm3_mem[cs * 16 + ip++] | vm3_mem[cs * 16 + ip++] << 8;

        if (operand_1_size == OP1_WORD)
            dest_word = vm3_dest_op_word(operand_1_type, operand_1);
        else if (operand_1_size == OP1_BYTE)
            dest = vm3_dest_op_byte(operand_1_type, operand_1);
    }

    if (operand_2_type) {
        operand_2 = vm3_mem[cs * 16 + ip++] | vm3_mem[cs * 16 + ip++] << 8;

        if (operand_2_size == OP2_WORD)
            src_word = vm3_src_op_word(operand_2_type, operand_2);
        else if (operand_2_size == OP2_BYTE)
            src = vm3_src_op_byte(operand_2_type, operand_2);
    }

    if (cs * 16 + ip > MEM_SIZE)
        raise(SIGSEGV);

    switch (opcode & 0xff) {
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

        case OPCODE_MUL:
            if (dest_word)
                *dest_word *= src_word;
            else if (dest)
                *dest *= src;
            else
                raise(SIGILL);
            break;

        case OPCODE_DIV:
            if (dest_word)
                *dest_word /= src_word;
            else if (dest)
                *dest /= src;
            else
                raise(SIGILL);
            break;

        case OPCODE_PUSH_WORD:
            sp -= 2;

            if (ss * 16 + sp < 0)
                raise(SIGSEGV);

            *((uint16_t *) &vm3_mem[ss * 16 + sp]) = src_word;
            break;

        case OPCODE_POP_WORD:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            if (dest_word)
                *dest_word = *((uint16_t *) &vm3_mem[ss * 16 + sp]);
            else
                raise(SIGILL);

            sp += 2;
            break;

        case OPCODE_PUSH_BYTE:
            sp -= 1;

            if (ss * 16 + sp < 0)
                raise(SIGSEGV);

            vm3_mem[ss * 16 + sp] = (uint8_t) src_word;
            break;

        case OPCODE_POP_BYTE:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            if (dest)
                *dest = vm3_mem[ss * 16 + sp];
            else
                raise(SIGILL);
            sp += 1;
            break;

        case OPCODE_HLT:
            return 0;

        case OPCODE_OUT:
            vm3_do_output = 1;
            break;

        case OPCODE_IN:
            vm3_do_input = 1;
            break;

        case OPCODE_CMP_WORD:
            if (dest_word)
                flags = (uint16_t) (*dest_word == src_word);
            break;

        case OPCODE_CMP_BYTE:
            if (dest)
                flags = (uint16_t) (*dest == src);
            break;

        case OPCODE_CALL:
            sp -= 2;

            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            *((uint16_t *) &vm3_mem[ss * 16 + sp]) = ip;
            ip += src_word;
            break;

        case OPCODE_RET:
            if (ss * 16 + sp > MEM_SIZE)
                raise(SIGSEGV);

            ip = *((uint16_t *) &vm3_mem[ss * 16 + sp]);
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

        case OPCODE_RDRAND:
            if (es * 16 + (int16_t)di < 0 || es * 16 + (int16_t)di > MEM_SIZE)
                raise(SIGSEGV);
            vm3_mem[ds * 16 + (int16_t)di] = (uint8_t)(rand() & 0xff);
            break;

        default:
            raise(SIGILL);
    }

    if (cs * 16 + ip >= MEM_SIZE)
        raise(SIGSEGV);

    return 1;
}

uint16_t *vm3_dest_op_word(uint16_t type, uint16_t val) {
    uint16_t *tmp = nullptr;

    switch (type) {
        case OP1_REG:
        case OP2_REG:
            if (val > 11)
                raise(SIGILL);
            tmp = &vm3_regs[val];
            break;

        case OP1_MEM:
        case OP2_MEM:
            if (ds * 16 + val + sizeof(uint16_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = (uint16_t *) &vm3_mem[ds * 16 + val];
            break;

        case 0:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint16_t vm3_src_op_word(uint16_t type, uint16_t val) {
    uint16_t tmp = 0;

    switch (type) {
        case OP1_REG:
        case OP2_REG:
            if (val > 11)
                raise(SIGILL);
            tmp = vm3_regs[val];
            break;

        case OP1_IMM:
        case OP2_IMM:
            tmp = val;
            break;

        case OP1_MEM:
        case OP2_MEM:
            if (ds * 16 + val + sizeof(uint16_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = *((uint16_t *) &vm3_mem[ds * 16 + val]);
            break;

        case 0:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint8_t *vm3_dest_op_byte(uint16_t type, uint16_t val) {
    uint8_t *tmp = nullptr;

    switch (type) {
        case OP1_REG:
        case OP2_REG:
            if (val > GPREG_COUNT)
                raise(SIGILL);
            tmp = (uint8_t *) &vm3_regs[val];
            break;

        case OP1_MEM:
        case OP2_MEM:
            if (ds * 16 + val + sizeof(uint8_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = &vm3_mem[ds * 16 + val];
            break;

        case 0:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}

uint8_t vm3_src_op_byte(uint16_t type, uint16_t val) {
    uint8_t tmp = 0;

    switch (type) {
        case OP1_REG:
        case OP2_REG:
            if (val > GPREG_COUNT)
                raise(SIGILL);
            tmp = (uint8_t) vm3_regs[val];
            break;

        case OP1_IMM:
        case OP2_IMM:
            tmp = (uint8_t) val;
            break;

        case OP1_MEM:
        case OP2_MEM:
            if (ds * 16 + val + sizeof(uint8_t) > MEM_SIZE)
                raise(SIGSEGV);
            tmp = vm3_mem[ds * 16 + val];
            break;

        case 0:
            break;

        default:
            raise(SIGILL);
    }

    return tmp;
}
