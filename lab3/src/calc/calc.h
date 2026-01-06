#pragma once

#include "../secondary/avector/avector.h"
#include "../task/task.h"
#include "../secondary/tech/dgridresult.h"
#include "../names.h"

double tol(const AVector<double> &x, const DIAM &A, const DIAV &b);

void tol_helper(const Task &task, const std::string &suffix_name = "");

bool check_is_empty_tolerance_set(const Task &task, const std::string &suffix_name = "");

void correct_b(const Task &task, const std::string &suffix_name = "");

void correct_a(const Task &task, const std::string &suffix_name = "");

void correct_ab(const Task &task, const std::string &suffix_name = "");

void graph_tol(const Task &task, const std::string &suffix_name = "");

void graph_tol_task(const Task &task, const std::string &suffix_name = "");

void find_argmax_tol(const Task &task, const std::string &suffix_name = "");

void find_gen_tol(const Task &task, const std::string &suffix_name = "");

DGridResult check_grid(const Task &task);