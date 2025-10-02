#ifndef NUMERICAL_METHODS_AMATRIX_CPP
#define NUMERICAL_METHODS_AMATRIX_CPP

#include "amatrix.h"
#include <limits>
#include <utility>

template<typename T>
bool isSameDimensions(AMatrix<T> left, AMatrix<T> right) {
    if ((left.getCols() == right.getCols()) && (left.getRows() == right.getRows()))
        return true;
    return false;
}

template<typename T>
T AMatrix<T>::norm1() const {
    T max_sum = 0, local_sum;
    for (std::size_t column = 0; column < cols; column++) {
        local_sum = 0;
        for (std::size_t row = 0; row < rows; row++) {
            local_sum += fabs(mat[row][column]);
        }
        if (local_sum > max_sum)
            max_sum = local_sum;
    }
    return max_sum;
}

template<typename T>
T AMatrix<T>::norm2() const {
    //Frobenius
    T res = 0;
    for (std::size_t row = 0; row < rows; row++) {
        for (std::size_t column = 0; column < cols; column++) {
            res += pow(fabs(mat[row][column]), 2);
        }
    }
    return pow(res, 0.5);
}

template<typename T>
T AMatrix<T>::normInf() const {
    T max_sum = 0, local_sum;
    for (std::size_t row = 0; row < rows; row++) {
        local_sum = 0;
        for (std::size_t column = 0; column < cols; column++) {
            local_sum += fabs(mat[row][column]);
        }
        if (local_sum > max_sum) {
            max_sum = local_sum;
        }
    }
    return max_sum;
}

template<typename T>
T AMatrix<T>::condD() const {
    T max = 0, min = std::numeric_limits<T>::max();
    for (std::size_t i = 0; i < rows; i++) {
        if (mat[i][i] > min) {
            min = mat[i][i];
        }
        if (mat[i][i] < max) {
            max = mat[i][i];
        }
    }
    if (fabs(min) != 0) {
        T condD = fabs(max) / fabs(min);
        return condD;
    } else throw std::runtime_error("AMatrix condD: / 0");
}

template<typename T>
AMatrix<T> AMatrix<T>::getMatr(size_t i, size_t j) const {
    size_t di = 0, dj = 0;
    AMatrix<T> res(rows - 1, cols - 1);
    for (size_t ki = 0; ki < rows - 1; ki++) {
        if (ki == i)
            di = 1;
        dj = 0;
        for (size_t kj = 0; kj < rows - 1; kj++) {
            if (kj == j)
                dj = 1;
            res[ki][kj] = mat[ki + di][kj + dj];
        }
    }
    return res;
};

template<typename T>
AMatrix<T> AMatrix<T>::makeE(std::size_t _rows, std::size_t _cols) {
    AMatrix<T> mat(_rows, _cols, "E");
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
            if (i == j)
                mat[i][j] = 1;
            else
                mat[i][j] = 0;
        }
    }
    return mat;
}

template<typename T>
AMatrix<T> AMatrix<T>::makeRand(std::size_t _rows, std::size_t _cols) {
    AMatrix<T> mat(_rows, _cols, "RAND");
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
            mat[i][j] = rand() % (AMATRIX_RAND_END - AMATRIX_RAND_START + 1) + AMATRIX_RAND_START;
        }
    }
    return mat;
}

template<typename T>
AMatrix<T> AMatrix<T>::makeHilb(std::size_t _rows, std::size_t _cols) {
    AMatrix<T> mat(_rows, _cols, "HILB");
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
            mat[i][j] = 1.0 / (j + i + 1);
        }
    }
    return mat;
}

template<typename T>
AMatrix<T>::AMatrix(std::size_t _rows, std::size_t _cols, T _initial, std::string _name):rows(_rows), cols(_cols),
                                                                                         name(std::move(_name)) {
    mat = std::vector<AVector<T>>(rows, AVector<T>(cols,
                                                   _initial));
}

template<typename T>
AMatrix<T>::AMatrix(std::size_t _rows, std::size_t _cols, T _initial):AMatrix(_rows, _cols, _initial, "untitled") {}

template<typename T>
AMatrix<T>::AMatrix(std::size_t _rows, std::size_t _cols, std::string _name):AMatrix(_rows, _cols, (T) 0,
                                                                                     _name) {}

