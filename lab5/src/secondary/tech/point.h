#pragma once

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