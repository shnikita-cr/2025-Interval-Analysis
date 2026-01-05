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

bool check_grid(const Task &task, DGridResult &result) {
    // Создание 2D сетки
    std::vector<Interval<double>> bounds = {
            Interval<double>(0.0, 1.0),  // X ∈ [0, 1]
            Interval<double>(0.0, 2.0)   // Y ∈ [0, 2]
    };

    DGrid grid(bounds);

    // Функция для вычисления
    auto my_function = [&](const AVector<double> &x) {
        return tol(x, task.A, task.b);
    };

    // Вычисление на сетке 101x201
    result = evaluate_grid(grid, my_function);

    // Сохранение в файл

    result.saveFile("../../data/results.txt");

    double mx = result.getMax().getX(),
            my = result.getMax().getY();

    std::cout << "maximum:"
              << "x: " << mx
              << "y: " << my << std::endl;

    return my > 0;
}