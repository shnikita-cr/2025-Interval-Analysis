#pragma once

#include "../task/task.h"
#include "../calc/calc.h"

void tol_helper(const Task &task, const std::string &suffix_name) {
    find_argmax_tol(task, suffix_name);
    find_gen_tol(task, suffix_name);
    graph_tol(task, suffix_name);
}

void test_task(const Task &task, int i) {
    std::cout << "================" << " task: " << i << "start =============";
    // A
    bool is_empty = check_is_empty_tolerance_set(task, std::to_string(i));
    if (!is_empty) {
        tol_helper(task, "before_cor_task_" + std::to_string(i))
    } else {
        correct_b(task, std::to_string(i));
        correct_a(task, std::to_string(i));
        correct_ab(task, std::to_string(i));
    }
    std::cout << "================" << " task: " << i << "end ==============";
}