#pragma once

#include "../task/task.h"
#include "../calc/calc.h"
#include "../secondary/flogger.h"

void test_task(const Task &task) {
    flogger.log_framed("task", task.id, "start");
    flogger.level++;



    flogger.level--;
    flogger.log_framed("task", task.id, "end");
}

