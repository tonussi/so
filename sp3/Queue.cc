/**
 * authors: ILE, LUCAS e LAURA
 * grupo: Agrupamento 8
 */
#include <iostream>
#include <string.h>

#include "Queue.h"

using namespace std;

namespace BOOOS {

    // start the queue at bottom with no size
    Queue::Queue() {
    	_length = 0;
    	head()->next(0);
    	head()->prev(0);
    }

    // destroy the queue
    Queue::~Queue() {
    	//while ( this->_length ) {
    	//    // remove each element
    	//	this->remove();
    	//}
        _length = 0;
    	head()->next(0);
    	head()->prev(0);
    }

    void Queue::insert( Queue::Element * elem ) {
    	if (elem == 0)
	    	throw 1;
        //  prepare to insert a first element at begin of the queue
        if ( Queue::length() == 0 ) {
            // prepare the queue's head same as Queue::head()->next(elem);
            Queue::head()->next(elem);
            // adjust the queue's tail same as Queue::head()->prev(elem);
            Queue::head()->prev(elem);
            // points to the head of the queue
            elem->next(elem);
            // points to the tail of the queue [tail] <- [elem] -> [head]
            elem->prev(elem);
            // growth of the queue by 1 element
            // move to the end
        } else {
            // get the head element
            Element * aux = Queue::head();
            // bool check = false;
            // if its not a first element then insert the folowers
            // declare a starting point for iteration
            if ( Queue::search(elem) != NULL ) {
            //for (aux = head()->next(); aux != head()->next(); aux = aux->next()) {
                //if ( aux == elem ) {
                    // check if the element is already in the queue
                throw 2; // check = true; // true if the element already exists
            }
            // otherwise the element is not in the queue
                // get the end of the queue
                aux = head()->prev();
            	// elem->prev(Queue::head()->prev());
            	// point to the next element
            	aux->next(elem);
            	// connect the new element to the next and previous element
            	// Queue::head()->prev(elem); elem->next(_head.next());
            	elem->prev(aux);
            	elem->next(Queue::head()->next());
        		Queue::head()->prev(elem);
            	//cout << "QUEUE HEAD: " << Queue::head()  << "\nANTERIOR: " << Queue::head()->prev() << "\nPROXIMO: " << Queue::head()->next()  << endl;
        }
        Queue::_length++;
        //cout << Queue::head() << "\n" << Queue::head()->prev() << "\n" << Queue::head()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev()->prev() << "\n" << endl;
        //cout << Queue::head()->next()->prev() << "\n" << Queue::head()->next()->next() << endl << endl;
	}

    Queue::Element * Queue::search(Element * elem) {
        for (Element * aux = head()->next(); aux != head()->next(); aux = aux->next()) {
            //cout << Queue::head() << "\n" << Queue::head()->prev() << "\n" << Queue::head()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev()->prev() << "\n" << endl;
            if ( aux == elem ) {
                //cout << Queue::head() << "\n" << Queue::head()->prev() << "\n" << Queue::head()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev()->prev() << "\n" << endl;
                return aux;
            }
        }
        return NULL;
    }

    Queue::Element * Queue::remove() {
        if (Queue::_length < 1)
            throw 1;
        // declare a element to pop out
        Element * aux = head()->next();
        // enable poping out only if the queue has one or more elements
        // if the queue has just 1 than pick the previous element from the head
        if ( Queue::_length == 1 ) {
    		Queue::head()->prev(0);
    		Queue::head()->next(0);
        } else {
            // if the queue has more than one element then prepare to unlink elements
            // get the element in the head to be pop out and removed from the queue
            aux = Queue::head()->next();
            // steps to disconect the element from the previous and next element
            // Queue::head()->next(aux->next());
            //cout << "QUEUE PREVIOUS: " << Queue::head()->prev()->next() << "\n" << "AUX NEXT: " << aux->next() << endl;
            aux->next()->prev(Queue::head()->prev());
            Queue::head()->prev()->next(Queue::head()->next()->next());
            Queue::head()->next(Queue::head()->next()->next());
        }
        _length--;
        return aux;
    }
}
