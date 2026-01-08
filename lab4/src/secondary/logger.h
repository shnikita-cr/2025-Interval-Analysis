#pragma once

#include <ostream>
#include <iomanip>
#include <initializer_list>
#include <string>
#include <chrono>
#include <ctime>

class Logger {
private:
    std::ostream &output;
    int levelSize;

    static std::string nowStamp() {
        using namespace std::chrono;

        const auto now = system_clock::now();
        const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        const std::time_t tt = system_clock::to_time_t(now);
        std::tm tm{};

#if defined(_WIN32)
        localtime_s(&tm, &tt);
#else
        localtime_r(&tt, &tm);
#endif

        std::ostringstream ss;
        ss << std::setfill('0')
           << std::setw(4) << (tm.tm_year + 1900) << "."
           << std::setw(2) << (tm.tm_mon + 1) << "."
           << std::setw(2) << tm.tm_mday
           << "::"
           << std::setw(2) << tm.tm_hour << ":"
           << std::setw(2) << tm.tm_min << ":"
           << std::setw(2) << tm.tm_sec << ":"
           << std::setw(3) << ms.count();
        return ss.str();
    }

    void writePrefix() {
        output << nowStamp() << " ";
        output << std::string(levelSize * level, ' ');
    }

public:
    explicit Logger(std::ostream &output) : output(output), levelSize(4), level(0) {}

    int level;

    template<typename... Args>
    void log(Args... args) {
        writePrefix();
        (void) std::initializer_list<int>{((output << args << " "), 0)...};
        output << std::endl;
    }

    template<typename... Args>
    void log_framed(Args... args) {
        writePrefix();
        output << "========== ";
        output << std::left;
        (void) std::initializer_list<int>{((output << std::setw(18) << args << " "), 0)...};
        output << " ==========" << std::endl;
    }

    void log_start(const std::string &s) {
        log_framed(s + " start");
        level++;
    }

    void log_end(const std::string &s) {
        level--;
        log_framed(s + " end");
    }

    template<typename... Args>
    void debug(Args... args) {
        if (VERBOSE) {
            log("DEBUG",args...);
        }
    }
};
