#pragma once


#include <ostream>

template<typename... Args>
void printAll(Args... args) {
    (void) std::initializer_list<int>{((std::cout << args << " "), 0)...};
    std::cout << std::endl;
}

class DGrid {
private:
    std::vector<Interval<double>> bounds;
public:
    const std::vector<Interval<double>> &getBounds() const {
        return bounds;
    }

public:
    DGrid(const std::vector<Interval<double>> &bounds) : bounds(bounds) {}
};

template<typename Coord, typename Value>
class Point {
    Coord x;
    Value y;
public:
    Point(Coord _x, Value _y) : x(_x), y(_y) {}

    Coord getX() const {
        return x;
    }

    void setX(Coord _x) {
        Point::x = _x;
    }

    Value getY() const {
        return y;
    }

    void setY(Value _y) {
        Point::y = _y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        os << point.x << " " << point.y;
        return os;
    }
};

struct DGridResult {
    std::vector<Point<AVector<double>, double>> points;

    friend std::ostream &operator<<(std::ostream &os, const DGridResult &result) {
        for (const auto &point: result.points) {
            os << point << std::endl;
        }
        return os;
    }

    void saveFile(const std::string &fname) {
        std::ofstream of(fname);
        of << *this << std::endl;
    }

    Point<AVector<double>, double> getMax() {
        auto max = std::max_element(points.begin(), points.end(),
                                    [](const Point<AVector<double>, double> &a,
                                       const Point<AVector<double>, double> &b) -> bool {
                                        return a.getY() < b.getY();
                                    });

        if (max != points.end()) {
            return *max;
        }
        throw std::runtime_error("DGridResult empty in getMax()");
    }
};


DGridResult
evaluate_grid(const DGrid &grid, const std::function<double(AVector<double>)> &func, size_t points_per_dim = 2) {
    const auto &bounds = grid.getBounds();
    size_t n = bounds.size();

    if (n == 0) {
        throw std::invalid_argument("Grid must have at least one dimension");
    }

    if (points_per_dim < 2) {
        throw std::invalid_argument("At least 2 points per dimension required");
    }

    // Вычисляем общее количество точек в сетке
    size_t total_points = 1;
    for (size_t i = 0; i < n; ++i) {
        total_points *= points_per_dim;
    }

    // Подготавливаем массивы координат для каждого измерения
    std::vector<std::vector<double>> dim_coords(n);
    for (int dim = 0; dim < n; ++dim) {
        double down = bounds[dim].getDown();
        double up = bounds[dim].getUp();
        double step = (up - down) / (points_per_dim - 1);

        dim_coords[dim].resize(points_per_dim);
        for (int i = 0; i < points_per_dim; ++i) {
            dim_coords[dim][i] = down + i * step;
        }
    }

    // Подготавливаем результат
    DGridResult result;
    result.points.reserve(total_points);

    // Вектор индексов для текущей точки (инициализируем нулями)
    std::vector<int> indices(n, 0);

    // Генерируем все точки сетки
    for (size_t point_idx = 0; point_idx < total_points; ++point_idx) {
        // Создаем AVector с координатами текущей точки
        AVector<double> point(n);
        for (int dim = 0; dim < n; ++dim) {
            // Используем operator[] для доступа к элементам AVector
            point[dim] = dim_coords[dim][indices[dim]];
        }

        // Вычисляем значение функции в точке
        double value = func(point);

        // Добавляем точку в результат
        result.points.emplace_back(point, value);

        // Переходим к следующей комбинации индексов
        // Увеличиваем индексы как число в смешанной системе счисления
        for (int dim = n - 1; dim >= 0; --dim) {
            indices[dim]++;
            if (indices[dim] < points_per_dim) {
                break; // Если в текущем разряде не переполнение, выходим
            }
            indices[dim] = 0; // Сбрасываем текущий разряд и переходим к следующему

            // Если это был последний разряд и мы его сбросили - цикл завершится сам
        }
    }

    return result;
}
