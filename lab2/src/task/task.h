#pragma once

#include "../interval/interval.h"
#include <cmath>

using DI = Interval<double>;

double f1(double x) {
    return pow(x, 3) - 3 * pow(x, 2) + 2;
}

double A1 = 0, B1 = 3;

double f2(double x) {
    return pow(x, 5) - 2 * pow(x, 3) + sin(x);
}

double A2 = 0, B2 = 100;

uint32_t DOT_COUNT = 300;

struct Task {
    double A, B;

    double (*f)(double);

    uint32_t n;

    Task(double a, double b, double (*f)(double), uint32_t n) : A(a), B(b), f(f), n(n) {}
};