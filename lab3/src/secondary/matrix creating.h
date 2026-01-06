#pragma once

#include "amatrix/amatrix.h"
#include "avector/avector.h"

AMatrix<double> matrix_make_ortho(std::size_t dim) {
    //E
    AMatrix<double> e = AMatrix<double>::makeE(dim, dim);
//    std::cout << e;

    //W
    AMatrix<double> w = AMatrix<double>::makeRand(dim, 1);
//    std::cout << w;

    //wT
    AMatrix<double> wt(1, dim, "wT");
    wt = w.getT();
//    std::cout << wt;

    //wwT
    AMatrix<double> wwt(dim, dim, "w*wT");
    wwt = w * wt;
    //std::cout << wwt << std::endl;

    AMatrix<double> q(dim, dim, "Q");
    wwt = wwt * (2 / pow(w.norm(2), 2));
    q = e - wwt;
    //std::cout << q.isSymmetric() << std::endl;

    return q;
}

AMatrix<double> matrix_make_triangle_l(std::size_t dim, double cond) {
    AMatrix<double> b(dim, dim);
    int k = 0;
    for (std::size_t row = 0; row < b.getRows(); row++) {
        for (std::size_t column = 0; column < b.getCols(); column++) {
            if (row == column) {
                if (k == 0) {
                    b[row][column] = cond;
                } else
                    b[row][column] = 1;
                k++;
            }
            if (row > column) {
                b[row][column] = ((row + column) % 2 == 0);
            }
        }
    }
    return b;
}

AMatrix<double> matrix_make_diag_d(std::size_t dim, double det_element, double det_divisor, std::size_t n) {
    AMatrix<double> d(dim, dim, 0, "diagonal");
    for (std::size_t row = 0; row < d.getRows(); row++) {
        d[row][row] = det_element;
        //todo try catch n>dim
        if (n == -1)
            continue;
        else if (row < n)
            d[row][row] /= det_divisor;
    }
    return d;
}

AMatrix<double> matrix_make_cond(std::size_t dim, double cond) {
    AMatrix<double> q = matrix_make_ortho(dim),
            b = matrix_make_triangle_l(dim, cond);
    //b make diag+;
    return (q.getT() * b) * q;
}

AMatrix<double> matrix_make_det(std::size_t dim, double det_element, int det_divisor, std::size_t n) {
    AMatrix<double> q = matrix_make_ortho(dim),
            d = matrix_make_diag_d(dim, det_element, det_divisor, n);
    return (q.getT() * d) * q;
}

AMatrix<double> matrix_make_lambda(std::size_t dim, AVector<double> &lambda_true, AMatrix<double> &q) {
    AMatrix<double> lambdaD(dim, dim, "LAMBDA_TRUE_DMATRIX");
    for (std::size_t i = 0; i < dim; i++) {
        lambdaD[i][i] = lambda_true[i];
    }
    q = matrix_make_ortho(dim);

    return (q.getT() * lambdaD) * q;
}

void matrix_make_ldr(AMatrix<double> &A,
                     AMatrix<double> &L, AMatrix<double> &D, AMatrix<double> &R) {
    L.setName("L");
    D.setName("D");
    R.setName("R");


    std::size_t columns = A.getCols(),
            rows = A.getCols();
    //rows i
    //columns j

    //первый шаг
    //std::cout << "m: " << 0 << std::endl;
    D[0][0] = A[0][0];
    //std::cout << "\td i:" << 0 << " j:" << 0 << std::endl;
    for (std::size_t j = 0; j < columns; j++) {
        if (D[0][0] == 0) {
            std::cerr << "matrix getDeterminant == 0! Exit!" << std::endl;
            D[0][0] = 1;
            return;
        }
        R[0][j] = A[0][j] / D[0][0];
        //std::cout << "\tr i:" << 0 << " j:" << j << std::endl;
    }

    for (std::size_t i = 0; i < rows; i++) {
        if (D[0][0] == 0) {
            std::cerr << "matrix getDeterminant == 0! Exit!" << std::endl;
            D[0][0] = 1;
            return;
        }
        L[i][0] = A[i][0] / D[0][0];
        //std::cout << "\tl i:" << i << " j:" << 0 << std::endl;
    }

    //шаг m
    double sum_k;
    for (std::size_t m = 1; m < rows; m++) {
        //std::cout << "m: " << m << std::endl;
        sum_k = 0;
        //D
        for (std::size_t k = 0; k <= m - 1; k++) {
            sum_k += L[m][k] * D[k][k] * R[k][m];
        }
        //std::cout << "\td i:" << m << " j:" << m << std::endl;
        D[m][m] = A[m][m] - sum_k;

        for (std::size_t j = m; j < columns; j++) {
            sum_k = 0;
            //R
            for (std::size_t k = 0; k <= m - 1; k++) {
                sum_k += L[m][k] * D[k][k] * R[k][j];
            }
            //std::cout << "\tr i:" << m << " j:" << m << std::endl;
            if (D[m][m] == 0) {
                std::cerr << "matrix getDeterminant == 0! Exit!" << std::endl;
                D[m][m] = 1;
                return;
            }
            R[m][j] = (A[m][j] - sum_k) / D[m][m];
            //todo проверить деление на ноль для определителя

        }

        for (std::size_t i = m; i < rows; i++) {
            sum_k = 0;
            //L
            for (std::size_t k = 0; k <= m - 1; k++) {
                sum_k += L[i][k] * D[k][k] * R[k][m];
            }
            if (D[m][m] == 0) {
                std::cerr << "matrix getDeterminant == 0! Exit!" << std::endl;
                D[m][m] = 1;
                return;
            }
            L[i][m] = (A[i][m] - sum_k) / D[m][m];
            //std::cout << "\tl i:" << m << " j:" << m << std::endl;
        }
    }
}


AVector<double> matrix_make_slae(AMatrix<double> A, AVector<double> x) {
    return A * x;
}
