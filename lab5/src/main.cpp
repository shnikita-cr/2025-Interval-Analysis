#include "task/task.h"
#include "task/taskFuncs.h"
#include "test/test.h"
#include "task/taskValues.h"

int main() {
    testTask(
            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0, 1},
                                                  {0, 1}})}, x_c, y_c);
//    testTask(
//            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0, 1},
//                                                  {0, 1}})}, x_c, y_c);
//    testTask(
//            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{-0.5, 0.5},
//                                                  {-0.5, 0.5}})}, x_c, y_c);
//    testTask(
//            Task{f1_d, f1_di, f2_d, f2_di, DGrid({{0.0, 0.8},
//                                                  {0.0, 0.8}})}, x_c, y_c);
    return 0;
}