#pragma once

class DGrid {
private:
    std::vector <Interval<double>> bounds;
public:
    const std::vector <Interval<double>> &getBounds() const {
        return bounds;
    }

public:
    DGrid(const std::vector <Interval<double>> &bounds) : bounds(bounds) {}
};

