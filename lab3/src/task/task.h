#pragma once

#include "../names.h"

struct Task {
    DIAM A;                    // Интервальная матрица системы
    DIAV b;                    // Интервальный вектор правой части
    size_t n;                  // Количество точек по каждой координате (одинаковое)
    std::vector<Interval<double>> bounds;

    // Конструктор с разрешением сетки
    Task(const DIAM &as, const DIAV &bs, std::vector<Interval<double>> bounds = {}, size_t resolution = 100) : A(as),
                                                                                                               b(bs),
                                                                                                               n(resolution),
                                                                                                               bounds(std::move(
                                                                                                                       bounds)) {
        validate();
    }

    void validate() const {
        if (A.getRows() != b.getDim() && b.getDim() != bounds.size()) {
            throw std::invalid_argument("Matrix rows must equal vector dimension and bounds dimension");
        }
        if (n < 2) {
            throw std::invalid_argument("Grid resolution must be at least 2");
        }
    }

    // Общее количество точек в сетке
    size_t getTotalGridPoints() const {
        return std::size_t(std::pow(n, b.getDim()));
    }

    // Печать информации о задаче
    void printInfo() const {
        std::cout << "Task Info:\n";
        std::cout << "  Matrix A: " << A.getRows() << "x" << A.getCols() << "\n";
        std::cout << "  Vector b: " << b.getDim() << "\n";
        std::cout << "  Grid resolution: " << n << "x" << n << "\n";
        std::cout << "  Total points: " << getTotalGridPoints() << "\n";
    }
};