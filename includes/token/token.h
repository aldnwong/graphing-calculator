#ifndef ALDNWONG_TOKEN_H
#define ALDNWONG_TOKEN_H
#include <iostream>

using namespace std;

enum token_type {
    INTEGER,
    OPERATOR,
    FUNCTION,
    LEFTPAREN,
    RIGHTPAREN,
    COMMA
};

class Token {
public:
    Token(token_type type):_token(type) {
        // Empty
    }
    
    token_type TypeOf(ostream& outs = cout) {
        return _token;
    }

    friend ostream& operator <<(ostream& outs, const Token& t) {
        t.print(outs);
        return outs;
    }

    friend ostream& operator <<(ostream& outs, const Token*& t) {
        t->print(outs);
        return outs;
    }

    virtual void print(ostream& outs = cout) const {
        outs << _token;
    }
private:
    token_type _token;
};

#endif