#pragma once

#include "../task/task.h"
#include "../calc/calc.h"
#include "../secondary/flogger.h"
#include "../secondary/tech/evaluate_grid.h"
#include "../secondary/print_all.h"

void saveInterval(const DI &i, const std::string &fileName) {
    std::ofstream of(fileName);
    of << i.getDown() << " " << i.getUp() << std::endl;
}

void savePoint(const DP &i, const std::string &fileName) {
    std::ofstream of(fileName);
    of << i << std::endl;
}

std::string format(double d, int precision) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << d;
    return ss.str();
}

static inline void saveTaskVisualizationData(
        const std::function<double(const DV &)> &f1,
        const std::function<double(const DV &)> &f2,
        const DGrid &grid,
        const std::string &dir,
        const std::vector<DIAV> &boxes,
        size_t pointsPerDim = 200) {
    flogger.log_start("saveTaskVisualizationData");
    flogger.log("dir =", dir, "pointsPerDim =", pointsPerDim, "boxes =", boxes.size());

    saveFunctionGrid2d(f1, grid, dir + "f1_grid.txt", pointsPerDim);
    saveFunctionGrid2d(f2, grid, dir + "f2_grid.txt", pointsPerDim);
    saveBoxes2d(boxes, dir + "boxes.txt");

    flogger.log_end("saveTaskVisualizationData");
}


void testTask(Task task, double x_c, double y_c) {
    flogger.log_start("test");
    flogger.log("x_c:", x_c, "y_c", y_c);

    std::string savePath("../data/" + format(x_c, 1) + "/");
    std::vector<DIAV> boxes{{{0, 1},   {0, 1}},
                            {{0, 0.5}, {0, 0.5}}};


    saveTaskVisualizationData(task.f1_d, task.f2_d, task.dGrid, savePath, boxes);

    flogger.log_end("test");
}