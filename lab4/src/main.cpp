#include "test/test.h"
#include "task/input.h"
#include "task/preprocess.h"

int main() {
    std::string name1 = "../input/-0.205_lvl_side_a_fast_data.bin",
            name2 = "../input/0.225_lvl_side_a_fast_data.bin";
    InputFile file1 = InputFile::read(name1);
    InputFile file2 = InputFile::read(name2);

    DV Xd{file1.flattenVolts()};
    DV Yd{file2.flattenVolts()};

    double N = 14;
    double rad = 1 / std::pow(2, N);
    DIAV X = puff(Xd, rad);
    DIAV Y = puff(Yd, rad);

    DI aRange(0.0, 1.0);
    DI tRange(-2.0, 0.0);
    size_t n = 151;
//    testB1(X, Y, 0.001, aRange, tRange, n);
//    testB2(X, Y, 0.001, aRange, tRange, 1000000);
//    testB3(X, Y, 0.001, aRange, tRange, 1000000);
    testB4(X, Y, 0.001, aRange, tRange, 1000000);
    return 0;
}