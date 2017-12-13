#include <unistd.h>
#include <cstdint>
#include <cstring>

#include "IO.h"

#define MEM_SIZE 0xffff
extern uint8_t vm1_mem[MEM_SIZE];
extern uint8_t vm2_mem[MEM_SIZE];
extern uint8_t vm3_mem[MEM_SIZE];

size_t readAll(int fd, void *buf, size_t max_len) {
    uint32_t len = 0;
    size_t i;

    read(fd, &len, sizeof(len));
    if(len > (uint32_t)max_len)
        len = (uint32_t)max_len;
    for(i = 0; i < len; i++) {
        if (read(fd, (uint8_t *)buf + i, 1) != 1)
            return 0;
    }
    return i;
}

void readAll_vm1(void *buf, size_t len) {
    memcpy(buf, &vm1_mem[0x7000], len);
}

void readAll_vm2(void *buf, size_t len) {
    memcpy(buf, &vm2_mem[0x7000], len);
}

void readAll_vm3(void *buf, size_t len) {
    memcpy(buf, &vm3_mem[0x7000], len);
}

void write_vm1(void *buf, size_t len) {
    size_t copy_len = 0x1000;
    if (len < copy_len)
        copy_len = len;
    memcpy(&vm1_mem[0x8000], buf, copy_len);
}

void write_vm2(void *buf, size_t len) {
    size_t copy_len = 0x1000;
    if (len < copy_len)
        copy_len = len;
    memcpy(&vm2_mem[0x8000], buf, copy_len);
}

void write_vm3(void *buf, size_t len) {
    size_t copy_len = 0x1000;
    if (len < copy_len)
        copy_len = len;
    memcpy(&vm3_mem[0x8000], buf, copy_len);
}
