//
// Created by Yosef on 23/04/2023.
//


#include "Scheduler.h"


void Scheduler::set_thread(size_t i, Thread & thread) {
        if (this->threads.size() > MAX_THREAD_NUM) {
            std::cerr << "Maximum number of threads delimited";
        }
        else {
            this->threads[i] = thread;
        }
}

Thread& Scheduler::get_thread(size_t i) {
    return this->threads[i];
}