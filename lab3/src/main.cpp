#include "interval/interval.h"
#include "test/test.h"
#include "task/task.h"

using DI = Interval<double>;

Task task{A1, b1};

int main() {
    test_task(task);
    return 0;
}
