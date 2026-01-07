#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include "../../main.h"

const int INTERVAL_PRECISION = PRECISION, INTERVAL_MAX_ELEMENT_WIDTH = INTERVAL_PRECISION + 5;

template<class T>
class Interval {
private:
    T down;
    T up;
public:
    Interval(T _down, T _up) : down(_down), up(_up) {}

    Interval(T _mid) : down(_mid), up(_mid) {}

    Interval(const Interval &other) {
        down = other.down;
        up = other.up;
    }

    Interval &operator=(const Interval &right) {
        if (this != &right) {
            down = right.down;
            up = right.up;
        }
        return *this;
    }

    bool operator==(const Interval<T> &rhs) const {
        return down == rhs.down &&
               up == rhs.up;
    }

    bool operator!=(const Interval<T> &rhs) const {
        return !(down == rhs.down && up == rhs.up);
    }

    friend std::ostream &operator<<(std::ostream &os, const Interval &interval) {
        std::cout << std::fixed << std::setprecision(INTERVAL_PRECISION);
        os << "interval: [" << std::setw(INTERVAL_MAX_ELEMENT_WIDTH) << interval.down << ", " << interval.up << "]";
        return os;
    }

    T length() const {
        return up - down;
    }

    bool contains(T x) const {
        return down <= x && x <= up;
    }

    bool in(const Interval<T> &rhs) {
        return down >= rhs.down && up <= rhs.up;
    }

    T zeroDepth() const {
        if (down <= 0 && up >= 0) {
            return std::min(std::abs(down), std::abs(up));
        } else {
            return -std::min(std::abs(down), std::abs(up));
        }
    }

    T getDown() const { return down; }

    T getUp() const { return up; }

    T getWidth() const { return up - down; }

    T getMid() const { return (up + down) / 2; }

    T getRad() const { return getWidth() / 2; }

    int getSgn() const {
        if (down >= 0 && up >= 0) {
            return 1;
        } else if (down <= 0 && up <= 0) {
            return -1;
        } else {
            return 0;
        }
    }

    T abs() const {
        return std::max(std::abs(up), std::abs(down));
    }


    friend T abs(const Interval<T> &I) {
        return std::max(std::abs(I.getUp()), std::abs(I.getDown()));
    }

    T magn() const {
        if (this->contains(0)) {
            return 0;
        } else {
            return std::min(std::abs(up), std::abs(down));
        }
    }

    friend Interval pow_i(const Interval<T> &I, unsigned int n) {
        if (n == 0) return Interval<T>(static_cast<T>(1));
        Interval<T> res(static_cast<T>(1));
        for (unsigned int i = 0; i < n; ++i) {
            res = res * I;
        }
        return res;
    }

    void sortEnds() {
        down = std::min(down, up);
        up = std::max(down, up);
    }

    void scale(T factor) {
        T mid = getMid(), rad = getRad();
        down = mid - rad * factor;
        up = mid + rad * factor;
    }

    T hi() {
        if (*this == 0) {
            return 0;
        }
        if (std::abs(down) <= std::abs(up)) {
            return down / up;
        } else {
            return up / down;
        }
    }

    [[nodiscard]] bool isEmpty() const {
        return down > up;
    }

    friend Interval hull(const Interval &a, const Interval &b) {
        if (a.isEmpty()) {
            return b;
        }
        if (b.isEmpty()) {
            return a;
        }
        return {std::min(a.getDown(), b.getDown()), std::max(a.getUp(), b.getUp())};
    }

    friend Interval intersect(const Interval &a, const Interval &b) {
        T d = std::max(a.getDown(), b.getDown());
        T u = std::min(a.getUp(), b.getUp());
        return Interval(d, u);
    }

    friend T dist(const Interval<T> &lhs, const Interval<T> &rhs) {
        return std::max(std::abs(lhs.down - rhs.down), std::abs(lhs.up - rhs.up));
    }

    friend T cut(const T &x, const Interval<T> &X) {
        if (x > X.getUp())
            return X.getUp();
        else if (x < X.getDown())
            return X.getDown();
        else
            return x;
    }

    // Comparison operators
    friend bool operator>(const Interval<T> &l, const Interval<T> &r) {
        return l.down > r.up;
    }

    friend bool operator>=(const Interval<T> &l, const Interval<T> &r) {
        return l.down >= r.up;
    }

