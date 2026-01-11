#pragma once

template<typename Coord>
class Point {
    Coord x;
    double y;
public:
    Point(Coord _x, double _y) : x(_x), y(_y) {}

    Coord getX() const {
        return x;
    }

    void setX(Coord _x) {
        Point::x = _x;
    }

    double getY() const {
        return y;
    }

    void setY(double _y) {
        Point::y = _y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        os << point.x << " " << point.y;
        return os;
    }
};