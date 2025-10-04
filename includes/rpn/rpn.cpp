#include "rpn.h"

RPN::RPN():mode(RADIAN) {
    _queue = Queue<Token*>();
}

RPN::RPN(const Queue<Token*>& queue):mode(RADIAN) {
    _queue = queue;
}

double RPN::operator() (double x_val) {
    Stack<Token*> stack;
    for (Queue<Token*>::Iterator it = _queue.begin(); it != _queue.end(); it++) {
        Token *t = *it;
        Integer *lhs, *rhs;
        switch(t->TypeOf()) {
            case INTEGER:
                stack.push(t);
                break;
                
            case OPERATOR:
                rhs = static_cast<Integer*>(stack.pop());
                lhs = static_cast<Integer*>(stack.pop());

                switch(static_cast<Operator*>(t)->get_operator()) {
                    case ADD:
                        stack.push(new Integer(lhs->get_num() + rhs->get_num()));
                        break;
                    
                    case SUBTRACT:
                        stack.push(new Integer(lhs->get_num() - rhs->get_num()));
                        break;
                    
                    case MULTIPLY:
                        stack.push(new Integer(lhs->get_num() * rhs->get_num()));
                        break;

                    case DIVIDE:
                        stack.push(new Integer(lhs->get_num() / rhs->get_num()));
                        break;

                    case POWER:
                        stack.push(new Integer(pow(lhs->get_num(), rhs->get_num())));
                        break;
                }
                rhs = nullptr;
                lhs = nullptr;
                break;

            case FUNCTION:
                switch(static_cast<Function*>(t)->get_function()) {
                    case SIN:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (mode == DEGREE)
                            stack.push(new Integer(sin((lhs->get_num() * M_PI)/180)));
                        else if (mode == RADIAN)
                            stack.push(new Integer(sin(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case COS:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (mode == DEGREE)
                            stack.push(new Integer(cos((lhs->get_num() * M_PI)/180)));
                        else if (mode == RADIAN)
                            stack.push(new Integer(cos(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case TAN:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (mode == DEGREE)
                            stack.push(new Integer(tan((lhs->get_num() * M_PI)/180)));
                        else if (mode == RADIAN)
                            stack.push(new Integer(tan(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case CSC:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (mode == DEGREE)
                            stack.push(new Integer(1.0/sin((lhs->get_num() * M_PI)/180)));
                        else if (mode == RADIAN)
                            stack.push(new Integer(1.0/sin(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case SEC:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (mode == DEGREE)
                            stack.push(new Integer(1.0/cos((lhs->get_num() * M_PI)/180)));
                        else if (mode == RADIAN)
                            stack.push(new Integer(1.0/cos(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case COT:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (mode == DEGREE)
                            stack.push(new Integer(1.0/tan((lhs->get_num() * M_PI)/180)));
                        else if (mode == RADIAN)
                            stack.push(new Integer(1.0/tan(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case LOG:
                        lhs = static_cast<Integer*>(stack.pop());
                        stack.push(new Integer(log10(lhs->get_num())));
                        lhs = nullptr;
                        break;

                    case LN:
                        lhs = static_cast<Integer*>(stack.pop());
                        stack.push(new Integer(log(lhs->get_num())));
                        lhs = nullptr;
                        break;
                    
                    case MAX:
                        rhs = static_cast<Integer*>(stack.pop());
                        stack.pop();
                        lhs = static_cast<Integer*>(stack.pop());
                        stack.push((lhs->get_num() > rhs->get_num()) ? lhs : rhs);
                        break;

                    case MIN:
                        rhs = static_cast<Integer*>(stack.pop());
                        stack.pop();
                        lhs = static_cast<Integer*>(stack.pop());
                        stack.push((lhs->get_num() < rhs->get_num()) ? lhs : rhs);
                        break;
                    
                    case SQRT:
                        lhs = static_cast<Integer*>(stack.pop());
                        if (lhs->get_num() <= 0)
                            stack.push(new Integer(INFINITY));
                        else
                            stack.push(new Integer(sqrt(lhs->get_num())));
                        
                        break;

                    case X:
                        stack.push(new Integer(x_val));
                        break;
                    
                    case E:
                        stack.push(new Integer(M_E));
                        break;

                    case X_NEGATIVE:
                        stack.push(new Integer(-1 * x_val));
                        break;
                }
                break;

            case COMMA:
                stack.push(new Comma());
                break;
        }
    }
    
    return static_cast<Integer*>(stack.pop())->get_num();
}

void RPN::set_mode(trig_mode new_mode) {
    mode = new_mode;
}

trig_mode RPN::get_mode() {
    return mode;
}

void RPN::set_input(const Queue<Token*>& queue) {
    _queue = queue;
}