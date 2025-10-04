#ifndef ALDNWONG_NODE_H
#define ALDNWONG_NODE_H
#include <iostream>

using namespace std;

template <typename ITEM_TYPE>
struct node {
    node(const ITEM_TYPE& item = ITEM_TYPE(), node* next = nullptr):_item(item), _next(next) {
        // Empty
    }
    ITEM_TYPE _item;
    node* _next;

    template <typename T>
    friend std::ostream& operator <<(std::ostream& outs, const node<T> &printMe);
};

template <typename T>
ostream& operator <<(ostream& outs, const node<T> &printMe) {
    outs << "[" << printMe._item << "] -> ";
    return outs;
}

#endif