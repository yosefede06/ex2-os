//
// Created by Yosef on 23/04/2023.
//


#include "Scheduler.h"

void Scheduler::change_thread(int signal) {
//    total_quantums++;
}

//
//
//Scheduler::Scheduler(int quantum_usecs){
//
//
//}


int Scheduler::init_scheduler(int quantum_usecs ){
    struct sigaction sa = {0};
    struct itimerval timer;
    // Install timer_handler as the signal handler for SIGVTALRM.
    sa.sa_handler = change_thread;
    if (sigaction(SIGVTALRM, &sa, NULL) < 0)
    {
        fprintf(stderr,"Sigaction error.\n");
        return -1;
    }

    auto *thread = new Thread(RUNNNING, 1, false);
    this->set_thread(0, *thread);
    sigemptyset (&this->set);
    sigaddset (&this->set, SIGVTALRM);

    // Configure the timer to expire after 1 sec... */
    timer.it_value.tv_sec = quantum_usecs / SECOND;        // first time interval, seconds part
    timer.it_value.tv_usec = quantum_usecs % SECOND;        // first time interval, microseconds part

    // configure the timer to expire every 3 sec after that.
    timer.it_interval.tv_sec = quantum_usecs / SECOND;    // following time intervals, seconds part
    timer.it_interval.tv_usec = quantum_usecs % SECOND;    // following time intervals, microseconds part

    // Start a virtual timer. It counts down whenever this process is executing.
    if (setitimer(ITIMER_VIRTUAL, &timer, NULL))
    {
        printf("setitimer error.");
        exit(EXIT_FAILURE);
    }
    return 0;
}

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

