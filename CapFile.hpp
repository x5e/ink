#pragma once

#include <map>
#include "hdr_t.hpp"
#include "muid.hpp"
#include "parser.hpp"

#define MILLION 1'000'000


namespace ink {
    using path_t = std::string;

    class CapFile {
        const path_t path_;
        const int fd;
        off_t location;
        size_t max_packet_size = 0;
        std::map<muts_t, off_t> index_;
    public:
        explicit CapFile(path_t file_path);

        void receive(const char*, size_t, muts_t);

        muts_t goes_to() const {
            VERIFY(not index_.empty());
            return (--index_.end())->first;
        }
    };


}