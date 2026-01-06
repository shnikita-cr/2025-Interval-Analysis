#include "calc.h"
#include "../secondary/tech/print_all.h"
#include "../secondary/flogger.h"
#include "../secondary/tech/dgrid.h"
#include "../secondary/tech/evaluate_grid.h"
#include "../names.h"

double tol(const AVector<double> &x, const DIAM &A, const DIAV &b) {
    if (!(A.getRows() == b.getDim() && A.getCols() == x.getDim())) {
        printAll("Wrong shape for tol(x,A,B)!");
        return -1;
    }
    size_t m = b.getDim();
    double min = 0;
    for (size_t i = 0; i < m; i++) {
        auto val = b[i].getRad() - abs(b[i].getMid() - (A[i], x));
        if (val < min) {
            min = val;
        }
    }
    return min;
}

bool check_is_empty_tolerance_set(const Task &task, const std::string &suffix_name) {
    return true;
}

void correct_b(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("correct_b", "start");
    flogger.level++;
    tol_helper(task, "after_cor_task_b " + suffix_name);
    flogger.level--;
    flogger.log_framed("correct_b", "end");
}

void correct_a(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("correct_a", "start");
    flogger.level++;
    tol_helper(task, "after_cor_task_a " + suffix_name);
    flogger.level--;
    flogger.log_framed("correct_a", "end");
}

void correct_ab(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("correct_ab", "start");
    flogger.level++;
    tol_helper(task, "after_cor_task_ab " + suffix_name);
    flogger.level--;
    flogger.log_framed("correct_ab", "end");
}

void graph_tol(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("graph_tol", "start");
    flogger.level++;
    flogger.level--;
    flogger.log_framed("graph_tol", "end");
}

void graph_tol_task(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("graph_tol_task", "start");
    flogger.level++;
    flogger.level--;
    flogger.log_framed("graph_tol_task", "end");
}

void find_argmax_tol(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("find_argmax_tol", "start");
    flogger.level++;

    auto result = check_grid(task);

    flogger.level--;
    flogger.log_framed("find_argmax_tol", "end");
}

void find_gen_tol(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("find_gen_tol", "start");
    flogger.level++;
    flogger.level--;
    flogger.log_framed("find_gen_tol", "end");
}

void tol_helper(const Task &task, const std::string &suffix_name) {
    flogger.log_framed("tol_helper", "start");
    flogger.log_framed("tol_helper", suffix_name);
    flogger.level++;
    find_argmax_tol(task, suffix_name);
    find_gen_tol(task, suffix_name);
    graph_tol(task, suffix_name);
    graph_tol_task(task, suffix_name);
    flogger.level--;
    flogger.log_framed("tol_helper", "end");
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
