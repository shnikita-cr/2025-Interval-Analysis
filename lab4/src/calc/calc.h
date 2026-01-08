#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>

#include "../names.h"
#include "../secondary/tech/dgridresult.h"
#include "../secondary/tech/dgrid.h"
#include "../secondary/tech/evaluate_grid.h"
#include "../secondary/flogger.h"

//global

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

inline double medianOfSorted(const std::vector<double> &v) {
    LogScope scope("medianOfSorted");
    flogger.log("size =", v.size());

    const size_t n = v.size();
    if (n == 0) throw std::runtime_error("medianOfSorted: empty vector");

    if (n % 2 == 1) {
        const double res = v[n / 2];
        flogger.log("median =", res);
        return res;
    }

    const double res = 0.5 * (v[n / 2 - 1] + v[n / 2]);
    flogger.log("median =", res);
    return res;
}

inline double jaccardSample(const DIAV &a, const DIAV &b) {
    const size_t n = a.getDim();
    if (n != b.getDim() || n == 0) {
        flogger.log("result =", 0.0);
        return 0.0;
    }

    double sumInter = 0.0;
    double sumUnion = 0.0;
    size_t used = 0;

    for (size_t i = 0; i < n; ++i) {
        if (a[i].isEmpty() || b[i].isEmpty()) continue;

        const DI inter = intersect(a[i], b[i]);
        if (i == 0)
            flogger.debug("a[i]", a[i], "b[i]", b[i]);
        const double interLen = inter.length();

        const double lenA = a[i].length();
        const double lenB = b[i].length();
        const double uniLen = lenA + lenB - interLen;

        sumInter += interLen;
        sumUnion += uniLen;
        ++used;
    }

    if (sumUnion <= 0.0) {
        flogger.log("result =", 0.0);
        return 0.0;
    }

    const double res = sumInter / sumUnion;
//    flogger.debug("sumInter", sumInter, "sumUnion", sumUnion);
    flogger.debug("frac", res);
    return res;
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

    size_t used = 0;
    for (size_t i = 0; i < n; ++i) {
        if (x[i].isEmpty()) continue;
        lows.push_back(x[i].getDown());
        ups.push_back(x[i].getUp());
        ++used;
    }

    flogger.log("used =", used);
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

    size_t used = 0;
    for (size_t i = 0; i < n; ++i) {
        if (x[i].isEmpty()) continue;
        ev.push_back(Event{x[i].getDown(), 0});
        ev.push_back(Event{x[i].getUp(), 1});
        ++used;
    }

    flogger.log("usedIntervals =", used, "events =", ev.size());
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

inline DIAV predictMul(const DIAV &x, double t) {
    const size_t n = x.getDim();
    std::vector<DI> out;
    out.reserve(n);

    for (size_t i = 0; i < n; ++i) out.push_back(x[i] * t);
    return DIAV(out);
}

//B1

inline double evalB1Add(double a, const DIAV &x, const DIAV &y) {
    flogger.debug("evalB1Add a:", a);
    const double res = jaccardSample(x + a, y);
    return res;
}

inline double evalB1Mul(double t, const DIAV &x, const DIAV &y) {
    flogger.debug("evalB1Mul t:", t);
    const double res = jaccardSample(predictMul(x, t), y);
    return res;
}

inline DGridResult runB1AddGrid(const DIAV &x, const DIAV &y, const DI &aRange, size_t pointsPerDim) {
    LogScope scope("runB1AddGrid");
    flogger.log("aRange =", aRange, "pointsPerDim =", pointsPerDim, "dimX =", x.getDim(), "dimY =", y.getDim());

    DGrid grid(std::vector<DI>{aRange});
    auto func = [&x, &y](const AVector<double> &p) -> double { return evalB1Add(p[0], x, y); };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);
    flogger.log("gridPoints =", r.points.size());

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());
    return r;
}

