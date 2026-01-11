#include "task/task.h"
#include "task/taskFuncs.h"
#include "test/test.h"
#include "task/taskValues.h"

int main() {
    size_t iters = 5;

    testTask(
            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0, 1},
                                                  {0, 1}})}, x_c, y_c, iters);
//    testTask(
//            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0.5, 1.5},
//                                                  {0.5, 1.5}})}, x_c, y_c, iters);
//    testTask(
//            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0.7, 1.5},
//                                                  {0.7, 1.5}})}, x_c, y_c, iters);
//    testTask(
//            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0.40, 0.60},
//                                                  {0.60, 0.75}})}, x_c, y_c, iters);
    return 0;
}