#pragma once

#include "../task/task.h"
#include "../calc/calc.h"
#include "../secondary/flogger.h"

void test_task(const Task &task) {
    flogger.log_framed("task", task.id, "start");
    flogger.level++;
    // A
    bool is_empty = check_is_empty_tolerance_set(task, "_init");
    if (!is_empty) {
        flogger.log("tolerance set is NOT empty");
        tol_helper(task, "no_cor");
    } else {
        //B
        DP mx = tol_helper(task, "before_cor");
        flogger.log("tolerance set IS empty");
        correct_b(task, "cb_", mx);
        correct_a(task, "cA_", mx);
        correct_ab(task, "cAb", mx);
    }
    flogger.level--;
    flogger.log_framed("task", task.id, "end");
}

