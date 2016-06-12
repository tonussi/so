/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */

#include "Task.h"
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>
#include "Scheduler.h"
#include <iostream>

const int STACKSIZE = 2e15;

namespace BOOOS {

    volatile Task * Task::__running;
    int Task::global_tid;
    Queue  Task::__ready;
    Task* Task::__main;
    int Task::ntask;

    void Task::init() {
        Task::ntask = 0;
        __main = new Task();
    }

    Task::Task(){
        global_tid = 0;
        this->_tid = global_tid;
        getcontext(&this->_context);
        this->stack = (char*) malloc(STACKSIZE);

        if ( this->stack ) {
            this->_context.uc_stack.ss_sp = this->stack;
            this->_context.uc_stack.ss_size = STACKSIZE;
            this->_context.uc_stack.ss_flags = 0;
            this->_context.uc_link = 0;
            __running = this;
        } else {
            throw "error";
        }
        Scheduler::userTasks++;
        Task::ntask++;
        __ready.insert(this);
    }

    Task::Task(void (*entry_point)(void *), int nargs, void * arg) {
        this->_state = READY;
        this->_tid = ++global_tid;
        getcontext(&this->_context);
        void * aux;
        aux = malloc(STACKSIZE);
        this->stack =(char*) aux;
        if ( this->stack ) {
            this->_context.uc_stack.ss_sp = this->stack;
            this->_context.uc_stack.ss_size = STACKSIZE;
            this->_context.uc_stack.ss_flags = 0;
            this->_context.uc_link = 0;
        } else {
            throw "error";
        }
        Scheduler::userTasks++;
        Task::ntask++;
        makecontext(&this->_context, entry_point, nargs, arg);
        __ready.insert(this);
    }

    Task::~Task() {
        Scheduler::userTasks--;
        free(this->stack);
    }

    void Task::pass_to(Task * t, State s) {
        this->_state = s;
        __running = t;
        swapcontext(&this->_context, &t->_context);
    }

    void Task::yield () {
        this->pass_to(Scheduler::self(), READY);
    }

    void Task::exit(int code) {
        this->_state = FINISHING;
        Task::ntask--;
    }

    int Task::count() {
      return Task::ntask;
    }

} /* namespace BOOOS */
