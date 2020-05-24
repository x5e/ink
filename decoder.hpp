#pragma once

#include <memory>
#include "verify.hpp"
#include "misc.hpp"
#include "rows.hpp"
#define DECODE_REQUIRE(x) if (not (x)) throw ::ink::DecodeError(__FILE__, __LINE__)

namespace ink {
    class DecodeError : public std::runtime_error {
    public:
        DecodeError(const char *fn, int line) : std::runtime_error(std::string(fn) + std::to_string(line)) {}
    };

    uint32_t decode_array_prefix(cstr_t &ptr);

    int64_t decode_bigint(cstr_t& ptr);

    void decode_string(cstr_t& ptr, String);

}