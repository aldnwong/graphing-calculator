#ifndef ALDNWONG_RPN_H
#define ALDNWONG_RPN_H
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/token.h"
#include "../token/integer.h"
#include "../token/operator.h"
#include "../token/function.h"
#include "../token/comma.h"

using namespace std;

enum trig_mode {
    DEGREE,
    RADIAN
};

class RPN {
public:
    RPN();
    RPN(const Queue<Token*>& queue);
    double operator()(double x_val = 0);
    void set_input(const Queue<Token*>& queue);
    void set_mode(trig_mode new_mode);
    trig_mode get_mode();

private:
    Queue<Token*> _queue;
    trig_mode mode;
};

#endif