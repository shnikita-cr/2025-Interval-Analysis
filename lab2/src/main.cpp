#include <iostream>
#include "interval/interval.h"
#include "secondary/tech.h"
#include "task/task.h"

using DI = Interval<double>;

double f(double x) {
    return f1(x);
}

double A = A1, B = B1;

//double f(double x) {
//    return f2(x);
//}
//
//double A = A2, B = B2;

int main() {
    std::cout << "main()" << std::endl;
    std::cout << DI(0) << std::endl;

    size_t n = 100;
    DVector x(n), y_f_linear(n), y_f_width(n);
    vectorFillXValues(x, A, B, n);
    vectorFillFValues(y_f_linear, A, B, n, f);

    vectorToFile(x, "../../data/x_h.txt");
    vectorToFile(y_f_linear, "../../data/y_f_linear.txt");
    return 0;
}
