#pragma once

#include "../secondary/tech.h"
#include "../task/task.h"
#include "../tech.h"

void graph_f(const Task &task) {
    size_t n = 100;
    DVector x(n), y_f_linear(n), y_f_width(n);
    vectorFillXValues(x, task.x.getDown(), task.x.getUp(), n);
    vectorFillFValues(y_f_linear, task.x.getDown(), task.x.getUp(), n, task.f);

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
    if (task.n < 2) {
        throw std::invalid_argument("Number of points must be at least 2");
    }
    double step = (task.x.getUp() - task.x.getDown()) / (task.n - 1);

    double x0 = task.x.getDown();
    double y0 = task.f_pi({x0}).getMid();
    double max_val = std::abs(y0);

    for (uint32_t i = 1; i < task.n; ++i) {
        double x = task.x.getDown() + i * step;
        double y = task.f_pi({x}).getMid();

        if (std::abs(y) > max_val) {
            max_val = y;
        }
    }

    return max_val;
}

DI estimate_f_B1(const Task &task) {
    return task.f_i(task.x);
}

DI estimate_f_B2(const Task &task) {
    return task.f_g(task.x);
}

DI estimate_f_B3(const Task &task) {
    std::vector<DI> numbers = {task.x.getDown(), task.x.getMid(), task.x.getUp()};
    std::vector<double> ms = {task.x.getDown(), task.x.getMid(), task.x.getUp()};

    std::for_each(numbers.begin(), numbers.end(), [&](DI &n) {
        n = task.f(n.getMid()) + task.f_pi(task.x) * (task.x - n);
        printAll(n);
    });

    DI min_number = numbers.front();
    double min_m = ms.front();

    for (size_t i = 0; i < numbers.size(); i++) {
        if (numbers[i].getWidth() < min_number.getWidth()) {
            min_number = numbers[i];
            min_m = ms[i];
        }
    }

    printAll("m:", min_m);
    return min_number;
}

DI estimate_f_B4(const Task &task) {
    return DI(0);
}

DI estimate_f_B5(const Task &task) {
    return DI(0);
}