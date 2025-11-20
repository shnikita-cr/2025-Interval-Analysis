#pragma once

#include "../secondary/tech.h"
#include "../task/task.h"
#include "../tech.h"
#include "../equations/equation solving.h"

double f(double delta) {
    return allContainZero(delta);
}

void calcTest() {
    std::cout << "calcTest()" << std::endl;
    double eps = 0.0001;

    auto ans = calcMethod(f, A, B, eps);
    std::cout << "ans: " << ans << std::endl;
    std::cout << getStartIntervalMatrix(ans) << std::endl;
    auto dets = calculateAllDeterminants(getStartIntervalMatrix(ans));
    for (const auto &d: dets) {
        std::cout << "\tresult matrix det: " << d;
        std::cout << "\tcontains zero? " << d.contains(0) << std::endl;
    }
    std::cout << std::endl;
}

void testF() {
    double delta = 0;
    while (true) {
        std::cin >> delta;
        std::cout << "delta: " << delta << std::endl;
        auto dets = calculateAllDeterminants(getStartIntervalMatrix(delta));
        std::cout << "sum: " << aggregate_dets(dets, [](const std::vector<DI> &d) {
            DI res = 0;
            for (auto &i: d) {
                std::cout << "\tdet_i: " << i << " contains 0? " << i.contains(0) << std::endl;
                res += i;
            }
            return res;
        }) << std::endl;
        std::cout << "matrix delta: " << getStartIntervalMatrix(delta) << std::endl;
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