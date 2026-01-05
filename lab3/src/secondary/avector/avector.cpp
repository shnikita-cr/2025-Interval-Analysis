#ifndef NUMERICAL_METHODS_AVECTOR_CPP
#define NUMERICAL_METHODS_AVECTOR_CPP

#include "avector.h"
#include <cmath>
#include <algorithm>
#include <iostream>

template<typename T>
AVector<T> AVector<T>::makeE(std::size_t dim, std::size_t idx) {
    AVector<T> e(dim, (T) 0, "e");
    e[idx] = 1;
    return e;
}

template<typename T>
AVector<T> AVector<T>::makeRand(std::size_t dim) {
    AVector<T> r(dim, (T) 0, "RAND");
    for (std::size_t i = 0; i < dim; i++) {
        r[i] = rand() % AVECTOR_RAND_END + AVECTOR_RAND_START;
    }
    return r;
}

template<typename T>
AVector<T>::AVector(std::size_t _dim):dim(_dim), name("untitled"), values(std::vector<T>(_dim, 0)) {
}

template<typename T>
AVector<T>::AVector(std::size_t _dim, T _initial):dim(_dim), name("untitled"), values(std::vector<T>(_dim, _initial)) {
}

template<typename T>
AVector<T>::AVector(std::size_t _dim, T _initial, std::string _name):dim(_dim), name(std::move(_name)),
                                                                     values(std::vector<T>(_dim, _initial)) {}

template<typename T>
AVector<T>::AVector(std::size_t _dim, std::string _name) :dim(_dim), name(std::move(_name)),
                                                          values(std::vector<T>(_dim, 0)
                                                          ) {}

template<typename T>
AVector<T>::AVector(std::initializer_list<T> _list):dim(_list.size()) {
    values = _list;
}

template<typename T>
AVector<T>::AVector(const std::vector<T> &other) :dim(other.size()) {
    values = other;
}


template<typename T>
T &AVector<T>::operator[](std::size_t i) {
    if (i < dim) {
        return values[i];
    } else throw std::out_of_range("AVector []: wrong idx");
}

template<typename T>
const T &AVector<T>::operator[](std::size_t i) const {
    if (i < dim) {
        return values[i];
    } else throw std::out_of_range("AVector []: wrong idx");
}

template<typename T>
AVector<T> &AVector<T>::operator=(const AVector<T> &other) {
    if (this != &other) {
        if (isSameDimensions(other)) {
            name = name + " from " + other.name;
            for (std::size_t i = 0; i < dim; i++) {
                values[i] = other[i];
            }
        } else {
            { throw std::length_error("AVector u=v: wrong dimensions"); }
        }
    }
    return *this;
}

template<typename T>
bool AVector<T>::operator==(const AVector<T> &other) const {
    if (this == &other)
        return true;
    if (isSameDimensions(other)) {
        for (std::size_t i = 0; i < dim; i++) {
            if (fabs(values[i] - other[i]) > AVECTOR_EPS) {
                return false;
            }
        }
        return true;
    }
    return false;
}

template<typename T>
T AVector<T>::norm(long int p) const {
    if (values.empty()) {
        return 0;
    }
    T d = 0;
    switch (p) {
        case 1:
            for (auto i: values) {
                d += std::abs(i);
            }
            return d;
        case -1:
            return *std::max_element(values.begin(), values.end(),
                                     [](T a, T b) { return std::abs(a) < std::abs(b); });
        case 0:
        default:
            if (p < -1) {
                throw std::invalid_argument("AVector norm: wrong p");
            }
            for (auto i: values) {
                d += std::pow(std::abs(i), p);
            }
            return std::pow(d, 1.0 / p);
    }
}


template<typename T>
AVector<T> &AVector<T>::normalize(long int p) {
    T n = norm(p);
    if (n == 0) {
        throw std::runtime_error("AVector normalize: / 0");
    }
    *this /= n;
    return *this;
}

template<typename T>
AVector<T> AVector<T>::operator*(T k) const {
    AVector av(dim);
    for (std::size_t i = 0; i < dim; i++) {
        av[i] = values[i] * k;
    }
    return av;
}

template<typename T>
AVector<T> &AVector<T>::operator*=(T k) {
    for (std::size_t i = 0; i < dim; i++) {
        values[i] *= k;
    }
    return *this;
}

template<typename T>
AVector<T> AVector<T>::operator/(T k) const {
    if (k != 0) {
        AVector av(dim);
        for (std::size_t i = 0; i < dim; i++) {
            av[i] = values[i] / k;
        }
        return av;
    } else throw std::runtime_error("AVector: / 0");
}

template<typename T>
AVector<T> &AVector<T>::operator/=(T k) {
    if (k != 0) {
        for (std::size_t i = 0; i < dim; i++) {
            values[i] /= k;
        }
    } else throw std::runtime_error("AVector: /= 0");
    return *this;
}

template<typename K>
AVector<K> operator*(K k, AVector<K> &v) {
    for (std::size_t i = 0; i < v.dim; i++) {
        v[i] *= k;
    }
    return v;
}

template<typename T>
AVector<T> AVector<T>::operator+(AVector &other) const {
    if (isSameDimensions(other)) {
        AVector av(dim);
        for (std::size_t i = 0; i < dim; i++) {
            av[i] = values[i] + other[i];
        }
        return av;
    } else throw std::length_error("AVector u+v: wrong dimensions");
}

template<typename T>
AVector<T> &AVector<T>::operator+=(AVector &other) {
    if (isSameDimensions(other)) {
        for (std::size_t i = 0; i < dim; i++) {
            values[i] += other[i];
        }
    } else throw std::length_error("AVector u+=v: wrong dimensions");
    return *this;
}

template<typename T>
AVector<T> AVector<T>::operator-(AVector &other) const {
    if (isSameDimensions(other)) {
        AVector av(dim);
        for (std::size_t i = 0; i < dim; i++) {
            av[i] = values[i] - other[i];
        }
        return av;
    } else throw std::length_error("AVector u-v: wrong dimensions");
}

template<typename T>
AVector<T> &AVector<T>::operator-=(AVector &other) {
    if (isSameDimensions(other)) {
        for (std::size_t i = 0; i < dim; i++) {
            values[i] -= other[i];
        }
    } else throw std::length_error("AVector u-=v: wrong dimensions");
    return *this;
}

template<typename T>
T AVector<T>::operator,(const AVector &other) const {
    if (isSameDimensions(other)) {
        T d = 0;
        for (std::size_t i = 0; i < dim; i++) {
            d += values[i] * other[i];
        }
        return d;
    } else throw std::length_error("AVector (u,v): wrong dimensions");
}

template<typename T>
template<typename V>
T AVector<T>::operator,(const AVector<V> &other) const {
    if (isSameDimensions(other)) {
        T d = 0;
        for (std::size_t i = 0; i < dim; i++) {
            d += values[i] * other[i];
        }
        return d;
    } else throw std::length_error("AVector (u,v): wrong dimensions");
}

#endif
