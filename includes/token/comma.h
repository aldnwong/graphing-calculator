#ifndef ALDNWONG_COMMA_H
#define ALDNWONG_COMMA_H
#include <iostream>
#include "token.h"

using namespace std;

class Comma : public Token {
public:
    Comma():Token(COMMA) {
        // Empty
    }
    
    virtual void print(ostream& outs = cout) const {
        outs << ",";
    }

private:
    
};

#endif