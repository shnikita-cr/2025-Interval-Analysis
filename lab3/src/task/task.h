#pragma once

#include "../interval/interval.h"
#include <cmath>
#include <limits>

using DI = Interval<double>;

inline DI quad_range(double a, double b, double c, const DI &X) {
    double A = X.getDown(), B = X.getUp();
    auto q = [&](double x) { return a * x * x + b * x + c; };

    double qA = q(A), qB = q(B);
    double lo = std::min(qA, qB);
    double hi = std::max(qA, qB);

    if (a != 0.0) {
        double xv = -b / (2.0 * a);
        if (xv >= A && xv <= B) {
            double qv = q(xv);
            lo = std::min(lo, qv);
            hi = std::max(hi, qv);
        }
    }
    return DI(lo, hi);
}

inline DI sin_i_strict(const DI &X) {
    double A = X.getDown(), B = X.getUp();
    const double PI = 3.141592653589793238462643383279502884;
    const double TWO_PI = 2.0 * PI;

    auto sin_minmax_on = [&](double a, double b) -> DI {
        double sA = std::sin(a), sB = std::sin(b);
        double lo = std::min(sA, sB);
        double hi = std::max(sA, sB);

        auto check_hit = [&](double t0) {
            double kmin = std::ceil((a - t0) / TWO_PI);
            double kmax = std::floor((b - t0) / TWO_PI);
            return kmin <= kmax;
        };

        if (check_hit(PI / 2)) hi = 1.0;
        if (check_hit(3 * PI / 2)) lo = -1.0;

        return DI(lo, hi);
    };

    return sin_minmax_on(A, B);
}

inline DI cos_i_strict(const DI &X) {
    double A = X.getDown(), B = X.getUp();
    const double PI = 3.141592653589793238462643383279502884;
    const double TWO_PI = 2.0 * PI;

    auto cos_minmax_on = [&](double a, double b) -> DI {
        double cA = std::cos(a), cB = std::cos(b);
        double lo = std::min(cA, cB);
        double hi = std::max(cA, cB);

        auto check_hit = [&](double t0) {
            double kmin = std::ceil((a - t0) / TWO_PI);
            double kmax = std::floor((b - t0) / TWO_PI);
            return kmin <= kmax;
        };

        if (check_hit(0.0)) hi = 1.0;
        if (check_hit(PI)) lo = -1.0;

        return DI(lo, hi);
    };

    return cos_minmax_on(A, B);
}

// ---------------------- f1 ----------------------
inline double f1(double x) {
    return std::pow(x, 3) - 3 * std::pow(x, 2) + 2;
}

inline DI f1_i(DI x) {
    return x.map(pow, 3) - 3 * x.map(pow, 2) + 2;
}

inline DI f1_g(DI x) {
    return x.map(pow, 2) * (x - 3) + 2;
}

inline DI f1_p_tight(DI X) {
    return quad_range(3.0, -6.0, 0.0, X);
}

inline DI X1 = {0, 3};

// ---------------------- f2 ----------------------
inline double f2(double x) {
    return std::pow(x, 5) - 2 * std::pow(x, 3) + std::sin(x);
}

inline DI f2_i(DI x) {
    return x.map(pow, 5) - 2 * x.map(pow, 3) + sin_i_strict(x);
}

inline DI f2_g(DI x) {
    return x.map(pow, 3) * (x.map(pow, 2) - 2) + sin_i_strict(x);
}

inline DI f2_p(DI x) {
    return 5 * x.map(pow, 4) - 6 * x.map(pow, 2) + cos_i_strict(x);
}

inline DI X2 = {0, 1.5};

inline uint32_t DOT_COUNT = 300;

struct Task {
    DI x;

    double (*f)(double);

    DI (*f_i)(DI);

    DI (*f_g)(DI);

    DI (*f_pi)(DI);

    uint32_t n;

    Task(const DI &x, double (*f)(double), DI (*fI)(DI), DI (*fG)(DI), DI (*fP)(DI), uint32_t n)
            : x(x), f(f), f_i(fI), f_g(fG), f_pi(fP), n(n) {}
};