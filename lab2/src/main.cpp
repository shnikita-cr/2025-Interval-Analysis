#include <iostream>
#include "interval/interval.h"
#include "secondary/tech.h"
#include "task/task.h"

using DI = Interval<double>;

int main() {
    std::cout << "main()" << std::endl;
    std::cout << DI(0) << std::endl;

    size_t n = 100;
    DVector x(n), y_f_linear(n), y_f_width(n);
    double a = -10, b = 10;
    vectorFillXValues(x, a, b, n);
    vectorFillFValues(y_f_linear, a, b, n, f1_linear);

    vectorToFile(x, "../../data/x_h.txt");
    vectorToFile(y_f_linear, "../../data/y_f_linear.txt");
    return 0;
}
