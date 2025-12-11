cmake_minimum_required(VERSION 3.25)
project(lab2)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab2
        main.cpp
        main.h
        tech.h
        interval/interval.h
        task/task.h
        test/test.h
        test/test_intervals.h secondary/tech.h calc/calc.h)