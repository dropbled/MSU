#include "PiecewiseConstantFunction.h"
#include <cstdlib>  // Для функции std::strtol
#include <cstring>  // Для функции std::strtok
#include <iostream>
#include <sstream>
#include <stdexcept> // Для std::invalid_argument и std::out_of_range
#include <limits>

PiecewiseConstantFunction::PiecewiseConstantFunction() : values(nullptr), intervals(nullptr), num_intervals(0) {}

PiecewiseConstantFunction::PiecewiseConstantFunction(const char* str) : values(nullptr), intervals(nullptr), num_intervals(0) {
    if (str != nullptr && strlen(str) > 0) {
        parse_string(str);
    } else {
        // Обработка случая, когда передана пустая строка или строка без символов
        std::cerr << "Error: Empty or invalid input string!" << std::endl;
    }
}

void PiecewiseConstantFunction::parse_string(const char* str) { //Парсер (строка имеет формат "<значение1>;<интервал1>;<значение2>;<интервал2>;...;<значениеN>;<интервалN>")
    std::istringstream ss(str);
    std::string token;

    // Подсчет количества промежутков
    while (std::getline(ss, token, ';')) {
        ++num_intervals;
    }
    num_intervals /= 2; // Поправка

    // Выделение памяти для массивов значений и интервалов
    values = new double[num_intervals];
    intervals = new double[num_intervals];

    // Повторный разбор строки для заполнения массивов значений и интервалов
    ss.clear();
    ss.seekg(0);  // Сброс позиции потока в начало
    int i = 0;
    while (std::getline(ss, token, ';')) {
        try {
            values[i] = std::stod(token);
        } catch (const std::invalid_argument& ia) {
            // В случае ошибки преобразования строки в число выводим сообщение об ошибке
            std::cerr << "Invalid argument: " << ia.what() << '\n';
            // Записываем нуль вместо значения
            values[i] = 0.0;
        } catch (const std::out_of_range& oor) {
            // В случае выхода за границы диапазона выводим сообщение об ошибке
            std::cerr << "Out of range: " << oor.what() << '\n';
            // Записываем нуль вместо значения
            values[i] = 0.0;
        }

        // Считываем следующий токен (интервал)
        std::getline(ss, token, ';');
        try {
            intervals[i] = std::stod(token);
        } catch (const std::invalid_argument& ia) {
            std::cerr << "Invalid argument: " << ia.what() << '\n';
            intervals[i] = 0.0;
        } catch (const std::out_of_range& oor) {
            std::cerr << "Out of range: " << oor.what() << '\n';
            intervals[i] = 0.0;
        }
        ++i;
    }
}


// Конструктор копирования
PiecewiseConstantFunction::PiecewiseConstantFunction(const PiecewiseConstantFunction& other) : values(nullptr), intervals(nullptr), num_intervals(0) {
    // Копируем данные из other
    num_intervals = other.num_intervals;
    values = new double[num_intervals];
    intervals = new double[num_intervals];
    std::memcpy(values, other.values, num_intervals * sizeof(double));
    std::memcpy(intervals, other.intervals, num_intervals * sizeof(double));
}

// Оператор присваивания
PiecewiseConstantFunction& PiecewiseConstantFunction::operator=(const PiecewiseConstantFunction& other) {
    if (this != &other) {
        // Освобождаем текущие ресурсы
        delete[] values;
        delete[] intervals;
        // Копируем данные из other
        num_intervals = other.num_intervals;
        values = new double[num_intervals];
        intervals = new double[num_intervals];
        std::memcpy(values, other.values, num_intervals * sizeof(double));
        std::memcpy(intervals, other.intervals, num_intervals * sizeof(double));
    }
    return *this;
}

// Деструктор
PiecewiseConstantFunction::~PiecewiseConstantFunction() {
    delete[] values;
    delete[] intervals;
}

// Оператор ()
double PiecewiseConstantFunction::operator()(double x) const {
    // Если точка x находится перед первым интервалом, возвращаем 0
    if (x < intervals[0]) {
        return 0.0;
    }
    // Находим промежуток, в котором находится точка x
    int interval_index = 0;
    while (interval_index < num_intervals && x >= intervals[interval_index]) {
        ++interval_index;
    }

    // Если точка x входит в последний интервал, возвращаем значение на этом интервале
    if (interval_index == num_intervals) {
        return values[num_intervals - 1];
    }

    // Если точка x входит в первый интервал, возвращаем значение на этом интервале
    if (interval_index == 0) {
        return values[0];
    }
    // Возвращаем значение на предыдущем интервале
    return values[interval_index - 1];
}


