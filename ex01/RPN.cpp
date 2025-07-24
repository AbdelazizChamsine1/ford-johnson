#include "RPN.hpp"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

RPN::RPN() {}

RPN::RPN(const RPN& other) : stack(other.stack) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        stack = other.stack;
    }
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

void RPN::applyOperator(char op) {
    if (stack.size() < 2)
        throw std::runtime_error("Error");

    int right = stack.top();
    stack.pop();
    int left = stack.top();
    stack.pop();
    int result = 0;

    if (op == '+') result = left + right;
    else if (op == '-') result = left - right;
    else if (op == '*') result = left * right;
    else if (op == '/') {
        if (right == 0)
            throw std::runtime_error("Error: division by zero");
        result = left / right;
    }

    stack.push(result);
}

int RPN::evaluate(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (token.length() == 1 && isOperator(token[0])) {
            applyOperator(token[0]);
        } else if (token.length() == 1 && std::isdigit(token[0])) {
            int num = token[0] - '0';
            stack.push(num);
        } else {
            throw std::runtime_error("Error");
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Error");

    return stack.top();
}
