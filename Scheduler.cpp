
//
// Created by Yosef on 23/04/2023.
//


#include "Scheduler.h"

void Scheduler::change_thread(int signal) {
    // Block all signals contained in set.
    this->block_signals();
    this->total_quantums++;
    if (save_current_execution_context() == 1) {
        return;
    }
    // ready <-> running
    ready_thread(this->running_thread_tid);
    this->run_next_thread();
    if (setitimer(ITIMER_VIRTUAL, &this->timer, nullptr) == FAILURE_ERROR)
    {
        handleErrorSystemCall("TIMER ERROR");
    }
    // Unblock all signals contained in set.
    this->unblock_signals();
}

void Scheduler::ready_thread(size_t tid) {
    if(this->sleeping_threads.find(tid) == this->sleeping_threads.end()) {
        this->get_thread(tid).state = READY;
        ready_threads.push_back(tid);
    }
}

/**
 *
 * @return 1 in case is called by sigsetjmp else 0;
*/
int Scheduler::save_current_execution_context() {
    if(sigsetjmp(this->get_thread(this->running_thread_tid).env, 1) == 1) {
        this->unblock_signals();
        return 1;
    }
    return 0;
}
void Scheduler::_handle_sleep_threads() {
    size_t tid;
    size_t num_quantums;
    std::deque<size_t> awake_threads;
    for (auto sleep_thread: this->sleeping_threads) {
        tid = sleep_thread.first;
        num_quantums = sleep_thread.second;
        Thread & thread = get_thread(tid);
        if (num_quantums <= total_quantums) {
            awake_threads.push_back(tid);
            if (thread.state == READY) {
                this->ready_thread(tid);
            }
        }
    }
    for (auto & awake_thread : awake_threads) {
        this->sleeping_threads.erase(awake_thread);
    }
}

void Scheduler::run_next_thread () {
    _handle_sleep_threads();
    this->running_thread_tid = this->ready_threads.front();
    this->ready_threads.pop_front();
    Thread new_thread = this->get_thread(this->running_thread_tid);
    new_thread.state = RUNNNING;
    new_thread.quantum_t++;
    siglongjmp(new_thread.env, 1);
}



Scheduler::Scheduler(int quantum_usecs, void (* callback_handler)(int)){
    this->_callback_handler = callback_handler;
    this->_quantum_usecs = quantum_usecs;
    this->running_thread_tid = 0;
}


int Scheduler::init_scheduler(){
    struct sigaction sa = {nullptr};
    // Install timer_handler as the signal handler for SIGVTALRM.
    sa.sa_handler = this->_callback_handler;
    if (sigaction(SIGVTALRM, &sa, nullptr) == FAILURE_ERROR)
    {
        fprintf(stderr,"Sigaction error.\n");
        return -1;
    }
    auto *thread = new Thread(RUNNNING, 1, false);
    if(this->set_thread(0, *thread) == FAILURE_ERROR) {
        return FAILURE_ERROR;
    }
    sigemptyset (&this->set);
    sigaddset (&this->set, SIGVTALRM);

    // Configure the timer to expire after 1 sec... */
    this->timer.it_value.tv_sec = _quantum_usecs / SECOND;        // first time interval, seconds part
    this->timer.it_value.tv_usec = _quantum_usecs % SECOND;        // first time interval, microseconds part

    // configure the timer to expire every 3 sec after that.
    this->timer.it_interval.tv_sec = _quantum_usecs / SECOND;    // following time intervals, seconds part
    this->timer.it_interval.tv_usec = _quantum_usecs % SECOND;    // following time intervals, microseconds part

    return 0;
}

int Scheduler::set_thread(size_t i, Thread & thread) {

    if (this->threads.size() > MAX_THREAD_NUM) {
        return handleErrorLibrary("Maximum number of threads delimited");
    }
    this->threads[i] = &thread;
    return 0;
}

Thread& Scheduler::get_thread(size_t i) {
    return *this->threads[i];
}


int Scheduler::add_new_thread(State state, size_t quantum, bool allocate_stack, thread_entry_point entry_point) {
    for (int tid = 0; tid < MAX_THREAD_NUM; tid++) {
        if (!check_thread(tid)) {
            auto * thread = new Thread (state, quantum, allocate_stack, entry_point);
            this->set_thread(tid, *thread);
            if (state == READY) {
                this->ready_threads.push_back(tid);
            }
            return tid;
        }
    }
    return -1;
}

bool Scheduler::check_thread(size_t i) {
    auto it = this->threads.find(i);
    return it != this->threads.end();
}



int Scheduler::block_signals(){
    if(sigprocmask(SIG_BLOCK, &this->set, nullptr) == FAILURE_ERROR) {
        handleErrorSystemCall("Signal Block failed");
    }
    return 0;
}

int Scheduler::unblock_signals(){
    if(sigprocmask(SIG_UNBLOCK, &this->set, nullptr) == FAILURE_ERROR) {
        handleErrorSystemCall("Signal Unblock failed");
    }
    return 0;
}

int Scheduler::get_total_quantums() const {
    return total_quantums;
}


int Scheduler::remove_thread(size_t tid) {
    if (!check_thread(tid)) {
        return handleErrorLibrary("No thread with Id to remove");
    }
    Thread & thread = get_thread(tid);
    this->threads.erase(tid);
    switch (thread.state) {
        case READY:
            // Removes thread from ready list if state is READY
            this->remove_thread_from_ready(tid);
            break;
        case RUNNNING:
            this->run_next_thread();
            break;
        case BLOCKED:
            this->blocked_threads.erase(tid);
            break;
    }
    if(this->sleeping_threads.find(tid) != this->sleeping_threads.end()) {
        sleeping_threads.erase(tid);
    }
    return 0;
}

void Scheduler::remove_thread_from_ready(size_t tid) {
    for (int i = 0; i < this->ready_threads.size(); i++) {
        if (this->ready_threads[i] == tid) {
            this->ready_threads.erase(this->ready_threads.begin() + i);
        }
    }
}

int Scheduler::block_thread(size_t tid) {
    Thread & thread = this->get_thread(tid);
    switch (thread.state) {
        case READY:
            // Removes thread from ready list if state is READY
            this->remove_thread_from_ready(tid);
            break;
        case RUNNNING:
            // saves state
            if (this->save_current_execution_context() == 1) {
                return 0;
            }
            this->run_next_thread();
            break;
        case BLOCKED:
            return -1;
    }
    this->blocked_threads.insert(tid);
    thread.state = BLOCKED;
    return 0;
}


void Scheduler::unblock_thread(size_t tid) {
    Thread & thread = this->get_thread(tid);
    switch (thread.state) {
        case BLOCKED:
            this->ready_thread(tid);
            this->blocked_threads.erase(tid);
            break;
        default:
            break;
    }
}

int Scheduler::get_running_thread_tid() const {
    return (int) this->running_thread_tid;
}

void Scheduler::sleep_running_thread(size_t num_quantums) {
    size_t tid = get_running_thread_tid();
    this->sleeping_threads[tid] = this->total_quantums + num_quantums + 1;
    if (this->save_current_execution_context() == 1) {
        return;
    }
    run_next_thread();
}

void Scheduler::remove_all() {
    std::deque<size_t> tids;
    for(auto & thread: this->threads) {
        size_t tid = thread.first;
        tids.push_back(tid);
    }
    for(auto & tid: tids) {
        this->remove_thread(tid);
    }
}
