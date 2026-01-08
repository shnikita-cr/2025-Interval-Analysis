cmake_minimum_required(VERSION 3.25)
project(lab5)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab5
        src/main.cpp
        src/main.h
        src/secondary/tech/print_all.h
        src/secondary/interval/interval.h
        src/task/task.h
        src/test/test.h
        src/secondary/vector_tech.h
        src/calc/calc.h
        "src/secondary/matrix creating.h"
        src/secondary/amatrix/amatrix.cpp
        "src/secondary/equations/equation solving.h"
        src/secondary/logger.h
        src/calc/calc.cpp src/secondary/tech/dgridresult.h src/secondary/tech/dgrid.h src/secondary/tech/point.h src/secondary/tech/evaluate_grid.h src/task/task_values.h src/secondary/flogger.h)