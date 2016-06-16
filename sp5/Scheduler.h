/*
 * File:   Scheduler.h
 * Author: <PREENCHER>
 *
 * Created on September 27, 2015, 11:19 AM
 */

#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include "Thread.h"
#include "Scheduling_Queue.h"

#include <queue>

class Scheduler {
public:
    Scheduler();

    Scheduler(const Scheduler& orig) {

    }

    virtual ~Scheduler() {

    }
private:
    Scheduling_Queue* _readyQueue;
    Thread* _choosen;
public:
    static const bool preemptive = true;
public:
    Thread* choose();  // choose a new thread to run
    Thread* choosen(); // returns the last chosen thread (syntax error: the method should be called chosen)
    void reschedule();
    void insert(Thread* thread);
    Thread* remove(Thread* thread);
};

#endif	/* SCHEDULER_H */

