#ifndef ALDNWONG_MYQUEUE_H
#define ALDNWONG_MYQUEUE_H
#include <iostream>
#include <cassert>
#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"

using namespace std;

template <typename ITEM_TYPE>
class Queue
{
public:
class Iterator{
    public:
        friend class Queue;                     
        Iterator():_ptr(nullptr) {
            // Empty
        }
        Iterator(node<ITEM_TYPE>* p):_ptr(p) {
            // Empty   
        }  

        bool is_null() {
            return _ptr == nullptr;
        }

        friend Iterator operator ++(Iterator& it, int unused) {
            Iterator copy = it;
            it._ptr = it._ptr -> _next;
            return copy;
        }
        
        ITEM_TYPE& operator *() {
            return _ptr -> _item;
        }

        const ITEM_TYPE& operator *() const {
            return _ptr -> _item;
        }
        
        ITEM_TYPE* operator ->() {
            assert(_ptr);
            return &(_ptr -> _item);
        }      

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._ptr != rhs._ptr;
        }
                                                
    private:
        node<ITEM_TYPE>* _ptr;                          
    };

    Queue();
    ~Queue();
    Queue(const Queue<ITEM_TYPE> &copyThis);
    Queue& operator =(const Queue& RHS);
                                                        
    bool empty() const;     
    int size() const;

    ITEM_TYPE front();
    ITEM_TYPE back();

    void push(ITEM_TYPE);
    ITEM_TYPE pop();

    Iterator begin() const;
    Iterator end() const;

    void print_pointers();

    template <class U>
    friend ostream& operator <<(ostream& outs, const Queue<U>& l);

private:
    node<ITEM_TYPE>* head;
    node<ITEM_TYPE>* tail;
    int _size;
};

template <typename ITEM_TYPE>
Queue<ITEM_TYPE>::Queue():head(nullptr), tail(nullptr), _size(0) {
    // Empty
}

template <typename ITEM_TYPE>
Queue<ITEM_TYPE>::~Queue() {
    _clear_list(head);
}

template <typename ITEM_TYPE>
Queue<ITEM_TYPE>::Queue(const Queue<ITEM_TYPE> &copyThis):_size(copyThis._size) {
    tail = _copy_list(head, copyThis.head);
}

template <typename ITEM_TYPE>
Queue<ITEM_TYPE>& Queue<ITEM_TYPE>::operator =(const Queue<ITEM_TYPE>& RHS) {
    if (&RHS == this) 
        return *this;

    _clear_list(head);
    tail = _copy_list(head, RHS.head);
    _size = RHS._size;
    return *this;
}
    
template <typename ITEM_TYPE>
bool Queue<ITEM_TYPE>::empty() const { 
    return head == nullptr; 
}      

template <typename ITEM_TYPE>
int Queue<ITEM_TYPE>::size() const { 
    return _size; 
}

template <typename ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::front() {
    return head -> _item;
}

template <typename ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::back() {
    return tail -> _item;
}

template <typename ITEM_TYPE>
void Queue<ITEM_TYPE>::push(ITEM_TYPE item) {
    tail = _insert_after(head, tail, item);
    _size++;
}

template <typename ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::pop() {
    assert(!empty() && "Attempted to pop when Queue was empty");
    ITEM_TYPE deleted = _delete_node(head, head);
    _size--;
    return deleted;
}

template <typename ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator Queue<ITEM_TYPE>::begin() const {
    return Queue<ITEM_TYPE>::Iterator(head);
}     

template <typename ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator Queue<ITEM_TYPE>::end() const {
    return Queue<ITEM_TYPE>::Iterator(nullptr);
}    

template <class U>
ostream& operator <<(ostream& outs, const Queue<U>& l) {
    for (node<U> *walker = l.head; walker != nullptr; walker = walker->_next) {
        outs << *walker;
    }
    outs << "|||";
    return outs;
}

template <typename ITEM_TYPE>
void Queue<ITEM_TYPE>::print_pointers() {
    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next) {
        cout << walker << " -> ";
    }
    cout << "|||";
}

#endif