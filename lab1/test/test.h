#pragma once

#include "../secondary/tech.h"
#include "../task/task.h"
#include "../tech.h"
#include "../equations/equation solving.h"

double f(double delta) {
    return allContainZero(delta);
}

void bisection_test() {
    std::cout << "bisection_test()" << std::endl;
    double eps = 0.001;

    auto ans = bisection_method(f, A, B, eps);
    std::cout << "ans: " << ans << std::endl;
    std::cout << getStartIntervalMatrix(ans) << std::endl;
}

void test_f() {
    double delta = 0;
    while (true) {
        std::cin >> delta;
        std::cout << "delta: " << delta << std::endl;
        auto dets = calculateAllDeterminants(getStartIntervalMatrix(delta));
        std::cout << "zero: " << aggregate_dets(dets, [](const std::vector<DI> &d) {
            DI res = 0;
            for (auto &i: d) {
                res += i;
            }
            return res;
        })
                  << std::endl;
        std::cout << "matrix: " << getStartIntervalMatrix(delta) << std::endl;
    }
}

void vis() {
    std::cout << "vis()" << std::endl;
    uint32_t n = 300;
    DVector x_h(n), y_h(n);

    vectorFillXValues(x_h, A, B, n);
    vectorFillFValues(y_h, A, B, n, f);
    vectorToFile(x_h, "../data/x_h.txt");
    vectorToFile(y_h, "../data/y_h.txt");
}