#include <iostream>
#include "secondary/amatrix/amatrix.h"
#include "interval/interval.h"
#include "test_intervals.h"
#include "tech.h"
#include "equations/equation solving.h"
#include "secondary/tech.h"
#include "task/task.h"

using DI = Interval<double>;
using DIAM = AMatrix<DI>;
//using ADMatrix = AMatrix<double>;

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
    double a = 0, b = 5;
    uint32_t n = 300;
    DVector x_h(n), y_h(n);

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

