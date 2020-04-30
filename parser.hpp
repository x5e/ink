#pragma once

#include "Id.hpp"
#include <memory>
#include "verify.hpp"
#include "misc.hpp"

namespace ink {
    class parse_error : public std::runtime_error {
    public:
        parse_error(const char *fn, int line) : std::runtime_error(std::string(fn) + std::to_string(line)) {}
    };

    uint32_t parse_array_prefix(cstr_t &ptr);

    Muid parse_muid(cstr_t& ptr);

    int64_t parse_bigint(cstr_t& ptr);

    std::string parse_string(cstr_t& ptr);




    std::shared_ptr<Row> parse_row(cstr_t& ptr) {
        int count = parse_array_prefix(ptr);
        VERIFY(count >= 2);
        char row_tag = *ptr++;
        std::shared_ptr<Row> out;
        switch (row_tag) {
            case '\x13':
                out = std::make_shared<TrxnRow>();
                break;
            default:
                throw parse_error(__FILE__, __LINE__);
        }
        out->parse(ptr, count - 1);
        return out;
    }

}