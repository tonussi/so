/*
 * File:   Queue.h
 * Author: <preencher>
 *
 * Created on September 27, 2015, 11:28 AM
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include <list>

/**
 * Used by Thread as waiting queues for threads that uses void Thread::sleep(Queue* e) and void Thread::wakeup(Queue q)
 **/

// INSERT YOUR CODE HERE ; CHANGE THE QUEUE THE WAY YOU WANT

template <class T>
class Queue : public std::list<T> {
public: // constructors
    Queue();
    Queue(const Queue& orig);
    virtual ~Queue();
private: // user defined
    // INSERT YOUR CODE HERE
    // ...
};

#endif	/* QUEUE_H */

