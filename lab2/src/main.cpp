#include "interval/interval.h"
#include "test/test_intervals.h"
#include "test/test.h"

using DI = Interval<double>;

Task task1{A1, B1, f1, DOT_COUNT};
Task task2{A2, B2, f2, DOT_COUNT};

int main() {
    test_task(task1);
//    test_task(task2);
//    test_intervals();
    return 0;
}
