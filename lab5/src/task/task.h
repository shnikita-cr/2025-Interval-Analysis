#pragma once

#include "../names.h"

template<class F>
struct Task {
    DIAV X;
    DIAV Y;
    F func;

    Task(const DIAV &x, const DIAV &y, F func) : X(x), Y(y), func(func) {
        validate();
    }

    bool validate() {
        return (X.getDim() == Y.getDim());
    }
};