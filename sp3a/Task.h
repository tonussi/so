/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */

#ifndef TASK_H_
#define TASK_H_

#include "Queue.h"
#include <ucontext.h>

namespace BOOOS {

    class Task : public Queue::Element {
    public:
    	enum State {
    		READY,
    		WAITING,
    		RUNNING,
    		FINISHING
    	};
    
    	/*!
    	 * Construtor
    	 *
    	 * \param (*entry_pont)(void) a function
    	 * \param nargs number of arguments
    	 * \param arg array of arguments
    	 */
    	Task(void (*entry_point)(void), int nargs, void * arg);
    
    	Task(void (*entry_point)(void*), int nargs, void * arg);
    
    	virtual ~Task();
    
    	/*!
    	 * \return: task id
    	 */
    	int tid() { return _tid; }
    
    	/*!
    	 * \return: task current state
    	 */
    	State state() { return _state; }
    
    	/*!
    	 * \brief transfer the state of a task to another state
    	 *
    	 * \param t a task
    	 * \param s next state
    	 */
    	void pass_to(Task * t, State s = READY);
    
    	/*!
    	 * \brief finalize the process
    	 * \param: code to the function
    	 */
    	void exit(int code);
    
    	/*!
    	 * \return: the current task running
    	 */
    	static Task * self() { return (Task*) __running; }
    
    	static void init();
    
    private:
    	Task();
    	static volatile Task * __running;
    	static Task * __main;
    	State _state;
    	int _tid;
    	static int _tid_counter;
    	char* _stack;
    	static int STACK_SIZE;
    	ucontext_t context;
    };

} /* namespace BOOOS */

#endif /* TASK_H_ */
