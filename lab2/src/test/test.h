#pragma once

#include "../task/task.h"
#include "../calc/calc.h"


void test_task(const Task &task) {
    int dummy;

    std::cout << "========================A=====================================" << std::endl;
    //A
    graph_f(task);
    DI ran_value = ran(task.f, task.x.getDown(), task.x.getUp(), task.n);
    std::cout << "ran(f,X) = " << ran_value << std::endl;

    std::cout << "========================C=====================================" << std::endl;
    //C
    double L = find_l(task);
    std::cout << "L constant = " << L << std::endl;
    DI X = task.x;

    //B
    //B1
    std::cout << "========================B1====================================" << std::endl;

    DI estimate_value_B1 = estimate_f_B1(task);
    std::cout << "estimate_value_B1 = " << estimate_value_B1 << std::endl;
    double estimate_value_B1_dist = dist(estimate_value_B1, ran_value);
    std::cout << "estimate_value_B1_dist = " << estimate_value_B1_dist << std::endl;
    double left_B1 = estimate_value_B1.getRad();
    double right_B1 = L * X.getRad();
    std::cout << "rad(F(X)) = " << left_B1 << "; L*rad(X) = " << right_B1 << std::endl;
    std::cout << "L*rad(X) - rad(F(X)) = " << right_B1 - left_B1 << std::endl;
    std::cout << "rad(F(X)) <= L*rad(X) == 1? " << (left_B1 <= right_B1) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B1.getWidth()/2 << std::endl;

//    B2
    std::cin>>dummy;
    std::cout << "========================B2====================================" << std::endl;

    DI estimate_value_B2 = estimate_f_B2(task);
    std::cout << "estimate_value_B2 = " << estimate_value_B2 << std::endl;
    double estimate_value_B2_dist = dist(estimate_value_B2, ran_value);
    std::cout << "estimate_value_B2_dist = " << estimate_value_B2_dist << std::endl;
    double left_B2 = estimate_value_B2.getRad();
    double right_B2 = L * X.getRad();
    std::cout << "rad(F(X)) = " << left_B2 << "; L*rad(X) = " << right_B2 << std::endl;
    std::cout << "L*rad(X) - rad(F(X)) = " << right_B2 - left_B2 << std::endl;
    std::cout << "rad(F(X)) <= L*rad(X) == 1? " << (left_B2 <= right_B2) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B2.getWidth()/2 << std::endl;

    //B3
    std::cin>>dummy;
    std::cout << "========================B3====================================" << std::endl;

    DI estimate_value_B3 = estimate_f_B3(task);
    std::cout << "estimate_value_B3 = " << estimate_value_B3 << std::endl;
    double estimate_value_B3_dist = dist(estimate_value_B3, ran_value);
    std::cout << "estimate_value_B3_dist = " << estimate_value_B3_dist << std::endl;
    double left_B3 = estimate_value_B3.getRad();
    double right_B3 = L * X.getRad();
    std::cout << "rad(F(X)) = " << left_B3 << "; L*rad(X) = " << right_B3 << std::endl;
    std::cout << "L*rad(X) - rad(F(X)) = " << right_B3 - left_B3 << std::endl;
    std::cout << "rad(F(X)) <= L*rad(X) == 1? " << (left_B3 <= right_B3) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B3.getWidth()/2 << std::endl;


    //B4
    std::cin>>dummy;
    std::cout << "========================B4====================================" << std::endl;

    DI estimate_value_B4 = estimate_f_B4(task);
    std::cout << "estimate_value_B4 = " << estimate_value_B4 << std::endl;
    double estimate_value_B4_dist = dist(estimate_value_B4, ran_value);
    std::cout << "estimate_value_B4_dist = " << estimate_value_B4_dist << std::endl;
    double left_B4 = estimate_value_B4.getRad();
    double right_B4 = L * X.getRad();
    std::cout << "rad(F(X)) = " << left_B4 << "; L*rad(X) = " << right_B4 << std::endl;
    std::cout << "L*rad(X) - rad(F(X)) = " << right_B4 - left_B4 << std::endl;
    std::cout << "rad(F(X)) <= L*rad(X) == 1? " << (left_B4 <= right_B4) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B4.getWidth() / 2 << std::endl;

   //B5
    std::cin>>dummy;
    std::cout << "========================B5====================================" << std::endl;

    DI estimate_value_B5 = estimate_f_B5(task);
    std::cout << "estimate_value_B5 = " << estimate_value_B5 << std::endl;
    double estimate_value_B5_dist = dist(estimate_value_B5, ran_value);
    std::cout << "estimate_value_B5_dist = " << estimate_value_B5_dist << std::endl;
    double left_B5 = estimate_value_B5.getRad();
    double right_B5 = L * X.getRad();
    std::cout << "rad(F(X)) = " << left_B5 << "; L*rad(X) = " << right_B5 << std::endl;
    std::cout << "L*rad(X) - rad(F(X)) = " << right_B5 - left_B5 << std::endl;
    std::cout << "rad(F(X)) <= L*rad(X) == 1? " << (left_B5 <= right_B5) << std::endl;
    std::cout << "inaccuracy = " << estimate_value_B5.getWidth()/2 << std::endl;
}