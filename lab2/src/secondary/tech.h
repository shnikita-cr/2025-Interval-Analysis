#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

//main.h
using DVector = std::vector<double>;
const int TECH_PRECISION = 5, TECH_MAX_ELEMENT_WIDTH = TECH_PRECISION + 5;

//#define VECTOR_TO_FILE_PUTPUT_STYLE "fixed"
#define VECTOR_TO_FILE_PUTPUT_STYLE "scientific"

uint32_t vectorToFile(DVector &v, const std::string &file_name) {
    std::fstream out(file_name, std::ofstream::out | std::ofstream::trunc);
    if (out) {
        uint32_t counter = 0;
        if (VECTOR_TO_FILE_PUTPUT_STYLE == "fixed") {
            for (auto i: v) {
                //fout.width(MAX_ELEMENT_WIDTH);
                out << std::fixed << std::setprecision(TECH_PRECISION) << i << std::endl;
                counter++;
            }
        }
        if (VECTOR_TO_FILE_PUTPUT_STYLE == "scientific") {
            for (auto i: v) {
                //fout.width(MAX_ELEMENT_WIDTH);
                out << std::scientific << std::setprecision(TECH_PRECISION) << i << std::endl;
                counter++;
            }
        }
        std::cout << counter << " values written to from vector to file " << file_name << std::endl;
        return counter;
    }
    std::cerr << "error opening file " << file_name << std::endl;
    return -1;
}

void vectorDiff(const DVector &a, const DVector &b, DVector &c) {
    if (a.size() == b.size())
        for (uint32_t i = 0; i < a.size(); i++) {
            c[i] = a[i] - b[i];
        }
}

void vectorFillXValues(DVector &x_h, double a, double b, uint32_t n) {
    double h = (b - a) / (n - 1);
    for (uint32_t i = 0; i < n; i++) {
        x_h[i] = a + i * h;
    }
    std::cout<<n<<" values filled to vector"<<std::endl;
}

void vectorFillFValues(DVector &y_h, double a, double b, uint32_t n, double f(double)) {
    double step = (b - a) / (n - 1);
    for (uint32_t i = 0; i < n; i++) {
        y_h[i] = f(a + i * step);
    }
    std::cout<<n<<" values filled to vector"<<std::endl;
}

void vectorToDisplay(const DVector &v, const std::string &vector_name) {
    std::cout << "print vector " << vector_name << "\t";
    std::setfill(' ');
    std::cout << std::fixed << std::setprecision(TECH_PRECISION);
    for (auto i: v) {
        std::cout << std::setw(TECH_MAX_ELEMENT_WIDTH) << i << ' ';
    }
    std::cout << std::endl;
}


