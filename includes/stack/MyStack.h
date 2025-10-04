#ifndef ALDNWONG_MYSTACK_H
#define ALDNWONG_MYSTACK_H
#include <iostream>
#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"

using namespace std;

template <typename ITEM_TYPE>
class Stack
{
public:
    class Iterator{
    public:
        friend class Stack;                     
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

    Stack();
    ~Stack();
    Stack(const Stack<ITEM_TYPE> &copyThis);
    Stack& operator =(const Stack& RHS);

    void push(ITEM_TYPE item);
    ITEM_TYPE pop();

    ITEM_TYPE top();
    Iterator begin() const;  
    Iterator end() const;                

    bool empty() const;     
    int size() const;

    template <class U>
    friend ostream& operator <<(ostream& outs, const Stack<U>& l);

private:
    node<ITEM_TYPE>* head;
    int _size;
};

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack():head(nullptr), _size(0) {
    // Empty
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack() {
    _clear_list(head);
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE> &copyThis):head(_copy_list(copyThis.head)), _size(copyThis._size) {
    // Empty
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::operator =(const Stack<ITEM_TYPE>& RHS) {
    if (&RHS == this) 
        return *this;

    _clear_list(head);
    head = _copy_list(RHS.head);
    _size = RHS._size;
    return *this;
}

template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item) {
    _insert_head(head, item);
    _size++;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop() {
    if (_size == 0)
        throw 404;
    ITEM_TYPE deleted = _delete_node(head, head);
    _size--;
    return deleted;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top() {
    if (head == nullptr)
        return ITEM_TYPE();
    
    return head -> _item;
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const {
    return Stack<ITEM_TYPE>::Iterator(head);
}     

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const {
    return Stack<ITEM_TYPE>::Iterator(nullptr);
}     
    
template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty() const { 
    return head == nullptr; 
}      

template <typename ITEM_TYPE>
int Stack<ITEM_TYPE>::size() const { 
    return _size; 
}

template <class U>
ostream& operator <<(ostream& outs, const Stack<U>& l) {
    for (node<U> *walker = l.head; walker != nullptr; walker = walker->_next) {
        outs << *walker;
    }
    outs << "|||";
    return outs;
}

#endif