/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */

#ifndef TASK_H_
#define TASK_H_

#include <ucontext.h>

#include "Queue.h"

namespace BOOOS {

    /*!
     * This class represents the process control block
     */
    class Task : public Queue::Element {
    public:

    	enum State {
    	    // the process is ready in the queue
    		READY,
    		// the process is waiting something
    		WAITING,
    		// the process is being processed
    		RUNNING,
    		// the process is dead
    		FINISHING,
    		// This is the state when the context switching
    		SCHEDULER
    	};

    	/*!
    	 * Task constructor
    	 *
    	 * \param (*entry_pont)(void): some function
    	 * \param nargs: quantity of arguments that are being passed
    	 * \param arg: array of arguments that are being passed
    	 */
    	Task(void (*entry_point)(void *), int nargs, void * arg);
    	Task();

        // task pcb destructor
    	virtual ~Task();

        // \return: the task id
    	int tid() { return _tid; }

        // return the process state, including SCHEDULER state
    	State state() { return _state; }

        /**
         * This function must save the context of the object and then loads the
         * Task's context. The new State is the new transition state of the process.
    	 *
    	 * \param t: The task
    	 * \param s: The state
    	 */
    	void pass_to(Task * t, State s = READY);

        /**
         * This should finalize the process taking it to final state
    	 * \param: code to finalize the task
    	 */
        // Task.h:53:11: error: ‘void BOOOS::Task::exit(int)’ cannot be overloaded
        // Task.h:49:14: error: with ‘void BOOOS::Task::exit(int)’
        void exit(int code);

    	void yield();

    	// returns the this task
    	static Task * self() { return (Task*)__running; }
    	static void init();
    	static int count();

    protected:

        // the actual state of the task
    	State _state;
    	static Queue  __ready;
    	static Task * __main;
    	static int ntask;

    private:
        // current task being executed
    	static volatile Task * __running;
    	char * stack;
    	ucontext_t _context;
    	int _tid;
    	static int global_tid;

    };

} /* namespace BOOOS */

#endif /* TASK_H_ */
