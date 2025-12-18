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

int check_grid() {
    // Создание 2D сетки
    std::vector<Interval<double>> bounds = {
            Interval<double>(0.0, 1.0),  // X ∈ [0, 1]
            Interval<double>(0.0, 2.0)   // Y ∈ [0, 2]
    };

    Grid grid(bounds);

    // Функция для вычисления
    auto my_function = [](double x, double y) {
        return sin(x) * cos(y) * exp(-(x*x + y*y)/2.0);
    };

    // Вычисление на сетке 101x201
    GridResult2D result = evaluate_grid_2d(grid, 101, 201, my_function, true);

    // Сохранение в файл
    result.save_to_file("grid_results.txt", true);

    // Вывод статистики
    auto stats = result.compute_statistics();
    std::cout << "\nStatistics:\n";
    std::cout << "  Min value: " << stats.min_value << "\n";
    std::cout << "  Max value: " << stats.max_value << "\n";
    std::cout << "  Mean:      " << stats.mean << "\n";
    std::cout << "  Std dev:   " << stats.std_dev << "\n";

    // Поиск точки с максимальным значением
    auto max_points = find_extrema(result, false, 3);
    std::cout << "\nTop 3 maximum points:\n";
    for (size_t i = 0; i < max_points.size(); i++) {
        const auto& p = max_points[i];
        std::cout << "  " << i+1 << ": (" << p.x << ", " << p.y
                  << ") = " << p.value << "\n";
    }

    return 0;
}