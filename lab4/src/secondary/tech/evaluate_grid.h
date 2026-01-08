#pragma once

#include <chrono>
#include "dgridresult.h"
#include "dgrid.h"

DGridResult evaluate_grid(
        const DGrid &grid,
        const std::function<double(const AVector<double>&)> &func,
        size_t points_per_dim = 2,
        bool show_progress = true,
        size_t progress_bar_width = 30,
        std::chrono::milliseconds progress_update_period = std::chrono::milliseconds(250));