#pragma once

#include "../interval/interval.h"
#include "../secondary/amatrix/amatrix.h"
#include <cmath>
#include <limits>

using DI = Interval<double>;

using DIAM = AMatrix<DI>;
using DIAV = AVector<DI>;

DIAM A1{
        {{0.65, 1.25}, {0.70, 1.30}},
        {{0.75, 1.35}, {0.70, 1.30}}
};
DIAV b1{{2.75, 3.15},
        {2.85, 3.25}};

//DIAM A2{
//        {},
//        {},
//        {}
//};
//DIAV b2{};
//
//DIAM A3{
//        {},
//        {},
//        {},
//        {}
//};
//DIAV b3{};


struct Task {
    DIAM As;

    DIAV bs;

    Task(const DIAM &as, const DIAV &bs) : As(as), bs(bs) {}

    Task() = default;
};