#pragma once

#include "../interval/interval.h"
#include "../secondary/amatrix/amatrix.h"

using DI = Interval<double>;
using DIAM = AMatrix<DI>;

const double A = 0, B = 1;

//DIAM getStartIntervalMatrix(double delta) {
//    return {
//            {DI(0.95 - delta, 0.95 + delta), DI(1.00 - delta, 1.00 + delta)},
//            {DI(1.05 - delta, 1.05 + delta), DI(1.00 - delta, 1.00 + delta)},
//            {DI(1.10 - delta, 1.10 + delta), DI(1.00 - delta, 1.00 + delta)}
//    };
//}

DIAM getStartIntervalMatrix(double delta) {
    return {
            {DI(0.95 - delta, 0.95 + delta), DI(1.00, 1.00)},
            {DI(1.05 - delta, 1.05 + delta), DI(1.00, 1.00)},
            {DI(1.10 - delta, 1.10 + delta), DI(1.00, 1.00)}
    };
}