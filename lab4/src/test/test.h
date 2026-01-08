#pragma once

#include "../task/task.h"
#include "../calc/calc.h"
#include "../secondary/flogger.h"

void saveInterval(const DI &i, const std::string &fileName) {
    std::ofstream of(fileName);
    of << i.getDown() << " " << i.getUp() << std::endl;
}

void savePoint(const DP &i, const std::string &fileName) {
    std::ofstream of(fileName);
    of << i << std::endl;
}

void testB1(const DIAV &X, const DIAV &Y, double eps, const DI &boundsAdd, const DI &boundsMul, size_t n = 201) {
    flogger.log_start("testB1");
    {
        //a
        flogger.log_start("testB1::a");

        DGridResult rA = runB1AddGrid(X, Y, boundsAdd, n);
        DP aMax = rA.getMax();
        DI aInterval = estimateIntervalByEps(rA, eps);

        savePoint(aMax, "../data/b1/add_max.txt");
        saveInterval(aInterval, "../data/b1/add_int.txt");

        rA.saveFile("../data/b1/add_val.txt");
        flogger.log_end("testB1::a");
    }
    {
        //t
        flogger.log_start("testB1::t");


        DGridResult rT = runB1MulGrid(X, Y, boundsMul, n);
        DP tMax = rT.getMax();
        DI tInterval = estimateIntervalByEps(rT, eps);

        savePoint(tMax, "../data/b1/mul_max.txt");
        saveInterval(tInterval, "../data/b1/mul_int.txt");

        rT.saveFile("../data/b1/mul_val.txt");
        flogger.log_end("testB1::t");
    }
    flogger.log_end("testB1");
}

void testB2(const DIAV &X, const DIAV &Y, double eps, const DI &boundsAdd, const DI &boundsMul, size_t n = 201) {
    flogger.log_start("testB2");
    {
        //a
        flogger.log_start("testB2::a");

        DGridResult rA = runB2AddGrid(X, Y, boundsAdd, n);
        DP aMax = rA.getMax();
        DI aInterval = estimateIntervalByEps(rA, eps);

        savePoint(aMax, "../data/b2/add_max.txt");
        saveInterval(aInterval, "../data/b2/add_int.txt");

        rA.saveFile("../data/b2/add_val.txt");
        flogger.log_end("testB2::a");
    }
    {
        //t
        flogger.log_start("testB2::t");

        DGridResult rT = runB2MulGrid(X, Y, boundsMul, n);
        DP tMax = rT.getMax();
        DI tInterval = estimateIntervalByEps(rT, eps);

        savePoint(tMax, "../data/b2/mul_max.txt");
        saveInterval(tInterval, "../data/b2/mul_int.txt");

        rT.saveFile("../data/b2/mul_val.txt");
        flogger.log_end("testB2::t");
    }
    flogger.log_end("testB2");
}