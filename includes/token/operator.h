#ifndef ALDNWONG_OPERATOR_H
#define ALDNWONG_OPERATOR_H
#include <iostream>
#include <cassert>

using namespace std;

enum operator_type {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER
};

class Operator : public Token {
public:
    Operator(operator_type item):Token(OPERATOR), _item(item) {
        // Empty
    }
    
    Operator(string item):Token(OPERATOR) {
        switch(item.at(0)) {
            case '+':
                _item = ADD;
                break;
            case '-':
                _item = SUBTRACT;
                break;
            case '*':
                _item = MULTIPLY;
                break;
            case '/':
                _item = DIVIDE;
                break;
            case '^':
                _item = POWER;
                break;
            default:
                assert(false && "Unknown operator" && item.at(0));
                break;
        }
    }

    Operator(char item):Token(OPERATOR) {
        switch(item) {
            case '+':
                _item = ADD;
                break;
            case '-':
                _item = SUBTRACT;
                break;
            case '*':
                _item = MULTIPLY;
                break;
            case '/':
                _item = DIVIDE;
                break;
            case '^':
                _item = POWER;
                break;
            default:
                assert(false && "Unknown operator" && item);
                break;
        }
    }
    
    virtual void print(ostream& outs = cout) const {
        switch(_item) {
            case ADD:
                outs << "+";
                break;
            case SUBTRACT:
                outs << "-";
                break;
            case MULTIPLY:
                outs << "*";
                break;
            case DIVIDE:
                outs << "/";
                break;
            case POWER:
                outs << "^";
                break;
        }
    }

    friend bool operator <(Operator& lhs, Operator& rhs) {
        return (lhs.weight() < rhs.weight());
    }

    friend bool operator >(Operator& lhs, Operator& rhs) {
        return (lhs.weight() > rhs.weight());
    }

    friend bool operator ==(Operator& lhs, Operator& rhs) {
        return (lhs.weight() == rhs.weight());
    }

    friend bool operator >=(Operator& lhs, Operator& rhs) {
        return (lhs.weight() >= rhs.weight());
    }

    friend bool operator <=(Operator& lhs, Operator& rhs) {
        return (lhs.weight() <= rhs.weight());
    }

    operator_type get_operator() {
        return _item;
    } 

private:
    int weight() {
        switch(_item) {
            case ADD:
                return 1;
            case SUBTRACT:
                return 1;
            case MULTIPLY:
                return 2;
            case DIVIDE:
                return 2;
            case POWER:
                return 3;
        }
        return -1;
    }
    operator_type _item;
};

#endif