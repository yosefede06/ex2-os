//
// Created by Yosef on 21/04/2023.
//

#include "Thread.h"



Thread::Thread (State state, size_t quantum, bool allocate_stack, thread_entry_point entry_point) {
    if(allocate_stack) {
        stack = new char[STACK_SIZE];
    }
    else {
        stack = nullptr;
    }
    address_t sp = (address_t) this->stack + STACK_SIZE - sizeof(address_t);
    sigsetjmp(this->env, 1);
    this->quantum_t = quantum;
    (this->env->__jmpbuf)[JB_SP] = translate_address(sp);
    (this->env->__jmpbuf)[JB_PC] = translate_address((address_t) entry_point);
    sigemptyset(this->env->__saved_mask);
}

Thread::~Thread() {
    delete[] this->stack;
}

Thread::Thread() {

}
