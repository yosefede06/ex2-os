//
// Created by Yosef on 23/04/2023.
//

#ifndef EX2_OS_SCHEDULER_H
#define EX2_OS_SCHEDULER_H
#include <map>
#include <Thread.h>
#define MAX_THREAD_NUM 100 /* maximal number of threads */
#include <map>
#include <set>
class Scheduler {
private:
    std::map<size_t, Thread> threads;
    sigset_t set;

public:
    void set_thread(size_t i, Thread & thread);
    Thread& get_thread(size_t i);
};


#endif //EX2_OS_SCHEDULER_H
