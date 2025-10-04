#ifndef ALDNWONG_SHUNTING_YARD_H
#define ALDNWONG_SHUNTING_YARD_H
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../token/token.h"
#include "../queue/MyQueue.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/token.h"
#include "../token/integer.h"
#include "../token/operator.h"
#include "../token/function.h"
#include "../token/leftparen.h"
#include "../token/rightparen.h"
#include "../token/comma.h"

using namespace std;

class ShuntingYard {
public:
    ShuntingYard();
    ShuntingYard(const Queue<Token*>& inf);
    Queue<Token*> postfix(const Queue<Token*>& inf);
    Queue<Token*> postfix();
    void infix(const Queue<Token*>& inf);
    bool infix_string(string inf_str);
private:
    Queue<Token*> _infix;
};

#endif