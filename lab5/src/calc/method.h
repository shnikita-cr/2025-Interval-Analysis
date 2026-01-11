#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>
#include <string>

#include "../secondary/flogger.h"
#include "../secondary/interval/interval.h"
#include "../secondary/avector/avector.h"
#include "../names.h"

struct KrawczykRunResult {
    std::vector<DIAV> boxes;
    bool isEmpty = false;
};

static inline DV getMidPoint(const DIAV &x) {
    if (x.getDim() != 2) {
        throw std::runtime_error("getMidPoint: expected dim=2");
    }
    DV m(2);
    m[0] = x[0].getMid();
    m[1] = x[1].getMid();
    return m;
}

static inline DIAV intersectBox(const DIAV &a, const DIAV &b) {
    if (a.getDim() != 2 || b.getDim() != 2) {
        throw std::runtime_error("intersectBox: expected dim=2");
    }
    return DIAV({
                        intersect(a[0], b[0]),
                        intersect(a[1], b[1])
                });
}

static inline bool isBoxEmpty(const DIAV &x) {
    if (x.getDim() != 2) {
        throw std::runtime_error("isBoxEmpty: expected dim=2");
    }
    return x[0].isEmpty() || x[1].isEmpty();
}

void invert2x2(
        double a00, double a01,
        double a10, double a11,
        double &c00, double &c01,
        double &c10, double &c11) {

    const double det = a00 * a11 - a01 * a10;
    if (std::abs(det) < 1e-14) {
        throw std::runtime_error("invert2x2: singular (det ~ 0)");
    }

    const double invDet = 1.0 / det;
    c00 = a11 * invDet;
    c01 = -a01 * invDet;
    c10 = -a10 * invDet;
    c11 = a00 * invDet;
}

template<typename TaskT>
DIAV krawczykOperator(const TaskT &task, const DIAV &x, double x_c, double y_c) {
    flogger.log_start("krawczykOperator");

    if (x.getDim() != 2) {
        flogger.log_end("krawczykOperator");
        throw std::runtime_error("krawczykOperator: expected dim=2");
    }

    const DV x0 = getMidPoint(x);

    const double f0_1 = task.f1_d(x0);
    const double f0_2 = task.f2_d(x0);

    const double j00 = 1.0;
    const double j01 = -2.0 * x0[1];
    const double j10 = 2.0 * (x0[0] - x_c);
    const double j11 = 2.0 * (x0[1] - y_c);

    double c00, c01, c10, c11;
    invert2x2(j00, j01, j10, j11, c00, c01, c10, c11);

    const double base0 = x0[0] - (c00 * f0_1 + c01 * f0_2);
    const double base1 = x0[1] - (c10 * f0_1 + c11 * f0_2);

    const DI J11 = DI(1.0);
    const DI J12 = DI(-2.0) * x[1];
    const DI J21 = DI(2.0) * (x[0] - DI(x_c));
    const DI J22 = DI(2.0) * (x[1] - DI(y_c));

    const DI CJ00 = c00 * J11 + c01 * J21;
    const DI CJ01 = c00 * J12 + c01 * J22;
    const DI CJ10 = c10 * J11 + c11 * J21;
    const DI CJ11 = c10 * J12 + c11 * J22;

    const DI M00 = DI(1.0) - CJ00;
    const DI M01 = DI(0.0) - CJ01;
    const DI M10 = DI(0.0) - CJ10;
    const DI M11 = DI(1.0) - CJ11;

    const DI dx0 = x[0] - DI(x0[0]);
    const DI dx1 = x[1] - DI(x0[1]);

    const DI z0 = M00 * dx0 + M01 * dx1;
    const DI z1 = M10 * dx0 + M11 * dx1;

    const DI k0 = DI(base0) + z0;
    const DI k1 = DI(base1) + z1;

    const DIAV kx({k0, k1});

    flogger.log("x0 =", x0, "f(x0) =", f0_1, f0_2);
    flogger.log("C = [", c00, c01, ";", c10, c11, "]");
    flogger.log("K(x) =", kx);

    flogger.log_end("krawczykOperator");
    return kx;
}

template<typename TaskT>
KrawczykRunResult runKrawczyk(
        const TaskT &task,
        const DIAV &xInit,
        double x_c,
        double y_c,
        size_t iterations) {

    flogger.log_start("runKrawczyk");
    flogger.log("x_c =", x_c, "y_c =", y_c, "iterations =", iterations);

    if (xInit.getDim() != 2) {
        flogger.log_end("runKrawczyk");
        throw std::runtime_error("runKrawczyk: expected dim=2");
    }

    KrawczykRunResult out;
    out.boxes.reserve(iterations + 1);

    DIAV x = xInit;
    out.boxes.push_back(x);

    for (size_t k = 0; k < iterations; ++k) {
        flogger.log_start(std::string("iter ") + std::to_string(k));

        const DIAV kx = krawczykOperator(task, x, x_c, y_c);
        const DIAV next = intersectBox(x, kx);

        flogger.log("Xk =", x);
        flogger.log("K(Xk) =", kx);
        flogger.log("Xk+1 =", next);

        if (isBoxEmpty(next)) {
            flogger.log("empty intersection -> stop");
            out.isEmpty = true;
            flogger.log_end(std::string("iter ") + std::to_string(k));
            break;
        }

        x = next;
        out.boxes.push_back(x);

        flogger.log_end(std::string("iter ") + std::to_string(k));
    }

    flogger.log_end("runKrawczyk");
    return out;
}
