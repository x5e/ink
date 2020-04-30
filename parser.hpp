#pragma once

#include "Id.hpp"
#include <memory>
#include "verify.hpp"
#include "misc.hpp"
#include "rows.hpp"

namespace ink {
    class parse_error : public std::runtime_error {
    public:
        parse_error(const char *fn, int line) : std::runtime_error(std::string(fn) + std::to_string(line)) {}
    };

    uint32_t parse_array_prefix(cstr_t &ptr);

    Muid parse_muid(cstr_t& ptr);

    int64_t parse_bigint(cstr_t& ptr);

    String parse_string(cstr_t& ptr);

    Fancy<Row> parse_row(cstr_t& ptr);

}