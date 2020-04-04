#pragma once
#include <stdexcept>
#include <string>
#include <cerrno>
#include <iostream>
// This checks for unexpected input.  UNLIKE NORMAL ASSERTIONS, THIS SHOULD REMAIN ACTIVE IN PRODUCTION
#define VERIFY(x) if (not (x)) throw ink::VerificationFailure(#x, __FILE__, __LINE__)


namespace ink {

    class VerificationFailure : public std::runtime_error {
    public:
        explicit VerificationFailure(const char* expr, const char* file, int line)
        : std::runtime_error(std::string(expr) + " in " + file + " at line " + std::to_string(line) ) {
            std::cerr << this->what() << std::endl;
        }
    };

}