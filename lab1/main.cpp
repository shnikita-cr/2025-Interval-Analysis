#pragma once

#include <iostream>
#include "secondary/amatrix/amatrix.h"
#include "interval/interval.h"
#include "test/test_intervals.h"
#include "test/test.h"

using DI = Interval<double>;
using DIAM = AMatrix<DI>;

int main() {
    std::cout << "main()" << std::endl;
    vis();
    calcTest();
//    testF();
    return 0;
}

