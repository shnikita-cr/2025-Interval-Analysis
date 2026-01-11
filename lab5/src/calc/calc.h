#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <functional>

#include "../names.h"
#include "../secondary/tech/dgridresult.h"
#include "../secondary/tech/dgrid.h"
#include "../secondary/tech/evaluate_grid.h"
#include "../secondary/flogger.h"

class LogScope {
private:
    std::string name;
public:
    explicit LogScope(const std::string &name) : name(name) {
        flogger.log_start(name);
    }

    ~LogScope() {
        flogger.log_end(name);
    }
};
