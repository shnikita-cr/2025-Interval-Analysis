#pragma once

#include "../names.h"
#include "../secondary/tech/dgrid.h"

struct Task {
    std::function<double(const DIAV &)> f1;
    std::function<double(const DIAV &)> f2;

    DGrid dGrid;

    Task(const std::function<double(const DIAV &)> &f1, const std::function<double(const DIAV &)> &f2,
         const DGrid &dGrid) : f1(f1), f2(f2), dGrid(dGrid) {}
};