#pragma once

#include <map>
#include "hdr_t.hpp"
#include "muid.hpp"
#include "parser.hpp"

#define MILLION 1'000'000


namespace ink {
    using path_t = std::string;

    class CapFile {
        int fd;
        const path_t path_;
        std::map<uint64_t, std::ios::pos_type> index_; // timestamp => position in file
    public:
        explicit CapFile(path_t  file_path);

        void receive(const std::string &, const TrxnRow&);

        uint64_t goes_to() const {
            VERIFY(not index_.empty());
            return (--index_.end())->first;
        }
    };


}