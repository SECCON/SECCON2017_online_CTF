#include <unistd.h>
#include <cstdio>
#include <csignal>
#include <cstdint>
#include <cstdlib>

#include "VM1.h"
#include "VM2.h"
#include "VM3.h"

void SIGALRM_handler(int);
void SIGILL_handler(int);
void SIGSEGV_handler(int);

int main()
{
    setvbuf(stdout, nullptr, _IONBF, 0);

    signal(SIGALRM, SIGALRM_handler);
    signal(SIGILL, SIGILL_handler);
    signal(SIGSEGV, SIGSEGV_handler);
    alarm(90);

    srand(0x31337);
    init_VM3();
    init_VM2();
    init_VM1();

    while (true) {
        puts("Z");

        uint8_t vm_select = 0;
        read(0, &vm_select, sizeof(uint8_t));

        switch (vm_select) {
            case 1:
                run_VM1();
                break;

            case 2:
                run_VM2();
                break;

            case 3:
                run_VM3();
                break;

            case 4:
                init_VM1();
                break;

            case 5:
                init_VM2();
                break;

            case 6:
                init_VM3();
                break;

            default:
                puts("z");
                return 0;
        }
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void SIGALRM_handler(int a)
{
    puts("\x14");
    exit(-1);
}

void SIGILL_handler(int a)
{
    puts("\x04");
    exit(-2);
}

void SIGSEGV_handler(int a)
{
    puts("\x11");
    exit(-3);
}
#pragma clang diagnostic pop
