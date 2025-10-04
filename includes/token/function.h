#ifndef ALDNWONG_FUNCTION_H
#define ALDNWONG_FUNCTION_H
#include <iostream>
#include <cassert>

using namespace std;

enum function_type {
    SIN,
    COS,
    TAN,
    CSC,
    SEC,
    COT,
    X,
    X_NEGATIVE,
    MAX,
    MIN,
    SQRT,
    LN,
    LOG,
    E
};

class Function : public Token {
public:
    Function(function_type item):Token(FUNCTION), _item(item) {
        // Empty
    }

    Function(string item):Token(FUNCTION) {
        for (auto &character : item) 
            character = toupper(character);
        
        if (item == "X")
            _item = X;
        else if (item == "-X")
            _item = X_NEGATIVE;
        else if (item == "E")
            _item = E;
        else if (item == "SIN")
            _item = SIN;
        else if (item == "COS")
            _item = COS;
        else if (item == "TAN")
            _item = TAN;
        else if (item == "CSC")
            _item = CSC;
        else if (item == "SEC")
            _item = SEC;
        else if (item == "COT")
            _item = COT;
        else if (item == "MAX")
            _item = MAX;
        else if (item == "MIN")
            _item = MIN;
        else if (item == "SQRT")
            _item = SQRT;
        else if (item == "LN")
            _item = LN;
        else if (item == "LOG")
            _item = LOG;
        else 
            assert(false && "Unknown function");
    }
    
    virtual void print(ostream& outs = cout) const {
        switch(_item) {
            case SIN:
                outs << "sin";
                break;
            case COS:
                outs << "cos";
                break;
            case TAN:
                outs << "tan";
                break;
            case CSC:
                outs << "csc";
                break;
            case SEC:
                outs << "sec";
                break;
            case COT:
                outs << "cot";
                break;
            case MAX:
                outs << "max";
                break;
            case MIN:
                outs << "min";
                break;
            case SQRT:
                outs << "sqrt";
                break;
            case E:
                outs << "e";
                break;
            case X:
                outs << "X";
                break;
            case LN:
                outs << "ln";
                break;
            case LOG:
                outs << "log";
                break;
            case X_NEGATIVE:
                outs << "-X";
                break;
        }
    }

    function_type get_function() {
        return _item;
    } 

private:
    function_type _item;
};

#endif