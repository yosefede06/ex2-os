//
// Created by Yosef on 21/04/2023.
//

#ifndef EX2_OS_THREAD_H
#define EX2_OS_THREAD_H
typedef void (*thread_entry_point)(void);
#ifdef __x86_64__
/* code for 64 bit Intel arch */

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

#else
/* code for 32 bit Intel arch */

typedef unsigned int address_t;
#define JB_SP 4
#define JB_PC 5


/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
                 "rol    $0x9,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}


#endif
#include "iostream"
#include <setjmp.h>
#include <signal.h>
using namespace std;
#define MAX_THREAD_NUM 100 /* maximal number of threads */
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */
#define SECOND 1000000
#define STACK_SIZE 4096


enum State {
    RUNNNING, BLOCKED, READY
};

class Thread {
    public :
        State state;
        char * stack;
        sigjmp_buf env;
        size_t quantum_t;
        Thread(State state, size_t quantum, bool allocate_stack, thread_entry_point entry_point = nullptr);
        Thread();
        ~Thread();
};




#endif //EX2_OS_THREAD_H
