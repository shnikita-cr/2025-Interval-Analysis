#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <functional>

#include "../names.h"
#include "../secondary/tech/dgridresult.h"
#include "../secondary/tech/dgrid.h"
#include "../secondary/tech/evaluate_grid.h"
#include "../secondary/flogger.h"

class LogScope {
private:
    std::string name;
public:
    explicit LogScope(const std::string &name) : name(name) {
        flogger.log_start(name);
    }

    ~LogScope() {
        flogger.log_end(name);
    }
};

enum class MODEL_OP {
    ADD,
    MUL
};

inline double medianOfSorted(const std::vector<double> &v) {
    LogScope scope("medianOfSorted");
    const size_t n = v.size();
    flogger.log("size =", n);

    if (n == 0) throw std::runtime_error("medianOfSorted: empty vector");

    double res = 0.0;
    if (n % 2 == 1) {
        res = v[n / 2];
    } else {
        res = 0.5 * (v[n / 2 - 1] + v[n / 2]);
    }

    flogger.log("median =", res);
    return res;
}

inline double jaccardSample(const DIAV &a, const DIAV &b) {
    const size_t n = a.getDim();
    if (n == 0 || n != b.getDim()) return 0.0;

    double sumInter = 0.0;
    double sumUnion = 0.0;

    for (size_t i = 0; i < n; ++i) {
        if (a[i].isEmpty() || b[i].isEmpty()) continue;

        const double aDown = a[i].getDown();
        const double aUp = a[i].getUp();
        const double bDown = b[i].getDown();
        const double bUp = b[i].getUp();

        const double interLeft = std::max(aDown, bDown);
        const double interRight = std::min(aUp, bUp);
        const double interLen = std::max(0.0, interRight - interLeft);

        const double lenA = a[i].length();
        const double lenB = b[i].length();
        const double uniLen = lenA + lenB - interLen;

        sumInter += interLen;
        sumUnion += uniLen;
    }

    if (sumUnion <= 0.0) return 0.0;
    return sumInter / sumUnion;
}

inline DI computeMedK(const DIAV &x) {
    LogScope scope("computeMedK");

    const size_t n = x.getDim();
    flogger.log("dim =", n);
    if (n == 0) throw std::runtime_error("computeMedK: empty sample");

    std::vector<double> lows;
    std::vector<double> ups;
    lows.reserve(n);
    ups.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        if (x[i].isEmpty()) continue;
        lows.push_back(x[i].getDown());
        ups.push_back(x[i].getUp());
    }

    flogger.log("used =", lows.size());
    if (lows.empty()) throw std::runtime_error("computeMedK: all intervals are empty");

    std::sort(lows.begin(), lows.end());
    std::sort(ups.begin(), ups.end());

    const double mLow = medianOfSorted(lows);
    const double mUp = medianOfSorted(ups);

    const DI res(mLow, mUp);
    flogger.log("result =", res);
    return res;
}

inline DI computeMedP(const DIAV &x) {
    LogScope scope("computeMedP");

    const size_t n = x.getDim();
    flogger.log("dim =", n);
    if (n == 0) throw std::runtime_error("computeMedP: empty sample");

    std::vector<DI> v;
    v.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        if (!x[i].isEmpty()) v.push_back(x[i]);
    }

    flogger.log("used =", v.size());
    if (v.empty()) throw std::runtime_error("computeMedP: all intervals are empty");

    std::sort(v.begin(), v.end(), [](const DI &a, const DI &b) {
        const double am = a.getMid();
        const double bm = b.getMid();
        if (am < bm) return true;
        if (am > bm) return false;
        return a.getWidth() < b.getWidth();
    });

    const size_t m = v.size();
    if (m % 2 == 1) {
        const DI res = v[m / 2];
        flogger.log("result =", res);
        return res;
    }

    const DI &left = v[m / 2 - 1];
    const DI &right = v[m / 2];

    const DI res(std::min(left.getDown(), right.getDown()),
                 std::max(left.getUp(), right.getUp()));
    flogger.log("result =", res);
    return res;
}

