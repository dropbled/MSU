#include "PiecewiseConstantFunction.h"
#include <iostream>

int main() {
    PiecewiseConstantFunction func("1;2;3");
/*
    // Создание функции из строки
    const char* str = "1.5;0.0;2.0;5.0;3.0;10.0"; //строка имеет формат "значение1;интервал1;...;значениеN;интервалN")
    PiecewiseConstantFunction func(str);
    std::cout << "Функция имеет вид:" << std::endl;
    std::cout << func << std::endl;

    // Вычисление значения функции в точке
    double x = -1.0;
    std::cout << "Значение функции в x = " << x << ": " << func(x) << std::endl;
    x = 0.0;
    std::cout << "Значение функции в x = " << x << ": " << func(x) << std::endl;
    x = 6.0;
    std::cout << "Значение функции в x = " << x << ": " << func(x) << std::endl;
    x = 12.0;
    std::cout << "Значение функции в x = " << x << ": " << func(x) << std::endl;



    // Вычисление интеграла функции на отрезке
    double x1 = -2.0, x2 = 12.0;
    std::cout << "Интеграл фцнкции от " << x1 << " до " << x2 << ": " << func.integral(x1, x2) << std::endl;
    x1 = 0.0, x2 = 12.0;
    std::cout << "Интеграл фцнкции от " << x1 << " до " << x2 << ": " << func.integral(x1, x2) << std::endl;
    x1 = 3.0, x2 = 9.0;
    std::cout << "Интеграл фцнкции от " << x1 << " до " << x2 << ": " << func.integral(x1, x2) << std::endl;
    x1 = 5.0, x2 = 7.0;
    std::cout << "Интеграл фцнкции от " << x1 << " до " << x2 << ": " << func.integral(x1, x2) << std::endl;

    // Создание второй функции для тестирования арифметических операций
    PiecewiseConstantFunction func2("0.5;0.0;1.5;5.0;2.0;10.0");
    std::cout << "Функция 2 имеет вид::" << std::endl;
    std::cout << func2 << std::endl;


    // Тестирование операторов сложения, вычитания, умножения и деления
    PiecewiseConstantFunction sum = func + func2;
    PiecewiseConstantFunction diff = func - func2;
    PiecewiseConstantFunction prod = func * func2;
    PiecewiseConstantFunction quot = func / func2;

    std::cout << "Сумма функций:" << std::endl;
    std::cout << sum << std::endl;

    std::cout << "Разность функций:" << std::endl;
    std::cout << diff << std::endl;

    std::cout << "Произведение функций:" << std::endl;
    std::cout << prod << std::endl;

    std::cout << "Частное функций:" << std::endl;
    std::cout << quot << std::endl;

    func += func2;
    std::cout << "Функции 1 и 2 после оператора +=:" << std::endl;
    std::cout << func << std::endl;

    func = func2;
    std::cout << "Функция, после присваивания значения функции 2:" << std::endl;
    std::cout << func << "\n   =    \n" << func2 << std::endl;
*/
    /*
    func -= func2;
    std::cout << "Функции 1 и 2 после оператора -=:" << std::endl;
    std::cout << func << std::endl;
    func *= func2;
    std::cout << "Функции 1 и 2 после оператора *=:" << std::endl;
    std::cout << func << std::endl;
    func /= func2;
    std::cout << "Функции 1 и 2 после оператора /=:" << std::endl;
    std::cout << func << std::endl;
    */
    return 0;
}
