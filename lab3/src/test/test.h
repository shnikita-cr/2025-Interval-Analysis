#pragma once

#include "../task/task.h"
#include "../calc/calc.h"
#include "../secondary/flogger.h"

void test_task(const Task &task) {
    flogger.log_framed("task", task.id, "start");
    flogger.level++;
    // A
    bool is_empty = check_is_empty_tolerance_set(task, "check");
    if (!is_empty) {
        flogger.log("tolerance set is NOT empty");
        tol_helper(task, "no_cor");
    } else {
        //B
        flogger.log("tolerance set IS empty");
        correct_b(task, "correct_b");
        correct_a(task, "correct_a");
        correct_ab(task, "correct_ab");
    }
    flogger.level--;
    flogger.log_framed("task", task.id, "end");
}

