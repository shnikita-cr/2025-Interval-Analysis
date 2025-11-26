#pragma once

#include "../task/task.h"
#include "../calc/calc.h"


void test_task(const Task &task) {
    //A
    graph_f(task);
    DI ran_value = ran(task.f, task.A, task.B, task.n);
    std::cout << "ran(f,X) = " << ran_value << std::endl;

    //C
    double L = find_l(task);
    std::cout << "L constant = " << L << std::endl;
    DI X = {task.A, task.B};


    //B
    //B1
    std::cout << "===================B1===============================" << std::endl;

    DI estimate_value_B1 = estimate_f_B1(task);
    std::cout << "estimate_value_B1 = " << estimate_value_B1 << std::endl;
    double estimate_value_B1_dist = dist(estimate_value_B1, ran_value);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B1_dist << std::endl;
    double left_B1 = estimate_value_B1.getRad();
    double right_B1 = L * X.getRad();
    std::cout << "left = " << left_B1 << "; right = " << right_B1 << std::endl;
    std::cout << "right - left = " << right_B1 - left_B1 << std::endl;
    std::cout << "left <= right " <<  (left_B1 <= right_B1) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B1.getWidth() << std::endl;

    //B2
    std::cout << "===================B2===============================" << std::endl;

    DI estimate_value_B2 = estimate_f_B2(task);
    std::cout << "estimate_value_B2 = " << estimate_value_B2 << std::endl;
    double estimate_value_B2_dist = dist(estimate_value_B2, ran_value);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B2_dist << std::endl;
    double left_B2 = estimate_value_B2.getRad();
    double right_B2 = L * X.getRad();
    std::cout << "left = " << left_B2 << "; right = " << right_B2 << std::endl;
    std::cout << "right - left = " << right_B2 - left_B2 << std::endl;
    std::cout << "left <= right " <<  (left_B2 <= right_B2) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B2.getWidth() << std::endl;

    //B3
    std::cout << "===================B3===============================" << std::endl;

    DI estimate_value_B3 = estimate_f_B3(task);
    std::cout << "estimate_value_B3 = " << estimate_value_B3 << std::endl;
    double estimate_value_B3_dist = dist(estimate_value_B3, ran_value);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B3_dist << std::endl;
    double left_B3 = estimate_value_B3.getRad();
    double right_B3 = L * X.getRad();
    std::cout << "left = " << left_B3 << "; right = " << right_B3 << std::endl;
    std::cout << "right - left = " << right_B3 - left_B3 << std::endl;
    std::cout << "left <= right " <<  (left_B3 <= right_B3) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B3.getWidth() << std::endl;

    //B4
    std::cout << "===================B4===============================" << std::endl;

    DI estimate_value_B4 = estimate_f_B4(task);
    std::cout << "estimate_value_B4 = " << estimate_value_B4 << std::endl;
    double estimate_value_B4_dist = dist(estimate_value_B4, ran_value);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B4_dist << std::endl;
    double left_B4 = estimate_value_B4.getRad();
    double right_B4 = L * X.getRad();
    std::cout << "left = " << left_B4 << "; right = " << right_B4 << std::endl;
    std::cout << "right - left = " << right_B4 - left_B4 << std::endl;
    std::cout << "left <= right " <<  (left_B4 <= right_B4) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B4.getWidth() << std::endl;

    std::cout << "===================B5===============================" << std::endl;

    //B5
    DI estimate_value_B5 = estimate_f_B5(task);
    std::cout << "estimate_value_B5 = " << estimate_value_B5 << std::endl;
    double estimate_value_B5_dist = dist(estimate_value_B5, ran_value);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B5_dist << std::endl;
    double left_B5 = estimate_value_B5.getRad();
    double right_B5 = L * X.getRad();
    std::cout << "left = " << left_B5 << "; right = " << right_B5 << std::endl;
    std::cout << "right - left = " << right_B5 - left_B5 << std::endl;
    std::cout << "left <= right " <<  (left_B5 <= right_B5) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B5.getWidth() << std::endl;

}