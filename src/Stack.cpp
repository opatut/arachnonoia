#include "Stack.hpp"

Stack::Stack() {}

void Stack::push(std::shared_ptr<State> state) {
    states.push(state);
}

void Stack::pop() {
    states.pop();
}

std::shared_ptr<State> Stack::current() {
    return states.top();
}
