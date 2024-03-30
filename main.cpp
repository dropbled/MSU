#include "PiecewiseConstantFunction.h"
#include <iostream>

int main() {
    PiecewiseConstantFunction func("");
    /*
    // Создание функции из строки
    const char* str = "1.5;0.0;2.0;5.0;3.0;10.0"; // пример строкового представления (строка имеет формат "<значение1>;<интервал1>;<значение2>;<интервал2>;...;<значениеN>;<интервалN>")
    PiecewiseConstantFunction func(str);
    std::cout << "Function from string representation:" << std::endl;
    std::cout << func << std::endl;

    // Вычисление значения функции в точке
    double x = -1.0;
    std::cout << "Value of the function at x = " << x << ": " << func(x) << std::endl;
    x = 0.0;
    std::cout << "Value of the function at x = " << x << ": " << func(x) << std::endl;
    x = 6.0;
    std::cout << "Value of the function at x = " << x << ": " << func(x) << std::endl;
    x = 12.0;
    std::cout << "Value of the function at x = " << x << ": " << func(x) << std::endl;



    // Вычисление интеграла функции на отрезке
    double x1 = -2.0, x2 = 12.0;
    std::cout << "Integral of the function from " << x1 << " to " << x2 << ": " << func.integral(x1, x2) << std::endl;
    x1 = 0.0, x2 = 12.0;
    std::cout << "Integral of the function from " << x1 << " to " << x2 << ": " << func.integral(x1, x2) << std::endl;
    x1 = 3.0, x2 = 9.0;
    std::cout << "Integral of the function from " << x1 << " to " << x2 << ": " << func.integral(x1, x2) << std::endl;
    x1 = 5.0, x2 = 7.0;
    std::cout << "Integral of the function from " << x1 << " to " << x2 << ": " << func.integral(x1, x2) << std::endl;

    // Создание второй функции для тестирования арифметических операций
    PiecewiseConstantFunction func2("0.5;0.0;1.5;5.0;2.0;10.0");
    std::cout << "Function 2 from string representation:" << std::endl;
    std::cout << func2 << std::endl;


    // Тестирование операторов сложения, вычитания, умножения и деления
    PiecewiseConstantFunction sum = func + func2;
    PiecewiseConstantFunction diff = func - func2;
    PiecewiseConstantFunction prod = func * func2;
    PiecewiseConstantFunction quot = func / func2;

    std::cout << "Sum of two functions:" << std::endl;
    std::cout << sum << std::endl;

    std::cout << "Difference of two functions:" << std::endl;
    std::cout << diff << std::endl;

    std::cout << "Product of two functions:" << std::endl;
    std::cout << prod << std::endl;

    std::cout << "Quotient of two functions:" << std::endl;
    std::cout << quot << std::endl;

    // Тестирование комбинированных операторов
    std::cout << "Combined operators testing:" << std::endl;
    func += func2;
    std::cout << "Function after adding another function in place:" << std::endl;
    std::cout << func << std::endl;
    /*
    func -= func2;
    std::cout << "Function after adding another function in place:" << std::endl;
    std::cout << func << std::endl;
    func *= func2;
    std::cout << "Function after adding another function in place:" << std::endl;
    std::cout << func << std::endl;
    func /= func2;
    std::cout << "Function after adding another function in place:" << std::endl;
    std::cout << func << std::endl;
    */
    return 0;
}
