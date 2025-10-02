#pragma once

#include "interval/interval.h"
#include "secondary/amatrix/amatrix.h"

template<typename T>
Interval<T> det(const AMatrix <Interval<T>> &m) {
    std::size_t n = m.getRows();
    if (n != m.getCols())
        throw std::invalid_argument("Matrix must be square");

    if (n == 1)
        return m[0][0];

    if (n == 2)
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];

    Interval<T> total(0.0, 0.0);
    for (std::size_t col = 0; col < n; ++col) {
        // Build submatrix
        AMatrix <Interval<T>> sub(n - 1, n - 1);
        for (std::size_t i = 1; i < n; ++i) {
            std::size_t sub_j = 0;
            for (std::size_t j = 0; j < n; ++j) {
                if (j == col) continue;
                sub[i - 1][sub_j] = m[i][j];
                ++sub_j;
            }
        }

        Interval<T> sign = (col % 2 == 0)
                           ? Interval<T>(1.0, 1.0)
                           : Interval<T>(-1.0, -1.0);

        total = total + sign * m[0][col] * det(sub);
    }
    return total;
}
