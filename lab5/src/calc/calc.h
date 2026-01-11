#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <functional>

#include "../names.h"
#include "../secondary/tech/dgridresult.h"
#include "../secondary/tech/dgrid.h"
#include "../secondary/tech/evaluate_grid.h"
#include "../secondary/flogger.h"

class LogScope {
private:
    std::string name;
public:
    explicit LogScope(const std::string &name) : name(name) {
        flogger.log_start(name);
    }

    ~LogScope() {
        flogger.log_end(name);
    }
};


static inline void saveFunctionGrid2d(
        const std::function<double(const DV &)> &f,
        const DGrid &grid,
        const std::string &fileName,
        size_t pointsPerDimX1,
        size_t pointsPerDimX2) {
    flogger.log_start("saveFunctionGrid2d");
    flogger.log("file =", fileName, "n1 =", pointsPerDimX1, "n2 =", pointsPerDimX2);

    const auto &b = grid.getBounds();
    if (b.size() != 2) throw std::runtime_error("saveFunctionGrid2d: grid must be 2D");
    if (pointsPerDimX1 < 2 || pointsPerDimX2 < 2)
        throw std::runtime_error("saveFunctionGrid2d: pointsPerDim must be >= 2");

    const double x1Down = b[0].getDown();
    const double x1Up = b[0].getUp();
    const double x2Down = b[1].getDown();
    const double x2Up = b[1].getUp();

    const double step1 = (x1Up - x1Down) / static_cast<double>(pointsPerDimX1 - 1);
    const double step2 = (x2Up - x2Down) / static_cast<double>(pointsPerDimX2 - 1);

    std::ofstream of(fileName);
    if (!of) throw std::runtime_error("saveFunctionGrid2d: cannot open file");

    of << std::setprecision(17);

    DV p(2);
    for (size_t j = 0; j < pointsPerDimX2; ++j) {
        const double x2 = x2Down + static_cast<double>(j) * step2;
        p[1] = x2;

        for (size_t i = 0; i < pointsPerDimX1; ++i) {
            const double x1 = x1Down + static_cast<double>(i) * step1;
            p[0] = x1;

            const double val = f(p);
            of << x1 << " " << x2 << " " << val << "\n";
        }
    }

    flogger.log_end("saveFunctionGrid2d");
}

static inline void saveFunctionGrid2d(
        const std::function<double(const DV &)> &f,
        const DGrid &grid,
        const std::string &fileName,
        size_t pointsPerDim = 200) {
    saveFunctionGrid2d(f, grid, fileName, pointsPerDim, pointsPerDim);
}

static inline void writeBoxPolyline2d(std::ostream &os, const DIAV &box) {
    if (box.getDim() != 2) throw std::runtime_error("writeBoxPolyline2d: box must be 2D");

    const double x1L = box[0].getDown();
    const double x1R = box[0].getUp();
    const double x2L = box[1].getDown();
    const double x2R = box[1].getUp();

    os << std::setprecision(17);

    os << x1L << " " << x2L << "\n";
    os << x1R << " " << x2L << "\n";
    os << x1R << " " << x2R << "\n";
    os << x1L << " " << x2R << "\n";
    os << x1L << " " << x2L << "\n";

    os << "NaN NaN\n";
}

static inline void saveBoxes2d(const std::vector<DIAV> &boxes, const std::string &fileName) {
    flogger.log_start("saveBoxes2d");
    flogger.log("file =", fileName, "count =", boxes.size());

    std::ofstream of(fileName);
    if (!of) throw std::runtime_error("saveBoxes2d: cannot open file");

    for (const auto & boxe : boxes) {
        writeBoxPolyline2d(of, boxe);
    }

    flogger.log_end("saveBoxes2d");
}

