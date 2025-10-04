#ifndef ALDNWONG_INTEGER_H
#define ALDNWONG_INTEGER_H
#include <iostream>
#include <string>
#include "token.h"

using namespace std;

class Integer : public Token {
public:
    Integer():Token(INTEGER), _item(0) {
        // Empty
    }
    
    Integer(double item):Token(INTEGER), _item(item) {
        // Empty
    }

    Integer(string item):Token(INTEGER) {
        _item = stod(item);
    }

    virtual void print(ostream& outs = cout) const {
        outs << _item;
    }

    double get_num() {
        return _item;
    } 

private:
    double _item;
};

#endif