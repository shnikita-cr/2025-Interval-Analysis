#pragma once

#include "../secondary/tech.h"
#include "../task/task.h"
#include "../tech.h"

bool check_is_empty_tolerance_set(const Task &task, const std::string &basicString) {
    return false;
}

void correct_b(const Task &task, const std::string &basicString) {

}

void correct_a(const Task &task, const std::string &basicString) {

}

void correct_ab(const Task &task, const std::string &basicString) {

}

void graph_tol(const Task &task, const std::string &basicString) {

}

void find_argmax_tol(const Task &task, const std::string &basicString) {

}

void find_gen_tol(const Task &task, const std::string &basicString) {

}



double tol(const DIAV &x, const DIAM &A, const DIAV &b) {
    if (!(A.getRows() == b.getDim() && A.getCols() == x.getDim())) {
        printAll("Wrong shape for tol(x,A,B)!");
        return -1;
    }
    size_t m = b.getDim();
    double min = 0;
    for (size_t i = 0; i < m; i++) {
        auto val = b[i].getRad() - std::abs(b[i].getMid() - (A[i], x).getMid());
        if (val < min) {
            min = val;
        }
    }
    return min;
}