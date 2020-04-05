#pragma once

#include <fstream>
#include <map>
#include "hdr_t.hpp"
#include "misc.hpp"
#include "muid.hpp"
#include "verify.hpp"

#define MILLION 1000000


namespace ink {

    class CapFile {
        std::fstream handle_;
        muid story_;
        std::string path_;
        std::map<uint64_t, std::ios::pos_type> index_;
    public:
        explicit CapFile(const muid &story);

        void append(const std::string &msg);
    };
}