#pragma once


#include <ostream>

template<typename... Args>
void printAll(Args... args) {
    (void) std::initializer_list<int>{((std::cout << args << " "), 0)...};
    std::cout << std::endl;
}

class DGrid {
private:
    std::vector<Interval<double>> bounds;

public:
    DGrid(const std::vector<Interval<double>> &bounds) : bounds(bounds) {}
};

template<typename Coord, typename Value>
class Point {
    Coord x;
    Value y;
public:
    Point(Coord _x, Value _y) : x(_x), y(_y) {}

    Coord getX() const {
        return x;
    }

    void setX(Coord _x) {
        Point::x = _x;
    }

    Value getY() const {
        return y;
    }

    void setY(Value _y) {
        Point::y = _y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        os << point.x << " " << point.y;
        return os;
    }
};

struct DGridResult {
    std::vector<Point<AVector<double>, double>> points;

    friend std::ostream &operator<<(std::ostream &os, const DGridResult &result) {
        for (const auto &point: result.points) {
            os << point << std::endl;
        }
        return os;
    }

    void saveFile(const std::string &fname) {
        std::ofstream of(fname);
        of << *this << std::endl;
    }

    Point<AVector<double>, double> getMax() {
        return *std::max_element(points.begin(), points.end(),
                                 [](const Point<AVector<double>, double> &a,
                                    const Point<AVector<double>, double> &b) -> bool {
                                     return a.getY() < b.getY();
                                 });
    }
};

DGridResult evaluate_grid(const DGrid &grid, const std::function<double(AVector<double>)> &func) {
    return DGridResult{};
}
