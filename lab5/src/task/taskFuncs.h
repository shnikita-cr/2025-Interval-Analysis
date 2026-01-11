#pragma once

#include "../names.h"
#include "taskValues.h"

double f1_d(const DV &x) {
    if (x.getDim() != 2) throw std::runtime_error("f1_d: expected dim=2");
    return x[0] - x[1] * x[1];
}

double f2_d(const DV &x) {
    if (x.getDim() != 2) throw std::runtime_error("f2_d: expected dim=2");
    const double dx = x[0] - x_c;
    const double dy = x[1] - y_c;
    return dx * dx + dy * dy - 1.0;
}

DI f2_di(const DIAV &x) {
    if (x.getDim() != 2) throw std::runtime_error("f2_di: expected dim=2");
    const DI dx = x[0] - DI(x_c);
    const DI dy = x[1] - DI(y_c);
    return (dx * dx) + (dy * dy) - DI(1.0);
}

DI f1_di(const DIAV &x) {
    if (x.getDim() != 2) throw std::runtime_error("f1_di: expected dim=2");
    return x[0] - (x[1] * x[1]);
}
