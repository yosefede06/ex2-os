//
// Created by Yosef on 23/04/2023.
//

#ifndef EX2_OS_SCHEDULER_H
#define EX2_OS_SCHEDULER_H
#include <map>
#include "Thread.h"
#include "uthreads.h"
#include <map>
#include <sys/time.h>
#include <set>
#include "Handle.h"
#include <signal.h>
#include <queue>

class Scheduler {

private:
    struct itimerval timer{};
    sigset_t set{};
    int total_quantums = 0;
    int _quantum_usecs;
    void (*_callback_handler)(int);
    size_t running_thread_tid;
    std::map<size_t, Thread*> threads;
    std::deque<size_t> ready_threads;
    std::set<size_t> blocked_threads;
    std::map<size_t, size_t> sleeping_threads;

public:
    Scheduler(int quantum_usecs, void (* callback_handler)(int));
    int set_thread(size_t i, Thread & thread);
    Thread& get_thread(size_t i);
    void change_thread(int signal);
    int init_scheduler();
    int block_signals();
    int unblock_signals();
    int get_total_quantums() const;
    bool check_thread(size_t);
    int add_new_thread(State state, size_t quantum, bool allocate_stack, thread_entry_point entry_point);
    int remove_thread(size_t);
    void run_next_thread();
    void remove_thread_from_ready(size_t tid);
    void block_thread(size_t tid);
    void unblock_thread(size_t tid);
    int save_current_execution_context();
    void ready_thread(size_t);
    int get_running_thread_tid() const;
    void sleep_running_thread(size_t);
    void _handle_sleep_threads();
    void remove_all();
    void reset_time();
};


#endif //EX2_OS_SCHEDULER_H
