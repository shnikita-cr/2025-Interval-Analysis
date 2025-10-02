cmake_minimum_required(VERSION 3.25)
project(lab1)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab1 main.cpp
        secondary/amatrix/amatrix.h
        secondary/avector/avector.h
        secondary/avector/avector.cpp
        secondary/amatrix/amatrix.cpp
        "secondary/matrix creating.h"
        secondary/tech.h interval/interval.h
        test_intervals.cpp
        tech.h
        main.h
        main.h "equations/equation solving.h")
