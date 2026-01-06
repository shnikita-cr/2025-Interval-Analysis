#include "interval/interval.h"
#include "test/test.h"
#include "task/task.h"

using DI = Interval<double>;


Task task1{A1, b1};
Task task2{A2, b2};
Task task3{A3, b3};

int main() {
//    check_grid(task1);
    test_task(task1, 1);
//    test_task(task2, 2);
//    test_task(task3, 3);
    return 0;
}
