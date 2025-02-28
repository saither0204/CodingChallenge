#include <iostream>
#include <sstream>
#include <stack>
#include <cctype>
using namespace std;

double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        default: throw runtime_error("Invalid operator");
    }
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double evaluateExpression(const string &expression) {
    stack<double> values;
    stack<char> operators;
    stringstream ss(expression);
    char token;
    
    while (ss >> token) {
        if (isdigit(token)) {
            ss.putback(token);
            double value;
            ss >> value;
            values.push(value);
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                if (values.size() < 2) throw runtime_error("Invalid expression");
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperation(a, b, op));
            }
            if (operators.empty()) throw runtime_error("Mismatched parentheses");
            operators.pop();
        } else if (token == '+' || token == '-' || token == '*' || token == '/') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                if (values.size() < 2) throw runtime_error("Invalid expression");
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(token);
        } else {
            throw runtime_error("Invalid character in expression");
        }
    }
    
    while (!operators.empty()) {
        if (values.size() < 2) throw runtime_error("Invalid expression");
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOperation(a, b, op));
    }
    
    if (values.size() != 1) throw runtime_error("Invalid expression");
    return values.top();
}

int main(int argc, char* argv[]) {
    string expression;
    
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            expression += argv[i];
            if (i < argc - 1) expression += " ";
        }
    } else {
        cout << "Enter an expression: ";
        getline(cin, expression);
    }
    
    try {
        double result = evaluateExpression(expression);
        cout << "Result: " << result << endl;
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}
