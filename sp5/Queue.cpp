/*
 * File:   Queue.cpp
 * Author: <preencher>
 *
 * Created on September 27, 2015, 11:28 AM
 */

#include "Queue.h"

template <class T> Queue<T>::Queue() : std::list<T>() {
}

template <class T> Queue<T>::Queue(const Queue& orig) : std::list<T>(orig) {
}

template <class T> Queue<T>::~Queue() {
}

// INSERT YOUR CODE HERE
// ...
