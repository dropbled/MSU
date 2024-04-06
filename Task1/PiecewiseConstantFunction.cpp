#include "PiecewiseConstantFunction.h"
#include <cstring>  // Для функции std::memcpy
#include <iostream>
#include <sstream> // Для чтения строк
#include <stdexcept> // Для std::invalid_argument и std::out_of_range
#include <limits> //Для std::numeric_limits<double>::infinity()

PiecewiseConstantFunction::PiecewiseConstantFunction() : values(nullptr), intervals(nullptr), num_intervals(0) {}

PiecewiseConstantFunction::PiecewiseConstantFunction(const char* str) : values(nullptr), intervals(nullptr), num_intervals(0) {
    if (str != nullptr && strlen(str) > 0) {
        parse_string(str);
    } else {
        std::cerr << "Ошибка: Пустая или неприемлемая строка" << std::endl;
    }
}

void PiecewiseConstantFunction::parse_string(const char* str) { //Парсер (строка имеет формат "значение1;интервал1;...;значениеN;интервалN")
    std::istringstream ss(str);
    std::string token;

    // Подсчет количества промежутков
    while (std::getline(ss, token, ';')) {
        num_intervals++;
    }
    if (num_intervals%2 != 0 ){
        std::cerr << "Ошибка: Неверное количество значений/интервалов" << std::endl;
    }
    num_intervals /= 2; // Поправка (иначе выделение работало криво)

    // Выделение памяти для массивов значений и интервалов
    values = new double[num_intervals];
    intervals = new double[num_intervals];

    // Разбор строки для заполнения массивов значений и интервалов
    ss.clear();
    ss.seekg(0);  // Сброс позиции потока в начало
    int i = 0;
    while (std::getline(ss, token, ';')) {
        try {
            values[i] = std::stod(token);
        } catch (const std::invalid_argument& ia) { // В случае ошибки преобразования строки в число выводим сообщение об ошибке
            std::cerr << "Ошибка преобразования строки в число: " << ia.what() << '\n';
            values[i] = 0.0;
        } catch (const std::out_of_range& oor) {    // В случае выхода за границы диапазона выводим сообщение об ошибке
            std::cerr << "Выход за границы диапазона: " << oor.what() << '\n';
            values[i] = 0.0;
        }

        // Считываем следующий интервал
        std::getline(ss, token, ';');
        try {
            intervals[i] = std::stod(token);
        } catch (const std::invalid_argument& ia) {
            std::cerr << "Ошибка преобразования строки в число: " << ia.what() << '\n';
            intervals[i] = 0.0;
        } catch (const std::out_of_range& oor) {
            std::cerr << "Выход за границы диапазона: " << oor.what() << '\n';
            intervals[i] = 0.0;
        }
        i++;
    }
}


// Конструктор копирования
PiecewiseConstantFunction::PiecewiseConstantFunction(const PiecewiseConstantFunction& other) : values(nullptr), intervals(nullptr), num_intervals(0) {
    num_intervals = other.num_intervals;
    values = new double[num_intervals];
    intervals = new double[num_intervals];
    std::memcpy(values, other.values, num_intervals * sizeof(double));
    std::memcpy(intervals, other.intervals, num_intervals * sizeof(double));
}

// Оператор присваивания
PiecewiseConstantFunction& PiecewiseConstantFunction::operator=(const PiecewiseConstantFunction& other) {
    if (this != &other) {
        delete[] values;
        delete[] intervals;
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
        interval_index++;
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
    double result = 0.0; 

    // Проходим по всем интервалам
    for (int i = 0; i < num_intervals; i++) {
        double interval_start = intervals[i]; 
        double interval_end = (i == num_intervals - 1) ? std::numeric_limits<double>::infinity() : intervals[i + 1];
        
        // Рассчитываем вклад текущего интервала в интеграл
        double interval_length = std::min(interval_end, x2) - std::max(interval_start, x1); 
        if (interval_length > 0) {
            double interval_value = values[i]; 
            result += interval_value * interval_length; 
        }
    }

    return result; 
}


// Оператор сложения
PiecewiseConstantFunction PiecewiseConstantFunction::operator+(const PiecewiseConstantFunction& other) const {
    // Создаем новый объект с тем же количеством интервалов
    PiecewiseConstantFunction result;
    result.num_intervals = num_intervals;
    result.values = new double[num_intervals];
    result.intervals = new double[num_intervals];

    // Складываем значения функций
    for (int i = 0; i < num_intervals; i++) {
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
    for (int i = 0; i < num_intervals; i++) {
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
    for (int i = 0; i < num_intervals; i++) {
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
    for (int i = 0; i < num_intervals; i++) {
        if (other.values[i] != 0) {
            result.values[i] = values[i] / other.values[i];
        } else {
            // Обработка деления на ноль (бесконечность)
            result.values[i] = std::numeric_limits<double>::infinity();
        }
        result.intervals[i] = intervals[i];
    }
    return result;
}

// Операторы -=, *=, /=, +=
PiecewiseConstantFunction& PiecewiseConstantFunction::operator+=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; i++) {
        values[i] += other.values[i];
    }
    return *this;
}

PiecewiseConstantFunction& PiecewiseConstantFunction::operator-=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; i++) {
        values[i] -= other.values[i];
    }
    return *this;
}

PiecewiseConstantFunction& PiecewiseConstantFunction::operator*=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; i++) {
        values[i] *= other.values[i];
    }
    return *this;
}

PiecewiseConstantFunction& PiecewiseConstantFunction::operator/=(const PiecewiseConstantFunction& other) {
    for (int i = 0; i < num_intervals; i++) {
        values[i] /= other.values[i];
    }
    return *this;
}
// Оператор <<
std::ostream& operator<<(std::ostream& os, const PiecewiseConstantFunction& func) {
    os << "Значения: ";
    for (int i = 0; i < func.num_intervals; i++) {
        os << func.values[i] << " ";
    }
    os << "\nИнтервалы: ";
    for (int i = 0; i < func.num_intervals; i++) {
        os << func.intervals[i] << " ";
    }
    return os;
}