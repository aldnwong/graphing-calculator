#ifndef ALDNWONG_LINKED_LIST_FUNCTIONS_H
#define ALDNWONG_LINKED_LIST_FUNCTIONS_H
#include <iostream>
#include <cassert>
#include "../node/node.h"

using namespace std;

// Declarations

template <typename ITEM_TYPE>
void _print_list(node<ITEM_TYPE>* head);
template <typename ITEM_TYPE>
void _print_list_backwards(node<ITEM_TYPE> *print_node);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _search_list(node<ITEM_TYPE>* head, ITEM_TYPE key);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_head(node<ITEM_TYPE> *&head, ITEM_TYPE insert_this);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_after(node<ITEM_TYPE>*& head, node<ITEM_TYPE> *after_this, ITEM_TYPE insert_this);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_before(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* before_this, ITEM_TYPE insert_this);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _previous_node(node<ITEM_TYPE>* head, node<ITEM_TYPE>* prev_to_this);
template <typename ITEM_TYPE>
ITEM_TYPE _delete_node(node<ITEM_TYPE>*&head, node<ITEM_TYPE>* delete_this);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _copy_list(node<ITEM_TYPE>* head);
template <typename T>
node<T> *_copy_list(node<T>* &dest, node<T> *src);
template <typename ITEM_TYPE>
void _clear_list(node<ITEM_TYPE>*& head);
template <typename ITEM_TYPE>
ITEM_TYPE& _at(node<ITEM_TYPE>* head, int pos);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending=true);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted_and_add(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending=true);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _where_this_goes(node<ITEM_TYPE>* head, ITEM_TYPE item, bool ascending=true);
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head); 

// Definitions

template <typename ITEM_TYPE>
void _print_list(node<ITEM_TYPE>* head) {
    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next) {
        cout << *walker;
    }
    cout << "|||";
}

template <typename ITEM_TYPE>
void _print_list_backwards(node<ITEM_TYPE> *print_node) {
    if (print_node == nullptr) {
        cout << "|||";
        return;
    }
    _print_list_backwards(print_node->_next);
    cout << print_node->_item;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _search_list(node<ITEM_TYPE>* head, ITEM_TYPE key) {
    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next) {
        if (walker->_item == key) {
            return walker;
        }
    }
    return nullptr;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_head(node<ITEM_TYPE> *&head, ITEM_TYPE insert_this) {
    node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insert_this, head);
    head = temp;
    return temp;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_after(node<ITEM_TYPE>*& head, node<ITEM_TYPE> *after_this, ITEM_TYPE insert_this) {
    if (head == nullptr || after_this == nullptr) 
        return _insert_head(head, insert_this);
    node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insert_this, after_this->_next);
    after_this->_next = temp;
    return temp;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_before(node<ITEM_TYPE>*& head, node<ITEM_TYPE>* before_this, ITEM_TYPE insert_this) {
    node<ITEM_TYPE> *after_this = _previous_node(head, before_this);

    if (after_this == nullptr) 
        return _insert_head(head, insert_this);

    return _insert_after(head, after_this, insert_this);
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _previous_node(node<ITEM_TYPE>* head, node<ITEM_TYPE>* prev_to_this) {
    if (head == nullptr || head == prev_to_this || prev_to_this == nullptr) 
        return nullptr;

    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next) {
        if (walker->_next == prev_to_this) {
            return walker;
        }
    }
    
    assert(false && "Node must exist in the list to use the _previous_node function.");
    return nullptr; // Program should terminate before it gets here but I'll put this here anyway so the compiler doesn't scream at me
}

template <typename ITEM_TYPE>
ITEM_TYPE _delete_node(node<ITEM_TYPE>*&head, node<ITEM_TYPE>* delete_this) {
    ITEM_TYPE item = delete_this->_item;
    node<ITEM_TYPE>* before_this = _previous_node(head, delete_this);

    if (before_this == nullptr)
        head = delete_this->_next;
    else    
        before_this->_next = delete_this->_next;

    delete delete_this;
    return item;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _copy_list(node<ITEM_TYPE>* head) {
    if (head == nullptr)
        return nullptr;

    node<ITEM_TYPE> *new_head = new node<ITEM_TYPE>(head->_item);
    node<ITEM_TYPE> *new_head_walker = new_head;

    for (node<ITEM_TYPE> *walker = head->_next; walker != nullptr; walker = walker->_next) {
        new_head_walker->_next = new node<ITEM_TYPE>(walker->_item);
        new_head_walker = new_head_walker->_next;
    }
    return new_head;
}

template <typename T>
node<T> *_copy_list(node<T>* &dest, node<T> *src) {
    if (src == nullptr)
        return nullptr;

    dest = _copy_list(src);

    node<T> *last_node = nullptr;

    for (node<T> *walker = src->_next; walker != nullptr; walker = walker->_next) {
        last_node = walker;
    }

    return last_node;
}

template <typename ITEM_TYPE>
void _clear_list(node<ITEM_TYPE>*& head) {
    node<ITEM_TYPE>* current_walker = head;
    node<ITEM_TYPE>* next_walker;
    
    while (current_walker != nullptr) {
        next_walker = current_walker->_next;
        delete current_walker;
        current_walker = next_walker;
    }

    head = nullptr;
}

template <typename ITEM_TYPE>
ITEM_TYPE& _at(node<ITEM_TYPE>* head, int pos) {
    node<ITEM_TYPE> *walker = head;
    for (int i = 0; i < pos; i++) {
        assert(walker != nullptr && "Index out of bounds");
        walker = walker->_next;
    }
    return walker->_item;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending) {
    node<ITEM_TYPE> *pos = _where_this_goes(head, item, ascending);
    if (head == nullptr || pos == nullptr)
        return _insert_head(head, item);

    if (pos->_item == item) 
        return nullptr;
    node<ITEM_TYPE> *item_ptr = _insert_after(head, pos, item);
    return item_ptr;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted_and_add(node<ITEM_TYPE>* &head, ITEM_TYPE item, bool ascending) {
    if (head == nullptr)
        return _insert_head(head, item);

    if (_search_list(head, item) != nullptr) {
        node<ITEM_TYPE> *match = _search_list(head, item);
        item += match->_item;
        _delete_node(head, match);
        return _insert_sorted_and_add(head, item, ascending);
    }

    node<ITEM_TYPE> *pos = _where_this_goes(head, item, ascending);

    if (pos == nullptr)
        return _insert_head(head, item);

    node<ITEM_TYPE> *item_ptr = _insert_after(head, pos, item);
    return item_ptr;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _where_this_goes(node<ITEM_TYPE>* head, ITEM_TYPE item, bool ascending) {
    if (head == nullptr || (!ascending && head->_item <= item)) 
        return nullptr;

    node <ITEM_TYPE> *before = head;
    
    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next) {
        if ((ascending && walker->_item >= item) || (!ascending && walker->_item <= item))
            break;
        
        before = walker;
    }

    return before;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head) {
    if (head == nullptr)
        return head;

    node<ITEM_TYPE> *walker = head;
    for (; walker->_next != nullptr; walker = walker->_next);
    return walker;
}

#endif