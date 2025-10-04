#include "shunting_yard.h"

ShuntingYard::ShuntingYard() {
    _infix = Queue<Token*>();
}

ShuntingYard::ShuntingYard(const Queue<Token*>& inf) {
    _infix = inf;
}

Queue<Token*> ShuntingYard::postfix(const Queue<Token*>& inf) {
    Stack<Token*> op_stack;
    Queue<Token*> postfix_queue;
    Stack<Token*> trig_hold;
    int paren_depth = 0;

    for (Queue<Token*>::Iterator it = inf.begin(); it != inf.end(); it++) { 
        Token *t = *it;
        switch(t->TypeOf()) {
            case INTEGER:
                postfix_queue.push(t);
                break;

            case OPERATOR:
                if (op_stack.empty() || op_stack.top()->TypeOf() == LEFTPAREN || op_stack.top()->TypeOf() == COMMA || *static_cast<Operator*>(op_stack.top()) < *static_cast<Operator*>(t)) {
                    op_stack.push(t);
                    break;
                }

                while (!op_stack.empty() && op_stack.top()->TypeOf() != LEFTPAREN && op_stack.top()->TypeOf() != COMMA && *static_cast<Operator*>(op_stack.top()) >= *static_cast<Operator*>(t)) 
                    postfix_queue.push(op_stack.pop());
                
                op_stack.push(t);
                break;

            case FUNCTION:
                switch(static_cast<Function*>(t)->get_function()) {
                    case X:
                    case X_NEGATIVE:
                    case E:
                        postfix_queue.push(t);
                        break;
                    
                    case SIN:
                    case COS:
                    case TAN:
                    case CSC:
                    case SEC:
                    case COT:
                    case MAX:
                    case MIN:
                    case SQRT:
                    case LN:
                    case LOG:
                        trig_hold.push(t);
                        break;
                }
                break;

            case COMMA:
                while (op_stack.top()->TypeOf() != LEFTPAREN) {
                    postfix_queue.push(op_stack.pop());
                }

                postfix_queue.push(t);
                break;

            case LEFTPAREN:
                op_stack.push(t);
                paren_depth++;
                break;

            case RIGHTPAREN:
                while (op_stack.top()->TypeOf() != LEFTPAREN) {
                    postfix_queue.push(op_stack.pop());
                }

                op_stack.pop();

                if (!trig_hold.empty() && paren_depth == trig_hold.size()) {
                    postfix_queue.push(trig_hold.pop());
                }

                paren_depth--;

                break;
        }
    }

    while (!op_stack.empty()) 
        postfix_queue.push(op_stack.pop());

    return postfix_queue;
}

Queue<Token*> ShuntingYard::postfix() {
    return postfix(_infix);
}

void ShuntingYard::infix(const Queue<Token*>& inf) {
    _infix = inf;
}

