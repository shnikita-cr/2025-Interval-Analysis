#include "calc.h"
#include "../secondary/tech/print_all.h"
#include "../secondary/flogger.h"
#include "../secondary/tech/dgrid.h"
#include "../secondary/tech/evaluate_grid.h"
#include <limits>

double tol(const AVector<double> &x, const DIAM &A, const DIAV &b) {
    if (!(A.getRows() == b.getDim() && A.getCols() == x.getDim())) {
        printAll("Wrong shape for tol(x,A,B)!");
        return -1;
    }
    size_t m = b.getDim();
    double min = std::numeric_limits<double>::max();
    for (size_t i = 0; i < m; i++) {
        auto val = b[i].getRad() - (b[i].getMid() - (A[i], x)).abs();
        if (val < min) {
            min = val;
        }
    }
    return min;
}

bool check_is_empty_tolerance_set(const Task &task, const std::string &suffix_name) {
    flogger.log("check_is_empty_tolerance_set", "start");
    flogger.level++;
    auto bounds = task.bounds;
    DGrid grid(bounds);
    auto my_function = [&](const AVector<double> &x) -> double {
        int ans = 1;
        for (size_t i = 0; i < x.getDim(); i++) {
            ans *= (task.A[i], x).in(task.b[i]);
        }
        return ans;
    };

    DGridResult result;
    result = evaluate_grid(grid, my_function, task.n);
    Point maxPoint = result.getMax();
    result.saveFile("../data/" + std::to_string(task.id) + "/" + suffix_name + "_tolS" + ".txt");
    flogger.level--;
    flogger.log("check_is_empty_tolerance_set", "end");
    return maxPoint.getY() == 0;
}

void correct_b(const Task &task, const std::string &suffix_name, const DP &mx) {
    flogger.log_framed("correct_b", "start");
    flogger.level++;

    DIAV ext(task.b.getDim(), DI(-1, 1));
    DIAV b = task.b;
    double C = mx.getY() + 2;
    flogger.log("C coef for [-1, 1]:", C);
    ext = ext * C;
    b = b - ext;
    Task modifiedTask(task.A, b, task.bounds, task.id, task.n);

    tol_helper(modifiedTask, suffix_name, true);
    check_is_empty_tolerance_set(modifiedTask, suffix_name);

    flogger.log("A", modifiedTask.A);
    flogger.log("b", modifiedTask.b);

    flogger.level--;
    flogger.log_framed("correct_b", "end");
}

void innerMinus(DIAM &A, DIAM &E) {
    for (size_t i = 0; i < A.getRows(); i++) {
        for (size_t j = 0; j < A.getCols(); j++) {
            A[i][j] = (A[i][j] | E[i][j]);
        }
    }
}

double min(const DM &A) {
    double mx = std::numeric_limits<double>::max();
    for (size_t i = 0; i < A.getRows(); i++) {
        for (size_t j = 0; j < A.getCols(); j++) {
            if (A[i][j] < mx) {
                mx = A[i][j];
            }
        }
    }
    return mx;
}

double sum(const DV &A) {
    double sum = 0;
    for (size_t i = 0; i < A.getDim(); i++) {
        sum += A[i];
    }
    return sum;
}

DV rad(const DIAV &V) {
    DV radV(V.getDim(), 0);
    for (size_t i = 0; i < V.getDim(); i++) {
        radV[i] = V[i].getRad();
    }
    return radV;
}

DM rad(const DIAM &M) {
    DM radM(M.getRows(), M.getCols(), 0);
    for (size_t i = 0; i < M.getRows(); i++) {
        for (size_t j = 0; j < M.getCols(); j++) {
            radM[i][j] = M[i][j].getRad();
        }
    }
    return radM;
}

void correct_a(const Task &task, const std::string &suffix_name, const DP &mx) {
    flogger.log_framed("correct_a", "start");
    flogger.level++;

    DIAM A = task.A;
    DM radA = rad(A);
    DIAM E(A.getRows(), A.getCols(), DI(-1, 1));

    Interval eI = {
            std::abs(mx.getY()) / sum(mx.getX()), min(radA)
    };
    flogger.log("e in:", eI);
    double e = eI.getUp();
    flogger.log("e =", e);
    E = E * e;
    innerMinus(A, E);
    Task modifiedTask(A, task.b, task.bounds, task.id, task.n);

    tol_helper(modifiedTask, suffix_name, true);
    check_is_empty_tolerance_set(modifiedTask, suffix_name);

    flogger.log("A", modifiedTask.A);
    flogger.log("b", modifiedTask.b);

    flogger.level--;
    flogger.log_framed("correct_a", "end");
}

void correct_ab(const Task &task, const std::string &suffix_name, const DP &mx1) {
    flogger.log_framed("correct_ab", "start");
    flogger.level++;

    flogger.log("max:", mx1);

    //from A
    DIAM A = task.A;
    //
    DM radA = rad(A);
    DIAM E(A.getRows(), A.getCols(), DI(-1, 1));

    Interval eI = {
            std::abs(mx1.getY()) / sum(mx1.getX()), min(radA)
    };
    flogger.log("e in:", eI);
    double e = eI.getUp();
    flogger.log("e =", e);
    E = E * e;
    innerMinus(A, E);

    Task modifiedATask(A, task.b, task.bounds, task.id, task.n);
    DP mx2 = tol_helper(modifiedATask, "ab_cor", false);

    //from b
    DIAV b = task.b;
    //
    DIAV ext(task.b.getDim(), DI(-1, 1));
    double C = mx2.getY();
    flogger.log("C coef for [-1, 1]:", C);
    ext = ext * C;
    b = b - ext;
    //

    flogger.log(mx2);

    //
    Task modifiedAbTask(A, b, task.bounds, task.id, task.n);


    tol_helper(modifiedAbTask, suffix_name, true);
    check_is_empty_tolerance_set(modifiedAbTask, suffix_name);

    flogger.log("A", modifiedAbTask.A);
    flogger.log("b", modifiedAbTask.b);

    flogger.level--;
    flogger.log_framed("correct_ab", "end");
}

DP tol_helper(const Task &task, const std::string &suffix_name, bool positiveTol) {
    flogger.log("tol_helper", "start");
    flogger.log("tol_helper", suffix_name);
    flogger.level++;

    auto bounds = task.bounds;
    DGrid grid(bounds);
    auto my_function = [&](const AVector<double> &x) -> double {
        if (positiveTol)
            return std::max(0.0, tol(x, task.A, task.b));
        else
            return tol(x, task.A, task.b);
    };
    DGridResult result;
    result = evaluate_grid(grid, my_function, task.n);
    result.saveFile("../data/" + std::to_string(task.id) + "/" + suffix_name + "_tolF_val" + ".txt");

    // findGen Tol todo

    //find argMax Tol A1
    DP mx = result.getMax();
    flogger.log("max: ", mx);
    std::ofstream max_val_file("../data/" + std::to_string(task.id) + "/" + suffix_name + "_tolF_max" + ".txt");
    max_val_file << mx << std::endl;

    flogger.level--;
    flogger.log("tol_helper", "end");
    return mx;
}

DGridResult check_grid(const Task &task) {
    flogger.log("check_grid", "start");
    flogger.level++;

    auto bounds = task.bounds;
    DGrid grid(bounds);
    auto my_function = [&](const AVector<double> &x) -> double {
        return tol(x, task.A, task.b);
    };
    DGridResult result;
    result = evaluate_grid(grid, my_function, task.n);

    flogger.level--;
    flogger.log("check_grid", "end");
    return result;
}
