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
        this->_length = 0;
    }

    // destroy the queue
    Queue::~Queue() {
    	while ( this->_length ) {
    	    // remove each element
    		this->remove();
    	}
    }

    void Queue::insert( Queue::Element * elem ) {
        Element * aux;
        bool check = false;
        if ( elem != 0 ) {
            //  prepare to insert a first element at begin of the queue
            if ( Queue::_length == 0 ) {
                // prepare the queue's head
                Queue::head()->next(elem);
                // adjust the queue's tail
                Queue::head()->prev(elem);
                // points to the head of the queue
                elem->next(elem);
                // points to the tail of the queue [tail] <- [elem] -> [head]
                elem->prev(elem);
                // growth of the queue by 1 element
                Queue::_length++;
			} else {
			    // if its not a first element then insert the folowers
			    // declare a starting point for iteration
			    for (aux = head()->next(); aux != head()->next(); aux = aux->next()) {
			        if ( aux == elem ) {
			            // check if the element is already in the queue
			            check = true; // true if the element already exists
			            break;
			        }
			    }
			    // otherwise the element is not in the queue
			    if ( check == false ) {
			        // get the head previous element
                    aux = Queue::head()->prev();
                	elem->prev(Queue::head()->prev());
                	// point to the next element
                	aux->next(elem);
                	// connect the new element to the next and previous element
                	Queue::head()->prev(elem);
                	elem->next(Queue::head()->next());
                	Queue::head()->next()->prev(Queue::head()->prev());
                	cout << Queue::head() << "\n" << Queue::head()->prev() << "\n" << Queue::head()->next() << "\n"  << endl;
                	cout << elem << "\n" << elem->prev() << "\n" << elem->next() << "\n"  << endl;
                    Queue::_length++;
                }
            }
        }
        cout << Queue::head() << "\n" << Queue::head()->prev() << "\n" << Queue::head()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev() << "\n" << Queue::head()->prev()->prev()->prev()->prev() << "\n" << endl;
        cout << Queue::head()->next()->prev() << "\n" << Queue::head()->next()->next() << endl << endl;
	}

    Queue::Element * Queue::remove() {
        // declare a element to pop out
        Element * aux;
        // enable poping out only if the queue has one or more elements
        if ( Queue::_length > 0 ) {
            // if the queue has just 1 than pick the previous element from the head
            if ( Queue::_length == 1 ) {
                aux = Queue::head()->prev();
            } else {
                // if the queue has more than one element then prepare to unlink elements
                if ( Queue::_length > 1 ) {
                    // get the element in the head to be pop out and removed from the queue
                    aux = Queue::head()->next();
                    // steps to disconect the element from the previous and next element
                    Queue::head()->next(aux->next());
                    cout << Queue::head()->prev()->next() << "\n" << aux->next() << endl;
                    aux->next()->prev(Queue::head()->prev());
                    Queue::head()->prev()->next(aux->next());
                    Queue::head()->next()->prev(head()->prev());
                }
            }
            Queue::_length--;
            return aux;
        }
        return 0;
    }
}
