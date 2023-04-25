//
// Created by Yosef on 21/04/2023.
//

#include "Thread.h"

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

Thread::Thread (State state, size_t quantum, bool allocate_stack, thread_entry_point entry_point) {
    if(allocate_stack) {
        this->stack = new char[STACK_SIZE];
    }
    else {
        this->stack = nullptr;
    }
    address_t sp = (address_t) this->stack + STACK_SIZE - sizeof(address_t);
    sigsetjmp(this->env, 1);
    this->quantum_t = quantum;
    this->state = state;
    (this->env->__jmpbuf)[JB_SP] = translate_address(sp);
    (this->env->__jmpbuf)[JB_PC] = translate_address((address_t) entry_point);
    sigemptyset(&this->env->__saved_mask);
}

Thread::~Thread() {
    delete[] this->stack;
}

Thread::Thread() {

}
