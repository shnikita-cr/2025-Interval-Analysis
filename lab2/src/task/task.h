#pragma once

#include "../interval/interval.h"
#include <cmath>

using DI = Interval<double>;

double f1(double x) {
    return pow(x, 3) - 3 * pow(x, 2) + 2;
}

DI f1_i(DI x) {
    return x.map(pow, 3) - 3 * x.map(pow, 2) + 2;
}

DI f1_g(DI x) {
    return x.map(pow, 2) * (x - 3) + 2;
}

DI f1_p(DI x) {
    return 3 * x.map(pow, 2) - 6 * x;
}

DI X1 = {0, 3};

double f2(double x) {
    return pow(x, 5) - 2 * pow(x, 3) + sin(x);
}

DI f2_i(DI x) {
    return x.map(pow, 5) - 2 * x.map(pow, 3) + x.map(sin);
}

DI f2_g(DI x) {
    return x.map(pow, 3) * (x.map(pow, 2) - 2) + x.map(sin);
}

DI f2_p(DI x) {
    return 5 * x.map(pow, 4) - 2 * 3 * x.map(pow, 2) + x.map(cos);
}

DI X2 = {0, 1.5};

uint32_t DOT_COUNT = 300;

struct Task {
    DI x;

    double (*f)(double);

    DI (*f_i)(DI);

    DI (*f_g)(DI);

    DI (*f_pi)(DI);

    uint32_t n;

    Task(const DI &x, double (*f)(double), DI (*fI)(DI), DI (*fG)(DI), DI (*fP)(DI), uint32_t n) : x(x), f(f),
                                                                                                   f_i(fI),
                                                                                                   f_g(fG),
                                                                                                   f_pi(fP),
                                                                                                   n(n) {}
};