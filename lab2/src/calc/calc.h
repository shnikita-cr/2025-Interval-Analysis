#pragma once

#include "../secondary/tech.h"
#include "../task/task.h"
#include "../tech.h"

void graph_f(const Task &task) {
    size_t n = task.n;
    DVector x(n), y_f_linear(n), y_f_width(n);
    vectorFillXValues(x, task.x.getDown(), task.x.getUp(), n);
    vectorFillFValues(y_f_linear, task.x.getDown(), task.x.getUp(), n, task.f);

    vectorToFile(x, "../../data/x_h.txt");
    vectorToFile(y_f_linear, "../../data/y_f.txt");
}

DI ran(double (*f)(double), double a, double b, uint32_t n) {
    if (n < 2) {
        throw std::invalid_argument("Number of points must be at least 2");
    }
    double step = (b - a) / (n - 1);

    double x0 = a;
    double y0 = f(x0);
    double min_val = y0;
    double max_val = y0;

    for (uint32_t i = 1; i < n; ++i) {
        double x = a + i * step;
        double y = f(x);

        if (y < min_val) {
            min_val = y;
        }
        if (y > max_val) {
            max_val = y;
        }
    }

    return {min_val, max_val};
}

double find_l(const Task &task) {
    DI d = task.f_pi(task.x);
    return std::max(std::abs(d.getDown()), std::abs(d.getUp()));
}

DI estimate_f_B1(const Task &task) {
    return task.f_i(task.x);
}

DI estimate_f_B2(const Task &task) {
    return task.f_g(task.x);
}

DI estimate_f_B3(const Task &task) {
    std::vector<double> ms = {task.x.getDown(), task.x.getMid(), task.x.getUp()};

    DI FpX = task.f_pi(task.x);

    auto eval_for_m = [&](double m) -> DI {
        return task.f(m) + FpX * (task.x - DI(m));
    };

    DI best = eval_for_m(ms[0]);
    double best_m = ms[0];
    for (size_t i = 1; i < ms.size(); ++i) {
        double m = ms[i];
        DI cur = eval_for_m(m);
        if (cur.getWidth() < best.getWidth()) {
            best = cur;
            best_m = m;
        }
    }

#if VERBOSE
    printAll("B3 best center m =", best_m);
#endif
    return best;
}


static DI B4_for_center(const Task& task, double m) {
    DI X = task.x;
    double a = X.getDown(), b = X.getUp();
    DI Xm = DI(m);

    DI XL(a, m), XR(m, b);

    DI SL = task.f_pi(XL);
    DI SR = task.f_pi(XR);

    DI left  = task.f(m) + SL * (XL - Xm);
    DI right = task.f(m) + SR * (XR - Xm);
    return hull(left, right);
}


DI estimate_f_B4(const Task &task) {
    const int N = 11;
    DI X = task.x;
    double a = X.getDown(), b = X.getUp();
    double h = (b - a) / (N - 1);

    DI best = B4_for_center(task, a);
    double best_m = a;

    for (int i = 1; i < N; ++i) {
        double m = a + i * h;
        DI cur = B4_for_center(task, m);
        if (cur.getWidth() < best.getWidth()) {
            best = cur;
            best_m = m;
        }
    }
#if VERBOSE
    printAll("B4 best center m =", best_m);
#endif
    return best;
}

static DI mean_value_form(const Task &task, double c) {
    DI FpX = task.f_pi(task.x);
    return task.f(c) + FpX * (task.x - DI(c));
}

DI estimate_f_B5(const Task &task) {
    DI X = task.x;
    double m = X.getMid();
    double r = X.getRad();

    DI FpX = task.f_pi(X);
    double p = cut(FpX.getMid() / FpX.getRad(), DI(-1.0, 1.0));

    double c_down = m - p * r;
    double c_up = m + p * r;

    DI I_down = mean_value_form(task, c_down);
    DI I_up = mean_value_form(task, c_up);
    DI I = intersect(I_down, I_up);

#if VERBOSE
    printAll("B5 p =", p);
    printAll("B5 c_down =", c_down, " c_up =", c_up);
    printAll("B5 fmv(c_down) =", I_down);
    printAll("B5 fmv(c_up)   =", I_up);
    printAll("B5 intersect   =", I);
#endif

    if (I.isEmpty()) {
        DI H = hull(I_down, I_up);
#if VERBOSE
        printAll("B5 intersection empty, fallback hull =", H);
#endif
        return H;
    }
    return I;
}
