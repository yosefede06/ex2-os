//
// Created by Yosef on 23/04/2023.
//

#ifndef EX2_OS_SCHEDULER_H
#define EX2_OS_SCHEDULER_H
#include <map>
#include <Thread.h>
#define MAX_THREAD_NUM 100 /* maximal number of threads */
#include <map>
#include <sys/time.h>
#include <set>
class Scheduler {
private:
    std::map<size_t, Thread> threads;
    sigset_t set;
    int total_quantums = 0;

public:
//    Scheduler();
    void set_thread(size_t i, Thread & thread);
    Thread& get_thread(size_t i);
    void change_thread(int signal);
    int init_scheduler(int quantum_usecs);
};


#endif //EX2_OS_SCHEDULER_H
