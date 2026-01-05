#pragma once

#include "../interval/interval.h"
#include "../secondary/amatrix/amatrix.h"
#include <cmath>
#include <limits>

using DI = Interval<double>;

using DIAM = AMatrix<DI>;
using DIAV = AVector<DI>;

DIAM A1{
        {{0.65, 1.25}, {0.70, 1.30}},
        {{0.75, 1.35}, {0.70, 1.30}}
};
DIAV b1{{2.75, 3.15},
        {2.85, 3.25}};


DIAM A2{
        {{0.65, 1.25}, {0.70, 1.30}},
        {{0.75, 1.35}, {0.70, 1.30}},
        {{0.80, 1.40}, {0.70, 1.30}}
};
DIAV b2{{2.75, 3.15},
        {2.85, 3.25},
        {2.90, 3.30}};


DIAM A3{
        {{0.65,  1.25}, {0.70, 1.30}},
        {{0.75,  1.35}, {0.70, 1.30}},
        {{0.80,  1.40}, {0.70, 1.30}},
        {{-0.30, 0.30}, {0.70, 1.30}}
};
DIAV b3{{2.75, 3.15},
        {2.85, 3.25},
        {2.90, 3.30},
        {1.80, 2.20}};


#include <cstddef>

struct Task {
    DIAM A;                    // Интервальная матрица системы
    DIAV b;                    // Интервальный вектор правой части
    size_t n;                  // Количество точек по каждой координате (одинаковое)

    // Конструктор с разрешением сетки
    Task(const DIAM &as, const DIAV &bs, size_t resolution = 100)
            : A(as), b(bs), n(resolution) {
        validate();
    }

    // Валидация задачи
    void validate() const {
        if (A.getRows() != b.getDim()) {
            throw std::invalid_argument("Matrix rows must equal vector dimension");
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