#pragma once

#include "muid.hpp"
#include <memory>
#include "verify.hpp"

namespace ink {
    class parse_error : public std::runtime_error {
    public:
        parse_error(const char *fn, int line) : std::runtime_error(std::string(fn) + std::to_string(line)) {}
    };

    using ccp = const char *;

    int parse_array_prefix(ccp &ptr);

    void parse_muid(ccp& ptr, muid& an_id);

    class row {
    public:
        muid id_ = {};
    };

    class trxn_row : public row {
    public:
        muid story = {};
        muid acct = {};
        explicit trxn_row(ccp &ptr);
    };

}