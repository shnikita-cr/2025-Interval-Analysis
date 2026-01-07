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
    flogger.log_framed("check_is_empty_tolerance_set", "start");
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
    flogger.log_framed("check_is_empty_tolerance_set", "end");
    return maxPoint.getY() == 0;
}

void correct_b(const Task &task, const std::string &suffix_name, double mx) {
    flogger.log_framed("correct_b", "start");
    flogger.level++;

    DIAV ext(task.b.getDim(), DI(-1, 1));
    DIAV b = task.b;
    ext = ext * (mx + 2);
    b = b - ext;
    Task modifiedTask(task.A, b, task.bounds, task.id, task.n);

    tol_helper(modifiedTask, suffix_name, false);

    flogger.level--;
    flogger.log_framed("correct_b", "end");
}

void correct_a(const Task &task, const std::string &suffix_name, double mx) {
    flogger.log_framed("correct_a", "start");
    flogger.level++;


    tol_helper(task, suffix_name, true);

    flogger.level--;
    flogger.log_framed("correct_a", "end");
}

void correct_ab(const Task &task, const std::string &suffix_name, double mx) {
    flogger.log_framed("correct_ab", "start");
    flogger.level++;


    tol_helper(task, suffix_name, true);

    flogger.level--;
    flogger.log_framed("correct_ab", "end");
}

double tol_helper(const Task &task, const std::string &suffix_name, bool positiveTol) {
    flogger.log_framed("tol_helper", "start");
    flogger.log_framed("tol_helper", suffix_name);
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
    auto mx = result.getMax();
    flogger.log("max: ", mx);
    std::ofstream max_val_file("../data/" + std::to_string(task.id) + "/" + suffix_name + "_tolF_max" + ".txt");
    max_val_file << mx << std::endl;

    flogger.level--;
    flogger.log_framed("tol_helper", "end");
    return mx.getY();
}

DGridResult check_grid(const Task &task) {
    flogger.log_framed("check_grid", "start");
    flogger.level++;

    auto bounds = task.bounds;
    DGrid grid(bounds);
    auto my_function = [&](const AVector<double> &x) -> double {
        return tol(x, task.A, task.b);
    };
    DGridResult result;
    result = evaluate_grid(grid, my_function, task.n);

    flogger.level--;
    flogger.log_framed("check_grid", "end");
    return result;
}
