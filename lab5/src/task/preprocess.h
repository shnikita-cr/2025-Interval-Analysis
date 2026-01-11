#pragma once

#include "../names.h"

DIAV puff(const DV &x, double rad) {
    DIAV X(x.getDim());
    for (size_t i = 0; i < X.getDim(); i++) {
        X[i] = {x[i] - rad, x[i] + rad};
    }
    return X;
}