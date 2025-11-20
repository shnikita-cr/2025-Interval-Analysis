#include <iostream>
#include "interval/interval.h"

using DI = Interval<double>;

int main() {
    std::cout << "main()" << std::endl;
    std::cout << DI(0) << std::endl;
    return 0;
}

