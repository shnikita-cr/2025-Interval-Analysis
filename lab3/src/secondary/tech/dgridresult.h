#pragma once

#include <ostream>
#include <fstream>
#include <vector>
#include "../avector/avector.h"
#include "point.h"

struct DGridResult {
    std::vector<Point<AVector<double>, double>>
            points;

    friend std::ostream &operator<<(std::ostream &os, const DGridResult &result) {
        for (const auto &point: result.points) {
            os << point << std::endl;
        }
        return os;
    }

    void saveFile(const std::string &fileName) const {
        std::ofstream of(fileName);
        of << *this << std::endl;
    }

    Point<AVector<double>, double> getMax() {
        auto max = std::max_element(points.begin(), points.end(),
                                    [](const Point<AVector<double>, double> &a,
                                       const Point<AVector<double>, double> &b) -> bool {
                                        return a.getY() < b.getY();
                                    });

        if (max != points.end()) {
            return *max;
        }
        throw std::runtime_error("DGridResult empty in getMax()");
    }
};