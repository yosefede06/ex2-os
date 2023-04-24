//
// Created by Yosef on 21/04/2023.
//

#ifndef EX2_OS_THREAD_H
#define EX2_OS_THREAD_H
typedef void (*thread_entry_point)(void);

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
