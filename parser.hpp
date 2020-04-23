#pragma once

#include "muid.hpp"
#include <memory>
#include "verify.hpp"

namespace ink {
    class parse_error : public std::runtime_error {
    public:
        parse_error(const char *fn, int line) : std::runtime_error(std::string(fn) + std::to_string(line)) {}
    };

    uint32_t parse_array_prefix(ccp_t &ptr);

    void parse_muid(ccp_t& ptr, muid& an_id);

    class Row {
    public:
        muid id_ = {};
    };

    class TrxnRow : public Row {
    public:
        muid story = {};
        muid acct = {};
        explicit TrxnRow(ccp_t &ptr);
    };

}