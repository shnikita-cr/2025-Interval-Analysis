#include "test/test.h"
#include "task/task.h"
#include "task/task_values.h"

Task task1{A1, b1, {{-10, 10}, {-10, 10}}, 1};
Task task2{A2, b2, {{-10, 10}, {-10, 10}}, 2};
Task task3{A3, b3, {{-10, 10}, {-10, 10}}, 3};

int main() {
    test_task(task1);
//    test_task(task2, 2);
//    test_task(task3, 3);
    return 0;
}