inline DI computeMode(const DIAV &x) {
    LogScope scope("computeMode");

    const size_t n = x.getDim();
    flogger.log("dim =", n);
    if (n == 0) throw std::runtime_error("computeMode: empty sample");

    struct Event {
        double pos;
        int type;
    };

    std::vector<Event> ev;
    ev.reserve(n * 2);

    size_t usedIntervals = 0;
    for (size_t i = 0; i < n; ++i) {
        if (x[i].isEmpty()) continue;
        ev.push_back(Event{x[i].getDown(), 0});
        ev.push_back(Event{x[i].getUp(), 1});
        ++usedIntervals;
    }

    flogger.log("usedIntervals =", usedIntervals, "events =", ev.size());
    if (ev.empty()) throw std::runtime_error("computeMode: all intervals are empty");

    std::sort(ev.begin(), ev.end(), [](const Event &a, const Event &b) {
        if (a.pos < b.pos) return true;
        if (a.pos > b.pos) return false;
        return a.type < b.type;
    });

    int active = 0;

    int bestSegCover = -1;
    double bestSegL = ev[0].pos;
    double bestSegR = ev[0].pos;
    bool hasBestSeg = false;

    int bestPointCover = -1;
    double bestPoint = ev[0].pos;

    double prev = ev[0].pos;
    size_t i = 0;

    while (i < ev.size()) {
        const double cur = ev[i].pos;

        const double segLen = cur - prev;
        if (segLen > 0.0) {
            if (active > bestSegCover) {
                bestSegCover = active;
                bestSegL = prev;
                bestSegR = cur;
                hasBestSeg = true;
            } else if (active == bestSegCover && hasBestSeg) {
                bestSegL = std::min(bestSegL, prev);
                bestSegR = std::max(bestSegR, cur);
            }
        }

        int starts = 0;
        int ends = 0;

        while (i < ev.size() && ev[i].pos == cur && ev[i].type == 0) {
            ++starts;
            ++i;
        }
        while (i < ev.size() && ev[i].pos == cur && ev[i].type == 1) {
            ++ends;
            ++i;
        }

        const int pointCover = active + starts;
        if (pointCover > bestPointCover) {
            bestPointCover = pointCover;
            bestPoint = cur;
        }

        active += starts;
        active -= ends;
        prev = cur;
    }

    const DI res = (hasBestSeg && bestSegCover > 0) ? DI(bestSegL, bestSegR) : DI(bestPoint, bestPoint);
    flogger.log("bestSegCover =", bestSegCover, "bestPointCover =", bestPointCover, "result =", res);
    return res;
}

inline double evalModel(MODEL_OP op, double s, const DI &repX, const DI &repY) {
    if (op == MODEL_OP::ADD) {
        const DI yHat = repX + DI(s);
        return yHat.jaccard(repY);
    }
    const DI yHat = repX * s;
    return yHat.jaccard(repY);
}

inline double evalModel(MODEL_OP op, double s, const DIAV &x, const DIAV &y) {
    const size_t n = x.getDim();
    if (n == 0 || n != y.getDim()) return 0.0;

    double sumInter = 0.0;
    double sumUnion = 0.0;

    if (op == MODEL_OP::ADD) {
        for (size_t i = 0; i < n; ++i) {
            if (x[i].isEmpty() || y[i].isEmpty()) continue;

            const double xDown = x[i].getDown() + s;
            const double xUp = x[i].getUp() + s;

            const double yDown = y[i].getDown();
            const double yUp = y[i].getUp();

            const double interLeft = std::max(xDown, yDown);
            const double interRight = std::min(xUp, yUp);
            const double interLen = std::max(0.0, interRight - interLeft);

            const double lenX = std::max(0.0, xUp - xDown);
            const double lenY = y[i].length();
            const double uniLen = lenX + lenY - interLen;

            sumInter += interLen;
            sumUnion += uniLen;
        }
    } else {
        const double t = s;
        for (size_t i = 0; i < n; ++i) {
            if (x[i].isEmpty() || y[i].isEmpty()) continue;

            double xDown = x[i].getDown() * t;
            double xUp = x[i].getUp() * t;
            if (t < 0.0) std::swap(xDown, xUp);

            const double yDown = y[i].getDown();
            const double yUp = y[i].getUp();

            const double interLeft = std::max(xDown, yDown);
            const double interRight = std::min(xUp, yUp);
            const double interLen = std::max(0.0, interRight - interLeft);

            const double lenX = std::max(0.0, xUp - xDown);
            const double lenY = y[i].length();
            const double uniLen = lenX + lenY - interLen;

            sumInter += interLen;
            sumUnion += uniLen;
        }
    }

    if (sumUnion <= 0.0) return 0.0;
    return sumInter / sumUnion;
}

inline DGridResult runFullGrid(const std::string &scopeName,
                               const DIAV &x,
                               const DIAV &y,
                               const DI &range,
                               size_t pointsPerDim,
                               MODEL_OP op) {
    LogScope scope(scopeName);
    flogger.log("range =", range, "pointsPerDim =", pointsPerDim, "dimX =", x.getDim(), "dimY =", y.getDim());

    DGrid grid(std::vector<DI>{range});
    auto func = [&x, &y, op](const AVector<double> &p) -> double {
        return evalModel(op, p[0], x, y);
    };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);
    flogger.log("gridPoints =", r.points.size());

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());
    return r;
}

