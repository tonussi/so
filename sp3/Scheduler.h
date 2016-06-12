/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <ucontext.h>
#include <iostream>

#include <Task.h>
#include <Queue.h>

namespace BOOOS {

    class Scheduler : public Task {
        // to share the same components and functionalities
    	friend class Task;
    protected:
    	Scheduler();
    public:
        // strategy to choose between different scheduling types
    	enum SchedulerType {
    		SCHED_FCFS,
    		SCHED_PRIORITY
    	};
    	virtual ~Scheduler();
    	static void init();
    	static void dispatcher(void*);
    	static Scheduler * self() { return scheduler; }
    protected:
    	virtual Task * choose_next();
    	static Scheduler * scheduler;
    	static volatile int userTasks;
    };

} /* namespace BOOOS */

#endif /* SCHEDULER_H_ */
