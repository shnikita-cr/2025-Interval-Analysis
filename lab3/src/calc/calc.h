#pragma once

#include "../secondary/avector/avector.h"
#include "../task/task.h"
#include "../secondary/tech/dgridresult.h"
#include "../names.h"

double tol(const AVector<double> &x, const DIAM &A, const DIAV &b);

DM rad(const DIAM &M);

DV rad(const DIAV &V);

DP tol_helper(const Task &task, const std::string &suffix_name = "", bool positiveTol = false);

bool check_is_empty_tolerance_set(const Task &task, const std::string &suffix_name = "");

void correct_b(const Task &task, const std::string &suffix_name = "", const DP &mx = {{},
                                                                                      {}});

void correct_a(const Task &task, const std::string &suffix_name = "", const DP &mx = {{},
                                                                                      {}});

void correct_ab(const Task &task, const std::string &suffix_name = "", const DP &mx1 = {{},
                                                                                        {}});

DGridResult check_grid(const Task &task);