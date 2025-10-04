#ifndef ALDNWONG_LEFTPAREN_H
#define ALDNWONG_LEFTPAREN_H
#include <iostream>

using namespace std;

class LeftParen : public Token {
    public:
        LeftParen():Token(LEFTPAREN) {
            // Empty
        }
    
        virtual void print(ostream& outs = cout) const {
            outs << "(";
        }
    
    private:
        
    };

#endif