template<typename AggFunc>
inline DGridResult runAggGrid(const std::string &scopeName,
                              const DIAV &x,
                              const DIAV &y,
                              const DI &range,
                              size_t pointsPerDim,
                              MODEL_OP op,
                              AggFunc agg) {
    LogScope scope(scopeName);
    flogger.log("range =", range, "pointsPerDim =", pointsPerDim, "dimX =", x.getDim(), "dimY =", y.getDim());

    const DI repX = agg(x);
    const DI repY = agg(y);
    flogger.log("repX =", repX, "repY =", repY);

    DGrid grid(std::vector<DI>{range});
    auto func = [repX, repY, op](const AVector<double> &p) -> double {
        return evalModel(op, p[0], repX, repY);
    };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);
    flogger.log("gridPoints =", r.points.size());

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());
    return r;
}

inline DGridResult runB1AddGrid(const DIAV &x, const DIAV &y, const DI &aRange, size_t pointsPerDim) {
    return runFullGrid("runB1AddGrid", x, y, aRange, pointsPerDim, MODEL_OP::ADD);
}

inline DGridResult runB1MulGrid(const DIAV &x, const DIAV &y, const DI &tRange, size_t pointsPerDim) {
    return runFullGrid("runB1MulGrid", x, y, tRange, pointsPerDim, MODEL_OP::MUL);
}

inline DGridResult runB2AddGrid(const DIAV &x, const DIAV &y, const DI &aRange, size_t pointsPerDim) {
    return runAggGrid("runB2AddGrid", x, y, aRange, pointsPerDim, MODEL_OP::ADD, computeMode);
}

inline DGridResult runB2MulGrid(const DIAV &x, const DIAV &y, const DI &tRange, size_t pointsPerDim) {
    return runAggGrid("runB2MulGrid", x, y, tRange, pointsPerDim, MODEL_OP::MUL, computeMode);
}

inline DGridResult runB3AddGrid(const DIAV &x, const DIAV &y, const DI &aRange, size_t pointsPerDim) {
    return runAggGrid("runB3AddGrid", x, y, aRange, pointsPerDim, MODEL_OP::ADD, computeMedK);
}

inline DGridResult runB3MulGrid(const DIAV &x, const DIAV &y, const DI &tRange, size_t pointsPerDim) {
    return runAggGrid("runB3MulGrid", x, y, tRange, pointsPerDim, MODEL_OP::MUL, computeMedK);
}

inline DGridResult runB4AddGrid(const DIAV &x, const DIAV &y, const DI &aRange, size_t pointsPerDim) {
    return runAggGrid("runB4AddGrid", x, y, aRange, pointsPerDim, MODEL_OP::ADD, computeMedP);
}

inline DGridResult runB4MulGrid(const DIAV &x, const DIAV &y, const DI &tRange, size_t pointsPerDim) {
    return runAggGrid("runB4MulGrid", x, y, tRange, pointsPerDim, MODEL_OP::MUL, computeMedP);
}

inline DI estimateIntervalByEps(const DGridResult &r, double eps) {
    LogScope scope("estimateIntervalByEps");
    flogger.log("points =", r.points.size(), "eps =", eps);

    if (r.points.empty()) throw std::runtime_error("estimateIntervalByEps: empty grid result");
    if (eps < 0.0) eps = 0.0;

    auto maxIt = std::max_element(
            r.points.begin(), r.points.end(),
            [](const DP &a, const DP &b) { return a.getY() < b.getY(); }
    );

    const double fMax = maxIt->getY();
    const double thr = fMax - eps;
    flogger.log("FMax =", fMax, "threshold =", thr, "sMax =", maxIt->getX()[0]);

    bool any = false;
    double sMin = 0.0;
    double sMax = 0.0;
    size_t accepted = 0;

    for (const auto &pt: r.points) {
        if (pt.getY() < thr) continue;
        const double s = pt.getX()[0];

        if (!any) {
            sMin = sMax = s;
            any = true;
        } else {
            sMin = std::min(sMin, s);
            sMax = std::max(sMax, s);
        }
        ++accepted;
    }

    flogger.log("acceptedPoints =", accepted);

    if (!any) {
        const double s = maxIt->getX()[0];
        const DI res(s, s);
        flogger.log("result =", res);
        return res;
    }

    const DI res(sMin, sMax);
    flogger.log("result =", res);
    return res;
}
