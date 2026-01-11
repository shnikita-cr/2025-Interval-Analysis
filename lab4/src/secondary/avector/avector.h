#ifndef NUMERICAL_METHODS_AVECTOR_H
#define NUMERICAL_METHODS_AVECTOR_H

#include <vector>
#include <string>
#include <iomanip>
#include <utility>

#define AVECTOR_VERBOSE 0

const double AVECTOR_EPS = 10e-10;
const int AVECTOR_PRECISION = 5, AVECTOR_MAX_ELEMENT_WIDTH = AVECTOR_PRECISION + 5;
const long int AVECTOR_RAND_END = 100, AVECTOR_RAND_START = 0;

template<typename T> class AVector {
private:
    std::vector<T> values;
    const std::size_t dim;
    std::string name;
public:
    static AVector<T> makeE(std::size_t dim, std::size_t idx);
    static AVector<T> makeRand(std::size_t dim);

    explicit AVector(std::size_t _dim);
    AVector(std::size_t _dim, T _initial);
    AVector(std::size_t _dim, T _initial, std::string _name);
    AVector(std::size_t _dim, std::string _name);
    AVector(std::initializer_list<T> _list);
    AVector(const std::vector<T>& other);
    T &operator[](size_t i);
    const T &operator[](size_t i) const;
    AVector<T> &operator=(const AVector<T>& other);
    virtual ~AVector() = default;
    bool operator==(const AVector<T> &other) const;

    //info
    std::size_t getDim() const { return dim; }
    T norm(long int p = 2) const;

    template<class K>
    bool isSameDimensions(const AVector<K> &other) const {return (dim == other.getDim());}

    //io
    friend void operator>>(std::istream &in, AVector<T> &av) {
        for (std::size_t i = 0; i < av.dim; i++) {
            in >> av[i];
        }
    }
    friend std::ostream &operator<<(std::ostream &out, const AVector<T> &av) {
        if (AVECTOR_VERBOSE)
            out << "vector " << av.name << ":" << std::endl;
        for (std::size_t i = 0; i < av.dim; i++) {
            if (AVECTOR_VERBOSE) {
                out << "V[" << i << "] ";
            }
            out.width(AVECTOR_MAX_ELEMENT_WIDTH);
            out.precision(AVECTOR_PRECISION);
            out << std::fixed << av.values[i] << "\t";
        }
//        out<<std::endl;
        return out;
    }

    AVector<T> &normalize(long int p = 2);

    // v */ k
    AVector<T> operator*(T k) const;
    AVector<T> &operator*=(T k);
    AVector<T> operator/(T k) const;
    AVector<T> &operator/=(T k);

    //k * v
    template<typename K>
    friend AVector<K> operator*(K k, AVector<K>& v);

    AVector<T> operator+(T other) const;

    //u+v
    AVector<T> operator+(AVector<T> &other) const;
    AVector<T> &operator+=(AVector<T> &other);
    AVector<T> operator-(AVector<T> &other) const;
    AVector<T> &operator-=(AVector<T> &other);

    // (u,v)
    T operator,(const AVector<T> &other) const;

    template<class V>
    T operator,(const AVector<V> &other) const;

    //cast
    explicit operator std::vector<T>() const{
        return values;
    }

    void apply(const std::unary_function<T,T> &f);
    void agg(const std::unary_function<T,std::vector<T>>& f);
};

#include "avector.cpp"

#endif //NUMERICAL_METHODS_AVECTOR_H
