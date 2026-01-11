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

void testTask(Task task, double x_c, double y_c) {
    flogger.log_start("test");
    flogger.log("x_c:", x_c, "y_c", y_c);

    std::string savePath("../data/" + format(x_c, 1) + "/");
    printAll(savePath);

    flogger.log_end("test");
}