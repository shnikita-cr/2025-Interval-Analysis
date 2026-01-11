#include <vector>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "dgridresult.h"
#include "dgrid.h"

DGridResult evaluate_grid(
        const DGrid &grid,
        const std::function<double(const AVector<double>&)> &func,
        size_t points_per_dim,
        bool show_progress,
        size_t progress_bar_width,
        std::chrono::milliseconds progress_update_period) {
    const auto &bounds = grid.getBounds();
    const size_t n = bounds.size();

    if (n == 0) {
        throw std::invalid_argument("Grid must have at least one dimension");
    }

    if (points_per_dim < 2) {
        throw std::invalid_argument("At least 2 points per dimension required");
    }

    size_t total_points = 1;
    for (size_t i = 0; i < n; ++i) {
        total_points *= points_per_dim;
    }

    std::vector<std::vector<double>> dim_coords(n);
    for (size_t dim = 0; dim < n; ++dim) {
        const double down = bounds[dim].getDown();
        const double up = bounds[dim].getUp();
        const double step = (up - down) / static_cast<double>(points_per_dim - 1);

        dim_coords[dim].resize(points_per_dim);
        for (size_t i = 0; i < points_per_dim; ++i) {
            dim_coords[dim][i] = down + static_cast<double>(i) * step;
        }
    }

    DGridResult result;
    result.points.reserve(total_points);

    std::vector<size_t> indices(n, 0);

    auto print_progress = [&](size_t done, std::chrono::steady_clock::time_point start) {
        if (!show_progress) return;
        if (total_points == 0) return;

        const double frac = static_cast<double>(done) / static_cast<double>(total_points);
        size_t filled = static_cast<size_t>(frac * static_cast<double>(progress_bar_width));
        if (filled > progress_bar_width) filled = progress_bar_width;

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = now - start;
        const double elapsed_sec = std::chrono::duration<double>(elapsed).count();

        double eta_sec = 0.0;
        if (done > 0 && elapsed_sec > 0.0) {
            const double rate = static_cast<double>(done) / elapsed_sec;
            if (rate > 0.0) {
                eta_sec = (static_cast<double>(total_points - done)) / rate;
            }
        }

        auto format_hms = [](double sec) {
            if (sec < 0.0) sec = 0.0;
            const long long s = static_cast<long long>(sec + 0.5);
            const long long hh = s / 3600;
            const long long mm = (s % 3600) / 60;
            const long long ss = s % 60;
            std::ostringstream os;
            os << std::setfill('0') << std::setw(2) << hh << ":"
               << std::setw(2) << mm << ":"
               << std::setw(2) << ss;
            return os.str();
        };

        std::cerr << "\r[";
        for (size_t i = 0; i < progress_bar_width; ++i) {
            std::cerr << (i < filled ? '#' : '-');
        }
        std::cerr << "] "
                  << std::setw(6) << std::fixed << std::setprecision(2) << (frac * 100.0) << "% "
                  << "(" << done << "/" << total_points << ") "
                  << "elapsed " << format_hms(elapsed_sec) << " "
                  << "eta " << format_hms(eta_sec)
                  << std::flush;
    };

    const auto start_time = std::chrono::steady_clock::now();
    auto last_print = start_time;

    if (show_progress) {
        print_progress(0, start_time);
    }

    for (size_t point_idx = 0; point_idx < total_points; ++point_idx) {
        AVector<double> point(n);
        for (size_t dim = 0; dim < n; ++dim) {
            point[dim] = dim_coords[dim][indices[dim]];
        }

        const double value = func(point);
        result.points.emplace_back(point, value);

        for (size_t dim = n; dim-- > 0;) {
            ++indices[dim];
            if (indices[dim] < points_per_dim) {
                break;
            }
            indices[dim] = 0;
        }

        if (show_progress) {
            const auto now = std::chrono::steady_clock::now();
            if (now - last_print >= progress_update_period || point_idx + 1 == total_points) {
                print_progress(point_idx + 1, start_time);
                last_print = now;
            }
        }
    }

    if (show_progress) {
        std::cerr << std::endl;
    }

    return result;
}
