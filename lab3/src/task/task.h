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
    DIAM As;                    // Интервальная матрица системы
    DIAV bs;                    // Интервальный вектор правой части
    size_t grid_resolution;     // Количество точек по каждой координате (одинаковое)

    // Конструктор с разрешением сетки
    Task(const DIAM &as, const DIAV &bs, size_t resolution = 100)
            : As(as), bs(bs), grid_resolution(resolution) {
        validate();
    }

    // Валидация задачи
    void validate() const {
        if (As.getRows() != bs.getDim()) {
            throw std::invalid_argument("Matrix rows must equal vector dimension");
        }
        if (As.getCols() != 2) {
            throw std::invalid_argument("Matrix must have 2 columns for 2D grid");
        }
        if (grid_resolution < 2) {
            throw std::invalid_argument("Grid resolution must be at least 2");
        }
    }

    // Получить количество точек по X и Y (одинаковое)
    size_t getGridPointsX() const { return grid_resolution; }

    size_t getGridPointsY() const { return grid_resolution; }

    // Общее количество точек в сетке
    size_t getTotalGridPoints() const {
        return grid_resolution * grid_resolution;
    }

    // Печать информации о задаче
    void printInfo() const {
        std::cout << "Task Info:\n";
        std::cout << "  Matrix A: " << As.getRows() << "x" << As.getCols() << "\n";
        std::cout << "  Vector b: " << bs.getDim() << "\n";
        std::cout << "  Grid resolution: " << grid_resolution << "x" << grid_resolution << "\n";
        std::cout << "  Total points: " << getTotalGridPoints() << "\n";
    }
};