#pragma once
#include <stdexcept>
#include <string>
#include <cerrno>
#include <iostream>
#include "typedefs.hpp"
#define REQUIRE(x) if (not (x)) return ink::barf(#x, __FILE__, __LINE__)
#define PROPAGATE(x) do {ink::error_t retval = (x); if (retval) {return retval;}} while (0)

namespace ink {
    error_t barf(const char* expr, const char* filename, int line) {
        static std::string x;
        x = std::string(expr) + " " + filename + " " + std::to_string(line);
        return x.c_str();
    }
}