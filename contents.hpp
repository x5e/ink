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

    static int parse_array_prefix(ccp &ptr) {
        int count;
        if (((*ptr) & 0xF0) == 0x90) {
            count = (*ptr) & 0x0F;
            ptr += 1;
            return count;
        }
        if ((*ptr) == '\xdc' or *ptr == '\xdd') {
            throw std::runtime_error("not implemented");
        }
        throw std::runtime_error("unexpected");
    }

    static void parse_muid(ccp& ptr, muid& an_id) {
        char tag = *ptr++;
        if (tag == '\xc0') {
            an_id.zero();
            return;
        }
        if (tag != '\xd8')
            throw parse_error(__FILE__, __LINE__);
        char kind = *ptr++;
        if (kind != '\x01')
            throw parse_error(__FILE__, __LINE__);
        an_id.copy_from(ptr);
        ptr += 16;
    }

    class row {
    public:
        muid id_ = {};
    };

    class trxn_row : public row {
    public:
        muid story = {};
        muid acct = {};
        explicit trxn_row(ccp &ptr) {
            int count = parse_array_prefix(ptr);
            if (count < 3)
                throw parse_error(__FILE__, __LINE__);
            char row_tag = *ptr++;
            if (row_tag != '\x13')
                throw parse_error(__FILE__, __LINE__);
            parse_muid(ptr, id_);
            parse_muid(ptr, story);
            if (count >= 4) {
                parse_muid(ptr, acct);
            }
        }
    };

}