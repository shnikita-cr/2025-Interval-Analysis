#include <iostream>
#include "interval/interval.h"

void test_ar() {
    Interval x(1, 2);
    std::cout << x << std::endl;
    std::cout << x + x << std::endl;
    std::cout << x - x << std::endl;
    std::cout << x * x << std::endl;
    std::cout << x / x << std::endl;
}