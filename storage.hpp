#pragma once

#include <fstream>
#include <map>
#include <utility>
#include "hdr_t.hpp"
#include "misc.hpp"
#include "muid.hpp"
#include "verify.hpp"
#include "contents.hpp"

#define MILLION 1000000


namespace ink {
    using path = std::string;

    class CapFile {
        std::fstream handle_;
        muid story_;
        std::string path_;
        std::map<uint64_t, std::ios::pos_type> index_;
    public:
        CapFile(const muid &story, const path& location);

        void receive(const std::string &, const trxn_row&);
    };


    class FileSet {
        path directory_;
        std::map<muid, std::shared_ptr<CapFile>> cap_files;
    public:
        explicit FileSet(path loc): directory_(std::move(loc)) {}
        void receive(const std::string &msg);
    };
}