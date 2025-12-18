#pragma once



template<typename... Args>
void printAll(Args... args) {
    (void) std::initializer_list<int>{((std::cout << args << " "), 0)...};
    std::cout << std::endl;
}

// Функция для создания 2D сетки на основе Task
Grid createGridFromTask(const Task& task, const Interval<double>& x_range,
                        const Interval<double>& y_range) {
    std::vector<Interval<double>> bounds = {x_range, y_range};
    return Grid(bounds);
}

// Основная функция вычисления tol на сетке для Task
TolGridResult evaluateTaskOnGrid(
        const Task& task,
        const Interval<double>& x_range,
        const Interval<double>& y_range,
        bool compute_components = false,
        bool show_progress = true
) {
    // Создаем сетку
    Grid grid = createGridFromTask(task, x_range, y_range);

    // Вычисляем tol на сетке
    return evaluate_tol_on_grid(
            grid,
            task.getGridPointsX(),
            task.getGridPointsY(),
            TolParameters(task.As, task.bs),
            show_progress,
            compute_components
    );
}