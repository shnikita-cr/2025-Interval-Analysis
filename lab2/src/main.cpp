#include "interval/interval.h"
#include "test/test.h"
#include "task/task.h"

using DI = Interval<double>;

Task task1{X1, f1, f1_i, f1_g, f1_p_tight, DOT_COUNT};
Task task2{X2, f2, f2_i, f2_g, f2_p, DOT_COUNT};

int main() {
    test_task(task1);
    // test_task(task2);
    // test_intervals();
    return 0;
}
