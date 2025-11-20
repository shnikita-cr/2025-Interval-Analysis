#pragma once

#include <ostream>
#include <vector>
#include <algorithm>
#include <cmath>

template<class T>
class Interval {
private:
    T down;
    T up;
public:
    Interval(T _down, T _up) : down(_down), up(_up) {}

    Interval(T _mid) : down(_mid), up(_mid) {}

    bool operator==(const Interval &rhs) const {
        return down == rhs.down &&
               up == rhs.up;
    }

    bool operator!=(const Interval &rhs) const {
        return !(*this == rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const Interval &interval) {
        os << "interval: [" << interval.down << ", " << interval.up << "]";
        return os;
    }

    T length() const {
        return up - down;
    }

    bool contains(T x) const {
        return down <= x && x <= up;
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
        return !(l == r);
    }

    friend bool operator==(const T &l, const Interval<T> &r) {
        return r == l;
    }

    friend bool operator!=(const T &l, const Interval<T> &r) {
        return !(r == l);
    }

    Interval map(std::function<T(T)> f) const {
        return {f(down), f(up)};
    }
};