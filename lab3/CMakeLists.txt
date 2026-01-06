cmake_minimum_required(VERSION 3.25)
project(lab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab3
        src/main.cpp
        src/main.h
        src/tech.h
        src/interval/interval.h
        src/task/task.h
        src/test/test.h
        src/test/test_intervals.h
        src/secondary/tech.h
        src/calc/calc.h
        "src/secondary/matrix creating.h"
        src/secondary/amatrix/amatrix.cpp
        "src/equations/equation solving.h" src/logger.h)