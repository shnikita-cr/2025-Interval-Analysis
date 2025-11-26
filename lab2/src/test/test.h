#pragma once

#include "../task/task.h"
#include "../calc/calc.h"


void test_task(const Task &task) {
    //A
    graph_f(task);
    DI ran = ran(task.f, task.A, task.B, task.n)
    std::cout << "ran(f,X) = " << ran << std::endl;

    //C
    double L = find_l(task);
    std::cout << "L constant = " << L << std::endl;
    DI X = {task.A, task.B};


    //B
    //B1
    DI estimate_value_B1 = estimate_f_B1(task);
    std::cout << "estimate_value_B1 = " << estimate_value_B1 << std::endl;
    double estimate_value_B1_dist = dist(estimate_value_B1, ran);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B1_dist << std::endl;
    double left_B1 = estimate_value_B1.getRad();
    double right_B1 = L * X.getRad();


    //B2
    DI estimate_value_B2 = estimate_f_B2(task);
    std::cout << "estimate_value_B2 = " << estimate_value_B2 << std::endl;
    double estimate_value_B2_dist = dist(estimate_value_B2, ran);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B2_dist << std::endl;
    double left_B2 = estimate_value_B2.getRad();
    double right_B2 = L * X.getRad();


    //B3
    DI estimate_value_B3 = estimate_f_B3(task);
    std::cout << "estimate_value_B3 = " << estimate_value_B3 << std::endl;
    double estimate_value_B3_dist = dist(estimate_value_B3, ran);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B3_dist << std::endl;
    double left_B3 = estimate_value_B3.getRad();
    double right_B3 = L * X.getRad();


    //B4
    DI estimate_value_B4 = estimate_f_B4(task);
    std::cout << "estimate_value_B4 = " << estimate_value_B4 << std::endl;
    double estimate_value_B4_dist = dist(estimate_value_B4, ran);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B4_dist << std::endl;
    double left_B4 = estimate_value_B4.getRad();
    double right_B4 = L * X.getRad();


    //B5
    DI estimate_value_B5 = estimate_f_B5(task);
    std::cout << "estimate_value_B5 = " << estimate_value_B5 << std::endl;
    double estimate_value_B5_dist = dist(estimate_value_B5, ran);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B5_dist << std::endl;
    double left_B5 = estimate_value_B5.getRad();
    double right_B5 = L * X.getRad();
}