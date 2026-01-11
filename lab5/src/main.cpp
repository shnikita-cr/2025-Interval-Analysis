#include "task/task.h"
#include "task/taskValues.h"
#include "test/test.h"


int main() {
    double y_c = 0;
    testTask(
            Task{f1, f2, DGrid({{0, 1},
                                {0, 1}})}, 0.0, y_c);
    testTask(
            Task{f1, f2, DGrid({{0, 1},
                                {0, 1}})}, 0.5, y_c);
    testTask(
            Task{f1, f2, DGrid({{-0.5, 0.5},
                                {-0.5, 0.5}})}, 1.0, y_c);
    testTask(
            Task{f1, f2, DGrid({{0.0, 0.8},
                                {0.0, 0.8}})}, 1.2, y_c);
    return 0;
}