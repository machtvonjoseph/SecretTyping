#include <iostream>
#include <string>
#include <sstream>

int evaluate(const std::string& expression, size_t& index) {
    char op = expression[index++];
    int num1, num2;

    if (expression[index] == '(') {
        num1 = evaluate(expression, ++index);
    } else {
        std::istringstream(expression.substr(index)) >> num1;
        index += std::to_string(num1).size();
    }

    if (expression[index] == '(') {
        num2 = evaluate(expression, ++index);
    } else {
        std::istringstream(expression.substr(index)) >> num2;
        index += std::to_string(num2).size();
    }

    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            return num1 / num2;
        default:
            std::cerr << "Invalid operator" << std::endl;
            return 0;
    }
}

int main() {
    std::string expression = "(- 6(+ 1 2))";
    size_t index = 1; // Start from the second character (skip the initial parenthesis)
    int result = evaluate(expression, index);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
