#pragma once

#include <fstream>
#include <iomanip>

class Logger {
private:
    std::ostream &output;
    int levelSize;
public:
    explicit Logger(std::ostream &output) : output(output), levelSize(4), level(0) {}

    int level;

    template<typename... Args>
    void log(Args... args) {
        output << std::string(levelSize * level, ' ');
        (void) std::initializer_list<int>{((output << args << " "), 0)...};
        output << std::endl;
    }

    template<typename... Args>
    void log_framed(Args... args) {
        output << std::string(levelSize * level, ' ');
        output << "========== ";
        output << std::left;
        (void) std::initializer_list<int>{((output << std::setw(18) << args << " "), 0)...};
        output << " ==========" << std::endl;
    }
};