// Оператор интеграл
double PiecewiseConstantFunction::integral(double x1, double x2) const {
    double result = 0.0; // Переменная для хранения результата интегрирования

    // Проходим по всем интервалам
    for (int i = 0; i < num_intervals; ++i) {
        double interval_start = intervals[i]; // Начало текущего интервала
        double interval_end = (i == num_intervals - 1) ? std::numeric_limits<double>::infinity() : intervals[i + 1]; // Конец текущего интервала
        
        // Рассчитываем вклад текущего интервала в интеграл
        double interval_length = std::min(interval_end, x2) - std::max(interval_start, x1); // Длина интервала, попадающего в заданный промежуток
        if (interval_length > 0) {
            double interval_value = values[i]; // Значение функции на текущем интервале
            result += interval_value * interval_length; // Добавляем вклад интервала в интеграл
        }
    }

    return result; // Возвращаем результат интегрирования
}


// Оператор сложения
PiecewiseConstantFunction PiecewiseConstantFunction::operator+(const PiecewiseConstantFunction& other) const {
    // Создаем новый объект с тем же количеством интервалов
    PiecewiseConstantFunction result;
    result.num_intervals = num_intervals;
    result.values = new double[num_intervals];
    result.intervals = new double[num_intervals];

    // Складываем значения функций
    for (int i = 0; i < num_intervals; ++i) {
        result.values[i] = values[i] + other.values[i];
        result.intervals[i] = intervals[i];
    }
    return result;
}

// Оператор вычитания
PiecewiseConstantFunction PiecewiseConstantFunction::operator-(const PiecewiseConstantFunction& other) const {
    PiecewiseConstantFunction result;
    result.num_intervals = num_intervals;
    result.values = new double[num_intervals];
    result.intervals = new double[num_intervals];
    for (int i = 0; i < num_intervals; ++i) {
        result.values[i] = values[i] - other.values[i];
        result.intervals[i] = intervals[i];
    }
    return result;
}

// Оператор умножения
PiecewiseConstantFunction PiecewiseConstantFunction::operator*(const PiecewiseConstantFunction& other) const {
    PiecewiseConstantFunction result;
    result.num_intervals = num_intervals;
    result.values = new double[num_intervals];
    result.intervals = new double[num_intervals];
    for (int i = 0; i < num_intervals; ++i) {
        result.values[i] = values[i] * other.values[i];
        result.intervals[i] = intervals[i];
    }
    return result;
}

// Оператор деления
PiecewiseConstantFunction PiecewiseConstantFunction::operator/(const PiecewiseConstantFunction& other) const {
    PiecewiseConstantFunction result;
    result.num_intervals = num_intervals;
    result.values = new double[num_intervals];
    result.intervals = new double[num_intervals];
    for (int i = 0; i < num_intervals; ++i) {
        if (other.values[i] != 0) {
            result.values[i] = values[i] / other.values[i];
        } else {
            // Обработка деления на ноль, например, можно установить значение на этом интервале в бесконечность
            result.values[i] = std::numeric_limits<double>::infinity();
        }
        result.intervals[i] = intervals[i];
    }
    return result;
}

// Операторы -=, *=, /=, +=
PiecewiseConstantFunction& PiecewiseConstantFunction::operator+=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; ++i) {
        values[i] += other.values[i];
    }
    return *this;
}

PiecewiseConstantFunction& PiecewiseConstantFunction::operator-=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; ++i) {
        values[i] -= other.values[i];
    }
    return *this;
}

PiecewiseConstantFunction& PiecewiseConstantFunction::operator*=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; ++i) {
        values[i] *= other.values[i];
    }
    return *this;
}

PiecewiseConstantFunction& PiecewiseConstantFunction::operator/=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; ++i) {
        values[i] /= other.values[i];
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const PiecewiseConstantFunction& func) {
    os << "Values: ";
    for (int i = 0; i < func.num_intervals; ++i) {
        os << func.values[i] << " ";
    }
    os << "\nIntervals: ";
    for (int i = 0; i < func.num_intervals; ++i) {
        os << func.intervals[i] << " ";
    }
    return os;
}