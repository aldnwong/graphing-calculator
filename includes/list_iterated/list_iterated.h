#ifndef ALDNWONG_LIST_H
#define ALDNWONG_LIST_H
#include <iostream>
#include <cassert>
#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"

using namespace std;

template <class ITEM_TYPE>
class List
{
public:
class Iterator{
    public:
        friend class List;                              
        Iterator():_ptr(nullptr) {
            // Empty
        }

        Iterator(node<ITEM_TYPE>* p):_ptr(p) {
            // Empty   
        } 

        operator bool() {
            return _ptr != nullptr;
        }                                

        ITEM_TYPE& operator *() {
            assert(_ptr);
            return _ptr -> _item;
        }

        const ITEM_TYPE& operator *() const {
            assert(_ptr);
            return _ptr -> _item;
        }

        ITEM_TYPE* operator ->() {
            assert(_ptr);
            return &(_ptr -> _item);
        }                     

        bool is_null() {
            return _ptr == nullptr;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._ptr != rhs._ptr;
        }

        Iterator& operator++() {
            assert(_ptr);
            _ptr = _ptr -> _next;
            return *this;
        }                        

        friend Iterator operator ++(Iterator& it, int unused) {
            assert(it._ptr);
            Iterator copy = it;
            it._ptr = it._ptr -> _next;
            return copy;
        }         

    private:
        node<ITEM_TYPE>* _ptr;                      
    };                    

    List();
    ~List();
    List(const List<ITEM_TYPE> &copyThis);
    List& operator =(const List& RHS);

    Iterator insert_head(ITEM_TYPE i);
    Iterator insert_after(ITEM_TYPE i, Iterator iMarker);
    Iterator insert_before(ITEM_TYPE i, Iterator iMarker);
    Iterator insert_sorted(ITEM_TYPE i); 
    ITEM_TYPE Delete(Iterator iMarker);
    
    ITEM_TYPE& operator[](int index);  
    Iterator prev(Iterator iMarker);  
    Iterator begin() const;  
    Iterator end() const;    
    Iterator last_node() const;            
    Iterator search(const ITEM_TYPE &key);          
                                                        
    bool empty() const;     
    int size() const;

    template <class U>
    friend ostream& operator <<(ostream& outs, const List<U>& l);
    void Print() const;

private:
    node<ITEM_TYPE>* head;
    int _size;
};

template <typename ITEM_TYPE>
List<ITEM_TYPE>::List():head(nullptr), _size(0) {
    // Empty
}

template <typename ITEM_TYPE>
List<ITEM_TYPE>::~List() {
    _clear_list(head);
}

template <typename ITEM_TYPE>
List<ITEM_TYPE>::List(const List<ITEM_TYPE> &copyThis):head(_copy_list(copyThis.head)), _size(copyThis._size) {
    // Empty
}

template <typename ITEM_TYPE>
List<ITEM_TYPE>& List<ITEM_TYPE>::operator =(const List<ITEM_TYPE>& RHS) {
    if (&RHS == this) 
        return *this;

    _clear_list(head);
    head = _copy_list(RHS.head);
    _size = RHS._size;
    return *this;
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::insert_head(ITEM_TYPE i) {
    node<ITEM_TYPE> *inserted_item = _insert_head(head, i);
    _size++;
    return List<ITEM_TYPE>::Iterator(inserted_item);
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::insert_after(ITEM_TYPE i, typename List<ITEM_TYPE>::Iterator iMarker) {
    node<ITEM_TYPE> *inserted_item = _insert_after(head, iMarker._ptr, i);
    _size++;
    return List<ITEM_TYPE>::Iterator(inserted_item);
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::insert_before(ITEM_TYPE i, typename List<ITEM_TYPE>::Iterator iMarker) {
    node<ITEM_TYPE> *inserted_item = _insert_before(head, iMarker._ptr, i);
    _size++;
    return List<ITEM_TYPE>::Iterator(inserted_item);
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::insert_sorted(ITEM_TYPE i) {
    node<ITEM_TYPE> *inserted_item = _insert_sorted(head, i);
    _size++;
    return List<ITEM_TYPE>::Iterator(inserted_item);
} 

template <typename ITEM_TYPE>
ITEM_TYPE List<ITEM_TYPE>::Delete(typename List<ITEM_TYPE>::Iterator iMarker) {
    ITEM_TYPE deleted = _delete_node(head, iMarker._ptr);
    _size--;
    return deleted;
}

template <typename ITEM_TYPE>
ITEM_TYPE& List<ITEM_TYPE>::operator[](int index) {
    ITEM_TYPE item = _at(head, index);
    return item;
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::prev(typename List<ITEM_TYPE>::Iterator iMarker) {
    node<ITEM_TYPE> *previous = _previous_node(head, iMarker._ptr);
    return List<ITEM_TYPE>::Iterator(previous);
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::begin() const {
    return List<ITEM_TYPE>::Iterator(head);
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::end() const {
    return List<ITEM_TYPE>::Iterator(nullptr);
}     

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::last_node() const {
    node<ITEM_TYPE>* ptr = _last_node(head);
    return List<ITEM_TYPE>::Iterator(ptr);
}

template <typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::search(const ITEM_TYPE &key) {
    node<ITEM_TYPE> *result = _search_list(head, key);
    return List<ITEM_TYPE>::Iterator(result);
}        
    
template <typename ITEM_TYPE>
bool List<ITEM_TYPE>::empty() const { 
    return head == nullptr; 
}      

template <typename ITEM_TYPE>
int List<ITEM_TYPE>::size() const { 
    return _size; 
}

template <class U>
ostream& operator <<(ostream& outs, const List<U>& l) {
    for (node<U> *walker = l.head; walker != nullptr; walker = walker->_next) {
        outs << *walker;
    }
    outs << "|||";
    return outs;
}

template <typename ITEM_TYPE>
void List<ITEM_TYPE>::Print() const {
    _print_list(head);
}

#endif