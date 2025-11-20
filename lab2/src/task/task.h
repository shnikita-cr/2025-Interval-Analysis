#pragma once

#include "../interval/interval.h"
#include <cmath>

using DI = Interval<double>;

double f1_linear(double x) {
    return pow(x, 3) - 3 * pow(x, 2) + 2;
}

double A1 = 0, B1 = 3;
