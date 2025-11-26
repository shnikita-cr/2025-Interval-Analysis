#pragma once

#include <iostream>
#include "../interval/interval.h"
#include "../task/task.h"
#include "../secondary/tech.h"

void test_intervals() {
    DI a = {-2, -1}, b = {3, 4}, c = {3}, d = {-5, 1}, e = {-12, -8};
    DI f = {2, 3}, g = {1, 5}, h = {-1, 8};
    std::cout << a << std::endl;
    std::cout << b << std::endl;

    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
    std::cout << a * b << std::endl;
    std::cout << a / b << std::endl;

    std::cout << (-1) * b << std::endl;

    std::cout << c.in(b) << std::endl;
    std::cout << pow(d, 3) << std::endl;

    std::cout << e.abs() << std::endl;
    std::cout << e.hi() << std::endl;

    std::cout << "dist " << dist(f, g) << std::endl;
    std::cout << "dist " << dist(f, h) << std::endl;

    std::cout << (a + b) - b << std::endl;
    std::cout << (a * b) / b << std::endl;

}