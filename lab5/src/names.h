#pragma once

#include "secondary/interval/interval.h"
#include "secondary/avector/avector.h"
#include "secondary/amatrix/amatrix.h"
#include "secondary/tech/point.h"

using DI = Interval<double>;
using DIAM = AMatrix<DI>;
using DIAV = AVector<DI>;
using DV = AVector<double>;
using DM = AMatrix<double>;
using DP = Point<AVector<double>>;
