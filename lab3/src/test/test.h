#pragma once

#include "../task/task.h"
#include "../calc/calc.h"

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

bool check_grid(const Task &task) {
    flogger.log_framed("check_grid", "start");
    flogger.level++;
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

    result = evaluate_grid(grid, my_function, task.n);

    result.saveFile("../data/results.txt");

    AVector<double> mx = result.getMax().getX();
    double my = result.getMax().getY();

    std::cout << "maximum:"
              << "x: " << mx
              << "y: " << my << std::endl;

    flogger.level--;
    flogger.log_framed("check_grid", "end");
    return my > 0;
}