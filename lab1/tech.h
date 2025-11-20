#pragma once

#include "interval/interval.h"
#include "secondary/amatrix/amatrix.h"
#include "task/task.h"

// Функция для генерации всех комбинаций размера k из n элементов
std::vector<std::vector<size_t>> generateCombinations(size_t n, size_t k) {
    std::vector<std::vector<size_t>> combinations;
    std::vector<size_t> current;

    std::function<void(size_t, size_t)> generate;
    generate = [&](size_t start, size_t remaining) {
        if (remaining == 0) {
            combinations.push_back(current);
            return;
        }

        for (size_t i = start; i <= n - remaining; ++i) {
            current.push_back(i);
            generate(i + 1, remaining - 1);
            current.pop_back();
        }
    };

    generate(0, k);
    return combinations;
}

// Функция для создания подматрицы по выбранным строкам и столбцам
template<typename T>
AMatrix<T> createSubmatrix(const AMatrix<T> &original,
                           const std::vector<size_t> &selected_rows,
                           const std::vector<size_t> &selected_cols) {
    size_t rows = selected_rows.size();
    size_t cols = selected_cols.size();
    AMatrix<T> sub(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            sub[i][j] = original[selected_rows[i]][selected_cols[j]];
        }
    }
    return sub;
}

// Функция для вычисления всех определителей квадратных подматриц максимального размера
template<typename T>
std::vector<T> calculateAllDeterminants(const AMatrix<T> &matrix) {
    std::vector<T> determinants;

    // Если матрица квадратная, возвращаем один определитель
    if (matrix.getRows() == matrix.getCols()) {
        determinants.push_back(matrix.det());
        return determinants;
    }

    // Для неквадратных матриц находим все определители квадратных подматриц
    size_t min_dim = std::min(matrix.getRows(), matrix.getCols());

    // Генерируем все комбинации строк и столбцов
    auto row_combinations = generateCombinations(matrix.getRows(), min_dim);
    auto col_combinations = generateCombinations(matrix.getCols(), min_dim);

    // Вычисляем определители для всех комбинаций подматриц
    for (const auto &rows: row_combinations) {
        for (const auto &cols: col_combinations) {
            auto submatrix = createSubmatrix(matrix, rows, cols);
            determinants.push_back(submatrix.det());
        }
    }

    return determinants;
}

template<typename Applicator>
auto aggregate_dets(const std::vector<DI> &dets, Applicator f) {
    return f(dets);
}

template<typename Predicate>
bool checkAllDeterminants(double delta, Predicate condition) {
    auto determinants = calculateAllDeterminants(getStartIntervalMatrix(delta));
    if (determinants.empty()) {
        throw std::runtime_error("No determinants found");
    }

    return std::all_of(determinants.begin(), determinants.end(), condition);
}

// Проверка, что все определители содержат ноль
bool allContainZero(double delta) {
    return checkAllDeterminants(delta, [](const DI &det) {
        return det.contains(0);
    });
}