template<typename T>
AMatrix<T>::AMatrix(std::size_t _rows, std::size_t _cols) :AMatrix(_rows, _cols, (T) 0, "untitled") {}

template<typename T>
AMatrix<T>::AMatrix(std::initializer_list<AVector<T>> _list)
        : mat(_list), rows(_list.size()), cols(_list.size() > 0 ? _list.begin()->getDim() : 0) {
    for (const auto &row: mat) {
        if (row.getDim() != cols) {
            throw std::invalid_argument("AMatrix {{}}: All rows must have the same number of columns");
        }
    }
}

template<typename T>
AMatrix<T>::AMatrix(const std::vector<std::vector<T>> &other)
        : rows(other.size()), cols(!other.empty() ? other[0].size() : 0) {
    mat.reserve(rows);
    for (const auto &row: other) {
        if (row.size() != cols) {
            throw std::invalid_argument("AMatrix vector{{}}: All rows must have the same number of columns");
        }
        mat.emplace_back(row);
    }
}


template<typename T>
AVector<T> &AMatrix<T>::operator[](size_t idx) {
    if (idx < rows) {
        return mat[idx];
    } else throw std::out_of_range("AMatrix []: wrong idx");
}

template<typename T>
const AVector<T> &AMatrix<T>::operator[](size_t idx) const {
    if (idx < rows) {
        return mat[idx];
    } else throw std::out_of_range("AMatrix []: wrong idx");
}

template<typename T>
bool AMatrix<T>::operator==(AMatrix<T> other) {
    if (isSameDimensions(other)) {
        for (std::size_t _row = 0; _row < rows; _row++) {
            for (std::size_t _col = 0; _col < cols; _col++) {
                if (fabs(mat[_row][_col] - other[_row][_col]) > AMATRIX_EPS)
                    return false;
            }
        }
        return true;
    }
    return false;
}

template<typename T>
AMatrix<T> &AMatrix<T>::operator=(const AMatrix<T> &right) {
    if (this != &right) {
        if (isSameDimensions(right)) {
            name = name + " from " + right.name;
            for (std::size_t row = 0; row < rows; row++) {
                for (std::size_t col = 0; col < cols; col++)
                    mat[row][col] = right[row][col];
            }
        } else { throw std::length_error("AMatrix A=B: wrong dimensions"); }
    }
    return *this;
}

template<typename T>
AMatrixDoubleType AMatrix<T>::norm(long p) const {
    switch (p) {
        case 1:
            return norm1();
        case 2:
            return norm2();
        case -1:
            return normInf();
        default:
            return normInf();
    }
}

template<typename T>
AMatrixDoubleType AMatrix<T>::det() const {
    if (rows != cols || cols == 0)
        throw std::length_error("AMatrix det: wrong dimensions of matrix");
    switch (rows) {
        case 1:
            return static_cast<AMatrixDoubleType>(mat[0][0]);
        case 2:
            return (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]);
        default:
            int k = 1;//(-1)^1
            AMatrixDoubleType result = 0;
            for (int i = 0; i < rows; i++) {
                result += k * mat[i][0] * getMatr(i, 0).det();
                k *= -1;
            }
            return result;
    }
}

template<typename T>
AMatrixDoubleType AMatrix<T>::cond(long int p) const {
    if (isDiag() == 0)
        return pow((*this).norm(p), 2);
    return condD();
}

template<typename T>
T AMatrix<T>::maxComponent() {
    T max_value = 0;
    for (std::size_t col = 0; col < cols; col++) {
        for (std::size_t row = 0; row < rows; row++) {
            if (abs(mat[row][col]) > abs(max_value)) {
                max_value = mat[row][col];
            }
        }
    }
    return max_value;
}

template<typename T>
T AMatrix<T>::minComponent() {
    T max_value = std::numeric_limits<T>::max();
    for (std::size_t col = 0; col < cols; col++) {
        for (std::size_t row = 0; row < rows; row++) {
            if (abs(mat[row][col]) < abs(max_value)) {
                max_value = mat[row][col];
            }
        }
    }
    return max_value;
}

template<typename T>
bool AMatrix<T>::isDiag() const {
    for (std::size_t row = 0; row < rows; row++) {
        for (std::size_t column = 0; column < cols; column++) {
            if (row != column && (mat[row][column] != 0))
                return false;
        }
    }
    return true;
}

