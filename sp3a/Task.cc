/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */

#include "Task.h"

namespace BOOOS {

volatile Task * Task::__running;
Task * Task::__main;
int Task::_tid_counter = 0;
int Task::STACK_SIZE = 32768;

Task::Task(void (*entry_point)(void), int nargs, void * arg) {

	if (nargs < 0) {
		throw "Error, number of arguments is invalid";
	}

	/**
	 * \brief getcontex(): get the current context
	 * \param &this->context: the current context of the process
	 */
	getcontext(&(this->context));
	this->_stack = new char[Task::STACK_SIZE];
	// get user context link
	this->context.uc_link = (ucontext_t*)&(__running->context);
	// save the stack pointer
	this->context.uc_stack.ss_sp = this->_stack;
	// save the task's stack
	this->context.uc_stack.ss_size = Task::STACK_SIZE;
	// set task to ready
	this->_state = Task::READY;
	// update the task id counter
	this->_tid = ++Task::_tid_counter;
	// remake the context
	// void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
	makecontext(&(this->context), (void (*) (void))entry_point, nargs, arg);
}

Task::Task(void (*entry_point)(void*), int nargs, void * arg) {

	if (nargs < 0) {
		throw "Error, number of arguments is invalid";
	}

	getcontext(&(this->context));
	this->_stack = new char[Task::STACK_SIZE];
	this->context.uc_link          = (ucontext_t*)&(__running->context);
	this->context.uc_stack.ss_sp   = this->_stack;
	this->context.uc_stack.ss_size = Task::STACK_SIZE;
	this->_state = Task::READY;
	this->_tid = ++Task::_tid_counter;
	makecontext(&(this->context), (void (*) (void))entry_point, nargs, arg);
}

Task::~Task() {
	delete _stack;
}

void Task::pass_to(Task * t, State s) {
	this->_state = s;
	Task::__running = t;
	t->_state = Task::RUNNING;
	// int swapcontext(ucontext_t *oucp, const ucontext_t *USER_CONTEXT_PROCESS);
	swapcontext(&(this->context), &(t->context));
}

void Task::exit(int code) {
	this->pass_to(__main, Task::FINISHING);
}

void Task::init() {
	Task::_tid_counter = 0;
	__main = new Task();
	__running = __main;
	__main->_state = Task::RUNNING;
}

Task::Task() {
	this->_tid = 0;
	this->_stack = new char[Task::STACK_SIZE];
	this->context.uc_stack.ss_sp   = this->_stack;
	this->context.uc_stack.ss_size = Task::STACK_SIZE;
	this->_state = Task::READY;
}

} /* namespace BOOOS */