bool ShuntingYard::infix_string(string inf_str) {
    double num_hold = 0;
    bool is_num = false;
    bool holding_num = false;
    double num_mult = 10;
    int paren_expected = 0;
    Queue<Token*> infix_queue;
    for (auto it = inf_str.begin(); it != inf_str.end(); it++) {
        is_num = false;

        switch(*it) {
            case ' ':
                continue;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':    
                is_num = true;
                holding_num = true;
                if (num_mult > 0) { // if not decimal
                    num_hold *= 10;
                    num_hold += *it - '0';
                }
                else { // if decimal
                    num_hold += (*it - '0') * num_mult;
                    num_mult /= 10;
                }
                break;

            case '.':
                is_num = true;
                holding_num = true;
                num_mult = .1;
                break;

        }

        if (!is_num) {
            if (holding_num) {
                infix_queue.push(new Integer(num_hold));
                num_hold = 0;
                holding_num = false;
                num_mult = 10;
            }

            switch(*it) {
                case '+':
                case '*':
                case '/':
                case '^':
                    infix_queue.push(new Operator(*it));
                    break;

                case '-':
                    if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != LEFTPAREN && infix_queue.back()->TypeOf() != COMMA) {
                        infix_queue.push(new Operator(*it));
                    }
                    else {
                        infix_queue.push(new Integer(-1));
                        infix_queue.push(new Operator('*'));
                    }
                    break;

                case 'x':
                case 'X':
                    if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) {
                        infix_queue.push(new Operator('*'));
                    }

                    infix_queue.push(new Function("X"));
                    break;

                case 'e':
                case 'E':
                    if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) {
                        infix_queue.push(new Operator('*'));
                    }

                    infix_queue.push(new Function("E"));
                    break;

                case 's':
                case 'S':
                    it++;
                    if (*it == 'i' || *it == 'I') {
                        it++;
                        if (*it != 'n' && *it != 'N')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("sin"));
                    }
                    else if (*it == 'e' || *it == 'E') {
                        it++;
                        if (*it != 'c' && *it != 'C')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("sec"));
                    }
                    else if (*it == 'q' || *it == 'Q') {
                        it++;
                        if (*it != 'r' && *it != 'R')
                            return false;
                        it++;
                        if (*it != 't' && *it != 'T')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("sqrt"));
                    }
                    else {
                        return false;
                    }
                    break;

                case 'c':
                case 'C':
                    it++;
                    if (*it == 'o' || *it == 'O') {
                        it++;
                        if (*it == 's' || *it == 'S') {
                            if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                                infix_queue.push(new Operator('*'));
                            infix_queue.push(new Function("cos"));
                        }
                        else if (*it == 't' || *it == 'T') {
                            if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                                infix_queue.push(new Operator('*'));
                            infix_queue.push(new Function("cot"));
                        }
                        else {
                            return false;
                        }
                    }
                    else if (*it == 's' || *it == 'S') {
                        it++;
                        if (*it != 'c' && *it != 'C')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("csc"));
                    }
                    else {
                        return false;
                    }
                    break;

                case 't':
                case 'T':
                    it++;
                    if (*it == 'a' || *it == 'A') {
                        it++;
                        if (*it != 'n' && *it != 'N')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("tan"));
                    }
                    // else if (*it == 'h' || *it == 'H') {
                    //     it++;
                    //     if (*it != 'e' && *it != 'E')
                    //         return false;
                    //     it++;
                    //     if (*it != 't' && *it != 'T')
                    //         return false;
                    //     it++;
                    //     if (*it != 'a' && *it != 'A')
                    //         return false;
                        
                    // }
                    else {
                        return false;
                    }
                    break;
                
                case 'm':
                case 'M':
                    it++;
                    if (*it == 'a' || *it == 'A') {
                        it++;
                        if (*it != 'x' && *it != 'X')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("MAX"));
                    }
                    else if (*it == 'i' || *it == 'I') {
                        it++;
                        if (*it != 'n' && *it != 'N')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("MIN"));
                    }
                    else {
                        return false;
                    }
                    break;

                case '(':
                    if (!infix_queue.empty() && (infix_queue.back()->TypeOf() == INTEGER || infix_queue.back()->TypeOf() == RIGHTPAREN || (infix_queue.back()->TypeOf() == FUNCTION && static_cast<Function*>(infix_queue.back())->get_function() == X)))
                        infix_queue.push(new Operator('*'));
                    paren_expected++;
                    infix_queue.push(new LeftParen());
                    break;

                case ')':
                    paren_expected--;
                    infix_queue.push(new RightParen());
                    break;

                case ',':
                    infix_queue.push(new Comma());
                    break;

                case 'l':
                case 'L':
                    it++;
                    if (*it == 'o' || *it == 'O') {
                        it++;
                        if (*it != 'g' && *it != 'G')
                            return false;
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("LOG"));
                    }
                    else if (*it == 'n' || *it == 'N') {
                        if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != COMMA && infix_queue.back()->TypeOf() != LEFTPAREN) 
                            infix_queue.push(new Operator('*'));
                        infix_queue.push(new Function("LN"));
                    }
                    else {
                        return false;
                    }
                    break;

                case 'p':
                case 'P':
                    it++;
                    if (*it != 'i' && *it != 'I')
                        return false;
                    if (!infix_queue.empty() && infix_queue.back()->TypeOf() != OPERATOR && infix_queue.back()->TypeOf() != LEFTPAREN) {
                        infix_queue.push(new Operator('*'));
                    }
                    infix_queue.push(new Integer(M_PI));
                    break;

                default:
                    return false;
            }
        }
    }

    if (holding_num) {
        infix_queue.push(new Integer(num_hold));
        num_hold = 0;
        holding_num = false;
        num_mult = 10;
    }

    if (paren_expected != 0)
        return false;

    infix(infix_queue);

    return true;
}