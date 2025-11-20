#include <iostream>
#include "../interval/interval.h"

void test_ar() {
    Interval x(1, 2);
    std::cout << "Initial: "<< x << std::endl;
    std::cout << x + x << std::endl;
    std::cout << x - x << std::endl;
    std::cout << x * x << std::endl;
    std::cout << x / x << std::endl;

    std::cout << x * 2 << std::endl;
    std::cout << x / 2 << std::endl;
}