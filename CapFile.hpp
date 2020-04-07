#pragma once

#include <map>
#include "hdr_t.hpp"
#include "muid.hpp"
#include "parser.hpp"

#define MILLION 1000000


namespace ink {
    using path = std::string;

    class CapFile {
        int fd;
        muid story_;
        std::string path_;
        std::map<uint64_t, std::ios::pos_type> index_;
        off_t index_offset_;
    public:
        CapFile(const muid &story, const path& directory, off_t index_offset);

        void receive(const std::string &, const TrxnRow&);

        uint64_t goes_to() const {
            VERIFY(not index_.empty());
            return (--index_.end())->first;
        }

        off_t get_index_offset() const noexcept {return index_offset_;}
    };


}