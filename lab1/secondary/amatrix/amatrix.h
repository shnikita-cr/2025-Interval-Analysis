#ifndef NUMERICAL_METHODS_AMATRIX_H
#define NUMERICAL_METHODS_AMATRIX_H

#include <vector>
#include <string>
#include <iomanip>
#include <utility>
#include "../avector/avector.h"
#include "../../interval/interval.h"

#include "../../main.h"
#define AMATRIX_VERBOSE 0

using AMatrixDoubleType = Interval<double>;

const AMatrixDoubleType AMATRIX_EPS = AMatrixDoubleType(10e-10, 10e-10);
const int AMATRIX_PRECISION = PRECISION, AMATRIX_MAX_ELEMENT_WIDTH = AMATRIX_PRECISION + 4;
const long int AMATRIX_RAND_END = 100, AMATRIX_RAND_START = 0;

template<typename T>
class AMatrix {
private:
    std::vector<AVector<T>> mat;
    std::string name;

private:
    const std::size_t rows;
    const std::size_t cols;

    T norm1() const;

    T norm2() const;

    T normInf() const;

    T condD() const;

    AMatrix<T> getMatr(size_t i, size_t j) const;

public:
    static AMatrix<T> makeE(std::size_t _rows, std::size_t _cols);

    static AMatrix<T> makeRand(std::size_t _rows, std::size_t _cols);

    static AMatrix<T> makeHilb(std::size_t _rows, std::size_t _cols);

    AMatrix(std::size_t _rows, std::size_t _cols);

    AMatrix(std::size_t _rows, std::size_t _cols, T _initial);

    AMatrix(std::size_t _rows, std::size_t _cols, std::string _name);

    AMatrix(std::size_t _rows, std::size_t _cols, T _initial, std::string _name);

    AMatrix(std::initializer_list<AVector<T>> _list);

    AMatrix(const std::vector<std::vector<T>> &other);

    AVector<T> &operator[](size_t idx);

    const AVector<T> &operator[](size_t idx) const;

    AMatrix<T> &operator=(const AMatrix<T> &right);

    bool operator==(AMatrix<T> other);

    virtual ~AMatrix() = default;

    void setName(const std::string &_name) {
        AMatrix::name = _name;
    }

    //info
    std::size_t getRows() const { return rows; }

    std::size_t getCols() const { return cols; }

    AMatrixDoubleType norm(long int p = 2) const;

    AMatrixDoubleType det() const;

    AMatrixDoubleType cond(long int p = 2) const;

    T maxComponent();

    T minComponent();

    bool isSquare() const { return rows == cols; }

    bool isDiag() const;

    bool isSym() const;

    bool isVector() const { return (rows == 1) || (cols == 1); }

    bool isSameDimensions(const AMatrix<T> &right) const {
        return (cols == right.getCols()) && (rows == right.getRows());
    }

    //io
    friend std::ostream &operator<<(std::ostream &out, AMatrix<T> mx) {
        out << "matrix " << mx.name << ":" << std::endl;
        for (std::size_t row = 0; row < mx.rows; row++) {
            for (std::size_t column = 0; column < mx.cols; column++) {
                if (AMATRIX_VERBOSE) {
                    out << "M[" << row << "," << column << "] ";
                }
                out.width(AMATRIX_MAX_ELEMENT_WIDTH);
                out.precision(AMATRIX_PRECISION);
                out << std::fixed << mx[row][column];
                if (column + 1 == mx.cols) {
                    out << std::endl;
                } else {
                    out << " ";
                }
            }
        }
        return out;
    }

    friend void operator>>(std::istream &in, AMatrix<T> &mx) {
        for (std::size_t row = 0; row < mx.rows; row++) {
            for (std::size_t column = 0; column < mx.cols; column++) {
                in >> mx.mat[row][column];
            }
        }
    }

    AMatrix<T> &normalize(long int p);

    AMatrix<T> getT() const;

    AVector<T> getDiagVec() const;

    //Arithmetic matrix-matrix operations
    AMatrix<T> operator+(const AMatrix<T> &right) const;

    AMatrix<T> &operator+=(const AMatrix<T> &right);

    AMatrix<T> operator-(const AMatrix<T> &right) const;

    AMatrix<T> &operator-=(const AMatrix<T> &right);

    //Arithmetic scalar-matrix operations
    AMatrix<T> operator*(T k) const;

    AMatrix<T> &operator*=(T k);

    AMatrix<T> operator/(T k) const;

    AMatrix<T> &operator/=(T k);

    template<typename K>
    friend AMatrix<K> operator*(K k, const AMatrix<K> &right);

    //multiplication
    AVector<T> operator*(const AVector<T> &right);

    AMatrix<T> operator*(const AMatrix<T> &right);
};

#include "amatrix.cpp"


#endif //NUMERICAL_METHODS_AMATRIX_H
