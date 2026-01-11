#include "test/test.h"
#include "task/task.h"
#include "task/task_values.h"
#include "test/test_intervals.h"

size_t resolution = 200;

Task task0{A0, b0, {{0, 20}, {0, 20}}, 0, resolution};
Task task1{A1, b1, {{-10, 10}, {-10, 10}}, 1, resolution};
Task task2{A2, b2, {{-10, 10}, {-10, 10}}, 2, resolution};
Task task3{A3, b3, {{-10, 10}, {-10, 10}}, 3, resolution};

int main() {
//    test_im();
//    test_task(task0);
    test_task(task1);
    test_task(task2);
    test_task(task3);
    return 0;
}