template<typename T>
bool AMatrix<T>::isSym() const {
    for (std::size_t i = 0; i < cols; i++) {
        for (std::size_t j = i + 1; j < rows; j++) {
            if (mat[i][j] - mat[j][i] > AMATRIX_EPS)
                return false;
        }
    }
    return true;
}

template<typename T>
AMatrix<T> &AMatrix<T>::normalize(long int p) {
    (*this) /= norm(p);
    return *this;
}

template<typename T>
AMatrix<T> AMatrix<T>::getT() const {
    AMatrix<T> result(cols, rows, (T) 0, "(" + name + " T)");
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            result[j][i] = mat[i][j];
        }
    }
    return result;
}

template<typename T>
AVector<T> AMatrix<T>::getDiagVec() const {
    AVector<T> result(rows, (T) 0);
    for (std::size_t i = 0; i < rows; i++) {
        result[i] = mat[i][i];
    }
    return result;
}

template<typename T>
AMatrix<T> AMatrix<T>::operator+(const AMatrix<T> &right) const {
    if (isSameDimensions(right)) {
        AMatrix result(rows, cols, 0.0);
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                result[i][j] = mat[i][j] + right[i][j];
            }
        }
        return result;
    } else throw std::length_error("AMatrix A+B: wrong dimensions");
}

template<typename T>
AMatrix<T> &AMatrix<T>::operator+=(const AMatrix<T> &right) {
    if (isSameDimensions(right)) {
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                this->mat[i][j] += right[i][j];
            }
        }
        return *this;
    } else throw std::length_error("AMatrix A+=B: wrong dimensions");
}

template<typename T>
AMatrix<T> AMatrix<T>::operator-(const AMatrix<T> &right) const {
    if (isSameDimensions(right)) {
        AMatrix<T> result(rows, cols, 0.0);
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                result[i][j] = mat[i][j] - right[i][j];
            }
        }
        return result;
    } else throw std::length_error("AMatrix A-B: wrong dimensions");
}

template<typename T>
AMatrix<T> &AMatrix<T>::operator-=(const AMatrix<T> &right) {
    if (isSameDimensions(right)) {
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                mat[i][j] -= right[i][j];
            }
        }
    } else throw std::length_error("AMatrix A-=B: wrong dimensions");
    return *this;
}

template<typename T>
AMatrix<T> AMatrix<T>::operator*(const T k) const {
    AMatrix<T> result(rows, cols, 0.0);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            result[i][j] = mat[i][j] * k;
        }
    }
    return result;
}

template<typename T>
AMatrix<T> AMatrix<T>::operator/(const T k) const {
    if (k != 0) {
        AMatrix<T> result(rows, cols, 0.0);
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                result[i][j] = mat[i][j] / k;
            }
        }
        return result;
    }
    throw std::runtime_error("AMatrix / 0");
}

template<typename T>
AMatrix<T> &AMatrix<T>::operator*=(const T k) {
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            mat[i][j] *= k;
        }
    }
    return *this;
}

template<typename T>
AMatrix<T> &AMatrix<T>::operator/=(const T k) {
    if (k != 0) {
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                mat[i][j] /= mat[i][j] / k;
            }
        }
        return *this;
    }
    throw std::runtime_error("AMatrix / 0");
}

template<typename K>
AMatrix<K> operator*(K k, const AMatrix<K> &right) {
    AMatrix<K> result(right.rows, right.cols, 0.0);
    for (std::size_t i = 0; i < right.rows; i++) {
        for (std::size_t j = 0; j < right.cols; j++) {
            result[i][j] = right[i][j] * k;
        }
    }
    return result;
}

template<typename T>
AVector<T> AMatrix<T>::operator*(const AVector<T> &right) {
    if (cols != right.getDim()) {
        throw std::length_error("AMatrix A*v: wrong dimension");
    }
    AVector<T> result(right.getDim(), 0.0);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            result[i] += mat[i][j] * right[j];
        }
    }
    return result;
}

template<typename T>
AMatrix<T> AMatrix<T>::operator*(const AMatrix<T> &right) {
    if (cols != right.rows) {
        throw std::length_error("AMatrix A*B: wrong dimension");
    }
    std::size_t r_cols = right.getCols();
    AMatrix<T> result(rows, r_cols, 0.0);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < r_cols; j++) {
            for (std::size_t k = 0; k < cols; k++) {
                result[i][j] += mat[i][k] * right[k][j];
            }
        }
    }
    return result;
}

#endif