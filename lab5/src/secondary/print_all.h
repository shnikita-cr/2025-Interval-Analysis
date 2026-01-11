#pragma once

#include <iostream>

template<typename... Args>
void printAll(Args... args) {
    (void) std::initializer_list<int>{((std::cout << args << " "), 0)...};
    std::cout << std::endl;
}

