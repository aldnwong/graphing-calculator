#ifndef ALDNWONG_RIGHTPAREN_H
#define ALDNWONG_RIGHTPAREN_H
#include <iostream>

using namespace std;

class RightParen : public Token {
public:
    RightParen():Token(RIGHTPAREN) {
        // Empty
    }
    
    virtual void print(ostream& outs = cout) const {
        outs << ")";
    }

private:
    
};

#endif