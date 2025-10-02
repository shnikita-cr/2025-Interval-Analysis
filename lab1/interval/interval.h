#pragma once

#include <ostream>
#include <vector>
#include <algorithm>

template<class T>
class Interval {
private:
    T down;
    T up;
public:
    Interval(T a, T b) : down(a), up(b) {}

    Interval(T v) : down(v), up(v) {}

    bool operator==(const Interval &rhs) const {
        return down == rhs.down &&
               up == rhs.up;
    }

    bool operator!=(const Interval &rhs) const {
        return rhs != *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Interval &interval) {
        os << "interval : [" << interval.down << ", " << interval.up << "]";
        return os;
    }

    T length() {
        return up - down;
    }

    bool contains(T x) const {
        return down <= x && x <= up;
    }

    T zeroDepth() const {
        if (down <= 0 && up >= 0) {
            // ноль внутри, берём минимальное расстояние до границ
            return std::min(std::abs(down), std::abs(up));
        } else {
            // ноль снаружи, возвращаем отрицательное расстояние
            return -std::min(std::abs(down), std::abs(up));
        }
    }


    friend Interval operator+(const Interval<T> &l, const Interval<T> &r) {
        return Interval(l.down + r.down, l.up + r.up);
    }

    friend Interval operator-(const Interval<T> &l, const Interval<T> &r) {
        return Interval(l.down - r.up, l.up - r.down);
    }

    friend Interval operator*(const Interval<T> &l, const Interval<T> &r) {
        std::vector v = {
                l.down * r.down, l.down * r.up,
                l.up * r.down, l.up * r.up
        };

        return Interval(*std::min_element(v.begin(), v.end()), *std::max_element(v.begin(), v.end()));
    }

    friend Interval operator/(const Interval<T> &l, const Interval<T> &r) {
        if (r.contains(0)) {
            throw std::runtime_error("Interval /: r has 0");
        }
        std::vector v = {
                l.down / r.down, l.down / r.up,
                l.up / r.down, l.up / r.up
        };

        return Interval(*std::min_element(v.begin(), v.end()), *std::max_element(v.begin(), v.end()));
    }
};
