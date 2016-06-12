/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */

#include "Scheduler.h"

#include <ucontext.h>
#include <iostream>

namespace BOOOS {

    Scheduler * Scheduler::scheduler = 0;
    volatile int Scheduler::userTasks;

    Scheduler::Scheduler() {
        Scheduler::scheduler = (Scheduler *) new Task(scheduler, 0, 0);
        Scheduler::scheduler->_state = SCHEDULER;
    }

    Scheduler::~Scheduler() {
        delete scheduler;
    }

    void Scheduler::init() {
        scheduler = new Scheduler();
    }

    void Scheduler::scheduler(void*) {
        while ( (userTasks == Task::ntask) > 0 ) {
            Task * next  = scheduler->choose_next();
            if (next) {
               self()->pass_to(next, READY); __ready.insert(next);
            }
        }
        scheduler->exit(0);
    }

    Task * Scheduler::choose_next() {
        return (Task*)__ready.remove();
    }

} /* namespace BOOOS */
