#pragma once

#include "../task/task.h"
#include "../calc/calc.h"
#include "../secondary/flogger.h"

void test_task(const Task &task, int i) {
    flogger.log_framed("task", i, "start");
    flogger.level++;
    // A
    bool is_empty = check_is_empty_tolerance_set(task, std::to_string(i));
    if (!is_empty) {
        flogger.log("tolerance set is NOT empty");
        tol_helper(task, "before_cor_task_" + std::to_string(i));
    } else {
        flogger.log("tolerance set IS empty");
        correct_b(task, std::to_string(i));
        correct_a(task, std::to_string(i));
        correct_ab(task, std::to_string(i));
    }
    flogger.level--;
    flogger.log_framed("task", i, "end");
}

