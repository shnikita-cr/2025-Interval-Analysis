//
// Created by nick on 17.03.2024.
//

#ifndef NUMERICAL_METHODS_ALL_EQUATION_SOLVING_H
#define NUMERICAL_METHODS_ALL_EQUATION_SOLVING_H

#include <limits>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "../main.h"

#define TARGET_DATA "iter"
//#define TARGET_DATA "flaw"

double bisection_method(double f(double), double a, double b, double eps) {
    double c;
    int iter = 0;
    std::cout.setf(std::ios::fixed);
    if (VERBOSE) {
        std::cout << "bisection method" << std::endl;
        std::cout << "\tbegin" << std::endl;
    }
    while (2 * eps < fabs(b - a)) {
        c = (a + b) / 2;
        if (VERBOSE) {
            std::cout << "\t#" << iter << "\ta==" << a << "\tf(a)==" << f(a) << "\tb==" << b << "\tf(b)==" << f(b)
                      << "\tc=="
                      << c << std::endl;
            //cout << "\t\t" << setprecision(PRECISION) << iter << "\t" << abs(true_x - c) << endl;
        }
        if (f(a) * f(c) <= 0) {
            b = c;
        } else {
            a = c;
        }
        iter++;
    }
    if (VERBOSE) {
        std::cout << "\tend" << std::endl;
        std::cout << "iterations: " << iter << std::endl << std::endl;
    }
    double ans = (a + b) / 2;

    if (VERBOSE) {
        std::cout << "ans" << std::endl;
        std::cout << std::setprecision(PRECISION) << "\tx==" << ans << "\tf(" << ans << ")==" << f(ans) << "\teps=="
                  << eps
                  << std::endl;
        //cout << "\rows\ttrue x:" << true_x << endl;
        //cout << "\tflaw:" << setprecision(PRECISION) << true_x - ans << endl;

    }

    return ans;
}

double newtons_method(double f(double), double fs(double), double c, double m1, double M2, double eps, double true_x) {
    double x_k1 = c,
            x_k = 0,
            x_0 = c;
    int iter = 0;

    const double f_s = fs(x_0);

    std::cout.setf(std::ios::fixed);
    if (VERBOSE) {
        std::cout << "Newton's modified method" << std::endl;
        std::cout << "\tbegin" << std::endl;
    }
    //while (f(x_k1 - eps) * f(x_k1 + eps) >= 0) {
    while ((M2 / (2 * m1)) * (abs(x_k1 - x_k)) > eps) {
        x_k = x_k1;
        x_k1 = x_k - f(x_k) / f_s;

        if (VERBOSE) {
            std::cout << "\t#" << iter << "\tx_k: " << x_k << "\tf(x_k)==" << f(x_k) << std::endl;
            //cout << "\t\t" << setprecision(PRECISION) << iter << "\t" << abs(true_x - x_k) << endl;
        }
        iter++;
    }
    if (VERBOSE) {
        std::cout << "\tend" << std::endl;
        std::cout << "iterations: " << iter << std::endl << std::endl;
    }
    double ans = x_k1;
    if (VERBOSE) {
        std::cout << "ans" << std::endl;
        std::cout << std::setprecision(PRECISION) << "\tx==" << ans << "\tf(" << ans << ")==" << f(ans) << "\teps=="
                  << eps
                  << std::endl;
        //cout << "\rows\ttrue x:" << true_x << endl;
        //cout << "\tflaw:" << setprecision(PRECISION) << true_x - ans << endl;
    }
    if (TARGET_DATA == "flaw")
        return abs(ans - true_x);
    else
        return iter;
}

#endif //NUMERICAL_METHODS_ALL_EQUATION_SOLVING_H
