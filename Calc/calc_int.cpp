#include <iostream>
#include <stack>
#include <stdexcept>
#include <cctype>
using namespace std;

class Parser {
    static char c;
    static stack<int> operands;

    static void gc() {
        cin >> c;
    }

    class S {
    public:
        void parse() {
            A().parse();

            while (c == '+') {
                gc(); // пропускаем '+'
                A().parse(); // разбираем следующий A
                int right = operands.top(); operands.pop();
                int left = operands.top(); operands.pop();
                operands.push(left + right);
            }
        }
    };

    class A {
    public:
        void parse() {
            B().parse(); // разбираем B

            while (c == '*') {
                gc(); // пропускаем '*'
                B().parse(); // разбираем следующий B
                int right = operands.top(); operands.pop();
                int left = operands.top(); operands.pop();
                operands.push(left * right);
            }
        }
    };

    class B {
    public:
        void parse() {
            if (isdigit(c)) { // если это число
                int value = 0;
                while (isdigit(c)) {
                    value = value * 10 + (c - '0');
                    gc();
                }
                operands.push(value);
            } else if (c == '(') { // если это открывающая скобка
                gc(); // пропустить '('
                S().parse(); // разбираем S
                if (c != ')') {
                    throw runtime_error("Expected ')'");
                }
                gc(); // пропускаем ')'
            } else {
                throw runtime_error("Unexpected character");
            }
        }
    };

public:
    void analyze() {
        try {
            gc(); // начинаем парсинг
            S().parse(); // разбираем выражение
            if (c != '@') { // проверка окончания
                throw runtime_error("Expected '@' at the end");
            }
            cout << "Result: " << operands.top() << endl; // вывод результата
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};

stack<int> Parser::operands;
char Parser::c;

int main() {
    cout << "Enter an expression (end with '@'): ";
    Parser().analyze(); // запускаем анализатор
    return 0;
}
