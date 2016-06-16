/*
 * BOOOS.h
 *
 */

#include "Queue.h"

namespace BOOOS {


// Queue constructor: must initialize queue's attributes
Queue::Queue() {
	_length = 0;
	head()->next(0);
	head()->prev(0);
}

// Queue destructor: must finalize queue's attributes
Queue::~Queue() {
    _length = 0;
	head()->next(0);
	head()->prev(0);
}

// insert method: must insert the Element at the end of the queue
// Error messages:
//   01: invalid Element (if elem == 0)
//   02: duplicated Element (if elem already is on the queue)
void Queue::insert(Queue::Element * elem) {
	if (elem == 0)
		throw 1;
	if (length() == 0)
	{
		head()->next(elem);
		head()->prev(elem);
		elem->next(elem);
		elem->prev(elem);		
	}
	else
	{
		Element * aux = head();
		do
		{
			aux = aux->next();				
			if (aux == elem)
				throw 2;				
		} while (aux != head()->prev());
		aux = head()->prev();
		aux->next(elem);
		elem->prev(aux);
		elem->next(head()->next());
		head()->prev(elem);
	}
	_length++;
}

// remove: must search and remove the element from the queue
// Error messages:
//   01: queue is empty
Queue::Element * Queue::remove() {
	if (length() == 0)
		throw 1;
	Element * aux = head()->next();
	if (length() == 1)
	{								
		head()->prev(0);
		head()->next(0);			
	}
	else
	{
		aux = head()->next();
		aux->next()->prev(head()->prev());
		head()->prev()->next(head()->next()->next());
		head()->next(head()->next()->next());
	}		
	_length--;
	return aux;
}

}
