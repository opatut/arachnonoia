#ifndef STACK_HPP
#define STACK_HPP

#include <memory>
#include <stack>
#include "State.hpp"

class Stack {
public:
    Stack();

    void push(std::shared_ptr<State> state);
    void pop();
    std::shared_ptr<State> current();

private:
    std::stack<std::shared_ptr<State>> states;

};

#endif
