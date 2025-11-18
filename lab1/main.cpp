#include <iostream>
#include "secondary/amatrix/amatrix.h"
#include "interval/interval.h"
#include "test_intervals.h"
#include "tech.h"
#include "equations/equation solving.h"
#include "secondary/tech.h"

using DI = Interval<double>;
using DIAM = AMatrix<DI>;
//using ADMatrix = AMatrix<double>;


DIAM getStartIntervalMatrix(double delta) {
    return {
            {DI(0.95 - delta, 0.95 + delta), DI(1.00 - delta, 1.00 + delta)},
//            {DI(1.05 - delta, 1.05 + delta), DI(1.05 - delta, 1.05 + delta)},
            {DI(1.10 - delta, 1.10 + delta), DI(1.10 - delta, 1.10 + delta)}
    };
}

DI ff(double delta) {
    DIAM a = getStartIntervalMatrix(delta);
    return det(a);
}

double f(double delta) {
//    return ff(delta).zeroDepth();
    return ff(delta).contains(1);
}

void bisection_test() {
    double a = 0, b = 5, eps = 0.001;

    auto ans = bisection_method(f, a, b, eps);
    std::cout << "ans: " << ans << std::endl;
    std::cout << "det ans: " << ff(ans) << std::endl;
    std::cout << getStartIntervalMatrix(ans) << std::endl;
}

void vis() {
    double a = -5,
            b = 5;
    uint32_t n = 300;
    DVector x_h(n),
            y_h(n);

    vectorFillXValues(x_h, a, b, n);
    vectorFillFValues(y_h, a, b, n, f);
    vectorToFile(x_h, "../data/x_h.txt");
    vectorToFile(y_h, "../data/y_h.txt");
}

int main() {
    bisection_test();
    vis();
    return 0;
}

