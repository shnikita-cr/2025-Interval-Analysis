#pragma once

#include "../secondary/tech.h"
#include "../task/task.h"

void graph_f(const Task &task) {
    size_t n = 100;
    DVector x(n), y_f_linear(n), y_f_width(n);
    vectorFillXValues(x, task.A, task.B, n);
    vectorFillFValues(y_f_linear, task.A, task.B, n, task.f);

    vectorToFile(x, "../../data/x_h.txt");
    vectorToFile(y_f_linear, "../../data/y_f.txt");
}

DI ran(double (*f)(double), double a, double b, uint32_t n) {
    if (n < 2) {
        throw std::invalid_argument("Number of points must be at least 2");
    }
    double step = (b - a) / (n - 1);

    double x0 = a;
    double y0 = f(x0);
    double min_val = y0;
    double max_val = y0;

    for (uint32_t i = 1; i < n; ++i) {
        double x = a + i * step;
        double y = f(x);

        if (y < min_val) {
            min_val = y;
        }
        if (y > max_val) {
            max_val = y;
        }
    }

    return {min_val, max_val};
}

double find_l(const Task &task) {
    return 0;
}

DI estimate_f_B1(const Task &task) {
    return DI(0);
}

DI estimate_f_B2(const Task &task) {
    return DI(0);
}

DI estimate_f_B3(const Task &task) {
    return DI(0);
}

DI estimate_f_B4(const Task &task) {
    return DI(0);
}

DI estimate_f_B5(const Task &task) {
    return DI(0);
}