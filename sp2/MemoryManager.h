/*
 * File:   MemoryManager.h
 * Authors: Lucas e Ilê
 * Strategy: INSERT THE MEMORY MANAGEMENT SCHEMA HERE (THE ALGORITHM YOU ARE IMPLEMENTING -- RANDOMLY GENERATED BY MOODLE AS A VARIATION)
 *
 * Created on ...
 */
#ifndef MEMORYMANAGER_H
#define	MEMORYMANAGER_H

#ifndef nullptr
#define nullptr 0
#endif

class Partition {
public:

    Partition(unsigned int beginAddress, unsigned int endAddress, bool isFree) {
        _beginAddress = beginAddress;
        _endAddress = endAddress;
        _isFree = isFree;
    }
public: // do not change

    unsigned int getBeginAddress() const {
        return _beginAddress;
    }

    unsigned int getEndAddress() const {
        return _endAddress;
    }

    unsigned int getLength() const {
        return _endAddress - _beginAddress + 1;
    }
private: // do not change
    unsigned int _beginAddress;
    unsigned int _endAddress;
    bool _isFree;
private:
    // INSERT YOUR CODE HERE
    // ...
};

/** DISCLAIMER: This Element and List class will be used ONLY FOR PARTITIONS.
 *  We choose that, and choose not to use Templates.
 */

// Element: simple Element class to be used as a Element List
class Element {
public:
    Element(Partition* p) : _part(p) {
        // nullptr is used on original file, so we supose that exists as (void*) 0.
        _next = (Element*) nullptr;
    }
    
    // getNext: returns the refference to the next Element
    Element* getNext() {
        return _next;
    }
    
    // setNext: set the refference from the next Element to the given value
    void setNext(Element* n) {
        _next = n;
    }
    
    // getValue: returns the content(Partition) from the Element
    Partition* getValue() {
        return _part;
    }

private:
    // attributes
    // the partition itself
    Partition* _part;
    Element* _next;
    // methods

};

// SimpleList: simple List class to contain all the partitions
class SimpleList {
public:
    // constructor
    SimpleList() {
        _size = 0;
        _head = (Element*) nullptr;
    }

    // getSize: returns the size of the list
    unsigned int getSize() {
        return _size;
    }

    // append: insert a new element on the END of the list
    void append(Element* el) {
        // if list is empty
        if(!_size) {
            // the new partition is the _head partition
            _head = el;
        } else {
            // auxiliar values
            Element* aux = _head;
            Element* next = aux->getNext();
            // while next still not null
            while(next) {
                // aux turns next and
                aux = next;
                // next turns aux->_next
                next = aux->getNext();
            }
            // now we know we are in the last Element of the list,
            // we set the last->_next to the Element
            aux->setNext(el);
        }
        // +1 to _size
        _size++;
    }
    
    // insertSpecific: put the new Element on the specific position
    void insertSpecific(Element* el, unsigned int pos) {
        // if the position is greater or equal _size 
        if(pos >= _size) {
            // just append
            append(el);
        } else {
            // auxiliar values
            Element* aux = _head;
            Element* next = aux->getNext();
            // search for the right position
            for(int i = 0 ; i < pos - 1 ; i++) {
                // aux turns next and
                aux = next;
                // next turns aux->_next
                next = aux->getNext();
            }
            // now the aux is the pos - 1 value and the _next from the new
            // Element will be the next
            aux->setNext(el);
            el->setNext(next);
            // +1 to _size
            _size++;
        }
    }
    
    // remove: remove the first Partition(Element) from the list
    Partition* remove() {
        // if empty list
        if(!_size) {
            return (Partition*) nullptr;
        }
        // store the return value
        Partition* rv = _head->getValue();
        // store the old _head
        Element* old = _head;
        // cleanup and att _head
        _head = _head->getNext();
        old->setNext((Element*) nullptr);
        // -1 to _size
        _size--;
        // return the partition
        return rv;
    }
    
    // removeSpecific: remove the specific Partition(Element) from the list
    Partition* removeSpecific(unsigned int pos) {
        // if empty list
        if(!_size) {
            return (Partition*) nullptr;
        }
        // return value at the end
        Partition* rv;
        // auxiliar values
        Element* aux = _head;
        Element* next = aux->getNext();
        
        // if position is greater or equal than size
        if(pos >= _size) {
            // search for the right position
            for(int i = 0 ; i < _size - 1 ; i++) {
                // aux turns next and
                aux = next;
                // next turns aux->_next
                next = aux->getNext();
            }
        } else {
            // search for the right position
            for(int i = 0 ; i < pos - 1 ; i++) {
                // aux turns next and
                aux = next;
                // next turns aux->_next
                next = aux->getNext();
            }
        }
        // now the aux is the pos - 1 value and the _next is the Element
        // to be removed
        // set the next from aux to be the next from the removed Element
        aux->setNext(next->getNext());
        // and cleanup the next->next
        next->setNext((Element*) nullptr);
        // get the Partition
        rv = next->getValue();
        // -1 to _size
        _size--;

        return rv;
    }
    
    // getPartition: get a specific Partition but does not remove it
    Partition* getPartition(unsigned int pos) {
        // if empty list
        if(!_size) {
            return (Partition*) nullptr;
        }
        // return value at the end
        Partition* rv;
        // auxiliar values
        Element* aux = _head;

        // if position is greater or equal than size
        if(pos >= _size) {
            // search for the right position
            for(int i = 0 ; i < _size ; i++) {
                // aux turns aux->_next
                aux = aux->getNext();
            }
        } else {
            // search for the right position
            for(int i = 0 ; i < pos ; i++) {
                // aux turns aux->_next
                aux = aux->getNext();
            }
        }
        // now the aux is the pos value, we just need get the Partition
        rv = aux->getValue();

        return rv;
    }

    Element* getHead() {
        return this->_head;
    }

private:
    // attributes
    // _size: stores the size of the list(cannot be negative)
    unsigned int _size;
    // _head: is the head element from the list
    Element* _head;
    
    // methods

};

enum MemoryAllocationAlgorithm {FirstFit, NextFit, BestFit, WorstFit};

class MemoryManager {
public: // do not change
    MemoryManager(MemoryAllocationAlgorithm algorithm);
    MemoryManager(const MemoryManager& orig);
    virtual ~MemoryManager();
public: // do not change
    Partition* allocateMemory(unsigned int size);
    void deallocateMemory(Partition* partition);
    void showMemory();
    unsigned int getNumPartitions();
    Partition* getPartition(unsigned int index);
private: // private attributes and methods
    // INSERT YOUR CODE HERE
    // ...
    // attributes
    // _algo stores the value of algorithm used
    MemoryAllocationAlgorithm _algo;
    // _listPartitions is the list from partitions
    SimpleList* _listPartitions;
    // _indexLastPartition stores the index of the last partition we created.
    // This is used in NextFit algorithm.
    unsigned int _indexLastPartition;

    // methods
    // 'fit' methods
    Partition* firstFit (unsigned int size);
    Partition* nextFit  (unsigned int size);
    Partition* bestFit  (unsigned int size);
    Partition* worstFit (unsigned int size);
    
    // print methods
    void printPartition(Partition* p);
    void printFreeSpace(unsigned int start, unsigned int end);

};

#endif	/* MEMORYMANAGER_H */
