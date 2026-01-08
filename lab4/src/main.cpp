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

    return 0;
}