inline DGridResult runB1MulGrid(const DIAV &x, const DIAV &y, const DI &tRange, size_t pointsPerDim) {
    LogScope scope("runB1MulGrid");
    flogger.log("tRange =", tRange, "pointsPerDim =", pointsPerDim, "dimX =", x.getDim(), "dimY =", y.getDim());

    DGrid grid(std::vector<DI>{tRange});
    auto func = [&x, &y](const AVector<double> &p) -> double { return evalB1Mul(p[0], x, y); };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);
    flogger.log("gridPoints =", r.points.size());

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());
    return r;
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
    size_t used = 0;

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
        ++used;
    }

    flogger.log("acceptedPoints =", used);

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

// B2

inline double evalB2Add(double a, const DI &modeX, const DI &modeY) {
    const DI aa(a);
    const DI yHat = modeX + aa;
    return yHat.jaccard(modeY);
}

inline double evalB2Mul(double t, const DI &modeX, const DI &modeY) {
    const DI yHat = modeX * t;
    return yHat.jaccard(modeY);
}

inline DGridResult runB2AddGrid(const DIAV &x, const DIAV &y, const DI &aRange, size_t pointsPerDim) {
    LogScope scope("runB2AddGrid");

    const DI modeX = computeMode(x);
    const DI modeY = computeMode(y);

    flogger.log("aRange =", aRange, "pointsPerDim =", pointsPerDim);
    flogger.log("modeX =", modeX, "modeY =", modeY);

    DGrid grid(std::vector<DI>{aRange});
    auto func = [modeX, modeY](AVector<double> p) -> double {
        return evalB2Add(p[0], modeX, modeY);
    };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());

    return r;
}

inline DGridResult runB2MulGrid(const DIAV &x, const DIAV &y, const DI &tRange, size_t pointsPerDim) {
    LogScope scope("runB2MulGrid");

    const DI modeX = computeMode(x);
    const DI modeY = computeMode(y);

    flogger.log("tRange =", tRange, "pointsPerDim =", pointsPerDim);
    flogger.log("modeX =", modeX, "modeY =", modeY);

    DGrid grid(std::vector<DI>{tRange});
    auto func = [modeX, modeY](AVector<double> p) -> double {
        return evalB2Mul(p[0], modeX, modeY);
    };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());

    return r;
}

inline double evalB3Add(double a, const DI& medKX, const DI& medKY) {
    const DI aa(a);
    const DI yHat = medKX + aa;
    return yHat.jaccard(medKY);
}

inline double evalB3Mul(double t, const DI& medKX, const DI& medKY) {
    const DI yHat = medKX * t;
    return yHat.jaccard(medKY);
}

inline DGridResult runB3AddGrid(const DIAV& x, const DIAV& y, const DI& aRange, size_t pointsPerDim) {
    LogScope scope("runB3AddGrid");

    const DI medKX = computeMedK(x);
    const DI medKY = computeMedK(y);

    flogger.log("aRange =", aRange, "pointsPerDim =", pointsPerDim);
    flogger.log("medKX =", medKX, "medKY =", medKY);

    DGrid grid(std::vector<DI>{aRange});
    auto func = [medKX, medKY](AVector<double> p) -> double {
        return evalB3Add(p[0], medKX, medKY);
    };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());

    return r;
}

inline DGridResult runB3MulGrid(const DIAV& x, const DIAV& y, const DI& tRange, size_t pointsPerDim) {
    LogScope scope("runB3MulGrid");

    const DI medKX = computeMedK(x);
    const DI medKY = computeMedK(y);

    flogger.log("tRange =", tRange, "pointsPerDim =", pointsPerDim);
    flogger.log("medKX =", medKX, "medKY =", medKY);

    DGrid grid(std::vector<DI>{tRange});
    auto func = [medKX, medKY](AVector<double> p) -> double {
        return evalB3Mul(p[0], medKX, medKY);
    };

    DGridResult r = evaluate_grid(grid, func, pointsPerDim, VERBOSE);

    const DP best = r.getMax();
    flogger.log("sMax =", best.getX()[0], "FMax =", best.getY());

    return r;
}