    friend bool operator<(const Interval<T> &l, const Interval<T> &r) {
        return l.up < r.down;
    }

    friend bool operator<=(const Interval<T> &l, const Interval<T> &r) {
        return l.up <= r.down;
    }

    // Arithmetic operators with equality
    Interval &operator+=(const Interval<T> &rhs) {
        down += rhs.down;
        up += rhs.up;
        return *this;
    }

    Interval &operator-=(const Interval<T> &rhs) {
        down -= rhs.up;
        up -= rhs.down;
        return *this;
    }

    Interval &operator*=(const T &rhs) {
        down *= rhs;
        up *= rhs;
        if (rhs < 0) std::swap(down, up);
        return *this;
    }

    Interval &operator/=(const T &rhs) {
        if (rhs == 0) throw std::runtime_error("Division by zero");
        down /= rhs;
        up /= rhs;
        if (rhs < 0) std::swap(down, up);
        return *this;
    }

    // Unary operators
    Interval operator+() const {
        return *this;
    }

    Interval operator-() const {
        return Interval(-up, -down);
    }

    // Friend arithmetic operators
    friend Interval operator+(const Interval<T> &l, const Interval<T> &r) {
        return Interval(l.down + r.down, l.up + r.up);
    }

    friend Interval operator-(const Interval<T> &l, const Interval<T> &r) {
        return Interval(l.down - r.up, l.up - r.down);
    }

    friend Interval operator|(const Interval<T> &l, const Interval<T> &r) {
        //inner minus
        return Interval(l.down - r.down, l.up - r.up);
    }

    friend Interval operator*(const Interval<T> &l, const T &r) {
        if (r >= 0) {
            return Interval(l.down * r, l.up * r);
        } else {
            return Interval(l.up * r, l.down * r);
        }
    }

    friend Interval operator*(const T &l, const Interval<T> &r) {
        return r * l;
    }

    friend Interval operator/(const Interval<T> &l, const T &r) {
        if (r == 0) throw std::runtime_error("Division by zero");
        if (r >= 0) {
            return Interval(l.down / r, l.up / r);
        } else {
            return Interval(l.up / r, l.down / r);
        }
    }

    friend Interval operator*(const Interval<T> &l, const Interval<T> &r) {
        std::vector<T> v = {
                l.down * r.down, l.down * r.up,
                l.up * r.down, l.up * r.up
        };
        return Interval(*std::min_element(v.begin(), v.end()),
                        *std::max_element(v.begin(), v.end()));
    }

    friend Interval operator/(const Interval<T> &l, const Interval<T> &r) {
        if (r.contains(0)) {
            throw std::runtime_error("Interval division: divisor contains 0");
        }
        std::vector<T> v = {
                l.down / r.down, l.down / r.up,
                l.up / r.down, l.up / r.up
        };
        return Interval(*std::min_element(v.begin(), v.end()),
                        *std::max_element(v.begin(), v.end()));
    }

    // Scalar comparison operators
    friend bool operator>(const Interval<T> &l, const T &r) {
        return l.down > r;
    }

    friend bool operator>=(const Interval<T> &l, const T &r) {
        return l.down >= r;
    }

    friend bool operator<(const Interval<T> &l, const T &r) {
        return l.up < r;
    }

    friend bool operator<=(const Interval<T> &l, const T &r) {
        return l.up <= r;
    }

    friend bool operator>(const T &l, const Interval<T> &r) {
        return l > r.up;
    }

    friend bool operator>=(const T &l, const Interval<T> &r) {
        return l >= r.up;
    }

    friend bool operator<(const T &l, const Interval<T> &r) {
        return l < r.down;
    }

    friend bool operator<=(const T &l, const Interval<T> &r) {
        return l <= r.down;
    }

    // Equality comparison with scalar
    friend bool operator==(const Interval<T> &l, const T &r) {
        return (l.down == r) && (l.up == r);
    }

    friend bool operator!=(const Interval<T> &l, const T &r) {
        return l != r;
    }

    friend bool operator==(const T &l, const Interval<T> &r) {
        return r == l;
    }

    friend bool operator!=(const T &l, const Interval<T> &r) {
        return r != l;
    }

    template<typename F, typename ...Args>
    Interval map(const F &f, const Args &... args) const {
        return Interval(f(down, args...), f(up, args...));
    }
};