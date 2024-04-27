#include <string>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <cctype>
using namespace std;

class Parser {
    static char c;
    static stack<string> stk;

    static void gc() {
        cin >> c;
    }

    class S {
    public:
        void parse() {
            stk.push("(S)"); // Метка S
            A().parse(); // Разбираем A
            if (stk.top() == "(S)") {
                stk.pop();
            }
            while (c == '+') {
                if (stk.top() == "( ) )") {
                    stk.pop();
                }
                if (stk.top() == "( ( )") {
                    stk.pop();
                }
                int op1 = stoi(stk.top()); // Берем последнее получившееся число
                stk.push("(+)"); // Метка  +
                gc(); // Пропускаем '+'
                A().parse(); // Разбираем следующий A
                if (stk.top() == "( ) )") {
                    stk.pop(); 
                }
                if (stk.top() == "( ( )") {
                    stk.pop(); 
                }
                int op2 = stoi(stk.top()); // Берем новое число
                string Sum = to_string(op1+op2); //Считаем и кидаем в стек сумму
                stk.push("Сумма = ");
                stk.push(Sum);
            }
        }
    };

    class A {
    public:
        void parse() {
            stk.push("(A)"); // Метка A
            B().parse(); // Разбираем B
            if (stk.top() == "(A)") {
                stk.pop(); 
            }
            while (c == '*') {
                if (stk.top() == "( ) )") {
                    stk.pop(); 
                }
                if (stk.top() == "( ( )") {
                    stk.pop(); 
                }
                int op1 = stoi(stk.top());
                gc(); // Пропускаем '*'
                stk.push("(*)"); // Метка *
                B().parse(); // Разбираем следующий B
                if (stk.top() == "( ) )") {
                    stk.pop(); 
                }
                if (stk.top() == "( ( )") {
                    stk.pop(); 
                }
                int op2 = stoi(stk.top());
                string Pr = to_string(op1*op2);
                stk.push("Произведение = ");
                stk.push(Pr);
            }
        }
    };

    class B {
    public:
        void parse() {
            stk.push("(B)"); // Метка начала B
            if (isdigit(c)) { // Если это число
                int value = 0;
                while (isdigit(c)) {
                    value = value * 10 + (c - '0');
                    gc();
                }
                stk.push(to_string(value)); // Добавляем число в стек
            } else if (c == '(') { // Если это открывающая скобка
                stk.push("( ( )"); // Метка  (
                gc(); // Пропустить '('
                S().parse(); // Разбираем S
                if (c != ')') {
                    throw runtime_error("Ожидалось ')'");
                }
                stk.push("( ) )"); // Метка )
                gc(); // Пропускаем ')'
            } else {
                throw runtime_error("Неожиданный символ");
            }
        }
    };

public:
    void analyze() {
        try {
            gc(); // Начинаем парсинг
            S().parse(); // Разбираем выражение
            if (c != '@') { // Проверка окончания
                throw runtime_error("Ожидалось '@' в конце");
            }
            cout << "ОК, финальный вид стека / только ответ:" << endl;
            /*while (!stk.empty()) {
                cout << stk.top() << endl;
                stk.pop();
            }*/
            cout << stk.top() << endl;
        } catch (const exception &e) {
            cout << "Ожибка: " << e.what() << endl;
        }
    }
};

stack<string> Parser::stk;
char Parser::c;

int main() {
    cout << "Введите выражение (в конце '@'): ";
    Parser().analyze(); // Запускаем анализатор
    return 0;
}
