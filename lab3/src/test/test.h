#pragma once

#include "../task/task.h"
#include "../calc/calc.h"

void tol_helper(const Task &task, const std::string &suffix_name) {
    find_argmax_tol(task, suffix_name);
    find_gen_tol(task, suffix_name);
    graph_tol(task, suffix_name);
}

void test_task(const Task &task, int i) {
    std::cout << "================" << " task: " << i << " start ============" << std::endl;
    // A
    bool is_empty = check_is_empty_tolerance_set(task, std::to_string(i));
    if (!is_empty) {
        tol_helper(task, "before_cor_task_" + std::to_string(i));
    } else {
        correct_b(task, std::to_string(i));
        correct_a(task, std::to_string(i));
        correct_ab(task, std::to_string(i));
    }
    std::cout << "================" << " task: " << i << " end   ============" << std::endl;
}

bool check_grid(const Task &task) {
    // Создание 2D сетки
    std::vector<Interval<double>> bounds = {
            Interval<double>(0.0, 1.0),  // X ∈ [0, 1]
            Interval<double>(0.0, 2.0)   // Y ∈ [0, 2]
    };

    DGrid grid(bounds);


    // Функция для вычисления
    auto my_function = [&](const AVector<double> &x) -> double {
        return x.norm(1);
//        return tol(x, task.A, task.b);
    };

    DGridResult result;

    result = evaluate_grid(grid, my_function, 2);

    result.saveFile("../data/results.txt");

    AVector<double> mx = result.getMax().getX();
    double my = result.getMax().getY();

    std::cout << "maximum:"
              << "x: " << mx
              << "y: " << my << std::endl;

    return my > 0;
}