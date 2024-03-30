#ifndef PIECEWISECONSTANTFUNCTION_H
#define PIECEWISECONSTANTFUNCTION_H

#include <iostream>

class PiecewiseConstantFunction {
public:
    PiecewiseConstantFunction();
    PiecewiseConstantFunction(const char* str);
    PiecewiseConstantFunction(const PiecewiseConstantFunction& other);
    ~PiecewiseConstantFunction();
    PiecewiseConstantFunction& operator=(const PiecewiseConstantFunction& other);

    double operator()(double x) const;
    double integral(double x1, double x2) const;

    PiecewiseConstantFunction operator+(const PiecewiseConstantFunction& other) const;
    PiecewiseConstantFunction operator-(const PiecewiseConstantFunction& other) const;
    PiecewiseConstantFunction operator*(const PiecewiseConstantFunction& other) const;
    PiecewiseConstantFunction operator/(const PiecewiseConstantFunction& other) const;
    PiecewiseConstantFunction& operator+=(const PiecewiseConstantFunction& other);
    PiecewiseConstantFunction& operator-=(const PiecewiseConstantFunction& other);
    PiecewiseConstantFunction& operator*=(const PiecewiseConstantFunction& other);
    PiecewiseConstantFunction& operator/=(const PiecewiseConstantFunction& other);

    friend std::ostream& operator<<(std::ostream& os, const PiecewiseConstantFunction& func);

private:
    double* values; // массив значений на каждом промежутке
    double* intervals; // массив границ промежутков
    int num_intervals; // количество промежутков

    // private methods
    void parse_string(const char* str);
};

#endif // PIECEWISECONSTANTFUNCTION_H