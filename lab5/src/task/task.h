#pragma once

#include "../names.h"
#include "../secondary/tech/dgrid.h"

struct Task {
    std::function<double(const DV &)> f1_d;
    std::function<DI(const DIAV &)> f1_di;
    std::function<double(const DV &)> f2_d;
    std::function<DI(const DIAV &)> f2_di;
    DGrid dGrid;
};